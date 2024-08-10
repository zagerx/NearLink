/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: SLE Client Source.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include "string.h"
#include "common_def.h"
#include "osal_task.h"
#include "cmsis_os2.h"
#include "securec.h"
#include "bts_le_gap.h"
#include "sle_errcode.h"
#include "sle_device_discovery.h"
#include "sle_connection_manager.h"
#include "sle_ssap_client.h"
#include "sle_low_latency.h"
#include "sle_client.h"

#define SLE_SEEK_INTERVAL_DEFAULT 100
#define SLE_SEEK_WINDOW_DEFAULT 100
#define SLE_UART_TASK_DELAY_MS 1000

#define APP_UUID_LEN                    2
#define SLE_SSAP_MIN_HANDLE     1
#define SLE_SSAP_MAX_HANDLE     0xFFFF

#define DEFAULT_INITIATOR_FILTER_POLICY 0
#define DEFAULT_INITIATE_PHYS           1
#define DEFAULT_NEGOTIATE               1
#define DEFAULT_SCAN_INTERVAL           0x20
#define DEFAULT_SCAN_WINDOW             0x20
#define DEFAULT_CONN_INTERVAL_MIN       0x20
#define DEFAULT_CONN_INTERVAL_MAX       0x20
#define DEFAULT_SUPERVISION_TIMEOUT     50

static uint16_t g_sle_mouse_client_conn_id = 0;
static uint8_t g_sle_mouse_client_conn_state = SLE_ACB_STATE_NONE;
uint16_t g_app_conn_id = 0;
uint8_t g_app_client_id = 0;
static uint8_t g_client_uuid[APP_UUID_LEN] = {0x0, 0x0};
static sle_addr_t g_remote_addr = {0};

uint8_t get_g_sle_mouse_client_conn_state(void)
{
    return g_sle_mouse_client_conn_state;
}

uint16_t get_g_sle_mouse_client_conn_id(void)
{
    return g_sle_mouse_client_conn_id;
}

void sle_save_remote_addr(const sle_addr_t *addr)
{
    memcpy_s(&g_remote_addr, sizeof(sle_addr_t), addr, sizeof(sle_addr_t));
}

void sle_client_start_scan(void)
{
    sample_debug_print("=====================================.\r\n");
    sample_debug_print("sle client start scan.\r\n");
    sle_seek_param_t param = { 0 };
    param.own_addr_type = 0;
    param.filter_duplicates = 0;
    param.seek_filter_policy = 0;
    param.seek_phys = 1;
    param.seek_type[0] = 1;
    param.seek_interval[0] = SLE_SEEK_INTERVAL_DEFAULT;
    param.seek_window[0] = SLE_SEEK_WINDOW_DEFAULT;
    sle_set_seek_param(&param);
    sle_start_seek();
}

void sle_set_default_conn_param(void)
{
    sle_default_connect_param_t param = {
        .enable_filter_policy = DEFAULT_INITIATOR_FILTER_POLICY,
        .initiate_phys        = DEFAULT_INITIATE_PHYS,
        .gt_negotiate         = DEFAULT_NEGOTIATE,
        .scan_interval        = DEFAULT_SCAN_INTERVAL,
        .scan_window          = DEFAULT_SCAN_WINDOW,
        .min_interval         = DEFAULT_CONN_INTERVAL_MIN,
        .max_interval         = DEFAULT_CONN_INTERVAL_MAX,
        .timeout              = DEFAULT_SUPERVISION_TIMEOUT,
    };
    sle_default_connection_param_set(&param);
}

void sle_client_find_service(void)
{
    sample_debug_print("=====================================.\r\n");
    sample_debug_print("sle client start find service.\r\n");
    ssapc_find_structure_param_t service = { 0 };
    service.type = SSAP_FIND_TYPE_PRIMARY_SERVICE;
    service.start_hdl = SLE_SSAP_MIN_HANDLE;
    service.end_hdl = SLE_SSAP_MAX_HANDLE;
    ssapc_find_structure(g_app_client_id, g_app_conn_id, &service);
}

void sle_client_find_property(app_ssapc_find_service_t *result)
{
    ssapc_find_structure_param_t property = { 0 };
    property.type = SSAP_FIND_TYPE_PROPERTY;
    property.start_hdl = result->service.start_hdl;
    property.end_hdl = result->service.end_hdl;
    // find by handle时不需要uuid
    ssapc_find_structure(result->client_id, result->conn_id, &property);
}

errcode_t sle_register_client(void)
{
    // register client
    errcode_t ret;
    sle_uuid_t app_uuid = {0};
    app_uuid.len = sizeof(g_client_uuid);
    if (memcpy_s(app_uuid.uuid, app_uuid.len, g_client_uuid, sizeof(g_client_uuid)) != EOK) {
        return ERRCODE_SLE_MEMCPY_FAIL;
    }
    ret = ssapc_register_client(&app_uuid, &g_app_client_id);
    if (ret != ERRCODE_SLE_SUCCESS) {
        sample_debug_print("[uuid server] sle reg server fail, ret:%x\r\n", ret);
        return ERRCODE_SLE_FAIL;
    }
    return ERRCODE_SLE_SUCCESS;
}
