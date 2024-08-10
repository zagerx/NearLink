/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides uart port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef UART_PORT_H
#define UART_PORT_H

#include <stdint.h>
#include "platform_core.h"
#include "std_def.h"
#include "debug_print.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_uart UART
 * @ingroup  drivers_port
 * @{
 */

/*  Uart FIFO Level corresponding number of passes  */
#define UART_FIFO_INT_LEVEL_1_8_PASSNUM 4
#define UART_FIFO_INT_LEVEL_1_4_PASSNUM 8
#define UART_FIFO_INT_LEVEL_1_2_PASSNUM 16
#define UART_FIFO_INT_LEVEL_3_4_PASSNUM 24
#define UART_FIFO_INT_LEVEL_7_8_PASSNUM 28

#define SW_UART_BAUDRATE 115200UL
#define UART_L_BAUDRATE  115200UL
#define UART_BUS_MAX_NUM UART_BUS_MAX_NUMBER

typedef struct {
    uint32_t uartdr;
    uint32_t uartrsr;
    uint32_t res0;
    uint32_t res1;
    uint32_t res2;
    uint32_t res3;
    uint32_t uartfr;
    uint32_t res4;
    uint32_t uartilpr;
    uint32_t uartibrd;  // Integer baud divider
    uint32_t uartfbrd;  // Fractional Baud divider
    uint32_t uartlcr_h;
    uint32_t uartcr;
    uint32_t uartifls;
    uint32_t uartimsc;
    uint32_t uartris;
    uint32_t uartmis;
    uint32_t uarticr;
    uint32_t uartdmacr;
} uart_ctrl_t;

/** Uart FIFO Level */
typedef enum {
    UART_FIFO_INT_LEVEL_1_8,
    UART_FIFO_INT_LEVEL_1_4,
    UART_FIFO_INT_LEVEL_1_2,
    UART_FIFO_INT_LEVEL_3_4,
    UART_FIFO_INT_LEVEL_7_8,
    UART_FIFO_INT_LEVEL_MAX
} uart_fifo_int_lvl_t;

/**
 * @brief  Base address list for all of the IPs.
 */
extern const uintptr_t g_uart_base_addrs[UART_BUS_MAX_NUM];

/**
 * @brief  Register hal funcs objects into hal_uart module.
 */
void uart_port_register_hal_funcs(uart_bus_t bus);

/**
 * @brief  Get the bus clock of specified uart.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @return The bus clock of specified uart.
 */
uint32_t uart_port_get_clock_value(uart_bus_t bus);

/**
 * @brief  Config the pinmux of the uarts above.
 */
void uart_port_config_pinmux(uart_bus_t bus);

/**
 * @brief  unRegister the interrupt of uarts.
 */
void uart_port_unregister_irq(uart_bus_t bus);

/**
 * @brief  Register the interrupt of uarts.
 */
void uart_port_register_irq(uart_bus_t bus);

/**
 * @brief  Handler of UART0 IRQ.
 */
void irq_uart0_handler(void);

/**
 * @brief  Handler of UART1 IRQ.
 */
void irq_uart1_handler(void);

/**
 * @brief  Handler of UART2 IRQ.
 */
void irq_uart2_handler(void);

void hal_uart_clear_pending(uart_bus_t uart);

uintptr_t uart_porting_base_addr_get(uart_bus_t bus);

#if defined(CONFIG_UART_SUPPORT_DMA)
/**
 * @brief  Get the DMA destination handshaking select of uart transfer.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @return The DMA destination handshaking select of uart transfer.
 */
uint8_t uart_port_get_dma_trans_dest_handshaking(uart_bus_t bus);

/**
 * @brief  Get the DMA source handshaking select of uart transfer.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @return The DMA source handshaking select of uart transfer.
 */
uint8_t uart_port_get_dma_trans_src_handshaking(uart_bus_t bus);
#endif  /* CONFIG_UART_SUPPORT_DMA */

/**
 * @brief  uart lock.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 */
uint32_t uart_porting_lock(uart_bus_t bus);

/**
 * @brief  uart unlock.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @param  [in]  irq_sts The irq lock number of uart.
 */
void uart_porting_unlock(uart_bus_t bus, uint32_t irq_sts);

/**
 * @brief  UART clock enable or disable.
 * @param  [in]  bus The bus index of I2C.
 * @param  [in]  on Enable or disable.
 */
void uart_port_clock_enable(uart_bus_t bus, bool on);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif