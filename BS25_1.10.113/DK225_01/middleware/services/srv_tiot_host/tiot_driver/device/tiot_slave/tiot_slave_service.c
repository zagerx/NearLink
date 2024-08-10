/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved. \n
 *
 * Description: tiot slave service interface. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2024-01-10, Create file. \n
 */
#include "tiot_slave_service.h"
#include "tiot_slave_device_info.h"
#include "tiot_slave_board_port.h"
#include "tiot_board_log.h"
#include "tiot_controller.h"
#include "tiot_packet.h"
#include "tiot_sys_msg_types.h"
#include "tiot_board_pin_port.h"
#include "tiot_board_uart_port.h"
#include "tiot_board_misc_port.h"
#include "tiot_board.h"
#include "tiot_service_interface.h"
#include "tiot_controller_helper.h"

#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
/* 加载 & 数据收发复用，不同时使用 */
/* 数据收发暂定5K用于缓存数据. */
/* 固件加载时 ~3K用于保存解析后命令, ~128字节用于加载回复接收. */
#define TIOT_SLAVE_FW_LOAD_REUSE_BUFF_END   128U

#define TIOT_SLAVE_COMMON_BUFF_MAX    ((CONFIG_TIOT_SLAVE_PACKET_BUFF_MAX > TIOT_SLAVE_FW_LOAD_REUSE_BUFF_END) ? \
                                 CONFIG_TIOT_SLAVE_PACKET_BUFF_MAX : TIOT_SLAVE_FW_LOAD_REUSE_BUFF_END)
#endif

#define TIOT_SLAVE_PACKET_QUEUE_NODES    8

typedef enum {
    TIOT_SLAVE_TX_SUBSYS_0 = 0,
    TIOT_SLAVE_TX_SUBSYS_1 = 1,
    TIOT_SLAVE_TX_SUBSYS_2 = 2,
    TIOT_SLAVE_TX_SUBSYS_MAX
} tiot_slave_tx_subsys_id;

typedef enum {
    TIOT_SLAVE_RX_SUBSYS_0 = 0,
    TIOT_SLAVE_RX_SUBSYS_MAX
} tiot_slave_rx_subsys_id;

static tiot_controller g_tiot_slave_controllers[CONFIG_TIOT_SLAVE_DEV_NUM] = { 0 };
/* 仅UART需要，使用SPI可优化. */
static tiot_packet_context g_tiot_slave_pkt_ctx[CONFIG_TIOT_SLAVE_DEV_NUM];
static tiot_packet g_tiot_slave_pkt_queue_nodes[CONFIG_TIOT_SLAVE_DEV_NUM][TIOT_SLAVE_PACKET_QUEUE_NODES];
static tiot_packet_queue g_tiot_slave_pkt_queues[CONFIG_TIOT_SLAVE_DEV_NUM][TIOT_SLAVE_RX_SUBSYS_MAX];
#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
static uint8_t g_tiot_slave_common_buff_mem[CONFIG_TIOT_SLAVE_DEV_NUM][TIOT_SLAVE_COMMON_BUFF_MAX];
#endif

void tiot_slave_service_close_ext(uint8_t dev_id);

static uint16_t tiot_slave_packet_manager_match_func(uint32_t subsys_code)
{
    if ((subsys_code == GNSS_LAST_MSG)) {
        return TIOT_SLAVE_RX_SUBSYS_0;
    }
    return TIOT_SLAVE_RX_SUBSYS_MAX;
}

static void slave_host_wakeup_device_cb(uint32_t pin)
{
    tiot_controller *ctrl;
    /* 需要在板级锁中断源. */
    ctrl = tiot_find_controller_by_pin(g_tiot_slave_controllers, CONFIG_TIOT_SLAVE_DEV_NUM,
                                       pin, TIOT_SLAVE_PIN_HOST_WAKEUP_DEVICE);
    if (ctrl != NULL) { /* wakeup host */
        (void)tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_WAKEUP);
    }
}

#ifdef CONFIG_TIOT_SLAVE_UART
static int32_t tiot_slave_service_packet_init(tiot_controller *ctrl, uint8_t dev_id)
{
    int32_t ret;
#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
    tiot_packet_buffer pkt_buff = { g_tiot_slave_common_buff_mem[dev_id], CONFIG_TIOT_SLAVE_PACKET_BUFF_MAX, 0 };

#endif
    tiot_packet_context_param param = {
        .flags = 0,
        .rx_manager_info = { g_tiot_slave_pkt_queues[dev_id], TIOT_SLAVE_RX_SUBSYS_MAX,
                             tiot_slave_packet_manager_match_func },
        .buff = NULL
    };
#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
    param.buff = &pkt_buff;
#endif
    g_tiot_slave_pkt_queues[dev_id][TIOT_SLAVE_RX_SUBSYS_0].queue_nodes = g_tiot_slave_pkt_queue_nodes[dev_id];
    g_tiot_slave_pkt_queues[dev_id][TIOT_SLAVE_RX_SUBSYS_0].queue_size = TIOT_SLAVE_PACKET_QUEUE_NODES;
    tiot_packet_context *pkt_ctx = &g_tiot_slave_pkt_ctx[dev_id];
    ret = tiot_packet_init(pkt_ctx, &ctrl->transfer, &param);
    if (ret != 0) {
        tiot_print_err("[TIOT]packet init fail.\r\n");
    }
    return ret;
}

static int32_t tiot_slave_service_install_packet(tiot_controller *ctrl, uint8_t dev_id)
{
    tiot_xfer_packet_ops tiot_slave_packet_ops = {
        .read_out = tiot_packet_read_out,
        .rx_data_store = tiot_packet_rx_data_store,
        .tx_push = tiot_packet_tx_push,
    };
    const tiot_device_timings *timings = &ctrl->dev_info->timings;
    /* 收发模块注册包格式 */
    if (tiot_xfer_install_packet(&ctrl->transfer, (tiot_xfer_packet_ops *)&tiot_slave_packet_ops,
                                 (uintptr_t)&g_tiot_slave_pkt_ctx[dev_id]) != 0) {
        tiot_print_err("[TIOT]packet install fail.\r\n");
        return -1;
    }
    /* 必须等待device UART初始化后才能接收消息, 待确认时间. */
    osal_msleep(timings->init_time_ms);
    return 0;
}
#endif

static int32_t slave_is_request_sleep_cmd(uint16_t subsys, const uint8_t *data, uint32_t len)
{
    return (subsys == SUBSYS_SYS && len == 1 && data[0] == SYS_INF_REQUEST_SLEEP);
}

static inline int32_t tiot_slave_get_dev_id(tiot_controller *ctrl, uint8_t *dev_id)
{
    uint8_t i;
    for (i = 0; i < CONFIG_TIOT_SLAVE_DEV_NUM; i++) {
        if (ctrl == &g_tiot_slave_controllers[i]) {
            *dev_id = i;
            return 0;
        }
    }
    tiot_print_err("[TIoT]invalid device\n");
    return -1;
}

static inline void tiot_slave_set_open_param(tiot_xfer_open_param *xfer_open_param, tiot_service_open_param *param)
{
    if (param == NULL) {
        return;
    }
    xfer_open_param->rx_cbk_param.buff = param->buff;
    xfer_open_param->rx_cbk_param.buff_len = param->buff_len;
    xfer_open_param->rx_cbk_param.rx_callback = param->rx_callback;
}

