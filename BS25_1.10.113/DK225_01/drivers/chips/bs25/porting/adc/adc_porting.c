/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides adc port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#include "td_base.h"
#include "chip_core_irq.h"
#include "soc_osal.h"
#include "pinctrl_porting.h"
#include "adc_porting.h"
#include "platform_core.h"
#include "hal_adc_v152.h"
#include "nv.h"
#include "pm_pmu.h"
#include "pm_clock.h"
#include "chip_io.h"
#include "osal_interrupt.h"

#define ADC_BASE_ADDR 0x57036000
#define ADC_PMU_BASE_ADDR 0x57008700
#define ADC_AON_OFFSET 0x220
#define MCU_DIAG_CTL_BASE_ADDR 0x52004000
#define ADC_DIAG_OFFSET 0x100
#define ADC_NV_KEY_ID 0x1

afe_scan_mode_t g_adc_working_afe = AFE_SCAN_MODE_MAX_NUM;
afe_config_t g_afe_nv_config = { 0 };
static adc_irq_t g_adc_irq[AFE_SCAN_MODE_MAX_NUM] = { {GADC_DONE_IRQN, GADC_ALARM_IRQN},
                                                      {HADC_DONE_IRQN, HADC_ALARM_IRQN} };
static bool g_use_record_cali[AFE_SCAN_MODE_MAX_NUM] = {false, false};

uintptr_t adc_porting_base_addr_get(void)
{
    return (uintptr_t)ADC_BASE_ADDR;
}

uintptr_t adc_pmu_base_addr_get(void)
{
    return (uintptr_t)ADC_PMU_BASE_ADDR;
}

uintptr_t adc_aon_cfg_addr_get(void)
{
    return (uintptr_t)(ULP_AON_CTL_RB_ADDR + ADC_AON_OFFSET);
}

uintptr_t adc_diag_cfg_addr_get(void)
{
    return (uintptr_t)(MCU_DIAG_CTL_BASE_ADDR + ADC_DIAG_OFFSET);
}

void adc_port_register_hal_funcs(void)
{
    hal_adc_register_funcs(hal_adc_v152_funcs_get());
}

void adc_port_unregister_hal_funcs(void)
{
    hal_adc_unregister_funcs();
}

void adc_port_init_clock(adc_clock_t clock)
{
    uapi_unused(clock);
    uapi_clock_control(CLOCK_CONTROL_XO_CLKOUT_ENABLE, CLOCK_D_CLKN2AFE_EN);
    uapi_clock_control(CLOCK_CONTROL_XO_CLKOUT_ENABLE, CLOCK_D_CLKP2AFE_EN);
}

void adc_port_clock_enable(bool on)
{
    if (on) {
        uapi_clock_control(CLOCK_CONTROL_XO_CLKOUT_ENABLE, CLOCK_D_CLKN2AFE_EN);
        uapi_clock_control(CLOCK_CONTROL_XO_CLKOUT_ENABLE, CLOCK_D_CLKP2AFE_EN);
    } else {
        uapi_clock_control(CLOCK_CONTROL_XO_CLKOUT_DISABLE, CLOCK_D_CLKN2AFE_EN);
        uapi_clock_control(CLOCK_CONTROL_XO_CLKOUT_DISABLE, CLOCK_D_CLKP2AFE_EN);
    }
}

static void irq_adc_done_handler(void)
{
    hal_adc_done_irq_handler(g_adc_working_afe);
}

static void irq_adc_alarm_handler(void)
{
    hal_adc_alarm_irq_handler(g_adc_working_afe);
}

void adc_port_register_irq(afe_scan_mode_t afe_scan_mode)
{
    if (afe_scan_mode == AFE_SCAN_MODE_MAX_NUM) { return; }
    osal_irq_request(g_adc_irq[afe_scan_mode].done_irqn, (osal_irq_handler)irq_adc_done_handler, NULL, NULL, NULL);
    osal_irq_request(g_adc_irq[afe_scan_mode].alarm_irqn, (osal_irq_handler)irq_adc_alarm_handler, NULL, NULL, NULL);
    osal_irq_enable(g_adc_irq[afe_scan_mode].done_irqn);
    osal_irq_enable(g_adc_irq[afe_scan_mode].alarm_irqn);
    g_adc_working_afe = afe_scan_mode;
}

void adc_port_unregister_irq(afe_scan_mode_t afe_scan_mode)
{
    if (afe_scan_mode == AFE_SCAN_MODE_MAX_NUM) { return; }
    osal_irq_disable(g_adc_irq[afe_scan_mode].done_irqn);
    osal_irq_disable(g_adc_irq[afe_scan_mode].alarm_irqn);
    osal_irq_free(g_adc_irq[afe_scan_mode].done_irqn, NULL);
    osal_irq_free(g_adc_irq[afe_scan_mode].alarm_irqn, NULL);
    g_adc_working_afe = AFE_SCAN_MODE_MAX_NUM;
}

