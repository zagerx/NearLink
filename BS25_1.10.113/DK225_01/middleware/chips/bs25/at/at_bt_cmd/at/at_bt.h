/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: At bt header \n
 * Author: @CompanyNameTag \n
 */

#ifndef AT_PLT_H
#define AT_PLT_H

#include "td_base.h"
#include "platform_core.h"
#include "at.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AT_UART_BUS TEST_SUITE_UART_BUS // UART H0

/* at UART Reception PIN to use */
#define AT_UART_TX_PIN TEST_SUITE_UART_TX_PIN
#define AT_UART_RX_PIN TEST_SUITE_UART_RX_PIN

#define AT_UART_BAUD_RATE 115200

td_void los_at_bt_cmd_register(td_void);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of at_bt.h */
