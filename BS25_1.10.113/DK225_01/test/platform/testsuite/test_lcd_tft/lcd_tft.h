/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd tft init \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-18， Create file. \n
 */

#ifndef LCD_TFT_H
#define LCD_TFT_H

#include "test_suite_log.h"

#define print(fmt, arg...) test_suite_log_stringf(fmt, ##arg)

void lcd_tft_init(void);

#endif