/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: SLE Server Source.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include <stdint.h>
#include "securec.h"
#include "osal_inner.h"
#include "../application_task.h"
#include "sle_errcode.h"
#include "sle_ssap_server.h"
#include "sle_device_discovery.h"
#include "sle_server.h"

/*******************************************************
                    广播参数宏定义
*******************************************************/

#define NAME_MAX_LENGTH                           15
/* 连接调度间隔12.5ms，单位125us */
#define SLE_CONN_INTV_MIN_DEFAULT                 0x64
/* 连接调度间隔12.5ms，单位125us */
#define SLE_CONN_INTV_MAX_DEFAULT                 0x64
/* 连接调度间隔25ms，单位125us */
#define SLE_ADV_INTERVAL_MIN_DEFAULT              0xC8
/* 连接调度间隔25ms，单位125us */
#define SLE_ADV_INTERVAL_MAX_DEFAULT              0xC8
/* 超时时间5000ms，单位10ms */
#define SLE_CONN_SUPERVISION_TIMEOUT_DEFAULT      0x1F4
/* 超时时间4990ms，单位10ms */
#define SLE_CONN_MAX_LATENCY                      0x1F3
/* 广播发送功率 */
#define SLE_ADV_TX_POWER                          10
/* 广播ID */
#define SLE_ADV_HANDLE_DEFAULT                    1
/* 最大广播数据长度 */
#define SLE_ADV_DATA_LEN_MAX                      251

#define SLE_ADV_DATA_LEN 30
#define SLE_ADV_RSP_DATA_LEN 12
#define BD_ADDR_LEN6 6

typedef enum sle_adv_channel {
    SLE_ADV_CHANNEL_MAP_77                 = 0x01,
    SLE_ADV_CHANNEL_MAP_78                 = 0x02,
    SLE_ADV_CHANNEL_MAP_79                 = 0x04,
    SLE_ADV_CHANNEL_MAP_DEFAULT            = 0x07
} sle_adv_channel_map_t;

static uint8_t g_sle_adv_data[SLE_ADV_DATA_LEN] = {
    // flag
    0x01,
    0x01, 0x01,
    0x05,
    0x04, 0x0B, 0x06, 0x09, 0x06,
    // appearance
    0x03,
    0x12, 0x09, 0x06, 0x07, 0x03, 0x02, 0x05, 0x00,
    0x06, 0x0A,
    'd', 'u', 'a', 'l', '_', 'm', 'o', 'u', 's', 'e'
};

static uint8_t g_sle_adv_rsp_data[SLE_ADV_RSP_DATA_LEN] = {
    0x0B,
    0x0A, 'd', 'u', 'a', 'l', '_', 'm', 'o', 'u', 's', 'e',
};

static uint8_t g_sle_mac_addr[BD_ADDR_LEN6] = {0x33, 0x22, 0x44, 0x66, 0x55, 0x14};

/*******************************************************
                    SSAP参数宏定义
*******************************************************/

#define APP_UUID_LEN                2
#define HID_ELEMENT_NUM             6
#define DIS_ELEMENT_NUM             4
#define SLE_MOUSE_REPORT_LENGTH     4
#define SLE_SRV_ENCODED_REPORT_LEN  8
#define MANUFACTURE_PNP_ID          (uint8_t *)"HUAWEI-103F-12D1-0001"
#define MANUFACTURE_PNP_ID_LENGTH   21
#define MOUSE_APPEARANCE_LENGTH     3

typedef enum {
    SLE_UUID_INDEX0, // hid service
    SLE_UUID_INDEX1, // input report
    SLE_UUID_INDEX2, // CCCD
    SLE_UUID_INDEX3, // Report Reference
    SLE_UUID_INDEX4, // Report Map
    SLE_UUID_INDEX5, // Hid Control Point
} sle_uuid_index_t;

typedef enum {
    SLE_DIS_INDEX0, // dis service
    SLE_DIS_INDEX1, // name
    SLE_DIS_INDEX2, // appearance
    SLE_DIS_INDEX3, // pnp id
} sle_dis_index_t;

static uint8_t g_sle_hid_control_point = 1;

static uint8_t g_app_uuid[APP_UUID_LEN] = {0x0, 0x0};
static uint8_t g_app_server_id = 0;
static uint8_t g_cccd[2] = {0x00, 0x0};

