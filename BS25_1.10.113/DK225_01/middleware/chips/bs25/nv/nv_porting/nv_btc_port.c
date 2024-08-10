/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: btc & bth common
 */
#include "stddef.h"
#include "soc_osal.h"
#include "ipc_actions.h"
#include "ipc.h"
#include "uart.h"
#include "securec.h"
#include "nv.h"
#include "key_id.h"
#include "nv_btc_port.h"

#define BT_IPC_DELAY_TIME 500
#define BT_CUSTOMIZE_DEBUG 0
uint8_t g_nv_flag = 0;
uint8_t *g_write_nv_info = NULL;
uint8_t *g_read_nv_info = NULL;
uint8_t *g_read_nv_customize_info = NULL;
osal_semaphore sem_config_nv;

int app_write_config_nv_act(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    uint8_t nv_info_index;
    uint16_t key = BTC_CONFIG_NV_INFO_ID;
    size_t key_len = (uint16_t)sizeof(config_nv_info_struct); // 写NV ipc payload的长度
    if (g_write_nv_info == NULL) {
        osal_printk("[INFO]wait for flag trigger to 1!");
        return 0;
    }
    if (g_nv_flag == 1) {
        errcode_t nv_ret_value = uapi_nv_write(key, g_write_nv_info, (uint16_t)key_len);
        for (nv_info_index = 0; nv_info_index < key_len; nv_info_index++) {
            osal_printk("[INFO]after writting:%x \n", *(g_write_nv_info + nv_info_index));
        }
        if (nv_ret_value != ERRCODE_SUCC) {
            /* ERROR PROCESS */
            osal_printk("[ERROR]write nv fail! ret:%x \r\n", nv_ret_value);
            osal_vfree(g_write_nv_info);
            g_write_nv_info = NULL;
            return -1;
        }
        osal_printk("[SUCCESS]write nv success!\r\n");
    }
    if (g_write_nv_info != NULL) {
        osal_vfree(g_write_nv_info);
        g_write_nv_info = NULL;
    }
    g_nv_flag = 0;
    return 0;
}

#if CONFIG_NV_AUTO
void app_write_config_nv_sem(void)
{
    uint8_t nv_info_index;
    uint16_t key = BTC_CONFIG_NV_INFO_ID;
    size_t key_len = (uint16_t)sizeof(config_nv_info_struct); // 写NV ipc payload的长度
    if (g_write_nv_info != NULL) {
        if (osal_sem_trydown(&sem_config_nv) == OSAL_SUCCESS) {
            errcode_t nv_ret_value = uapi_nv_write(key, g_write_nv_info, (uint16_t)key_len);
            for (nv_info_index = 0; nv_info_index < key_len; nv_info_index++) {
                osal_printk("[CALI]write_value:%x \n", *(g_write_nv_info + nv_info_index));
            }
            if (nv_ret_value != ERRCODE_SUCC) {
                /* ERROR PROCESS */
                osal_printk("[ERROR]write nv fail! ret:%x \r\n", nv_ret_value);
            }
            osal_printk("[NV]write nv success!\r\n");
        }
        if (g_write_nv_info != NULL) {
            osal_vfree(g_write_nv_info);
            g_write_nv_info = NULL;
        }
    }
}
#endif
/* A核上电后自动读出NV值 */
void app_read_nv_msg(void)
{
    uint16_t key = 0xB;                                       // NV结构的key_id
    size_t key_len = (uint16_t)sizeof(config_nv_info_struct); // 写NV ipc payload的长度
    uint16_t real_len = 0;
    if (g_read_nv_info == NULL) {
        g_read_nv_info = (uint8_t *)osal_vmalloc(key_len);
    }
    if (uapi_nv_read(key, (uint16_t)key_len, &real_len, g_read_nv_info) != ERRCODE_SUCC) {
        /* ERROR PROCESS */
        osal_vfree(g_read_nv_info);
        g_read_nv_info = NULL;
    }
}

void app_read_nv_customize_msg(void)
{
    uint16_t key = 0xC;                                             // NV结构的key_id
    size_t key_len = (uint16_t)sizeof(bt_nv_customize_info_struct); // 写NV ipc payload的长度
    uint16_t real_len = 0;
    if (g_read_nv_customize_info == NULL) {
        g_read_nv_customize_info = (uint8_t *)osal_vmalloc(key_len);
    }
    if (uapi_nv_read(key, (uint16_t)key_len, &real_len, g_read_nv_customize_info) != ERRCODE_SUCC) {
        /* ERROR PROCESS */
        osal_vfree(g_read_nv_customize_info);
        g_read_nv_customize_info = NULL;
    }
    osal_printk("[APPS]Read customized nv success!");
}

/* A核收到BT核IPC消息回调处理函数 */
static bool get_bt_write_nv_msg_handler(ipc_action_t message, const volatile ipc_payload *payload_p, cores_t src,
    uint32_t id)
{
    g_nv_flag = 0;
    size_t key_len = (uint16_t)sizeof(config_nv_info_struct); // 写NV ipc payload的长度
    if (message != IPC_ACTION_BT_WRITE_NV) {
        osal_printk("[ERROR] message:%d\r\n", message);
        return false;
    }
    if (g_write_nv_info == NULL) {
        g_write_nv_info = (uint8_t *)osal_vmalloc(key_len);
    }
    (void)memcpy_s(g_write_nv_info, key_len, (uint8_t *)payload_p, key_len);
    UNUSED(message);
    UNUSED(src);
    UNUSED(id);
#if CONFIG_NV_AUTO
    osal_sem_up(&sem_config_nv); // 释放校准值写NV信号量
#else
    /* APP PROCESS */
    if (g_nv_flag != 1) {
        g_nv_flag = 1;
        osal_printk("[INFO]NV flag writted to 1!\r\n");
        return true;
    }
#endif
    return true;
}