static int32_t tiot_slave_service_open(tiot_controller *ctrl, void *param)
{
    int ret = -1;
    uint8_t dev_id;
    uint32_t host_wakeup_dev_pin;
    uint32_t power_ctrl_pin;
    tiot_xfer_open_param xfer_open_param = { 0 };
     tiot_uart_config uart_config = { CONFIG_TIOT_SLAVE_UART_BAUDRATE,
         { .data_bits = TIOT_UART_ATTR_DATABIT_8, .parity = TIOT_UART_ATTR_PARITY_NONE,
           .stop_bits = TIOT_UART_ATTR_STOPBIT_1, .flow_ctrl = TIOT_UART_ATTR_FLOW_CTRL_ENABLE } };
    if (tiot_slave_get_dev_id(ctrl, &dev_id) != 0) {
        return -1;
    }
    tiot_slave_set_open_param(&xfer_open_param, param);
    ret = tiot_slave_service_packet_init(ctrl, dev_id);
    if (ret != 0) {
        goto fw_load_fail;
    }
    /* 传输启动 */
    if (tiot_xfer_open(&ctrl->transfer, &xfer_open_param) != 0) {
        /* 通信接口打开失败 */
        goto xfer_open_fail;
    }
    /* 使能中断 */
    host_wakeup_dev_pin = ctrl->pm.pm_info[TIOT_SLAVE_PIN_HOST_WAKEUP_DEVICE];
    power_ctrl_pin = ctrl->pm.pm_info[TIOT_SLAVE_PIN_POWER_CTRL];
    /* 默认先不开启唤醒slave中断，主机允许slave睡眠后再开启 */
    if (host_wakeup_dev_pin != TIOT_PIN_NONE) {
        (void)tiot_board_pin_register_int_callback(host_wakeup_dev_pin, slave_host_wakeup_device_cb);
        (void)tiot_board_pin_enable_int(host_wakeup_dev_pin, TIOT_PIN_INT_DISABLE);
    }
    /* 上电 */
    if (tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_POWERON) != 0) {
        goto power_on_fail;
    }
    (void)tiot_xfer_set_config(&ctrl->transfer, &uart_config);
    ret = tiot_slave_service_install_packet(ctrl, dev_id);
    if (ret != 0) {
        goto pkt_install_fail;
    }
    /* 通过配置poweron管脚有无，判断主机侧是否控制上电，需要与主机侧保持一致，slave侧不能控制这根管脚，仅用于判断. */
    /* 主机侧如果控制上电，切换为WORK状态，不释放唤醒锁. */
    /* 主机侧如果不控制上电，仅进行通信和低功耗，切换为SLEEP状态，释放唤醒锁. */
    (void)tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_REPORT_WORK);
    if (power_ctrl_pin == TIOT_PIN_NONE) {
        if (tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_REQUEST_SLEEP) != 0) {
            goto pkt_install_fail;
        }
    }
    /* 打开OK */
    tiot_print_suc("[TIoT]Controller open succ.\n");
    return 0;

pkt_install_fail:
    tiot_packet_deinit(&g_tiot_slave_pkt_ctx[dev_id]);
power_on_fail:
    tiot_pin_int_release(host_wakeup_dev_pin);
fw_load_fail:
xfer_open_fail:
    return ret;
}

static void tiot_slave_service_close(tiot_controller *ctrl)
{
    uint8_t dev_id;
    uint32_t host_wakeup_dev_pin;

    if (tiot_slave_get_dev_id(ctrl, &dev_id) != 0) {
        return;
    }
    (void)tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_POWEROFF);

    tiot_xfer_close(&ctrl->transfer);
    tiot_xfer_uninstall_packet(&ctrl->transfer);
    tiot_packet_deinit(&g_tiot_slave_pkt_ctx[dev_id]);

    host_wakeup_dev_pin = ctrl->pm.pm_info[TIOT_SLAVE_PIN_HOST_WAKEUP_DEVICE];
    tiot_pin_int_release(host_wakeup_dev_pin);
}

static int32_t tiot_slave_service_write(tiot_controller *ctrl, const uint8_t *data,
                                        uint32_t len, const tiot_xfer_tx_param *param)
{
    int32_t ret;
    uint8_t dev_id;
    const uint32_t tx_subsys_convert[TIOT_SLAVE_TX_SUBSYS_MAX] = { SUBSYS_GNSS, SUBSYS_OM, SUBSYS_SYS };
    const tiot_device_timings *timings = &ctrl->dev_info->timings;
    tiot_xfer_tx_param tx_param = { 0 };

    if (param->subsys >= TIOT_SLAVE_TX_SUBSYS_MAX) {
        tiot_print_err("[TIoT]subsys id error %d.\n", param->subsys);
        return -1;
    }
    tx_param.subsys = tx_subsys_convert[param->subsys];
    if (tiot_slave_get_dev_id(ctrl, &dev_id) != 0) {
        return -1;
    }

    if (!slave_is_request_sleep_cmd(tx_param.subsys, data, len)) {
        (void)tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_WORK_VOTE_UP);
    }

    ret = tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_WAKEUP);
    if (ret != 0) {
        tiot_print_err("[TIoT]wakeup host fail.\n");
        goto vote_down;
    }
    /* 等待成功唤醒Host */
    ret = tiot_pm_wait_state(&ctrl->pm, TIOT_PM_STATE_WORK, timings->wakeup_wait_ms);
    if (ret <= 0) {
        tiot_print_err("[TIoT]wakeup host fail(wait WORK).\n");
        goto vote_down;
    }
    ret = tiot_xfer_send(&ctrl->transfer, data, len, &tx_param);
vote_down:
    if (!slave_is_request_sleep_cmd(tx_param.subsys, data, len)) {
        (void)tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_WORK_VOTE_DOWN);
    }
    return ret;
}

static int32_t tiot_slave_service_read(tiot_controller *ctrl, uint8_t *buff, uint32_t buff_len,
                                       const tiot_xfer_rx_param *param)
{
    uint8_t dev_id;
    if (tiot_slave_get_dev_id(ctrl, &dev_id) != 0) {
        return -1;
    }
    return tiot_xfer_recv(&ctrl->transfer, buff, buff_len, param);
}

int32_t tiot_slave_service_init(void)
{
    uint8_t i;
    int32_t ret;
    tiot_board_info per_board_info;
    tiot_controller_info ctrl_info;
    for (i = 0; i < CONFIG_TIOT_SLAVE_DEV_NUM; i++) {
        if (tiot_slave_board_init(&i) != 0) {
            tiot_print_err("[TIoT]board init fail\n");
            return -1;
        }
    }
    const tiot_slave_board_info *board_info = tiot_slave_board_get_info();
    if (board_info == NULL) {
        tiot_print_err("[TIoT]no board info.\n");
        goto deinit;
    }
    for (i = 0; i < CONFIG_TIOT_SLAVE_DEV_NUM; i++) {
        per_board_info.fw_info.cfg_path = board_info->cfg_path;
        per_board_info.hw_info.xmit_id = board_info->hw_infos[i].xmit_id;
        per_board_info.hw_info.pm_info = board_info->hw_infos[i].pm_info;
        ctrl_info.board_info = &per_board_info;
        ctrl_info.dev_info = tiot_slave_device_get_info();
        ret = tiot_controller_init(&g_tiot_slave_controllers[i], &ctrl_info);
        if (ret != 0) {
            tiot_print_err("[TIoT]controller init fail\n");
            goto deinit;
        }
    }
    for (i = 0; i < CONFIG_TIOT_SLAVE_DEV_NUM; i++) {
        g_tiot_slave_controllers[i].ops.open = tiot_slave_service_open;
        g_tiot_slave_controllers[i].ops.close = tiot_slave_service_close;
        g_tiot_slave_controllers[i].ops.write = tiot_slave_service_write;
        g_tiot_slave_controllers[i].ops.read = tiot_slave_service_read;
    }
    return 0;
deinit:
    tiot_slave_service_deinit();
    return -1;
}

void tiot_slave_service_deinit(void)
{
    uint8_t i;
    for (i = 0; i < CONFIG_TIOT_SLAVE_DEV_NUM; i++) {
        tiot_controller_deinit(&g_tiot_slave_controllers[i]);
    }
    for (i = 0; i < CONFIG_TIOT_SLAVE_DEV_NUM; i++) {
        tiot_slave_board_deinit(&i);
    }
}

uintptr_t tiot_slave_service_get_ctrl(uint8_t dev_id)
{
    return (uintptr_t)&g_tiot_slave_controllers[dev_id];
}
