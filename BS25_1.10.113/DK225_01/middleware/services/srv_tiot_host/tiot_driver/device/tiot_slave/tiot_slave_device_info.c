/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved. \n
 *
 * Description: tiot slave device info. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2024-01-10, Create file. \n
 */
#include "tiot_slave_device_info.h"
#include "tiot_slave_board_port.h"
#include "tiot_pm_slave.h"
#include "tiot_board_uart_port.h"
#include "tiot_board_log.h"
#include "tiot_controller.h"
#include "tiot_sys_msg_handle.h"

#define SLAVE_POWERON_WAIT_MS      0      /* Wait time before power on. */
#define SLAVE_BOOT_TIME_MS         0      /* Power on ==> Boot time. */
#define SLAVE_INIT_TIME_MS         0      /* Boot ==> Init time. */
#define SLAVE_WAKEUP_PULSE_MS      1      /* GPIO wake up pulse. */
#define SLAVE_WAKEUP_WAIT_MS       300     /* Wake up wait ms. */

#define SLAVE_BAUD_CHANGE_WAIT_US  0      /* UART切换波特率等待时间. Baudrate change wait μs. */

static const tiot_pm_event_entry g_slave_pm_event_map[TIOT_PM_EVENT_MAX] = {
    /* Power Off */
    { TIOT_PM_TAG_POWERON, TIOT_PM_STATE_POWEROFF, tiot_pm_slave_power_off },
    /* Power On */
    { TIOT_PM_TAG_POWEROFF, TIOT_PM_STATE_INIT, tiot_pm_slave_power_on },
    /* Host wakeup device. */
    { TIOT_PM_TAG_POWERON, TIOT_PM_STATE_WAKING, tiot_pm_slave_wakeup_handle },
    /* Device recv allow msg. */
    { TIOT_PM_TAG_CAN_SLEEP, TIOT_PM_STATE_SLEEP, tiot_pm_slave_sleep_ack_handle },
    /* Device wakeup host. */
    { TIOT_PM_TAG_CAN_WAKEUP, TIOT_PM_STATE_WORK, tiot_pm_slave_wakeup_ack_handle },
    /* Recv msg from host. */
    { TIOT_PM_TAG_CAN_WORK, TIOT_PM_STATE_WORK, NULL },
    { TIOT_PM_TAG_POWERON, TIOT_PM_STATE_INVALID, tiot_pm_slave_work_vote_up },
    { TIOT_PM_TAG_POWERON, TIOT_PM_STATE_INVALID, tiot_pm_slave_work_vote_down }
};

static int tiot_slave_rx_wait_func(const void *param)
{
    /* 通过UART上报数据，不需要拉高管脚来通知读取 */
    (void)param;
    return 0;
}

static int32_t tiot_slave_cmd_handle_stub(tiot_fw *fw, uint16_t ext_cmd, const uint8_t *cmd, uint16_t len)
{
    tiot_unused(fw);
    tiot_unused(ext_cmd);
    tiot_unused(cmd);
    tiot_unused(len);
    return 0;
}

static tiot_device_info g_tiot_slave_device_info = {
    .pm_event_map = (tiot_pm_event_entry *)g_slave_pm_event_map,
    .pm_event_map_size = sizeof(g_slave_pm_event_map) / sizeof(tiot_pm_event_entry),
    .exec_cbs = { NULL, NULL, tiot_slave_cmd_handle_stub },
    .fw_path = NULL,
    .xfer_info = { TIOT_XMIT_TYPE_UART, tiot_slave_rx_wait_func },
    .pkt_sys_msg_handle = tiot_pkt_sys_msg_slave_handle,
    .timings = {
        .power_on_wait_ms = SLAVE_POWERON_WAIT_MS,
        .boot_time_ms = SLAVE_BOOT_TIME_MS,
        .init_time_ms = SLAVE_INIT_TIME_MS,
        .wakeup_pulse_ms = SLAVE_WAKEUP_PULSE_MS,
        .wakeup_wait_ms = SLAVE_WAKEUP_WAIT_MS,
        .baud_change_wait_us = SLAVE_BAUD_CHANGE_WAIT_US,
    }
};

const tiot_device_info *tiot_slave_device_get_info(void)
{
    return &g_tiot_slave_device_info;
}
