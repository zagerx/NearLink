/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides efuse porting template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-03， Create file. \n
 */
#ifndef EFUSE_PORTING_H
#define EFUSE_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_core.h"
#include "hal_efuse_v130_reg_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_efuse EFUSE
 * @ingroup  drivers_port
 * @{
 */

typedef enum {
    HAL_EFUSE_REGION_0,
    HAL_EFUSE_REGION_MAX,
} hal_efuse_region_t;

/**
 * @brief  Base address list.
 */
extern efuse_regs_t *g_efuse_base_addr[HAL_EFUSE_REGION_MAX];

/**
 * @brief Register hal funcs objects into hal_efuse module.
 */
void efuse_port_register_hal_funcs(void);

/**
 * @brief Unregister hal funcs objects from hal_efuse module.
 */
void efuse_port_unregister_hal_funcs(void);

 /**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif