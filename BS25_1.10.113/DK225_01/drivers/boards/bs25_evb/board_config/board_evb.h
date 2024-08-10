/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  BS25 evb board config
 * Author: @CompanyNameTag
 * Create: 2021-06-16
 */
#ifndef BOARD_EVB_CONFIG_H
#define BOARD_EVB_CONFIG_H

#include "pinctrl_porting.h"
#include "platform_types.h"

hal_pio_config_t g_pio_function_config[PIN_MAX_NUMBER] = {
#ifdef USE_EMBED_FLASH
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO0
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO1
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO2
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO3
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO4
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO5
#else
    { HAL_PIO_FUNC_QSPI1, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO0 S_MGPIO0~5 for QSPI XIP NOR Flash
    { HAL_PIO_FUNC_QSPI1, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO1
    { HAL_PIO_FUNC_QSPI1, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO2
    { HAL_PIO_FUNC_QSPI1, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO3
    { HAL_PIO_FUNC_QSPI1, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO4
    { HAL_PIO_FUNC_QSPI1, PIN_DS_MAX, HAL_PIO_PULL_UP }, // !< S_MGPIO5
#endif
    { HAL_PIO_FUNC_AFE, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO6
    { HAL_PIO_FUNC_AFE, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO7

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO8
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO9

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO10
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO11

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO12
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO13

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO14
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO15

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO16
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO17
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO18
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO19

    { HAL_PIO_FUNC_AFE, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO20

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO21

    { HAL_PIO_FUNC_AFE, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO22
    { HAL_PIO_FUNC_AFE, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO23

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO24
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO25

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO26

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO27
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO28

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO29

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO30
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_MGPIO31

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO0

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO1 S_AGPIO1~4 for I2S0
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO2
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO3
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO4

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO5
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO6

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO7

    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO8
    { HAL_PIO_FUNC_GPIO, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO9

    { HAL_PIO_FUNC_SWD, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO10
    { HAL_PIO_FUNC_SWD, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO11

    { HAL_PIO_FUNC_UART_H0_M1, PIN_DS_MAX, HAL_PIO_PULL_MAX }, // !< S_AGPIO12 S_AGPIO13~13 for UARTH0
    { HAL_PIO_FUNC_UART_H0_M1, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO13

    { HAL_PIO_FUNC_UART_L0_M1, PIN_DS_MAX, HAL_PIO_PULL_DOWN }, // !< S_AGPIO14 S_AGPIO14~15 for UARTL0
    { HAL_PIO_FUNC_UART_L0_M1, PIN_DS_MAX, HAL_PIO_PULL_MAX }, // !< S_AGPIO15
};
#endif