void adc_port_power_on(bool on)
{
    if (on) {
        uapi_pmu_control(PMU_CONTROL_ADLDO1_POWER, PMU_CONTROL_POWER_ON);
        uapi_pmu_control(PMU_CONTROL_ADLDO2_POWER, PMU_CONTROL_POWER_ON);
    } else {
        uapi_pmu_control(PMU_CONTROL_ADLDO1_POWER, PMU_CONTROL_POWER_OFF);
        uapi_pmu_control(PMU_CONTROL_ADLDO2_POWER, PMU_CONTROL_POWER_OFF);
    }
}

static errcode_t adc_read_nv_cfg(afe_config_t *afe_config)
{
    if (afe_config == NULL) {
        return ERRCODE_NV_INVALID_PARAMS;
    }
    uint16_t key_len;
    if (uapi_nv_read(ADC_NV_KEY_ID, sizeof(afe_config_t), &key_len, (uint8_t *)afe_config) != ERRCODE_SUCC ||
        key_len != sizeof(afe_config_t)) {
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

errcode_t adc_write_nv_cfg(afe_config_t *afe_config)
{
    if (afe_config == NULL) {
        return ERRCODE_NV_INVALID_PARAMS;
    }
    if (uapi_nv_write(ADC_NV_KEY_ID, (uint8_t *)afe_config, sizeof(afe_config_t)) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

afe_config_t *adc_nv_config_get(void)
{
    if (adc_read_nv_cfg(&g_afe_nv_config) != ERRCODE_SUCC) {
        return NULL;
    }
    return &g_afe_nv_config;
}

errcode_t adc_set_cali_code(afe_scan_mode_t afe_scan_mode, afe_config_t *afe_config)
{
    if (afe_scan_mode >= AFE_SCAN_MODE_MAX_NUM) {
        return ERRCODE_INVALID_PARAM;
    }
    if (afe_scan_mode == AFE_GADC_MODE) {
        hal_gafe_os_cali_set_code(afe_config->gadc_os_code);
        hal_gafe_cdac_cali_set_code32(afe_config->gadc_wt_b14);
        for (uint8_t i = 0; i < GADC_WCAL_DATA_BYTES8; i++) {
            hal_gafe_cdac_cali_set_code16(i, *((&(afe_config->gadc_wt_b13)) + i));
        }
        hal_gafe_cdac_cali_sync();
        g_use_record_cali[AFE_GADC_MODE] = true;
    } else {
        hal_hafe_os_cali_set_code(afe_config->hadc_os_code);
        for (uint8_t i = 0; i < HADC_WCAL_DATA_BYTES32; i++) {
            hal_hafe_cdac_cali_set_code32(i, *((&(afe_config->hadc_wt_b22)) + i));
        }
        for (uint8_t j = 0; j < HADC_WCAL_DATA_BYTES - HADC_WCAL_DATA_BYTES32; j++) {
            hal_hafe_cdac_cali_set_code16(j, *((&(afe_config->hadc_wt_b18)) + j));
        }
        hal_hafe_cdac_cali_sync();
        hal_hafe_dcoc_cali_set_code_cfg(afe_config->dac_code2);
        g_use_record_cali[AFE_HADC_MODE] = true;
    }
    return ERRCODE_SUCC;
}

bool adc_get_cali_code_source(afe_scan_mode_t afe_scan_mode)
{
    return g_use_record_cali[afe_scan_mode];
}

errcode_t adc_calibration(afe_scan_mode_t afe_scan_mode, bool os_cali, bool cdac_cali, bool dcoc_cali)
{
    errcode_t ret = hal_adc_v152_cali(afe_scan_mode, os_cali, cdac_cali, dcoc_cali);
    g_use_record_cali[afe_scan_mode] = false;
    return ret;
}

void hadc_gain_set(uint8_t pga_gain, uint8_t ia_gain)
{
    cfg_hadc_ctrl2_t cfg_hadc_ctrl2 = { g_adc_regs->cfg_hadc_ctrl_2 };
    cfg_hadc_ctrl2.b.cfg_hadc_pga_gain = (adc_v152_pga_gain_t)pga_gain;
    cfg_hadc_ctrl2.b.cfg_hadc_ia_gain = (adc_v152_ia_gain_t)ia_gain;
    g_adc_regs->cfg_hadc_ctrl_2 = cfg_hadc_ctrl2.d32;
}

void hal_cpu_trace_restart(void)
{
    uint32_t temp = osal_irq_lock();
    writew(0x52000054, 0xf2f);
    writew(0x52000054, 0xf3f);
    reg16_clrbit(0x52004204, 0x9);
    reg16_clrbit(0x52004100, 0x0);
    writew(0x5200410c, 0x1FF);
    reg16_setbit(0x52004204, 0x0);

    writew(0x52004210, 0x6000);
    writew(0x52004214, 0x5200);
    writew(0x52004218, 0x7ff8);
    writew(0x5200421c, 0x5200);
    writew(0x52004208, 0x7FE);
    writew(0x5200420c, 0x0);
    writew(0x52004104, 0x0);
    reg16_setbit(0x52004108, 0x1);

    writew(0x52004204, 0x1);
    writew(0x52004204, 0x201);
    writew(0x52004204, 0x301);
    writew(0x52004108, 0x3);
    osal_irq_restore(temp);
}