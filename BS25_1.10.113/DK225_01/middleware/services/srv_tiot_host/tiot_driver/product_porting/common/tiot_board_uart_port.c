/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Board UART port\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-25, Create file. \n
 */
#include "tiot_board_uart_port.h"
#include "tiot_board_log.h"
#include "uart.h"
#include "uart_porting.h"
#include "pinctrl.h"
#include "pinctrl_porting.h"
#include "tiot_board_uart_port_config.h"
#include "gpio.h"

#define UART_RW_TIMEOUT    0xFFFFFFFF
#define UART_RX_BUFF_MAX   1024

static void board_uart_txrx_pinmux_init(tiot_board_uart_port *port)
{
    /* 取消rx管脚下拉配置，否则漏电 */
    uapi_pin_set_pull(port->pin_config.rx_pin, PIN_PULL_NONE);
    /* 防止板级未初始化. */
    uapi_pin_set_mode(port->pin_config.tx_pin, (pin_mode_t)port->pinmux[UART_PIN_TYPE_TX]);
    uapi_pin_set_mode(port->pin_config.rx_pin, (pin_mode_t)port->pinmux[UART_PIN_TYPE_RX]);
}

static void board_uart_switch_flowctrl(tiot_board_uart_port *port)
{
    /* 板级uart初始化不会初始化ctsrts pinmux... */
    if (port->flow_ctrl_bk == TIOT_UART_ATTR_FLOW_CTRL_ENABLE) {
        port->pin_config.cts_pin = port->pin_config_orig.cts_pin;
        port->pin_config.rts_pin = port->pin_config_orig.rts_pin;
        /* 取消cts管脚下拉配置，否则漏电 */
        uapi_pin_set_pull(port->pin_config.cts_pin, PIN_PULL_NONE);
        uapi_pin_set_mode(port->pin_config.cts_pin, (pin_mode_t)port->pinmux[UART_PIN_TYPE_CTS]);
        uapi_pin_set_mode(port->pin_config.rts_pin, (pin_mode_t)port->pinmux[UART_PIN_TYPE_RTS]);
    } else {
        port->pin_config.cts_pin = PIN_NONE;
        port->pin_config.rts_pin = PIN_NONE;
    }
}

static int32_t board_uart_reopen(uart_bus_t bus, tiot_board_uart_port *port, const uart_attr_t *attr)
{
    uart_buffer_config_t buff_cfg = { (uint8_t *)port->rx_buff, UART_RX_BUFFER_SIZE };
    (void)uapi_uart_deinit(bus);
    board_uart_txrx_pinmux_init(port);
    board_uart_switch_flowctrl(port);
    errcode_t ret = uapi_uart_init(bus, &(port->pin_config), attr, NULL, &buff_cfg);
    if (ret != ERRCODE_SUCC) {
        return -1;
    }
    uapi_uart_register_rx_callback(bus, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                   UART_RX_BUFF_MAX, port->rx_callback);
    return 0;
}

int32_t tiot_board_uart_open(tiot_xmit *xmit, tiot_xmit_callbacks *cbs)
{
    enum tiot_uart_port port_type = board_uart_bus_convert_port((uart_bus_t)xmit->id);
    if (port_type == TIOT_UART_NONE) {
        return -1;
    }
#if defined SUPPORT_UART_TYPE_CHOOSE && SUPPORT_UART_TYPE_CHOOSE == 1
    tiot_board_uart_port *port;
    if (HAL_GetUartNum() == HAL_UART_0) {
        port = &g_tiot_board_uart_port[port_type];
    } else {
        port = &g_tiot_board_uart_port_flowctrl[port_type];
    }
#else
    tiot_board_uart_port *port = &g_tiot_board_uart_port[port_type];
#endif
    const uart_attr_t init_attr = { 115200, UART_DATA_BIT_8, UART_STOP_BIT_1, UART_PARITY_NONE,
                                    ((port->flow_ctrl_bk == TIOT_UART_ATTR_FLOW_CTRL_ENABLE) ?
                                    UART_FLOW_CTRL_RTS_CTS : UART_FLOW_CTRL_NONE) };
    port->pin_config.tx_pin = port->pin_config_orig.tx_pin;
    port->pin_config.rx_pin = port->pin_config_orig.rx_pin;
    if (board_uart_reopen((uart_bus_t)xmit->id, port, &init_attr) != 0) {
        return -1;
    }
    g_tiot_xmit_cbs = *cbs;
    port->xmit = xmit;
    xmit->handle = port;
    return 0;
}

#ifdef GPIO_SET_DIR_INPUT
static void board_set_uart_pin_dir(uart_bus_t bus, gpio_direction_t dir)
{
    enum tiot_uart_port port_type = board_uart_bus_convert_port(bus);
#if defined SUPPORT_UART_TYPE_CHOOSE && SUPPORT_UART_TYPE_CHOOSE == 1
    tiot_board_uart_port *port;
    if (HAL_GetUartNum() == HAL_UART_0) {
        port = &g_tiot_board_uart_port[port_type];
    } else {
        port = &g_tiot_board_uart_port_flowctrl[port_type];
    }
#else
    tiot_board_uart_port *port = &g_tiot_board_uart_port[port_type];
#endif

    (void)uapi_pin_set_mode((pin_t)port->pin_config_orig.tx_pin, (pin_mode_t)HAL_PIO_FUNC_GPIO);
    (void)uapi_gpio_set_dir((pin_t)port->pin_config_orig.tx_pin, dir);

    (void)uapi_pin_set_mode((pin_t)port->pin_config_orig.rx_pin, (pin_mode_t)HAL_PIO_FUNC_GPIO);
    (void)uapi_gpio_set_dir((pin_t)port->pin_config_orig.rx_pin, dir);
    if (port->flow_ctrl_bk == TIOT_UART_ATTR_FLOW_CTRL_ENABLE) {
        (void)uapi_pin_set_mode((pin_t)port->pin_config_orig.rts_pin, (pin_mode_t)HAL_PIO_FUNC_GPIO);
        (void)uapi_gpio_set_dir((pin_t)port->pin_config_orig.rts_pin, dir);

        (void)uapi_pin_set_mode((pin_t)port->pin_config_orig.cts_pin, (pin_mode_t)HAL_PIO_FUNC_GPIO);
        (void)uapi_gpio_set_dir((pin_t)port->pin_config_orig.cts_pin, dir);
    }
}
#endif

void tiot_board_uart_close(tiot_xmit *xmit)
{
    uapi_uart_unregister_rx_callback(xmit->id);
    (void)uapi_uart_deinit(xmit->id);
#ifdef GPIO_SET_DIR_INPUT
    /* 将uart的相关管脚配置成输入高阻 */
    board_set_uart_pin_dir(xmit->id, GPIO_DIRECTION_INPUT);
#endif
}

int32_t tiot_board_uart_write(tiot_xmit *xmit, const uint8_t *buff, uint32_t len)
{
    return uapi_uart_write(xmit->id, buff, len, UART_RW_TIMEOUT);
}

int32_t tiot_board_uart_read(tiot_xmit *xmit, uint8_t *buff, uint32_t len)
{
    return uapi_uart_read(xmit->id, buff, len, UART_RW_TIMEOUT);
}

int32_t tiot_board_uart_set_config(tiot_xmit *xmit, tiot_uart_config *config)
{
    errcode_t ret;
    tiot_board_uart_port *port = xmit->handle;
    if ((config == NULL) || (config->attr.data_bits >= TIOT_UART_ATTR_DATABIT_BUTT) ||
        (config->attr.stop_bits >= TIOT_UART_ATTR_STOPBIT_BUTT) ||
        (config->attr.parity >= TIOT_UART_ATTR_PARITY_BUTT)) {
        return -1;
    }
    if (port == NULL) {
        return -1;
    }
    /* 确保当前无数据正在FIFO内传输，才能修改配置. */
    while (uapi_uart_has_pending_transmissions(xmit->id) == true) {;}

    const uart_attr_t attr = { config->baudrate, UART_DATA_BIT_8, UART_STOP_BIT_1, UART_PARITY_NONE,
                               ((config->attr.flow_ctrl == TIOT_UART_ATTR_FLOW_CTRL_ENABLE) ?
                               UART_FLOW_CTRL_RTS_CTS : UART_FLOW_CTRL_NONE) };
    /* 调整流控需要重新init. */
    if (config->attr.flow_ctrl != port->flow_ctrl_bk) {
        port->flow_ctrl_bk = config->attr.flow_ctrl;
        if (board_uart_reopen((uart_bus_t)xmit->id, port, &attr) != 0) {
            return -1;
        }
    }
    ret = uapi_uart_set_attr(xmit->id, &attr);
    return (ret == ERRCODE_SUCC) ? 0 : -1;
}
