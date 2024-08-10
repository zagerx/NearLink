/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides V152 HAL adc \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-30， Create file. \n
 */
#include "common_def.h"
#include "hal_clocks_application.h"
#include "soc_osal.h"
#include "adc_porting.h"
#include "hal_adc_v152.h"

hal_gafe_sample_info_t g_gafe_sample = GADC_DEFAULT_CONFIG;
hal_hafe_sample_info_t g_hafe_sample = HADC_DEFAULT_CONFIG;
static adc_irq_t g_adc_irq[AFE_SCAN_MODE_MAX_NUM] = { {GADC_DONE_IRQN, GADC_ALARM_IRQN},
                                                      {HADC_DONE_IRQN, HADC_ALARM_IRQN} };
static volatile int32_t g_sample_data[CONFIG_AFE_SAMPLE_TIMES] __attribute__((section(".afe_diag_mem")));
static bool g_hafe_lp_flag = false;

static errcode_t hal_adc_v152_init(void)
{
    if (hal_adc_v152_regs_init() != ERRCODE_SUCC) {
        return ERRCODE_ADC_REG_ADDR_INVALID;
    }
    hal_afe_mtcmos_en();
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_50);
    hal_afe_iso_release();
    hal_afe_ana_rstn_release();
    hal_afe_dig_apb_rstn_release();
    hal_afe_dig_clk_release();
    return ERRCODE_SUCC;
}

static errcode_t hal_adc_v152_deinit(void)
{
    return ERRCODE_SUCC;
}

static errcode_t hal_adc_v152_channel_set(adc_channel_t ch, bool on)
{
    if (on == false) {
        hal_gafe_channel_close();
    } else {
        g_gafe_sample.cfg_dreg_data0.b.channel_sel = (adc_v152_gadc_channel_sel_t)ch;
    }
    return ERRCODE_SUCC;
}

static void hal_adc_v152_power_on(void)
{
    hal_afe_power_on(&g_gafe_sample, &g_hafe_sample);
}

static void hal_adc_v152_diag_start(adc_channel_t channel)
{
    hal_clocks_mcpu_soft_reset_control(HAL_MCPU_SOFT_RST_IR, HAL_SOFT_RST_RESET);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_50);
    hal_clocks_mcpu_soft_reset_control(HAL_MCPU_SOFT_RST_IR, HAL_SOFT_RST_DERESET);
    hal_adc_v152_sample_start(channel, (uint32_t)(uintptr_t)g_sample_data,
                              (uint32_t)(sizeof(g_sample_data) / sizeof(uint32_t)));
    hal_cpu_trace_restart();
}

errcode_t hal_adc_v152_cali(afe_scan_mode_t afe_scan_mode, bool os_cali, bool cdac_cali, bool dcoc_cali)
{
    if (os_cali) {
        hal_adc_v152_os_cali(afe_scan_mode);
    }
    if (cdac_cali) {
        hal_adc_v152_cdac_cali(afe_scan_mode);
    }
    if (afe_scan_mode == AFE_GADC_MODE) {
        cfg_gadc_data0_t cfg_adc_data0;
        cfg_adc_data0.d32 = g_adc_regs->cfg_gadc_data_0;
        cfg_adc_data0.b.cont_mode = COUNT_MODE;
        g_adc_regs->cfg_gadc_data_0 = cfg_adc_data0.d32;
    }
    if ((afe_scan_mode == AFE_HADC_MODE) && (dcoc_cali == true)) {
        hal_adc_v152_dcoc_cali();
    }
    if (!adc_get_cali_code_source(afe_scan_mode)) {
        adc_channel_t channel = (afe_scan_mode == AFE_GADC_MODE ? GADC_CHANNEL_1 : HADC_CHANNEL_0);
        hal_adc_v152_diag_start(channel);
    }
    return ERRCODE_SUCC;
}

static void hal_adc_v152_enable(afe_scan_mode_t afe_scan_mode)
{
    hal_adc_adldo_pwron();
    if (afe_scan_mode == AFE_GADC_MODE) {
        hal_adc_gadc_enable();
    } else {
        hal_adc_hadc_enable();
    }
    if (adc_get_cali_code_source(afe_scan_mode)) {
        adc_channel_t channel = (afe_scan_mode == AFE_GADC_MODE ? GADC_CHANNEL_1 : HADC_CHANNEL_0);
        hal_adc_v152_diag_start(channel);
    }
}

static void hal_adc_v152_power_off(afe_scan_mode_t afe_scan_mode)
{
    if (afe_scan_mode == AFE_GADC_MODE) {
        hal_gafe_power_off();
    } else {
        hal_hafe_power_off();
    }
}

static void hal_adc_v152_power_en(afe_scan_mode_t afe_scan_mode, bool on)
{
    if (on) {
        hal_adc_v152_adldo_vset();
        hal_adc_v152_power_on();
        hal_adc_v152_enable(afe_scan_mode);
    } else {
        hal_adc_v152_power_off(afe_scan_mode);
    }
}

__attribute__((weak)) void hal_cpu_trace_restart(void)
{
}

static int32_t hal_adc_v152_manual(adc_channel_t channel)
{
    UNUSED(channel);
    return 0;
}

