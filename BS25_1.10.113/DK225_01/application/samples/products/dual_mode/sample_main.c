/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: BLE and SLE dual mode main sample source.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include "soc_osal.h"
#include "app_init.h"
#include "bts_le_gap.h"
#include "ble/ble_callbacks.h"
#include "application_task.h"
#if defined(CONFIG_SAMPLE_DUAL_MODE_SLE_SERVER)
#include "sle/sle_server.h"
#elif defined(CONFIG_SAMPLE_DUAL_MODE_SLE_CLIENT)
#include "sle/sle_client.h"
#endif

#define APPLICATION_TASK_STACK_SIZE             0x200
#define APPLICATION_TASK_PRIO                   17      /* 线程优先级高于app_main */

static void dual_mode_main(void)
{
    bt_gap_cb_register();

    sample_debug_print("app main in\r\n");
    osal_task *task_handle = NULL;
    osal_kthread_lock();
    task_handle = osal_kthread_create((osal_kthread_handler)application_task_run, 0, "ApplicationTask",
                                      APPLICATION_TASK_STACK_SIZE);
    if (task_handle != NULL) {
        osal_kthread_set_priority(task_handle, APPLICATION_TASK_PRIO);
        osal_kfree(task_handle);
    }
    osal_kthread_unlock();
}

/* Run the dual mode main. */
/*
 * app_run中运行的函数实际在app_main函数的app_tasks_init中运行，
 * 为了不影响app_main线程的运行，单独开出一个application_task_run线程
 */
app_run(dual_mode_main);