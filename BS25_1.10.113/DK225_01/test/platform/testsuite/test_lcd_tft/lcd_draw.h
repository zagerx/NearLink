/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd tft draw, draw image or rectangle \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-18， Create file. \n
 */

#ifndef LCD_DRAW_H
#define LCD_DRAW_H

#include "stdint.h"
#define GREEN  0x07E0
#define RED    0xF800
#define BLUE   0x001F
#define WHITE  0xFFFF
#define BLACK  0x0000

void lcd_set_deriction(uint8_t direction);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_clear(uint16_t color);
void lcd_draw_rec(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
void lcd_show_pictrue(const uint8_t *data, uint32_t length);

#endif