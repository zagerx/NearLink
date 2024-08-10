/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: BLE application callback functions.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include "stdint.h"
#include "errcode.h"
#include "osal_msgqueue.h"
#include "../application_task.h"
#include "bts_le_gap.h"
#include "bts_gatt_server.h"
#include "bts_gatt_client.h"

static void ble_enable_cbk(errcode_t status)
{
    if (g_power_on == false) {
        sample_debug_print("power on status: %d\r\n", status);
        if (g_ble_mode == true) {
            app_message_t msg_node = { status, APP_MSG_TYPE_BLE, APP_TASK_RECV_BLE_POWER_ON, 0, 0 };
            osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
        } else if (g_sle_mode == true) {
            app_message_t msg_node = { status, APP_MSG_TYPE_SLE, APP_TASK_RECV_SLE_POWER_ON, 0, 0 };
            osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
        } else {
            sample_debug_print("both ble and sle is off\r\n");
        }
    } else {
        sample_debug_print("ble enable status: %d\r\n", status);
        app_message_t msg_node = { status, APP_MSG_TYPE_BLE, APP_TASK_RECV_BLE_ENABLE, 0, 0 };
        osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
    }
}

static void ble_adv_enable_cbk(uint8_t adv_id, adv_status_t status)
{
    sample_debug_print("ble adv enable handle: %d, status: %d.\r\n", adv_id, status);
}

static void ble_adv_disable_cbk(uint8_t adv_id, adv_status_t status)
{
    sample_debug_print("ble adv disable handle: %d, status: %d.\r\n", adv_id, status);
}

static void ble_adv_terminate_cbk(uint8_t adv_id, adv_status_t status)
{
    sample_debug_print("ble adv terminate handle: %d, status: %d.\r\n", adv_id, status);
    app_message_t msg_node = { status, APP_MSG_TYPE_BLE, APP_TASK_RECV_BLE_ADV_TERMINATE, 0, 0 };
    osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
}

static void ble_connect_change_cbk(uint16_t conn_id, bd_addr_t *addr,
    gap_ble_conn_state_t conn_state, gap_ble_pair_state_t pair_state, gap_ble_disc_reason_t disc_reason)
{
    if (conn_state == GAP_BLE_STATE_CONNECTED) {
        sample_debug_print("ble connected handle: 0x%04x, addr: %02x:%02x:xx:xx:%02x:%02x, pair_state: %d, reason: %d.\r\n",
            conn_id, addr->addr[0], addr->addr[1], addr->addr[4], addr->addr[5], pair_state, disc_reason);
    } else if (conn_state == GAP_BLE_STATE_DISCONNECTED) {
        sample_debug_print("ble disconnected handle: 0x%04x, addr: %02x:%02x:xx:xx:%02x:%02x, pair_state: %d, reason: %d.\r\n",
            conn_id, addr->addr[0], addr->addr[1], addr->addr[4], addr->addr[5], pair_state, disc_reason);
    }
}

void bt_gap_cb_register(void)
{
    gap_ble_callbacks_t gap_cb = { 0 };
    gap_cb.ble_enable_cb = ble_enable_cbk;
    gap_cb.start_adv_cb = ble_adv_enable_cbk;
    gap_cb.stop_adv_cb = ble_adv_disable_cbk;
    gap_cb.terminate_adv_cb = ble_adv_terminate_cbk;
    gap_cb.conn_state_change_cb = ble_connect_change_cbk;
    if (gap_ble_register_callbacks(&gap_cb) != ERRCODE_BT_SUCCESS) {
        sample_debug_print("bt gap register cbk failed\r\n");
    }
}

/* ble模式待开发 */