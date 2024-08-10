/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: RISCV31 Machine timer config for LiteOS
 * Author: @CompanyNameTag
 * Create: 2021-9-27
 */
#include "hal_hwi.h"
#include "los_hwi.h"
#include "los_tick_pri.h"
#include "los_hwi_pri.h"
#include "chip_io.h"
#include "arch_barrier.h"
#include "oal_interface.h"
#include "platform_core.h"
#include "idle_config.h"
#include "interrupt.h"
#include "soc_osal.h"
#if defined(CONFIG_PM_SYS_SUPPORT)
#include "pm_sys.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MACHINE_INT_NUM                 NUM_HAL_INTERRUPT_TIMER
#define CLK_CYCLE_PER_OS_TICK           16000

#define MACHINE_TIMER_BASE              TIMER_3_BASE_ADDR
#define MACHINE_TIMER_LOAD_COUNT        0x00
#define MACHINE_TIMER_LOAD_COUNT_L      0x00
#define MACHINE_TIMER_LOAD_COUNT_H      0x02
#define MACHINE_TIMER_CURRENT_VALUE     0x04
#define MACHINE_TIMER_CONTROL_REG       0x08
#define MACHINE_TIMER_EOI               0x0C
#define MACHINE_TIMER_INT_STATUS        0x10
#define MACHINE_TIMER_ENABLE_USRMODE    0x3

static inline void machine_timer_init(void)
{
    // before config timer, disable it
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), 0);

    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_LOAD_COUNT), CLK_CYCLE_PER_OS_TICK);
    // user define mode and enable it
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), MACHINE_TIMER_ENABLE_USRMODE);
}

void os_tick_timer_disable(void)
{
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), 0);
}

void os_tick_timer_enable(void)
{
    reg16_clrbit((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), 0);
    // user define mode
    reg16_setbit((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), 1);
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_LOAD_COUNT), CLK_CYCLE_PER_OS_TICK);
    // enable
    reg16_setbit((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), 0);
}

static inline uint32_t machine_timer_get_cur_count(void)
{
    return readl(MACHINE_TIMER_BASE + MACHINE_TIMER_CURRENT_VALUE);
}

static inline uint32_t machine_timer_get_load_count(void)
{
    return readl(MACHINE_TIMER_BASE + MACHINE_TIMER_LOAD_COUNT);
}

static int32_t HalTickEntry(void)
{
    // clear the timer interrupt
    int_clear_pending_irq((int32_t)MACHINE_INT_NUM);
    readl(MACHINE_TIMER_BASE + MACHINE_TIMER_EOI);
    OsTickHandler();
#if defined(CONFIG_PM_SYS_SUPPORT)
    pm_tick_handler_entry();
#endif
    return 0;
}

LITE_OS_SEC_TEXT_INIT void HalClockStart(void)
{
    uint32_t ret;
    uint32_t lock_int_save;
#if defined(OS_SYS_CLOCK) && (OS_SYS_CLOCK == 0)
    return;
#endif
    if ((LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (OS_SYS_CLOCK < LOSCFG_BASE_CORE_TICK_PER_SECOND)) {
        return;
    }

    lock_int_save = LOS_IntLock();
    ret = osal_irq_request((uint32_t)MACHINE_INT_NUM, (osal_irq_handler)HalTickEntry, NULL, NULL, NULL);
    if (ret != OSAL_SUCCESS) {
        PRINT_ERR("%s %d: osal_irq_request ret:0x%x\n", __FUNCTION__, __LINE__, ret);
    }
    osal_irq_enable(MACHINE_INT_NUM);

    machine_timer_init();
    idle_task_config();
    LOS_IntRestore(lock_int_save);
}

void HalClockInit(void)
{
    SET_SYS_CLOCK(OS_SYS_CLOCK);
}

void HalDelayUs(uint32_t usecs)
{
    uint64_t cycles = (uint64_t)usecs * GET_SYS_CLOCK() / OS_SYS_US_PER_SECOND;
    uint64_t deadline;
    uint64_t now;
    uint32_t last_cnt;
    uint32_t cur_cnt;
    uint32_t load_cnt = 0;
    uint8_t  is_pending;

    (void)HalIrqPendingGet(MACHINE_INT_NUM, &is_pending);
    last_cnt = machine_timer_get_cur_count();
    deadline = HalClockGetCycles() + cycles;
    for (;;) {
        now = HalClockGetCycles();
        if (OS_INT_ACTIVE != 0) {
            cur_cnt = machine_timer_get_cur_count();
            // calculate tick timer turn over count.
            load_cnt = last_cnt < cur_cnt ? load_cnt + 1 : load_cnt;
            // if not pendind when calling HalDelayUs, machine_timer_get_cur_count will calculate the first pending.
            if ((is_pending == 0) && (load_cnt > 0)) {
                now += (load_cnt - 1) * OS_CYCLE_PER_TICK;
            } else if (is_pending == 1) { // if already pending, need calculate all load_cnt.
                now += load_cnt * OS_CYCLE_PER_TICK;
            }
            last_cnt = cur_cnt;
        }
        if (now >= deadline) {
            break;
        }
        __asm__ volatile ("nop");
    }
}

uint64_t HalClockGetCycles(void)
{
    uint64_t tick_count;
    uint64_t cycle;
    uint32_t cycle_in_tick;
    uint32_t lock_int_save;
    uint32_t load_tick;
    uint32_t cur_tick;
    uint8_t    is_pending = 0;

    load_tick = OS_CYCLE_PER_TICK;
    lock_int_save = LOS_IntLock();

    tick_count = LOS_TickCountGet();
    cur_tick = machine_timer_get_cur_count();
    if ((HalIrqPendingGet(MACHINE_INT_NUM, &is_pending) == LOS_OK) && (is_pending != 0)) {
        cur_tick = machine_timer_get_cur_count();
        tick_count++;
    }
    cycle_in_tick = load_tick - cur_tick;
    cycle = tick_count * load_tick + cycle_in_tick;

    LOS_IntRestore(lock_int_save);
    return cycle;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
