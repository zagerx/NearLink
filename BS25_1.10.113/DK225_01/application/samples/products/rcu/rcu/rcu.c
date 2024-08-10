/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: RCU Source. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-09-21, Create file. \n
 */
#include "securec.h"
#include "soc_osal.h"
#include "common_def.h"
#include "app_init.h"
#include "watchdog.h"
#include "osal_addr.h"
#include "osal_task.h"
#include "osal_msgqueue.h"
#include "keyscan.h"
#include "keyscan_porting.h"
#include "adc.h"
#include "pdm.h"
#include "gpio.h"
#include "pinctrl.h"
#include "pm_pmu.h"
#include "pm_clock.h"
#include "hal_adc.h"
#include "hal_dma.h"
#include "sle_amic/sle_amic_vdt_codec/sle_vdt_codec.h"
#include "sle_amic/sle_amic_vdt_pdm/sle_vdt_pdm.h"
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
#include "sle_errcode.h"
#include "sle_device_discovery.h"
#include "sle_connection_manager.h"
#include "sle_rcu_server/sle_rcu_server.h"
#include "sle_rcu_server/sle_rcu_server_adv.h"
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
#include "ble_rcu_server/ble_rcu_server.h"
#include "ble_rcu_server/ble_rcu_server_adv.h"
#include "ble_rcu_server/ble_hid_rcu_server.h"
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */
#include "bts_le_gap.h"
#include "../ir/ir_nec.h"

#define SLE_RCU_SERVER_TASK_PRIO           24
#define USB_HID_MAX_KEY_LENTH              6
#define KEYSCAN_REPORT_MAX_KEY_NUMS        6
#define CONVERT_DEC_TO_HEX                 16
#define MAX_NUM_OF_DEC                     10
#define LENGTH_OF_KEY_VALUE_STR            5
#define SLE_RCU_PARAM_ARGC_2               2
#define SLE_RCU_PARAM_ARGC_3               3
#define SLE_RCU_PARAM_ARGC_4               4

#define SLE_RCU_STATE_DISCONNECT           0
#define SLE_RCU_STATE_CONNECTED            1

#define RCU_TASK_STACK_SIZE                0xc00
#define RCU_TASK_DURATION_MS               2000
#define SLE_RCU_WAIT_SSAPS_READY           500
#define SLE_RCU_SERVER_DELAY_COUNT         3
#define SLE_ADV_HANDLE_DEFAULT             1
#define SLE_RCU_SERVER_MSG_QUEUE_MAX_SIZE  32
#define SLE_RCU_SERVER_LOG                 "[sle rcu server]"
#define USB_RCU_TASK_DELAY_MS              10

#define RCU_KEY_A                          0x4
#define RCU_KEY_B                          0x5
#define RCU_KEY_C                          0x6
#define RCU_KEY_D                          0x7
#define RCU_KEY_E                          0x8
#define RCU_KEY_F                          0x9
#define RCU_KEY_G                          0xA
#define RCU_KEY_H                          0xB
#define RCU_KEY_I                          0xC
#define RCU_KEY_J                          0xD
#define RCU_KEY_K                          0xE

#define RCU_KEY_APPLIC                     0x65
#define RCU_KEY_ENTER                      0x28
#define RCU_KEY_PAGEUP                     0x4B
#define RCU_KEY_PAGEDN                     0x4E
#define RCU_KEY_RIGHT                      0x4F
#define RCU_KEY_LEFT                       0x50
#define RCU_KEY_DOWN                       0x51
#define RCU_KEY_UP                         0x52

#define IR_NEC_USER_CODE                   0x00
#define IR_NEC_KEY_UP                      0xCA
#define IR_NEC_KEY_DOWN                    0xD2
#define IR_NEC_KEY_RIGHT                   0xC1
#define IR_NEC_KEY_LEFT                    0x99
#define IR_NEC_KEY_SELECT                  0xCE
#define IR_NEC_KEY_BACK                    0x90
#define IR_NEC_KEY_MENU                    0x9D
#define IR_NEC_KEY_POWER                   0x9C
#define IR_NEC_KEY_HOME                    0xCB
#define IR_NEC_KEY_VOLUMEUP                0x80
#define IR_NEC_KEY_VOLUMEDOWN              0x81
#define IR_NEC_KEY_MUTE                    0xDD

#define SLE_VDT_SERVER_LOG                 "[sle vdt server]"
#define ADC_GADC_CHANNEL7                  7
#define ADC_GADC_CHANNEL6                  6
#define SLE_VDT_MIC_OFFSET_16              16
#define SLE_VDT_MIC_OFFSET_24              24
#define SLE_VDT_ENCODE_SIZE                68
#define RING_BUFFER_NUMBER                 4
#define PDM_DMA_TRANSFER_EVENT             1

static bool g_switch_mouse_and_keyboard = true;
static bool g_check_consumer_send = false;
static bool g_check_mouse_send = false;
static bool g_check_keyboard_send = false;

#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER) && !defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
static bool g_switch_sle_and_ble = false;
#else
static bool g_switch_sle_and_ble = true;
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */

static uint32_t g_keyboard_send_count = 0;
static uint16_t g_conn_id = 0;

static uint8_t g_sle_pdm_buffer[CONFIG_USB_UAC_MAX_RECORD_SIZE] = { 0 };
static uint32_t g_pdm_dma_data0[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t g_pdm_dma_data1[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t g_pdm_dma_data2[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t g_pdm_dma_data3[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t *g_pdm_dma_data[RING_BUFFER_NUMBER] = {
    g_pdm_dma_data0, g_pdm_dma_data1, g_pdm_dma_data2, g_pdm_dma_data3};
static uint8_t g_out_encode_data[SLE_VDT_ENCODE_SIZE] = { 0 };
static uint8_t g_write_buffer_state = 0;
static uint8_t g_read_buffer_state = 0;
static osal_event g_dma_event_id;

static uint32_t keystate = 0;
static uint32_t pressdown = 1;
static uint32_t pressup = 0;

#if defined(CONFIG_KEYSCAN_USE_FULL_KEYS_TYPE)
static const uint8_t g_key_map[KEYSCAN_MAX_ROW][KEYSCAN_MAX_COL] = {
    { 0x29, 0x2B, 0x14, 0x35, 0x04, 0x1E, 0x1D, 0x00 },
    { 0x3D, 0x3C, 0x08, 0x3B, 0x07, 0x20, 0x06, 0x00 },
    { 0x00, 0x39, 0x1A, 0x3A, 0x16, 0x1F, 0x1B, 0x00 },
    { 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0xE4, 0x00 },
    { 0x0A, 0x17, 0x15, 0x22, 0x09, 0x21, 0x19, 0x05 },
    { 0x0B, 0x1C, 0x18, 0x23, 0x0D, 0x24, 0x10, 0x11 },
    { 0x3F, 0x30, 0x0C, 0x2E, 0x0E, 0x25, 0x36, 0x00 },
    { 0x00, 0x00, 0x12, 0x40, 0x0F, 0x26, 0x37, 0x00 },
    { 0x34, 0x2F, 0x13, 0x2D, 0x33, 0x27, 0x00, 0x38 },
    { 0x3E, 0x2A, 0x00, 0x41, 0x31, 0x42, 0x28, 0x2C },
    { 0x00, 0x00, 0xE3, 0x00, 0x00, 0x43, 0x00, 0x51 },
    { 0xE2, 0x00, 0x00, 0x00, 0x00, 0x45, 0xE5, 0xE6 },
    { 0x00, 0x53, 0x00, 0x00, 0xE1, 0x44, 0x00, 0x4F },
    { 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x50 },
    { 0x5F, 0x5C, 0x61, 0x5E, 0x59, 0x62, 0x55, 0x5B },
    { 0x54, 0x60, 0x56, 0x57, 0x5D, 0x5A, 0x58, 0x63 },
};
#else
static const uint8_t g_key_map[KEYSCAN_MAX_ROW][KEYSCAN_MAX_COL]  = {
    { 0xD, 0x4 },
    { 0x4F, 0xE },
    { 0x51, 0x52 },
};
#endif /* CONFIG_KEYSCAN_USE_FULL_KEYS_TYPE */

typedef union mouse_key {
    struct {
        uint8_t left_key   : 1;
        uint8_t right_key  : 1;
        uint8_t mid_key    : 1;
        uint8_t reserved   : 5;
    } b;
    uint8_t d8;
} mouse_key_t;

#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
typedef struct usb_hid_sle_mouse_report {
    uint8_t kind;
    mouse_key_t key;
    int8_t x;                 /* A negative value indicates that the mouse moves left. */
    int8_t y;                 /* A negative value indicates that the mouse moves up. */
    int8_t wheel;             /* A negative value indicates that the wheel roll forward. */
} usb_hid_sle_mouse_report_t;

typedef struct usb_hid_sle_keyboard_report {
    uint8_t kind;
    uint8_t special_key;                         /*!< 8bit special key(Lctrl Lshift Lalt Lgui Rctrl Rshift Ralt Rgui) */
    uint8_t reserve;
    uint8_t key[USB_HID_MAX_KEY_LENTH]; /*!< Normal key */
} usb_hid_sle_keyboard_report_t;

typedef struct usb_hid_sle_consumer_report {
    uint8_t kind;
    uint8_t comsumer_key0;
    uint8_t comsumer_key1;
} usb_hid_sle_consumer_report_t;
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */

#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
typedef struct usb_hid_ble_mouse_report {
    mouse_key_t key;
    int8_t x;                 /* A negative value indicates that the mouse moves left. */
    int8_t y;                 /* A negative value indicates that the mouse moves up. */
    int8_t wheel;             /* A negative value indicates that the wheel roll forward. */
} usb_hid_ble_mouse_report_t;

typedef struct usb_hid_ble_keyboard_report {
    uint8_t special_key;                         /*!< 8bit special key(Lctrl Lshift Lalt Lgui Rctrl Rshift Ralt Rgui) */
    uint8_t reserve;
    uint8_t key[USB_HID_MAX_KEY_LENTH]; /*!< Normal key */
} usb_hid_ble_keyboard_report_t;

typedef struct usb_hid_ble_consumer_report {
    uint8_t comsumer_key0;
    uint8_t comsumer_key1;
} usb_hid_ble_consumer_report_t;
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */

#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
static usb_hid_sle_keyboard_report_t g_hid_sle_keyboard_report;
static usb_hid_sle_mouse_report_t g_hid_sle_mouse_report;
static usb_hid_sle_consumer_report_t g_hid_sle_consumer_report;
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */

#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
static usb_hid_ble_keyboard_report_t g_hid_ble_keyboard_report;
static usb_hid_ble_mouse_report_t g_hid_ble_mouse_report;
static usb_hid_ble_consumer_report_t g_hid_ble_consumer_report;
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */

static void sle_usb_vdt_dma_transfer_done_callback(uint8_t intr, uint8_t channel, uintptr_t arg);

#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
static void ssaps_server_read_request_cbk(uint8_t server_id, uint16_t conn_id, ssaps_req_read_cb_t *read_cb_para,
                                          errcode_t status)
{
    unused(server_id);
    unused(conn_id);
    unused(read_cb_para);
    unused(status);
}

static void ssaps_server_write_request_cbk(uint8_t server_id, uint16_t conn_id, ssaps_req_write_cb_t *write_cb_para,
                                           errcode_t status)
{
    unused(server_id);
    unused(conn_id);
    unused(write_cb_para);
    unused(status);
}

static void sle_rcu_keyboard_send_report(uint8_t key_value, bool send_flag)
{
    if (key_value != 0) {
        g_hid_sle_keyboard_report.key[g_keyboard_send_count++] = key_value;
    }
    if (send_flag) {
        g_hid_sle_keyboard_report.kind = 0x1;
        sle_rcu_server_send_report_by_handle((uint8_t *)(uintptr_t)&g_hid_sle_keyboard_report,
                                             sizeof(usb_hid_sle_keyboard_report_t), g_conn_id);
        if (memset_s(&g_hid_sle_keyboard_report, sizeof(g_hid_sle_keyboard_report), 0,
                     sizeof(g_hid_sle_keyboard_report)) != EOK) {
            g_keyboard_send_count = 0;
            return;
        }
        g_keyboard_send_count = 0;
    }
}

static void sle_rcu_mouse_send_report(uint8_t key_value, bool send_flag)
{
    switch (key_value) {
        case RCU_KEY_RIGHT:
            g_hid_sle_mouse_report.x = 0x10;
            break;
        case RCU_KEY_LEFT:
            g_hid_sle_mouse_report.x = 0xF0;
            break;
        case RCU_KEY_DOWN:
            g_hid_sle_mouse_report.y = 0x10;
            break;
        case RCU_KEY_UP:
            g_hid_sle_mouse_report.y = 0xF0;
            break;
        default:
            break;
    }
    if (send_flag) {
        g_hid_sle_mouse_report.kind = 0x4;
        sle_rcu_server_send_report_by_handle((uint8_t *)(uintptr_t)&g_hid_sle_mouse_report,
                                             sizeof(usb_hid_sle_mouse_report_t), g_conn_id);
        if (memset_s(&g_hid_sle_mouse_report, sizeof(g_hid_sle_mouse_report), 0,
                     sizeof(g_hid_sle_mouse_report)) != EOK) {
            return;
        }
    }
}

static void sle_rcu_consumer_send_report(uint8_t key_value)
{
    if (memset_s(&g_hid_sle_consumer_report, sizeof(g_hid_sle_consumer_report), 0,
                 sizeof(g_hid_sle_consumer_report)) != EOK) {
        return;
    }
    g_hid_sle_consumer_report.kind = 0x3;
    switch (key_value) {
        case RCU_KEY_A:
            g_hid_sle_consumer_report.comsumer_key0 = 0xE2;
            g_hid_sle_consumer_report.comsumer_key1 = 0x00;
            break;
        case RCU_KEY_B:
            g_hid_sle_consumer_report.comsumer_key0 = 0x23;
            g_hid_sle_consumer_report.comsumer_key1 = 0x02;
            break;
        case RCU_KEY_C:
            g_hid_sle_consumer_report.comsumer_key0 = 0x24;
            g_hid_sle_consumer_report.comsumer_key1 = 0x02;
            break;
        case RCU_KEY_D:
            g_hid_sle_consumer_report.comsumer_key0 = 0x21;
            g_hid_sle_consumer_report.comsumer_key1 = 0x02;
            break;
        case RCU_KEY_E:
            g_hid_sle_consumer_report.comsumer_key0 = 0xE9;
            g_hid_sle_consumer_report.comsumer_key1 = 0x00;
            break;
        case RCU_KEY_F:
            g_hid_sle_consumer_report.comsumer_key0 = 0xEA;
            g_hid_sle_consumer_report.comsumer_key1 = 0x00;
            break;
        default:
            break;
    }
    sle_rcu_server_send_report_by_handle((uint8_t *)(uintptr_t)&g_hid_sle_consumer_report,
                                         sizeof(usb_hid_sle_consumer_report_t), g_conn_id);
}

static void sle_rcu_system_power_down_send_report(void)
{
    if (memset_s(&g_hid_sle_consumer_report, sizeof(g_hid_sle_consumer_report), 0,
                 sizeof(g_hid_sle_consumer_report)) != EOK) {
        return;
    }
    g_hid_sle_consumer_report.kind = 0x2;
    g_hid_sle_consumer_report.comsumer_key0 = 0x01;
    g_hid_sle_consumer_report.comsumer_key1 = 0x00;
    sle_rcu_server_send_report_by_handle((uint8_t *)(uintptr_t)&g_hid_sle_consumer_report,
                                         sizeof(usb_hid_sle_consumer_report_t), g_conn_id);
}

static void sle_rcu_send_end(void)
{
    if (g_check_consumer_send) {
        if (memset_s(&g_hid_sle_consumer_report, sizeof(g_hid_sle_consumer_report), 0,
                     sizeof(g_hid_sle_consumer_report)) != EOK) {
            return;
        }
        g_hid_sle_consumer_report.kind = 0x3;
        sle_rcu_server_send_report_by_handle((uint8_t *)(uintptr_t)&g_hid_sle_consumer_report,
                                             sizeof(usb_hid_sle_consumer_report_t), g_conn_id);
        g_check_consumer_send = false;
    }
    if (g_check_mouse_send) {
        if (memset_s(&g_hid_sle_mouse_report, sizeof(g_hid_sle_mouse_report), 0,
                     sizeof(g_hid_sle_mouse_report)) != EOK) {
            return;
        }
        g_hid_sle_mouse_report.kind = 0x4;
        sle_rcu_server_send_report_by_handle((uint8_t *)(uintptr_t)&g_hid_sle_mouse_report,
                                             sizeof(usb_hid_sle_mouse_report_t), g_conn_id);
        g_check_mouse_send = false;
    }
    if (g_check_keyboard_send) {
        if (memset_s(&g_hid_sle_keyboard_report, sizeof(g_hid_sle_keyboard_report), 0,
                     sizeof(g_hid_sle_keyboard_report)) != EOK) {
            return;
        }
        g_hid_sle_keyboard_report.kind = 0x1;
        sle_rcu_server_send_report_by_handle((uint8_t *)(uintptr_t)&g_hid_sle_keyboard_report,
                                             sizeof(usb_hid_sle_keyboard_report_t), g_conn_id);
        g_check_keyboard_send = false;
    }
}
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */

#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
static void ble_rcu_keyboard_send_report(uint8_t key_value, bool send_flag)
{
    if (key_value != 0) {
        g_hid_ble_keyboard_report.key[g_keyboard_send_count++] = key_value;
    }
    if (send_flag) {
        ble_hid_rcu_server_send_keyboard_input_report_by_uuid((uint8_t *)(uintptr_t)&g_hid_ble_keyboard_report,
                                                              sizeof(usb_hid_ble_keyboard_report_t), g_conn_id);
        if (memset_s(&g_hid_ble_keyboard_report, sizeof(g_hid_ble_keyboard_report), 0,
                     sizeof(g_hid_ble_keyboard_report)) != EOK) {
            g_keyboard_send_count = 0;
            return;
        }
        g_keyboard_send_count = 0;
    }
}

static void ble_rcu_mouse_send_report(uint8_t key_value, bool send_flag)
{
    switch (key_value) {
        case RCU_KEY_RIGHT:
            g_hid_ble_mouse_report.x = 0x10;
            break;
        case RCU_KEY_LEFT:
            g_hid_ble_mouse_report.x = 0xF0;
            break;
        case RCU_KEY_DOWN:
            g_hid_ble_mouse_report.y = 0x10;
            break;
        case RCU_KEY_UP:
            g_hid_ble_mouse_report.y = 0xF0;
            break;
        default:
            break;
    }
    if (send_flag) {
        ble_hid_rcu_server_send_mouse_input_report_by_uuid((uint8_t *)(uintptr_t)&g_hid_ble_mouse_report,
                                                           sizeof(usb_hid_ble_mouse_report_t), g_conn_id);
        if (memset_s(&g_hid_ble_mouse_report, sizeof(g_hid_ble_mouse_report), 0,
                     sizeof(g_hid_ble_mouse_report)) != EOK) {
            return;
        }
    }
}

static void ble_rcu_consumer_send_report(uint8_t key_value)
{
    if (memset_s(&g_hid_ble_consumer_report, sizeof(g_hid_ble_consumer_report), 0,
                 sizeof(g_hid_ble_consumer_report)) != EOK) {
        return;
    }
    switch (key_value) {
        case RCU_KEY_A:
            g_hid_ble_consumer_report.comsumer_key0 = 0xE2;
            g_hid_ble_consumer_report.comsumer_key1 = 0x00;
            break;
        case RCU_KEY_B:
            g_hid_ble_consumer_report.comsumer_key0 = 0x23;
            g_hid_ble_consumer_report.comsumer_key1 = 0x02;
            break;
        case RCU_KEY_C:
            g_hid_ble_consumer_report.comsumer_key0 = 0x24;
            g_hid_ble_consumer_report.comsumer_key1 = 0x02;
            break;
        case RCU_KEY_D:
            g_hid_ble_consumer_report.comsumer_key0 = 0x21;
            g_hid_ble_consumer_report.comsumer_key1 = 0x02;
            break;
        case RCU_KEY_E:
            g_hid_ble_consumer_report.comsumer_key0 = 0xE9;
            g_hid_ble_consumer_report.comsumer_key1 = 0x00;
            break;
        case RCU_KEY_F:
            g_hid_ble_consumer_report.comsumer_key0 = 0xEA;
            g_hid_ble_consumer_report.comsumer_key1 = 0x00;
            break;
        default:
            break;
    }
    ble_hid_rcu_server_send_consumer_input_report_by_uuid((uint8_t *)(uintptr_t)&g_hid_ble_consumer_report,
                                                          sizeof(usb_hid_ble_consumer_report_t), g_conn_id);
}

static void ble_rcu_system_power_down_send_report(void)
{
    if (memset_s(&g_hid_ble_consumer_report, sizeof(g_hid_ble_consumer_report), 0,
                 sizeof(g_hid_ble_consumer_report)) != EOK) {
        return;
    }
    g_hid_ble_consumer_report.comsumer_key0 = 0x01;
    g_hid_ble_consumer_report.comsumer_key1 = 0x00;
    ble_hid_rcu_server_send_power_input_report_by_uuid((uint8_t *)(uintptr_t)&g_hid_ble_consumer_report,
                                                       sizeof(usb_hid_ble_consumer_report_t), g_conn_id);
}

static void ble_rcu_send_end(void)
{
    if (g_check_consumer_send) {
        if (memset_s(&g_hid_ble_consumer_report, sizeof(g_hid_ble_consumer_report), 0,
                     sizeof(g_hid_ble_consumer_report)) != EOK) {
            return;
        }
        ble_hid_rcu_server_send_consumer_input_report_by_uuid((uint8_t *)(uintptr_t)&g_hid_ble_consumer_report,
                                                              sizeof(usb_hid_ble_consumer_report_t), g_conn_id);
        g_check_consumer_send = false;
    }
    if (g_check_mouse_send) {
        if (memset_s(&g_hid_ble_mouse_report, sizeof(g_hid_ble_mouse_report), 0,
                     sizeof(g_hid_ble_mouse_report)) != EOK) {
            return;
        }
        ble_hid_rcu_server_send_mouse_input_report_by_uuid((uint8_t *)(uintptr_t)&g_hid_ble_mouse_report,
                                                           sizeof(usb_hid_ble_mouse_report_t), g_conn_id);
        g_check_mouse_send = false;
    }
    if (g_check_keyboard_send) {
        if (memset_s(&g_hid_ble_keyboard_report, sizeof(g_hid_ble_keyboard_report), 0,
                     sizeof(g_hid_ble_keyboard_report)) != EOK) {
            return;
        }
        ble_hid_rcu_server_send_keyboard_input_report_by_uuid((uint8_t *)(uintptr_t)&g_hid_ble_keyboard_report,
                                                              sizeof(usb_hid_ble_keyboard_report_t), g_conn_id);
        g_check_keyboard_send = false;
    }
}
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */

static void rcu_keyboard_send_report(uint8_t key_value)
{
    if (g_switch_sle_and_ble) {
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
        sle_rcu_keyboard_send_report(key_value, false);
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
    } else {
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
        ble_rcu_keyboard_send_report(key_value, false);
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */
    }
    g_check_keyboard_send = true;
}

static void rcu_mouse_and_keyboard_send_report(uint8_t key_value)
{
    if (g_switch_sle_and_ble) {
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
        if (g_switch_mouse_and_keyboard) {
            sle_rcu_mouse_send_report(key_value, false);
            g_check_mouse_send = true;
        } else {
            sle_rcu_keyboard_send_report(key_value, false);
            g_check_keyboard_send = true;
        }
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
    } else {
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
        if (g_switch_mouse_and_keyboard) {
            ble_rcu_mouse_send_report(key_value, false);
            g_check_mouse_send = true;
        } else {
            ble_rcu_keyboard_send_report(key_value, false);
            g_check_keyboard_send = true;
        }
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */
    }
}

static void rcu_system_power_down_send_report(void)
{
    if (g_switch_sle_and_ble) {
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
        sle_rcu_system_power_down_send_report();
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
    } else {
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
        ble_rcu_system_power_down_send_report();
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */
    }
}

static void rcu_mouse_and_keyboard_send_start(void)
{
    if (g_switch_sle_and_ble) {
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
        if (g_check_mouse_send) {
            sle_rcu_mouse_send_report(0, true);
        }
        if (g_check_keyboard_send) {
            sle_rcu_keyboard_send_report(0, true);
        }
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
    } else {
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
        if (g_check_mouse_send) {
            ble_rcu_mouse_send_report(0, true);
        }
        if (g_check_keyboard_send) {
            ble_rcu_keyboard_send_report(0, true);
        }
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */
    }
}

static void rcu_send_end(void)
{
    if (g_switch_sle_and_ble) {
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
        sle_rcu_send_end();
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
    } else {
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
        ble_rcu_send_end();
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */
    }
    if (keystate == pressdown) {
        uapi_adc_power_en(AFE_AMIC_MODE, false);
        keystate = pressup;
    }
}

static void rcu_consumer_send_report(uint8_t key_value)
{
    if (g_switch_sle_and_ble) {
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
        sle_rcu_consumer_send_report(key_value);
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
    } else {
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
        ble_rcu_consumer_send_report(key_value);
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */
    }
    g_check_consumer_send = true;
}

#if defined(CONFIG_SAMPLE_SUPPORT_IR)
static void sle_ir_function_switch(uint8_t key)
{
    switch (key) {
        case RCU_KEY_A:
            ir_transmit_nec(IR_NEC_USER_CODE, IR_NEC_KEY_MUTE);
            break;
        case RCU_KEY_B:
            ir_transmit_nec(IR_NEC_USER_CODE, IR_NEC_KEY_HOME);
            break;
        case RCU_KEY_C:
            ir_transmit_nec(IR_NEC_USER_CODE, IR_NEC_KEY_BACK);
            break;
        case RCU_KEY_D:
            ir_transmit_nec(IR_NEC_USER_CODE, IR_NEC_KEY_VOLUMEDOWN);
            break;
        case RCU_KEY_E:
            ir_transmit_nec(IR_NEC_USER_CODE, IR_NEC_KEY_VOLUMEUP);
            break;
        default:
            break;
    }
}
#endif

static int rcu_keyscan_callback(int key_nums, uint8_t key_values[])
{
    rcu_send_end();
    for (uint8_t i = 0; i < key_nums; i++) {
        switch (key_values[i]) {
            case RCU_KEY_A:
            case RCU_KEY_B:
            case RCU_KEY_C:
            case RCU_KEY_D:
            case RCU_KEY_E:
            case RCU_KEY_F:
#if defined(CONFIG_SAMPLE_SUPPORT_IR)
                sle_ir_function_switch(key_values[i]);
#endif
                rcu_consumer_send_report(key_values[i]);
                break;
            case RCU_KEY_H:
                rcu_system_power_down_send_report();
                break;
            case RCU_KEY_APPLIC:
            case RCU_KEY_ENTER:
            case RCU_KEY_PAGEUP:
            case RCU_KEY_PAGEDN:
                rcu_keyboard_send_report(key_values[i]);
                break;
            case RCU_KEY_RIGHT:
            case RCU_KEY_LEFT:
            case RCU_KEY_DOWN:
            case RCU_KEY_UP:
                rcu_mouse_and_keyboard_send_report(key_values[i]);
                break;
            case RCU_KEY_G:
                g_switch_mouse_and_keyboard = !g_switch_mouse_and_keyboard;
                break;
            case RCU_KEY_I:
                g_switch_sle_and_ble = !g_switch_sle_and_ble;
                break;
            case RCU_KEY_J:
                g_conn_id = !g_conn_id;
                break;
            case RCU_KEY_K:
                uapi_adc_power_en(AFE_AMIC_MODE, true);
                keystate = pressdown;
                break;
            default:
                break;
        }
    }
    rcu_mouse_and_keyboard_send_start();
    return 1;
}

static void sle_vdt_set_phy_param(void)
{
    sle_set_phy_t param = { 0 };
    param.tx_format = 1;         // 无线帧类型2
    param.rx_format = 1;         // 无线帧类型2
    param.tx_phy = 0;            // 0 1M 1 2M 2 4M
    param.rx_phy = 0;
    param.tx_pilot_density = 0;  // 导频密度4:1
    param.rx_pilot_density = 0;  // 导频密度4:1
    param.g_feedback = 0;
    param.t_feedback = 0;
    if (sle_set_phy_param(0, &param) != 0) {
        return;
    }
    osal_printk("sle_vdt_set_phy_param ok!\r\n");
}

static void sle_vdt_adc_set_io(pin_t pin)
{
    uapi_pin_set_mode(pin, 0);
    uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT);
    uapi_pin_set_pull(pin, PIN_PULL_NONE);
#if defined(CONFIG_PINCTRL_SUPPORT_IE)
    uapi_pin_set_ie(pin, PIN_IE_1);
#endif
}

static void sle_vdt_adc_init(void)
{
    uapi_pin_init();
    uapi_gpio_init();

    uapi_pmu_control(PMU_CONTROL_MICLDO_POWER, PMU_CONTROL_POWER_ON);
    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_MICLDO, PMU_MICLDO_VSET_2V0);

    sle_vdt_adc_set_io(CONFIG_ADC_USE_PIN1);
    sle_vdt_adc_set_io(CONFIG_ADC_USE_PIN2);
    uapi_adc_init(ADC_CLOCK_NONE);
    uapi_adc_power_en(AFE_AMIC_MODE, true);
    uapi_adc_open_differential_channel(ADC_GADC_CHANNEL7, ADC_GADC_CHANNEL6);
    adc_calibration(AFE_AMIC_MODE, true, true, true);

    return;
}

static void sle_usb_vdt_dma_transfer_restart(void)
{
    g_write_buffer_state = (g_write_buffer_state + 1) % RING_BUFFER_NUMBER;
    if (g_write_buffer_state != g_read_buffer_state) {
        uint32_t ret = osal_event_write(&g_dma_event_id, PDM_DMA_TRANSFER_EVENT);
        if (ret != OSAL_SUCCESS) {
            return;
        }
    }
    if (sle_vdt_pdm_start_dma_transfer(g_pdm_dma_data[g_write_buffer_state],
        sle_usb_vdt_dma_transfer_done_callback) != 0) {
        return;
    }
}

static void sle_usb_vdt_dma_transfer_done_callback(uint8_t intr, uint8_t channel, uintptr_t arg)
{
    unused(channel);
    unused(arg);

    switch (intr) {
        case HAL_DMA_INTERRUPT_TFR:
            sle_usb_vdt_dma_transfer_restart();
            break;
        case HAL_DMA_INTERRUPT_ERR:
            osal_printk("%s DMA transfer error.\r\n", SLE_VDT_SERVER_LOG);
            break;
        default:
            break;
    }
}

static void rcu_amic_init(void)
{
    sle_vdt_set_phy_param();

    sle_vdt_adc_init();
    if (sle_vdt_pdm_init() != 0) {
        osal_printk("%s Init the PDM fail.\r\n", SLE_VDT_SERVER_LOG);
    }
    if (uapi_pdm_start() != ERRCODE_SUCC) {
        osal_printk("%s Start the PDM fail.\r\n", SLE_VDT_SERVER_LOG);
    }

    sle_vdt_codec_init();

    if (sle_vdt_pdm_start_dma_transfer(g_pdm_dma_data[g_write_buffer_state],
        sle_usb_vdt_dma_transfer_done_callback) != 0) {
        return;
    }
}

static void rcu_keyscan_init(void)
{
    /* keyscan init */
    uapi_set_keyscan_value_map((uint8_t **)g_key_map, KEYSCAN_MAX_ROW, KEYSCAN_MAX_COL);
#if defined(CONFIG_KEYSCAN_USE_FULL_KEYS_TYPE)
    keyscan_porting_type_sel(FULL_KEYS_TYPE);
#else
    keyscan_porting_type_sel(SIX_KEYS_TYPE);
#endif
    uapi_keyscan_init(EVERY_ROW_PULSE_40_US, HAL_KEYSCAN_MODE_0, KEYSCAN_INT_VALUE_RDY);
    osal_msleep(RCU_TASK_DURATION_MS);
    uapi_keyscan_register_callback(rcu_keyscan_callback);
    uapi_keyscan_enable();
}

static void *rcu_task(const char *arg)
{
    unused(arg);

    osal_printk("enter rcu_task!\r\n");

    if (osal_event_init(&g_dma_event_id) != OSAL_SUCCESS) {
        osal_printk("%s touch osal_event_init fail! \r\n", SLE_VDT_SERVER_LOG);
    }
    rcu_keyscan_init();
#if defined(CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER)
    ble_rcu_server_init();
#endif /* CONFIG_SAMPLE_SUPPORT_BLE_RCU_SERVER */

#if defined(CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER)
    /* sle server init */
    sle_rcu_server_init(ssaps_server_read_request_cbk, ssaps_server_write_request_cbk);

    while (get_g_ssaps_ready() == false) {
        osal_msleep(RCU_TASK_DURATION_MS);
    }
    while (get_g_conn_update() == 0) {
        osal_msleep(RCU_TASK_DURATION_MS);
    }

    rcu_amic_init();
    uint32_t buffer_filled_count = 0;
    while (1) {
        uapi_watchdog_kick();
        if (get_g_sle_conn_num() == 0 || keystate != pressdown) {
            continue;
        }
        uint32_t ret = osal_event_read(&g_dma_event_id, PDM_DMA_TRANSFER_EVENT, OSAL_WAIT_FOREVER,
                                       OSAL_WAITMODE_AND | OSAL_WAITMODE_CLR);
        if (ret & PDM_DMA_TRANSFER_EVENT) {
            for (uint32_t i = 0; i < CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA; i++) {
                g_sle_pdm_buffer[buffer_filled_count++] =
                    (uint8_t)(g_pdm_dma_data[g_read_buffer_state][i] >> SLE_VDT_MIC_OFFSET_16);
                g_sle_pdm_buffer[buffer_filled_count++] =
                    (uint8_t)(g_pdm_dma_data[g_read_buffer_state][i] >> SLE_VDT_MIC_OFFSET_24);
            }
            g_read_buffer_state = (g_read_buffer_state + 1) % RING_BUFFER_NUMBER;
            buffer_filled_count = 0;
            uint8_t *out_data = g_out_encode_data;
            uint32_t encode_data_len = sle_vdt_codec_encode(g_sle_pdm_buffer, &out_data);
            sle_rcu_amic_server_send_report_by_handle(out_data, encode_data_len, 0);
        }
    }
    uapi_keyscan_deinit();
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_RCU_SERVER */
    return NULL;
}

static void rcu_entry(void)
{
    osal_task *task_handle = NULL;

    if (uapi_clock_control(CLOCK_CLKEN_ID_MCU_CORE, CLOCK_FREQ_LEVEL_HIGH) == ERRCODE_SUCC) {
        osal_printk("Config succ.\r\n");
    } else {
        osal_printk("Config fail.\r\n");
    }

    osal_kthread_lock();
    task_handle = osal_kthread_create((osal_kthread_handler)rcu_task, 0, "SleRcuSrverTask",
                                      RCU_TASK_STACK_SIZE);
    if (task_handle != NULL) {
        osal_kthread_set_priority(task_handle, SLE_RCU_SERVER_TASK_PRIO);
        osal_kfree(task_handle);
    }
    osal_kthread_unlock();
}

/* Run the rcu_entry. */
app_run(rcu_entry);