static bool get_bt_read_nv_msg_handler(ipc_action_t message, const volatile ipc_payload *payload_p, cores_t src,
    uint32_t id)
{
    if (message != IPC_ACTION_BT_READ_NV) {
        osal_printk("[ERROR] message:%d\r\n", message);
        return false;
    }
    UNUSED(message);
    UNUSED(payload_p);
    UNUSED(src);
    UNUSED(id);
    /* APP PROCESS */
    size_t key_len = (uint16_t)sizeof(config_nv_info_struct);
    for (uint8_t nv_info_index = 0; nv_info_index < key_len; nv_info_index++) {
        osal_printk("[IPC]:read_value:%x \n", *(g_read_nv_info + nv_info_index));
    }
    uint8_t ret_val =
        ipc_spin_send_message_timeout(CORES_BT_CORE, IPC_ACTION_APP_LOAD_NV, (ipc_payload *)g_read_nv_info,
        sizeof(ipc_payload_config_nv_info_t), IPC_PRIORITY_HIGHEST, false, BT_IPC_DELAY_TIME);
    if (ret_val != 0) {
        osal_printk("[IPC] message send fail! ret:%x", ret_val);
        return false;
    }
    if (g_read_nv_info != NULL) {
        osal_vfree(g_read_nv_info);
        g_read_nv_info = NULL;
    }
    return true;
}

static bool get_bt_read_nv_customize_msg_handler(ipc_action_t message, const volatile ipc_payload *payload_p,
    cores_t src, uint32_t id)
{
    if (message != IPC_ACTION_BT_READ_NV_CUSTOMIZE) {
        osal_printk("[ERROR] message:%d\r\n", message);
        return false;
    }
    UNUSED(message);
    UNUSED(payload_p);
    UNUSED(src);
    UNUSED(id);
    /* APP PROCESS */
#if BT_CUSTOMIZE_DEBUG
    for (uint8_t nv_info_index = BT_NV_START_OFFSET; nv_info_index <= BT_CUSTOMIZE_NV_VALID; nv_info_index++) {
        osal_printk("[CUSTOMIZE]nv_customize_info:%x \n", *(g_read_nv_customize_info + nv_info_index));
    }
#endif
    uint8_t ret_val = ipc_spin_send_message_timeout(CORES_BT_CORE, IPC_ACTION_APP_LOAD_NV_CUSTOMIZE,
        (ipc_payload *)g_read_nv_customize_info, sizeof(ipc_payload_bt_nv_customize_info_t), IPC_PRIORITY_HIGHEST,
        false, BT_IPC_DELAY_TIME);
    if (ret_val != 0) {
        osal_printk("[IPC] message send fail! ret:%x", ret_val);
        return false;
    }
    if (g_read_nv_customize_info != NULL) {
        osal_vfree(g_read_nv_customize_info);
        g_read_nv_customize_info = NULL;
    }
    return true;
}

/* A核上电时注册收到BT核IPC消息的回调 */
void ipc_app_write_nv_msg(void)
{
    (void)ipc_register_handler(IPC_ACTION_BT_WRITE_NV, get_bt_write_nv_msg_handler);
}

void ipc_app_read_nv_msg(void)
{
    (void)ipc_register_handler(IPC_ACTION_BT_READ_NV, get_bt_read_nv_msg_handler);
}

void ipc_app_read_nv_customize_msg(void)
{
    (void)ipc_register_handler(IPC_ACTION_BT_READ_NV_CUSTOMIZE, get_bt_read_nv_customize_msg_handler);
}

void btc_chnl_75_disable_reduction(uint8_t flag)
{
    if (flag == 0x1) {
        uint16_t key = BTC_CUSTOMIZE_ID;
        uint16_t key_len = (uint16_t)sizeof(bt_nv_customize_info_struct); // 写NV ipc payload的长度
        uint16_t real_len = 0;
        uint8_t *read_value = osal_vmalloc(key_len);
        if (uapi_nv_read(key, key_len, &real_len, read_value) != ERRCODE_SUCC) {
            /* ERROR PROCESS */
            osal_vfree(read_value);
            read_value = NULL;
            osal_printk("[ERROR]chnl_75_disable: read nv fail.\r\n");
            return;
        }
        read_value[0x1] = (read_value[0x1] | (0x1 << 0x6));
        errcode_t nv_ret_value = uapi_nv_write(key, read_value, key_len);
        if (nv_ret_value != ERRCODE_SUCC) {
            /* ERROR PROCESS */
            osal_printk("[ERROR]write nv fail. ret:%x \r\n", nv_ret_value);
            osal_vfree(read_value);
            return;
        }
        if (read_value != NULL) {
            osal_vfree(read_value);
            read_value = NULL;
        }
    }
}