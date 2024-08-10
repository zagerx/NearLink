/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved. \n
 *
 * Description: TIoT pm slave. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2024-02-28, Create file. \n
 */
#include "tiot_pm_slave.h"
#include "tiot_pm_wakelock.h"
#include "tiot_board_pin_port.h"
#include "tiot_board_log.h"
#include "tiot_controller.h"
#include "tiot_board.h"

tiot_pm_retcode tiot_pm_slave_power_off(tiot_pm *pm)
{
    tiot_unused(pm);
    tiot_pm_wake_unlock();
    tiot_print_info("[TIoT][pm]power off.\r\n");
    return TIOT_PM_EVTRET_NEXT_STATE;
}

tiot_pm_retcode tiot_pm_slave_power_on(tiot_pm *pm)
{
    tiot_unused(pm);
    tiot_pm_wake_lock();
    tiot_print_info("[TIoT][pm]power on.\r\n");
    return TIOT_PM_EVTRET_NEXT_STATE;
}

tiot_pm_retcode tiot_pm_slave_wakeup_handle(tiot_pm *pm)
{
    /* Only SLEEP -> WAKING */
    uint32_t wakeup_in_pin;
    uint32_t wakeup_out_pin;
    tiot_controller *ctrl = tiot_container_of(pm, tiot_controller, pm);
    const tiot_device_timings *timings = &ctrl->dev_info->timings;

    if (pm->state != TIOT_PM_STATE_SLEEP) {
        return TIOT_PM_EVTRET_KEEP_STATE;
    }

    tiot_pm_wake_lock();

    wakeup_out_pin = pm->pm_info[TIOT_PIN_DEVICE_WAKEUP_HOST];
    if (wakeup_out_pin != TIOT_PIN_NONE) {
        if (tiot_board_pin_set_level(wakeup_out_pin, TIOT_PIN_LEVEL_LOW) != 0) {
            tiot_print_err("[TIoT:pm]wakeout pin set level low fail.\r\n");
            goto wakeout_fail;
        }
        osal_msleep(timings->wakeup_pulse_ms);
        if (tiot_board_pin_set_level(wakeup_out_pin, TIOT_PIN_LEVEL_HIGH) != 0) {
            tiot_print_err("[TIoT:pm]wakeout pin set level high fail.\r\n");
            goto wakeout_fail;
        }
    } else {
        tiot_print_warning("[TIoT][pm]wakeout pin is invalid.\r\n");
    }
    /* 唤醒host/唤醒ack后关闭唤醒源中断使能. */
    wakeup_in_pin = pm->pm_info[TIOT_PIN_HOST_WAKEUP_DEVICE];
    tiot_board_pin_enable_int(wakeup_in_pin, TIOT_PIN_INT_DISABLE);
    return TIOT_PM_EVTRET_NEXT_STATE;
wakeout_fail:
    tiot_pm_wake_unlock();
    return TIOT_PM_EVTRET_FAIL;
}

#ifdef CONFIG_PM_MANAGE_LOWPOWER
tiot_pm_retcode tiot_pm_slave_sleep_ack_handle(tiot_pm *pm)
{
    uint32_t wakeup_in_pin;
    uint32_t wakeup_out_pin;
    /* When we reporting msg, ignore allow sleep msg. */
    if (pm->work_vote == 1) {
        return TIOT_PM_EVTRET_KEEP_STATE;
    }

    wakeup_out_pin = pm->pm_info[TIOT_PIN_DEVICE_WAKEUP_HOST];
    /* 睡眠前，slave侧拉低唤醒主机管脚. */
    if (wakeup_out_pin != TIOT_PIN_NONE) {
        if (tiot_board_pin_set_level(wakeup_out_pin, TIOT_PIN_LEVEL_LOW) != 0) {
            tiot_print_err("[TIoT][pm]wakeup_out_pin set low fail.\r\n");
            return TIOT_PM_EVTRET_FAIL;
        }
    }
    /* 进睡眠前 打开唤醒源中断使能. */
    wakeup_in_pin = pm->pm_info[TIOT_PIN_HOST_WAKEUP_DEVICE];
    tiot_board_pin_enable_int(wakeup_in_pin, TIOT_PIN_INT_ENABLE);
    tiot_print_info("[TIoT][pm]goes to sleep.\r\n");
    /* 移除否决票 */
    tiot_pm_wake_unlock();
    return TIOT_PM_EVTRET_NEXT_STATE;
}

tiot_pm_retcode tiot_pm_slave_wakeup_ack_handle(tiot_pm *pm)
{
    tiot_unused(pm);
    tiot_print_info("[TIoT][pm]wakeup succ.\r\n");
    return TIOT_PM_EVTRET_NEXT_STATE;
}

tiot_pm_retcode tiot_pm_slave_work_vote_up(tiot_pm *pm)
{
    /* Prevent sleeping while sending data. */
    tiot_pm_wake_lock();
    pm->work_vote = 1;
    return TIOT_PM_EVTRET_KEEP_STATE;
}

tiot_pm_retcode tiot_pm_slave_work_vote_down(tiot_pm *pm)
{
    pm->work_vote = 0;
    tiot_pm_wake_unlock();
    return TIOT_PM_EVTRET_KEEP_STATE;
}
#endif
