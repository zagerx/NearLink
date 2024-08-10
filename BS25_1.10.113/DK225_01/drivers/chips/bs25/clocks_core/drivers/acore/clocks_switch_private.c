/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description:  CLOCKS CORE PRIVATE SOURCE.
 * Author: @CompanyNameTag
 * Create: 2022-04-13
 */

#include "clocks_config.h"
#include "hal_clocks_application.h"
#include "clocks_switch.h"
#include "pmu_cmu.h"
#include "clocks_switch_private.h"

clocks_pll_src_t clocks_pll_veto_type_translation(clocks_clk_src_t clk_src)
{
    switch (clk_src) {
        case CLOCKS_CLK_SRC_TCXO_2X:
            return CLOCKS_PLL_SRC_TCXO_COM_DLL2;
        case CLOCKS_CLK_SRC_ULPFLL_MCU_CORE:
            return CLOCKS_PLL_SRC_ULPFLL_MCU_CORE;
        case CLOCKS_CLK_SRC_FNPLL_MCU_HS:
            return CLOCKS_PLL_SRC_FNPLL_MCU_HS;
        default:
            return CLOCKS_PLL_SRC_MAX;
    }
}

void clocks_pll_power_on(clocks_pll_src_t pll_src)
{
    switch (pll_src) {
        case CLOCKS_PLL_SRC_TCXO_COM_DLL2:
            hal_clocks_ccrg_xo_dll2_clk_en((switch_type_t)true);
            break;
        case CLOCKS_PLL_SRC_ULPFLL_MCU_CORE:
            pmu_cmu_ulpfll_reinit(clocks_get_ulpfll_cfg());
            break;
        case CLOCKS_PLL_SRC_FNPLL_MCU_HS:
            pmu_cmu_fnpll_reinit(clocks_get_fnpll_cfg());
            break;
        default:
            break;
    }
}

void clocks_pll_power_off(clocks_pll_src_t pll_src)
{
    switch (pll_src) {
        case CLOCKS_PLL_SRC_TCXO_COM_DLL2:
            hal_clocks_ccrg_xo_dll2_clk_en((switch_type_t)false);
            break;
        case CLOCKS_PLL_SRC_ULPFLL_MCU_CORE:
            pmu_cmu_ulpfll_deinit();
            break;
        case CLOCKS_PLL_SRC_FNPLL_MCU_HS:
            pmu_cmu_fnpll_deinit();
            break;
        default:
            break;
    }
}

static hal_clocks_ccrg_mcu_group_clk_src_t clocks_clkout_src_sel_type_translation(clocks_clk_src_t clk_src)
{
    switch (clk_src) {
        case CLOCKS_CLK_SRC_RC:
            return HAL_CLOCKS_CCRG_PAD_CLKOUT_CLK_SRC_RC;
        case CLOCKS_CLK_SRC_32K:
            return HAL_CLOCKS_CCRG_PAD_CLKOUT_CLK_SRC_32K;
        case CLOCKS_CLK_SRC_TCXO:
            return HAL_CLOCKS_CCRG_PAD_CLKOUT_CLK_SRC_XO;
        case CLOCKS_CLK_SRC_PAD_CLKIN:
            return HAL_CLOCKS_CCRG_PAD_CLKOUT_CLK_SRC_CLKIN;
        case CLOCKS_CLK_SRC_ULPFLL_MCU_CORE:
            return HAL_CLOCKS_CCRG_PAD_CLKOUT_CLK_SRC_ULPFLL_MCU_CORE;
        case CLOCKS_CLK_SRC_FNPLL_MCU_HS:
            return HAL_CLOCKS_CCRG_PAD_CLKOUT_CLK_SRC_FNPLL_MCU_HS;
        case CLOCKS_CLK_SRC_TCXO_2X:
            return HAL_CLOCKS_CCRG_PAD_CLKOUT_CLK_SRC_XO_DLL2;
        default:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX;
    }
}

hal_clocks_ccrg_mcu_group_clk_src_t clocks_src_sel_type_translation(clocks_ccrg_module_t module,
                                                                    clocks_clk_src_t clk_src)
{
    if (module >= CLOCKS_CCRG_MODULE_PAD_OUT0) {
        return clocks_clkout_src_sel_type_translation(clk_src);
    }
    switch (clk_src) {
        case CLOCKS_CLK_SRC_RC:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_RC;
        case CLOCKS_CLK_SRC_TCXO:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_XO;
        case CLOCKS_CLK_SRC_TCXO_2X:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_XO_DLL2;
        case CLOCKS_CLK_SRC_ULPFLL_MCU_CORE:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_ULPFLL_MCU_CORE;
        case CLOCKS_CLK_SRC_FNPLL_MCU_HS:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_FNPLL_MCU_HS;
        default:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX;
    }
}

void system_i2s_clk_config(clocks_i2s_src_t source_config)
{
    UNUSED(source_config);
}

uint32_t clocks_get_pll_frequency(clocks_clk_src_t clk_src)
{
    uint32_t clk_freq = 0;
    switch (clk_src) {
        case CLOCKS_CLK_SRC_ULPFLL_MCU_CORE:
            clk_freq = CMU_ULPFLL_FREQUENCY;
            break;
        case CLOCKS_CLK_SRC_FNPLL_MCU_HS:
            clk_freq = CMU_FNPLL_FREQUENCY;
            break;
        default:
            break;
    }
    return clk_freq;
}