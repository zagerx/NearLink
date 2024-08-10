/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Application core main function for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#include "system_init.h"

#define TEST_UART_CONFIG_PARAM_NUM 5
#define TEST_UART_RX_PARAM_NUM 4
#define TEST_UART_RX_BUFF_SIZE 1024
#define TEST_UART_TX_PARAM_NUM 4
#define TEST_UART_TX_BUFF_SIZE 1024
#define TEST_UART_READ_MAX_TIME 100000

uint8_t g_uart2_rx_buff[TEST_UART_RX_BUFF_SIZE] = { 0 };
static uart_buffer_config_t g_uart2_buffer_config = {
    .rx_buffer = g_uart2_rx_buff,
    .rx_buffer_size = TEST_UART_RX_BUFF_SIZE
};

static uint8_t g_uart3_rx_buff[TEST_UART_RX_BUFF_SIZE] = { 0 };
uart_buffer_config_t g_uart3_buffer_config = {
    .rx_buffer = g_uart3_rx_buff,
    .rx_buffer_size = TEST_UART_RX_BUFF_SIZE
};

errcode_t system_uart2_init(void)
{
    uapi_pin_set_mode(S_AGPIO8, 1);
    uapi_pin_set_mode(S_AGPIO9, 1);
    uart_bus_t uart2_bus = UART_BUS_2;

    uart_pin_config_t uart2_pin_config = {
        .tx_pin = S_AGPIO8,
        .rx_pin = S_AGPIO9,
        .cts_pin = PIN_NONE,
        .rts_pin = PIN_NONE};
    uart_attr_t uart2_attr = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_BIT_8,
        .stop_bits = UART_STOP_BIT_1,
        .parity = UART_PARITY_NONE};
    uart_extra_attr_t uart2_extra_attr;
    uart2_extra_attr.tx_dma_enable = 0;
    uart2_extra_attr.rx_dma_enable = 0;
    uart2_extra_attr.rx_int_threshold = UART_FIFO_INT_LEVEL_1_8;
    errcode_t ret = uapi_uart_init(uart2_bus, &uart2_pin_config, &uart2_attr,
                                   &uart2_extra_attr, &g_uart2_buffer_config);
    return ret;
}

errcode_t system_uart3_init(void)
{
    uapi_pin_set_mode(S_AGPIO12, 1);
    uapi_pin_set_mode(S_AGPIO13, 1);
    uart_bus_t uart3_bus = UART_BUS_1;

    uart_pin_config_t uart3_pin_config = {
        .tx_pin = S_AGPIO12,
        .rx_pin = S_AGPIO13,
        .cts_pin = PIN_NONE,
        .rts_pin = PIN_NONE};
    uart_attr_t uart3_attr = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_BIT_8,
        .stop_bits = UART_STOP_BIT_1,
        .parity = UART_PARITY_NONE};
    uart_extra_attr_t uart3_extra_attr;
    uart3_extra_attr.tx_dma_enable = 0;
    uart3_extra_attr.rx_dma_enable = 0;
    uart3_extra_attr.rx_int_threshold = UART_FIFO_INT_LEVEL_1_8;
    errcode_t ret = uapi_uart_init(uart3_bus, &uart3_pin_config,
                                   &uart3_attr, &uart3_extra_attr, &g_uart3_buffer_config);
    return ret;
}

void system_init(void)
{
    system_uart2_init();
    system_uart3_init();

    /* USER CODE BEGIN system_init */
    /* USER CODE END system_init */
}