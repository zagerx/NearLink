/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides sio port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-15， Create file. \n
 */
#ifndef SIO_PORTING_H
#define SIO_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_core.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_sio SIO
 * @ingroup  drivers_port
 * @{
 */

#define I2S_FIFO_SIZE                   16
#define HAL_CFG_I2S_BCLK_DIV_EN         0x1000
#define HAL_CFG_I2S_BCLK_DIV_NUM        0x1004
#define HAL_CFG_I2S_CLK_SEL             0x1014
#define HAL_CFG_I2S_MS_CFG              0x1024
#define HAL_CFG_I2S_FS_DIV_EN           0x1008
#define HAL_CFG_I2S_FS_DIV_NUM          0x100c
#define HAL_CFG_I2S_FS_DIV_RATIO_NUM    0x1010
#define HAL_CFG_I2S_GPIO_OEN            0x101c
#define I2S_GPIO_INPUT                  0x7
#define I2S_GPIO_OUTPUT                 0x0
#define I2S_CLKEN_BIT                   2
#define I2S_DUTY_CYCLE                  2
#define PCM_DIV_RATIO_NUM               1
#define CFG_I2S_BCLK_DIV_EN             0
#define CFG_I2S_BCLK_DIV_NUM            4
#define CFG_I2S_BCLK_LOAD_DIV_EN        1
#define I2S_FS_DIV_NUM_CONFIG_BITS      11
#define I2S_FS_DIV_RATIO_NUM_BITS       11
#define I2S_READ_WAIT_TIMES_MAX         8
#define CONFIG_PCM_RX_THRESHOLD         0xc
#define CONFIG_PCM_TX_THRESHOLD         0xc
#define CONFIG_I2S_BUS_MAX_NUM          1
#define CONFIG_I2S_SUPPORT_LOOPBACK     1

/**
 * @brief  Get the base address of a specified sio.
 * @param  [in]  bus PCM device to use.
 * @return The base address of specified sio.
 */
uintptr_t sio_porting_base_addr_get(sio_bus_t bus);

/**
 * @brief  Register hal funcs objects into hal_sio module.
 * @param  [in]  bus PCM device to use.
 */
void sio_porting_register_hal_funcs(sio_bus_t bus);

/**
 * @brief  Unregister hal funcs objects from hal_sio module.
 * @param  [in]  bus PCM device to use.
 */
void sio_porting_unregister_hal_funcs(sio_bus_t bus);

/**
 * @brief  Register the interrupt of sio.
 * @param  [in]  bus PCM device to use.
 */
void sio_porting_register_irq(sio_bus_t bus);

/**
 * @brief  Unregister the interrupt of sio.
 * @param  [in]  bus PCM device to use.
 */
void sio_porting_unregister_irq(sio_bus_t bus);

/**
 * @brief  The interrupt handler of sio0.
 */
void irq_sio0_handler(void);

/**
 * @brief  SIO clock.
 * @param  [in]  enable Enable or not.
 */
void sio_porting_clock_enable(bool enable);

/**
 * @brief  SIO bclk clock.
 * @param  [in]  enable Enable or not.
 */
void sio_porting_bclk_clock_enable(bool enable);

/**
 * @brief  Set i2s fs clock.
 * @param  [in]  drive_mode Drive mode.
 * @param  [in]  data_width Data width.
 * @param  [in]  number_of_channels Channel number.
 */
void sio_porting_i2s_drive_mode(uint8_t drive_mode, uint8_t data_width, uint8_t number_of_channels);

/**
 * @brief  Set pcm fs clock.
 * @param  [in]  drive_mode Drive mode.
 * @param  [in]  data_width Data width.
 * @param  [in]  number_of_channels Channel number.
 */
void sio_porting_pcm_drive_mode(uint8_t drive_mode, uint8_t data_width, uint8_t number_of_channels);

/**
 * @brief  Set i2s gpio mode.
 * @param  [in]  drive_mode Drive mode.
 */
void sio_porting_i2s_gpio_mode(uint8_t drive_mode);

/**
 * @brief  Config the pin mode.
 */
void sio_porting_i2s_pinmux(void);

#if defined(CONFIG_I2S_SUPPORT_DMA)
/**
 * @brief  Get the src handshaking of dma.
 * @param  [in]  bus PCM/I2S device to use.
 */
uint32_t i2s_port_get_dma_trans_src_handshaking(sio_bus_t bus);

/**
 * @brief  Get the dest handshaking of dma.
 * @param  [in]  bus PCM/I2S device to use.
 */
uint32_t i2s_port_get_dma_trans_dest_handshaking(sio_bus_t bus);

/**
 * @brief  Get the tx merge addr of I2S.
 * @param  [in]  bus PCM/I2S device to use.
 */
uintptr_t i2s_porting_tx_merge_data_addr_get(sio_bus_t bus);

/**
 * @brief  Get the rx merge addr of I2S.
 * @param  [in]  bus PCM/I2S device to use.
 */
uintptr_t i2s_porting_rx_merge_data_addr_get(sio_bus_t bus);

#endif

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif