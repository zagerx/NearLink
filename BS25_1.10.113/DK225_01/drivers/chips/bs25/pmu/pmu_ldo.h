/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: hal pmu buck ldo header file. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-12-09, Create file. \n
 */

#ifndef SRC_DRIVERS_BSXX_HAL_APPLICATION_HAL_PMU_LDO_H
#define SRC_DRIVERS_BSXX_HAL_APPLICATION_HAL_PMU_LDO_H

#include "chip_io.h"

/** @defgroup connectivity_drivers_hal_pmu_buck_ldo PMU BUCK LDO
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */

/**
 * @brief  Enum defining the pmu system status.
 */
typedef enum {
    HAL_PMU_SYSTEM_OFF = 0,             //!< The whole system is power off.
    HAL_PMU_SYSTEM_BOOTING = 1,         //!< The whole system is booting.
    HAL_PMU_SYSTEM_SLEEPING = 2,        //!< The whole system is sleeping.
    HAL_PMU_SYSTEM_WORK = 3,            //!< The whole system is work.
    HAL_PMU_SYSTEM_PROTECT = 4,         //!< The whole system is Power-off protection.
    HAL_PMU_SYSTEM_PROTECTING = 5,      //!< The whole system is Power-off protecting.
    HAL_PMU_SYSTEM_SLEEP = 6,           //!< The whole system is sleep.
    HAL_PMU_SYSTEM_WAKING = 7,          //!< The whole system is waking.
} hal_pmu_system_current_status_type_t;

typedef enum {
    REF2_EN_BG,
    EN_REFBUFFER,
    EN_INTLDO2,
    EN_INTLDO2_DELAY,
    EN_IBG,
    EN_IPOLY,
    EN_ITUNE,
    EN_XLDO,
    EN_XLDO_DELAY,
    EN_CLKLDO1,
    EN_CLKLDO1_DELAY,
} hal_pmu_man_sel_t;

typedef enum {
    CMU_XO_CORE_PD,
    CMU_FAST_XO_ISO,
    CMU_RC_PD,
    CMU_RC_RSTN,
    CMU_FAST_XO_LOOP_RSTN,
    CMU_TCXO2ANA_PD,
    CMU_XO2DBB_PD,
    CMU_CLK_XO2DBB_GT,
    CMU_TCXO_EN,
    CMU_A32M_CLKEN,
    CMU_B32M_CLKEN,
    CMU_RC2_PD,
    CMU_RC2_RSTN,
    CMU_RC2_DBB_PD,
    CMU_RC2_DBB_GT,
    CMU_RC2_CLKEN,
} hal_cmu_man_sel_t;

typedef enum {
    HAL_PMU_XLDO_LEVEL_1P5V     = 1,
    HAL_PMU_XLDO_LEVEL_1P525V,
    HAL_PMU_XLDO_LEVEL_1P55V,
    HAL_PMU_XLDO_LEVEL_1P575V,
    HAL_PMU_XLDO_LEVEL_1P6V,
    HAL_PMU_XLDO_LEVEL_1P625V,
    HAL_PMU_XLDO_LEVEL_1P65V    = 0,
    HAL_PMU_XLDO_LEVEL_1P675V   = 7,
    HAL_PMU_XLDO_LEVEL_1P7V,
    HAL_PMU_XLDO_LEVEL_1P725V,
    HAL_PMU_XLDO_LEVEL_1P75V,
    HAL_PMU_XLDO_LEVEL_1P775V,
    HAL_PMU_XLDO_LEVEL_1P8V,
    HAL_PMU_XLDO_LEVEL_1P825V,
    HAL_PMU_XLDO_LEVEL_1P85V,
    HAL_PMU_XLDO_LEVEL_1P875V,
} hal_pmu_xldo_level_type_t;

typedef enum {
    HAL_PMU_INTLDO_LEVEL_1P65V  = 7,
    HAL_PMU_INTLDO_LEVEL_1P675V = 6,
    HAL_PMU_INTLDO_LEVEL_1P7V   = 5,
    HAL_PMU_INTLDO_LEVEL_1P75V  = 0,
    HAL_PMU_INTLDO_LEVEL_1P775V = 1,
    HAL_PMU_INTLDO_LEVEL_1P8V   = 2,
    HAL_PMU_INTLDO_LEVEL_1P825V = 3,
    HAL_PMU_INTLDO_LEVEL_1P725V = 4,
} hal_pmu_intldo_level_type_t;

