/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Board Misc port. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-04, Create file. \n
 */
#include "tiot_board_misc_port.h"
#include "tiot_board_misc_port_config.h"
#include "soc_osal.h"
#include "pm_veto.h"

int32_t tiot_board_wake_lock_init(tiot_board_wakelock *lock)
{
    tiot_unused(lock);
    return 0;
}

void tiot_board_wake_lock(tiot_board_wakelock *lock)
{
    tiot_unused(lock);
#if ENABLE_LOW_POWER == YES
    (void)uapi_pm_add_sleep_veto(TIOT_WAKEUP_LOCK_ID);
#endif
}

void tiot_board_wake_unlock(tiot_board_wakelock *lock)
{
    tiot_unused(lock);
#if ENABLE_LOW_POWER == YES
    (void)uapi_pm_remove_sleep_veto(TIOT_WAKEUP_LOCK_ID);
#endif
}

void tiot_board_wake_lock_destroy(tiot_board_wakelock *lock)
{
    tiot_unused(lock);
}

void tiot_board_udelay(uint32_t us)
{
    osal_udelay(us);
}