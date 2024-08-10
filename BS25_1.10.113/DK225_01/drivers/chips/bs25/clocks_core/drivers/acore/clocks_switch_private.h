/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description:  CLOCKS CORE PRIVATE HEADER.
 * Author: @CompanyNameTag
 * Create: 2022-04-13
 */
#ifndef CLOCKS_SWITCH_PRIVATE_H
#define CLOCKS_SWITCH_PRIVATE_H

#include "clocks_types.h"
#include "pmu_cmu.h"

/**
 * @addtogroup connectivity_drivers_non_os_clocks_core
 * @{
 */
#define LPPLL_I2S_49152K_CLK_DIV                1UL
#define PLL_I2S_CODEC_CLKDIV                    8UL
#define EXCLK_I2S_CODEC_CLKDIV                  1UL

/**
 * @brief  Modules in the common crg.
 */
typedef enum {
    CLOCKS_CCRG_MODULE_MCU_CORE,
    CLOCKS_CCRG_MODULE_MCU_PERP_LS,
    CLOCKS_CCRG_MODULE_MCU_PERP_UART,
    CLOCKS_CCRG_MODULE_MCU_PERP_SPI,
    CLOCKS_CCRG_MODULE_COM_BUS,
    CLOCKS_CCRG_MODULE_MEM_BUS,
    CLOCKS_CCRG_MODULE_USB_PHY_BUS,
    CLOCKS_CCRG_MODULE_PDM,
    CLOCKS_CCRG_MODULE_XIP_OPI,
    CLOCKS_CCRG_MODULE_XIP_QSPI,
    CLOCKS_CCRG_MODULE_PAD_OUT0,
    CLOCKS_CCRG_MODULE_MAX,
} clocks_ccrg_module_t;

/**
 * @brief  The clock source of the system.
 */
typedef enum {
    CLOCKS_CLK_SRC_RC,
    CLOCKS_CLK_SRC_32K = 1,
    CLOCKS_CLK_SRC_TCXO = 2,
    CLOCKS_CLK_SRC_TCXO_2X = 3,
    CLOCKS_CLK_SRC_ULPFLL_MCU_CORE = 4,
    CLOCKS_CLK_SRC_FNPLL_MCU_HS = 5,
    CLOCKS_CLK_SRC_PAD_CLKIN = 6,
    CLOCKS_CLK_SRC_MAX,
} clocks_clk_src_t;

/**
 * @brief clock config struct type.
 */
typedef struct {
    clocks_clk_src_t clk_src;
    uint8_t clk_div;
} clocks_clk_cfg_t;

/**
 * @brief PLL type enum.
 */
typedef enum {
    CLOCKS_PLL_SRC_TCXO_COM_DLL2,
    CLOCKS_PLL_SRC_ULPFLL_MCU_CORE,
    CLOCKS_PLL_SRC_FNPLL_MCU_HS,
    CLOCKS_PLL_SRC_MAX,
} clocks_pll_src_t;

/**
 * @}
 */
#endif
