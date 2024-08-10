/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides uart port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "uart_porting.h"
#include "hal_uart_v120.h"
#ifdef SUPPORT_HAL_PINCTRL
#include "pinctrl_porting.h"
#include "pinctrl.h"
#endif
#include "uart.h"
#include "platform_core.h"
#include "securec.h"
#include "osal_interrupt.h"
#include "pm_clock.h"
#if defined(CONFIG_UART_SUPPORT_DMA)
#include "dma_porting.h"
#endif

#define UART_BUS_0_BASE_ADDR UART0_BASE
#if (UART_BUS_MAX_NUM > 1)
#define UART_BUS_1_BASE_ADDR UART1_BASE
#endif
#if (UART_BUS_MAX_NUM > 2)
#define UART_BUS_2_BASE_ADDR UART2_BASE
#endif
#define UART_TRANS_LEN_MAX   1024

const uintptr_t g_uart_base_addrs[UART_BUS_MAX_NUM] = {
    (uintptr_t)UART_BUS_0_BASE_ADDR,
#if (UART_BUS_MAX_NUM > 1)
    (uintptr_t)UART_BUS_1_BASE_ADDR,
#endif
#if (UART_BUS_MAX_NUM > 2)
    (uintptr_t)UART_BUS_2_BASE_ADDR
#endif
};

uintptr_t uart_porting_base_addr_get(uart_bus_t bus)
{
    return g_uart_base_addrs[bus];
}

#ifdef SUPPORT_HAL_PINCTRL
static uart_pin_config_t g_pin_config_map[UART_BUS_MAX_NUM] = {
    /* UART_BUS_0 */
    {
        .tx_pin     = S_AGPIO15,
        .rx_pin     = S_AGPIO14,
        .cts_pin    = PIN_NONE,
        .rts_pin    = PIN_NONE
    },
#if (UART_BUS_MAX_NUM > 1)
    /* UART_BUS_1 */
    {
        .tx_pin     = S_AGPIO12,
        .rx_pin     = S_AGPIO13,
        .cts_pin    = PIN_NONE,
        .rts_pin    = PIN_NONE
    },
#endif
#if (UART_BUS_MAX_NUM > 2)
    /* UART_BUS_2 */
    {
        .tx_pin     = PIN_NONE,
        .rx_pin     = PIN_NONE,
        .cts_pin    = PIN_NONE,
        .rts_pin    = PIN_NONE
    }
#endif
};
#endif

typedef struct uart_interrupt {
    core_irq_t irq_num;
    osal_irq_handler irq_func;
}
hal_uart_interrupt_t;

static const hal_uart_interrupt_t g_uart_interrupt_lines[UART_BUS_MAX_NUMBER] = {
    { UART_0_IRQN, (osal_irq_handler)irq_uart0_handler },
#if (UART_BUS_MAX_NUM > 1)
    { UART_1_IRQN, (osal_irq_handler)irq_uart1_handler },
#endif
#if (UART_BUS_MAX_NUM > 2)
    { UART_2_IRQN, (osal_irq_handler)irq_uart2_handler },
#endif
};

void uart_port_register_hal_funcs(uart_bus_t bus)
{
    hal_uart_register_funcs(bus, hal_uart_v120_funcs_get());
}

#ifdef SUPPORT_CLOCKS_CORE
#include "clocks_core_common.h"
#endif
#include "debug_print.h"

#define CLK_32M     32000000ULL
#define CLK_16M     16000000ULL

uint32_t uart_port_get_clock_value(uart_bus_t bus)
{
    UNUSED(bus);
#ifdef SUPPORT_CLOCKS_CORE
#if defined(BUILD_APPLICATION_SSB)
    return CLK_32M;
#else
    return CLK_16M;
#endif
#else
    return UART_CLOCK_FRQ;
#endif
}

void uart_port_config_pinmux(uart_bus_t bus)
{
    UNUSED(bus);
#if SUPPORT_HAL_PINCTRL
#if (UART_BUS_MAX_NUM > 1)
#ifdef USE_EMBED_FLASH
    uapi_pin_set_mode(g_pin_config_map[UART_BUS_1].tx_pin, HAL_PIO_FUNC_UART_H0_M1);
    uapi_pin_set_mode(g_pin_config_map[UART_BUS_1].rx_pin, HAL_PIO_FUNC_UART_H0_M1);
#endif
#endif
    uapi_pin_set_mode(g_pin_config_map[UART_BUS_0].tx_pin, HAL_PIO_FUNC_UART_L0_M1);
    uapi_pin_set_mode(g_pin_config_map[UART_BUS_0].rx_pin, HAL_PIO_FUNC_UART_L0_M1);
#endif
}

void uart_port_register_irq(uart_bus_t bus)
{
    osal_irq_request(g_uart_interrupt_lines[bus].irq_num, g_uart_interrupt_lines[bus].irq_func, NULL, NULL, NULL);
    osal_irq_set_priority(g_uart_interrupt_lines[bus].irq_num, irq_prio(g_uart_interrupt_lines[bus].irq_num));
    osal_irq_enable(g_uart_interrupt_lines[bus].irq_num);
}

void uart_port_unregister_irq(uart_bus_t bus)
{
    osal_irq_disable(g_uart_interrupt_lines[bus].irq_num);
}

void irq_uart0_handler(void)
{
    hal_uart_irq_handler(UART_BUS_0);
    osal_irq_clear(UART_0_IRQN);
}

#if (UART_BUS_MAX_NUM > 1)
void irq_uart1_handler(void)
{
    hal_uart_irq_handler(UART_BUS_1);
    osal_irq_clear(UART_1_IRQN);
}
#endif

