/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides uart port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "interrupt/osal_interrupt.h"
#include "common_def.h"
#include "hal_timer_v100.h"
#include "timer.h"
#include "platform_core.h"
#include "chip_io.h"
#include "chip_core_irq.h"
#include "pm_clock.h"
#include "timer_porting.h"

#define TIMER_CONFIG_OFFSET_PER_CHANNEL  0x14UL

static uintptr_t g_timer_base_addr[TIMER_MAX_NUM] = {
    TIMER_0_BASE_ADDR,
    TIMER_1_BASE_ADDR,
    TIMER_2_BASE_ADDR,
    TIMER_3_BASE_ADDR,
};

uintptr_t timer_porting_comm_addr_get(void)
{
    return TIMER_BASE_ADDR;
}

uintptr_t timer_porting_base_addr_get(timer_index_t index)
{
    return g_timer_base_addr[index];
}

static int timer0_irq_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_timer_v100_irq_handler(TIMER_INDEX_0);
    osal_irq_clear(TIMER_0_IRQN);
    return 0;
}

static int timer1_irq_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_timer_v100_irq_handler(TIMER_INDEX_1);
    osal_irq_clear(TIMER_1_IRQN);
    return 0;
}

static int timer2_irq_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_timer_v100_irq_handler(TIMER_INDEX_2);
    osal_irq_clear(TIMER_2_IRQN);
    return 0;
}

static int timer3_irq_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_timer_v100_irq_handler(TIMER_INDEX_3);
    osal_irq_clear(TIMER_3_IRQN);
    return 0;
}

static osal_irq_handler g_timer_irq_handler[TIMER_MAX_NUM] = {
    timer0_irq_handler,
    timer1_irq_handler,
    timer2_irq_handler,
    timer3_irq_handler,
};

void timer_port_register_irq(timer_index_t index, uint32_t id, uint16_t priority)
{
    osal_irq_disable(id);
    osal_irq_request(id, (osal_irq_handler)g_timer_irq_handler[index], NULL, NULL, NULL);
    osal_irq_set_priority(id, priority);
    osal_irq_enable(id);
}

void timer_port_unregister_irq(timer_index_t index, uint32_t id)
{
    unused(index);
    osal_irq_disable(id);
    osal_irq_free(id, NULL);
}

void timer_port_clear_eoi(uint8_t timer_index)
{
    uint32_t timer_base_addr = TIMER_BASE_ADDR + (TIMER_CONFIG_OFFSET_PER_CHANNEL * timer_index);
    readl(timer_base_addr + TIMER_EOI);
}

static uint32_t g_timer_clock_value = CONFIG_TIMER_CLOCK_VALUE;
void timer_porting_clock_value_set(uint32_t clock)
{
    g_timer_clock_value = clock;
}

uint32_t timer_porting_clock_value_get(void)
{
    return g_timer_clock_value;
}

void timer_port_clock_enable(bool on)
{
    if (on) {
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_ENABLE, CLOCK_APERP_MTIMER_CLKEN);
    } else {
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_MTIMER_CLKEN);
    }
}

uint64_t timer_porting_us_2_cycle(uint32_t us)
{
    uint32_t clock_value = timer_porting_clock_value_get();
    return ((uint64_t)(us) * (uint64_t)(clock_value / (MS_PER_S * US_PER_MS)));
}

uint32_t timer_porting_cycle_2_us(uint64_t cycle)
{
    uint32_t clock_value = timer_porting_clock_value_get();
    return (uint32_t)((cycle * (MS_PER_S * US_PER_MS)) / (clock_value));
}

uint32_t timer_porting_compensat_by_tcxo(uint64_t diff)
{
    if (diff < TIMER_COMPENSAT_1_CYCLE_BY_TCXO) {
        return 0;
    } else if (diff < TIMER_COMPENSAT_2_CYCLE_BY_TCXO) {
        return TIMER_1_CYCLE;
    } else if (diff < TIMER_COMPENSAT_3_CYCLE_BY_TCXO) {
        return TIMER_2_CYCLE;
    } else {
        return diff / TIMER_COMPENSAT_1_CYCLE_BY_TCXO;
    }
}