/**
 * @brief  PMU1 reset manual control config.
 * @param  power_switch    TURN_ON/TURN_OFF.
 */
void hal_pmu_reset_manual_config(switch_type_t power_switch);

/**
 * @brief  PMU1 reset control select.
 * @param  manual_switch   Manual reg config/reset signal.
 */
void hal_pmu_reset_manual_select(switch_type_t manual_switch);

/**
 * @brief  GET system current status.
 * @return System current status.
 */
hal_pmu_system_current_status_type_t hal_pmu_system_current_status(void);

/**
 * @brief  Get System power on count.
 * @return System boot count.
 */
uint16_t hal_pmu_system_boot_count(void);

/**
 * @brief  Config XLDO level voltage.
 * @param  level Config XLDO voltage.
 */
void hal_pmu_xldo_set_voltage(hal_pmu_xldo_level_type_t level);

/**
 * @brief  Control XLDO bypass mode enable/disable.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_xldo_enable_bypass(switch_type_t enable_bypass);

/**
 * @brief  Control XLDO level high_z enable/disable.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_xldo_enable_high_resistance(switch_type_t enable_high_z);

/**
 * @brief  Control XLDO ictr.
 * @param  ictr    Config XLDO ictr.
 */
void hal_pmu_xldo_set_ictr(uint8_t ictr);

/**
 * @brief  Config INTLDO level voltage.
 * @param  level Config INTLDO voltage.
 */
void hal_pmu_intldo_set_voltage(hal_pmu_intldo_level_type_t level);

/**
 * @brief  Control INTLDO bypass mode enable/disable.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_intldo_enable_bypass(switch_type_t enable_bypass);

/**
 * @brief  Control INTLDO level high_z enable/disable.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_intldo_enable_high_resistance(switch_type_t enable_high_z);

/**
 * @brief  Config XLDO delay time.
 * @param  delay_time LDO power on delay time.
 */
void hal_pmu_en_xldo_delay_time_config(uint16_t delay_time);

/**
 * @brief  Config CMU manual select value.
 * @param  cmu_sel_control Which CMU moudle.
 * @param  select_en       Manual value.
 */
void hal_cmu_manual_select_control(hal_cmu_man_sel_t cmu_sel_control, switch_type_t select_en);

/**
 * @brief  Config CMU manual select value.
 * @param  cmu_sel_set CMU manual select value.
 */
void hal_cmu_manual_select_set(uint16_t cmu_sel_set);

/**
 * @brief  Config CMU manual select value.
 * @param  manual_switch Set/Clear CMU manual select default value.
 */
void hal_cmu_manual_select(switch_type_t manual_switch);

/**
 * @brief  Config CMU manual or auto.
 * @param  cmu_man_control Which CMU moudle.
 * @param  manual_en       Manual or auto.
 */
void hal_cmu_manual_config_control(hal_cmu_man_sel_t cmu_man_control, switch_type_t manual_en);

/**
 * @brief  Config CMU manual or auto.
 * @param  cmu_man_set Set CMU manual value.
 */
void hal_cmu_manual_config_set(uint16_t cmu_man_set);

/**
 * @brief  Config CMU manual value.
 * @param  power_switch Set/Clear CMU manual default value.
 */
void hal_cmu_manual_config(switch_type_t power_switch);

/**
 * @brief  Config PMU manual select value.
 * @param  pmu_sel_control Config which PMU moudle.
 * @param  select_en       Manual value.
 */
void hal_pmu_manual_select_control(hal_pmu_man_sel_t pmu_sel_control, switch_type_t select_en);

/**
 * @brief  Config PMU manual select value.
 * @param  pmu_manual_select PMU manual select value.
 */
void hal_pmu_manual_select_set(uint16_t pmu_manual_select);

/**
 * @brief  Config PMU manual select value.
 * @param  manual_switch Set/Clear PMU manual select default value.
 */
void hal_pmu_manual_select(switch_type_t manual_switch);

/**
 * @brief  Config PMU manual or auto.
 * @param  pmu_man_control Which PMU moudle.
 * @param  manual_en       Manual or auto.
 */
void hal_pmu_manual_config_control(hal_pmu_man_sel_t pmu_man_control, switch_type_t manual_en);

/**
 * @brief  Config PMU manual value.
 * @param  pmu_manual_config PMU manual value.
 */
void hal_pmu_manual_config_set(uint16_t pmu_manual_config);

/**
 * @brief  Config PMU manual value.
 * @param  power_switch Set/Clear PMU manual default value.
 */
void hal_pmu_manual_config(switch_type_t power_switch);

/**
 * @brief  Config AON sleep ready manual select value.
 * @param  manual_switch Set/Clear PMU manual select default value.
 */
void hal_aon_sleep_ready_manual_select(switch_type_t manual_switch);

/**
 * @brief  Config AON sleep ready manual value.
 * @param  power_switch Set/Clear PMU manual default value.
 */
void hal_aon_sleep_ready_manual_config(switch_type_t power_switch);

/**
 * @brief  Config PMU2 manual value.
 * @param  power_config Power value.
 */
void hal_pmu2_power_set(uint8_t power_config);

/**
 * @brief  Config phyldo vset.
 * @param  vset phyldo_vset.
 */
void pmu_phyldo_vset_cfg(uint8_t vset);

/**
 * @brief  Get phyldo vset.
 * @return phyldo_vset.
 */
uint8_t pmu_phyldo_get_vset(void);

/**
 * @brief  Enable/Disable phyldo.
 * @param  en true: enable, false: disable.
 */
void pmu_phyldo_enable(bool en);

/**
 * @brief  Enable/Disable phyldo bypass.
 * @param  en true: enable, false: disable.
 */
void pmu_phyldo_enable_bypass(bool en);

/**
 * @brief  Config adldo1 vset.
 * @param  vset adldo1_vset.
 */
void pmu_adldo1_vset_cfg(uint8_t vset);

/**
 * @brief  Get adldo1 vset.
 * @return adldo1_vset.
 */
uint8_t pmu_adldo1_get_vset(void);

/**
 * @brief  Enable/Disable adldo1.
 * @param  en true: enable, false: disable.
 */
void pmu_adldo1_enable(bool en);

/**
 * @brief  Enable/Disable adldo1 bypass.
 * @param  en true: enable, false: disable.
 */
void pmu_adldo1_enable_bypass(bool en);

/**
 * @brief  Config adldo2 vset.
 * @param  vset adldo2_vset.
 */
void pmu_adldo2_vset_cfg(uint8_t vset);

/**
 * @brief  Get adldo2 vset.
 * @return adldo2_vset.
 */
uint8_t pmu_adldo2_get_vset(void);

/**
 * @brief  Enable/Disable adldo2.
 * @param  en true: enable, false: disable.
 */
void pmu_adldo2_enable(bool en);

/**
 * @brief  Enable/Disable adldo2 bypass.
 * @param  en true: enable, false: disable.
 */
void pmu_adldo2_enable_bypass(bool en);

/**
 * @brief  Enable/Disable tsensor_switch.
 * @param  en true: enable, false: disable.
 */
void pmu_tsensor_switch_enable(bool en);

/**
 * @brief  Enable/Disable switch1.
 * @param  en true: enable, false: disable.
 */
void pmu_switch1_enable(bool en);

/**
 * @brief  Enable/Disable efuse_switch.
 * @param  en true: enable, false: disable.
 */
void pmu_efuse_switch_enable(bool en);

/**
 * @brief  Enable/Disable ulp_clkldo.
 * @param  en true: enable, false: disable.
 */
void pmu_ulp_clkldo_enable(bool en);

/**
 * @brief  Config ulp_clkldo vset.
 * @param  vset ulp_clkldo vset.
 */
void pmu_ulp_clkldo_vset_cfg(uint8_t vset);

/**
 * @brief  Enable/Disable auxldo1.
 * @param  en true: enable, false: disable.
 */
void pmu_auxldo1_enable(bool en);

/**
 * @brief  Config auxldo1 vset.
 * @param  vset auxldo1_vset.
 */
void pmu_auxldo1_vset_cfg(uint8_t vset);

/**
 * @brief  Get auxldo1 vset.
 * @return auxldo1_vset.
 */
uint8_t pmu_auxldo1_get_vset(void);


/**
  * @}
  */

#endif
