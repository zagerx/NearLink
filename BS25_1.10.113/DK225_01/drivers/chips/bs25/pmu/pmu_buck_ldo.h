/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Pmu buck ldo header file. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-1-28, Create file. \n
 */

#ifndef NON_OS_APPLICATION_CORE_PMU_BUCK_LDO_H
#define NON_OS_APPLICATION_CORE_PMU_BUCK_LDO_H

#include "chip_io.h"
#include "errcode.h"

/** @defgroup connectivity_drivers_non_os_pmu_buck_ldo PMU BUCK LDO
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */

typedef enum pmu_buck_ldo {
    PMU_BUCK_LDO_0P9 = 0,
    PMU_BUCK_LDO_1P8,
    PMU_BUCK_LDO_1P1,
    PMU_BUCK_LDO_1P3,
} pmu_buck_ldo_t;
#define PMU_BUCK_LDO_MAX (PMU_BUCK_LDO_1P3 + 1)

typedef enum pmu_buck_ldo_state {
    PMU_BUCK_LDO_STATE_OFF = 0,
    PMU_BUCK_LDO_STATE_ON,
    PMU_BUCK_LDO_STATE_ECO
} pmu_buck_ldo_state_t;

typedef enum pmu_sleep_ctrl_mode {
    PMU_SLEEP_CTRL_MODE_REG,   // Controlled by register.
    PMU_SLEEP_CTRL_MODE_VSET,  // Enter/Exit ECO voltage controlled by sleep_ctrl.
    PMU_SLEEP_CTRL_MODE_ON,    // Power on/off controlled by sleep_ctrl.
    PMU_SLEEP_CTRL_MODE_ECO    // Enter/Exit ECO mode controlled by sleep_ctrl.
} pmu_sleep_ctrl_mode_t;

/**
 * @brief  Set buck ldo state.
 * @param  ldo Which buck ldo select.
 * @param  state Buck ldo state to be configured.
 */
void pmu_buck_ldo_set_state(pmu_buck_ldo_t ldo, pmu_buck_ldo_state_t state);

/**
 * @brief  Get buck ldo status.
 * @param  ldo Which buck ldo select.
 * @return Buck ldo status.
 */
pmu_buck_ldo_state_t pmu_buck_ldo_get_status(pmu_buck_ldo_t ldo);

/**
 * @brief  Config buck ldo vset.
 * @param  ldo Which buck ldo select.
 * @param  vset Buck ldo vset value to be configured.
 */
void pmu_buck_ldo_vset_cfg(pmu_buck_ldo_t ldo, uint8_t vset);

/**
 * @brief  Get buck ldo vset value.
 * @param  ldo Which buck ldo select.
 * @return Buck ldo vset value.
 */
uint8_t pmu_buck_ldo_get_vset(pmu_buck_ldo_t ldo);

/**
 * @brief  Config buck ldo eco vset.
 * @param  ldo Which buck ldo select.
 * @param  vset Buck ldo eco vset value to be configured.
 */
void pmu_buck_ldo_eco_vset_cfg(pmu_buck_ldo_t ldo, uint8_t vset);

/**
 * @brief  Get buck ldo eco vset value.
 * @param  ldo Which buck ldo select.
 * @return Buck ldo eco vset value.
 */
uint8_t pmu_buck_ldo_get_eco_vset(pmu_buck_ldo_t ldo);

/**
 * @brief  Enter/Exit ECO voltage controlled by sleep_ctrl.
 * @param  ldo Which buck ldo select.
 * @param  en true: enable, false: disable.
 */
void pmu_buck_ldo_sleep_vset_ctrl(pmu_buck_ldo_t ldo, bool en);

/**
 * @brief  Power on/off controlled by sleep_ctrl.
 * @param  ldo Which buck ldo select.
 * @param  en true: enable, false: disable.
 */
void pmu_buck_ldo_sleep_onoff_ctrl(pmu_buck_ldo_t ldo, bool en);

/**
 * @brief  Enter/Exit ECO mode controlled by sleep_ctrl.
 * @param  ldo Which buck ldo select.
 * @param  en true: enable, false: disable.
 */
void pmu_buck_ldo_sleep_eco_ctrl(pmu_buck_ldo_t ldo, bool en);

/**
 * @brief Trigger the pmu_wdt reset and reboot.
 */
void pmu_trigger_pmu_wdt_reboot(void);

/**
  * @}
  */

#endif
