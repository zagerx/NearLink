/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: hal pmu bt sub header file. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-5-5, Create file. \n
 */

#ifndef SRC_DRIVERS_HAL_APPLICATION_HAL_PMU_BT_SUB_H
#define SRC_DRIVERS_HAL_APPLICATION_HAL_PMU_BT_SUB_H

/** @defgroup connectivity_drivers_hal_pmu_bt_sub PMU BT SUB
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */

#define CLB_CTL_M_SOFT_RST_2_REG                0x570000B8
#define SOFT_RET_GLB_B_CRG_N_BIT                0
#define SOFT_RET_GLB_B_LGC_N_BIT                1
#define SOFT_RET_GLB_B_CPU_N_BIT                2

#define PMU1_B_SOC_PWR_EN_CFG_REG               0x57004028
#define B_SOC_PWR_EN                            0
#define B_SOC_PWR_ACK                           4
#define B_SOC_ISO_EN                            8

#define PMU1_BMEM_PWR_EN_CFG_REG                0x57004034
#define BMEM_PWR_EN                             0
#define BMEM_PWR_ACK                            4
#define BMEM_ISO_EN                             8

#define M3_SRAM_ORIGIN_APP_MAPPING              0xA6000000
#define BT_VECTORS_LENGTH                       0x204

/**
 * @brief  Power on/off BT system.
 * @param  en true: enable, false: disable.
 */
void pmu_bt_onoff_ctrl(bool en);
/**
  * @}
  */

#endif
