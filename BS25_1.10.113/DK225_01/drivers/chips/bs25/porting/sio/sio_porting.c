/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides sio porting template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-01， Create file. \n
 */
#include "common_def.h"
#include "hal_sio_v150.h"
#include "oal_interface.h"
#include "soc_osal.h"
#include "interrupt.h"
#include "pinctrl_porting.h"
#include "pinctrl.h"
#include "pm_clock.h"
#include "pm_lpc.h"
#include "dma_porting.h"
#include "sio_porting.h"

#define SIO_BUS_0_BASE_ADDR          0x5203003c

static uintptr_t g_sio_base_addrs[I2S_MAX_NUMBER] = {
    (uintptr_t)SIO_BUS_0_BASE_ADDR,
};

typedef struct sio_interrupt {
    core_irq_t irq_num;
    osal_irq_handler irq_func;
}sio_interrupt_t;

static const sio_interrupt_t g_sio_interrupt_lines[I2S_MAX_NUMBER] = {
    { I2S_IRQN, (osal_irq_handler)irq_sio0_handler },
};

uintptr_t sio_porting_base_addr_get(sio_bus_t bus)
{
    return g_sio_base_addrs[bus];
}

void sio_porting_register_hal_funcs(sio_bus_t bus)
{
    hal_sio_register_funcs(bus, hal_sio_v150_funcs_get());
}

void sio_porting_unregister_hal_funcs(sio_bus_t bus)
{
    hal_sio_unregister_funcs(bus);
}

void sio_porting_register_irq(sio_bus_t bus)
{
    osal_irq_request(g_sio_interrupt_lines[bus].irq_num, g_sio_interrupt_lines[bus].irq_func, NULL, NULL, NULL);
    osal_irq_enable(g_sio_interrupt_lines[bus].irq_num);
}

void sio_porting_unregister_irq(sio_bus_t bus)
{
    osal_irq_disable(g_sio_interrupt_lines[bus].irq_num);
}

void irq_sio0_handler(void)
{
    hal_sio_v150_irq_handler(SIO_BUS_0);
    osal_irq_clear(I2S_IRQN);
}

static void sio_porting_i2s_clk_sel(void)
{
    uapi_reg_write(M_CTL_RB_BASE + HAL_CFG_I2S_CLK_SEL, 0);
}

static void sio_porting_master_mode_sel(bool enable)
{
    if (enable) {
        uapi_reg_setbit(M_CTL_RB_BASE + HAL_CFG_I2S_MS_CFG, 0);
    } else {
        uapi_reg_clrbit(M_CTL_RB_BASE + HAL_CFG_I2S_MS_CFG, 0);
    }
}

void sio_porting_clock_enable(bool enable)
{
    if (enable) {
        pm_lpc_ccrg_clk_pdm_enable(LPC_CCRG_CLK_PDM_USED_I2S, true);
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_ENABLE, CLOCK_APERP_I2S_CLKEN);
    } else {
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_I2S_CLKEN);
        pm_lpc_ccrg_clk_pdm_enable(LPC_CCRG_CLK_PDM_USED_I2S, false);
    }
}

void sio_porting_bclk_clock_enable(bool enable)
{
    if (enable) {
        uapi_reg_setbit(M_CTL_RB_BASE + HAL_CFG_I2S_BCLK_DIV_EN, CFG_I2S_BCLK_DIV_EN);
    } else {
        uapi_reg_clrbit(M_CTL_RB_BASE + HAL_CFG_I2S_BCLK_DIV_EN, CFG_I2S_BCLK_DIV_EN);
    }
}

static void sio_porting_i2s_set_fs(uint8_t data_width, uint8_t ch)
{
    uapi_reg_clrbit(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_EN, 0);
    uapi_reg_setbits((unsigned int)(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_NUM), 0,
                     I2S_FS_DIV_NUM_CONFIG_BITS, (unsigned int)(ch * data_width));
    uapi_reg_setbits(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_RATIO_NUM, 0, I2S_FS_DIV_RATIO_NUM_BITS,
                     (unsigned int)((ch * data_width) / I2S_DUTY_CYCLE));
    uapi_reg_setbit(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_EN, 0);
    uapi_reg_clrbit(M_CTL_RB_BASE + HAL_CFG_I2S_BCLK_DIV_EN, CFG_I2S_BCLK_DIV_EN);
    uapi_reg_setbit(M_CTL_RB_BASE + HAL_CFG_I2S_BCLK_DIV_EN, CFG_I2S_BCLK_LOAD_DIV_EN);
#if defined(CONFIG_SAMPLE_SUPPORT_I2S_DMA_MASTER)
    uapi_reg_write(M_CTL_RB_BASE + HAL_CFG_I2S_BCLK_DIV_NUM, CONFIG_SAMPLE_I2S_DMA_BCLK_DIV_NUM);
#else
    uapi_reg_write(M_CTL_RB_BASE + HAL_CFG_I2S_BCLK_DIV_NUM, CONFIG_I2S_BCLK_DIV_NUM);
#endif
    uapi_reg_setbit(M_CTL_RB_BASE + HAL_CFG_I2S_BCLK_DIV_EN, CFG_I2S_BCLK_DIV_EN);
}

static void sio_porting_pcm_set_fs(uint8_t data_width, uint8_t ch)
{
    sio_porting_clock_enable(false);
    uapi_reg_clrbit(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_EN, 0);
    uapi_reg_setbits((unsigned int)(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_NUM), 0,
                     I2S_FS_DIV_NUM_CONFIG_BITS, (unsigned int)(ch * data_width));
    uapi_reg_setbits(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_RATIO_NUM, 0, I2S_FS_DIV_RATIO_NUM_BITS, PCM_DIV_RATIO_NUM);
    uapi_reg_setbit(M_CTL_RB_BASE + HAL_CFG_I2S_FS_DIV_EN, 0);
    sio_porting_clock_enable(true);
}

void sio_porting_i2s_drive_mode(uint8_t drive_mode, uint8_t data_width, uint8_t number_of_channels)
{
    sio_porting_i2s_clk_sel();
    if (drive_mode == 0) {
        sio_porting_master_mode_sel(false);
        return;
    }

    sio_porting_master_mode_sel(true);
    sio_porting_i2s_set_fs(data_width, number_of_channels);
}

void sio_porting_i2s_gpio_mode(uint8_t drive_mode)
{
    if (drive_mode == 1) {
        uapi_reg_write(M_CTL_RB_BASE + HAL_CFG_I2S_GPIO_OEN, I2S_GPIO_OUTPUT);
    } else {
        uapi_reg_write(M_CTL_RB_BASE + HAL_CFG_I2S_GPIO_OEN, I2S_GPIO_INPUT);
    }
}

void sio_porting_pcm_drive_mode(uint8_t drive_mode, uint8_t data_width, uint8_t number_of_channels)
{
    if (drive_mode == 0) {
        sio_porting_master_mode_sel(false);
        return;
    }
    sio_porting_master_mode_sel(true);
    sio_porting_pcm_set_fs(data_width, number_of_channels);
}

void sio_porting_i2s_pinmux(void)
{
    uapi_pin_set_mode(S_AGPIO1, HAL_PIO_FUNC_I2S0_M1);
    uapi_pin_set_mode(S_AGPIO2, HAL_PIO_FUNC_I2S0_M1);
    uapi_pin_set_mode(S_AGPIO3, HAL_PIO_FUNC_I2S0_M1);
    uapi_pin_set_mode(S_AGPIO4, HAL_PIO_FUNC_I2S0_M1);
}

#if defined(CONFIG_I2S_SUPPORT_DMA)
#define I2S_BUS_0_REAL_BASE             (I2S_BUS_0_BASE_ADDR - 0x3c)
#define I2S_MERGE_TX_DATA_ADDR          (I2S_BUS_0_REAL_BASE + 0xc0)
#define I2S_MERGE_RX_DATA_ADDR          (I2S_BUS_0_REAL_BASE + 0xa0)
#define I2S_TX_SPLIT_LEFT_DATA_ADDR     (I2S_BUS_0_REAL_BASE + 0x4c)
#define I2S_TX_SPLIT_RIGHT_DATA_ADDR    (I2S_BUS_0_REAL_BASE + 0x50)
#define I2S_RX_SPLIT_LEFT_DATA_ADDR     (I2S_BUS_0_REAL_BASE + 0x54)
#define I2S_RX_SPLIT_RIGHT_DATA_ADDR    (I2S_BUS_0_REAL_BASE + 0x58)
uint32_t i2s_port_get_dma_trans_src_handshaking(sio_bus_t bus)
{
    unused(bus);
    return HAL_DMA_HANDSHAKING_I2S_RX;
}

uint32_t i2s_port_get_dma_trans_dest_handshaking(sio_bus_t bus)
{
    unused(bus);
    return HAL_DMA_HANDSHAKING_I2S_TX;
}

uintptr_t i2s_porting_tx_merge_data_addr_get(sio_bus_t bus)
{
    unused(bus);
    return I2S_MERGE_TX_DATA_ADDR;
}

uintptr_t i2s_porting_rx_merge_data_addr_get(sio_bus_t bus)
{
    unused(bus);
    return I2S_MERGE_RX_DATA_ADDR;
}
#endif