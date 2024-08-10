/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: SLE client application task Source.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include "stdint.h"
#include "soc_osal.h"
#include "osal_inner.h"
#include "application_task.h"
#include "sle/sle_client.h"
#include "sle_device_discovery.h"
#include "sle_connection_manager.h"

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
            sle_ssapc_register_cbks();
            break;
        case APP_TASK_RECV_SLE_ENABLE:
            /*
             * options:
             *   1. 注册client
             *   2. 启动扫描
             *   3. low latency回调注册，dongle/rx模式使能
             */
            sle_register_client();
            sle_client_start_scan();
            break;
        case APP_TASK_RECV_SLE_SCAN_REPORT:
            /*
             * options:
             *   1. 设置默认扫描参数
             *   2. 启动连接
             */
            sle_set_default_conn_param();
            sle_connect_remote_device((sle_addr_t *)param);
            break;
        case APP_TASK_RECV_SLE_CONNECT:
            /*
             * options:
             *   1. 连接参数更新
             *   2. 与对端设备配对/加密
             *   3. 使能low latency，调用接口sle_low_latency_set，此接口只可在client端调用
             */
            /*
             * sle_pair_remote_device在每次连接后必须调用，
             * 如果对端设备是未配对过的设备，那么配对后起加密，
             * 如果对端设备是已经配对的设备，那么直接起加密
             */
            sle_pair_remote_device((sle_addr_t *)param); 
            break;
        case APP_TASK_RECV_SLE_DISCONNECT:
            /*
             * options:
             *   1. 重新起扫描
             *   2. 进入低功耗
             *   3. 如果已使能low latency，那么在断连后必须disable low latency，使用接口sle_low_latency_set，两端皆需要调用
             */
            sle_client_start_scan();
            break;
        case APP_TASK_RECV_SLE_PAIRED:
            /*
             * options:
             *   1. 服务发现，服务发现注意，必须先发现service，后发现property，不可一次性发现全部property
             *   2. 使能low latency，调用接口sle_low_latency_set，此接口只可在client端调用
             */
            sle_client_find_service();
            break;
        case APP_TASK_RECV_SLE_SSAPC_FIND_SERVICE:
            /*
             * options:
             *   1. property或者descriptor发现
             */
            sle_client_find_property((app_ssapc_find_service_t *)param);
            break;
        case APP_TASK_RECV_SLE_LOW_LATENCY:
            break;
        default:
            break;
    }
    if (param != NULL) {
        osal_kfree(param);
    }
}