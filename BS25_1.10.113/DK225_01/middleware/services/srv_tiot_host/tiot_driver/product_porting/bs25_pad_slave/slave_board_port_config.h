/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved. \n
 *
 * Description: tiot slave board port config. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2024-01-1, Create file. \n
 */
#ifndef SLAVE_BOARD_PORT_CONFIG_H
#define SLAVE_BOARD_PORT_CONFIG_H

#include "tiot_slave_board_port.h"
#include "gpio.h"
#include "uart.h"
#if defined SUPPORT_UART_TYPE_CHOOSE && SUPPORT_UART_TYPE_CHOOSE == 1
#include "board_uart_config.h"
#include "platform_uart.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static tiot_slave_board_hw_info g_slave_board_hw_info = { UART_BUS_1,
                                                          { TIOT_PIN_NONE, TIOT_PIN_UART_WAKEUP_NUM, S_MGPIO1 } };
static tiot_slave_board_info g_slave_board_info = {
    .cfg_path = NULL,
    .hw_infos = &g_slave_board_hw_info
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif