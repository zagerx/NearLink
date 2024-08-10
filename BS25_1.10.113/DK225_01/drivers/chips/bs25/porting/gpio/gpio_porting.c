/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides gpio port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-26， Create file. \n
 */
#include "hal_gpio_v100.h"
#include "chip_io.h"
#include "platform_types.h"
#include "platform_core.h"
#include "chip_core_irq.h"
#include "arch_port.h"
#include "soc_osal.h"
#include "gpio_porting.h"

#define GPIO_CHANNEL_0_BASE_ADDR GPIO0_BASE_ADDR
#define GPIO_CHANNEL_1_BASE_ADDR GPIO1_BASE_ADDR
#define CHANNEL_MAX_NUM GPIO_MAX_NUMBER

static uintptr_t g_gpio_base_addrs[GPIO_CHANNEL_MAX_NUM] = {
    (uintptr_t)GPIO_CHANNEL_0_BASE_ADDR,
    (uintptr_t)GPIO_CHANNEL_1_BASE_ADDR,
};

static uint32_t irq_num[GPIO_CHANNEL_MAX_NUM] = {
    GPIO_0_IRQN,
    GPIO_1_IRQN,
};

uintptr_t gpio_porting_base_addr_get(gpio_channel_t channel)
{
    return g_gpio_base_addrs[channel];
}

void gpio_port_register_irq(uint32_t int_id)
{
    osal_irq_request(int_id, (osal_irq_handler)irq_gpio_handler, NULL, NULL, NULL);
    osal_irq_set_priority(int_id, irq_prio(int_id));
}

int irq_gpio_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_gpio_irq_handler();
    return 0;
}

void gpio_context_adapter(void)
{
    gpio_context_t *ctxt = gpio_context_get();

    ctxt->pin_max_num = PIN_MAX_NUMBER;
    ctxt->is_ulp_enable = false;
#if ENABLE_GPIO_INTERRUPT == YES
    ctxt->is_irq_enable = true;
#else
    ctxt->is_irq_enable = false;
#endif
    for (int i = 0; i < CHANNEL_MAX_NUM; i++) {
        ctxt->irq_list[i] = irq_num[i];
    }
}

void gpio_hal_context_adapter(void)
{
    hal_gpio_context_t *hal_ctxt = hal_gpio_context_get();

    hal_ctxt->pin_max_num = PIN_MAX_NUMBER;
    hal_ctxt->is_ulp_enable = false;
}

void gpio_ulp_int_en(bool on)
{
    UNUSED(on);
}

void gpio_ulp_clock_switch_to_pclk(bool on)
{
    UNUSED(on);
}

static gpio_channel_t port_gpio_get_channel(pin_t pin)
{
    if (hal_gpio_context_get()->is_ulp_enable &&
        pin >= hal_gpio_context_get()->ulp_pin_start && pin <= hal_gpio_context_get()->ulp_pin_end) {
        return hal_gpio_context_get()->ulp_pin_channel;
    }
    return (gpio_channel_t)((uint32_t)pin / HAL_GPIO_CHANNEL_OFFSET);
}

static uint32_t port_gpio_get_channel_pin(pin_t pin)
{
    if (hal_gpio_context_get()->is_ulp_enable &&
        pin >= hal_gpio_context_get()->ulp_pin_start && pin <= hal_gpio_context_get()->ulp_pin_end) {
        return (uint32_t)pin - (uint32_t)hal_gpio_context_get()->ulp_pin_start;
    }
    return (uint32_t)pin % HAL_GPIO_CHANNEL_OFFSET;
}

void gpio_select_core(pin_t pin, cores_t core)
{
    if (pin >= PIN_NONE) {
        return;
    }
    uint32_t channel = (uint32_t)port_gpio_get_channel(pin);
    uint32_t channel_pin = port_gpio_get_channel_pin(pin);
    uint32_t addr_dsp = (uint32_t)(HAL_GPIO_D_CORE_SET_REG + channel * HAL_GPIO_CORE_SET_CHANNEL_OFFSET);
    uint32_t addr_non_dsp = (uint32_t)(HAL_GPIO_NON_D_CORE_SET_REG + channel * HAL_GPIO_CORE_SET_CHANNEL_OFFSET);
    uint32_t offset = ((uint32_t)(channel_pin / HAL_GPIO_CORE_SET_GPIOS)) << HAL_GPIO_CORE_SET_REG_OFFSET;
    channel_pin = (pin_t)(channel_pin % HAL_GPIO_CORE_SET_GPIOS);
    switch (core) {
        case CORES_APPS_CORE:
            reg16_clrbit((uintptr_t)addr_dsp + offset, channel_pin);
            reg16_clrbit((uintptr_t)addr_non_dsp + offset, channel_pin);
            break;
        case CORES_BT_CORE:
            reg16_clrbit((uintptr_t)addr_dsp + offset, channel_pin);
            reg16_setbit((uintptr_t)addr_non_dsp + offset, channel_pin);
            break;
        default:
            break;
    }
}