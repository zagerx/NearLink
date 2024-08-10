/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description:   HAL APP CLOCK PRIVATE SOURCE.
 * Author: @CompanyNameTag
 * Create: 2021-06-16
 */

#include "non_os.h"
#include "hal_reg_config.h"
#include "hal_clocks_application_private.h"

/* This register not belongs to com crg, need to move to other place. */
#define CMU_CLOCKS_MCU_CORE_CR_CH0_REG          (M_CTL_RB_BASE + 0x54C)
#define CMU_CLOCKS_MCU_CORE_CR_CH1_REG          (M_CTL_RB_BASE + 0x550)
#define CMU_CLOCKS_MCU_PERP_LS_CR_REG           (M_CTL_RB_BASE + 0x554)
#define CMU_CLOCKS_MCU_PERP_UART_CR_REG         (M_CTL_RB_BASE + 0x558)
#define CMU_CLOCKS_MCU_PERP_SPI_CR_REG          (M_CTL_RB_BASE + 0x55C)
#define CMU_CLOCKS_COM_BUS_CR_CH0_REG           (M_CTL_RB_BASE + 0x560)
#define CMU_CLOCKS_COM_BUS_CR_CH1_REG           (M_CTL_RB_BASE + 0x564)
#define CMU_CLOCKS_SDIOM_CR_REG                 (M_CTL_RB_BASE + 0x568)
#define CMU_CLOCKS_MEM_BUS_CR_REG               (M_CTL_RB_BASE + 0x56C)
#define CMU_CLOCKS_XIP_OPI_CR_REG               (M_CTL_RB_BASE + 0x570)
#define CMU_CLOCKS_XIP_QSPI_CR_REG              (M_CTL_RB_BASE + 0x574)
#define CMU_CLOCKS_USB_PHY_CR_REG               (M_CTL_RB_BASE + 0x604)
#define CMU_CLOCKS_PDM_CR_REG                   (M_CTL_RB_BASE + 0x608)
#define CMU_CLOCKS_PAD_CLK_OUT0_CR_REG          (M_CTL_RB_BASE + 0x598)

#define DSP_DIV_NUM_REG                         0x58007000
#define DSP0_DIV_NUM_POS                        0
#define DSP1_DIV_NUM_POS                        16
#define DSP_DIV_NUM_LEN                         6

#define CMU_CLOCKS_MSUB_NORMAL_REG  (M_CTL_RB_BASE + 0x3e0)
#define MSUB_CFG_REFRESH_REG        (M_CTL_RB_BASE + 0x3F0)
#define MSUB_CFG_REFRESH_BIT        0

static uint32_t g_ccrg_modules_reg_map[HAL_CLOCKS_MODULE_MAX] = {
    CMU_CLOCKS_MCU_CORE_CR_CH1_REG,
    CMU_CLOCKS_MCU_PERP_LS_CR_REG,
    CMU_CLOCKS_MCU_PERP_UART_CR_REG,
    CMU_CLOCKS_MCU_PERP_SPI_CR_REG,
    CMU_CLOCKS_COM_BUS_CR_CH1_REG,
    CMU_CLOCKS_MEM_BUS_CR_REG,
    CMU_CLOCKS_USB_PHY_CR_REG,
    CMU_CLOCKS_PDM_CR_REG,
    CMU_CLOCKS_XIP_OPI_CR_REG,
    CMU_CLOCKS_XIP_QSPI_CR_REG,
    CMU_CLOCKS_PAD_CLK_OUT0_CR_REG,
};

uint32_t hal_clocks_ccrg_reg_get(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch)
{
    if (module == HAL_CLOCKS_MODULE_MAX) {
        return 0;
    }
    uint32_t reg_addr = g_ccrg_modules_reg_map[module];

    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_CORE:
            reg_addr = (ch == HAL_CLOCKS_CRG_CH0) ? CMU_CLOCKS_MCU_CORE_CR_CH0_REG : CMU_CLOCKS_MCU_CORE_CR_CH1_REG;
        break;

        case HAL_CLOCKS_MODULE_COM_BUS:
            reg_addr = (ch == HAL_CLOCKS_CRG_CH0) ? CMU_CLOCKS_COM_BUS_CR_CH0_REG : CMU_CLOCKS_COM_BUS_CR_CH1_REG;
        break;

        default:
        break;
    }

    return reg_addr;
}

hal_clocks_crg_ch_t hal_clocks_ccrg_current_ch_get(hal_clocks_ccrg_module_t module)
{
    if (module != HAL_CLOCKS_MODULE_MCU_CORE) {
        return HAL_CLOCKS_CRG_CH_INVALID;
    }
    hal_clocks_crg_ch_t ch;
    non_os_enter_critical();
    ch = reg16_getbits((void *)((uintptr_t)g_ccrg_modules_reg_map[module]),
        CMU_CLOCKS_COM_CR_CH_SEL_STS_BIT, CMU_CLOCKS_COM_CR_CH_SEL_STS_LEN);
    non_os_exit_critical();
    return ch;
}

