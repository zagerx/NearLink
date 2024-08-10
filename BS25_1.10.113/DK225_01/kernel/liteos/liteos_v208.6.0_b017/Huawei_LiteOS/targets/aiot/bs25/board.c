/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#include "los_task_pri.h"
#include "los_init_pri.h"
#include "los_init.h"
#include "los_tick_pri.h"
#include "asm/dma.h"
#include "asm/memmap_config.h"


#ifdef LOSCFG_DRIVERS_USB
#include "soc/usb.h"
#endif

#ifdef LOSCFG_DRIVERS_BASE
#include "los_driverbase.h"
#endif

void dma_cache_clean(unsigned int start, unsigned int end)
{
    ArchDCacheCleanByAddr(start, end);
}

void dma_cache_inv(unsigned int start, unsigned int end)
{
    ArchDCacheFlushByAddr(start, end);
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

__attribute__((weak)) VOID BoardConfig(VOID)
{
    /* config start memory before startup usb_init */

    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());
}

UINT32 oal_get_sleep_ticks(VOID)
{
    UINT32 intSave;
    intSave = LOS_IntLock();
    UINT32 taskTimeout = OsSortLinkGetNextExpireTime(&OsPercpuGet()->taskSortLink);
#ifdef LOSCFG_BASE_CORE_SWTMR
    UINT32 swtmrTimeout = OsSortLinkGetNextExpireTime(&OsPercpuGet()->swtmrSortLink);
    if (swtmrTimeout < taskTimeout) {
        taskTimeout = swtmrTimeout;
    }
#endif
    LOS_IntRestore(intSave);
    return taskTimeout;
}

VOID oal_ticks_restore(UINT32 ticks)
{
    UINT32 intSave;
    intSave = LOS_IntLock();
    g_tickCount[ArchCurrCpuid()] += ticks;
    OsSortLinkUpdateExpireTime(ticks, &OsPercpuGet()->taskSortLink);
#ifdef LOSCFG_BASE_CORE_SWTMR
    OsSortLinkUpdateExpireTime(ticks, &OsPercpuGet()->swtmrSortLink);
#endif
    LOS_IntRestore(intSave);
}