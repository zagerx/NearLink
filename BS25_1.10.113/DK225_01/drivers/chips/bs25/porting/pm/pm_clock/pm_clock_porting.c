/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pm clock port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-12， Create file. \n
 */

#include "osal_interrupt.h"
#include "log_common.h"
#include "log_def.h"
#include "pm_clock.h"
#include "pm_pmu.h"
#include "clocks_switch.h"
#include "tcxo.h"
#include "pm_clock_porting.h"

#define M_CTL_M_CLKEN0_REG                  (M_CTL_RB_BASE + 0x40)
#define M_CTL_M_CLKEN1_REG                  (M_CTL_RB_BASE + 0x44)
#define PMU2_CMU_CTL_XO_RESERVE_0_REG       (PMU2_CMU_CTL_RB_BASE + 0x6C0)
#define M_CTL_GLB_CLKEN_REG                 (M_CTL_RB_BASE + 0x548)
#define PM_B_CTL_B_CRG_CLKSEL_REG           (B_CTL_RB_BASE + 0x804)
#define PM_B_32M_2X_CLK_STS_BIT             0
// Flag: 1P1电压档位。
#define PM_GLB_CTL_B_GLB_GP_B_REG0          (GLB_CTL_B_RB_BASE + 0x10)
// Flag: A核正在配置时钟频率。
#define PM_GLB_CTL_B_GLB_GP_B_REG1          (GLB_CTL_B_RB_BASE + 0x14)
// Flag: BT核正在配置时钟频率。
#define PM_GLB_CTL_B_GLB_GP_B_REG2          (GLB_CTL_B_RB_BASE + 0x18)
// Flag: 固定BT核频率标志寄存器
#define PM_GLB_CTL_B_GLB_GP_B_REG3          (GLB_CTL_B_RB_BASE + 0x1C)
#define BCPU_RST_REG                        (0x570000B8)
#define BCPU_RST_VALUE                      0x7
#define BUCK_RAISE_DELAY                    100

typedef bool (*clock_control_func_t)(uint8_t param);

#define pm_setbit(src, pos)   ((src) |= ((uint16_t)((1U) << (uint8_t)(pos))))
#define pm_clrbit(src, pos)   ((src) &= ~((uint16_t)((1U) << (uint8_t)(pos))))

static uint16_t g_pm_0p9_1v0_vote = 0;
static uint16_t g_pm_0p9_1v1_vote = 0;
static uint16_t g_pm_1p1_1v0_vote = 0;
static uint16_t g_pm_1p1_1v1_vote = 0;
static clock_vset_level_t g_pm_current_0p9_vset = CLOCK_VSET_LEVEL_0V9;
static clock_vset_level_t g_pm_current_1p1_vset = CLOCK_VSET_LEVEL_0V9;

static uint16_t g_pm_mcpu_dll2_freq_used = 0;
static uint16_t g_pm_mcpu_ulppll_freq_used = 0;
#define PM_MCPU_FREQ_USED_BY_MCU    0
#define PM_MCPU_FREQ_USED_BY_BTH    1

static bool clock_is_bcpu_clk_tcxo_2x(void)
{
    if (readw(BCPU_RST_REG) == BCPU_RST_VALUE) {
        if ((readw(PM_B_CTL_B_CRG_CLKSEL_REG) & BIT(PM_B_32M_2X_CLK_STS_BIT)) != 0) {
            return true;
        }
    }
    return false;
}

