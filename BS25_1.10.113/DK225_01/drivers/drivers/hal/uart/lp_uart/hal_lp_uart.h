/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides hal_lp_uart \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-23, Create file. \n
 */
#ifndef UART_HAL_LP_H
#define UART_HAL_LP_H
 
#include "hal_uart.h"
#include "hal_lp_uart_regs_op.h"
 
/**
 * @if Eng
 * @brief  Handler of the lp_uart tx interrupt request.
 * @param  [in]  uart The uart bus. see @ref uart_bus_t
 * @else
 * @brief  LP_UART tx中断处理函数
 * @param  [in]  uart 串口号， 参考 @ref uart_bus_t
 * @endif
 */
void hal_lp_uart_tx_irq_handler(uart_bus_t uart);
 
/**
 * @if Eng
 * @brief  Handler of the lp_uart rx interrupt request.
 * @param  [in]  uart The uart bus. see @ref uart_bus_t
 * @else
 * @brief  LP_UART rx中断处理函数
 * @param  [in]  uart 串口号， 参考 @ref uart_bus_t
 * @endif
 */
void hal_lp_uart_rx_irq_handler(uart_bus_t uart);
 
/**
 * @if Eng
 * @brief  Get interface between uart driver and lp_uart hal, see @ref hal_uart_funcs_t.
 * @return Interface between uart driver and lp_uart hal, see @ref hal_uart_funcs_t.
 * @else
 * @brief  获取Driver层UART和HAL层LP_UART的接口实例，参考 @ref hal_uart_funcs_t.
 * @return Driver层UART和HAL层LP_UART的接口实例，参考 @ref hal_uart_funcs_t.
 * @endif
 */
hal_uart_funcs_t *hal_lp_uart_funcs_get(void);
 
#endif