void hal_clocks_app_perips_config(hal_clocks_app_perips_clk_type_t app_clk, switch_type_t on)
{
    if (app_clk < HAL_CLOCKS_APP_MCLK_EN1_BASE) {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN0_BASE;
        hal_clocks_config_bit(CMU_CLOCKS_M_CLKEN0_REG, on, (REG16_POS)app_clk);
    } else if (app_clk < HAL_CLOCKS_APP_BUTT) {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN1_BASE;
        hal_clocks_config_bit(CMU_CLOCKS_M_CLKEN1_REG, on, (REG16_POS)app_clk);
    } else {
        return;
    }
}

bool hal_clocks_app_perips_get_config(hal_clocks_app_perips_clk_type_t app_clk)
{
    if (app_clk < HAL_CLOCKS_APP_MCLK_EN1_BASE) {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN0_BASE;
        return reg16_getbit(CMU_CLOCKS_M_CLKEN0_REG, (REG16_POS)app_clk);
    } else if (app_clk < HAL_CLOCKS_APP_BUTT) {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN1_BASE;
        return reg16_getbit(CMU_CLOCKS_M_CLKEN1_REG, (REG16_POS)app_clk);
    } else {
        return false;
    }
}

void hal_clocks_sub_normal_sel_cfg(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch)
{
    if (ch == HAL_CLOCKS_CRG_CH_INVALID) {
        return;
    }
    uint8_t pos;
    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_CORE:
            pos = HAL_CLOCKS_MCU_CORE_CH_SEL_NOR;
            break;
        case HAL_CLOCKS_MODULE_COM_BUS:
            pos = HAL_CLOCKS_COM_BUS_CH_SEL_NOR;
            break;
        default:
            return;
    }
    reg16_setbit(MSUB_CFG_REFRESH_REG, MSUB_CFG_REFRESH_BIT);
    if (ch == HAL_CLOCKS_CRG_CH1) {
        hal_clocks_config_bit(CMU_CLOCKS_MSUB_NORMAL_REG, true, (REG16_POS)pos);
    } else {
        hal_clocks_config_bit(CMU_CLOCKS_MSUB_NORMAL_REG, false, (REG16_POS)pos);
    }
    reg16_clrbit(MSUB_CFG_REFRESH_REG, MSUB_CFG_REFRESH_BIT);
}

void hal_clocks_glb_clken_config(hal_clocks_ccrg_module_t module, switch_type_t on)
{
    uint8_t pos;
    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_PERP_LS:
            pos = HAL_GLB_CLKEN_MCU_PERP_LS;
            break;
        case HAL_CLOCKS_MODULE_MEM_BUS:
            pos = HAL_GLB_CLKEN_MEM_BUS;
            break;
        case HAL_CLOCKS_MODULE_XIP_OPI:
            pos = HAL_GLB_CLKEN_XIP_OPI;
            break;
        case HAL_CLOCKS_MODULE_XIP_QSPI:
            pos = HAL_GLB_CLKEN_XIP_QSPI;
            break;
        case HAL_CLOCKS_MODULE_MCU_PERP_UART:
            pos = HAL_GLB_CLKEN_MCU_PERP_UART;
            break;
        case HAL_CLOCKS_MODULE_MCU_PERP_SPI:
            pos = HAL_GLB_CLKEN_MCU_PERP_SPI;
            break;
        case HAL_CLOCKS_MODULE_USB_PHY_BUS:
            hal_clocks_config_bit(CMU_CLOCKS_GLB_CLKEN_REG, on, (REG16_POS)HAL_GLB_CLKEN_USB_PHY);
            hal_clocks_config_bit(CMU_CLOCKS_GLB_CLKEN_REG, on, (REG16_POS)HAL_GLB_CLKEN_USB_BUS);
            return;
        case HAL_CLOCKS_MODULE_PDM:
            pos = HAL_GLB_CLKEN_PDM;
            break;
        default:
            return;
    }

    hal_clocks_config_bit(CMU_CLOCKS_GLB_CLKEN_REG, on, (REG16_POS)pos);
}

void hal_clocks_module_auto_cg_control_private(hal_clocks_module_auto_cg_t auto_cg_module, switch_type_t auto_en)
{
    UNUSED(auto_cg_module);
    UNUSED(auto_en);
}

bool hal_clocks_is_dual_channel_module(hal_clocks_ccrg_module_t module)
{
    if (module == HAL_CLOCKS_MODULE_MCU_CORE || module == HAL_CLOCKS_MODULE_COM_BUS) {
        return true;
    }
    return false;
}