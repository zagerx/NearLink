/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description:  Clock config header file.
 * Author: @CompanyNameTag
 * Create: 2023-05-17
 */
#ifndef CLOCKS_CONFIG_H
#define CLOCKS_CONFIG_H

#if !defined(BUILD_APPLICATION_FSB) && !defined(BUILD_APPLICATION_ROM)
#include "clocks_types.h"

/**
 * @addtogroup connectivity_drivers_non_os_clocks
 * @{
 */

#define CMU_ULPFLL_FREQUENCY    86016000
#define CMU_FNPLL_FREQUENCY     96000000

/**
 * @brief  Get the system clocks configuration for the specified clock level.
 * @return The system clocks configuration.
 */
const clocks_clk_cfg_t *clocks_system_all_clocks_get(void);

/**
 * @brief Get the ulpfll configuration.
 * @return The ulpfll configuration.
 */
cmu_ulpfll_config_t *clocks_get_ulpfll_cfg(void);

/**
 * @brief Get the fnpll configuration.
 * @return The fnpll configuration.
 */
cmu_fnpll_config_t *clocks_get_fnpll_cfg(void);

/**
 * @}
 */
#endif

#endif