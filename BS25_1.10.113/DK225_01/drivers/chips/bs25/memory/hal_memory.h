/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: Memory Map configuration
 * Author: @CompanyNameTag
 * Create: 2023-03-25
 */

#ifndef HAL_MEMORY_H
#define HAL_MEMORY_H

#include <stdbool.h>

/** @defgroup connectivity_drivers_hal_memory MEMORY
  * @ingroup  connectivity_drivers_hal
  * @{
  */
/**
 * @brief  Enum defining the available memory regions.
 * Represents a low -power mode for a specified region. to this only region 0 can be used for code,
 * as this is the only region that can address the full 1MB of FLASH.
 * The size of the Regions is different on different core or moudule.
 */
typedef enum {
    HAL_MEMORY_POWER_NORMAL = 0x0,      // !< Represents a the region is normal work power mode.
    HAL_MEMORY_POWER_LIGHTSLEEP = 0x1,  // !< Represents that it is a light if on normal state,
                                        // !< but it is noraml on sleep mode.
    HAL_MEMORY_POWER_DEEPSLEEP = 0x2,   // !< Represents a the region is deepsleep power mode.
    HAL_MEMORY_POWER_SHUTDOWN = 0x4,    // !< Represents a the region is shutdown power mode.
    HAL_MEMORY_POWER_NONE = 0x5,        // !< Represents a the region is error power mode.
} hal_memory_power_mode_type_t;

/**
 * @brief  Simply initialise the memory module internal structures.
 * @return None
 */
void hal_memory_init(void);

/**
  * @}
  */
#endif
