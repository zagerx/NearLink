/*
 * Copyright (c) @CompanyNameMagicTag 2020-2023. All rights reserved.
 * Description:   HAL APP CLOCK DRIVER.
 * Author: @CompanyNameTag
 * Create: 2020-01-13
 */
#include "hal_clocks_application.h"
#include "non_os.h"
#include "hal_clocks_glb.h"
#include "hal_reg_config.h"

#define CMU_CLOCKS_GATING_M_CLKSEL              (M_CTL_RB_BASE + 0x68)
#define CMU_CLOCKS_IR_CLK_SELECT                0

#define CMU_CLOCKS_M_DIV3_REG                   (M_CTL_RB_BASE + 0x7C)
#define CMU_CLOCKS_AUX_ADC_DIV                  0
#define CMU_CLOCKS_AUX_ADC_DIV_LEN              8
#define CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN          8
#define CMU_CLOCKS_AUX_ADC_DIV_EN               9

#define HAL_CLOCKS_APPLICATION_TIMEOUT 1000
#define CLOCK_DIV_0     0
#define CLOCK_DIV_1     1

void hal_clocks_mcrg_aux_adc_div_set(uint8_t div)
{
    hal_clocks_config_bit(CMU_CLOCKS_M_DIV3_REG, TURN_ON, (REG16_POS)CMU_CLOCKS_AUX_ADC_DIV_EN);
    reg16_clrbit(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN);
    reg16_setbits(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV, CMU_CLOCKS_AUX_ADC_DIV_LEN, div);
    reg16_setbit(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN);
    reg16_clrbit(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN);
}

uint8_t hal_clocks_mcrg_aux_adc_div_get(void)
{
    return (uint8_t)reg16_getbits(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV, CMU_CLOCKS_AUX_ADC_DIV_LEN);
}

void hal_clocks_bcrg_clk_en(switch_type_t clk_en)
{
    hal_reg_config_bit(CMU_CLOCKS_B_CRG_CLKEN1_REG, clk_en, (REG16_POS)CMU_CLOCKS_B_BUS_CLKEN_BIT);
}

void hal_clocks_ccrg_xo_dll2_clk_en(switch_type_t clk_en)
{
    hal_reg_config_bit(CMU_CLOCKS_COM_DLL2_REG, clk_en, (REG16_POS)CMU_CLOCKS_COM_XO_DLL2_EN_BIT);
}

void hal_clocks_ccrg_mcu_bus_div_set(uint8_t div)
{
    if (div == CLOCK_DIV_0) {
        div = CLOCK_DIV_1;
    }
    non_os_enter_critical();
    reg16_clrbit(CMU_CLOCKS_MCU_BUS_DIV_REG, CMU_CLOCKS_MCU_BUS_LOAD_DIV_EN_BIT);
    reg16_setbits(CMU_CLOCKS_MCU_BUS_DIV_REG, CMU_CLOCKS_MCU_BUS_DIV_NUM_BIT, CMU_CLOCKS_MCU_BUS_DIV_NUM_LEN, div);
    reg16_setbit(CMU_CLOCKS_MCU_BUS_DIV_REG, CMU_CLOCKS_MCU_BUS_LOAD_DIV_EN_BIT);
    reg16_clrbit(CMU_CLOCKS_MCU_BUS_DIV_REG, CMU_CLOCKS_MCU_BUS_LOAD_DIV_EN_BIT);
    non_os_exit_critical();
}

uint8_t hal_clocks_ccrg_mcu_bus_div_get(void)
{
    return reg16_getbits(CMU_CLOCKS_MCU_BUS_DIV_REG, CMU_CLOCKS_MCU_BUS_DIV_NUM_BIT, CMU_CLOCKS_MCU_BUS_DIV_NUM_LEN);
}

void hal_clocks_ccrg_module_ch_enable(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch, switch_type_t ch_en)
{
    uint32_t reg_addr = hal_clocks_ccrg_reg_get(module, ch);
    if (reg_addr != 0) {
        hal_reg_config_bit(reg_addr, ch_en, (REG16_POS)CMU_CLOCKS_COM_CR_CH_EN_BIT);
    }
}

