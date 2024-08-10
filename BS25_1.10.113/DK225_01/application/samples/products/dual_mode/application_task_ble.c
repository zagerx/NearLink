/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: BLE application task Source.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include "stdint.h"
#include "soc_osal.h"
#include "osal_inner.h"
#include "application_task.h"
#include "ble/ble_callbacks.h"
#include "bts_le_gap.h"

void application_task_recv_ble(uint8_t type, uint8_t status, uint16_t length, uint8_t *param)
{
    osal_unused(status);
    osal_unused(length);
    osal_unused(param);
    switch (type) {
        case APP_TASK_RECV_BLE_POWER_ON:
            enable_ble();
            break;
        case APP_TASK_RECV_BLE_ENABLE:
            // 服务注册
            // 设置广播参数
            // 起广播
            break;
        case APP_TASK_RECV_BLE_ADV_TERMINATE:
            // 重新起广播
            break;
        case APP_TASK_RECV_BLE_GATT_READ_REQ:
            // 重新起广播
            break;
        case APP_TASK_RECV_BLE_GATT_WRITE_REQ:
            // 重新起广播
            break;
        default:
            break;
    }
}