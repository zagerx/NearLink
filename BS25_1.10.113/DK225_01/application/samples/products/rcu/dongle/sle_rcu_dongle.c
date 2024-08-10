/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: SLE RCU Dongle Source. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-09-21, Create file. \n
 */
#include "securec.h"
#include "soc_osal.h"
#include "chip_io.h"
#include "common_def.h"
#include "app_init.h"
#include "gadget/f_hid.h"
#include "gadget/f_uac.h"
#include "osal_debug.h"
#include "watchdog.h"
#include "pm_clock.h"
#include "osal_task.h"
#include "implementation/usb_init.h"
#include "sle_connection_manager.h"
#include "sle_ssap_client.h"
#include "sle_rcu_client.h"
#include "sle_rcu_hid.h"
#include "../rcu/sle_amic/sle_amic_vdt_codec/sle_vdt_codec.h"

#define SLE_RCU_DONGLE_TASK_STACK_SIZE      0x1000
#define SLE_RCU_DONGLE_TASK_PRIO            24
#define SLE_RCU_DONGLE_TASK_DELAY_MS        2000
#define USB_HID_RCU_INIT_DELAY_MS           (500UL)
#define USB_RCU_KEYBOARD_REPORTER_LEN       9
#define USB_RCU_MOUSE_REPORTER_LEN          5
#define USB_RCU_CONSUMER_REPORTER_LEN       3
#define SLE_KRYBOARD_USB_MANUFACTURER       { 'H', 0, 'H', 0, 'H', 0, 'H', 0, 'l', 0, 'i', 0, 'c', 0, 'o', 0, 'n', 0 }
#define SLE_KRYBOARD_USB_MANUFACTURER_LEN   20
#define SLE_KRYBOARD_USB_PRODUCT    { 'H', 0, 'H', 0, '6', 0, '6', 0, '6', 0, '6', 0, ' ', 0, 'U', 0, 'S', 0, 'B', 0 }
#define SLE_KRYBOARD_USB_PRODUCT_LEN        22
#define SLE_KRYBOARD_USB_SERIAL             { '2', 0, '0', 0, '2', 0, '0', 0, '0', 0, '6', 0, '2', 0, '4', 0 }
#define SLE_KRYBOARD_USB_SERIAL_LEN         16
#define RECV_MAX_LENGTH                     13
#define USB_RECV_STACK_SIZE                 0x400
#define SLE_RCU_WAIT_SSAPS_READY            500
#define SLE_RCU_DONGLE_LOG                  "[sle rcu dongle]"

#define RECV_BUFFER_LENGTH                  4
#define SLE_VDT_DECODE_SIZE                 1024
#define SLE_VDT_TRANSFER_SIZE               96
#define SLE_RCU_PROPERTY_HANDLE             2

static uint8_t g_sle_usb_buffer[SLE_VDT_DECODE_SIZE] = { 0 };
static uint8_t g_out_decode_data1[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t g_out_decode_data2[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t g_out_decode_data3[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t g_out_decode_data4[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t *g_out_decode_data[RECV_BUFFER_LENGTH] = {
    g_out_decode_data1,
    g_out_decode_data2,
    g_out_decode_data3,
    g_out_decode_data4};
uint8_t g_write_index = 0;
uint8_t g_read_index = 0;

static bool g_sle_rcu_dongle_inited = false;
static uint32_t g_sle_rcu_dongle_hid_index = 0;

static void sle_rcu_keyboard_dongle_send_data(usb_hid_rcu_keyboard_report_t *rpt)
{
    if (rpt == NULL) {
        return;
    }
    rpt->kind = 0x1;
    int32_t ret = fhid_send_data(g_sle_rcu_dongle_hid_index, (char *)rpt, USB_RCU_KEYBOARD_REPORTER_LEN);
    if (ret == -1) {
        osal_printk("%s send data falied! ret:%d\n", SLE_RCU_DONGLE_LOG, ret);
        return;
    }
}

static void sle_rcu_mouse_dongle_send_data(usb_hid_rcu_mouse_report_t *rpt)
{
    if (rpt == NULL) {
        return;
    }
    rpt->kind = 0x4;
    int32_t ret = fhid_send_data(g_sle_rcu_dongle_hid_index, (char *)rpt, USB_RCU_MOUSE_REPORTER_LEN);
    if (ret == -1) {
        osal_printk("%s send data falied! ret:%d\n", SLE_RCU_DONGLE_LOG, ret);
        return;
    }
}

static void sle_rcu_consumer_dongle_send_data(usb_hid_rcu_consumer_report_t *rpt)
{
    if (rpt == NULL) {
        return;
    }
    int32_t ret = fhid_send_data(g_sle_rcu_dongle_hid_index, (char *)rpt, USB_RCU_CONSUMER_REPORTER_LEN);
    if (ret == -1) {
        osal_printk("%s send data falied! ret:%d\n", SLE_RCU_DONGLE_LOG, ret);
        return;
    }
}

static uint8_t sle_rcu_dongle_init_internal(device_type dtype)
{
    if (g_sle_rcu_dongle_inited) {
        return SLE_RCU_DONGLE_OK;
    }

    const char manufacturer[SLE_KRYBOARD_USB_MANUFACTURER_LEN] = SLE_KRYBOARD_USB_MANUFACTURER;
    struct device_string str_manufacturer = {
        .str = manufacturer,
        .len = SLE_KRYBOARD_USB_MANUFACTURER_LEN
    };

    const char product[SLE_KRYBOARD_USB_PRODUCT_LEN] = SLE_KRYBOARD_USB_PRODUCT;
    struct device_string str_product = {
        .str = product,
        .len = SLE_KRYBOARD_USB_PRODUCT_LEN
    };

    const char serial[SLE_KRYBOARD_USB_SERIAL_LEN] = SLE_KRYBOARD_USB_SERIAL;
    struct device_string str_serial_number = {
        .str = serial,
        .len = SLE_KRYBOARD_USB_SERIAL_LEN
    };

    struct device_id dev_id = {
        .vendor_id = 0x1111,
        .product_id = 0x0009,
        .release_num = 0x0800
    };

    if (dtype == DEV_UAC_HID) {
        g_sle_rcu_dongle_hid_index = sle_rcu_dongle_set_report_desc_hid();
    }

    if (usbd_set_device_info(dtype, &str_manufacturer, &str_product, &str_serial_number, dev_id) != 0) {
        osal_printk("%s set device info fail!\r\n", SLE_RCU_DONGLE_LOG);
        return SLE_RCU_DONGLE_FAILED;
    }

    if (usb_init(DEVICE, dtype) != 0) {
        osal_printk("%s usb_init failed!\r\n", SLE_RCU_DONGLE_LOG);
        return SLE_RCU_DONGLE_FAILED;
    }

    if (uac_wait_host(UAC_WAIT_HOST_FOREVER) != 0) {
        osal_printk("%s host can`t connect\r\n", SLE_RCU_DONGLE_LOG);
        return SLE_RCU_DONGLE_FAILED;
    }
    g_sle_rcu_dongle_inited = true;
    return SLE_RCU_DONGLE_OK;
}

static int32_t sle_vdt_usb_uac_send_data(const uint8_t *buf, int len)
{
    return fuac_send_message((void *)(uintptr_t)buf, len);
}

static uint8_t sle_rcu_dongle_init(void)
{
    if (!g_sle_rcu_dongle_inited) {
        if (sle_rcu_dongle_init_internal(DEV_UAC_HID) != SLE_RCU_DONGLE_OK) {
            return SLE_RCU_DONGLE_FAILED;
        }
        osal_msleep(USB_HID_RCU_INIT_DELAY_MS);
    }
    return SLE_RCU_DONGLE_OK;
}

static void sle_rcu_notification_cb(uint8_t client_id, uint16_t conn_id, ssapc_handle_value_t *data, errcode_t status)
{
    unused(client_id);
    unused(conn_id);
    unused(status);
    if (data == NULL || data->data_len == 0 || data->data == NULL) {
        return;
    }

    if (data -> handle == SLE_RCU_PROPERTY_HANDLE) {
        if (data->data_len == USB_RCU_KEYBOARD_REPORTER_LEN) {
            sle_rcu_keyboard_dongle_send_data((usb_hid_rcu_keyboard_report_t *)data->data);
        } else if (data->data_len == USB_RCU_MOUSE_REPORTER_LEN) {
            sle_rcu_mouse_dongle_send_data((usb_hid_rcu_mouse_report_t *)data->data);
        } else if (data->data_len == USB_RCU_CONSUMER_REPORTER_LEN) {
            sle_rcu_consumer_dongle_send_data((usb_hid_rcu_consumer_report_t *)data->data);
        }
    } else {
        if ((g_write_index + 1) % RECV_BUFFER_LENGTH != g_read_index) {
            (void)memcpy_s(g_out_decode_data[g_write_index], data->data_len, data->data, data->data_len);
            g_write_index = (g_write_index + 1) % RECV_BUFFER_LENGTH;
        }
    }
}

static void sle_rcu_indication_cb(uint8_t client_id, uint16_t conn_id, ssapc_handle_value_t *data, errcode_t status)
{
    unused(client_id);
    unused(conn_id);
    unused(status);
    if (data == NULL || data->data_len == 0 || data->data == NULL) {
        return;
    }
    if (data -> handle == SLE_RCU_PROPERTY_HANDLE) {
        if (data->data_len == USB_RCU_KEYBOARD_REPORTER_LEN) {
            sle_rcu_keyboard_dongle_send_data((usb_hid_rcu_keyboard_report_t *)data->data);
        } else if (data->data_len == USB_RCU_MOUSE_REPORTER_LEN) {
            sle_rcu_mouse_dongle_send_data((usb_hid_rcu_mouse_report_t *)data->data);
        } else if (data->data_len == USB_RCU_CONSUMER_REPORTER_LEN) {
            sle_rcu_consumer_dongle_send_data((usb_hid_rcu_consumer_report_t *)data->data);
        }
    } else {
        if ((g_write_index + 1) % RECV_BUFFER_LENGTH != g_read_index) {
            (void)memcpy_s(g_out_decode_data[g_write_index], data->data_len, data->data, data->data_len);
            g_write_index = (g_write_index + 1) % RECV_BUFFER_LENGTH;
        }
    }
}

static void *sle_rcu_dongle_task(const char *arg)
{
    unused(arg);
    uint8_t ret;

    osal_printk("%s enter sle_rcu_dongle_task\r\n", SLE_RCU_DONGLE_LOG);
    /* sle rcu dongle init */
    ret = sle_rcu_dongle_init();
    if (ret != SLE_RCU_DONGLE_OK) {
        osal_printk("%s sle_rcu_dongle_init fail! ret = %d\r\n", SLE_RCU_DONGLE_LOG, ret);
    }
    /* sle rcu client init */
    sle_rcu_client_init(sle_rcu_notification_cb, sle_rcu_indication_cb);
    sle_vdt_codec_init();
    while (get_ssap_find_ready() == 0) {
        osal_msleep(SLE_RCU_WAIT_SSAPS_READY);
    }
    osal_printk("%s get_g_ssap_find_ready.\r\n", SLE_RCU_DONGLE_LOG);
    /* delay for param update complete */
    osal_msleep(SLE_RCU_DONGLE_TASK_DELAY_MS);

    while (1) {
        uapi_watchdog_kick();
        if (g_read_index != g_write_index) {
            uint8_t *out_put_data = g_sle_usb_buffer;
            uint32_t decode_data_len = sle_vdt_codec_decode(g_out_decode_data[g_read_index], &out_put_data);
            if ((sle_vdt_usb_uac_send_data(out_put_data, decode_data_len) != 0)) {
                osal_printk("%s Send UAV to USB fail.\r\n", SLE_RCU_DONGLE_LOG);
            }
            g_read_index = (g_read_index + 1) % RECV_BUFFER_LENGTH;
        } else {
        }
    }
    return NULL;
}

static void sle_rcu_dongle_entry(void)
{
    osal_task *task_handle = NULL;
    if (uapi_clock_control(CLOCK_CLKEN_ID_MCU_CORE, CLOCK_FREQ_LEVEL_HIGH) != ERRCODE_SUCC) {
        return;
    }
    osal_printk("Config succ.\r\n");
    osal_kthread_lock();
    task_handle = osal_kthread_create((osal_kthread_handler)sle_rcu_dongle_task, 0, "SleRcuDongleTask",
                                      SLE_RCU_DONGLE_TASK_STACK_SIZE);
    if (task_handle != NULL) {
        osal_kthread_set_priority(task_handle, SLE_RCU_DONGLE_TASK_PRIO);
        osal_kfree(task_handle);
    }
    osal_kthread_unlock();
}

/* Run the sle_rcu_dongle_entry. */
app_run(sle_rcu_dongle_entry);