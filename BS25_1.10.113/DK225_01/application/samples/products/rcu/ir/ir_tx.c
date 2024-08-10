/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: IR TX Sample Source. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-09-10, Create file. \n
 */

#include <stdbool.h>
#include "soc_osal.h"
#include "securec.h"
#include "gpio_porting.h"
#include "gpio.h"
#include "pwm.h"
#include "tcxo.h"
#include "ir_tx.h"

#define S_2_NS                         1000000000
#define S_2_US                         1000000
#define US_2_NS                        1000
#define BIT_LEN_8                      8
#define PARAM_2                        2
#define PWM_HIGH_TICK_PARAM_3          3
#define GPIO_DELAY_NS                  7500
#define ONE_INSTRUCTION_CYCLE_NS       156
#define PWM_TEST_CHANNEL               0
#define TCXO_DELAY_US                  20

static pin_t g_gpio;

void ir_init(pin_t gpio)
{
    g_gpio = gpio;
    uapi_pin_set_mode(g_gpio, HAL_PIO_PWM0);
}

errcode_t ir_transmit(int freq, int *pattern, int len)
{
    if (freq <= 0 || pattern == NULL || len <= 0) {
        return ERRCODE_INVALID_PARAM;
    }

    int ir_freq = freq;
    int pwm_freq = uapi_pwm_get_frequency(PWM_TEST_CHANNEL);
    int pulse_time = S_2_US / ir_freq;
    int pwm_high_ticks = pwm_freq / ir_freq / PWM_HIGH_TICK_PARAM_3;
    int pwm_low_ticks =  pwm_freq / ir_freq;
    pwm_config_t cfg_repeat = {0, 0, 0, 0, false};

    uapi_pwm_init();
    uapi_pwm_set_group(0, 1u << PWM_TEST_CHANNEL);
    for (int i = 0; i < len; i++) {
        if (i % PARAM_2 == 0) {
            /* 高电平 */
            cfg_repeat.high_time = pwm_high_ticks;
            cfg_repeat.low_time = (pwm_high_ticks + 1) * PARAM_2;
            cfg_repeat.offset_time = 0x0;
            cfg_repeat.cycles = pattern[i] / pulse_time;
            uapi_pwm_open(PWM_TEST_CHANNEL, &cfg_repeat);
            uapi_pwm_start(PWM_TEST_CHANNEL);
            uapi_tcxo_delay_us(pattern[i] - TCXO_DELAY_US);
        } else {
            /* 低电平 */
            cfg_repeat.high_time = 0;
            cfg_repeat.low_time = pwm_low_ticks;
            cfg_repeat.offset_time = 0x0;
            cfg_repeat.cycles = pattern[i] / pulse_time;
            uapi_pwm_open(PWM_TEST_CHANNEL, &cfg_repeat);
            uapi_pwm_start(PWM_TEST_CHANNEL);
            uapi_tcxo_delay_us(pattern[i] - TCXO_DELAY_US);
        }
    }
    return ERRCODE_SUCC;
}