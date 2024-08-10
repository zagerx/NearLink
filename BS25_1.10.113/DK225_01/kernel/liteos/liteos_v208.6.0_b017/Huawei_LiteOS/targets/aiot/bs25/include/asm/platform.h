/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#ifndef _ASM_PLATFORM_H
#define _ASM_PLATFORM_H

#include "interrupt_config.h"
#include "register_config.h"
#include "los_printf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 内核心跳时钟主频:32M */
#define OS_SYS_CLOCK_CONFIG           32000000
#define OS_SYS_CLOCK                  OS_SYS_CLOCK_CONFIG

// extern void print_str(const char *str, ...);
extern void test_suite_uart_sendf(const char *str, ...);

#define PRINT_EXC(fmt, arg...)      test_suite_uart_sendf(fmt, ##arg)
#define LOS_PANIC(fmt, args...)     test_suite_uart_sendf(fmt, ##args)
extern VOID BoardConfig(VOID);

#ifdef LOSCFG_PLATFORM_OSAPPINIT
extern VOID app_init(VOID);
#endif

extern VOID os_tick_timer_disable(VOID);
extern VOID os_tick_timer_enable(VOID);
extern UINT32 oal_get_sleep_ticks(VOID);
extern VOID oal_ticks_restore(UINT32 ticks);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ASM_PLATFORM_H */