void hal_clocks_ccrg_module_config(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch, \
                                   uint8_t clk_src, uint8_t clk_div)
{
    uint32_t reg_addr = hal_clocks_ccrg_reg_get(module, ch);
    if (reg_addr == 0) {
        return;
    }

    non_os_enter_critical();
    // Disable the clock.
    reg16_clrbit(reg_addr, CMU_CLOCKS_COM_CR_CH_EN_BIT);
    // Select the clock source.
    reg16_setbits(reg_addr, CMU_CLOCKS_COM_CR_CLK_SEL_BIT, CMU_CLOCKS_COM_CR_CLK_SEL_LEN, clk_src);
    // Set the clock divisor.
    reg16_clrbit(reg_addr, CMU_CLOCKS_COM_CR_CLK_DIV_EN_BIT);
    if (clk_div > CLOCK_DIV_1) {
        reg16_setbits(reg_addr, CMU_CLOCKS_COM_CR_CLK_DIV_NUM_BIT, CMU_CLOCKS_COM_CR_CLK_DIV_NUM_LEN, clk_div);
        reg16_setbit(reg_addr, CMU_CLOCKS_COM_CR_CLK_DIV_EN_BIT);
    } else {
        reg16_setbits(reg_addr, CMU_CLOCKS_COM_CR_CLK_DIV_NUM_BIT, CMU_CLOCKS_COM_CR_CLK_DIV_NUM_LEN, CLOCK_DIV_0);
        reg16_clrbit(reg_addr, CMU_CLOCKS_COM_CR_CLK_DIV_EN_BIT);
    }
    // Enable the clock.
    reg16_setbit(reg_addr, CMU_CLOCKS_COM_CR_CH_EN_BIT);
    non_os_exit_critical();
}

void hal_clocks_set_mcpu_soft_trg_pulse_width(uint8_t pulse_width)
{
    non_os_enter_critical();
    reg16_setbits(CMU_CLOCKS_M_SOFT_TRG_REG, CMU_CLOCKS_M_SOFT_RST_PULSE_WIDTH_BIT, \
                  CMU_CLOCKS_M_SOFT_RST_PULSE_WIDTH_LEN, pulse_width);
    non_os_exit_critical();
}

void hal_clocks_mcpu_soft_reset_control(hal_mcpu_soft_rst_type_t hal_mcpu_soft_rst,
                                        hal_soft_rst_control_type_t rst_control)
{
    uint32_t rst_addr;
    uint8_t offset;

    if (hal_mcpu_soft_rst < HAL_MCPU_SOFT_RST_N1_OFFSET) {
        rst_addr = CMU_CLOCKS_M_SOFT_RST_REG;
        offset = (uint8_t)hal_mcpu_soft_rst;
    } else {
        rst_addr = CMU_CLOCKS_M_SOFT_RST1_REG;
        offset = (uint8_t)(hal_mcpu_soft_rst - HAL_MCPU_SOFT_RST_N1_OFFSET);
    }

    hal_clocks_config_bit(rst_addr, (switch_type_t)rst_control, (REG16_POS)offset);
}

void hal_clocks_mem_perips_config(hal_clocks_mem_perips_clk_type_t mem_clk, switch_type_t on)
{
    if ((uint16_t)mem_clk <= (uint16_t)POS_15) {
        hal_clocks_config_bit(CMU_CLOCK_MEM_CLKEN0_REG, on, (REG16_POS)mem_clk);
    } else {
        hal_clocks_config_bit(CMU_CLOCK_MEM_CLKEN1_REG, on, (REG16_POS)(((uint16_t)mem_clk - (uint16_t)POS_15)));
    }
}

void hal_clocks_xip_soft_reset_control(hal_mem_soft_rst_type_t hal_mem_soft_rst,
                                       hal_soft_rst_control_type_t rst_control)
{
    hal_clocks_config_bit(CMU_CLOCK_MEM_SOFT_RST_REG, (switch_type_t)rst_control, (REG16_POS)hal_mem_soft_rst);
}

static void hal_clocks_eh2h_auto_cg_control(hal_clocks_module_auto_cg_t auto_cg_module, switch_type_t auto_en)
{
    uint8_t offset = (uint8_t)(auto_cg_module - HAL_CLOCKS_MODULE_AUTO_CG_EH2H_OPI);
    hal_reg_config_bit(EH2H_AUTO_CG_CFG_REG, auto_en, (REG16_POS)offset);
}

void hal_clocks_module_auto_cg_control(hal_clocks_module_auto_cg_t auto_cg_module, switch_type_t auto_en)
{
    switch (auto_cg_module) {
        case HAL_CLOCKS_MODULE_AUTO_CG_COM_BUS:
            hal_reg_config_bit(COM_BUS_LP_CFG_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_CACHE:
            hal_reg_config_bit(CACHE_BUS_LP_CFG_REG, auto_en, (REG16_POS)CACHE_AUTO_CG_BYPASS_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MEM_AHB:
            hal_reg_config_bit(CACHE_BUS_LP_CFG_REG, auto_en, (REG16_POS)MEM_AHB_AUTO_CG_BYPASS_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_AON:
            hal_reg_config_bit(AON_BUS_LP_CFG_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_BT:
            hal_reg_config_bit(B_BUS_LP_CFG_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MCU_MAIN:
            hal_reg_config_bit(MCU_MAIN_AHB_LP_CLR_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MCU_SDIO_M:
            hal_reg_config_bit(SDIO_M_BUS_LP_CFG_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_SDIO_CFG:
            hal_reg_config_bit(SDIO_CFG_BUS_LP_CFG_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_DISPLAY:
            hal_reg_config_bit(DIS_BUS_LP_CFG_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MCU_CFG:
            hal_reg_config_bit(MCU_CFG_BUS_LP_CFG_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MAIN_AXI:
            hal_reg_config_bit(MAIN_AXI_DEFAULT_SLAVE_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_DISPLAY_AXI:
            hal_reg_config_bit(DIS_AXI_DEFAULT_SLAVE_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MCU_PERP_AXI:
            hal_reg_config_bit(MCU_PERP_AXI_DEFAULT_SLAVE_REG, auto_en, (REG16_POS)CMU_CLOCKS_MODULE_AUTO_CG_BIT);
            return;
        default:
            break;
    }
    hal_clocks_eh2h_auto_cg_control(auto_cg_module, auto_en);
    hal_clocks_module_auto_cg_control_private(auto_cg_module, auto_en);
}

void hal_clocks_bridging_exit_lp_config(switch_type_t on)
{
    hal_reg_config_bit(M_AXI_BRG_LP_CFG0_REG, on, (REG16_POS)MAIN_AXI_S1_BIT);
    hal_reg_config_bit(M_AXI_BRG_LP_CFG0_REG, on, (REG16_POS)MAIN_AXI_S2_BIT);

    hal_reg_config_bit(M_AXI_BRG_LP_CFG1_REG, on, (REG16_POS)SDIO_M_AH2X_BIT);
    hal_reg_config_bit(M_AXI_BRG_LP_CFG1_REG, on, (REG16_POS)MCPU_X2X_BIT);

    hal_reg_config_bit(AUDIO_BRG_LP_CFG_REG, on, (REG16_POS)COM_AHB_C2D_BIT);
    hal_reg_config_bit(AUDIO_BRG_LP_CFG_REG, on, (REG16_POS)COM_AHB_D2C_BIT);
}

void hal_clocks_aperp_32k_sel_manual_config(switch_type_t manual_en)
{
    hal_reg_config_bit(PMU1_APERP_32K_SEL_MAN_REG, manual_en, (REG16_POS)PMU1_APERP_32K_SEL_BIT);
}

void hal_clocks_aperp_32k_sel_control(switch_type_t sel_en)
{
    hal_reg_config_bit(PMU1_APERP_32K_SEL_REG, sel_en, (REG16_POS)PMU1_APERP_32K_SEL_BIT);
}

void hal_clocks_pll_module_clken(hal_clk_pll_t pll_module, switch_type_t clk_en)
{
    uint32_t addr;

    switch (pll_module) {
        case HAL_CLK_MCU_PLL:
            addr = PMU2_SCPLL_MCU_CLKEN_REG;
            break;
        case HAL_CLK_LP_PLL:
            addr = PMU2_LPPLL_CLKEN_REG;
            break;
        default:
            return;
    }

    hal_reg_config_bit(addr, clk_en, (REG16_POS)PMU2_PLL_CLKEN_BIT);
}


void hal_mcu_group_clock_switch(hal_clocks_ccrg_module_t module,
                                hal_clocks_ccrg_mcu_group_clk_src_t clk_source,
                                uint8_t div)
{
    if (module >= HAL_CLOCKS_MODULE_MAX) {
        return;
    }
    non_os_enter_critical();
    if (clk_source == HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX) {
        // Disable the clock.
        hal_clocks_glb_clken_config(module, TURN_OFF);
        if (module >= HAL_CLOCKS_MODULE_PAD_OUT0) {
            hal_clocks_ccrg_module_ch_enable(module, HAL_CLOCKS_CRG_CH_INVALID, TURN_OFF);
        }
        non_os_exit_critical();
        return;
    }
    if (hal_clocks_is_dual_channel_module(module)) {
        // Switch ch0.
        hal_clocks_sub_normal_sel_cfg(module, HAL_CLOCKS_CRG_CH0);
        // Config the clock.
        hal_clocks_ccrg_module_config(module, HAL_CLOCKS_CRG_CH1, (uint8_t)clk_source, div);
        // Switch ch1.
        hal_clocks_sub_normal_sel_cfg(module, HAL_CLOCKS_CRG_CH1);
    } else {
        // Disable the clock.
        hal_clocks_glb_clken_config(module, TURN_OFF);
        // Config the clock.
        hal_clocks_ccrg_module_config(module, HAL_CLOCKS_CRG_CH_INVALID, (uint8_t)clk_source, div);
    }
    // Enable the clock.
    hal_clocks_glb_clken_config(module, TURN_ON);
    non_os_exit_critical();
}