/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd tft oprator, write reg / date \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-18， Create file. \n
 */

#include "pinctrl.h"
#include "gpio.h"
#include "spi.h"
#include "spi_porting.h"
#include "tcxo.h"
#include "lcd_port.h"
#include "lcd_tft.h"
#include "memory_config_common.h"

#include "lcd_oprator.h"

#define SPI_FREQUENCY_MHZ  16

#define get_data_high_8bit(data) ((data) >> 8)
#define get_data_low_8bit(data)  ((data) & 0xff)
#define LENGTH_16BIT                2
#define RESET_DELAY_US              50
#define RESET_DELAY_US2             100
#define SPI_WRITE_TIMEOUT           0xffffffff  // no timeout
#define SPI_READ_TIMEOUT            0xffff
#define SEND_DATA_ONE_TIME          12800
#define DMA_SRC_4B_WITH             2
#define DMA_BURST_LENGTH_4          1

lcd_dev_t g_lcd_device;
void lcd_spi_write(uint8_t *data, uint32_t length)
{
    spi_xfer_data_t mouse_send_xfer = { 0 };
    mouse_send_xfer.tx_buff = data;
    mouse_send_xfer.tx_bytes = length;
#if !defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH)
#ifdef CONFIG_SPI_SUPPORT_DMA
    spi_dma_config_t dma_cfg = {0};
    uapi_spi_set_dma_mode(LCD_SPI, 0, &dma_cfg);
#endif
#endif  /* NOT CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */
    uapi_spi_master_write(LCD_SPI, &mouse_send_xfer, SPI_WRITE_TIMEOUT);
}

void lcd_spi_write_by_dma(uint8_t *data, uint32_t length)
{
    spi_xfer_data_t mouse_send_xfer = { 0 };
    mouse_send_xfer.tx_buff = data;
    mouse_send_xfer.tx_bytes = length;
#if !defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH)
#ifdef CONFIG_SPI_SUPPORT_DMA
    spi_dma_config_t dma_cfg = {
        .burst_length = DMA_BURST_LENGTH_4,
        .src_width = DMA_SRC_4B_WITH,
        .dest_width = 0,
        .priority = 0
    };
    uapi_spi_set_dma_mode(LCD_SPI, true, &dma_cfg);
#endif
#endif  /* NOT CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */
    uint32_t i = 0;
    for (i = 0; i < (uint32_t)(length / SEND_DATA_ONE_TIME); i++) {
        mouse_send_xfer.tx_buff = &data[i * SEND_DATA_ONE_TIME];
        mouse_send_xfer.tx_bytes = SEND_DATA_ONE_TIME;
        uapi_spi_master_write(LCD_SPI, &mouse_send_xfer, SPI_WRITE_TIMEOUT);
    }
    if (i * SEND_DATA_ONE_TIME < length) {
        mouse_send_xfer.tx_buff = &data[i * SEND_DATA_ONE_TIME];
        mouse_send_xfer.tx_bytes = length - i * SEND_DATA_ONE_TIME;
        uapi_spi_master_write(LCD_SPI, &mouse_send_xfer, SPI_WRITE_TIMEOUT);
    }
}

void lcd_spi_pinmux_init(void)
{
    uapi_pin_set_mode(SPI_PIN_MISO, SPI_PIN_MISO_PINMUX);
    uapi_pin_set_mode(SPI_PIN_MOSI, SPI_PIN_MOSI_PINMUX);
    uapi_pin_set_mode(SPI_PIN_CLK, SPI_PIN_CLK_PINMUX);
    uapi_pin_set_mode(SPI_PIN_CS, SPI_PIN_CS_PINMUX);
}

void lcd_spi_init(void)
{
    spi_attr_t config = { 0 };
    spi_extra_attr_t ext_config = { 0 };
    config.freq_mhz = SPI_FREQUENCY_MHZ;
    config.is_slave = false;
    config.frame_size = HAL_SPI_FRAME_SIZE_8;
    config.slave_num = 1;
    config.spi_frame_format = HAL_SPI_FRAME_FORMAT_STANDARD;
    config.bus_clk = SPI_CLK_FREQ;
    config.frame_format = 0;
    config.tmod = 0;
    config.sste = 0;
    lcd_spi_pinmux_init();
    errcode_t err = uapi_spi_init(LCD_SPI, &config, &ext_config);
    spi_porting_set_tx_mode(LCD_SPI);
    uapi_dma_init();
    uapi_dma_open();
    print("Open the spi 0x%x\r\n", err);
}

void lcd_write_data(uint8_t data)
{
    uapi_gpio_set_val(LCD_DC, GPIO_LEVEL_HIGH);
    lcd_spi_write(&data, 1);
}

void lcd_write_datas(const uint8_t *data, uint32_t length)
{
    uint32_t data_addr = (uint32_t)data;
    if (data_addr > FLASH_START || data_addr > EMBED_FLASH_START) {
        data_addr = dma_remap(data_addr);
    }
    uapi_gpio_set_val(LCD_DC, GPIO_LEVEL_HIGH);
    lcd_spi_write_by_dma((uint8_t *)(uintptr_t)data_addr, length);
}

void lcd_write_data_16bit(uint16_t data)
{
    uint8_t temp[LENGTH_16BIT];
    temp[0] = get_data_high_8bit(data);
    temp[1] = get_data_low_8bit(data);
    uapi_gpio_set_val(LCD_DC, GPIO_LEVEL_HIGH);
    lcd_spi_write(temp, LENGTH_16BIT);
}

void lcd_write_reg(uint8_t reg)
{
    uapi_gpio_set_val(LCD_DC, GPIO_LEVEL_LOW);
    lcd_spi_write(&reg, 1);
}

void lcd_write_reg_value(uint8_t reg, uint8_t value)
{
    lcd_write_reg(reg);
    lcd_write_data(value);
}

void lcd_reset(void)
{
    uapi_gpio_set_val(LCD_RESERT, GPIO_LEVEL_HIGH);
    uapi_tcxo_delay_ms(RESET_DELAY_US);
    uapi_gpio_set_val(LCD_RESERT, GPIO_LEVEL_LOW);
    uapi_tcxo_delay_ms(RESET_DELAY_US2);
    uapi_gpio_set_val(LCD_RESERT, GPIO_LEVEL_HIGH);
    uapi_tcxo_delay_ms(RESET_DELAY_US);
}

void lcd_read(uint8_t read_reg, uint8_t *data, uint8_t length)
{
    lcd_write_reg(read_reg);
    uapi_pin_set_mode(SPI_PIN_MOSI, 0);
    uapi_gpio_set_dir(SPI_PIN_MOSI, GPIO_DIRECTION_INPUT);
    spi_porting_set_rx_mode(LCD_SPI, length);
    spi_xfer_data_t mouse_send_xfer = { 0 };
    mouse_send_xfer.rx_buff = data;
    mouse_send_xfer.rx_bytes = length;
    uapi_spi_master_read(LCD_SPI, &mouse_send_xfer, SPI_READ_TIMEOUT);
}
