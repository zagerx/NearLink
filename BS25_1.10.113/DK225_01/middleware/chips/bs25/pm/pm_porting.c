/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pm port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-13， Create file. \n
 */

#include "los_task_pri.h"
#include "chip_io.h"
#include "platform_core.h"
#include "pm_sleep.h"
#include "pm_veto.h"
#include "pm_sleep_porting.h"
#include "pm.h"

#define PMU1_CTL_RST_MAN_REG                    (PMU1_CTL_RB_BASE + 0xE4)
#define PM_RST_PWR_C1_CRG_N_FRC_ON              2
#define PM_RST_PWR_C1_LOGIC_N_FRC_ON            1
#define PM_RST_PWR_C1_CPU_N_FRC_ON              0
#define PM_RST_PWR_C1_FRC_ON_MASK               (BIT(PM_RST_PWR_C1_CRG_N_FRC_ON) | \
                                                BIT(PM_RST_PWR_C1_LOGIC_N_FRC_ON) | \
                                                BIT(PM_RST_PWR_C1_CPU_N_FRC_ON))

#define ULP_AON_CTL_VDD0P7_TO_ISO_EN_CFG_REG    (ULP_AON_CTL_RB_ADDR + 0x108)
#define ULP_AON_CTL_RST_BOOT_32K_N_CFG_REG      (ULP_AON_CTL_RB_ADDR + 0x110)

#define PMU1_CTL_RB_APERP_32K_SEL_MAN           (PMU1_CTL_RB_BASE + 0x1C0)
#define PMU1_CTL_RB_APERP_32K_SEL_SEL           (PMU1_CTL_RB_BASE + 0x1C4)

#if defined(PM_MCPU_MIPS_STATISTICS_ENABLE) && (PM_MCPU_MIPS_STATISTICS_ENABLE == YES)
static uint32_t g_pm_time_before_sleep = 0;
static uint32_t g_pm_time_after_sleep = 0;
static uint32_t g_pm_total_idle_time = 0;
static uint32_t g_pm_total_work_time = 0;
#endif
typedef struct reg_cfg {
    uint32_t reg_addr;
    uint16_t value;
} reg_cfg_t;

static const reg_cfg_t g_wakeup_wait_time_config[] = {
    // PMU1
    { PMU1_CTL_RB_BASE + 0x134, 1 },   // AON_SLP_RDY_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x138, 1 },   // RST_AON_CRG_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x13C, 1 },   // GLB_CLK_FORCE_ON_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x140, 1 },   // RST_AON_LGC_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x144, 1 },   // REF2_EN_BG_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x148, 1 },   // EN_REFBUFFER_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x14C, 1 },   // EN_INTLDO2_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x150, 1 },   // EN_IBG_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x154, 1 },   // EN_IPOLY_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x158, 1 },   // EN_ITUNE_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x15C, 2 },   // EN_XLDO_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x120, 2 },   // EN_CLKLDO1_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x160, 6 },   // XO_CORE_PD_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x164, 6 },   // FAST_XO_ISO_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x168, 7 },   // RC_PD_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x16C, 8 },   // RC_RSTN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x170, 7 },   // RC2_PU_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x174, 8 },   // RC2_RSTN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x178, 9 },   // FAST_XO_LOOP_RSTN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x17C, 36 },  // TCXO2ANA_PD_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x180, 36 },  // XO2DBB_PD_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x184, 37 },  // CLK_XO2DBB_GT_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x188, 37 },  // RC2_DBB_PD_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x18C, 37 },  // RC2_DBB_ISO_EN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x190, 38 },  // RC2_CLKEN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x194, 40 },  // B32M_CLKEN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x198, 40 },  // A32M_CLKEN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x19C, 40 },  // TCXO_EN_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x1A0, 40 },  // APERP_32K_SEL_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x1A4, 40 },  // RST_PWR_C1_CRG_N_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x1A8, 40 },  // RST_PWR_C1_LGC_N_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x1AC, 40 },  // RST_PWR_C1_CPU_N_WKUP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x1B0, 40 },  // M_SYS_WKING_COMPLETE_TIME
    { PMU1_CTL_RB_BASE + 0x1B4, 40 },  // B_SYS_WKING_COMPLETE_TIME

    // ULP_AON
    { ULP_AON_CTL_RB_ADDR + 0x180, 2 }, // VDD_1P9_SW_OFF_BOOT_TIME
    { ULP_AON_CTL_RB_ADDR + 0x184, 2 }, // SLEEP_N_BOOT_TIMR
    { ULP_AON_CTL_RB_ADDR + 0x188, 3 }, // EN_REF_BG1_BOOT_TIMR
    { ULP_AON_CTL_RB_ADDR + 0x18C, 3 }, // VDD_0P7_TO_SYS_ISO_EN_BOOT_TIME
    { ULP_AON_CTL_RB_ADDR + 0x190, 4 }, // RST_BOOT_32K_N_BOOT_TIME
    { ULP_AON_CTL_RB_ADDR + 0x194, 4 }, // ULP_WKUP_AON_BOOT_TIME
    { ULP_AON_CTL_RB_ADDR + 0x198, 1 }, // RST_BOOT_32K_N_WKUP_TIME
    { ULP_AON_CTL_RB_ADDR + 0x19C, 1 }, // ULP_WKUP_AON_WKUP_TIME
};

static const reg_cfg_t g_sleep_wait_time_config[] = {
    // ULP_AON
    { ULP_AON_CTL_RB_ADDR + 0x1A0, 3 }, // ULP_SLP_HLD_TIME
    { ULP_AON_CTL_RB_ADDR + 0x1A4, 3 }, // VDD_1P9_SW_OFF_SLP_TIME
    { ULP_AON_CTL_RB_ADDR + 0x1A8, 2 }, // SLEEP_N_SLP_TIMR
    { ULP_AON_CTL_RB_ADDR + 0x1AC, 1 }, // EN_REF_BG1_SLP_TIMR
    { ULP_AON_CTL_RB_ADDR + 0x1B0, 1 }, // VDD_0P7_TO_SYS_ISO_EN_SLP_TIME
    { ULP_AON_CTL_RB_ADDR + 0x1B4, 1 }, // RST_BOOT_32K_N_SLP_TIME
    { ULP_AON_CTL_RB_ADDR + 0x1B8, 1 }, // ULP_WKUP_AON_SLP_TIME

    // PMU1
    { PMU1_CTL_RB_BASE + 0x2A0, 3 },  // AON_SLP_RDY_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2A4, 3 },  // REF2_EN_BG_SLP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x2A8, 3 },  // EN_REFBUFFER_SLP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x2AC, 3 },  // EN_INTLDO2_SLP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x2B0, 3 },  // EN_IBG_SLP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x2B4, 3 },  // EN_IPOLY_SLP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x2B8, 3 },  // EN_ITUNE_SLP_TIME_SOC
    { PMU1_CTL_RB_BASE + 0x2C4, 3 },  // EN_CLKLDO1_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2C8, 3 },  // EN_XLDO_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2D0, 3 },  // XO_CORE_PD_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2D4, 3 },  // FAST_XO_ISO_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2D8, 3 },  // RC_PD_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2DC, 3 },  // RC_RSTN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2E0, 3 },  // RC2_PU_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2E4, 3 },  // RC2_RSTN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2E8, 3 },  // FAST_XO_LOOP_RSTN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2EC, 3 },  // TCXO2ANA_PD_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2F0, 3 },  // XO2DBB_PD_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2F4, 3 },  // CLK_XO2DBB_GT_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2F8, 3 },  // RC2_DBB_PD_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x2FC, 3 },  // RC2_DBB_ISO_EN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x300, 3 },  // RC2_CLKEN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x304, 3 },  // B32M_CLKEN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x308, 3 },  // A32M_CLKEN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x30C, 3 },  // TCXO_EN_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x310, 2 },  // APERP_32K_SEL_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x314, 1 },  // RST_PWR_C1_CRG_N_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x318, 1 },  // RST_PWR_C1_LGC_N_SLP_TIME
    { PMU1_CTL_RB_BASE + 0x31C, 1 },  // RST_PWR_C1_CPU_N_SLP_TIME
};

static inline void pm_keep_cldo_open(void)
{
    /* Turn on cldo during sleep. */
    writew(PMU1_CTL_RST_MAN_REG, PM_RST_PWR_C1_FRC_ON_MASK);
    writew(ULP_AON_CTL_VDD0P7_TO_ISO_EN_CFG_REG, 0);
    writew(ULP_AON_CTL_RST_BOOT_32K_N_CFG_REG, 0x1);
}

static void pm_wakeup_wait_time_config(void)
{
    for (uint32_t i = 0; i < sizeof(g_wakeup_wait_time_config) / sizeof(reg_cfg_t); i++) {
        writew(g_wakeup_wait_time_config[i].reg_addr, g_wakeup_wait_time_config[i].value);
    }
}

static void pm_sleep_wait_time_config(void)
{
    for (uint32_t i = 0; i < sizeof(g_sleep_wait_time_config) / sizeof(reg_cfg_t); i++) {
        writew(g_sleep_wait_time_config[i].reg_addr, g_sleep_wait_time_config[i].value);
    }
}

