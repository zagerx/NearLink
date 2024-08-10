/*
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description:  BT PMU DRIVER
 * Author: @CompanyNameTag
 * Create: 2019-12-19
 */

#include "soc_osal.h"
#include "tcxo.h"
#include "panic.h"
#include "pm_lpc.h"
#include "platform_core.h"
#include "debug_print.h"
#include "pmu_cmu.h"

#define PMU2_CMU_CTL_XO_RESERVE_0_REG           (PMU2_CMU_CTL_RB_BASE + 0x6C0)
#define XO_RESERVE_0_D_CLK2FNPLL_EN_BIT         1

#define PMU2_CMU_CTL_MCU_HS_FBDIV_REG           (PMU2_CMU_CTL_RB_BASE + 0x5F8)
#define PMU2_CMU_CTL_MCU_HS_FRAC_H_REG          (PMU2_CMU_CTL_RB_BASE + 0x638)
#define PMU2_CMU_CTL_MCU_HS_FRAC_L_REG          (PMU2_CMU_CTL_RB_BASE + 0x63C)
#define PMU2_CMU_CTL_MCU_HS_REFDIV_REG          (PMU2_CMU_CTL_RB_BASE + 0x650)
#define PMU2_CMU_CTL_MCU_HS_POSTDIV1_REG        (PMU2_CMU_CTL_RB_BASE + 0x648)
#define PMU2_CMU_CTL_MCU_HS_POSTDIV2_REG        (PMU2_CMU_CTL_RB_BASE + 0x64C)

#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG0_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x5FC)
#define MCU_HS_FNPLL_CFG0_H_VALUE               0x3483
#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG0_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x600)
#define MCU_HS_FNPLL_CFG0_L_VALUE               0x22
#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG1_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x604)
#define MCU_HS_FNPLL_CFG1_H_VALUE               0x0
#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG1_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x608)
#define MCU_HS_FNPLL_CFG1_L_VALUE               0x5F
#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG2_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x60C)
#define MCU_HS_FNPLL_CFG2_H_VALUE               0x8001
#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG2_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x610)
#define MCU_HS_FNPLL_CFG2_L_VALUE               0x80A6
#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG3_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x614)
#define MCU_HS_FNPLL_CFG3_H_VALUE               0x28
#define PMU2_CMU_CTL_MCU_HS_FNPLL_CFG3_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x618)
#define MCU_HS_FNPLL_CFG3_L_VALUE               0x6271

#define PMU2_CMU_CTL_MCU_HS_PLL_MODE_REG        (PMU2_CMU_CTL_RB_BASE + 0x644)
#define PMU2_CMU_CTL_MCU_HS_BYPASS_REG          (PMU2_CMU_CTL_RB_BASE + 0x5F4)
#define PMU2_CMU_CTL_MCU_HS_FOUT2XPD_REG        (PMU2_CMU_CTL_RB_BASE + 0x62C)
#define PMU2_CMU_CTL_MCU_HS_FOUT4PHASEPD_REG    (PMU2_CMU_CTL_RB_BASE + 0x630)
#define PMU2_CMU_CTL_MCU_HS_FOUTPOSTDIVPD_REG   (PMU2_CMU_CTL_RB_BASE + 0x634)
#define PMU2_CMU_CTL_MCU_HS_PD_REG              (PMU2_CMU_CTL_RB_BASE + 0x640)
#define PMU2_CMU_CTL_MCU_HS_ISO_REG             (PMU2_CMU_CTL_RB_BASE + 0x654)
#define PMU2_CMU_CTL_MCU_HS_LOCK_REG            (PMU2_CMU_CTL_RB_BASE + 0x658)
#define PMU2_CMU_CTL_FNPLL_MCU_HS_CLKEN_REG     (PMU2_CMU_CTL_RB_BASE + 0x174)

#define FNPLL_LOCK_WAIT_MAX_US      300
#define FNPLL_MIN_LOCK_TIMES        3
#define FNPLL_MAX_ENABLE_TIMES      3

#define PMU2_CMU_CTL_MCU_CORE_FLL_FBDIV_REG     (PMU2_CMU_CTL_RB_BASE + 0x6B8)
#define PMU2_CMU_CTL_MCU_CORE_FLL_POSTDIV_REG   (PMU2_CMU_CTL_RB_BASE + 0x6BC)

#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG0_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x670)
#define MCU_CORE_FLL_CFG0_H_VALUE               0x8A55
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG0_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x674)
#define MCU_CORE_FLL_CFG0_L_VALUE               0xA9D7
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG1_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x678)
#define MCU_CORE_FLL_CFG1_H_VALUE               0x86
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG1_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x67C)
#define MCU_CORE_FLL_CFG1_L_VALUE               0x1400
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG2_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x680)
#define MCU_CORE_FLL_CFG2_H_VALUE               0x1020
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG2_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x684)
#define MCU_CORE_FLL_CFG2_L_VALUE               0x1020
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG3_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x688)
#define MCU_CORE_FLL_CFG3_H_VALUE               0x7C13
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG3_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x68C)
#define MCU_CORE_FLL_CFG3_L_VALUE               0xFF5
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG4_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x690)
#define MCU_CORE_FLL_CFG4_H_VALUE               0x143
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG4_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x694)
#define MCU_CORE_FLL_CFG4_L_VALUE               0xF918
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG5_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x698)
#define MCU_CORE_FLL_CFG5_H_VALUE               0x4080
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG5_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x69C)
#define MCU_CORE_FLL_CFG5_L_VALUE               0x118
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG6_H_REG    (PMU2_CMU_CTL_RB_BASE + 0x6A0)
#define MCU_CORE_FLL_CFG6_H_VALUE               0x0
#define PMU2_CMU_CTL_MCU_CORE_FLL_CFG6_L_REG    (PMU2_CMU_CTL_RB_BASE + 0x6A4)
#define MCU_CORE_FLL_CFG6_L_VALUE               0xC00

#define PMU2_CMU_CTL_MCU_CORE_FLL_EN_REG        (PMU2_CMU_CTL_RB_BASE + 0x6A8)
#define MCU_CORE_FLL_EN_ULPFLL_ISO_EN_BIT       2
#define MCU_CORE_FLL_EN_ULPFLL_EN_BIT           0
#define PMU2_CMU_CTL_MCU_CORE_FLL_LOCK_REG      (PMU2_CMU_CTL_RB_BASE + 0x6AC)
#define PMU2_CMU_CTL_ULPFLL_MCU_CORE_CLKEN_REG  (PMU2_CMU_CTL_RB_BASE + 0x17C)

#define ULPFLL_LOCK_WAIT_MAX_US     2000
#define ULPFLL_MIN_LOCK_TIMES       3
#define ULPFLL_MAX_ENABLE_TIMES     3

static void pmu_cmu_mcu_hs_fnpll_cfg(cmu_fnpll_config_t *pll_cfg)
{
    writew(PMU2_CMU_CTL_MCU_HS_ISO_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_PD_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_BYPASS_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_FOUTPOSTDIVPD_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_FOUT2XPD_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_FOUT4PHASEPD_REG, 0x1);

    writew(PMU2_CMU_CTL_MCU_HS_FBDIV_REG, pll_cfg->pll_fbdiv);
    writew(PMU2_CMU_CTL_MCU_HS_FRAC_H_REG, pll_cfg->pll_frac_h);
    writew(PMU2_CMU_CTL_MCU_HS_FRAC_L_REG, pll_cfg->pll_frac_l);
    writew(PMU2_CMU_CTL_MCU_HS_REFDIV_REG, 1);
    writew(PMU2_CMU_CTL_MCU_HS_POSTDIV1_REG, pll_cfg->pll_postdiv1);
    writew(PMU2_CMU_CTL_MCU_HS_POSTDIV2_REG, pll_cfg->pll_postdiv2);
    writew(PMU2_CMU_CTL_MCU_HS_PLL_MODE_REG, pll_cfg->pll_mode);

    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG0_H_REG, MCU_HS_FNPLL_CFG0_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG0_L_REG, MCU_HS_FNPLL_CFG0_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG1_H_REG, MCU_HS_FNPLL_CFG1_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG1_L_REG, MCU_HS_FNPLL_CFG1_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG2_H_REG, MCU_HS_FNPLL_CFG2_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG2_L_REG, MCU_HS_FNPLL_CFG2_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG3_H_REG, MCU_HS_FNPLL_CFG3_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_HS_FNPLL_CFG3_L_REG, MCU_HS_FNPLL_CFG3_L_VALUE);
}

void pmu_cmu_fnpll_reinit(cmu_fnpll_config_t *pll_cfg)
{
    uint8_t enable_times = 0;
    uint64_t base_us;
    uint8_t lock_count;

    uint32_t status = osal_irq_lock();
    pm_lpc_ulp_clkldo_enable(LPC_ULP_CLKLDO_USED_FNPLL, true);

    do {
        reg16_setbit(PMU2_CMU_CTL_XO_RESERVE_0_REG, XO_RESERVE_0_D_CLK2FNPLL_EN_BIT);
        pmu_cmu_mcu_hs_fnpll_cfg(pll_cfg);

        writew(PMU2_CMU_CTL_MCU_HS_PD_REG, 0x0);
        writew(PMU2_CMU_CTL_MCU_HS_ISO_REG, 0x0);

        uapi_tcxo_delay_us(20); // Delay 20us.

        lock_count = 0;
        base_us = uapi_tcxo_get_us();

        while (uapi_tcxo_get_us() - base_us < FNPLL_LOCK_WAIT_MAX_US) {
            if (readw(PMU2_CMU_CTL_MCU_HS_LOCK_REG) == 1) { // Lock.
                lock_count++;
            } else {
                lock_count = 0;
            }
            if (lock_count >= FNPLL_MIN_LOCK_TIMES) { break; }
            uapi_tcxo_delay_us(1);  // Delay 1us.
        }
        if (lock_count >= FNPLL_MIN_LOCK_TIMES) {
            break;
        } else if (++enable_times >= FNPLL_MAX_ENABLE_TIMES) {
            PRINT("[0x5700865C]: 0x%x, [0x57008660]: 0x%x, [0x57008664]: 0x%x, [0x57008668]: 0x%x \r\n",
                  readw(0x5700865C), readw(0x57008660), readw(0x57008664), readw(0x57008668));
            panic(PANIC_PMU_CMU, __LINE__);
        }
    } while (true);

    writew(PMU2_CMU_CTL_MCU_HS_FOUTPOSTDIVPD_REG, 0x0);
    writew(PMU2_CMU_CTL_MCU_HS_FOUT2XPD_REG, 0x0);
    writew(PMU2_CMU_CTL_MCU_HS_FOUT4PHASEPD_REG, 0x0);
    writew(PMU2_CMU_CTL_MCU_HS_BYPASS_REG, 0x0);
    uapi_tcxo_delay_us(1);
    writew(PMU2_CMU_CTL_FNPLL_MCU_HS_CLKEN_REG, 0x1);
    osal_irq_restore(status);
}

void pmu_cmu_fnpll_deinit(void)
{
    uint32_t status = osal_irq_lock();
    writew(PMU2_CMU_CTL_FNPLL_MCU_HS_CLKEN_REG, 0x0);
    writew(PMU2_CMU_CTL_MCU_HS_ISO_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_PD_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_BYPASS_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_FOUTPOSTDIVPD_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_FOUT2XPD_REG, 0x1);
    writew(PMU2_CMU_CTL_MCU_HS_FOUT4PHASEPD_REG, 0x1);
    reg16_clrbit(PMU2_CMU_CTL_XO_RESERVE_0_REG, XO_RESERVE_0_D_CLK2FNPLL_EN_BIT);
    pm_lpc_ulp_clkldo_enable(LPC_ULP_CLKLDO_USED_FNPLL, false);
    osal_irq_restore(status);
}

static void pmu_cmu_mcu_core_ulpfll_cfg(cmu_ulpfll_config_t *fll_cfg)
{
    reg16_setbit(PMU2_CMU_CTL_MCU_CORE_FLL_EN_REG, MCU_CORE_FLL_EN_ULPFLL_ISO_EN_BIT);
    reg16_clrbit(PMU2_CMU_CTL_MCU_CORE_FLL_EN_REG, MCU_CORE_FLL_EN_ULPFLL_EN_BIT);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_FBDIV_REG, fll_cfg->fll_fbdiv);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_POSTDIV_REG, fll_cfg->fll_postdiv);

    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG0_H_REG, MCU_CORE_FLL_CFG0_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG0_L_REG, MCU_CORE_FLL_CFG0_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG1_H_REG, MCU_CORE_FLL_CFG1_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG1_L_REG, MCU_CORE_FLL_CFG1_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG2_H_REG, MCU_CORE_FLL_CFG2_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG2_L_REG, MCU_CORE_FLL_CFG2_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG3_H_REG, MCU_CORE_FLL_CFG3_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG3_L_REG, MCU_CORE_FLL_CFG3_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG4_H_REG, MCU_CORE_FLL_CFG4_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG4_L_REG, MCU_CORE_FLL_CFG4_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG5_H_REG, MCU_CORE_FLL_CFG5_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG5_L_REG, MCU_CORE_FLL_CFG5_L_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG6_H_REG, MCU_CORE_FLL_CFG6_H_VALUE);
    writew(PMU2_CMU_CTL_MCU_CORE_FLL_CFG6_L_REG, MCU_CORE_FLL_CFG6_L_VALUE);
}

void pmu_cmu_ulpfll_reinit(cmu_ulpfll_config_t *fll_cfg)
{
    uint8_t enable_times = 0;
    uint64_t base_us, cur_us;
    uint8_t lock_count;
    uint32_t status = osal_irq_lock();
    pm_lpc_ulp_clkldo_enable(LPC_ULP_CLKLDO_USED_ULPFLL, true);
    do {
        pmu_cmu_mcu_core_ulpfll_cfg(fll_cfg);
        uapi_tcxo_delay_us(80); // Delay 80us.
        reg16_setbit(PMU2_CMU_CTL_MCU_CORE_FLL_EN_REG, MCU_CORE_FLL_EN_ULPFLL_EN_BIT);
        uapi_tcxo_delay_us(5000); // Delay 5000us.
        reg16_clrbit(PMU2_CMU_CTL_MCU_CORE_FLL_EN_REG, MCU_CORE_FLL_EN_ULPFLL_ISO_EN_BIT);

        lock_count = 0;
        base_us = uapi_tcxo_get_us();
        cur_us  = base_us;

        while (cur_us - base_us < ULPFLL_LOCK_WAIT_MAX_US) {
            if (readw(PMU2_CMU_CTL_MCU_CORE_FLL_LOCK_REG) == 1) {   // Lock.
                lock_count++;
            } else {
                lock_count = 0;
            }
            if (lock_count >= FNPLL_MIN_LOCK_TIMES) { break; }
            uapi_tcxo_delay_us(100);  // Delay 100us.
            cur_us = uapi_tcxo_get_us();
        }
        enable_times++;
        if (lock_count >= ULPFLL_MIN_LOCK_TIMES) {
            break;
        } else if (enable_times >= ULPFLL_MAX_ENABLE_TIMES) {
            reg32_setbits(0x5700867C, 0x4, 0x4, 0x2);
            PRINT("[0x570086B0]: 0x%x, [0x570086B4]: 0x%x \r\n", readw(0x570086B0), readw(0x570086B4));
            panic(PANIC_PMU_CMU, __LINE__);
            break;
        }
    } while (true);

    writew(PMU2_CMU_CTL_ULPFLL_MCU_CORE_CLKEN_REG, 0x1);    // Clken.
    osal_irq_restore(status);
}

void pmu_cmu_ulpfll_deinit(void)
{
    uint32_t status = osal_irq_lock();
    writew(PMU2_CMU_CTL_ULPFLL_MCU_CORE_CLKEN_REG, 0x0);
    reg16_setbit(PMU2_CMU_CTL_MCU_CORE_FLL_EN_REG, MCU_CORE_FLL_EN_ULPFLL_ISO_EN_BIT);
    reg16_clrbit(PMU2_CMU_CTL_MCU_CORE_FLL_EN_REG, MCU_CORE_FLL_EN_ULPFLL_EN_BIT);
    pm_lpc_ulp_clkldo_enable(LPC_ULP_CLKLDO_USED_ULPFLL, false);
    osal_irq_restore(status);
}

void hal_lpc_init(void)
{
    return;
}