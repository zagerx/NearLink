/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides lp_uart register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-23， Create file. \n
 */
#ifndef HAL_LP_UART_REGS_OP_H
#define HAL_LP_UART_REGS_OP_H
 
#include "hal_lp_uart_regs_def.h"
 
#define ext_reg(width) ((1u << (width)) - 1)
 
#define LPUART_CTRL_BIT_WIDTH_NUMERATOR 8
#define LPUART_CTRL_BIT_NUMERATOR ext_reg(LPUART_CTRL_BIT_WIDTH_NUMERATOR)
 
#define LPUART_CTRL_BIT_WIDTH_DENOMINATOR 8
#define LPUART_CTRL_BIT_DENOMINATOR ext_reg(LPUART_CTRL_BIT_WIDTH_DENOMINATOR)
 
#define LPUART_CTRL_BIT_WIDTH_RX_READY_POL 1
#define LPUART_CTRL_BIT_RX_READY_POL ext_reg(LPUART_CTRL_BIT_WIDTH_RX_READY_POL)
 
#define LPUART_CTRL_BIT_WIDTH_TX_READY_POL 1
#define LPUART_CTRL_BIT_TX_READY_POL ext_reg(LPUART_CTRL_BIT_WIDTH_TX_READY_POL)
 
#define LPUART_CTRL_BIT_WIDTH_DATA_BITS 2
#define LPUART_CTRL_BIT_DATA_BITS ext_reg(LPUART_CTRL_BIT_WIDTH_DATA_BITS)
 
#define LPUART_CTRL_BIT_WIDTH_PARITY_EN 1
#define LPUART_CTRL_BIT_PARITY_EN ext_reg(LPUART_CTRL_BIT_WIDTH_PARITY_EN)
 
#define LPUART_CTRL_BIT_WIDTH_PARITY_SEL 1
#define LPUART_CTRL_BIT_PARITY_SEL ext_reg(LPUART_CTRL_BIT_WIDTH_PARITY_SEL)
 
#define LPUART_CTRL_BIT_WIDTH_FRAM_ERR_CHECK 1
#define LPUART_CTRL_BIT_FRAM_ERR_CHECK ext_reg(LPUART_CTRL_BIT_WIDTH_FRAM_ERR_CHECK)
 
#define LPUART_CTRL_BIT_WIDTH_CLK_DIV 2
#define LPUART_CTRL_BIT_CLK_DIV ext_reg(LPUART_CTRL_BIT_WIDTH_CLK_DIV)
 
#define LPUART_CTRL_BIT_WIDTH_RESET 1
#define LPUART_CTRL_BIT_RESET ext_reg(LPUART_CTRL_BIT_WIDTH_RESET)
 
#define LPUART_CTRL_BIT_WIDTH_CLK_SEL 1
#define LPUART_CTRL_BIT_CLK_SEL ext_reg(LPUART_CTRL_BIT_WIDTH_CLK_SEL)
 
#define LPUART_CTRL_BIT_WIDTH_SYNC_EN 1
#define LPUART_CTRL_BIT_SYNC_EN ext_reg(LPUART_CTRL_BIT_WIDTH_SYNC_EN)
 
#define LPUART_CTRL_BIT_WIDTH_CLK_DIV2 4
#define LPUART_CTRL_BIT_CLK_DIV2 ext_reg(LPUART_CTRL_BIT_WIDTH_CLK_DIV2)
 
#define LPUART_SYNC_BIT_WIDTH_NORMAL 8
#define LPUART_SYNC_BIT_NORMAL ext_reg(LPUART_SYNC_BIT_WIDTH_NORMAL)
 
#define LPUART_SYNC_BIT_WIDTH_DEEP_SLEEP 8
#define LPUART_SYNC_BIT_DEEP_SLEEP ext_reg(LPUART_SYNC_BIT_WIDTH_DEEP_SLEEP)
 
#define LPUART_SYNC_BIT_WIDTH_RX_IDLE_TIMEOUT 8
#define LPUART_SYNC_BIT_RX_IDLE_TIMEOUT ext_reg(LPUART_SYNC_BIT_WIDTH_RX_IDLE_TIMEOUT)
 
#define LPUART_SYNC_BIT_WIDTH_RX_THRESHOLD_CNT 5
#define LPUART_SYNC_BIT_RX_THRESHOLD_CNT ext_reg(LPUART_SYNC_BIT_WIDTH_RX_THRESHOLD_CNT)
 
#define LPUART_STAT_BIT_WIDTH_FRAM_ERR_CNT 4
#define LPUART_STAT_BIT_FRAM_ERR_CNT ext_reg(LPUART_STAT_BIT_WIDTH_FRAM_ERR_CNT)
 
#define LPUART_STAT_BIT_WIDTH_PARITY_ERR_CNT 4
#define LPUART_STAT_BIT_PARITY_ERR_CNT ext_reg(LPUART_STAT_BIT_WIDTH_PARITY_ERR_CNT)
 
#define LPUART_STAT_BIT_WIDTH_PAD 4
#define LPUART_STAT_BIT_PAD ext_reg(LPUART_STAT_BIT_WIDTH_PAD)
 
#define LPUART_STAT_BIT_WIDTH_TX_FIFO_CNT 3
#define LPUART_STAT_BIT_TX_FIFO_CNT ext_reg(LPUART_STAT_BIT_WIDTH_TX_FIFO_CNT)
 
#define LPUART_STAT_BIT_WIDTH_RX_MACHINE 2
#define LPUART_STAT_BIT_RX_MACHINE ext_reg(LPUART_STAT_BIT_WIDTH_RX_MACHINE)
 
#define LPUART_STAT_BIT_WIDTH_TX_MACHINE 1
#define LPUART_STAT_BIT_TX_MACHINE ext_reg(LPUART_STAT_BIT_WIDTH_TX_MACHINE)
 
#define LPUART_STAT_BIT_WIDTH_RX_DATA_INT 1
#define LPUART_STAT_BIT_RX_DATA_INT ext_reg(LPUART_STAT_BIT_WIDTH_RX_DATA_INT)
 
#define LPUART_STAT_BIT_WIDTH_RX_OVER_FLOW_INT 1
#define LPUART_STAT_BIT_RX_OVER_FLOW_INT ext_reg(LPUART_STAT_BIT_WIDTH_RX_OVER_FLOW_INT)
 
#define LPUART_STAT_BIT_WIDTH_RX_FRAM_ERR_INT 1
#define LPUART_STAT_BIT_RX_FRAM_ERR_INT ext_reg(LPUART_STAT_BIT_WIDTH_RX_FRAM_ERR_INT)
 
#define LPUART_STAT_BIT_WIDTH_RX_PARITY_ERR_INT 1
#define LPUART_STAT_BIT_RX_PARITY_ERR_INT ext_reg(LPUART_STAT_BIT_WIDTH_RX_PARITY_ERR_INT)
 
#define LPUART_STAT_BIT_WIDTH_RX_TIMEOUT_INT 1
#define LPUART_STAT_BIT_RX_TIMEOUT_INT ext_reg(LPUART_STAT_BIT_WIDTH_RX_TIMEOUT_INT)
 
#define LPUART_STAT_BIT_WIDTH_RX_FIFO_INT 1
#define LPUART_STAT_BIT_RX_FIFO_INT ext_reg(LPUART_STAT_BIT_WIDTH_RX_FIFO_INT)
 
#define LPUART_STAT_BIT_WIDTH_TX_INT 1
#define LPUART_STAT_BIT_TX_INT ext_reg(LPUART_STAT_BIT_WIDTH_TX_INT)
 
#define LPUART_STAT_BIT_WIDTH_RX_FIFO_CNT 5
#define LPUART_STAT_BIT_RX_FIFO_CNT ext_reg(LPUART_STAT_BIT_WIDTH_RX_FIFO_CNT)
 
#define LPUART_IRQCFG_BIT_WIDTH_RX_NEW_DATA 1
#define LPUART_IRQCFG_BIT_RX_NEW_DATA ext_reg(LPUART_IRQCFG_BIT_WIDTH_RX_NEW_DATA)
 
