/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description:  Clock config source file.
 * Author: @CompanyNameTag
 * Create: 2023-05-17
 */
#include "product.h"
#include "clocks_config.h"

static cmu_ulpfll_config_t g_ulpfll_cfg = {
    // 86.016M
#if (RC_CLOCK_ON == YES)
    .fll_fbdiv = 0x540,
#else
    .fll_fbdiv = 0x521,
#endif
    .fll_postdiv = 0x0,
};

static cmu_fnpll_config_t g_fnpll_cfg = {
    // 96M
    .pll_fbdiv = 0x24,
    .pll_frac_h = 0x0,
    .pll_frac_l = 0x0,
    .pll_postdiv1 = 0x2,
    .pll_postdiv2 = 0x3,
    .pll_mode = 0x1,
};

static const clocks_clk_cfg_t g_system_all_core_clocks[CLOCKS_CCRG_MODULE_MAX] = {
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},             // CLOCKS_CCRG_MODULE_MCU_CORE
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_2},             // CLOCKS_CCRG_MODULE_MCU_PERP_LS
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_2},             // CLOCKS_CCRG_MODULE_MCU_PERP_UART
    {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_2},              // CLOCKS_CCRG_MODULE_MCU_PERP_SPI
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},             // CLOCKS_CCRG_MODULE_COM_BUS
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},             // CLOCKS_CCRG_MODULE_MEM_BUS
    {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},              // CLOCKS_CCRG_MODULE_USB_PHY_BUS
    {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},              // CLOCKS_CCRG_MODULE_PDM
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},             // CLOCKS_CCRG_MODULE_XIP_OPI
#if defined(USE_EMBED_FLASH)
    {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},              // CLOCKS_CCRG_MODULE_XIP_QSPI
#else
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},             // CLOCKS_CCRG_MODULE_XIP_QSPI
#endif
    {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},              // CLOCKS_CCRG_MODULE_PAD_OUT0
};

cmu_ulpfll_config_t *clocks_get_ulpfll_cfg(void)
{
    return &g_ulpfll_cfg;
}

cmu_fnpll_config_t *clocks_get_fnpll_cfg(void)
{
    return &g_fnpll_cfg;
}

const clocks_clk_cfg_t *clocks_system_all_clocks_get(void)
{
    return g_system_all_core_clocks;
}