static sle_item_handle_t g_service_hdl[HID_ELEMENT_NUM]              = {0};
static sle_item_handle_t g_dis_service_hdl[DIS_ELEMENT_NUM]          = {0};
static uint8_t g_sle_input_report[SLE_MOUSE_REPORT_LENGTH]           = {0};
static uint8_t g_input_report_descriptor[SLE_SRV_ENCODED_REPORT_LEN] = {0};
static uint8_t g_local_device_name[] = { 'd', 'u', 'a', 'l', '_', 'm', 'o', 'u', 's', 'e' };
static sle_addr_t g_remote_addr = {0};

static uint8_t g_hid_service_property[HID_ELEMENT_NUM] = {
    0,
    SSAP_OPERATE_INDICATION_BIT_READ | SSAP_OPERATE_INDICATION_BIT_NOTIFY,
    SSAP_OPERATE_INDICATION_BIT_READ | SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP | SSAP_OPERATE_INDICATION_BIT_WRITE,
    SSAP_OPERATE_INDICATION_BIT_READ | SSAP_OPERATE_INDICATION_BIT_WRITE,
    SSAP_OPERATE_INDICATION_BIT_READ | SSAP_OPERATE_INDICATION_BIT_NOTIFY,
    SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP,
};

static uint8_t g_sle_hid_group_uuid[HID_ELEMENT_NUM][SLE_UUID_LEN] = {
    /* Human Interface Device service UUID. */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0B },
    /* Report characteristic UUID. 输入报告信息 */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3C },
    /* CCCD */
    { 0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
      0x00, 0x10, 0x00, 0x00, 0x02, 0x29, 0x00, 0x00 },
    /* Report Reference characteristic UUID. 报告索引信息 */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3B },
    /* Report Map characteristic UUID. 类型和格式描述 */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x39 },
    /* Hid Control Point characteristic UUID.  工作状态指示 */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3A },
};

static uint8_t g_sle_report_map_datas[] = {
    0x00,                       /* type indicate */
    0x05, 0x01,                 /* Usage Page (Generic Desktop)             */
    0x09, 0x02,                 /* Usage (Mouse)                            */
    0xA1, 0x01,                 /* Collection (Application)                 */
    0x09, 0x01,                 /*  Usage (Pointer)                         */
    0xA1, 0x00,                 /*  Collection (Physical)                   */
    0x85, 0x01,                 /*   Report ID  */
    0x05, 0x09,                 /*      Usage Page (Buttons)                */
    0x19, 0x01,                 /*      Usage Minimum (01)                  */
    0x29, 0x03,                 /*      Usage Maximum (03)                  */
    0x15, 0x00,                 /*      Logical Minimum (0)                 */
    0x25, 0x01,                 /*      Logical Maximum (1)                 */
    0x95, 0x03,                 /*      Report Count (3)                    */
    0x75, 0x01,                 /*      Report Size (1)                     */
    0x81, 0x02,                 /*      Input (Data, Variable, Absolute)    */
    0x95, 0x01,                 /*      Report Count (1)                    */
    0x75, 0x05,                 /*      Report Size (5)                     */
    0x81, 0x01,                 /*      Input (Constant)    ;5 bit padding  */
    0x05, 0x01,                 /*      Usage Page (Generic Desktop)        */
    0x09, 0x30,                 /*      Usage (X)                           */
    0x09, 0x31,                 /*      Usage (Y)                           */
    0x16, 0x01, 0xF8,           /*      Logical Minimum (-2047)              */
    0x26, 0xFF, 0x07,           /*      Logical Maximum (2047)               */
    0x75, 0x0C,                 /*      Report Size (12)                     */
    0x95, 0x02,                 /*      Report Count (2)                    */
    0x81, 0x06,                 /*      Input (Data, Variable, Relative)    */
    0x05, 0x01,                 /*      Usage Page (Generic Desktop)        */
    0x09, 0x38,                 /*      Usage (Wheel)                       */
    0x15, 0x81,                 /*      Logical Minimum (-127)              */
    0x25, 0x7F,                 /*      Logical Maximum (127)               */
    0x75, 0x08,                 /*      Report Size (8)                     */
    0x95, 0x01,                 /*      Report Count (1)                    */
    0x81, 0x06,                 /*      Input (Data, Variable, Relative)    */
    0xC0,                       /* End Collection,End Collection            */
    0xC0,                       /* End Collection,End Collection            */
};

static uint8_t g_sle_dis_uuid[DIS_ELEMENT_NUM][SLE_UUID_LEN] = {
    /* DIS service UUID. 设备信息管理 */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x06, 0x09 },
    /* Device name characteristic UUID 设备名称 */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3F},
    /* Device appearance characteristic 设备外观 */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40},
      /* Pnp Id characteristic UUID（设备序列号） */
    { 0x37, 0xBE, 0xA8, 0x80, 0xFC, 0x70, 0x11, 0xEA,
      0xB7, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x2E },
};

void sle_save_remote_addr(const sle_addr_t *addr)
{
    memcpy_s(&g_remote_addr, sizeof(sle_addr_t), addr, sizeof(sle_addr_t));
}

errcode_t sle_register_server(void)
{
    // register server
    errcode_t ret;
    sle_uuid_t app_uuid = {0};
    app_uuid.len = sizeof(g_app_uuid);
    if (memcpy_s(app_uuid.uuid, app_uuid.len, g_app_uuid, sizeof(g_app_uuid)) != EOK) {
        return ERRCODE_SLE_MEMCPY_FAIL;
    }
    ret = ssaps_register_server(&app_uuid, &g_app_server_id);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle reg server fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }
    return ERRCODE_SLE_SUCCESS;
}

static uint8_t sle_get_server_id(void)
{
    return g_app_server_id;
}

static errcode_t sle_sample_set_uuid(uint8_t *uuid, sle_uuid_t *service_uuid)
{
    if (memcpy_s(service_uuid->uuid, SLE_UUID_LEN, uuid, SLE_UUID_LEN) != EOK) {
        sample_debug_print("sle mouse hid set uuid fail\r\n");
        return ERRCODE_SLE_MEMCPY_FAIL;
    }
    service_uuid->len = SLE_UUID_LEN;
    return ERRCODE_SLE_SUCCESS;
}

static errcode_t sle_sample_add_property_interface(uint32_t properties, uint8_t *uuid, uint16_t len, uint8_t *data,
    sle_item_handle_t* service_hdl)
{
    if ((data == NULL) || (service_hdl == NULL)) {
        sample_debug_print("sle sample add property interface param is NULL\r\n");
        return ERRCODE_SLE_FAIL;
    }
    ssaps_property_info_t property = {0};
    errcode_t ret = sle_sample_set_uuid(uuid, &property.uuid);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse uuid set fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }
    property.permissions = SSAP_PERMISSION_READ | SSAP_PERMISSION_WRITE;
    property.operate_indication = properties;
    property.value_len = len;
    property.value = data;
    return ssaps_add_property_sync(sle_get_server_id(), service_hdl->handle_in, &property, &service_hdl->handle_out);
}

static errcode_t sle_sample_add_descriptor_interface(uint32_t properties, uint16_t service_handle,
    uint16_t property_handle, uint16_t len, uint8_t *data)
{
    if (data == NULL) {
        sample_debug_print("sle sample add descriptor interface param is NULL\r\n");
        return ERRCODE_SLE_FAIL;
    }
    ssaps_desc_info_t descriptor = {0};
    descriptor.permissions = SSAP_PERMISSION_READ | SSAP_PERMISSION_WRITE;
    descriptor.operate_indication = properties;
    descriptor.type = SSAP_DESCRIPTOR_CLIENT_CONFIGURATION;
    descriptor.value_len = len;
    descriptor.value = data;
    return ssaps_add_descriptor_sync(sle_get_server_id(), service_handle, property_handle, &descriptor);
}

static errcode_t sle_mouse_service_add(void)
{
    errcode_t ret;
    sle_uuid_t service_uuid = {0};
    ret = sle_sample_set_uuid(g_sle_hid_group_uuid[SLE_UUID_INDEX0], &service_uuid);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse uuid set fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }

    ret = ssaps_add_service_sync(sle_get_server_id(), &service_uuid, true, &g_service_hdl[SLE_UUID_INDEX0].handle_out);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse add service fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }
    g_service_hdl[SLE_UUID_INDEX1].handle_in = g_service_hdl[SLE_UUID_INDEX0].handle_out;
    g_service_hdl[SLE_UUID_INDEX3].handle_in = g_service_hdl[SLE_UUID_INDEX0].handle_out;
    g_service_hdl[SLE_UUID_INDEX4].handle_in = g_service_hdl[SLE_UUID_INDEX0].handle_out;
    g_service_hdl[SLE_UUID_INDEX5].handle_in = g_service_hdl[SLE_UUID_INDEX0].handle_out;
    return ERRCODE_SLE_SUCCESS;
}

static errcode_t sle_mouse_property_and_descriptor_add(void)
{
    errcode_t ret = ERRCODE_SLE_SUCCESS;
    ret = sle_sample_add_property_interface(g_hid_service_property[SLE_UUID_INDEX1],
        g_sle_hid_group_uuid[SLE_UUID_INDEX1], SLE_MOUSE_REPORT_LENGTH, g_sle_input_report,
        &g_service_hdl[SLE_UUID_INDEX1]);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse add report fail, ret:%x, indet:%x\r\n", ret, SLE_UUID_INDEX1);
        return ERRCODE_SLE_FAIL;
    }
    sample_debug_print("[uuid server] sle mouse add report, proterty hdl:%x\r\n",
        g_service_hdl[SLE_UUID_INDEX1].handle_out);

    ret = sle_sample_add_descriptor_interface(g_hid_service_property[SLE_UUID_INDEX2],
        g_service_hdl[SLE_UUID_INDEX0].handle_out, g_service_hdl[SLE_UUID_INDEX1].handle_out, sizeof(g_cccd), g_cccd);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse add cccd fail, ret:%x, indet:%x\r\n", ret, SLE_UUID_INDEX2);
        return ERRCODE_SLE_FAIL;
    }

    g_input_report_descriptor[0] = 0x1;   // [1] : report id
    g_input_report_descriptor[1] = 0x1;   // [1] : input
    g_input_report_descriptor[2] = g_service_hdl[SLE_UUID_INDEX1].handle_out; // [2] rpt handle low
    g_input_report_descriptor[3] = 0;     // [3] rpt handle high
    ret = sle_sample_add_property_interface(g_hid_service_property[SLE_UUID_INDEX3],
        g_sle_hid_group_uuid[SLE_UUID_INDEX3], SLE_SRV_ENCODED_REPORT_LEN, g_input_report_descriptor,
        &g_service_hdl[SLE_UUID_INDEX3]);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse add report ref fail, ret:%x, indet:%x\r\n", ret, SLE_UUID_INDEX3);
        return ERRCODE_SLE_FAIL;
    }

    ret = sle_sample_add_property_interface(g_hid_service_property[SLE_UUID_INDEX4],
        g_sle_hid_group_uuid[SLE_UUID_INDEX4], sizeof(g_sle_report_map_datas), g_sle_report_map_datas,
        &g_service_hdl[SLE_UUID_INDEX4]);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse add report map ref fail, ret:%x, indet:%x\r\n", ret,
            SLE_UUID_INDEX4);
        return ERRCODE_SLE_FAIL;
    }

    ret = sle_sample_add_property_interface(g_hid_service_property[SLE_UUID_INDEX5],
        g_sle_hid_group_uuid[SLE_UUID_INDEX5], sizeof(uint8_t), &g_sle_hid_control_point,
        &g_service_hdl[SLE_UUID_INDEX5]);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse add hid ctrl point fail, ret:%x, indet:%x\r\n", ret,
            SLE_UUID_INDEX5);
        return ERRCODE_SLE_FAIL;
    }
    return ERRCODE_SLE_SUCCESS;
}

errcode_t sle_server_mouse_service_add(void)
{
    if (sle_mouse_service_add() != ERRCODE_SLE_SUCCESS) {
        ssaps_unregister_server(sle_get_server_id());
        return ERRCODE_SLE_FAIL;
    }

    if (sle_mouse_property_and_descriptor_add() != ERRCODE_SLE_SUCCESS) {
        ssaps_unregister_server(sle_get_server_id());
        return ERRCODE_SLE_FAIL;
    }
    sample_debug_print("[uuid server] sle pen add service, server_id:%x, service_handle:%x\r\n",
        g_app_server_id, g_service_hdl[SLE_UUID_INDEX0].handle_out);
    ssaps_start_service(g_app_server_id, g_service_hdl[SLE_UUID_INDEX0].handle_out);
    sample_debug_print("[uuid server] sle uuid add service out\r\n");
    return ERRCODE_SLE_SUCCESS;
}

static errcode_t sle_dis_service_add(void)
{
    errcode_t ret;
    sle_uuid_t service_uuid = {0};
    ret = sle_sample_set_uuid(g_sle_dis_uuid[SLE_DIS_INDEX0], &service_uuid);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse uuid set fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }

    ret = ssaps_add_service_sync(sle_get_server_id(), &service_uuid, 1, &g_dis_service_hdl[SLE_DIS_INDEX0].handle_out);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle mouse add service fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }
    g_dis_service_hdl[SLE_DIS_INDEX1].handle_in = g_dis_service_hdl[SLE_DIS_INDEX0].handle_out;
    g_dis_service_hdl[SLE_DIS_INDEX2].handle_in = g_dis_service_hdl[SLE_DIS_INDEX0].handle_out;
    g_dis_service_hdl[SLE_DIS_INDEX3].handle_in = g_dis_service_hdl[SLE_DIS_INDEX0].handle_out;
    return ERRCODE_SLE_SUCCESS;
}

static errcode_t sle_dis_property_and_descriptor_add(void)
{
    errcode_t ret = ERRCODE_SLE_SUCCESS;
    uint32_t properties = SSAP_OPERATE_INDICATION_BIT_READ;
    ret = sle_sample_add_property_interface(properties, g_sle_dis_uuid[SLE_DIS_INDEX1], sizeof(g_local_device_name),
        g_local_device_name, &g_dis_service_hdl[SLE_DIS_INDEX1]);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[dis server] sle dis add name fail, ret:%x, indet:%x\r\n", ret, SLE_DIS_INDEX1);
        return ERRCODE_SLE_FAIL;
    }

    uint8_t appearance_value[MOUSE_APPEARANCE_LENGTH] = {0x00, 0x05, 0x02}; // mouse appearance 0x00, 0x05, 0x02

    ret = sle_sample_add_property_interface(properties, g_sle_dis_uuid[SLE_DIS_INDEX2], sizeof(appearance_value),
        appearance_value, &g_dis_service_hdl[SLE_DIS_INDEX2]);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle dis add appearance fail, ret:%x, indet:%x\r\n", ret, SLE_DIS_INDEX2);
        return ERRCODE_SLE_FAIL;
    }

    uint8_t *facturer_pnp_id = (uint8_t*)MANUFACTURE_PNP_ID;

    ret = sle_sample_add_property_interface(properties, g_sle_dis_uuid[SLE_DIS_INDEX3], MANUFACTURE_PNP_ID_LENGTH,
        facturer_pnp_id, &g_dis_service_hdl[SLE_DIS_INDEX3]);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle dis add appearance fail, ret:%x, indet:%x\r\n", ret, SLE_DIS_INDEX2);
        return ERRCODE_SLE_FAIL;
    }
    return ERRCODE_SLE_SUCCESS;
}

errcode_t sle_server_dis_service_add(void)
{
    errcode_t ret = ERRCODE_SLE_SUCCESS;
    ret = sle_set_local_name(g_local_device_name, sizeof(g_local_device_name));
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[dis server] set local name fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }
    if (sle_dis_service_add() != ERRCODE_SLE_SUCCESS) {
        ssaps_unregister_server(sle_get_server_id());
        return ERRCODE_SLE_FAIL;
    }

    if (sle_dis_property_and_descriptor_add() != ERRCODE_SLE_SUCCESS) {
        ssaps_unregister_server(sle_get_server_id());
        return ERRCODE_SLE_FAIL;
    }
    sample_debug_print("[dis server] sle dis add service, server_id:%x, service_handle:%x\r\n",
        sle_get_server_id(), g_dis_service_hdl[SLE_DIS_INDEX0].handle_out);
    ret = ssaps_start_service(sle_get_server_id(), g_dis_service_hdl[SLE_DIS_INDEX0].handle_out);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[dis server] sle dis start service fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }
    sample_debug_print("[dis server] sle add dis service out\r\n");
    return ERRCODE_SLE_SUCCESS;
}

static void sle_set_addr(void)
{
    uint8_t *addr = g_sle_mac_addr;

    sle_addr_t sle_addr = {0};
    sle_addr.type = 0;
    if (memcpy_s(sle_addr.addr, BD_ADDR_LEN6, addr, BD_ADDR_LEN6) != EOK) {
        sample_debug_print("addr memcpy fail \r\n");
    } else {
        sample_debug_print("set sle mac in sle adv SUCC \r\n");
    }
    sle_set_local_addr(&sle_addr);
}

static int sle_set_default_announce_param(void)
{
    sle_set_addr();
    sle_announce_param_t param = {0};
    param.announce_mode = SLE_ANNOUNCE_MODE_CONNECTABLE_SCANABLE;
    param.announce_handle = SLE_ADV_HANDLE_DEFAULT;
    param.announce_gt_role = SLE_ANNOUNCE_ROLE_T_CAN_NEGO;
    param.announce_level = SLE_ANNOUNCE_LEVEL_NORMAL;
    param.announce_channel_map = SLE_ADV_CHANNEL_MAP_DEFAULT;
    param.announce_interval_min = SLE_ADV_INTERVAL_MIN_DEFAULT;
    param.announce_interval_max = SLE_ADV_INTERVAL_MAX_DEFAULT;
    param.conn_interval_min = SLE_CONN_INTV_MIN_DEFAULT;
    param.conn_interval_max = SLE_CONN_INTV_MAX_DEFAULT;
    param.conn_max_latency = SLE_CONN_MAX_LATENCY;
    param.conn_supervision_timeout = SLE_CONN_SUPERVISION_TIMEOUT_DEFAULT;
    if (memcpy_s(param.own_addr.addr, BD_ADDR_LEN6, g_sle_mac_addr, BD_ADDR_LEN6) != EOK) {
        sample_debug_print("set sle adv param memcpy addr fail\r\n");
        return ERRCODE_SLE_PARAM_ERR;
    }
    return sle_set_announce_param(param.announce_handle, &param);
}

static int sle_set_direct_announce_param(void)
{
    sle_set_addr();
    sle_announce_param_t param = {0};
    param.announce_mode = SLE_ANNOUNCE_MODE_CONNECTABLE_DIRECTED;
    param.announce_handle = SLE_ADV_HANDLE_DEFAULT;
    param.announce_gt_role = SLE_ANNOUNCE_ROLE_T_CAN_NEGO;
    param.announce_level = SLE_ANNOUNCE_LEVEL_NORMAL;
    param.announce_channel_map = SLE_ADV_CHANNEL_MAP_DEFAULT;
    param.announce_interval_min = SLE_ADV_INTERVAL_MIN_DEFAULT;
    param.announce_interval_max = SLE_ADV_INTERVAL_MAX_DEFAULT;
    param.conn_interval_min = SLE_CONN_INTV_MIN_DEFAULT;
    param.conn_interval_max = SLE_CONN_INTV_MAX_DEFAULT;
    param.conn_max_latency = SLE_CONN_MAX_LATENCY;
    param.conn_supervision_timeout = SLE_CONN_SUPERVISION_TIMEOUT_DEFAULT;
    if (memcpy_s(param.own_addr.addr, BD_ADDR_LEN6, g_sle_mac_addr, BD_ADDR_LEN6) != EOK ||
        memcpy_s(&param.peer_addr, sizeof(sle_addr_t), &g_remote_addr, sizeof(sle_addr_t)) != EOK) {
        sample_debug_print("set sle adv param memcpy addr fail\r\n");
        return ERRCODE_SLE_PARAM_ERR;
    }

    return sle_set_announce_param(param.announce_handle, &param);
}

static int sle_set_default_announce_data(void)
{
    errcode_t ret;
    sle_announce_data_t data = {0};
    uint8_t adv_handle = SLE_ADV_HANDLE_DEFAULT;

    sample_debug_print("set adv data default\r\n");
    data.announce_data = g_sle_adv_data;
    data.announce_data_len = SLE_ADV_DATA_LEN;
    data.seek_rsp_data = g_sle_adv_rsp_data;
    data.seek_rsp_data_len = SLE_ADV_RSP_DATA_LEN;

    ret = sle_set_announce_data(adv_handle, &data);
    if (ret == ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[SLE DD SDK] set announce data success.");
    } else {
        sample_debug_print("[SLE DD SDK] set adv param fail.");
    }
    return ERRCODE_SLE_SUCCESS;
}

void sle_server_adv_init(void)
{
    sample_debug_print("[sle server adv]\r\n");
    sle_set_default_announce_param();
    sle_set_default_announce_data();
    sle_start_announce(SLE_ADV_HANDLE_DEFAULT);
}

void sle_server_direct_adv_init(void)
{
    sample_debug_print("[sle server direct adv]\r\n");
    sle_set_direct_announce_param();
    sle_set_default_announce_data();
    sle_start_announce(SLE_ADV_HANDLE_DEFAULT);
}
