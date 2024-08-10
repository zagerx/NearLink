/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides lp_uart register \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-23, Create file. \n
 */
 
#ifndef HAL_LP_UART_REG_DEF_H
#define HAL_LP_UART_REG_DEF_H
 
#include <stdint.h>
 
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
 
/**
 * @defgroup drivers_hal_lp_uart_regs_def LP_UART Regs Definition
 * @ingroup  drivers_hal_lp_uart
 * @{
 */
 
typedef union lp_uart_ctrl {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t reg_numerator          : 8;    /* !< RW, LP_UART numerator register.
                                                    Configures baud rate, applies to both RX and TX */
        uint32_t reg_denominator        : 8;    /* !< RW, LP_UART denominator register.
                                                    Configures baud rate, applies to both RX and TX */
        uint32_t rx_ready_pol           : 1;    /* !< RW, LP_UART rx_ready_pol register.
                                                    Configures the polarity of the Hardware flow control ready signal.
                                                    If set to 0, then the hardware will drive low when there is space
                                                    in the RX FIFO and high when it is almost full.
                                                    If set to 1, the hardware drives high for space and low for full */
        uint32_t tx_ready_pol           : 1;    /* !< RW, LP_UART tx_ready_pol register.
                                                    Configures the polarity of the Hardware flow control ready signal.
                                                    If set to zero, the hardware will transmit when the line
                                                    is sampled low and will pause when the line is sampled high.
                                                    If set to 1, the hardware will pause when the line is low and
                                                    transmit when the line is high.
                                                    This is only checked at the start of a frame, frames
                                                    that are already in progress cannot be paused. */
 
        uint32_t reg_length             : 2;    /* !< RW, LP_UART reg_length register.
                                                    Length of data bits is this value plus 5,
                                                    applies to both RX and TX. */
        uint32_t reg_parity_en          : 1;    /* !< RW, LP_UART reg_parity_en register.
                                                    Enables Frame Error Check, applies to RX only.
                                                    the TX will always insert at least one stop bit. */
        uint32_t reg_partity_inv        : 1;    /* !< RW, LP_UART reg_partity_inv register.
                                                    0x0 : Even Parity
                                                    0x1 : Odd Parity
                                                    applies to both RX and TX. */
        uint32_t reg_stop_en            : 1;    /* !< RW, LP_UART reg_stop_en register.
                                                    Enables Frame Error Check, applies to RX only.
                                                    the TX will always insert at least one stop bit. */
        uint32_t reg_clk_uart_ctrl      : 2;    /* !< RW, LP_UART reg_clk_uart_ctrl register.
                                                    0x0 : CLK_LP = 0
                                                    0x1 : CLK_LP = CLK_DIG/128
                                                    0x2 : CLK_LP = CLK_DIG/32
                                                    0x3 : CLK_LP = CLK_DIG/64. */
        uint32_t reg_reset              : 1;    /* !< RW, LP_UART reg_reset register.
                                                    Active high reset for the LP UART. */
        uint32_t reg_clk_sel            : 1;    /* !< RW, LP_UART reg_clk_sel register.
                                                    0x0 : 32kHz RTC clock
                                                    0x1 : CLK_LP. */
        uint32_t reg_edge_sync          : 1;    /* !< RW, LP_UART reg_edge_sync register.
                                                    Allows the Rx logic to automatically resync on any edge of
                                                    the UART RX line. If this is not set it will only sync on
                                                    the first edge of the start bit. This can improve frequency
                                                    error tolerance, but it is data dependant. */
        uint32_t reg_clk_div            : 4;    /* !< RW, LP_UART reg_clk_div register.
                                                    Divides CLK_LP. May be used to support lower baud rates.
                                                    Clk division ratio = (1+Clk Divide). */
    } lp_uart_ctrl;
} lp_uart_ctrl_t;
 
typedef union lp_uart_ctrl1 {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t normal_sync            : 8;    /* !< RW, LP_UART normal_sync register.
                                                    Sync value to use in normal operation */
        uint32_t deep_sleep_sync        : 8;    /* !< RW, LP_UART deep_sleep_sync register.
                                                    Sync value to use when waking from deep sleep */
        uint32_t idle_timeout           : 8;    /* !< RW, LP_UART idle_timeout register.
                                                    Timeout threshold */
        uint32_t rx_threshold           : 5;    /* !< RW, LP_UART rx_threshold register.
                                                    Rx fifo waterline threshold */
        uint32_t reserved29_31          : 3;
    } lp_uart_ctrl1;
} lp_uart_ctrl1_t;
 
typedef union lp_uart_status {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t frame_error_count      : 4;    /* !< RO, LP_UART frame_error_count register.
                                                    Count of the number of stop bits mis-detected */
        uint32_t parity_error_count     : 4;    /* !< RO, LP_UART parity_error_count register.
                                                    Count of the parity failures */
        uint32_t reserved8_11           : 4;    /* !< RO, LP_UART reserved register. */
 
        uint32_t tx_fifo_level          : 3;    /* !< RO, LP_UART tx_fifo_level register.
                                                    0x0 : Tx Fifo was empty.
                                                    0x7 : Tx Fifo was full. */
        uint32_t rx_state_machine       : 2;    /* !< RO, LP_UART rx_state_machine register.
                                                    0x0 : IDEL.
                                                    0x1 : SHIFT.
                                                    0x2 : CHECK. */
        uint32_t tx_state_machine       : 1;    /* !< RO, LP_UART tx_state_machine register.
                                                    0x0 : IDEL.
                                                    0x1 : SHIFT. */
        uint32_t rx_new_data_irq        : 1;    /* !< RO, LP_UART rx_new_data register.
                                                    0x0 : Current status is not rx_new_data_irq
                                                    0x1 : Current status is rx_new_data_irq */
        uint32_t rx_fifo_overflow_irq   : 1;    /* !< RO, LP_UART rx_fifo_overflow register.
                                                    0x0 : Current status is not rx_fifo_overflow_irq
                                                    0x1 : Current status is rx_fifo_overflow_irq */
        uint32_t rx_frame_error_irq     : 1;    /* !< RO, LP_UART rx_frame_error register.
                                                    0x0 : Current status is not rx_frame_error_irq
                                                    0x1 : Current status is rx_frame_error_irq */
        uint32_t rx_parity_error_irq    : 1;    /* !< RO, LP_UART rx_parity_error register.
                                                    0x0 : Current status is not rx_parity_error_irq
                                                    0x1 : Current status is rx_parity_error_irq */
        uint32_t rx_fifo_timeout_int    : 1;    /* !< RO, LP_UART rx_fifo_timeout_int register.
                                                    0x0 : Current status is not rx_fifo_timeout_int
                                                    0x1 : Current status is rx_fifo_timeout_int */
        uint32_t rx_fifo_irq            : 1;    /* !< RO, LP_UART rx_fifo_interrupt register.
                                                    0x0 : Current status is not rx_fifo_interrupt
                                                    0x1 : Current status is rx_fifo_interrupt */
        uint32_t tx_irq                 : 1;    /* !< RO, LP_UART tx_irq register.
                                                    0x0 : Current status is not tx_irq
                                                    0x1 : Current status is tx_irq */
        uint32_t rx_fifo_level          : 5;    /* !< RO, LP_UART rx_fifo_level register.
                                                    0x0  : Rx Fifo was empty.
                                                    0x31 : Rx Fifo was full. */
        uint32_t reserved30_31          : 2;
    } lp_uart_status;
} lp_uart_status_t;
 
typedef union lp_uart_irq_clr {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t clear_rx_new_data_irq      : 1;    /* !< EXIT, LP_UART clear rx_new_data_irq register.
                                                        0x0 : Don't clear associated interrupt
                                                        0x1 : Clear associated interrupt */
        uint32_t clear_rx_fifo_overflow_irq : 1;    /* !< EXIT, LP_UART clear rx_fifo_overflow_irq register.
                                                        0x0 : Don't clear associated interrupt
                                                        0x1 : Clear associated interrupt */
        uint32_t clear_rx_frame_error_irq   : 1;    /* !< EXIT, LP_UART clear rx_frame_error_irq register.
                                                        0x0 : Don't clear associated interrupt
                                                        0x1 : Clear associated interrupt */
        uint32_t clear_rx_parity_error_irq  : 1;    /* !< EXIT, LP_UART clear rx_parity_error_irq register.
                                                        0x0 : Don't clear associated interrupt
                                                        0x1 : Clear associated interrupt */
        uint32_t clear_timeout_int          : 1;    /* !< EXIT, LP_UART clear timeout_int register.
                                                        0x0 : Don't clear associated interrupt
                                                        0x1 : Clear associated interrupt */
        uint32_t no_used                    : 1;    /* !< EXIT, LP_UART clear no_used register.
                                                        0x0 : rx fifo was empty
                                                        0x1 : rx fifo was not empty */
        uint32_t clear_tx_irq               : 1;    /* !< EXIT, LP_UART clear clear_tx_irq register.
                                                        0x0 : Don't clear associated interrupt
                                                        0x1 : Clear associated interrupt */
        uint32_t reserved7_31               : 25;
    } lp_uart_irq_clr;
} lp_uart_irq_clr_t;
 
