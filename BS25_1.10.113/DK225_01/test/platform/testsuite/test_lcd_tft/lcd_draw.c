/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd tft draw, draw image or rectangle \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-18， Create file. \n
 */

#include "lcd_draw.h"
#include "osal_addr.h"
#include "securec.h"
#include "lcd_oprator.h"
#include "lcd_tft.h"

uint16_t g_point_color = 0x0000;
uint16_t g_back_color = 0xFFFF;

#define LCD_W   160
#define LCD_H   80
#define LCD_YDIM_OFFSET  26

#ifndef bit
#define bit(n)  (1 << (n))
#endif
#define RGB     bit(3)
#define MV      bit(5)
#define MX      bit(6)
#define MY      bit(7)

#define LCD_CLEAR_MEM_SIZE   160

void lcd_set_deriction(uint8_t direction)
{
    g_lcd_device.setxcmd = 0x2A;
    g_lcd_device.setycmd = 0x2B;
    g_lcd_device.wramcmd = 0x2C;
    switch (direction) {
        case 0:
            g_lcd_device.width = LCD_W;
            g_lcd_device.height = LCD_H;
            lcd_write_reg_value(0x36, RGB | MY | MV); // BGR==1,MY==0,MX==0,MV==0
            break;
        case 1:
            g_lcd_device.width = LCD_H;
            g_lcd_device.height = LCD_W;
            lcd_write_reg_value(0x36, RGB | MY | MV); // BGR==1,MY==1,MX==0,MV==1
            break;
        default:
            break;
    }
}

void lcd_write_ram_propare(void)
{
    lcd_write_reg(g_lcd_device.wramcmd);
}

void lcd_set_windows(uint16_t x_star, uint16_t y_star, uint16_t x_end, uint16_t y_end)
{
    lcd_write_reg(g_lcd_device.setxcmd);
    lcd_write_data(0x00);
    lcd_write_data(x_star + 1);
    lcd_write_data(0x00);
    lcd_write_data(x_end + 1);

    lcd_write_reg(g_lcd_device.setycmd);
    lcd_write_data(0x00);
    lcd_write_data(y_star + LCD_YDIM_OFFSET);
    lcd_write_data(0x00);
    lcd_write_data(y_end + LCD_YDIM_OFFSET);

    lcd_write_ram_propare(); // 开始写入GRAM
}

void lcd_draw_rec(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    print("x1: %d, y1: %d, x2: %d, y2: %d\r\n", x1, y1, x2, y2);
    lcd_set_windows(x1, y1, x2 - 1, y2 - 1);
    for (uint8_t i = 0; i < x2 - x1; i++) {
        for (uint8_t m = 0; m < y2 - y1; m++) {
            lcd_write_data_16bit(color);
        }
    }
}

void lcd_clear(uint16_t color)
{
    uint16_t display_color = color;
    if (display_color != BLACK) {
        display_color = WHITE;
    }
    print("lcd clear w:%d, h:%d\r\n", g_lcd_device.width - 1, g_lcd_device.height - 1);
    uint32_t i;
    lcd_set_windows(0, 0, g_lcd_device.width - 1, g_lcd_device.height - 1);
    uint8_t *color_mem = (uint8_t *)osal_kmalloc(LCD_CLEAR_MEM_SIZE, 0);
    if (memset_s(color_mem, LCD_CLEAR_MEM_SIZE, display_color, LCD_CLEAR_MEM_SIZE) != EOK) {
        osal_kfree(color_mem);
        print("lcd memset error\n");
        return;
    }
    for (i = 0; i < (uint16_t)(g_lcd_device.width); i++) {
        lcd_write_datas(color_mem, LCD_CLEAR_MEM_SIZE);
    }
    osal_kfree(color_mem);
}

void lcd_set_cursor(uint16_t x_pos, uint16_t y_pos)
{
    lcd_set_windows(x_pos, y_pos, x_pos, y_pos);
}

void lcd_draw_point(uint16_t x, uint16_t y)
{
    lcd_set_cursor(x, y); // 设置光标位置
    lcd_write_data_16bit(g_point_color);
}

void lcd_show_pictrue(const uint8_t *data, uint32_t length)
{
    lcd_set_windows(0, 0, g_lcd_device.width - 1, g_lcd_device.height - 1);
    lcd_write_datas(data, length);
}

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int32_t xerr = 0;
    int32_t yerr = 0;
    int32_t incx;
    int32_t incy;

    int32_t delta_x = x2 - x1; // 计算坐标增量
    int32_t delta_y = y2 - y1;
    int32_t distance = (delta_x > delta_y) ? delta_x : delta_y;
    int32_t row = x1;
    int32_t col = y1;
    if (delta_x > 0) {
        incx = 1; // 设置单步方向
    } else if (delta_x == 0) {
        incx = 0; // 垂直线
    } else {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0) {
        incy = 1;
    } else if (delta_y == 0) {
        incy = 0; // 水平线
    } else {
        incy = -1;
        delta_y = -delta_y;
    }
    for (uint16_t t = 0; t <= distance + 1; t++) { // 画线输出
        lcd_draw_point(row, col);       // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            row += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            col += incy;
        }
    }
}