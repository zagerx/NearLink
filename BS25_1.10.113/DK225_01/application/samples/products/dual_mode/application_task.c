/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: Application task Source.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include <stdbool.h>
#include <stdint.h>
#include "sysdep.h"
#include "watchdog.h"
#include "soc_osal.h"
#include "osal_inner.h"
#include "osal_msgqueue.h"
#include "sle/sle_client.h"
#include "ble/ble_callbacks.h"
#include "sle_device_discovery.h"
#include "bts_le_gap.h"
#include "application_task.h"

#define APPLICATION_TASK_SLEEP 20

unsigned long app_msg_queue = 0; 

bool g_power_on = false;
bool g_ble_mode = false;
bool g_sle_mode = true;

uint8_t *g_msg_data = NULL;

static void application_task_queue_create(void)
{
    int ret = 0;
    ret = osal_msg_queue_create("application_msg", APPLICATION_MSG_QUEUE_SIZE,
        &app_msg_queue, 0, sizeof(app_message_t));
    if (ret != OSAL_SUCCESS) {
        goto task_create_fail;
    }
    g_msg_data = (uint8_t *)osal_kmalloc(sizeof(app_message_t), 0);
    if (g_msg_data == NULL) {
        goto task_create_fail;
    }
    sample_debug_print("create queue success %d \n", app_msg_queue); 
    return;
task_create_fail:
    sample_debug_print("create queue fail %d \n",ret);
}

void *application_task_run(const char *arg)
{
    osal_unused(arg);
    int ret = 0;
    uint32_t msg_data_size = sizeof(app_message_t);
    app_message_t *msg_node = NULL;

    application_task_queue_create();

    while (1) {
        /* 为了防止application_task_run线程一直占用cpu导致log线程走不进去，添加sleep释放cpu，sleep时间可修改 */
        (void)osal_msleep(APPLICATION_TASK_SLEEP);
        ret = osal_msg_queue_read_copy(app_msg_queue, g_msg_data, &msg_data_size, 0);
        if (ret != ERRCODE_SUCC) {
            uapi_watchdog_kick();   /* 若读不到数据，直接喂狗 */
            continue;
        }
        /* 接收消息队列的消息后，将ble与sle消息分流 */
        msg_node = (app_message_t *)g_msg_data;
        if (msg_node->msg_type == APP_MSG_TYPE_BLE) {
            application_task_recv_ble(msg_node->sub_type, msg_node->status,
                msg_node->length, msg_node->data);
        } else if (msg_node->msg_type == APP_MSG_TYPE_SLE) {
            application_task_recv_sle(msg_node->sub_type, msg_node->status,
                msg_node->length, msg_node->data);
        }
    }
}