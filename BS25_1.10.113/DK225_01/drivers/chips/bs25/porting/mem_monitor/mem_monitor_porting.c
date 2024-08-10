/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides mem_monitor port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-15, Create file. \n
 */
#include "soc_osal.h"
#include "common_def.h"
#include "tcxo.h"
#include "hal_mem_monitor_v150.h"
#include "mem_monitor_porting.h"

#define MEM_MONITOR_MEM_BASE_ADDR  0x55000820

uintptr_t mem_monitor_port_base_addr_get(void)
{
    return (uintptr_t)MEM_MONITOR_MEM_BASE_ADDR;
}

static int mem_monitor_handler(int a, void *tmp)
{
    unused(a);
    unused(tmp);

    hal_mem_monitor_irq_handler();
    return 0;
}

void mem_monitor_port_register_hal_funcs(void)
{
    hal_mem_monitor_register_funcs(hal_mem_monitor_v150_funcs_get());
}

void mem_monitor_port_unregister_hal_funcs(void)
{
    hal_mem_monitor_unregister_funcs();
}

void mem_monitor_port_register_irq(void)
{
    unused(mem_monitor_handler);
}

void mem_monitor_port_unregister_irq(void)
{
}

void mem_monitor_port_irq_lock(void)
{
    osal_irq_lock();
}

void mem_monitor_port_irq_unlock(void)
{
    osal_irq_unlock();
}

bool mem_monitor_port_check_addr(uint32_t addr_start, uint32_t addr_end)
{
    if ((addr_start < MEM_MONITOR_BEGINE_ADDR) ||
        (addr_end > MEM_MONITOR_END_ADDR)) {
        return false;
    }
    return true;
}
