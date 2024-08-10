/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd tft init \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-18， Create file. \n
 */

#include "gpio.h"
#include "pinctrl.h"
#include "tcxo.h"

#include "lcd_oprator.h"
#include "lcd_port.h"
#include "lcd_draw.h"
#include "lcd_tft.h"

#define LCD_SLEEP_DELAY         120

void lcd_gpio_init(void)
{
    pin_t gpio_pin[] = { LCD_POWN, LCD_BACK, LCD_RESERT, LCD_DC };
    for (uint8_t i = 0; i < sizeof(gpio_pin) / sizeof(pin_t); i++) {
        uapi_pin_set_mode(gpio_pin[i], 0);
        uapi_gpio_set_dir(gpio_pin[i], GPIO_DIRECTION_OUTPUT);
    }
    uapi_gpio_set_val(LCD_POWN, 1);
    uapi_gpio_set_val(LCD_BACK, 1);
}

static void lcd_tft_init_gmctrn(void)
{
    lcd_write_reg(0xE0);
    lcd_write_data(0x10);
    lcd_write_data(0x0E);
    lcd_write_data(0x02);
    lcd_write_data(0x03);
    lcd_write_data(0x0E);
    lcd_write_data(0x07);
    lcd_write_data(0x02);
    lcd_write_data(0x07);
    lcd_write_data(0x0A);
    lcd_write_data(0x12);
    lcd_write_data(0x27);
    lcd_write_data(0x37);
    lcd_write_data(0x00);
    lcd_write_data(0x0D);
    lcd_write_data(0x0E);
    lcd_write_data(0x10);

    lcd_write_reg(0xE1);
    lcd_write_data(0x10);
    lcd_write_data(0x0E);
    lcd_write_data(0x03);
    lcd_write_data(0x03);
    lcd_write_data(0x0F);
    lcd_write_data(0x06);
    lcd_write_data(0x02);
    lcd_write_data(0x08);
    lcd_write_data(0x0A);
    lcd_write_data(0x13);
    lcd_write_data(0x26);
    lcd_write_data(0x36);
    lcd_write_data(0x00);
    lcd_write_data(0x0D);
    lcd_write_data(0x0E);
    lcd_write_data(0x10);
}

static void lcd_tft_init_fmctr(void)
{
    lcd_write_reg(0xB1);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);

    lcd_write_reg(0xB2);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);

    lcd_write_reg(0xB3);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);

    lcd_write_reg(0xB4);
    lcd_write_data(0x03);
}

static void lcd_tft_init_pwctr(void)
{
    lcd_write_reg(0xC0);
    lcd_write_data(0x62);
    lcd_write_data(0x02);
    lcd_write_data(0x04);

    lcd_write_reg(0xC1);
    lcd_write_data(0xC0);

    lcd_write_reg(0xC2);
    lcd_write_data(0x0D);
    lcd_write_data(0x00);

    lcd_write_reg(0xC3);
    lcd_write_data(0x8D);
    lcd_write_data(0x6A);

    lcd_write_reg(0xC4);
    lcd_write_data(0x8D);
    lcd_write_data(0xEE);

    lcd_write_reg(0xC5);
    lcd_write_data(0x0E);
}

void lcd_tft_init(void)
{
    lcd_spi_init();
    lcd_gpio_init();
    lcd_reset();
    lcd_write_reg(0x11);  // Sleep exit
    uapi_tcxo_delay_ms(LCD_SLEEP_DELAY);
    // ST7735R Frame Rate
    lcd_write_reg(0x21);

    lcd_tft_init_fmctr();
    lcd_tft_init_pwctr();
    lcd_tft_init_gmctrn();

    lcd_write_reg(0x3A);     // 颜色格式 16bit
    lcd_write_data(0x05);

    lcd_write_reg(0x36);
    lcd_write_data(0xA8);    // BGR格式  0xA0:RGB格式
    lcd_write_reg(0x28);     // Display OFF
    lcd_set_deriction(0);    // 设置LCD显示方向
    lcd_write_reg(0x29);     // Display On
    lcd_clear(WHITE);
}