#if (UART_BUS_MAX_NUM > 2)
void irq_uart2_handler(void)
{
    hal_uart_irq_handler(UART_BUS_2);
    osal_irq_clear(UART_2_IRQN);
}
#endif

#ifdef SW_UART_DEBUG
#include <stdio.h>
#ifdef USE_CMSIS_OS
#define DEBUG_UART_RX_BUFFER_SIZE 256
#else
#define DEBUG_UART_RX_BUFFER_SIZE 1
#endif

uart_bus_t g_sw_debug_uart;
static bool g_sw_debug_uart_enabled = false;
uint8_t g_uart_rx_buffer[DEBUG_UART_RX_BUFFER_SIZE];

static void uart_rx_callback(const void *buf, uint16_t buf_len, bool remaining);

void sw_debug_uart_deinit(void)
{
    uapi_uart_deinit(g_sw_debug_uart);
    g_sw_debug_uart_enabled = false;
}

void uart_rx_callback(const void *buf, uint16_t buf_len, bool remaining)
{
    UNUSED(remaining);
    if (!g_sw_debug_uart_enabled) {
        return;
    }
    uapi_uart_write(SW_DEBUG_UART_BUS, (const void *)buf, buf_len, 0);
}

void sw_debug_uart_init(uint32_t baud_rate)
{
    uart_pin_config_t uart_pins;
    uart_attr_t uart_line_config;
    uart_buffer_config_t uart_buffer_config;

    g_sw_debug_uart = SW_DEBUG_UART_BUS;
    // TX configuration
    uart_pins.tx_pin = CHIP_FIXED_TX_PIN;
    uart_pins.rts_pin = PIN_NONE;

    // RX configuration
    uart_pins.rx_pin = CHIP_FIXED_RX_PIN;
    uart_pins.cts_pin = PIN_NONE;

    uart_line_config.baud_rate = baud_rate;
    uart_line_config.data_bits = UART_DATA_BIT_8;
    uart_line_config.parity = UART_PARITY_NONE;
    uart_line_config.stop_bits = UART_STOP_BIT_1;

    uart_buffer_config.rx_buffer_size = DEBUG_UART_RX_BUFFER_SIZE;
    uart_buffer_config.rx_buffer = g_uart_rx_buffer;

    (void)uapi_uart_init(SW_DEBUG_UART_BUS, &uart_pins, &uart_line_config, NULL, &uart_buffer_config);
    uapi_uart_register_rx_callback(SW_DEBUG_UART_BUS, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                   DEBUG_UART_RX_BUFFER_SIZE, uart_rx_callback);

    g_sw_debug_uart_enabled = true;
}

void print_str(const char *str, ...)
{
    static uint8_t s[UART_TRANS_LEN_MAX];  // This array needs to be large enough to store the string.
    va_list args;
    int32_t str_len;

    if (!g_sw_debug_uart_enabled || (str == NULL) || (strlen(str) == 0)) {
        return;
    }

    va_start(args, str);
    str_len = vsprintf_s((char *)s, sizeof(s), str, args);
    va_end(args);

    if (str_len < 0) {
        return;
    }
    uapi_uart_write(SW_DEBUG_UART_BUS, (const void *)s, str_len, 0);
}
#endif

void hal_uart_clear_pending(uart_bus_t uart)
{
    switch (uart) {
        case UART_BUS_0:
            osal_irq_clear(UART_0_IRQN);
            break;
        case UART_BUS_1:
            osal_irq_clear(UART_1_IRQN);
            break;
        case UART_BUS_2:
            osal_irq_clear(UART_2_IRQN);
            break;
        default:
            break;
    }
}

#if defined(CONFIG_UART_SUPPORT_DMA)
uint8_t uart_port_get_dma_trans_dest_handshaking(uart_bus_t bus)
{
    switch (bus) {
        case UART_BUS_0:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_L_TX;
        case UART_BUS_1:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_H0_TX;
        case UART_BUS_2:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_L2_TX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}

uint8_t uart_port_get_dma_trans_src_handshaking(uart_bus_t bus)
{
    switch (bus) {
        case UART_BUS_0:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_L_RX;
        case UART_BUS_1:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_H0_RX;
        case UART_BUS_2:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_L2_RX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}
#endif  /* CONFIG_UART_SUPPORT_DMA */

uint32_t uart_porting_lock(uart_bus_t bus)
{
    unused(bus);
    return osal_irq_lock();
}

void uart_porting_unlock(uart_bus_t bus, uint32_t irq_sts)
{
    unused(bus);
    osal_irq_restore(irq_sts);
}

void uart_port_clock_enable(uart_bus_t bus, bool on)
{
    clock_control_type_t m_control_type, glb_control_type;
    clock_mclken_aperp_type_t aperp_type;
    if (on) {
        m_control_type = CLOCK_CONTROL_MCLKEN_ENABLE;
        glb_control_type = CLOCK_CONTROL_GLB_CLKEN_ENABLE;
    } else {
        m_control_type = CLOCK_CONTROL_MCLKEN_DISABLE;
        glb_control_type = CLOCK_CONTROL_GLB_CLKEN_ENABLE;
    }

    switch (bus) {
        case UART_BUS_0:
            aperp_type = CLOCK_APERP_UART_L0_CLKEN;
            break;

        case UART_BUS_1:
            uapi_clock_control(glb_control_type, CLOCK_GLB_CLKEN_MCU_PERP_UART);
            aperp_type = CLOCK_APERP_UART_H0_CLKEN;
            break;

        case UART_BUS_2:
            aperp_type = CLOCK_APERP_UART_L2_CLKEN;
            break;

        default:
            return;
    }

    uapi_clock_control(m_control_type, aperp_type);
}