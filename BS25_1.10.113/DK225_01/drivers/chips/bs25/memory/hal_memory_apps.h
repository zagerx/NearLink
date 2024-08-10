/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: Memory Map configuration
 * Author: @CompanyNameTag
 * Create: 2023-03-25
 */

#ifndef HAL_MEMORY_APPS_H
#define HAL_MEMORY_APPS_H

#include "chip_io.h"
#include "hal_memory.h"

/** @defgroup connectivity_drivers_hal_memory_apps Memory Apps
  * @ingroup  connectivity_drivers_hal
  * @{
  */
/**
 * @brief  Enum defining the available memory regions.
 * Represents a low -power mode for a specified region. to this only region 0 can be used for code,
 * as this is the only region that can address the full 1MB of FLASH.
 * The size of the Regions is different on different core or moudule.
 */
/**
 * @brief  Enum defining the available memory regions.
 * See Soc Register map for details on setting up the memory map. Note the difference in the length fields!
 * Due to this only ITCM region x can be used for code, as this is the only region that can addr the full 128k of RAM.
 * DTCM regions x can be used for data and address only 256KB
 * This effectively duplicates the memory_region_e enum in the higher layer driver,
 * but decouples the HAL from the driver.
 */
typedef enum {
    HAL_MEMORY_ITCM_REGION_0,   // !< Bank0             - 64k
    HAL_MEMORY_ITCM_REGION_1,   // !< Bank1             - 64k
    HAL_MEMORY_ITCM_REGION_2,   // !< Bank2             - 64k
    HAL_MEMORY_ITCM_REGION_3,   // !< Bank3             - 64k
    HAL_MEMORY_ITCM_REGION_4,   // !< Bank4             - 64k
    HAL_MEMORY_ITCM_REGION_5,   // !< Bank5             - 64k
    HAL_MEMORY_ITCM_REGION_6,   // !< Bank6             - 64k
    HAL_MEMORY_ITCM_REGION_7,   // !< Bank7             - 64k
    HAL_MEMORY_ITCM_REGION_NUM, // !< MAX               - ITCM total 480k
} hal_memory_itcm_region_type_t;

typedef enum {
    HAL_MEMORY_DTCM_REGION0_1,  // !< Bank0_1           - 64k
    HAL_MEMORY_DTCM_REGION2_3,  // !< Bank2_3           - 64k
    HAL_MEMORY_DTCM_REGION4_5,  // !< Bank4_5           - 64k
    HAL_MEMORY_DTCM_REGION6_7,  // !< Bank6_7           - 64k
    HAL_MEMORY_DTCM_REGION_NUM, // !< MAX               - DTCM total 256k
} hal_memory_dtcm_region_type_t;

typedef enum {
    HAL_MEMORY_XIP_CACHE_REGION,
    HAL_MEMORY_XIP_DIAG_REGION,
    HAL_MEMORY_XIP_REGION_NUM,
} hal_memory_xip_region_type_t;

typedef enum {
    HAL_PMU_DSP_MEM0,
    HAL_PMU_DSP_MEM1,
} hal_pmu_dsp_mem_type_t;

typedef enum {
    HAL_EMMC_MEM,
    HAL_SD_H_MEM,
    HAL_SD_D_MEM,
} hal_sdio_mem_t;

typedef enum {
    HAL_SDIO_MEM_NORMAL  = 0,
    HAL_SDIO_MEM_SLP     = 1,
    HAL_SDIO_MEM_DSLP    = 2,
    HAL_SDIO_MEM_SD      = 4,
} hal_sdio_pwr_t;

/**
 * @brief  Config itcm sleep power mode of the specified region.
 * @param  region        Specifies the region of the itcm.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_itcm_set_sleep_power_mode(hal_memory_itcm_region_type_t region,
                                          hal_memory_power_mode_type_t mode);

/**
 * @brief  Config dtcm sleep power mode of the specified region.
 * @param  region        Specifies the region of the itcm.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_dtcm_set_sleep_power_mode(hal_memory_dtcm_region_type_t region,
                                          hal_memory_power_mode_type_t mode);
/**
 * @brief  Config cache sleep power mode of the specified region.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_cache_set_sleep_power_mode(hal_memory_power_mode_type_t mode);

/**
 * @brief  Config itcm normal power mode of the specified region.
 * @param  region        Specifies the region of the itcm.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_itcm_set_normal_power_mode(hal_memory_itcm_region_type_t region,
                                           hal_memory_power_mode_type_t mode);

/**
 * @brief  Config dtcm normal power mode of the specified region.
 * @param  region        Specifies the region of the itcm.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_dtcm_set_normal_power_mode(hal_memory_dtcm_region_type_t region,
                                           hal_memory_power_mode_type_t mode);

/**
 * @brief  Config cache normal power mode of the specified region.
 * @param  mode          Specifies the power mode of the region.
 * @return None
 */
void hal_memory_cache_set_normal_power_mode(hal_memory_power_mode_type_t mode);

/**
 * @brief  Get itcm power mode of the specified region.
 * @param  region                        Specifies the region of the itcm.
 * @return HAL_MEMORY_POWER_MODE            Specifies the power mode of the region.
 */
hal_memory_power_mode_type_t hal_memory_itcm_get_power_mode(hal_memory_itcm_region_type_t region);

/**
 * @brief  Get dtcm power mode of the specified region.
 * @param  region                        Specifies the region of the itcm.
 * @return HAL_MEMORY_POWER_MODE             Specifies the power mode of the region.
 */
hal_memory_power_mode_type_t hal_memory_dtcm_get_power_mode(hal_memory_dtcm_region_type_t region);

/**
 * @brief  Get cache power mode of the specified region.
 * @return HAL_MEMORY_POWER_MODE            Specifies the power mode of the region.
 */
hal_memory_power_mode_type_t hal_memory_cache_get_power_mode(void);

/**
 * @brief  Config rom shut down status
 * @param  power_control  ROM enter/exit SD status
 * @return None
 */
void hal_memory_rom_shut_down_control(switch_type_t power_control);

/**
 * @brief  Set GPU memory status
 * @param  mode  Memory status
 * @return None
 */
void hal_memory_gpu_set_power_mode(hal_memory_power_mode_type_t mode);

/**
 * @brief  Set Ram memory status
 * @param  mode  Memory status
 * @return None
 */
void hal_common_ram_set_power_mode(hal_memory_power_mode_type_t mode);

/**
 * @brief  Set XIP memory status
 * @param  region Which xip region
 * @param  mode  Memory status
 * @return None
 */
void hal_memory_xip_set_power_mode(hal_memory_xip_region_type_t region,
                                   hal_memory_power_mode_type_t mode);

/**
 * @brief  Control DSP dsi mem sd status
 * @param  power_control  DSP dsi memory status
 * @return None
 */
void hal_memory_dsi_shut_down_control(switch_type_t power_control);

/**
 * @brief  Control DSP dss mem sd status
 * @param  power_control  DSP dss memory status
 * @return None
 */
void hal_memory_dss_shut_down_control(switch_type_t power_control);

/**
 * @brief  Control DSP mem0/1 power gating
 * @param  hal_dsp_mem  DSP mem0/1
 * @param  power_switch Power status
 * @return None
 */
void hal_pmu_dsp_mem_power_enable(hal_pmu_dsp_mem_type_t hal_dsp_mem, switch_type_t power_switch);

/**
 * @brief  Control mcu hs mem power gating
 * @param  power_switch Power status
 * @return None
 */
void hal_pmu_mcu_hs_power_enable(switch_type_t power_switch);

/**
 * @brief  Control dsub power gating
 * @param  power_switch Power status
 * @return None
 */
void hal_pmu_dsub_power_enable(switch_type_t power_switch);

/**
 * @brief  Get the display sub power status.
 * @return True is power on, false is poweroff.
 */
bool hal_pmu_get_dsub_power_status(void);

/**
 * @brief  Control asub power gating
 * @param  power_switch Power status
 * @return None
 */
void hal_pmu_asub_power_enable(switch_type_t power_switch);

/**
 * @brief  SDIO device ram control.
 * @param  mem_ctrl  ram control.
 * @return None
 */
void hal_sdio_memory_control(uint16_t mem_ctrl);

/**
 * @brief  SDIO ram power control.
 * @param  mem_t  Which ram region.
 * @param  mem_pwr_mode  power on/off.
 * @return None
 */
void hal_sdio_memory_power_control(hal_sdio_mem_t mem_t, hal_sdio_pwr_t mem_pwr_mode);

/**
 * @brief  EMMC/SDIO host ram control.
 * @param  ram_ctrl  ram control value.
 * @return None
 */
void hal_emmc_sdio_host_ram_ctrl(uint8_t ram_ctrl);

/**
 * @brief  Codec ram control.
 * @param  mem_mode  ram mode control.
 * @return None
 */
void hal_codec_mem_ctrl(hal_memory_power_mode_type_t mem_mode);

/**
 * @brief  l2ram power control interface.
 * @param  power_switch  power on/off.
 */
void hal_pmu_l2ram_mem_power_enable(switch_type_t power_switch);

/**
  * @}
  */
#endif
