/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pwm port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#include "soc_osal.h"
#include "common_def.h"
#include "hal_pwm_v150.h"
#include "chip_core_irq.h"
#include "lpm_dev_ops.h"
#include "platform_core.h"
#include "pm_clock.h"
#include "pwm_porting.h"

#define BUS_CLOCK_TIME_16M      16000000UL

uintptr_t g_pwm_base_addr =  (uintptr_t)PWM_0_BASE;

static bool g_pwm_irq_created[CONFIG_PWM_CHANNEL_NUM] = { false };

uintptr_t pwm_porting_base_addr_get(void)
{
    return g_pwm_base_addr;
}

static int pwm_0_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_0);
    osal_irq_clear(PWM_0_IRQN);
    return 0;
}

static int pwm_1_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_1);
    osal_irq_clear(PWM_1_IRQN);
    return 0;
}

static int pwm_2_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_2);
    osal_irq_clear(PWM_2_IRQN);
    return 0;
}
#if CONFIG_PWM_CHANNEL_NUM > 5
static int pwm_3_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_3);
    osal_irq_clear(PWM_3_IRQN);
    return 0;
}

static int pwm_4_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_4);
    osal_irq_clear(PWM_4_IRQN);
    return 0;
}

static int pwm_5_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_5);
    osal_irq_clear(PWM_5_IRQN);
    return 0;
}
#endif
static osal_irq_handler const g_pwm_interrupt_handlers[CONFIG_PWM_CHANNEL_NUM] = {
    (osal_irq_handler)pwm_0_handler,
    (osal_irq_handler)pwm_1_handler,
    (osal_irq_handler)pwm_2_handler,
#if CONFIG_PWM_CHANNEL_NUM > 5
    (osal_irq_handler)pwm_3_handler,
    (osal_irq_handler)pwm_4_handler,
    (osal_irq_handler)pwm_5_handler
#endif
};

uint32_t pwm_irq_num[CONFIG_PWM_CHANNEL_NUM] = {
    (uintptr_t)PWM_0_IRQN,
    (uintptr_t)PWM_1_IRQN,
    (uintptr_t)PWM_2_IRQN,
#if CONFIG_PWM_CHANNEL_NUM > 5
    (uintptr_t)PWM_3_IRQN,
    (uintptr_t)PWM_4_IRQN,
    (uintptr_t)PWM_5_IRQN
#endif
};

void pwm_port_register_hal_funcs(void)
{
    hal_pwm_register_funcs(hal_pwm_v150_funcs_get());
}

void pwm_port_unregister_hal_funcs(void)
{
    hal_pwm_unregister_funcs();
}

void pwm_port_register_irq(pwm_channel_t channel)
{
    if (!g_pwm_irq_created[channel]) {
        osal_irq_request(pwm_irq_num[channel], g_pwm_interrupt_handlers[channel], NULL, NULL, NULL);
        osal_irq_enable(pwm_irq_num[channel]);
        g_pwm_irq_created[channel] = true;
    }
}

void pwm_port_unregister_irq(pwm_channel_t channel)
{
    if (g_pwm_irq_created[channel]) {
        osal_irq_disable(pwm_irq_num[channel]);
        osal_irq_free(pwm_irq_num[channel], NULL);
        g_pwm_irq_created[channel] = false;
    }
}

void pwm_port_clock_enable(bool on)
{
    if (on) {
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_ENABLE, CLOCK_APERP_PWM_CLKEN);
    } else {
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_PWM_CLKEN);
    }
}

uint32_t pwm_port_get_clock_value(pwm_channel_t channel)
{
    if (channel >= CONFIG_PWM_CHANNEL_NUM) {
        return 0;
    }
    return BUS_CLOCK_TIME_16M;
}
