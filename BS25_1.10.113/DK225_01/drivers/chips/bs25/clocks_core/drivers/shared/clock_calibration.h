/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  CLOCK CALIBRATION DRIVER
 * Author: @CompanyNameTag
 * Create: 2020-03-17
 */

#ifndef CLOCK_CALIBRATION_H
#define CLOCK_CALIBRATION_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_core.h"
#include "product.h"
#include "otp_map.h"

/* * @addtogroup connectivity_drivers_non_os_clocks_core
 * @{
 */
typedef enum {
    XO_CTRIM_DECIMAL_0FF = 0,
    XO_CTRIM_DECIMAL_250FF = 1,
    XO_CTRIM_DECIMAL_500FF = 2,
    XO_CTRIM_DECIMAL_750FF = 3,
    XO_CTRIM_DECIMAL_1000FF = 4,
    XO_CTRIM_DECIMAL_1250FF = 5,
    XO_CTRIM_DECIMAL_1500FF = 6,
    XO_CTRIM_DECIMAL_1750FF = 7,
} xo_ctrim_decimal_t;

#if (EXTERNAL_CLOCK_CALIBRATION == YES)
/**
 * @brief  Get the calibration result of external clock with cali count number.
 * @param  cali_count Num of external clock detect count.
 * @return Calibration result of external clock frequency.
 */
uint32_t calibration_get_external_clock_frq(uint32_t cali_count);
#endif

/**
 * @brief  Get the calibration result of 32k clock with cali_cycle times.
 * @param  cali_cycle num of 32K detect cycles.
 * @return Calibration result of 32k.
 */
uint32_t calibration_get_clock_frq(uint16_t cali_cycle);

/**
 * @brief  Get the calibration result of 32k clock with 100 times.
 * @return Calibration result of 32k.
 */
uint32_t calibration_get_clock_frq_result(void);

/**
 * @brief  Init XO core trim value.
 */
void calibration_xo_core_trim_init(void);

/**
 * @brief  Init and config XO(32M) core ctrim value.
 */
void calibration_xo_core_ctrim_init(void);

/**
 * @brief  Set XO(32M) core ctrim value.
 * @param  xo_ctrim_left ctrim left value.
 * @param  xo_ctrim_right ctrim right value.
 */
void calibration_set_xo_core_ctrim(uint8_t xo_ctrim_left, uint8_t xo_ctrim_right);

#ifdef XO_CORE_CTRIM_HIGH
void calibration_save_xo_core_ctrim(uint8_t xo_ctrim_left, uint8_t xo_ctrim_right);
#else
void calibration_save_xo_core_ctrim(uint8_t xo_ctrim_integer, xo_ctrim_decimal_t xo_ctrim_decimal);
#endif

/**
 * @brief  Get the XO(32M) ctrim value from efuse.
 * @param  xo_ctrim_value ctrim value.
 */
#ifdef XO_CORE_CTRIM_HIGH
    void calibration_read_xo_core_ctrim(uint16_t *xo_ctrim_value);
#else
    void calibration_read_xo_core_ctrim(uint8_t *xo_ctrim_value);
#endif

/**
 * @brief  XO(32M) ctrim calibration algorithm
 * @param  increase: true (increase), false (decrease)
 * @param  left_step_num: ctrim left step num
 * @param  right_step_num: ctrim right step num
 */
void calibration_xo_core_ctrim_algorithm(bool increase, uint8_t left_step_num, uint8_t right_step_num);

/**
 * @brief  Get XO(32M) core ctrim reg value.
 * @param  xo_ctrim_value:   xo(32m) ctrim reg value.
 * @param  xo_ctrim_left: xo(32M) ctrim left value.
 * @param  xo_ctrim_right: xo(32M) ctrim right value.
 */

void calibration_get_xo_core_ctrim_reg(uint16_t *xo_ctrim_value, uint8_t *xo_ctrim_left, uint8_t *xo_ctrim_right);

/**
 * @brief  Set XO(32M) core trim value.
 * @param  xo_trim trim integer value.
 */
void calibration_set_xo_core_trim_reg(uint8_t xo_trim);

/**
 * @}
 */
#endif
