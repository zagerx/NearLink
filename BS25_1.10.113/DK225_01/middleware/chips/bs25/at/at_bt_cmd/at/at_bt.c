/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: at plt cmd func \n
 * Author: @CompanyNameTag \n
 */

#include "at_bt.h"
#include "at_bt_cmd_register.h"
#include "at_bt_cmd_table.h"
#include "debug_print.h"
#include "uart.h"

#include "at_config.h"
#include "at_product.h"
#include "test_suite_uart.h"
#include "cmsis_os2.h"

#define AT_BT_STA_FUNC_NUM (sizeof(at_bt_cmd_parse_table) / sizeof(at_bt_cmd_parse_table[0]))
#define CRLF_STR                      "\r\n"
#define CR_ASIC_II                    0xD

void bth_ble_common_at_cmd_register(void);
void bth_sle_common_at_cmd_register(void);

uint32_t *g_at_task_id = NULL;
uint32_t *g_testsuite_task_id = NULL;
static uint8_t g_at_test_uart_rx_buffer_test[1]; /* 1:buff len */

td_void los_at_bt_cmd_register(td_void)
{
    uapi_at_bt_register_cmd(at_bt_cmd_parse_table, AT_BT_STA_FUNC_NUM);
}

static void at_write_func(const char *data)
{
    uapi_uart_write(AT_UART_BUS, (const uint8_t *)data, strlen(data), 0);
}

static void at_uart_rx_callback(const void *buffer, uint16_t length, bool error)
{
    UNUSED(error);
    if (length == 0) {
        return;
    }
    if (((char *)buffer)[0] == CR_ASIC_II) {
        uapi_uart_write(AT_UART_BUS, (uint8_t *)CRLF_STR, (uint16_t)strlen(CRLF_STR), 0);
    } else {
        uapi_uart_write(AT_UART_BUS, (const uint8_t *)buffer, (uint32_t)length, 0);
    }
    uapi_at_channel_data_recv(AT_UART_PORT, (uint8_t *)buffer, (uint32_t)length);
}

static void at_uart_init(void)
{
    uart_buffer_config_t uart_buffer_config;
    uart_pin_config_t uart_pin_config = {
        .tx_pin = AT_UART_TX_PIN,
        .rx_pin = AT_UART_RX_PIN,
        .cts_pin = PIN_NONE,
        .rts_pin = PIN_NONE
    };
    uart_attr_t uart_line_config = {
        .baud_rate = AT_UART_BAUD_RATE,
        .data_bits = UART_DATA_BIT_8,
        .stop_bits = UART_STOP_BIT_1,
        .parity = UART_PARITY_NONE
    };
    uart_buffer_config.rx_buffer_size = sizeof(g_at_test_uart_rx_buffer_test);
    uart_buffer_config.rx_buffer = g_at_test_uart_rx_buffer_test;
    uapi_uart_init(AT_UART_BUS, &uart_pin_config, &uart_line_config, NULL, &uart_buffer_config);
    uapi_uart_register_rx_callback(AT_UART_BUS, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
        1, at_uart_rx_callback);
}

static void at_base_api_queue_create(uint32_t msg_count, uint32_t msg_size, unsigned long *queue_id)
{
    osal_msg_queue_create(NULL, (unsigned short)msg_count, queue_id, 0, (unsigned short)msg_size);
}

static uint32_t at_base_api_msg_queue_write(unsigned long queue_id, void *msg_ptr,
                                            uint32_t msg_size, uint32_t timeout)
{
    return osal_msg_queue_write_copy(queue_id, msg_ptr, msg_size, timeout);
}

static uint32_t at_base_api_msg_queue_read(unsigned long queue_id, void *buf_ptr,
                                           uint32_t *buf_size, uint32_t timeout)
{
    return osal_msg_queue_read_copy(queue_id, buf_ptr, buf_size, timeout);
}

static void at_base_api_task_pause(void)
{
    osal_yield();
}

static void* at_base_api_malloc(uint32_t size)
{
    return osal_kmalloc(size, OSAL_GFP_ATOMIC);
}

static void at_base_api_free(void *addr)
{
    osal_kfree(addr);
}

static void at_base_api_register(void)
{
    at_base_api_t base_api = {
        .msg_queue_create_func = at_base_api_queue_create,
        .msg_queue_write_func = at_base_api_msg_queue_write,
        .msg_queue_read_func = at_base_api_msg_queue_read,
        .task_pause_func = at_base_api_task_pause,
        .malloc_func = at_base_api_malloc,
        .free_func = at_base_api_free,
    };
    uapi_at_base_api_register(base_api);
}

static void at_cmd_sw_callback(void)
{
    static volatile bool g_at_cmd_status = false;
    g_at_cmd_status = !g_at_cmd_status;
    if (g_at_cmd_status) {
        uapi_uart_deinit(TEST_SUITE_UART_BUS);
        at_uart_init();
        at_base_api_register();
        uapi_at_channel_write_register(AT_UART_PORT, at_write_func);
        at_bt_cmd_register();
        bth_ble_common_at_cmd_register();
        bth_sle_common_at_cmd_register();
        osThreadResume(g_at_task_id);
        osThreadSuspend(g_testsuite_task_id);
    } else {
        uapi_uart_deinit(AT_UART_BUS);
        test_suite_uart_init();
        osThreadResume(g_testsuite_task_id);
        osThreadSuspend(g_at_task_id);
    }
}

void set_task_id(uint32_t *at_task_id, uint32_t *testsuite_task_id)
{
    g_at_task_id = at_task_id;
    g_testsuite_task_id = testsuite_task_id;
}

int testsuite_sw_at(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    at_cmd_sw_callback();
    return 0;
}

at_ret_t at_sw_testsuite(void)
{
    at_cmd_sw_callback();
    return AT_RET_OK;
}