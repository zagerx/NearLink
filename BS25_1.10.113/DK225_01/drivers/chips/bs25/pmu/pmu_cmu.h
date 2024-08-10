/*
 * Copyright (c) @CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  BT PMU DRIVER HEADER FILE
 * Author: @CompanyNameTag
 * Create: 2019-12-19
 */

#ifndef NON_OS_PMU_APPLICATION_PMU_CMU_H
#define NON_OS_PMU_APPLICATION_PMU_CMU_H

#include "chip_io.h"

/** @defgroup connectivity_drivers_non_os_pmu_cmu PMU CMU
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */

typedef struct {
    uint16_t fll_fbdiv;
    uint8_t fll_postdiv;
} cmu_ulpfll_config_t;

typedef struct {
    uint16_t pll_fbdiv;
    uint16_t pll_frac_h;
    uint16_t pll_frac_l;
    uint8_t pll_postdiv1;
    uint8_t pll_postdiv2;
    uint8_t pll_mode;
} cmu_fnpll_config_t;

/**
 * @brief  Definition of PMU CMU cores.
 */
typedef enum {
    PMU_CMU_ULPFLL,
    PMU_CMU_FNPLL,
    PMU_CMU_MAX,
} pmu_cmu_core_t;

/**
 * @brief Get pll used or not.
 * @param pll Which PLL.
 * @return Return true if pll is used, false if not used.
 */
bool pmu_cmu_pll_used_get(pmu_cmu_core_t pll);

/**
 * @brief  Cmu pll power control.
 * @param  pll_core  Which PLL.
 * @param  pd_switch  power on or off.
 */
void pmu_cmu_pd_config(pmu_cmu_core_t pll_core, switch_type_t pd_switch);

/**
 * @brief  Get pmu cmu pll status.
 * @param  pll_core  Which PLL.
 * @return bool
 */
bool pmu_cmu_get_pll_status(pmu_cmu_core_t pll_core);

/**
 * @brief  Init set pmu cmu pll status.
 * @param  pll_core  Which PLL.
 * @param  used      PLL used or not..
 */
void pmu_cmu_init_pll_status(pmu_cmu_core_t pll_core, bool used);

/**
 * @brief  Cmu txco init.
 */
void pmu_cmu_tcxo_init(void);

/**
 * @brief  Adjust PLL clocks.
 * @param  step     PLL adjust step num.
 */
void pmu_cmu_pll_step(int32_t step);

/**
 * @brief  Register pll adjust ipc callback.
 */
void hal_lpc_init(void);

/**
 * @brief  Get pll config value.
 * @param  pll_core  Which PLL.
 * @return PLL clock value.
 */
uint32_t pmu_cmu_get_pll_clock_value(pmu_cmu_core_t pll_core);

void pmu_cmu_fnpll_reinit(cmu_fnpll_config_t *pll_cfg);

void pmu_cmu_fnpll_deinit(void);

void pmu_cmu_ulpfll_reinit(cmu_ulpfll_config_t *fll_cfg);

void pmu_cmu_ulpfll_deinit(void);

/**
  * @}
  */
#endif
