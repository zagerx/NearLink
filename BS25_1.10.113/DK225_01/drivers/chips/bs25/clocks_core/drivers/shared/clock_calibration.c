/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: CLOCK CALIBRATION DRIVER.
 * Author: @CompanyNameTag
 * Create: 2020-03-17
 */
#include "clock_calibration.h"
#include "platform_core.h"
#include "hal_32k_clock.h"
#include "otp_map.h"
#if CORE != WIFI
#include "otp.h"
#endif
#include "hal_tcxo.h"
#include "chip_io.h"
#include "non_os.h"
#include "tcxo.h"
#include "tcxo_porting.h"
#include "uart.h"

#define CALIBRATION_XO_CORE_TRIM_REG XO_CORE_TRIM_REG
#define CALIBRATION_XO_CORE_CTRIM_REG XO_CORE_CTRIM_REG

#define CALIBRATION_XO_CORE_TRIM_DEFAULT 1

#define CALIBRATION_XO_CORE_CTRIM_RIGHT_BIT 0
#define CALIBRATION_XO_CORE_CTRIM_RIGHT_LEN 7
#define CALIBRATION_XO_CORE_CTRIM_RIGHT_MAX 0x7F
#define CALIBRATION_XO_CORE_CTRIM_RIGHT_MIN 0
#define CALIBRATION_XO_CORE_CTRIM_RIGHT_POS 0x7F

#define CALIBRATION_XO_CORE_CTRIM_LEFT_BIT 7
#define CALIBRATION_XO_CORE_CTRIM_LEFT_LEN 7
#define CALIBRATION_XO_CORE_CTRIM_LEFT_MAX 0x7F
#define CALIBRATION_XO_CORE_CTRIM_LEFT_MIN 0
#define CALIBRATION_XO_CORE_CTRIM_LEFT_POS 0x7F

#define CALIBRATION_XO_CORE_CTRIM_LOW_POS 0xFF
#define CALIBRATION_XO_CORE_CTRIM_HIGH_POS 0xFF00

#define CALIBRATION_CLOCK_MUL 100
#define CALIBRATION_CLOCK_FREQ_32K_100 3276800

#define XO_CTRIM_CONFIG_WAIT_US 20ULL
#define XO_CTRIM_ADJUST_INTERVAL_VALUE 8
#define XO_CTRIM_LOW_BIT_ADJUST_MASK 7
#define XO_CTRIM_VALUE_DEFAULT 0x1FBF

static uint32_t g_clock_32k = CALIBRATION_CLOCK_FREQ_32K_100;

uint32_t calibration_get_clock_frq(uint16_t cali_cycle)
{
    uint32_t result;
    hal_32k_clock_calibration_cycle_config(cali_cycle);
    result = hal_32k_clock_get_detect_result();
    if (result != 0) {
        g_clock_32k = (uint32_t)(((uint64_t)(cali_cycle) * HAL_TCXO_TICKS_PER_SECOND * CALIBRATION_CLOCK_MUL) / result);
    }

    return g_clock_32k; // return 32K clock
}

#if (EXTERNAL_CLOCK_CALIBRATION == YES)
uint32_t calibration_get_external_clock_frq(uint32_t cali_count)
{
    uint32_t count_result;
    uint32_t clock_result = 0;
    hal_external_clock_calibration_count_config(cali_count);
    count_result = hal_external_clock_get_detect_result();
    if (count_result != 0) {
        clock_result = (uint32_t)(((uint64_t)(count_result) * HAL_TCXO_TICKS_PER_SECOND) / cali_count);
    }

    return clock_result; // return external clock detect frequency
}
#endif

uint32_t calibration_get_clock_frq_result(void)
{
    return g_clock_32k;
}

void calibration_xo_core_trim_init(void)
{
    // need use trim value from otp
    // the xo_trim and xo_ctrim should update at same time like this:
    // xo_trim:    0x1  0x03  ...  0x0F
    // xo_ctrim:   0x53 0x56  ...  0x76
    writew(CALIBRATION_XO_CORE_TRIM_REG, CALIBRATION_XO_CORE_TRIM_DEFAULT);
}

void calibration_xo_core_ctrim_init(void)
{
    uint8_t xo_ctrim_value_left = 0;
    uint8_t xo_ctrim_value_right = 0;
    uint16_t xo_ctrim_value = 0;
    calibration_read_xo_core_ctrim(&xo_ctrim_value);

    if (xo_ctrim_value == 0) {
        // use default value if NO otp config
        xo_ctrim_value = XO_CTRIM_VALUE_DEFAULT;
    }
    xo_ctrim_value_left = ((xo_ctrim_value) >> CALIBRATION_XO_CORE_CTRIM_LEFT_BIT) & CALIBRATION_XO_CORE_CTRIM_LEFT_POS;
    xo_ctrim_value_right = ((xo_ctrim_value) & CALIBRATION_XO_CORE_CTRIM_RIGHT_POS);
    calibration_set_xo_core_ctrim(xo_ctrim_value_left, xo_ctrim_value_right);
}

void calibration_set_xo_core_ctrim(uint8_t xo_ctrim_left, uint8_t xo_ctrim_right)
{
    if (xo_ctrim_left > CALIBRATION_XO_CORE_CTRIM_LEFT_MAX || xo_ctrim_right > CALIBRATION_XO_CORE_CTRIM_RIGHT_MAX) {
        return;
    }
    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_LEFT_BIT, CALIBRATION_XO_CORE_CTRIM_LEFT_LEN,
        xo_ctrim_left);
    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_RIGHT_BIT,
        CALIBRATION_XO_CORE_CTRIM_RIGHT_LEN, xo_ctrim_right);
}

void calibration_save_xo_core_ctrim(uint8_t xo_ctrim_left, uint8_t xo_ctrim_right)
{
    uint16_t value = 0;
    if (xo_ctrim_left > CALIBRATION_XO_CORE_CTRIM_LEFT_MAX || xo_ctrim_right > CALIBRATION_XO_CORE_CTRIM_RIGHT_MAX) {
        return;
    }
    uint8_t xo_ctrim_value_low = 0;
    uint8_t xo_ctrim_value_high = 0;
    otp_read_byte(&xo_ctrim_value_low, XO_CORE_CTRIM);
    otp_read_byte(&xo_ctrim_value_high, XO_CORE_CTRIM_HIGH);
    value = (xo_ctrim_value_low |
        (((uint16_t)xo_ctrim_value_high << XO_CTRIM_ADJUST_INTERVAL_VALUE) & CALIBRATION_XO_CORE_CTRIM_HIGH_POS));
    if (value == 0) {
        value = ((uint16_t)xo_ctrim_left << CALIBRATION_XO_CORE_CTRIM_LEFT_BIT) | (uint8_t)xo_ctrim_right;
        otp_write_byte(XO_CORE_CTRIM, (uint8_t)value);
        otp_write_byte(XO_CORE_CTRIM_HIGH,
            (uint8_t)((value >> XO_CTRIM_ADJUST_INTERVAL_VALUE) & CALIBRATION_XO_CORE_CTRIM_LOW_POS));
    }
}

void calibration_read_xo_core_ctrim(uint16_t *xo_ctrim_value)
{
    if (xo_ctrim_value == NULL) {
        return;
    }
    uint8_t xo_ctrim_value_low = 0;
    uint8_t xo_ctrim_value_high = 0;
    otp_read_byte(&xo_ctrim_value_low, XO_CORE_CTRIM);
    otp_read_byte(&xo_ctrim_value_high, XO_CORE_CTRIM_HIGH);
    *xo_ctrim_value = (xo_ctrim_value_low |
        (((uint16_t)xo_ctrim_value_high << XO_CTRIM_ADJUST_INTERVAL_VALUE) & CALIBRATION_XO_CORE_CTRIM_HIGH_POS));
}

void calibration_xo_core_ctrim_algorithm(bool increase, uint8_t left_step_num, uint8_t right_step_num)
{
    uint8_t xo_left = reg16_getbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_LEFT_BIT,
        CALIBRATION_XO_CORE_CTRIM_LEFT_LEN);

    uint8_t xo_right = reg16_getbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_RIGHT_BIT,
        CALIBRATION_XO_CORE_CTRIM_RIGHT_LEN);

    if (increase) {
        if ((xo_left + left_step_num) > CALIBRATION_XO_CORE_CTRIM_LEFT_MAX) {
            xo_left = CALIBRATION_XO_CORE_CTRIM_LEFT_MAX;
        } else {
            xo_left = xo_left + left_step_num;
        }

        if ((xo_right + right_step_num) > CALIBRATION_XO_CORE_CTRIM_RIGHT_MAX) {
            xo_right = CALIBRATION_XO_CORE_CTRIM_RIGHT_MAX;
        } else {
            xo_right = xo_right + right_step_num;
        }
    } else {
        if (xo_left < left_step_num) {
            xo_left = CALIBRATION_XO_CORE_CTRIM_LEFT_MIN;
        } else {
            xo_left = xo_left - left_step_num;
        }

        if (xo_right < right_step_num) {
            xo_right = CALIBRATION_XO_CORE_CTRIM_RIGHT_MIN;
        } else {
            xo_right = xo_right - right_step_num;
        }
    }

    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_LEFT_BIT, CALIBRATION_XO_CORE_CTRIM_LEFT_LEN,
        xo_left);
    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_RIGHT_BIT,
        CALIBRATION_XO_CORE_CTRIM_RIGHT_LEN, xo_right);
}

void calibration_get_xo_core_ctrim_reg(uint16_t *xo_ctrim_value, uint8_t *xo_ctrim_left, uint8_t *xo_ctrim_right)
{
    if ((xo_ctrim_value == NULL) || (xo_ctrim_left == NULL) || (xo_ctrim_right == NULL)) {
        return;
    }

    *xo_ctrim_value = readw(CALIBRATION_XO_CORE_CTRIM_REG);
    *xo_ctrim_left = ((*xo_ctrim_value) >> CALIBRATION_XO_CORE_CTRIM_LEFT_BIT) & CALIBRATION_XO_CORE_CTRIM_LEFT_POS;
    *xo_ctrim_right = ((*xo_ctrim_value) & CALIBRATION_XO_CORE_CTRIM_RIGHT_POS);
}

void calibration_set_xo_core_trim_reg(uint8_t xo_trim)
{
    non_os_enter_critical();
    writew(CALIBRATION_XO_CORE_TRIM_REG, xo_trim);
    non_os_exit_critical();
}
