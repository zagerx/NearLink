/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: Memory Map configuration
 * Author: @CompanyNameTag
 * Create: 2023-03-25
 */

#ifndef HAL_MEMORY_BT_H
#define HAL_MEMORY_BT_H

#include "hal_memory.h"
#include "chip_io.h"
#include "platform_core.h"

/** @defgroup connectivity_drivers_hal_memory_shared Memory Shared
  * @ingroup  connectivity_drivers_hal
  * @{
  */
/**
 * @brief  BCPU RAM total size 640KB.
 */
typedef enum {
    HAL_MEMORY_REGION_0,   // !< 128k
    HAL_MEMORY_REGION_1,   // !< 128k
    HAL_MEMORY_REGION_2,   // !< 128k
    HAL_MEMORY_REGION_3,   // !< 128k
    HAL_MEMORY_REGION_MAX, // !< total 640k
} hal_memory_region_type_t;

typedef enum {
    HAL_MEMORY_ROM_POWER_NORMAL = 0x15,     // !< Represents a the region is normal work power mode.
    HAL_MEMORY_ROM_POWER_TM = 0x55,         // !< Represents that it is a light if on normal state,
                                            //    but it is noraml on sleep mode.
    HAL_MEMORY_ROM_POWER_SHUTDOWN = 0x1015, // !< Represents a the region is shutdown power mode.
    HAL_MEMORY_ROM_POWER_NONE,              // !< Represents a the region is error power mode.
} hal_memory_rom_power_mode_t;

typedef enum {
    HAL_BRAM_SHARE_BT_DIAG_128K = 1,
    HAL_BRAM_SHARE_BT_DIAG_256K = 2,
    HAL_BRAM_SHARE_ALL_BCPU     = 3,
} hal_memory_bram_share_mode_t;

/**
 * @brief  Config sleep power mode of the specified region.
 * @param  region        Specifies the region of the bt ram.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_set_sleep_power_mode(hal_memory_region_type_t region, hal_memory_power_mode_type_t mode);

/**
 * @brief  Config normal power mode of the specified region.
 * @param  region        Specifies the region of the bt ram.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_set_normal_power_mode(hal_memory_region_type_t region, hal_memory_power_mode_type_t mode);

/**
 * @brief  Config bt trace ram power mode of the specified region.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_bt_trace_ram_power_mode(hal_memory_power_mode_type_t mode);

/**
 * @brief  Get memory power mode of the specified region.
 * @param  region                        Specifies the region of the itcm.
 * @return HAL_MEMORY_POWER_MODE            Specifies the power mode of the region.
 */
hal_memory_power_mode_type_t hal_memory_get_power_mode_region(hal_memory_region_type_t region);

/**
 * @brief  Control bt mem clock force enable.
 * @param  on   Force enable or not.
 * @return None
 */
void hal_memory_bt_mem_clk_force_on_control(switch_type_t on);

/**
 * @brief  Config bt rom sleep power mode of the specified region.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_set_rom_sleep_power_mode(hal_memory_rom_power_mode_t mode);

/**
 * @brief  Config bt rom normal power mode of the specified region.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_set_rom_normal_power_mode(hal_memory_rom_power_mode_t mode);

/**
 * @brief  Config bt share ram power mode of the specified region.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_set_bram_share_config(hal_memory_bram_share_mode_t mode);

/**
  * @}
  */
#endif
