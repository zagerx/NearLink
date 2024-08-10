/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: BLE MOUSE Sample Source. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-08-20, Create file. \n
 */
#include "cmsis_os2.h"
#include "soc_osal.h"
#include "common_def.h"
#include "osal_debug.h"
#include "app_init.h"
#include "bts_le_gap.h"
#include "ble_mouse_server/ble_mouse_server_adv.h"
#include "ble_mouse_server/ble_mouse_server.h"
#include "ble_mouse_server/ble_hid_mouse_server.h"
#include "mouse_sensor/mouse_sensor.h"
#include "mouse_wheel/mouse_wheel.h"
#include "mouse_button/mouse_button.h"
#include "mouse_init.h"

#define BLE_MOUSE_TASK_STACK_SIZE            0x800
#define BLE_MOUSE_TASK_PRIO                  (osPriority_t)(13)
static void *ble_mouse_task(const char *arg)
{
    unused(arg);
    bt_core_enable_cb_register();
    ble_mouse_init();
    osal_printk("ble mouse init ok\r\n");
    return NULL;
}

static void ble_mouse_entry(void)
{
    osThreadAttr_t attr;

    attr.name = "BLEmouseTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = BLE_MOUSE_TASK_STACK_SIZE;
    attr.priority = BLE_MOUSE_TASK_PRIO;
    if (osThreadNew((osThreadFunc_t)ble_mouse_task, NULL, &attr) == NULL) {
        /* Create task fail. */
    }
}

/* Run the ble_mouse_entry. */
app_run(ble_mouse_entry);