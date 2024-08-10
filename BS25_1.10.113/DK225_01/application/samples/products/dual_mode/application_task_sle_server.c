/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: SLE server application task Source.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include "stdint.h"
#include "soc_osal.h"
#include "osal_inner.h"
#include "application_task.h"
#include "sle/sle_server.h"
#include "sle_device_discovery.h"

void application_task_recv_sle(uint8_t type, uint8_t status, uint16_t length, uint8_t *param)
{
    osal_unused(status);
    osal_unused(length);
    switch (type) {
        case APP_TASK_RECV_SLE_POWER_ON:
            /* 上电后需要注册回调，并且使能sle，注意enable_sle_cb回调注册需要先于enable_sle */
            sle_dd_register_cbks();
            enable_sle();
            sle_cm_register_cbks();
            sle_ssaps_register_cbks();
            break;
        case APP_TASK_RECV_SLE_ENABLE:
            /*
             * options:
             *   1. 注册server
             *   2. 服务注册，可使用同步接口或者异步接口，异步接口依赖回调进行嵌套的注册
             *   3. 启动普通广播/定向广播
             *   4. low latency回调注册，mouse/tx模式使能
             */
            sle_register_server();
            sle_server_mouse_service_add();
            sle_server_dis_service_add();
            sle_server_adv_init();
            break;
        case APP_TASK_RECV_SLE_ADV_TERMINATE:
            /*
             * options:
             *   1. 启动普通广播/定向广播
             */
            sle_server_direct_adv_init();
            break;
        case APP_TASK_RECV_SLE_DISCONNECT:
            /*
             * options:
             *   1. 启动普通广播/定向广播
             *   2. 如果已使能low latency，那么在断连后必须disable low latency，使用接口sle_low_latency_set，两端皆需要调用
             */
            sle_server_direct_adv_init();
            break;
        case APP_TASK_RECV_SLE_PAIRED:
            break;
        default:
            break;
    }
    if (param != NULL) {
        osal_kfree(param);
    }
}