/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: PMU INTERRUPT HEADER
 * Author: @CompanyNameTag
 * Create:  2018-10-15
 */

#ifndef NON_OS_APPLICATION_CORE_PMU_INTERRUPT_H
#define NON_OS_APPLICATION_CORE_PMU_INTERRUPT_H

#include "chip_io.h"
#include "platform_core.h"

/** @defgroup connectivity_drivers_non_os_pmu_interrupt PMU Interrupt
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */
/**
 * @brief  System allow mcpu to sleep.
 * @param  on           turn on/turn off.
 */
void pmu_mcpu_allow_to_sleep(switch_type_t on);

/**
 * @brief  System bypass BT core status, only judge the M core state.
 * @param  on           Bypass bt core/Do not bypass bt.
 */
void pmu_lpm_system_bypass_bt_status(switch_type_t on);

/**
 * @brief  Get msystem sleep history.
 * @return Sleep history, true indicates that sleep has occurred, false indicates that sleep has not occurred.
 */
bool pmu_msystem_sleep_history(void);

/**
 * @brief  Clear msystem sleep history.
 */
void pmu_msystem_sleep_history_clear(void);

/**
 * @brief  Pmu interrupts init, register interrupt,set priority, enable interrupt,and so on.
 */
void pmu_init_interrupts(void);

typedef void (*pm_wkup_func_t)(void);
void pm_register_uart_h0_wkup_func(pm_wkup_func_t func);

/**
  * @}
  */
#endif
