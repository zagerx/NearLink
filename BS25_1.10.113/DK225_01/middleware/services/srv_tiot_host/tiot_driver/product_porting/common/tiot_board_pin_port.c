/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Board PIN port. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-20, Create file. \n
 */
#include "tiot_board_pin_port.h"
#include "gpio.h"
#include "tiot_board_pin_port_config.h"
#include "tiot_board_log.h"
#ifdef SUPPORT_PM_WAKEUP
#include "pmu_interrupt.h"
#include "pm_sleep_porting.h"
#endif

#define err2ret(errcode)      (((errcode) == ERRCODE_SUCC) ? 0 : -1)

/**
 * @brief  Board set pin level interface.
 * @param  [in] pin   Pin id.
 * @param  [in] level Pin level, see @ref tiot_pin_level.
 * @return zero if OK;
 *         non-zero if FAIL.
 */
int32_t tiot_board_pin_set_level(uint32_t pin, uint8_t level)
{
#ifdef SUPPORT_PM_WAKEUP
    if (pin == TIOT_PIN_UART_WAKEUP_NUM) {
        return 0;
    }
#endif
    errcode_t errcode;
    gpio_level_t gpio_level = (level == TIOT_PIN_LEVEL_LOW) ? GPIO_LEVEL_LOW : GPIO_LEVEL_HIGH;
    errcode = uapi_gpio_set_val((pin_t)pin, gpio_level);
    return err2ret(errcode);
}

/**
 * @brief  Board get current pin level interface.
 * @param  [in]  pin   Pin id.
 * @param  [out] level Current pin level, see @ref tiot_pin_level.
 * @return zero if OK;
 *         non-zero if FAIL.
 */
int32_t tiot_board_pin_get_level(uint32_t pin, uint8_t *level)
{
#ifdef SUPPORT_PM_WAKEUP
    if (pin == TIOT_PIN_UART_WAKEUP_NUM) {
        return 0;
    }
#endif
    gpio_level_t gpio_level = uapi_gpio_get_val((pin_t)pin);
    *level = (gpio_level == GPIO_LEVEL_LOW) ? TIOT_PIN_LEVEL_LOW : TIOT_PIN_LEVEL_HIGH;
    return 0;
}

/**
 * @brief  Board pin interrupt enable interface.
 * @param  [in] pin         Pin id.
 * @param  [in] int_enable  Interrupt enable or disable, see @ref tiot_pin_int_enable.
 * @return zero if OK;
 *         non-zero if FAIL.
 */
int32_t tiot_board_pin_enable_int(uint32_t pin, uint8_t int_enable)
{
#ifdef SUPPORT_PM_WAKEUP
    if (pin == TIOT_PIN_UART_WAKEUP_NUM) {
        pm_enable_uart_h0_wkup_int(((int_enable == TIOT_PIN_INT_ENABLE) ? true : false));
        return 0;
    }
#endif
    errcode_t errcode;
    if (int_enable == TIOT_PIN_INT_ENABLE) {
        errcode = uapi_gpio_enable_interrupt((pin_t)pin);
    } else {
        errcode = uapi_gpio_disable_interrupt((pin_t)pin);
    }
    return err2ret(errcode);
}

#ifdef SUPPORT_PM_WAKEUP
static void board_pm_wakeup_func(void)
{
    uint32_t pin;
    for (uint8_t index = 0; index < TIOT_INT_PIN_NUMBER; index++) {
        pin = g_tiot_int_pin_map[index].pin;
        if (g_tiot_int_pin_map[index].callback != NULL) {
            g_tiot_int_pin_map[index].callback(pin);
            break;
        }
    }
}
#endif

void tiot_board_gpio_callback(pin_t pin, uintptr_t param)
{
    (void)param;
    uapi_gpio_clear_interrupt(pin);
    for (uint8_t index = 0; index < TIOT_INT_PIN_NUMBER; index++) {
        if ((pin == g_tiot_int_pin_map[index].pin) && (g_tiot_int_pin_map[index].callback != NULL)) {
            g_tiot_int_pin_map[index].callback(pin);
            break;
        }
    }
}

/**
 * @brief  Board pin interrupt callback register interface.
 * @note   Porting should ensure this callback is called when enters pin irq if interrupt enabled.
 * @param  [in] pin     Pin id.
 * @param  [in] int_cb  Registed interrupt callback, see @ref tiot_pin_int_callback.
 * @return zero if OK;
 *         non-zero if FAIL.
 */
int32_t tiot_board_pin_register_int_callback(uint32_t pin, tiot_pin_int_callback int_cb)
{
    errcode_t errcode;
#ifdef SUPPORT_PM_WAKEUP
    if (pin == TIOT_PIN_UART_WAKEUP_NUM) {
        pm_register_uart_h0_wkup_func(((int_cb == NULL) ? NULL : board_pm_wakeup_func));
    } else {
#endif
        if (int_cb != NULL) {
            errcode = uapi_gpio_register_isr_func((pin_t)pin, GPIO_INTERRUPT_RISING_EDGE, tiot_board_gpio_callback);
        } else {
            errcode = uapi_gpio_unregister_isr_func((pin_t)pin);
        }
        if (errcode != ERRCODE_SUCC) {
            return -1;
        }
#ifdef SUPPORT_PM_WAKEUP
    }
#endif
    for (uint8_t index = 0; index < TIOT_INT_PIN_NUMBER; index++) {
        if (pin == g_tiot_int_pin_map[index].pin) {
            g_tiot_int_pin_map[index].callback = int_cb;
            break;
        }
    }
    return 0;
}