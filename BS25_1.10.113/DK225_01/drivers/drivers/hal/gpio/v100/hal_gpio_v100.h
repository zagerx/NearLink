/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V100 HAL gpio \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#ifndef HAL_GPIO_V100_H
#define HAL_GPIO_V100_H

#include "hal_gpio.h"
#include "hal_gpio_v100_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_gpio_v100 GPIO V100
 * @ingroup  drivers_hal_gpio
 * @{
 */

/**
 * @if Eng
 * @brief  Handler of the gpio interrupt request.
 * @else
 * @brief  GPIO中断处理函数
 * @endif
 */
void hal_gpio_irq_handler(void);

/**
 * @if Eng
 * @brief  Get the context of current module, see @ref gpio_context_t.
 * @return The context of gpio module, see @ref gpio_context_t.
 * @else
 * @brief  获取当前模块的上下文描述，参考 @ref gpio_context_t.
 * @return gpio 模块的上下文描述，参考 @ref gpio_context_t.
 * @endif
 */
gpio_context_t *gpio_context_get(void);

/**
 * @if Eng
 * @brief  Get the context of current module, see @ref hal_gpio_context_t.
 * @return The context of gpio module, see @ref hal_gpio_context_t.
 * @else
 * @brief  获取当前模块的上下文描述，参考 @ref hal_gpio_context_t.
 * @return 当前模块的上下文描述，参考 @ref hal_gpio_context_t.
 * @endif
 */
hal_gpio_context_t *hal_gpio_context_get(void);

/**
 * @if Eng
 * @brief  HAL GPIO init interface.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层GPIO的初始化接口。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
void hal_gpio_v100_init(void);

/**
 * @if Eng
 * @brief  HAL GPIO deinit interface.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层GPIO去初始化接口。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
void hal_gpio_v100_deinit(void);

/**
 * @if Eng
 * @brief  HAL GPIO setdir device interface.
 * @param  [in]  pin PIN to use. see @ref pin_t.
 * @param  [in]  dir Input or output direction. see @ref gpio_direction_t.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层GPIO设置输入输出方向接口。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @param  [in]  dir 输入输出方向， 参考 @ref gpio_direction_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_gpio_v100_setdir(pin_t pin, gpio_direction_t dir);

/**
 * @if Eng
 * @brief  HAL GPIO setdir device interface.
 * @param  [in]  pin PIN to use. see @ref pin_t.
 * @retval GPIO input or output direction.
 * @else
 * @brief  HAL层GPIO获取输入输出方向接口。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @retval GPIO输入输出方向 。
 * @endif
 */
gpio_direction_t hal_gpio_v100_getdir(pin_t pin);

/**
 * @if Eng
 * @brief  HAL GPIO control interface.
 * @param  [in]  pin PIN to use. see @ref pin_t
 * @param  [in]  id ID of the GPIO control.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层GPIO控制接口。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @param  [in]  id GPIO控制请求ID。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_gpio_v100_ctrl(pin_t pin, hal_gpio_ctrl_id_t id);

/**
 * @if Eng
 * @brief  HAL GPIO output interface.
 * @param  [in]  pin PIN to use. see @ref pin_t.
 * @param  [in]  level  Set to HIGH or LOW the output of a GPIO previously claimed for output. see @ref gpio_level_t.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层GPIO的output接口 。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @param  [in]  level 设置output输出HIGH or LOW，参考 @ref gpio_level_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_gpio_v100_output(pin_t pin, gpio_level_t level);

/**
 * @if Eng
 * @brief  HAL GPIO get output value interface.
 * @param  [in]  pin PIN to use. see @ref pin_t.
 * @retval GPIO output level value(high or low).
 * @else
 * @brief  HAL层获取GPIO的output值接口 。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @retval GPIO输出电平值 。
 * @endif
 */
gpio_level_t hal_gpio_v100_get_outval(pin_t pin);

/**
 * @if Eng
 * @brief  HAL GPIO get input level interface.
 * @param  [in]  pin PIN to use. see @ref pin_t.
 * @retval GPIO level, see @ref gpio_level_t.
 * @else
 * @brief  HAL层GPIO的input接口 。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @retval GPIO电平，参考 @ref gpio_level_t 。
 * @endif
 */
gpio_level_t hal_gpio_v100_get_inval(pin_t pin);

/**
 * @if Eng
 * @brief  HAL GPIO register a callback for the PIN.
 * @param  [in]  pin PIN to use. see @ref pin_t.
 * @param  [in]  trigger Trigger under which the callback will be invoked.
 * @param  [in]  callback Pointer to callback. see @ref gpio_callback_t.
 * @param  [in]  need_callback Whether callback is needed.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层GPIO的注册中断接口。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @param  [in]  trigger 触发回调的触发器。
 * @param  [in]  callback 指向回调的指针，参考 @ref gpio_callback_t 。
 * @param  [in]  need_callback 是否需要回调 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_gpio_v100_register(pin_t pin, uint32_t trigger, gpio_callback_t callback, bool need_callback);

/**
 * @if Eng
 * @brief  HAL GPIO unregister a callback for the PIN.
 * @param  [in]  pin PIN to use. see @ref pin_t.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层GPIO的注册中断接口。
 * @param  [in]  pin io引脚， 参考 @ref pin_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_gpio_v100_unregister(pin_t pin);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif