/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CLOCKS DRIVER HEADER
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef SRC_DRIVERS_NON_OS_SHARED_CLOCKS_H
#define SRC_DRIVERS_NON_OS_SHARED_CLOCKS_H

#include "std_def.h"
#include "hal_clocks_glb.h"

/** @defgroup connectivity_drivers_non_os_clocks CLOCKS
  * @ingroup  connectivity_drivers_non_os
  * @{
  */

/**
 * @brief  Initialize the clocks with the given configuration.
 */
void clocks_init(void);

/**
 * @brief  Init some hardware confing about low power.
 */
void clocks_hardware_sub_init(void);

/**
 * @brief  Return the value of the tcxo clock.
 * @return value of the tcxo clock in Hz.
 */
uint32_t clocks_get_tcxo_clock(void);

/**
 * @brief  Clocks bridging enter low power config.
 * @param  on  true:enter lp, false:exit lp.
 */
void clocks_bridging_low_powerp_config(bool on);

/**
 * @brief  Return the value of the core clock.
 * @return value of the core clock in Hz.
 * @note   the value returned is the mid one between the maximum limit and lower not the instant one.
 */
uint32_t clocks_get_core_frequency(void);

/**
 * @brief  Switch to rc 32k clock.
 */
void pm_clock_switch_to_rc32k(void);

/**
  * @}
  */
#endif
