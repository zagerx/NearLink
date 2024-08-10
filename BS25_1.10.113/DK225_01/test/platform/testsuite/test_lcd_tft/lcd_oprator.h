/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd tft oprator write reg or data \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-18， Create file. \n
 */

#ifndef LCD_SPI_H
#define LCD_SPI_H

#include "stdint.h"

typedef struct {
    uint16_t width;   // LCD 宽度
    uint16_t height;  // LCD 高度
    uint16_t id;      // LCD ID
    uint8_t  dir;      // 横屏还是竖屏控制：0，竖屏；1，横屏。
    uint16_t wramcmd; // 开始写gram指令
    uint16_t setxcmd; // 设置x坐标指令
    uint16_t setycmd; // 设置y坐标指令
} lcd_dev_t;

extern lcd_dev_t g_lcd_device;

void lcd_spi_init(void);

void lcd_reset(void);

void lcd_write_data_16bit(uint16_t data);
void lcd_write_data(uint8_t data);
void lcd_write_datas(const uint8_t *data, uint32_t length);
void lcd_write_reg(uint8_t data);
void lcd_write_reg_value(uint8_t reg, uint8_t value);
void lcd_read_rddid(void);
void lcd_spi_write_by_dma(uint8_t *data, uint32_t length);
#endif