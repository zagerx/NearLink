/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved. \n
 *
 * Description:  \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2024-01-13, Create file. \n
 */

#include "tiot_slave_board_port.h"
#include "gpio.h"
#include "uart.h"
#include "pinctrl.h"
#include "soc_osal.h"
#include "slave_board_port_config.h"

int32_t tiot_slave_board_init(void *param)
{
    tiot_unused(param);
    const uint32_t *pins = (const uint32_t *)g_slave_board_hw_info.pm_info;
    uint32_t wakeout_pin = pins[TIOT_SLAVE_PIN_DEVICE_WAKEUP_HOST];
    if (wakeout_pin != TIOT_PIN_NONE) {
        (void)uapi_pin_set_mode(wakeout_pin, (pin_mode_t)HAL_PIO_FUNC_GPIO);
        (void)uapi_pin_set_ds(wakeout_pin, (pin_drive_strength_t)(PIN_DS_MAX - 1));
        (void)uapi_pin_set_pull(wakeout_pin, PIN_PULL_DOWN);
        (void)uapi_gpio_set_dir(wakeout_pin, GPIO_DIRECTION_OUTPUT);
        (void)uapi_gpio_set_val(wakeout_pin, GPIO_LEVEL_LOW);
    }

#if defined SUPPORT_UART_TYPE_CHOOSE && SUPPORT_UART_TYPE_CHOOSE == 1
    g_slave_board_hw_info.xmit_id = (uintptr_t)(HAL_GetUartNum());
#endif
    return 0;
}

void tiot_slave_board_deinit(void *param)
{
    tiot_unused(param);
}

const tiot_slave_board_info *tiot_slave_board_get_info(void)
{
    return &g_slave_board_info;
}
