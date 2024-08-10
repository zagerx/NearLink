/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  PMU DRIVER
 * Author: @CompanyNameTag
 * Create:  2018-10-15
 */

#include "core.h"
#include "non_os.h"
#include "pmu.h"
#if CORE == MASTER_BY_ALL
#ifndef BUILD_APPLICATION_SSB
#include "otp.h"
#endif
#include "pmu_cmu.h"
#include "pmu_ldo.h"
#include "ulp_aon_control.h"
#include "hal_clocks_application.h"
#include "hal_clocks_glb.h"
#include "clocks_core.h"
#else
#include "hal_clocks_security.h"
#endif
#include "pmu_ldo.h"
#include "pmu_interrupt.h"

#include "debug_print.h"
#include "tcxo.h"
#include "pinctrl_porting.h"
#include "hal_reg_config.h"
#include "panic.h"
#include "clock_calibration.h"
#if defined(BUILD_APPLICATION_STANDARD)
#include "log_common.h"
#include "log_def.h"
#endif
#if CHIP_ASIC
#include "hal_memory_apps.h"
#endif
#include "pm_pmu.h"

#define PMU_LPM_SYS_SLP_HISTORY_REG             (PMU1_CTL_RB_BASE + 0x264)
/* SYSTEM_HISTORY BIT position */
#define PMU_SYS_HISTORY_CLR  1
#define PMU_SYS_HISTORY      0


#define TCXO_DELAY_150_US   150ULL

#define PMU_REG_ACK_TIME 1000
void pmu_reg_ack_timeout(uint32_t addr, uint16_t offset, bool on)
{
    uint64_t time = uapi_tcxo_get_ms();
    bool tmp = (bool)reg16_getbit((void *)((uintptr_t)addr), POS(offset));

    while (tmp != on) {
        if ((time + PMU_REG_ACK_TIME) < uapi_tcxo_get_ms()) {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_ack is checking, addr:%x, offset:%x", addr, offset);
#else
            PRINT("Reg_ack is checking, addr:%x, offset:%x\r\n", addr, offset);
#endif
        } else {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_ack has temed out, addr:%x, offset:%x", addr, offset);
#else
            PRINT("Reg_ack has temed out, addr:%x, offset:%x\r\n", addr, offset);
#endif
            break;
        }
        tmp = (bool)reg16_getbit((void *)((uintptr_t)addr), POS(offset));
    }
}

void pmu_reg_sts_timeout(uint32_t addr, uint16_t value)
{
    uint64_t time = uapi_tcxo_get_ms();
    uint16_t tmp = readw(addr);

    while (tmp != value) {
        if ((time + PMU_REG_ACK_TIME) < uapi_tcxo_get_ms()) {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_status is checking, addr:%x, cur_value:%x", addr, tmp);
#else
            PRINT("Reg_status is checking, addr:%x, cur_value:%x\r\n", addr, tmp);
#endif
        } else {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_status has temed out, addr:%x, cur_value:%x", addr, tmp);
#else
            PRINT("Reg_status has temed out, addr:%x, cur_value:%x\r\n", addr, tmp);
#endif
            break;
        }
        tmp = readw(addr);
    }
}

#ifdef BUILD_APPLICATION_SSB
void pmu_init(void)
{
#if defined(BS25_DEBUG_ENABLE) && (BS25_DEBUG_ENABLE == YES)
    writew(0x5702C380, 0x6);   // PMU1_RESERVED_0
    writew(0x570086A4, 0xC00); // MCU_CORE_PLL_CFG6
    writew(0x5700869C, 0x118); // MCU_CORE_PLL_CFG5
    // phyldo:3.3v
    writew(0x5702C220, 0xF);
    writew(0x5702C224, 0x11);   // phyldo en
    writew(0x5702C24C, 0x1000); // ulp_clkldo en
    uapi_tcxo_delay_us(120);    // delay 120us
    writew(0x5702C224, 0x13);   // phyldo en_delay
    writew(0x5702C24C, 0x3000); // ulp_clkldo en_delay
    uapi_tcxo_delay_us(100);    // delay 100us
    // PMU 优化寄存器
    writew(0x5702C408, 0x400B);
    uapi_tcxo_delay_ms(0x5);
    writew(0x5702C408, 0x200B); // 进eco前，bit13:0 -> bit14:1; 退eco后：bit14:0 -> bit13:1，0x400B待载能力不够
    writew(0x5702C404, 0x1D0);  // 放汇编一份
    writew(0x5702C450, 0x80);
    return;
#endif
}
#else /* IF BUILD_APPLICATION_STANDARD */
static void pmu_bs25_ldo_power_on(void)
{
    // xo cfg & trim
    writew(0x5700819C, 0x1FBF); // XO_CFG_1
    writew(0x570081A4, 0x6);    // XO_CFG_3
    /* CBG发现的XO驱动能力不足调整成0xA可解决 */
    writew(0x57028308, 0x10);    // XO_CORE_TRIM_NOR
    writew(0x57028310, 0x10);    // XO_CORE_CTRIM_NOR
    writew(0x57008020, 0xAAAA); // XO_TRIM
    writew(0x5702C380, 0x406);  // ---TRIM---
    writew(0x5702C400, 0x4);    // 提高buck eco模式待载能力

    calibration_xo_core_ctrim_init();

    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_0P9_ECO, PMU_BUCK_0P9_VSET_0V7);
    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P1_ECO, PMU_BUCK_1P1_VSET_0V8);
    uapi_pmu_control(PMU_CONTROL_BUCK_0P9_CTRL, PMU_BUCK_SWITCH_TO_ECO_VSET_WHEN_SLEEPING);
    uapi_pmu_control(PMU_CONTROL_BUCK_1P1_CTRL, PMU_BUCK_SWITCH_TO_ECO_VSET_WHEN_SLEEPING);

    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P8, PMU_BUCK_1P8_VSET_1V8);
    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_0P9, PMU_BUCK_0P9_VSET_0V9);
    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P1, PMU_BUCK_1P1_VSET_1V0);
    uapi_tcxo_delay_us(120);  // delay 120us
    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P3, PMU_BUCK_1P3_VSET_1V18);
    uapi_pmu_control(PMU_CONTROL_BUCK_1P3_CTRL, PMU_BUCK_POWER_ON);

    uapi_pmu_control(PMU_CONTROL_TSENSOR_SWITCH_POWER, PMU_CONTROL_POWER_OFF);
#if defined(USE_EMBED_FLASH)
    uapi_pmu_control(PMU_CONTROL_SWITCH1_POWER, PMU_CONTROL_POWER_OFF);
#else
    writew(0x5702C24C, 0x0);    // 关闭ulp_clkldo
    writew(0x5702C224, 0x10);   // 关闭phyldo
#endif

    // Disable ulp_clkldo.
    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_ULP_CLKLDO, PMU_CLKLDO_VSET_1V1);

    // Close auxldo1.
    uapi_pmu_control(PMU_CONTROL_AUXLDO1_POWER, PMU_CONTROL_POWER_OFF);

    // clkldo1:1.1v
    writew(0x57008204, 0x0);
    uapi_tcxo_delay_us(120); // delay 120us
    // clkldo1:on
    writew(0x570040D8, 0x200);
    uapi_tcxo_delay_us(120); // delay 120us
    writew(0x570040D8, 0x600);
}

void pmu_init(void)
{
    pmu_bs25_ldo_power_on();
    pmu_init_interrupts();
    writew(0x5702C018, 0x0); // Control log print.
    return;
}
#endif /* BUILD_APPLICATION_STANDARD END */


bool pmu_system_sleep_history(void)
{
    return reg16_getbit(PMU_LPM_SYS_SLP_HISTORY_REG, POS(PMU_SYS_HISTORY));
}

void pmu_system_sleep_history_clear(void)
{
    reg16_setbit(PMU_LPM_SYS_SLP_HISTORY_REG, POS(PMU_SYS_HISTORY_CLR));
}