static void hal_hafe_low_power_cfg(bool on)
{
    if (on == false && g_hafe_lp_flag == false) { return; }
    afe_hadc_cfg_data_t hadc_cfg;
    hadc_cfg.d32 = hal_hadc_cfg_get();
    if (on) {
        hadc_cfg.b.s2d_hadc_iso_en = 1;
        hadc_cfg.b.s2d_hadc_en = 0;
        hal_hadc_cfg_set(hadc_cfg);
        uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_5);
        hadc_cfg.b.s2d_hadc_dcoc_dac_en = 0;
        hal_hadc_cfg_set(hadc_cfg);
        uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_5);
        hadc_cfg.b.s2d_hadc_pga_en = 0;
        hal_hadc_cfg_set(hadc_cfg);
        uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_5);
        hadc_cfg.b.s2d_hadc_ia_en_dly = 0;
        hadc_cfg.b.s2d_hadc_ia_en = 0;
        hal_hadc_cfg_set(hadc_cfg);
        uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_5);
        hadc_cfg.b.s2d_hadc_sensor_en = 0;
        hal_hadc_cfg_set(hadc_cfg);
        g_hafe_lp_flag = true;
    } else {
        hal_adc_hadc_enable();
        g_hafe_lp_flag = false;
    }
}

static void hal_hadc_sample_with_lpc(void)
{
    hal_afe_diag_set();
    hal_afe_diag_send2aix();
    hal_afe_diag_clk_enable();
    hal_afe_diag_source_sel();
    hal_hafe_low_power_cfg(false);
    hal_afe_diag_set_full_done_mode();
    hal_afe_diag_sample_len((uint32_t)(sizeof(g_sample_data) / sizeof(uint32_t)));
    hal_afe_diag_sample_start_addr((uint32_t)(uintptr_t)g_sample_data);
    hal_afe_diag_sample_end_addr((uint32_t)(uintptr_t)g_sample_data + \
                                 (((uint32_t)(sizeof(g_sample_data) / sizeof(uint32_t))) * sizeof(uint32_t)));
    hal_afe_diag_sample_sync();
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_150);
    hal_afe_diag_sample_en();
    while (!hal_afe_diag_get_sts()) {}
    hal_hafe_low_power_cfg(true);
}

static int32_t hal_adc_v152_sample(adc_channel_t channel)
{
    int32_t sample_value = 0;
    if (channel == HADC_CHANNEL_0) {
        hal_hadc_sample_with_lpc();
        int8_t count = 0;
        int32_t temp = 0;
        uint8_t index0 = 0, index1 = 0;
        for (uint32_t i = 0; i < CONFIG_AFE_SAMPLE_TIMES; i++) {
            temp = g_sample_data[i];
            if ((temp & BIT(HAFE_SAMPLE_VALUE_SIGN_BIT)) != 0) {
                temp = temp - HAFE_SAMPLE_MINUS_VALUE;
                count++;
                index0 = i;
            } else {
                count--;
                index1 = i;
            }
            sample_value += temp;
        }
        if (count == CONFIG_AFE_SAMPLE_TIMES || count == -CONFIG_AFE_SAMPLE_TIMES) {
            sample_value /= CONFIG_AFE_SAMPLE_TIMES;
        } else if (count == 1) {
            sample_value = g_sample_data[index0];
            sample_value = ((sample_value & BIT(HAFE_SAMPLE_VALUE_SIGN_BIT)) == 0) ?
                           sample_value : sample_value - HAFE_SAMPLE_MINUS_VALUE;
        } else {
            sample_value = g_sample_data[index1];
            sample_value = ((sample_value & BIT(HAFE_SAMPLE_VALUE_SIGN_BIT)) == 0) ?
                           sample_value : sample_value - HAFE_SAMPLE_MINUS_VALUE;
        }
        hal_cpu_trace_restart();
    } else {
        hal_gafe_iso_on(g_gafe_sample.cfg_dreg_data1);
        hal_gafe_channel_sel(g_gafe_sample.cfg_dreg_data0.b.channel_sel);
        while (!hal_gafe_single_sample_get_sts()) {}
        hal_gafe_iso_off();
        sample_value = hal_gafe_sample_symbol_judge(hal_gafe_single_sample_get_value());
    }
    return sample_value;
}

static hal_adc_funcs_t g_hal_adc_v152_funcs = {
    .init = hal_adc_v152_init,
    .deinit = hal_adc_v152_deinit,
    .ch_set = hal_adc_v152_channel_set,
    .power_en = hal_adc_v152_power_en,
    .manual = hal_adc_v152_manual,
    .auto_sample = hal_adc_v152_sample
};

void hal_adc_done_irq_handler(afe_scan_mode_t afe_scan_mode)
{
    osal_irq_clear(g_adc_irq[afe_scan_mode].done_irqn);
}

void hal_adc_alarm_irq_handler(afe_scan_mode_t afe_scan_mode)
{
    osal_irq_clear(g_adc_irq[afe_scan_mode].alarm_irqn);
}

hal_adc_funcs_t *hal_adc_v152_funcs_get(void)
{
    return &g_hal_adc_v152_funcs;
}
