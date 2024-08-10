/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: CLOCKS CORE TYPES
 * Author: @CompanyNameTag
 * Create:
 */
#ifndef SRC_DRIVERS_NON_OS_CLOCKS_CORE_TYPES_H
#define SRC_DRIVERS_NON_OS_CLOCKS_CORE_TYPES_H

#include <stdint.h>
#include "chip_core_definition.h"
#include "chip_definitions.h"
#if MCU_ONLY
#include "clocks_switch_private.h"
#endif

/** @addtogroup connectivity_drivers_non_os_clocks_core
  * @{
  */
/**
 * @brief  Clock div cfg define
 */
#define CLOCK_DIV_0     0
#define CLOCK_DIV_1     1
#define CLOCK_DIV_2     2
#define CLOCK_DIV_3     3
#define CLOCK_DIV_4     4
#define CLOCK_DIV_5     5
#define CLOCK_DIV_6     6
#define CLOCK_DIV_7     7
#define CLOCK_DIV_8     8
#define CLOCK_DIV_9     9
#define CLOCK_DIV_10    10

/**
 * @brief System clock config enum.
 */
typedef enum {
    CLOCKS_ALL_TCXO_32M,
    CLOCKS_ALL_TCXO_64M,
    CLOCKS_ALL_PLL,
    CLOCKS_CONFIG_MAX,
} system_clocks_config_t;

/**
 * @brief I2s clock source enum.
 */
typedef enum {
    I2S_CLOCK_SOURCE_LPPLL,
    I2S_CLOCK_SOURCE_PLL,
    I2S_CLOCK_SOURCE_RC,
    I2S_CLOCK_SOURCE_CLKIN,
    I2S_CLOCK_PLL_OFF,
    I2S_CLOCK_SOURCE_MAX,
} clocks_i2s_src_t;

#if MCU_ONLY
/**
 * @brief cldo 0p8 voltage enum.
 */
typedef enum {
    CLDO_0P8_0V7,
    CLDO_0P8_0V75,
    CLDO_0P8_0V85,
    CLDO_0P8_0V9,
    CLDO_0P8_MAX,
} clocks_control_voltage_t;
#endif

/**
  * @}
  */
#endif