typedef union lp_uart_tx_data {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t tx_data                : 8;    /* !< EXIT, LP_UART tx_data register.
                                                    Data to transmit. */
        uint32_t reserved8_31           : 24;
    } lp_uart_tx_data;
} lp_uart_tx_data_t;
 
typedef union lp_uart_rx_data {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t rx_data                : 8;    /* !< RO, LP_UART rx_data register.
                                                    Received data that has passed parity and
                                                    frame checks if enabled. */
        uint32_t fifo_level             : 5;    /* !< RO, LP_UART fifo_level register.
                                                    0x0  : Rx Fifo was empty.
                                                    0x31 : Rx Fifo was full. */
        uint32_t reserved13_31          : 19;
    } lp_uart_rx_data;
} lp_uart_rx_data_t;
 
typedef union lp_uart_rx_freq_status {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t count_register         : 9;    /* !< RO, LP_UART count_register register.
                                                    Internal count register when first edge
                                                    (after start bit) was detected. */
        uint32_t bit_count              : 4;    /* !< RO, LP_UART bit_count register.
                                                    Indicates which bit the edge was likely on,
                                                    0=start bit, 1=first data bit, etc. */
        uint32_t reserved13_31          : 19;
    } lp_uart_rx_freq_status;
} lp_uart_rx_freq_status_t;
 
typedef union lp_uart_irq_conf {
    uint32_t d32;                          /*!< Raw register data. */
    struct {
        uint32_t rx_new_data_irq        : 1;    /* !< RW, LP_UART rx_new_data register.
                                                    0x0 : No new Rx data has been placed in the Fifo
                                                    0x1 : New Rx Data has been placed in the Fifo */
        uint32_t rx_fifo_overflow_irq   : 1;    /* !< RW, LP_UART rx_fifo_overflow register.
                                                    0x0 : Rx Fifo overflow does not occur
                                                    0x1 : Rx Fifo Overflow has occurred */
        uint32_t rx_frame_error_irq     : 1;    /* !< RW, LP_UART rx_frame_error register.
                                                    0x0 : Stop bit was not 1
                                                    0x1 : Stop bit was 1 */
        uint32_t rx_parity_error_irq    : 1;    /* !< RW, LP_UART rx_parity_error register.
                                                    0x0 : Parity check bit succeeded
                                                    0x1 : Parity bit failed */
        uint32_t rx_fifo_timeout_int    : 1;    /* !< RW, LP_UART rx_fifo_timeout register.
                                                    0x0 : Rx Fifo is not timeout
                                                    0x1 : Rx Fifo is timeout */
        uint32_t rx_fifo_interrupt      : 1;    /* !< RW, LP_UART rx_fifo_interrupt register.
                                                    0x0 : Rx Fifo is empty
                                                    0x1 : Rx Fifo is not empty */
        uint32_t tx_irq                 : 1;    /* !< RW, LP_UART rx_fifo_interrupt register.
                                                    0x0 : Tx Fifo is empty
                                                    0x1 : Tx Fifo is not empty */
        uint32_t reserved7_31           : 25;
    } lp_uart_irq_conf;
} lp_uart_irq_conf_t;
 
typedef struct lp_uart_reg {
    volatile uint32_t lp_uart_ctrl;
    volatile uint32_t lp_uart_ctrl1;
    volatile uint32_t lp_uart_status;
    volatile uint32_t lp_uart_irq_clr;
    volatile uint32_t lp_uart_tx_data;
    volatile uint32_t lp_uart_rx_data;
    volatile uint32_t lp_uart_rx_freq_status;
    volatile uint32_t lp_uart_irq_conf;
} lp_uart_reg_t;
 
/**
 * @} end of group DRIVER_HAL_LP_UART
 */
 
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
 
#endif