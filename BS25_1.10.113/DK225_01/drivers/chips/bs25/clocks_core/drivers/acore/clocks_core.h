/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  CLOCKS CORE PRIVATE HEADER.
 * Author: @CompanyNameTag
 * Create: 2021-06-16
 */
#ifndef CLOCKS_CORE_H
#define CLOCKS_CORE_H

#include "hal_clocks_application_private.h"
#include "clocks_types.h"

/**
 * @addtogroup connectivity_drivers_non_os_clocks_core
 * @{
 */
/**
 * @brief  Get the value of property is_ss_chip.
 * @return Value of the property is_ss_chip.
 */
bool is_ss_chip_get(void);

/**
 * @brief  Set the value of property is_ss_chip.
 * @param  value Value of the property is_ss_chip.
 */
void is_ss_chip_set(bool value);

/**
 * @brief  Set the 0p8 voltage according to the pvsensor.
 */
void clocks_set_0p8_voltage_according_pvsensor(void);

/**
 * @brief  Config i2s clock source.
 * @param  source_config Clock source.
 */
void clocks_system_i2s_clk_config(clocks_i2s_src_t source_config);

/**
 * @brief Clock init function used in ssb.
 */
void clocks_ssb_clk_init(void);

/**
 * @brief System Clock init function.
 * @return Return clock init success or failed.
 */
bool clocks_app_clk_init(void);

/**
 * @brief  Get spi clock frequency value.
 * @return Return clock of spi frequency.
 */
uint32_t clocks_get_spi_clock_value(void);

/**
 * @brief  Get xip clock frequency value.
 * @return Return clock of xip frequency.
 */
uint32_t clocks_get_xip_clock_value(void);

/**
 * @}
 */
#endif