/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides V150 HAL eflash \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-08， Create file. \n
 */
#ifndef HAL_EFLASH_V150_H
#define HAL_EFLASH_V150_H

#include "hal_eflash.h"
#include "hal_eflash_v150_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_eflash_v150 EFLASH V150
 * @ingroup  drivers_hal_eflash
 * @{
 */

/**
 * @if Eng
 * @brief  Get the instance of v150.
 * @return The instance of v150.
 * @else
 * @brief  获取v150实例。
 * @return v150实例。
 * @endif
 */
hal_eflash_funcs_t *hal_eflash_v150_funcs_get(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif