/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pinctrl port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-25， Create file. \n
 */
#ifndef PINCTRL_PORTING_H
#define PINCTRL_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "securec.h"
#include "platform_core.h"
#include "chip_io.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_pinctrl Pinctrl
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief  Definition of mode-multiplexing.
 */
typedef enum {
    PIN_MODE_0 = 0,
    PIN_MODE_1 = 1,
    PIN_MODE_2 = 2,
    PIN_MODE_3 = 3,
    PIN_MODE_4 = 4,
    PIN_MODE_5 = 5,
    PIN_MODE_6 = 6,
    PIN_MODE_7 = 7,
    PIN_MODE_MAX
} pin_mode_t;

/**
 * @brief  Definition of drive-strength.
 */
typedef enum {
    PIN_DS_0  = 0,
    PIN_DS_1  = 1,
    PIN_DS_2  = 2,
    PIN_DS_3  = 3,
    PIN_DS_4  = 4,
    PIN_DS_5  = 5,
    PIN_DS_6  = 6,
    PIN_DS_7  = 7,
    PIN_DS_MAX
} pin_drive_strength_t;

/**
 * @brief  Definition of pull-up/pull-down.
 */
typedef enum {
    PIN_PULL_NONE = 0,
    PIN_PULL_DOWN = 1,
    PIN_PULL_UP   = 2,
    PIN_PULL_MAX
} pin_pull_t;


typedef enum {
    /* pinmux M 0 funciton */
    HAL_PIO_FUNC_GPIO = 0, // Default NON ULP GPIO
    HAL_PIO_FUNC_AFE = 0,

    /* pinmux M 1 funciton */
    HAL_PIO_FUNC_QSPI1 = 1,
    HAL_PIO_FUNC_I2C0_M1 = 1,
    HAL_PIO_FUNC_I2C1_M1 = 1,
    HAL_PIO_FUNC_I2C2_M1 = 1,
    HAL_PIO_FUNC_CLKIN = 1,
    HAL_PIO_FUNC_CLKOUT = 1,
    HAL_PIO_FUNC_I2S0_M1 = 1,
    HAL_PIO_FUNC_PDM_M1 = 1,
    HAL_PIO_FUNC_UART_L0_M1 = 1,
    HAL_PIO_FUNC_UART_H0_M1 = 1,
    HAL_PIO_FUNC_UART_L2_M1 = 1,
    HAL_PIO_FUNC_SPI0_M1 = 1,
    HAL_PIO_FUNC_SPI1_M1 = 1,
    HAL_PIO_FUNC_SPI2_M1 = 1,
    HAL_PIO_FUNC_SPI3_M1 = 1,

    /* pinmux M 2 funciton */
    HAL_PIO_FUNC_SWD = 2,
    HAL_PIO_FUNC_KEY_IN = 2,
    HAL_PIO_FUNC_SPI0_M2 = 2,
    HAL_PIO_FUNC_SPI1_M2 = 2,
    HAL_PIO_FUNC_UART_L0_M2 = 2,
    HAL_PIO_FUNC_UART_H0_M2 = 2,
    HAL_PIO_FUNC_UART_L2_M2 = 2,
    HAL_PIO_FUNC_I2C3_M2 = 2,
    HAL_PIO_FUNC_I2C4_M2 = 2,
    HAL_PIO_FUNC_I2S0_M2 = 2,

    /* pinmux M 3 funciton */
    HAL_PIO_FUNC_UART_L1_M3 = 3,
    HAL_PIO_FUNC_UART_H0_M3 = 3,
    HAL_PIO_FUNC_QDEC = 3,
    HAL_PIO_FUNC_I2C1_M3 = 3,
    HAL_PIO_FUNC_I2C2_M3 = 3,
    HAL_PIO_FUNC_SPI0_M3 = 3,
    HAL_PIO_FUNC_PWM_P = 3,

    /* pinmux M 4 funciton */
    HAL_PIO_FUNC_KEY_OUT = 4,
    HAL_PIO_FUNC_PWM_N_M4 = 4,
    HAL_PIO_FUNC_PDM_M4 = 4,
    HAL_PIO_FUNC_UART_L0_M4 = 4,
    HAL_PIO_FUNC_SPI4_M4 = 4,
    HAL_PIO_FUNC_BT_GLP_SYNC = 4,

    /* pinmux M 5 funciton */
    HAL_PIO_FUNC_SPI4_M5 = 5,
    HAL_PIO_FUNC_I2C2_M5 = 5,
    HAL_PIO_FUNC_PWM_N_M5 = 5,
    HAL_PIO_FUNC_QSPI0 = 5,
    HAL_PIO_FUNC_RF_TX_ASSOCLK = 5,
    HAL_PIO_FUNC_BT_RF_TDC_SIGN_TEST = 5,

    /* pinmux M 6 funciton */
    HAL_PIO_FUNC_BT_WIFI_SW = 6,
    HAL_PIO_FUNC_PULSE_CAPTURE = 6,
    HAL_PIO_FUNC_BT_ACTIVE = 6,
    HAL_PIO_FUNC_BT_STATUS = 6,
    HAL_PIO_FUNC_BT_FREQ = 6,
    HAL_PIO_FUNC_WLAN_ACTIVE = 6,
    HAL_PIO_FUNC_BT_SP_SAMP_P = 6,
    HAL_PIO_FUNC_BT_FRAME_SYNC = 6,
    HAL_PIO_FUNC_BTS_SAMPLE = 6,
    HAL_PIO_FUNC_BT_DUAL_ANT_SW = 6,
    HAL_PIO_FUNC_EXTLNA_CTRL = 6,
    HAL_PIO_FUNC_EXTLNA_RX_EN = 6,
    HAL_PIO_FUNC_STB0_SPI = 6,
    HAL_PIO_FUNC_JTAG = 6,
    HAL_PIO_FUNC_UART_L1_M6 = 6,

    /* pinmux M 7 funciton */
    HAL_PIO_FUNC_DIAG = 7, // DIAG[0] ~ DIAG[15]

    HAL_PIO_FUNC_MAX = 8,
    // input high resistance. Need config as HAL_PIO_FUNC_BT_GPIO and GPIO input M
    HAL_PIO_FUNC_DEFAULT_HIGH_Z = 0xf,
} hal_pio_func_t;


typedef enum {
    HAL_PIO_PULL_NONE,  //!< No pull down or pull up enabled.
    HAL_PIO_PULL_DOWN,  //!< Pull down enabled for this pin.
    HAL_PIO_PULL_UP,    //!< Pull up enabled for this pin.
    HAL_PIO_PULL_MAX,
} hal_pio_pull_t;

typedef struct {
    hal_pio_func_t func;
    pin_drive_strength_t drive;
    hal_pio_pull_t pull;
} hal_pio_config_t;

/**
 * @brief  Check whether the mode configured for the pin is valid.
 * @param  [in]  pin  The index of pins. see @ref pin_t
 * @param  [in]  mode The Multiplexing mode. see @ref pin_mode_t
 * @return The value 'true' indicates that the mode is valid and the value 'false' indicates that the mode is invalid.
 */
bool pin_check_mode_is_valid(pin_t pin, pin_mode_t mode);

/**
 * @brief  Register hal funcs objects into hal_pinctrl module.
 */
void pin_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_pinctrl module.
 */
void pin_port_unregister_hal_funcs(void);

void get_pio_func_config(size_t *pin_num, hal_pio_config_t **pin_func_array);

/**
 * @}
 */
#define HAL_PIO_FUNC_INVALID        HAL_PIO_FUNC_MAX

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
