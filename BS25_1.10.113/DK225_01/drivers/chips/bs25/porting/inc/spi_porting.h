/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides spi porting template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-18， Create file. \n
 */
#ifndef SPI_PORTING_H
#define SPI_PORTING_H

#include <stdint.h>
#include "platform_core.h"
#include "hal_spi_v100_regs_def.h"
#include "dma_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_spi SPI
 * @ingroup  drivers_port
 * @{
 */

#define SPI_CLK_FREQ        16000000

#define SPI_BUS_MAX_NUM SPI_BUS_MAX_NUMBER
#define CONFIG_SPI_MAX_TIMEOUT 0xFFFFFFFF

#define DMA_HANDSHAKE_SPI_BUS_0_TX HAL_DMA_HANDSHAKING_SPI_M_TX
#define DMA_HANDSHAKE_SPI_BUS_0_RX HAL_DMA_HANDSHAKING_SPI_M_RX
#define DMA_HANDSHAKE_SPI_BUS_1_TX HAL_DMA_HANDSHAKING_SPI_MS0_TX
#define DMA_HANDSHAKE_SPI_BUS_1_RX HAL_DMA_HANDSHAKING_SPI_MS0_RX
#define DMA_HANDSHAKE_SPI_BUS_2_TX HAL_DMA_HANDSHAKING_SPI_MS1_TX
#define DMA_HANDSHAKE_SPI_BUS_2_RX HAL_DMA_HANDSHAKING_SPI_MS1_RX
#define DMA_HANDSHAKE_SPI_BUS_3_TX HAL_DMA_HANDSHAKING_SPI3_M_TX
#define DMA_HANDSHAKE_SPI_BUS_3_RX HAL_DMA_HANDSHAKING_SPI3_M_RX
#define DMA_HANDSHAKE_SPI_BUS_4_RX HAL_DMA_HANDSHAKING_SPI4_S_RX
#define DMA_HANDSHAKE_SPI_BUS_5_TX HAL_DMA_HANDSHAKING_QSPI0_2CS_TX
#define DMA_HANDSHAKE_SPI_BUS_5_RX HAL_DMA_HANDSHAKING_QSPI0_2CS_RX
#define DMA_HANDSHAKE_SPI_BUS_6_TX HAL_DMA_HANDSHAKING_QSPI1_2CS_TX
#define DMA_HANDSHAKE_SPI_BUS_6_RX HAL_DMA_HANDSHAKING_QSPI1_2CS_RX

/**
 * @brief  Spi dma control register.
 */
typedef enum {
    HAL_SPI_DMA_CONTROL_DISABLE = 0,        //!< Disables the transmit fifo and the receive fifo dma channel.
    HAL_SPI_DMA_CONTROL_RX_ENABLE = 1,      //!< Enables the receive fifo dma channel.
    HAL_SPI_DMA_CONTROL_TX_ENABLE = 2,      //!< Enables the transmit fifo dma channel.
    HAL_SPI_DMA_CONTROL_TXRX_ENABLE = 3,    //!< Enables the transmit fifo and the receive fifo dma channel.
    HAL_SPI_DMA_CONTROL_MAX_NUM,
    HAL_SPI_DMA_CONTROL_NONE = HAL_SPI_DMA_CONTROL_MAX_NUM,
} hal_spi_dma_control_t;

/**
 * @brief  SPI mode.
 */
typedef enum spi_mode {
    SPI_MODE_MASTER,        /*!< SPI Master mode. */
    SPI_MODE_SLAVE,         /*!< SPI Slave mode. */
    SPI_MODE_MAX_NUM,
    SPI_MODE_NONE = SPI_MODE_MAX_NUM
} spi_mode_t;

/**
 * @brief  SPI slave select.
 */
typedef enum spi_slave {
    SPI_SLAVE0 = 0,         /*!< SPI Slave index 0. */
    SPI_SLAVE1,             /*!< SPI Slave index 1. */
    SPI_SLAVE_MAX_NUM,
    SPI_SLAVE_NONE = SPI_SLAVE_MAX_NUM
} spi_slave_t;

/**
 * @brief  Base address list for all of the IPs.
 */
extern ssi_regs_t * g_spi_base_addrs[SPI_BUS_MAX_NUM];

/**
 * @brief  Get the base address of specified spi.
 * @param  [in]  bus The bus index of SPI.
 * @return The base address of specified spi.
 */
uintptr_t spi_porting_base_addr_get(spi_bus_t bus);

/**
 * @brief  unRegister the interrupt of spis.
 * @param  [in]  bus The bus index of SPI.
 */
void spi_port_unregister_irq(spi_bus_t bus);

/**
 * @brief  Register the interrupt of spis.
 * @param  [in]  bus The bus index of SPI.
 */
void spi_port_register_irq(spi_bus_t bus);

/**
 * @brief  Get the max slave number can be selected.
 * @param  [in]  bus The bus index of SPI.
 * @return The  max slave number can be selected.
 */
uint32_t spi_porting_max_slave_select_get(spi_bus_t bus);

/**
 * @brief  Set the spi work mode.
 * @param  [in]  bus The bus index of SPI.
 * @param  [in]  mode The mode of SPI.
 */
void spi_porting_set_device_mode(spi_bus_t bus, spi_mode_t mode);

/**
 * @brief  Get the spi work mode.
 * @param  [in]  bus The bus index of SPI.
 * @return The mode of SPI.
 */
spi_mode_t spi_porting_get_device_mode(spi_bus_t bus);

/**
 * @brief  Init spi pin for test.
 */
void spi_porting_test_spi_init_pin(void);

/**
 * @brief  Set the spi recieve data number.
 * @param  [in]  bus The bus index of SPI.
 * @param  [in]  num recieve data number of SPI.
 * @return The mode of SPI.
 */
void spi_porting_set_rx_mode(spi_bus_t bus, uint16_t num);

/**
 * @brief  Set the spi mode tx.
 * @param  [in]  bus The bus index of SPI.
 * @return The mode of SPI.
 */
void spi_porting_set_tx_mode(spi_bus_t bus);

/**
 * @brief  SPI lock.
 * @param  [in]  bus The bus index of SPI.
 * @return The irq lock number of SPI.
 */
uint32_t spi_porting_lock(spi_bus_t bus);

/**
 * @brief  SPI unlock.
 * @param  [in]  bus The bus index of SPI.
 * @param  [in]  irq_sts The irq lock number of SPI.
 */
void spi_porting_unlock(spi_bus_t bus, uint32_t irq_sts);

/**
 * @brief  Get the DMA destination handshaking select of SPI transfer.
 * @param  [in]  bus The SPI bus. see @ref spi_bus_t
 * @return The DMA destination handshaking select of spi transfer.
 */
uint8_t spi_port_get_dma_trans_dest_handshaking(spi_bus_t bus);

/**
 * @brief  Get the DMA source handshaking select of SPI transfer.
 * @param  [in]  bus The SPI bus. see @ref spi_bus_t
 * @return The DMA source handshaking select of SPI transfer.
 */
uint8_t spi_port_get_dma_trans_src_handshaking(spi_bus_t bus);

/**
 * @brief  Get the SPI DMA TX level.
 * @param  [in]  bus The SPI bus. see @ref spi_bus_t
 * @return The SPI DMA TX level.
 */
uint8_t spi_port_tx_data_level_get(spi_bus_t bus);

/**
 * @brief  Get the SPI DMA RX level of SPI transfer.
 * @param  [in]  bus The SPI bus. see @ref spi_bus_t
 * @return The SPI DMA RX level.
 */
uint8_t spi_port_rx_data_level_get(spi_bus_t bus);

void spi_funcreg_adapt(spi_bus_t bus);

/**
 * @brief  SPI clock enable or disable.
 * @param  [in]  bus The bus index of I2C.
 * @param  [in]  on Enable or disable.
 */
void spi_port_clock_enable(spi_bus_t bus, bool on);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif