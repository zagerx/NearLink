/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: SLE application callback functions.
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */

#include "securec.h"
#include "soc_osal.h"
#include "osal_inner.h"
#include "osal_msgqueue.h"
#include "../application_task.h"
#include "sle_errcode.h"
#include "sle_device_discovery.h"
#include "sle_connection_manager.h"
#include "sle_ssap_server.h"
#include "sle_ssap_client.h"
#include "sle_client.h"

#define SLE_ADV_REPORT_DEVICE_NAME_SHIFT_BYTES 20
#define CONNECTABLE_SCANABLE_ADV_REPORT         (SLE_SEEK_RESULT_TYPE_CONNECTABLE_ANNOUNCE_PACKET | \
                                                 SLE_SEEK_RESULT_TYPE_SCANABLE_ANNOUNCE_PACKET)
#define CONNECTABLE_SCANABLE_DIRECT_ADV_REPORT  (SLE_SEEK_RESULT_TYPE_CONNECTABLE_ANNOUNCE_PACKET | \
                                                 SLE_SEEK_RESULT_TYPE_SCANABLE_ANNOUNCE_PACKET    | \
                                                 SLE_SEEK_RESULT_TYPE_DIRECT_ANNOUNCE_PACKET)

static void sle_enable_cbk(errcode_t status)
{
    sample_debug_print("sle enable status:%x\r\n", status);
    app_message_t msg_node = { status, APP_MSG_TYPE_SLE, APP_TASK_RECV_SLE_ENABLE, 0, 0 };
    osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
}

static void sle_adv_enable_cbk(uint32_t announce_id, errcode_t status)
{
    sample_debug_print("sle announce enable id:%02x, state:%02x\r\n", announce_id, status);
}

static void sle_adv_disable_cbk(uint32_t announce_id, errcode_t status)
{
    sample_debug_print("sle announce disable id:%02x, state:%02x\r\n", announce_id, status);
}

static void sle_adv_terminal_cbk(uint32_t announce_id)
{
    sample_debug_print("sle announce terminal id:%02x\r\n", announce_id);
}

static uint8_t g_device_name[] = {'d', 'u', 'a', 'l', '_', 'm', 'o', 'u', 's', 'e'};
static void sle_scan_report_cbk(sle_seek_result_info_t *seek_result_data)
{
    // 停止扫描
    if (seek_result_data->event_type == CONNECTABLE_SCANABLE_ADV_REPORT ||
        seek_result_data->event_type == CONNECTABLE_SCANABLE_DIRECT_ADV_REPORT) {
        if (memcmp(seek_result_data->data + SLE_ADV_REPORT_DEVICE_NAME_SHIFT_BYTES,
            g_device_name, sizeof(g_device_name)) == EOK) {
            sle_addr_t *remote = (sle_addr_t *)osal_kmalloc(sizeof(sle_addr_t), 0);
            memcpy_s(remote, sizeof(sle_addr_t), &seek_result_data->addr, sizeof(sle_addr_t));
            app_message_t msg_node = { 0, APP_MSG_TYPE_SLE, APP_TASK_RECV_SLE_SCAN_REPORT,
                sizeof(sle_addr_t), (uint8_t *)remote };
            osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
            sle_stop_seek();
        }
    }
}

void sle_dd_register_cbks(void)
{
    sle_announce_seek_callbacks_t dd_cbks = { 0 };
    dd_cbks.sle_enable_cb = sle_enable_cbk;
    dd_cbks.announce_enable_cb = sle_adv_enable_cbk;
    dd_cbks.announce_disable_cb = sle_adv_disable_cbk;
    dd_cbks.announce_terminal_cb = sle_adv_terminal_cbk;
    dd_cbks.seek_result_cb = sle_scan_report_cbk;
    sle_announce_seek_register_callbacks(&dd_cbks);
}

static void sle_connect_change_cbk(uint16_t conn_id, const sle_addr_t *addr,
    sle_acb_state_t conn_state, sle_pair_state_t pair_state, sle_disc_reason_t disc_reason)
{
    if (conn_state == SLE_ACB_STATE_CONNECTED) {
        sample_debug_print("connect remote %02x:%02x:xx:xx:%02x:%02x, handle: 0x%04x, pair_state: %d, disc_reason: 0x%02x\r\n",
            addr->addr[0], addr->addr[1], addr->addr[4], addr->addr[5], conn_id, pair_state, disc_reason);
        sle_save_remote_addr(addr);
#if defined(CONFIG_SAMPLE_DUAL_MODE_SLE_CLIENT)
        sle_addr_t *remote = (sle_addr_t *)osal_kmalloc(sizeof(sle_addr_t), 0);
        memcpy_s(remote, sizeof(sle_addr_t), addr, sizeof(sle_addr_t));
        app_message_t msg_node = { 0, APP_MSG_TYPE_SLE, APP_TASK_RECV_SLE_CONNECT, sizeof(sle_addr_t), (uint8_t *)remote };
        osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
#endif
    } else if (conn_state == SLE_ACB_STATE_DISCONNECTED) {
        sample_debug_print(
            "disconnect remote %02x:%02x:xx:xx:%02x:%02x, handle: 0x%04x, pair_state: %d, disc_reason: 0x%02x\r\n",
            addr->addr[0], addr->addr[1], addr->addr[4], addr->addr[5], conn_id, pair_state, disc_reason);
        app_message_t msg_node = { 0, APP_MSG_TYPE_SLE, APP_TASK_RECV_SLE_DISCONNECT, 0, NULL };
        osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
    }
}

static void sle_connect_update_cbk(uint16_t conn_id, errcode_t status,
    const sle_connection_param_update_evt_t *param)
{
    sample_debug_print("connect update: %d %d %d\r\n", conn_id, status, param);
}

static void sle_auth_complete_cbk(uint16_t conn_id, const sle_addr_t *addr, errcode_t status,
    const sle_auth_info_evt_t* evt)
{
    osal_unused(addr);
    sample_debug_print("auth complete: 0x%04x, status: %d\r\n", conn_id, status);
    sample_debug_print("linkkey: %08x %08x %08x %08x\r\n",
        *(uint32_t *)(evt->link_key + 12), *(uint32_t *)(evt->link_key + 8),
        *(uint32_t *)(evt->link_key + 4), *(uint32_t *)(evt->link_key + 0));
}

static void sle_pair_complete_cbk(uint16_t conn_id, const sle_addr_t *addr, errcode_t status)
{
    sample_debug_print("[uuid server] pair complete conn_id:%02x, status:%x\r\n",
        conn_id, status);
    sample_debug_print("[uuid server] pair complete addr:%02x:**:**:**:%02x:%02x\r\n",
        addr->addr[0], addr->addr[4], addr->addr[5]);
#if defined(CONFIG_SAMPLE_DUAL_MODE_SLE_CLIENT)
    if (status == ERRCODE_SLE_SUCCESS) {
        app_message_t msg_node = { 0, APP_MSG_TYPE_SLE, APP_TASK_RECV_SLE_PAIRED, 0, NULL };
        osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
    }
#endif
}

static void sle_low_latency_cbk(uint8_t status, sle_addr_t *addr, uint8_t rate)
{
    osal_unused(addr);
    sample_debug_print("low latency cbk status: %d, rate: %d\r\n", status, rate);
}

void sle_cm_register_cbks(void)
{
    sle_connection_callbacks_t cm_cbks = { 0 };
    cm_cbks.connect_state_changed_cb = sle_connect_change_cbk;
    cm_cbks.connect_param_update_cb = sle_connect_update_cbk;
    cm_cbks.auth_complete_cb = sle_auth_complete_cbk;
    cm_cbks.pair_complete_cb = sle_pair_complete_cbk;
    cm_cbks.low_latency_cb = sle_low_latency_cbk;
    sle_connection_register_callbacks(&cm_cbks);
}

void sle_ssapc_exchange_info_cbk(uint8_t client_id, uint16_t conn_id, ssap_exchange_info_t *param,
    errcode_t status)
{
    sample_debug_print("[SSAPC] exchange info cbk, client_id: %d, conn_id: 0x%04x, mtu: %d, status: %x.\r\n",
        client_id, conn_id, param->mtu_size, status);
}

void sle_ssapc_find_service_cbk(uint8_t client_id, uint16_t conn_id, ssapc_find_service_result_t *service,
    errcode_t status)
{
    sample_debug_print("=====================================.\r\n");
    sample_debug_print("[SSAPC] find service cbk, client_id: %d, conn_id: 0x%04x, status: %x.\r\n",
        client_id, conn_id, status);
    sample_debug_print("[SSAPC] find service cbk, start handle: 0x%04x, end handle: 0x%04x.\r\n",
        service->start_hdl, service->end_hdl);
    sample_debug_print("[SSAPC] find service cbk, uuid len: %d, uuid:", service->uuid.len);
    for (uint8_t idx = 0; idx < service->uuid.len; ++idx) {
        sample_debug_print(" %02x", service->uuid.uuid[idx]);
    }
    sample_debug_print(".\r\n");
    app_ssapc_find_service_t *result =
        (app_ssapc_find_service_t *)osal_kmalloc(sizeof(app_ssapc_find_service_t), 0);
    result->client_id = client_id;
    result->conn_id = conn_id;
    memcpy_s(&result->service, sizeof(ssapc_find_service_result_t), service, sizeof(ssapc_find_service_result_t));
    app_message_t msg_node = { 0, APP_MSG_TYPE_SLE, APP_TASK_RECV_SLE_SSAPC_FIND_SERVICE,
        sizeof(app_ssapc_find_service_t), (uint8_t *)result };
    osal_msg_queue_write_copy(app_msg_queue, &msg_node, sizeof(app_message_t), 0);
}

void sle_ssapc_find_property_cbk(uint8_t client_id, uint16_t conn_id, ssapc_find_property_result_t *property,
    errcode_t status)
{
    sample_debug_print("=====================================.\r\n");
    sample_debug_print("[SSAPC] find property cbk, client_id: %d, conn_id: 0x%04x, ", client_id, conn_id);
    sample_debug_print("status: %x, handle 0x%04x, operate indication: 0x%08x.\r\n",
        status, property->handle, property->operate_indication);
    sample_debug_print("[SSAPC] find property cbk, uuid len: %d, uuid:", property->uuid.len);
    for (uint8_t idx = 0; idx < property->uuid.len; ++idx) {
        sample_debug_print(" %02x", property->uuid.uuid[idx]);
    }
    sample_debug_print(".\r\n");
    sample_debug_print("[SSAPC] find property cbk, descriptors count: %d, descriptors type:", property->descriptors_count);
    for (uint8_t idx = 0; idx < property->descriptors_count; ++idx) {
        sample_debug_print(" %d", property->descriptors_type[idx]);
    }
    sample_debug_print(".\r\n");
}

void sle_ssapc_find_structure_cmp_cbk(uint8_t client_id, uint16_t conn_id,
    ssapc_find_structure_result_t *structure_result, errcode_t status)
{
    sample_debug_print("=====================================.\r\n");
    sample_debug_print("[SSAPC] find structure cmp cbk, client_id: %d, conn_id: 0x%04x, status: 0x%08x.\r\n",
        client_id, conn_id, status);
    sample_debug_print("[SSAPC] find structure cmp cbk, type: %d, uuid len: %d, uuid:",
        structure_result->type, structure_result->uuid.len);
    for (uint8_t idx = 0; idx < structure_result->uuid.len; ++idx) {
        sample_debug_print(" %02x", structure_result->uuid.uuid[idx]);
    }
    sample_debug_print(".\r\n");
}

void sle_ssapc_register_cbks(void)
{
    ssapc_callbacks_t ssapc_cbks = {0};
    ssapc_cbks.exchange_info_cb = sle_ssapc_exchange_info_cbk;
    ssapc_cbks.find_structure_cb = sle_ssapc_find_service_cbk;
    ssapc_cbks.ssapc_find_property_cbk = sle_ssapc_find_property_cbk;
    ssapc_cbks.find_structure_cmp_cb = sle_ssapc_find_structure_cmp_cbk;
    /* SSAP数传业务相关sample暂未开发 */
    /*
     * ssapc_cbks.indication_cb = 
     * ssapc_cbks.notification_cb = 
     * ssapc_cbks.read_cfm_cb = 
     * ssapc_cbks.read_by_uuid_cmp_cb = 
     * ssapc_cbks.write_cfm_cb = 
     */
    ssapc_register_callbacks(&ssapc_cbks);
}

void sle_ssaps_start_service_cbk(uint8_t server_id, uint16_t handle, errcode_t status)
{
    sample_debug_print("[SSAPS] start service cbk, server_id: %d, handle: 0x%04x, status: 0x%08x.\r\n",
        server_id, handle, status);
}

void sle_ssaps_register_cbks(void)
{
    ssaps_callbacks_t ssaps_cbks = { 0 };
    /* SSAP异步注册服务相关sample暂未开发 */
    /*
     * ssaps_cbks.add_service_cb = 
     * ssaps_cbks.add_property_cb = 
     * ssaps_cbks.add_descriptor_cb = 
     */
    ssaps_cbks.start_service_cb = sle_ssaps_start_service_cbk;
    /* SSAP数传业务相关sample暂未开发 */
    /*
     * ssaps_cbks.mtu_changed_cb = 
     * ssaps_cbks.read_request_cb = 
     * ssaps_cbks.write_request_cb = 
     */
    ssaps_register_callbacks(&ssaps_cbks);
}