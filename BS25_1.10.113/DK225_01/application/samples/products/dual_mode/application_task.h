/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: application task header file.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#ifndef APPLICATION_TASK_H
#define APPLICATION_TASK_H

#include <stdbool.h>
#include "errcode.h"
#include "sample_main.h"

#define APPLICATION_MSG_QUEUE_SIZE  15

extern unsigned long app_msg_queue;
extern bool g_power_on;
extern bool g_ble_mode;
extern bool g_sle_mode;

/* APP消息类型 */
typedef enum {
    APP_MSG_TYPE_BLE,
    APP_MSG_TYPE_SLE,
} app_msg_type_t;

/* ble回调消息码 */
typedef enum {
    APP_TASK_RECV_BLE_POWER_ON,
    APP_TASK_RECV_BLE_ENABLE,
    APP_TASK_RECV_BLE_DISABLE,
    APP_TASK_RECV_BLE_ADV_ENABLE,
    APP_TASK_RECV_BLE_ADV_DISABLE,
    APP_TASK_RECV_BLE_ADV_TERMINATE,
    APP_TASK_RECV_BLE_SCAN_ENABLE,
    APP_TASK_RECV_BLE_SCAN_DISABLE,
    APP_TASK_RECV_BLE_SCAN_REPORT,
    APP_TASK_RECV_BLE_CONNECT,
    APP_TASK_RECV_BLE_DISCONNECT,
    APP_TASK_RECV_BLE_AUTH_CMP,
    APP_TASK_RECV_BLE_PAIRED,
    APP_TASK_RECV_BLE_GATT_READ_REQ,
    APP_TASK_RECV_BLE_GATT_WRITE_REQ,
} app_task_type_ble_t;

/* sle回调消息码 */
typedef enum {
    APP_TASK_RECV_SLE_POWER_ON,
    APP_TASK_RECV_SLE_ENABLE,
    APP_TASK_RECV_SLE_DISABLE,
    APP_TASK_RECV_SLE_ADV_ENABLE,
    APP_TASK_RECV_SLE_ADV_DISABLE,
    APP_TASK_RECV_SLE_ADV_TERMINATE,
    APP_TASK_RECV_SLE_SCAN_ENABLE,
    APP_TASK_RECV_SLE_SCAN_DISABLE,
    APP_TASK_RECV_SLE_SCAN_REPORT,
    APP_TASK_RECV_SLE_CONNECT,
    APP_TASK_RECV_SLE_DISCONNECT,
    APP_TASK_RECV_SLE_AUTH_CMP,
    APP_TASK_RECV_SLE_PAIRED,
    APP_TASK_RECV_SLE_SSAPS_READ_REQ,
    APP_TASK_RECV_SLE_SSAPS_WRITE_REQ,
    APP_TASK_RECV_SLE_SSAPC_FIND_SERVICE,
    APP_TASK_RECV_SLE_SSAPC_FIND_PROPERTY,
    APP_TASK_RECV_SLE_LOW_LATENCY,
} app_task_type_sle_t;

typedef struct{
    errcode_t status;
    uint8_t   msg_type; /* 参考app_msg_type_t */
    uint8_t   sub_type; /* 参考app_task_type_ble_t以及app_task_type_sle_t */
    uint16_t  length;
    uint8_t  *data;
} app_message_t;

void *application_task_run(const char *arg);
void application_task_recv_ble(uint8_t type, uint8_t status, uint16_t length, uint8_t *param);
void application_task_recv_sle(uint8_t type, uint8_t status, uint16_t length, uint8_t *param);

#endif /* APPLICATION_TASK_H */