static void pm_auto_cg_config(void)
{
    writew(0x55000470, 0x0);    // CACHE_BUS_LP_CFG
    writew(0x55000480, 0x0);    // AON_BUS_LP_CFG
    writew(0x55000490, 0x0);    // B_BUS_LP_CFG
    writew(0x550004A0, 0x0);    // MCU_MAIN_AHB_LP_CFG
    writew(0x550004E0, 0x0);    // MCU_CFG_BUS_LP_CFG
    writew(0x550004F0, 0x0);    // MAIN_AXI_DEFAULT_SLAVE
    writew(0x550004F8, 0x0);    // MCU_PERP_AXI_DEFAULT_SLAVE
    writew(0x55000500, 0x0);    // EH2H_AUTO_CG_CFG
    writew(0x55000904, 0x0);    // MAIN_AXI_NEW_AUTO_CG
    writew(0x55000908, 0x0);    // MEM_SUB_AUTO_CG_CFG
    writew(0x55000980, 0x0);    // BSUB_AH2H_AUTO_CG_CFG
    writew(0x550009C0, 0x0);    // MCU_AUTO_CG_CFG
    writew(0x550009C4, 0x0);    // MCU_PERP_AXI_NEW_AUTO_CG
    writew(0x55000A80, 0x0);    // DMA_AUTO_CG_CFG
    writew(0x55000A9C, 0x0);    // SPI_AUTO_CG_CFG
    writew(0x55000AA0, 0x0);    // SPI_SSI_CLK_AUTO_CG_CFG
    writew(0x52000190, 0x0);    // DAP_H2P_AUTOCG_BYPASS
    writew(0x52000400, 0x0);    // COM_BUS_LP_CFG
    writew(0x52000F80, 0x0);    // COM_AUTO_CG_CFG
    writew(0x52000180, 0x0);    // M_AXI_BRG_LP_CFG0
    writew(0x52000184, 0x0);    // M_AXI_BRG_LP_CFG1
    writew(0x5200018C, 0x0);    // M_AXI_BRG_LP_CFG3
}

static inline void pm_aperp_32k_switch_to_auto_mode(void)
{
    // The peripheral 32k clock is switched to auto mode so that gpio can wake up the system.
    writew(PMU1_CTL_RB_APERP_32K_SEL_SEL, 0);
}

void uapi_pm_lpc_init(void)
{
    pm_sleep_funcs_t funcs = {
        .start_tickless            = pm_port_start_tickless,
        .stop_tickless             = pm_port_stop_tickless,
        .get_sleep_ms              = pm_port_get_sleep_ms,
        .start_wakeup_timer        = pm_port_start_wakeup_timer,
        .allow_deepsleep           = pm_port_allow_deepsleep,
        .lightsleep_config         = pm_port_lightsleep_config,
        .deepsleep_config          = pm_port_deepsleep_config,
        .light_wakeup_config       = pm_port_light_wakeup_config,
        .deep_wakeup_config        = pm_port_deep_wakeup_config,
        .enter_wfi                 = pm_port_enter_wfi,
    };
    uapi_pm_register_sleep_funcs(&funcs);

    uapi_pm_veto_init();
    pm_wakeup_rtc_init();
    pm_keep_cldo_open();

    pm_wakeup_wait_time_config();
    pm_sleep_wait_time_config();
    pm_auto_cg_config();
    pm_aperp_32k_switch_to_auto_mode();
    uapi_pm_add_sleep_veto(PM_VETO_MCU);
#if defined(PM_MCPU_MIPS_STATISTICS_ENABLE) && (PM_MCPU_MIPS_STATISTICS_ENABLE == YES)
    g_pm_time_after_sleep = uapi_systick_get_ms();
#endif
    // INTLDO2 hiz（解决S_AGPIO5睡眠时拉低问题）
    reg16_setbit(0x5700814C, 0x4);
}

#if defined(PM_MCPU_MIPS_STATISTICS_ENABLE) && (PM_MCPU_MIPS_STATISTICS_ENABLE == YES)
void pm_record_time_before_sleep(void)
{
    g_pm_time_before_sleep = (uint32_t)uapi_systick_get_ms();
    g_pm_total_work_time += (g_pm_time_before_sleep - g_pm_time_after_sleep);
}

void pm_record_time_after_sleep(void)
{
    g_pm_time_after_sleep = (uint32_t)uapi_systick_get_ms();
    g_pm_total_idle_time += (g_pm_time_after_sleep - g_pm_time_before_sleep);
}

uint32_t pm_get_time_before_sleep(void)
{
    return g_pm_time_before_sleep;
}

uint32_t pm_get_total_work_time(void)
{
    return g_pm_total_work_time;
}

uint32_t pm_get_time_after_sleep(void)
{
    return g_pm_time_after_sleep;
}

uint32_t pm_get_total_idle_time(void)
{
    return g_pm_total_idle_time;
}
#endif