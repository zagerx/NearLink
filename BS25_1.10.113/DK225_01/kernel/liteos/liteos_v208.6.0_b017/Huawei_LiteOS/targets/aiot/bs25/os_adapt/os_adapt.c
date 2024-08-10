/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#include <stdint.h>
#include "los_typedef.h"
#include "los_printf.h"
#include "los_task.h"
#include "los_init_pri.h"
#include "los_task_pri.h"

#define APP_INIT_WDT_KICK_TIME_MS 20000
extern UINT32 uapi_watchdog_kick(VOID);

__attribute__((weak)) VOID app_init(VOID)
{
    UINT32 task_delay_tick = LOS_MS2Tick(APP_INIT_WDT_KICK_TIME_MS);
    while (1) {
        uapi_watchdog_kick();
        LOS_TaskDelay(task_delay_tick);
    }
    return;
}

VOID LOS_PrepareMainTask(VOID){
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());
}