#define LPUART_IRQCFG_BIT_WIDTH_RX_FIFO_OVER_FLOW 1
#define LPUART_IRQCFG_BIT_RX_FIFO_OVER_FLOW ext_reg(LPUART_IRQCFG_BIT_WIDTH_RX_FIFO_OVER_FLOW)
 
#define LPUART_IRQCFG_BIT_WIDTH_RX_FRAM_ERR 1
#define LPUART_IRQCFG_BIT_RX_FRAM_ERR ext_reg(LPUART_IRQCFG_BIT_WIDTH_RX_FRAM_ERR)
 
#define LPUART_IRQCFG_BIT_WIDTH_RX_PARITY_ERR 1
#define LPUART_IRQCFG_BIT_RX_PARITY_ERR ext_reg(LPUART_IRQCFG_BIT_WIDTH_RX_PARITY_ERR)
 
#define LPUART_IRQCFG_BIT_WIDTH_RX_TIMEOUT 1
#define LPUART_IRQCFG_BIT_RX_TIMEOUT ext_reg(LPUART_IRQCFG_BIT_WIDTH_RX_TIMEOUT)
 
#define LPUART_IRQCFG_BIT_WIDTH_RX_FIFO 1
#define LPUART_IRQCFG_BIT_RX_FIFO ext_reg(LPUART_IRQCFG_BIT_WIDTH_RX_FIFO)
 
#define LPUART_IRQCFG_BIT_WIDTH_TX 1
#define LPUART_IRQCFG_BIT_TX ext_reg(LPUART_IRQCFG_BIT_WIDTH_TX)
 
#define LPUART_IRQCLR_BIT_WIDTH_RX_NEW_DATA 1
#define LPUART_IRQCLR_BIT_RX_NEW_DATA ext_reg(LPUART_IRQCLR_BIT_WIDTH_RX_NEW_DATA)
 
#define LPUART_IRQCLR_BIT_WIDTH_RX_FIFO_OVER_FLOW 1
#define LPUART_IRQCLR_BIT_RX_FIFO_OVER_FLOW ext_reg(LPUART_IRQCLR_BIT_WIDTH_RX_FIFO_OVER_FLOW)
 
#define LPUART_IRQCLR_BIT_WIDTH_RX_FRAM_ERR 1
#define LPUART_IRQCLR_BIT_RX_FRAM_ERR ext_reg(LPUART_IRQCLR_BIT_WIDTH_RX_FRAM_ERR)
 
#define LPUART_IRQCLR_BIT_WIDTH_RX_PARITY_ERR 1
#define LPUART_IRQCLR_BIT_RX_PARITY_ERR ext_reg(LPUART_IRQCLR_BIT_WIDTH_RX_PARITY_ERR)
 
#define LPUART_IRQCLR_BIT_WIDTH_RX_TIMEOUT 1
#define LPUART_IRQCLR_BIT_RX_TIMEOUT ext_reg(LPUART_IRQCLR_BIT_WIDTH_RX_TIMEOUT)
 
#define LPUART_IRQCLR_BIT_WIDTH_RX_FIFO 1
#define LPUART_IRQCLR_BIT_RX_FIFO ext_reg(LPUART_IRQCLR_BIT_WIDTH_RX_FIFO)
 
#define LPUART_IRQCLR_BIT_WIDTH_TX 1
#define LPUART_IRQCLR_BIT_TX ext_reg(LPUART_IRQCLR_BIT_WIDTH_TX)
 
#define LPUART_RX_DATA_BIT_WIDTH_DATA 8
#define LPUART_RX_DATA_BIT_DATA ext_reg(LPUART_RX_DATA_BIT_WIDTH_DATA)
 
#define LPUART_RX_DATA_BIT_WIDTH_LEVEL 4
#define LPUART_RX_DATA_BIT_LEVEL ext_reg(LPUART_RX_DATA_BIT_WIDTH_LEVEL)
 
#define LPUART_TX_DATA_BIT_WIDTH_DATA 8
#define LPUART_TX_DATA_BIT_DATA ext_reg(LPUART_TX_DATA_BIT_WIDTH_DATA)
 
#define LPUART_RX_FREQ_BIT_WIDTH_REG_CNT 9
#define LPUART_RX_FREQ_BIT_REG_CNT ext_reg(LPUART_RX_FREQ_BIT_WIDTH_REG_CNT)
 
#define LPUART_RX_FREQ_BIT_WIDTH_BIT_CNT 4
#define LPUART_RX_FREQ_BIT_BIT_CNT ext_reg(LPUART_RX_FREQ_BIT_WIDTH_BIT_CNT)
 
extern lp_uart_reg_t *g_lp_uart_reg;
 
/**
 * @brief  Set reg_numerator of lp_uart_ctrl
 * @param  [in]  numerator_val The value needs to set lp_uart_ctrl.reg_numerator.
 */
void uart_ctrl_reg_numerator_set(uint8_t numerator_val);
 
/**
 * @brief  Set reg_denominator of lp_uart_ctrl
 * @param  [in]  denominator_val The value needs to set lp_uart_ctrl.reg_denominator.
 */
void uart_ctrl_reg_denominator_set(uint8_t denominator_val);
 
/**
 * @brief  Set rx_ready_pol of lp_uart_ctrl
 * @param  [in]  rx_rdy_pol_val The value needs to set lp_uart_ctrl.rx_ready_pol.
 */
void uart_ctrl_reg_rx_ready_pol_set(uint8_t rx_rdy_pol_val);
 
/**
 * @brief  Set tx_ready_pol of lp_uart_ctrl
 * @param  [in]  tx_rdy_pol_val The value needs to set lp_uart_ctrl.tx_ready_pol.
 */
void uart_ctrl_reg_tx_ready_pol_set(uint8_t tx_rdy_pol_val);
 
/**
 * @brief  Set reg_length of lp_uart_ctrl
 * @param  [in]  length_val The value needs to set lp_uart_ctrl.reg_length.
 */
void uart_ctrl_reg_length_set(uint8_t length_val);
 
/**
 * @brief  Set reg_parity_en of lp_uart_ctrl
 * @param  [in]  parity_en_val The value needs to set lp_uart_ctrl.reg_parity_en.
 */
void uart_ctrl_reg_parity_en_set(uint8_t parity_en_val);
 
/**
 * @brief  Set reg_partity_inv of lp_uart_ctrl
 * @param  [in]  parity_inv_val The value needs to set lp_uart_ctrl.reg_partity_inv.
 */
void uart_ctrl_reg_parity_inv_set(uint8_t parity_inv_val);
 
/**
 * @brief  Set reg_stop_en of lp_uart_ctrl
 * @param  [in]  stop_en_val The value needs to set lp_uart_ctrl.reg_stop_en.
 */
void uart_ctrl_reg_stop_en_set(uint8_t stop_en_val);
 
/**
 * @brief  Set reg_clk_uart_ctrl of lp_uart_ctrl
 * @param  [in]  clk_uart_ctrl_val The value needs to set lp_uart_ctrl.reg_clk_uart_ctrl.
 */
void uart_ctrl_reg_clk_uart_ctrl_set(uint8_t clk_uart_ctrl_val);
 
/**
 * @brief  Set reg_reset of lp_uart_ctrl
 * @param  [in]  reset_val The value needs to set lp_uart_ctrl.reg_reset.
 */
void uart_ctrl_reset_set(uint8_t reset_val);
 
/**
 * @brief  Set reg_clk_sel of lp_uart_ctrl
 * @param  [in]  clk_val The value needs to set lp_uart_ctrl.reg_clk_sel.
 */
void uart_ctrl_reg_clk_set(uint8_t clk_val);
 
/**
 * @brief  Set reg_edge_sync of lp_uart_ctrl
 * @param  [in]  edge_sync_val The value needs to set lp_uart_ctrl.reg_edge_sync.
 */
void uart_ctrl_reg_edge_sync_set(uint8_t edge_sync_val);
 
/**
 * @brief  Set reg_clk_div of lp_uart_ctrl
 * @param  [in]  clk_div_val The value needs to set lp_uart_ctrl.reg_clk_div.
 */
void uart_ctrl_clk_div_set(uint8_t clk_div_val);
 
/**
 * @brief  Get reg_numerator of lp_uart_ctrl
 * @return the value of the reg_numerator.
 */
uint8_t uart_ctrl_reg_numerator_get(void);
 
/**
 * @brief  Get reg_denominator of lp_uart_ctrl
 * @return the value of the reg_denominator.
 */
uint8_t uart_ctrl_reg_denominator_get(void);
 
/**
 * @brief  Get rx_ready_pol of lp_uart_ctrl
 * @return the value of the rx_ready_pol.
 */
uint8_t uart_ctrl_reg_rx_ready_pol_get(void);
 
/**
 * @brief  Get tx_ready_pol of lp_uart_ctrl
 * @return the value of the tx_ready_pol.
 */
uint8_t uart_ctrl_reg_tx_ready_pol_get(void);
 
/**
 * @brief  Get reg_length of lp_uart_ctrl
 * @return the value of the reg_length.
 */
uint8_t uart_ctrl_reg_length_get(void);
 
/**
 * @brief  Get reg_parity_en of lp_uart_ctrl
 * @return the value of the reg_parity_en.
 */
uint8_t uart_ctrl_reg_parity_en_get(void);
 
/**
 * @brief  Get reg_partity_inv of lp_uart_ctrl
 * @return the value of the reg_partity_inv.
 */
uint8_t uart_ctrl_reg_parity_inv_get(void);
 
/**
 * @brief  Get reg_stop_en of lp_uart_ctrl
 * @return the value of the reg_stop_en.
 */
uint8_t uart_ctrl_reg_stop_en_get(void);
 
/**
 * @brief  Get reg_clk_uart_ctrl of lp_uart_ctrl
 * @return the value of the reg_clk_uart_ctrl.
 */
uint8_t uart_ctrl_reg_clk_uart_ctrl_get(void);
 
/**
 * @brief  Get reg_reset of lp_uart_ctrl
 * @return the value of the reg_reset.
 */
uint8_t uart_ctrl_reset_get(void);
 
/**
 * @brief  Get reg_clk_sel of lp_uart_ctrl
 * @return the value of the reg_clk_sel.
 */
uint8_t uart_ctrl_reg_clk_get(void);
 
/**
 * @brief  Get reg_edge_sync of lp_uart_ctrl
 * @return the value of the reg_edge_sync.
 */
uint8_t uart_ctrl_reg_edge_sync_get(void);
 
/**
 * @brief  Get reg_clk_div of lp_uart_ctrl
 * @return the value of the reg_clk_div.
 */
uint8_t uart_ctrl_clk_div_get(void);
 
/**
 * @brief  Set normal_sync of lp_uart_ctrl1
 * @param  [in]  normal_sync_val The value needs to set lp_uart_ctrl.normal_sync.
 */
void uart_ctrl1_reg_normal_sync_set(uint8_t normal_sync_val);
 
/**
 * @brief  Set deep_sleep_sync of lp_uart_ctrl1
 * @param  [in]  deep_sleep_sync_val The value needs to set lp_uart_ctrl.deep_sleep_sync.
 */
void uart_ctrl1_deep_sleep_sync_set(uint8_t deep_sleep_sync_val);
 
/**
 * @brief  Set idle_timeout of lp_uart_ctrl1
 * @param  [in]  idle_timeout_val The value needs to set lp_uart_ctrl.idle_timeout.
 */
void uart_ctrl1_idle_timeout_set(uint8_t idle_timeout_val);
 
/**
 * @brief  Set rx_threshold of lp_uart_ctrl1
 * @param  [in]  rx_threshold_val The value needs to set lp_uart_ctrl.rx_threshold.
 */
void uart_ctrl1_rx_threshold_set(uint8_t rx_threshold_val);
 
/**
 * @brief  Get normal_sync of lp_uart_ctrl1
 * @return the value of the normal_sync.
 */
uint8_t uart_ctrl1_reg_normal_sync_get(void);
 
/**
 * @brief  Get deep_sleep_sync of lp_uart_ctrl1
 * @return the value of the deep_sleep_sync.
 */
uint8_t uart_ctrl1_deep_sleep_sync_get(void);
 
/**
 * @brief  Get idle_timeout of lp_uart_ctrl1
 * @return the value of the idle_timeout.
 */
uint8_t uart_ctrl1_idle_timeout_get(void);
 
/**
 * @brief  Get rx_threshold of lp_uart_ctrl1
 * @return the value of the rx_threshold.
 */
uint8_t uart_ctrl1_rx_threshold_get(void);
 
/**
 * @brief  Get frame_error_count of lp_uart_status
 * @return the value of the frame_error_count.
 */
uint8_t uart_status_frame_err_cnt_get(void);
 
/**
 * @brief  Get parity_error_count of lp_uart_status
 * @return the value of the parity_error_count.
 */
uint8_t uart_status_parity_err_cnt_get(void);
 
/**
 * @brief  Get tx_fifo_level of lp_uart_status
 * @return the value of the tx_fifo_level.
 */
uint8_t uart_status_tx_fifo_level_get(void);
 
/**
 * @brief  Get rx_state_machine of lp_uart_status
 * @return the value of the rx_state_machine.
 */
uint8_t uart_status_rx_state_machine_get(void);
 
/**
 * @brief  Get tx_state_machine of lp_uart_status
 * @return the value of the tx_state_machine.
 */
uint8_t uart_status_tx_state_machine_get(void);
 
/**
 * @brief  Get rx_new_data_irq of lp_uart_status
 * @return the value of the rx_new_data_irq.
 */
uint8_t uart_status_rx_new_data_irq_get(void);
 
/**
 * @brief  Get rx_fifo_overflow_irq of lp_uart_status
 * @return the value of the rx_fifo_overflow_irq.
 */
uint8_t uart_status_rx_fifo_overflow_irq_get(void);
 
/**
 * @brief  Get rx_frame_error_irq of lp_uart_status
 * @return the value of the rx_frame_error_irq.
 */
uint8_t uart_status_rx_frame_err_irq_get(void);
 
/**
 * @brief  Get rx_parity_error_irq of lp_uart_status
 * @return the value of the rx_parity_error_irq.
 */
uint8_t uart_status_rx_parity_err_irq_get(void);
 
/**
 * @brief  Get rx_fifo_timeout_int of lp_uart_status
 * @return the value of the rx_fifo_timeout_int.
 */
uint8_t uart_status_rx_fifo_timeout_int_get(void);
 
/**
 * @brief  Get rx_fifo_irq of lp_uart_status
 * @return the value of the rx_fifo_irq.
 */
uint8_t uart_status_rx_fifo_irq_get(void);
 
/**
 * @brief  Get tx_irq of lp_uart_status
 * @return the value of the tx_irq.
 */
uint8_t uart_status_tx_irq_get(void);
 
/**
 * @brief  Get rx_fifo_level of lp_uart_status
 * @return the value of the rx_fifo_level.
 */
uint8_t uart_status_rx_fifo_level_get(void);
 
/**
 * @brief  Clear rx_new_data interrupt
 */
void uart_irq_clr_rx_new_data_irq(void);
 
/**
 * @brief  Clear rx_fifo_overflow interrupt
 */
void uart_irq_clr_rx_fifo_overflow_irq(void);
 
/**
 * @brief  Clear rx_frame_error interrupt
 */
void uart_irq_clr_rx_frame_error_irq(void);
 
/**
 * @brief  Clear rx_parity_error interrupt
 */
void uart_irq_clr_rx_parity_error_irq(void);
 
/**
 * @brief  Clear timeout_int interrupt
 */
void uart_irq_clr_timeout_int(void);
 
/**
 * @brief  Clear no_used interrupt
 */
void uart_irq_clr_no_used(void);
 
/**
 * @brief  Clear tx interrupt
 */
void uart_irq_clr_tx_irq(void);
 
/**
 * @brief  Set tx_data of lp_uart_tx_data
 * @param  [in]  tx_data The value needs to set lp_uart_tx_data.tx_data.
 */
void uart_tx_data_set(uint8_t tx_data);
 
/**
 * @brief  Get rx_data of lp_uart_rx_data
 * @return the value of the rx_data.
 */
uint8_t uart_rx_data_get(void);
 
/**
 * @brief  Get fifo_level of lp_uart_rx_data
 * @return the value of the fifo_level.
 */
uint8_t uart_rx_data_fifo_level_get(void);
 
/**
 * @brief  Get count_register of lp_uart_rx_freq_status
 * @return the value of the count_register.
 */
uint32_t uart_rx_freq_status_register_cnt_get(void);
 
/**
 * @brief  Get bit_count of lp_uart_rx_freq_status
 * @return the value of the bit_count.
 */
uint8_t uart_rx_freq_status_bit_cnt_get(void);
 
/**
 * @brief  Set rx_new_data_irq of lp_uart_irq_conf
 * @param  [in]  rx_new_data_irq_val The value needs to set lp_uart_irq_conf.rx_new_data_irq.
 */
void uart_irq_config_rx_new_data_irq_set(uint8_t rx_new_data_irq_val);
 
/**
 * @brief  Set rx_fifo_overflow_irq of lp_uart_irq_conf
 * @param  [in]  rx_fifo_overflow_irq_val The value needs to set lp_uart_irq_conf.rx_fifo_overflow_irq.
 */
void uart_irq_config_rx_fifo_overflow_irq_set(uint8_t rx_fifo_overflow_irq_val);
 
/**
 * @brief  Set rx_frame_error_irq of lp_uart_irq_conf
 * @param  [in]  rx_frame_err_irq_val The value needs to set lp_uart_irq_conf.rx_frame_error_irq.
 */
void uart_irq_config_rx_frame_err_irq_set(uint8_t rx_frame_err_irq_val);
 
/**
 * @brief  Set rx_parity_error_irq of lp_uart_irq_conf
 * @param  [in]  rx_parity_err_irq_val The value needs to set lp_uart_irq_conf.rx_parity_error_irq.
 */
void uart_irq_config_rx_parity_err_irq_set(uint8_t rx_parity_err_irq_val);
 
/**
 * @brief  Set rx_fifo_timeout_int of lp_uart_irq_conf
 * @param  [in]  rx_fifo_timeout_int_val The value needs to set lp_uart_irq_conf.rx_fifo_timeout_int.
 */
void uart_irq_config_rx_fifo_timeout_int_set(uint8_t rx_fifo_timeout_int_val);
 
/**
 * @brief  Set rx_fifo_interrupt of lp_uart_irq_conf
 * @param  [in]  rx_fifo_irq_val The value needs to set lp_uart_irq_conf.rx_fifo_interrupt.
 */
void uart_irq_config_rx_fifo_irq_set(uint8_t rx_fifo_irq_val);
 
/**
 * @brief  Set tx_irq of lp_uart_irq_conf
 * @param  [in]  tx_irq_val The value needs to set lp_uart_irq_conf.tx_irq.
 */
void uart_irq_config_tx_irq_set(uint8_t tx_irq_val);
 
/**
 * @brief  Get rx_new_data_irq of lp_uart_irq_conf
 * @return the value of the rx_new_data_irq.
 */
uint8_t uart_irq_config_rx_new_data_irq_get(void);
 
/**
 * @brief  Get rx_fifo_overflow_irq of lp_uart_irq_conf
 * @return the value of the rx_fifo_overflow_irq.
 */
uint8_t uart_irq_config_rx_fifo_overflow_irq_get(void);
 
/**
 * @brief  Get rx_frame_error_irq of lp_uart_irq_conf
 * @return the value of the rx_frame_error_irq.
 */
uint8_t uart_irq_config_rx_frame_err_irq_get(void);
 
/**
 * @brief  Get rx_parity_error_irq of lp_uart_irq_conf
 * @return the value of the rx_parity_error_irq.
 */
uint8_t uart_irq_config_rx_parity_err_irq_get(void);
 
/**
 * @brief  Get rx_fifo_timeout_int of lp_uart_irq_conf
 * @return the value of the rx_fifo_timeout_int.
 */
uint8_t uart_irq_config_rx_fifo_timeout_int_get(void);
 
/**
 * @brief  Get rx_fifo_interrupt of lp_uart_irq_conf
 * @return the value of the rx_fifo_interrupt.
 */
uint8_t uart_irq_config_rx_fifo_irq_get(void);
 
/**
 * @brief  Get tx_irq of lp_uart_irq_conf
 * @return the value of the tx_irq.
 */
uint8_t uart_irq_config_tx_irq_get(void);
 
#endif