static clock_vset_level_t pm_pm_get_vset_by_clk(clock_crg_id_t id, clock_clk_src_t clk_src, uint8_t clk_div)
{
    if (clk_src >= CLOCK_CLK_SRC_MAX) {
        return CLOCK_VSET_LEVEL_0V9;
    }

    if ((id == CLOCK_CRG_ID_MCU_PERP_LS) || (id == CLOCK_CRG_ID_MCU_PERP_UART) ||
        (id == CLOCK_CRG_ID_MCU_PERP_SPI)) {
        if (clk_src == CLOCK_CLK_SRC_TCXO) {
            if (clk_div <= 0x1) {   // 32M
                return CLOCK_VSET_LEVEL_1V0;
            } else {
                return CLOCK_VSET_LEVEL_0V9;
            }
        }
        return CLOCK_VSET_LEVEL_1V1;
    }

    if (clk_src == CLOCK_CLK_SRC_TCXO) {
        if (id == CLOCK_CRG_ID_BT_CPU) {
            return CLOCK_VSET_LEVEL_1V0;
        }
        return CLOCK_VSET_LEVEL_0V9;
    } else if (clk_src == CLOCK_CLK_SRC_TCXO_2X) {
        if (id == CLOCK_CRG_ID_BT_CPU) {
            return CLOCK_VSET_LEVEL_1V1;
        }
        if (clk_div <= 0x1) { // 0x1: 1分频
            return CLOCK_VSET_LEVEL_1V0;
        } else {
            return CLOCK_VSET_LEVEL_0V9;
        }
    }

    if (id == CLOCK_CRG_ID_USB_PHY_BUS) {
        return CLOCK_VSET_LEVEL_1V1;
    } else if (id == CLOCK_CRG_ID_PDM) {
        return CLOCK_VSET_LEVEL_1V0;
    } else if (clk_div <= 0x1) {    // 0x1: 1分频
        return CLOCK_VSET_LEVEL_1V1;
    } else {
        return CLOCK_VSET_LEVEL_1V0;
    }
}

static void pm_buck_vset_1v1_vote(uint8_t id)
{
    switch (id) {
        case CLOCK_CRG_ID_MCU_CORE:
            pm_setbit(g_pm_0p9_1v1_vote, id);
            pm_setbit(g_pm_1p1_1v1_vote, id);
            pm_clrbit(g_pm_0p9_1v0_vote, id);
            pm_clrbit(g_pm_1p1_1v0_vote, id);
            break;

        case CLOCK_CRG_ID_MCU_PERP_LS:
        case CLOCK_CRG_ID_MCU_PERP_UART:
        case CLOCK_CRG_ID_MCU_PERP_SPI:
        case CLOCK_CRG_ID_PDM:
            pm_setbit(g_pm_0p9_1v1_vote, id);
            pm_clrbit(g_pm_0p9_1v0_vote, id);
            break;

        default:
            pm_setbit(g_pm_1p1_1v1_vote, id);
            pm_clrbit(g_pm_1p1_1v0_vote, id);
            break;
    }
}

static void pm_buck_vset_1v0_vote(uint8_t id)
{
    switch (id) {
        case CLOCK_CRG_ID_MCU_CORE:
            pm_setbit(g_pm_0p9_1v0_vote, id);
            pm_setbit(g_pm_1p1_1v0_vote, id);
            pm_clrbit(g_pm_0p9_1v1_vote, id);
            pm_clrbit(g_pm_1p1_1v1_vote, id);
            break;

        case CLOCK_CRG_ID_MCU_PERP_LS:
        case CLOCK_CRG_ID_MCU_PERP_UART:
        case CLOCK_CRG_ID_MCU_PERP_SPI:
        case CLOCK_CRG_ID_PDM:
            pm_setbit(g_pm_0p9_1v0_vote, id);
            pm_clrbit(g_pm_0p9_1v1_vote, id);
            break;

        default:
            pm_setbit(g_pm_1p1_1v0_vote, id);
            pm_clrbit(g_pm_1p1_1v1_vote, id);
            break;
    }
}

static void pm_buck_vset_0v9_vote(uint8_t id)
{
    switch (id) {
        case CLOCK_CRG_ID_MCU_CORE:
            pm_clrbit(g_pm_0p9_1v0_vote, id);
            pm_clrbit(g_pm_1p1_1v0_vote, id);
            pm_clrbit(g_pm_0p9_1v1_vote, id);
            pm_clrbit(g_pm_1p1_1v1_vote, id);
            break;

        case CLOCK_CRG_ID_MCU_PERP_LS:
        case CLOCK_CRG_ID_MCU_PERP_UART:
        case CLOCK_CRG_ID_MCU_PERP_SPI:
        case CLOCK_CRG_ID_PDM:
            pm_clrbit(g_pm_0p9_1v0_vote, id);
            pm_clrbit(g_pm_0p9_1v1_vote, id);
            break;

        default:
            pm_clrbit(g_pm_1p1_1v0_vote, id);
            pm_clrbit(g_pm_1p1_1v1_vote, id);
            break;
    }
}

static void pm_buck_vset_vote(uint8_t id, clock_vset_level_t level)
{
    if (level == CLOCK_VSET_LEVEL_1V1) {
        pm_buck_vset_1v1_vote(id);
    } else if (level == CLOCK_VSET_LEVEL_1V0) {
        pm_buck_vset_1v0_vote(id);
    } else {
        pm_buck_vset_0v9_vote(id);
    }

    pm_clrbit(g_pm_1p1_1v1_vote, CLOCK_CRG_ID_BT_CPU);
    pm_clrbit(g_pm_1p1_1v0_vote, CLOCK_CRG_ID_BT_CPU);

    // 标志A核所需1P1档位
    if (g_pm_1p1_1v1_vote != 0) {
        writew(PM_GLB_CTL_B_GLB_GP_B_REG0, CLOCK_VSET_LEVEL_1V1);
    } else if (g_pm_1p1_1v0_vote != 0) {
        writew(PM_GLB_CTL_B_GLB_GP_B_REG0, CLOCK_VSET_LEVEL_1V0);
    } else {
        writew(PM_GLB_CTL_B_GLB_GP_B_REG0, CLOCK_VSET_LEVEL_0V9);
    }

    // 1P1档位受BT频率控制
    if (clock_is_bcpu_clk_tcxo_2x()) {
        pm_setbit(g_pm_1p1_1v1_vote, CLOCK_CRG_ID_BT_CPU);
    } else {
        pm_setbit(g_pm_1p1_1v0_vote, CLOCK_CRG_ID_BT_CPU);
    }
}

static void pm_buck_vset_raise_judge(void)
{
    if (g_pm_1p1_1v1_vote != 0) {
        if (g_pm_current_1p1_vset < CLOCK_VSET_LEVEL_1V1) {
            uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P1, PMU_BUCK_1P1_VSET_1V1);
            g_pm_current_1p1_vset = CLOCK_VSET_LEVEL_1V1;
        }
    } else if (g_pm_1p1_1v0_vote != 0) {
        if (g_pm_current_1p1_vset < CLOCK_VSET_LEVEL_1V0) {
            uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P1, PMU_BUCK_1P1_VSET_1V0);
            g_pm_current_1p1_vset = CLOCK_VSET_LEVEL_1V0;
        }
    }

    uapi_tcxo_delay_us(BUCK_RAISE_DELAY);

    if (g_pm_0p9_1v1_vote != 0) {
        if (g_pm_current_0p9_vset < CLOCK_VSET_LEVEL_1V1) {
            uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_0P9, PMU_BUCK_0P9_VSET_1V1);
            g_pm_current_0p9_vset = CLOCK_VSET_LEVEL_1V1;
        }
    } else if (g_pm_0p9_1v0_vote != 0) {
        if (g_pm_current_0p9_vset < CLOCK_VSET_LEVEL_1V0) {
            uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_0P9, PMU_BUCK_0P9_VSET_1V0);
            g_pm_current_0p9_vset = CLOCK_VSET_LEVEL_1V0;
        }
    }
}

static void pm_buck_vset_reduce_judge(void)
{
    if (g_pm_1p1_1v1_vote == 0) {
        if (g_pm_1p1_1v0_vote == 0) {
            if (g_pm_current_1p1_vset > CLOCK_VSET_LEVEL_0V9) {
                uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P1, PMU_BUCK_1P1_VSET_0V9);
                g_pm_current_1p1_vset = CLOCK_VSET_LEVEL_0V9;
            }
        } else {
            if (g_pm_current_1p1_vset > CLOCK_VSET_LEVEL_1V0) {
                uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_1P1, PMU_BUCK_1P1_VSET_1V0);
                g_pm_current_1p1_vset = CLOCK_VSET_LEVEL_1V0;
            }
        }
    }

    if (g_pm_0p9_1v1_vote == 0) {
        if (g_pm_0p9_1v0_vote == 0) {
            if (g_pm_current_0p9_vset > CLOCK_VSET_LEVEL_0V9) {
                uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_0P9, PMU_BUCK_0P9_VSET_0V9);
                g_pm_current_0p9_vset = CLOCK_VSET_LEVEL_0V9;
            }
        } else {
            if (g_pm_current_0p9_vset > CLOCK_VSET_LEVEL_1V0) {
                uapi_pmu_ldo_set_voltage(PMU_LDO_ID_BUCK_0P9, PMU_BUCK_0P9_VSET_1V0);
                g_pm_current_0p9_vset = CLOCK_VSET_LEVEL_1V0;
            }
        }
    }
}

errcode_t uapi_clock_crg_config(clock_crg_id_t id, clock_clk_src_t clk_src, uint8_t clk_div)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(PM_GLB_CTL_B_GLB_GP_B_REG1, 0x5a);       // 0x5A: A核正在调频， 至少等待几个节拍再判断BT核加的锁
    clock_vset_level_t level = pm_pm_get_vset_by_clk(id, clk_src, clk_div);
    if (readw(PM_GLB_CTL_B_GLB_GP_B_REG2) != 0) {   // 等待BT调频结束
        writew(PM_GLB_CTL_B_GLB_GP_B_REG1, 0x0);    // 0x0: A核结束调频
        osal_irq_restore(irq_sts);
        return ERRCODE_FAIL;
    }

    pm_buck_vset_vote((uint8_t)id, level);

    pm_buck_vset_raise_judge();
    if (clk_src == CLOCK_CLK_SRC_ULPFLL_MCU_CORE || clk_src == CLOCK_CLK_SRC_FNPLL_MCU_HS) {
        writew(PM_GLB_CTL_B_GLB_GP_B_REG1, 0x0); // PLL锁定太久，避免BT等待太久，提前释放
    }

    system_ccrg_clock_config((clocks_ccrg_module_t)id, (clocks_clk_src_t)clk_src, clk_div);

    pm_buck_vset_reduce_judge();
    writew(PM_GLB_CTL_B_GLB_GP_B_REG1, 0x0); // 0x0: A核结束调频
    osal_irq_restore(irq_sts);
    oml_pf_log_print3(LOG_BCORE_PLT_LPM_SWITCH_CLK, LOG_NUM_LPM_SWITCH_CLK, LOG_LEVEL_INFO, \
        "[uapi_clock_crg_config]: id:0x%x, clk_src:0x%x, clk_div:0x%x\r\n", id, clk_src, clk_div);

    return ERRCODE_SUCC;
}

uint32_t uapi_clock_crg_get_freq(clock_crg_id_t id)
{
    return clocks_get_module_frequency((clocks_ccrg_module_t)id);
}

void uapi_clock_clken_config(clock_clken_id_t id, bool clk_en)
{
    unused(id);
    unused(clk_en);
}

static bool clock_control_freq_level_config(uint8_t level)
{
    errcode_t ret = ERRCODE_SUCC;
    if (level == CLOCK_FREQ_LEVEL_LOW_POWER) {
        pm_clrbit(g_pm_mcpu_dll2_freq_used, PM_MCPU_FREQ_USED_BY_MCU);
        pm_clrbit(g_pm_mcpu_ulppll_freq_used, PM_MCPU_FREQ_USED_BY_MCU);
        if ((g_pm_mcpu_dll2_freq_used == 0) && (g_pm_mcpu_ulppll_freq_used == 0)) {
            ret = uapi_clock_crg_config(CLOCK_CRG_ID_MCU_CORE, CLOCK_CLK_SRC_TCXO, 0x1);
        }
    } else if (level == CLOCK_FREQ_LEVEL_NORMAL) {
        pm_clrbit(g_pm_mcpu_ulppll_freq_used, PM_MCPU_FREQ_USED_BY_MCU);
        if (g_pm_mcpu_ulppll_freq_used == 0) {
            ret = uapi_clock_crg_config(CLOCK_CRG_ID_MCU_CORE, CLOCK_CLK_SRC_TCXO_2X, 0x1);
        }
        pm_setbit(g_pm_mcpu_dll2_freq_used, PM_MCPU_FREQ_USED_BY_MCU);
    } else {
        pm_clrbit(g_pm_mcpu_dll2_freq_used, PM_MCPU_FREQ_USED_BY_MCU);
        if (g_pm_mcpu_ulppll_freq_used == 0) {
            ret = uapi_clock_crg_config(CLOCK_CRG_ID_MCU_CORE, CLOCK_CLK_SRC_ULPFLL_MCU_CORE, 0x1);
        }
        pm_setbit(g_pm_mcpu_ulppll_freq_used, PM_MCPU_FREQ_USED_BY_MCU);
    }
    if (ret == ERRCODE_SUCC) {
        return true;
    } else {
        return false;
    }
}

static bool clock_control_mclken_enable(uint8_t type)
{
    if (type < CLOCK_APERP_MTIMER_CLKEN) {
        reg16_setbit(M_CTL_M_CLKEN0_REG, type);
    } else {
        reg16_setbit(M_CTL_M_CLKEN1_REG, (type - CLOCK_APERP_MTIMER_CLKEN));
    }
    return true;
}

static bool clock_control_mclken_disable(uint8_t type)
{
    if (type < CLOCK_APERP_MTIMER_CLKEN) {
        reg16_clrbit(M_CTL_M_CLKEN0_REG, type);
    } else {
        reg16_clrbit(M_CTL_M_CLKEN1_REG, (type - CLOCK_APERP_MTIMER_CLKEN));
    }
    return true;
}

static bool clock_control_xo_clkout_enable(uint8_t type)
{
    reg16_setbit(PMU2_CMU_CTL_XO_RESERVE_0_REG, type);
    return true;
}

static bool clock_control_xo_clkout_disable(uint8_t type)
{
    reg16_clrbit(PMU2_CMU_CTL_XO_RESERVE_0_REG, type);
    return true;
}

static bool clock_control_glb_clken_enable(uint8_t type)
{
    reg16_setbit(M_CTL_GLB_CLKEN_REG, type);
    return true;
}

static bool clock_control_glb_clken_disable(uint8_t type)
{
    reg16_clrbit(M_CTL_GLB_CLKEN_REG, type);
    return true;
}

static bool clock_control_bth_freq_config(uint8_t level)
{
    errcode_t ret = ERRCODE_SUCC;
    if (level == CLOCK_FREQ_LEVEL_LOW_POWER) {
        pm_clrbit(g_pm_mcpu_dll2_freq_used, PM_MCPU_FREQ_USED_BY_BTH);
        pm_clrbit(g_pm_mcpu_ulppll_freq_used, PM_MCPU_FREQ_USED_BY_BTH);
        if ((g_pm_mcpu_dll2_freq_used == 0) && (g_pm_mcpu_ulppll_freq_used == 0)) {
            ret = uapi_clock_crg_config(CLOCK_CRG_ID_MCU_CORE, CLOCK_CLK_SRC_TCXO, 0x1);
        }
    } else if (level == CLOCK_FREQ_LEVEL_NORMAL) {
        pm_clrbit(g_pm_mcpu_ulppll_freq_used, PM_MCPU_FREQ_USED_BY_BTH);
        if (g_pm_mcpu_ulppll_freq_used == 0) {
            ret = uapi_clock_crg_config(CLOCK_CRG_ID_MCU_CORE, CLOCK_CLK_SRC_TCXO_2X, 0x1);
        }
        pm_setbit(g_pm_mcpu_dll2_freq_used, PM_MCPU_FREQ_USED_BY_BTH);
    } else {
        pm_clrbit(g_pm_mcpu_dll2_freq_used, PM_MCPU_FREQ_USED_BY_BTH);
        if (g_pm_mcpu_ulppll_freq_used == 0) {
            ret = uapi_clock_crg_config(CLOCK_CRG_ID_MCU_CORE, CLOCK_CLK_SRC_ULPFLL_MCU_CORE, 0x1);
        }
        pm_setbit(g_pm_mcpu_ulppll_freq_used, PM_MCPU_FREQ_USED_BY_BTH);
    }
    if (ret == ERRCODE_SUCC) {
        return true;
    } else {
        return false;
    }
}

static bool clock_control_bt_core_freq_config(uint8_t level)
{
    if ((level == (uint8_t)CLOCK_MCU_CTL_BT_32M) || (level == (uint8_t)CLOCK_MCU_CTL_BT_64M)) {
        writew(PM_GLB_CTL_B_GLB_GP_B_REG3, (uint16_t)(clock_mcu_ctl_bt_t)level);
    } else {
        writew(PM_GLB_CTL_B_GLB_GP_B_REG3, (uint16_t)CLOCK_MCU_CTL_BT_AUTO);
    }
    return true;
}

static const clock_control_func_t g_clock_control_funcs[] = {
    clock_control_freq_level_config,
    clock_control_mclken_enable,
    clock_control_mclken_disable,
    clock_control_xo_clkout_enable,
    clock_control_xo_clkout_disable,
    clock_control_glb_clken_enable,
    clock_control_glb_clken_disable,
    clock_control_bth_freq_config,
    clock_control_bt_core_freq_config,
};

errcode_t uapi_clock_control(clock_control_type_t type, uint8_t param)
{
    if (type >= CLOCK_CONTROL_TYPE_MAX) {
        return ERRCODE_INVALID_PARAM;
    }
    bool ret = g_clock_control_funcs[type](param);
    if (ret) {
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}