/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pm pmu port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-13， Create file. \n
 */
#ifndef PM_PMU_PORTING_H
#define PM_PMU_PORTING_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_pm_pmu PM pmu
 * @ingroup  drivers_port_pm
 * @{
 */

typedef enum pmu_control_type {
    /* Buck simo. */
    PMU_CONTROL_BUCK_0P9_CTRL,              /* For buck configuration, see @ref pmu_buck_ctrl_t */
    PMU_CONTROL_BUCK_1P8_CTRL,              /* For buck configuration, see @ref pmu_buck_ctrl_t */
    PMU_CONTROL_BUCK_1P1_CTRL,              /* For buck configuration, see @ref pmu_buck_ctrl_t */
    PMU_CONTROL_BUCK_1P3_CTRL,              /* For buck configuration, see @ref pmu_buck_ctrl_t */

    /* LDO. */
    PMU_CONTROL_AUXLDO1_POWER,              /* For power configuration, see @ref pmu_control_power_t */
    PMU_CONTROL_PHYLDO_POWER,               /* For power configuration, see @ref pmu_control_power_t */
    PMU_CONTROL_ADLDO1_POWER,               /* For power configuration, see @ref pmu_control_power_t */
    PMU_CONTROL_ADLDO2_POWER,               /* For power configuration, see @ref pmu_control_power_t */
    PMU_CONTROL_ULP_CLKLDO_POWER,           /* For power configuration, see @ref pmu_control_power_t */
    PMU_CONTROL_EFUSE_SWITCH_POWER,         /* For power configuration, see @ref pmu_control_power_t */
    PMU_CONTROL_TSENSOR_SWITCH_POWER,       /* For power configuration, see @ref pmu_control_power_t */
    PMU_CONTROL_SWITCH1_POWER,              /* For power configuration, see @ref pmu_control_power_t */

    /* Sub system. */
    PMU_CONTROL_BT_POWER,                   /* For power configuration, see @ref pmu_control_power_t */

    /* Pmu watchdog reboot. */
    PMU_CONTROL_PMU_WDT_REBOOT,             /* Configuration parameters: None. */

    PMU_CONTROL_POWER_PULL,                 /* For power configuration, see @ref pmu_power_pull_t */

    PMU_CONTROL_TYPE_MAX,
} pmu_control_type_t;

typedef enum pmu_buck_ctrl {
    PMU_BUCK_POWER_OFF,
    PMU_BUCK_POWER_ON,
    PMU_BUCK_ENTER_ECO,
    PMU_BUCK_POWER_OFF_WHEN_SLEEPING,
    PMU_BUCK_ENTER_ECO_WHEN_SLEEPING,
    PMU_BUCK_SWITCH_TO_ECO_VSET_WHEN_SLEEPING,
    PMU_BUCK_INTO_NORMAL_STATE_WHEN_SLEEPING,
} pmu_buck_ctrl_t;

typedef enum pmu_control_power {
    PMU_CONTROL_POWER_OFF,
    PMU_CONTROL_POWER_ON,
} pmu_control_power_t;

typedef enum pmu_power_pull {
    PMU_POWER_PULL_UP,
    PMU_POWER_PULL_NONE,
} pmu_power_pull_t;

typedef enum pmu_ldo_id {
    PMU_LDO_ID_BUCK_0P9,        /* For voltage configuration, see @ref pmu_buck_0p9_vset_t */
    PMU_LDO_ID_BUCK_1P8,        /* For voltage configuration, see @ref pmu_buck_1p8_vset_t */
    PMU_LDO_ID_BUCK_1P1,        /* For voltage configuration, see @ref pmu_buck_1p3_vset_t */
    PMU_LDO_ID_BUCK_1P3,        /* For voltage configuration, see @ref pmu_buck_1p3_vset_t */
    PMU_LDO_ID_BUCK_0P9_ECO,    /* For voltage configuration, see @ref pmu_buck_0p9_vset_t */
    PMU_LDO_ID_BUCK_1P8_ECO,    /* For voltage configuration, see @ref pmu_buck_1p8_vset_t */
    PMU_LDO_ID_BUCK_1P1_ECO,    /* For voltage configuration, see @ref pmu_buck_1p3_vset_t */
    PMU_LDO_ID_BUCK_1P3_ECO,    /* For voltage configuration, see @ref pmu_buck_1p3_vset_t */
    PMU_LDO_ID_AUXLDO1,         /* For voltage configuration, see @ref pmu_auxldo1_vset_t */
    PMU_LDO_ID_PHYLDO,          /* For voltage configuration, see @ref pmu_phyldo_vset_t */
    PMU_LDO_ID_ADLDO1,          /* For voltage configuration, see @ref pmu_adldo1_vset_t */
    PMU_LDO_ID_ADLDO2,          /* For voltage configuration, see @ref pmu_adldo2_vset_t */
    PMU_LDO_ID_ULP_CLKLDO,      /* For voltage configuration, see @ref pmu_clkldo_vset_t */
} pmu_ldo_id_t;

typedef enum pmu_buck_0p9_vset {
    PMU_BUCK_0P9_VSET_0V6 = 0x0,
    PMU_BUCK_0P9_VSET_0V7 = 0x5,
    PMU_BUCK_0P9_VSET_0V8 = 0xA,
    PMU_BUCK_0P9_VSET_0V9 = 0xF,
    PMU_BUCK_0P9_VSET_1V0 = 0x14,
    PMU_BUCK_0P9_VSET_1V1 = 0x19,
    PMU_BUCK_0P9_VSET_1V2 = 0x1E,
    PMU_BUCK_0P9_VSET_1V3 = 0x23,
    PMU_BUCK_0P9_VSET_1V4 = 0x28,
} pmu_buck_0p9_vset_t;

typedef enum pmu_buck_1p8_vset {
    PMU_BUCK_1P8_VSET_1V0 = 0x0,
    PMU_BUCK_1P8_VSET_1V1 = 0x5,
    PMU_BUCK_1P8_VSET_1V2 = 0xA,
    PMU_BUCK_1P8_VSET_1V3 = 0xF,
    PMU_BUCK_1P8_VSET_1V4 = 0x14,
    PMU_BUCK_1P8_VSET_1V5 = 0x19,
    PMU_BUCK_1P8_VSET_1V6 = 0x1E,
    PMU_BUCK_1P8_VSET_1V7 = 0x23,
    PMU_BUCK_1P8_VSET_1V8 = 0x28,
    PMU_BUCK_1P8_VSET_1V9 = 0x2D,
    PMU_BUCK_1P8_VSET_2V0 = 0x32,
} pmu_buck_1p8_vset_t;

typedef enum pmu_buck_1p1_vset {
    PMU_BUCK_1P1_VSET_0V6 = 0x0,
    PMU_BUCK_1P1_VSET_0V7 = 0x5,
    PMU_BUCK_1P1_VSET_0V8 = 0xA,
    PMU_BUCK_1P1_VSET_0V9 = 0xF,
    PMU_BUCK_1P1_VSET_1V0 = 0x14,
    PMU_BUCK_1P1_VSET_1V025 = 0x15,
    PMU_BUCK_1P1_VSET_1V1 = 0x19,
    PMU_BUCK_1P1_VSET_1V2 = 0x1E,
    PMU_BUCK_1P1_VSET_1V3 = 0x23,
    PMU_BUCK_1P1_VSET_1V4 = 0x28,
} pmu_buck_1p1_vset_t;

typedef enum pmu_buck_1p3_vset {
    PMU_BUCK_1P3_VSET_1V0  = 0x0,
    PMU_BUCK_1P3_VSET_1V1  = 0x5,
    PMU_BUCK_1P3_VSET_1V18 = 0x9,
    PMU_BUCK_1P3_VSET_1V2  = 0xA,
    PMU_BUCK_1P3_VSET_1V3  = 0xF,
    PMU_BUCK_1P3_VSET_1V4  = 0x14,
    PMU_BUCK_1P3_VSET_1V5  = 0x19,
    PMU_BUCK_1P3_VSET_1V6  = 0x1E,
    PMU_BUCK_1P3_VSET_1V7  = 0x23,
    PMU_BUCK_1P3_VSET_1V8  = 0x28,
    PMU_BUCK_1P3_VSET_1V9  = 0x2D,
    PMU_BUCK_1P3_VSET_2V0  = 0x32,
} pmu_buck_1p3_vset_t;

typedef enum pmu_auxldo1_vset {
    PMU_AUXLDO1_VSET_2V4  = 0x0,
    PMU_AUXLDO1_VSET_2V5  = 0x2,
    PMU_AUXLDO1_VSET_2V6  = 0x4,
    PMU_AUXLDO1_VSET_2V7  = 0x6,
    PMU_AUXLDO1_VSET_2V75 = 0x7,
    PMU_AUXLDO1_VSET_2V85 = 0x8,
    PMU_AUXLDO1_VSET_2V9  = 0x9,
    PMU_AUXLDO1_VSET_2V95 = 0xA,
    PMU_AUXLDO1_VSET_3V0  = 0xB,
    PMU_AUXLDO1_VSET_3V05 = 0xC,
    PMU_AUXLDO1_VSET_3V1  = 0xD,
    PMU_AUXLDO1_VSET_3V2  = 0xE,
    PMU_AUXLDO1_VSET_3V3  = 0xF,
} pmu_auxldo1_vset_t;

typedef enum pmu_phyldo_vset {
    PMU_PHYLDO_VSET_2V4  = 0x0,
    PMU_PHYLDO_VSET_2V5  = 0x2,
    PMU_PHYLDO_VSET_2V6  = 0x4,
    PMU_PHYLDO_VSET_2V7  = 0x6,
    PMU_PHYLDO_VSET_2V75 = 0x7,
    PMU_PHYLDO_VSET_2V85 = 0x8,
    PMU_PHYLDO_VSET_2V9  = 0x9,
    PMU_PHYLDO_VSET_2V95 = 0xA,
    PMU_PHYLDO_VSET_3V0  = 0xB,
    PMU_PHYLDO_VSET_3V05 = 0xC,
    PMU_PHYLDO_VSET_3V1  = 0xD,
    PMU_PHYLDO_VSET_3V2  = 0xE,
    PMU_PHYLDO_VSET_3V3  = 0xF,
} pmu_phyldo_vset_t;

typedef enum pmu_adldo1_vset {
    PMU_ADLDO1_VSET_2V4  = 0x0,
    PMU_ADLDO1_VSET_2V5  = 0x2,
    PMU_ADLDO1_VSET_2V6  = 0x4,
    PMU_ADLDO1_VSET_2V7  = 0x6,
    PMU_ADLDO1_VSET_2V75 = 0x7,
    PMU_ADLDO1_VSET_2V85 = 0x8,
    PMU_ADLDO1_VSET_2V9  = 0x9,
    PMU_ADLDO1_VSET_2V95 = 0xA,
    PMU_ADLDO1_VSET_3V0  = 0xB,
    PMU_ADLDO1_VSET_3V05 = 0xC,
    PMU_ADLDO1_VSET_3V1  = 0xD,
    PMU_ADLDO1_VSET_3V2  = 0xE,
    PMU_ADLDO1_VSET_3V3  = 0xF,
} pmu_adldo1_vset_t;

typedef enum pmu_adldo2_vset {
    PMU_ADLDO2_VSET_2V4  = 0x0,
    PMU_ADLDO2_VSET_2V5  = 0x2,
    PMU_ADLDO2_VSET_2V6  = 0x4,
    PMU_ADLDO2_VSET_2V7  = 0x6,
    PMU_ADLDO2_VSET_2V75 = 0x7,
    PMU_ADLDO2_VSET_2V85 = 0x8,
    PMU_ADLDO2_VSET_2V9  = 0x9,
    PMU_ADLDO2_VSET_2V95 = 0xA,
    PMU_ADLDO2_VSET_3V0  = 0xB,
    PMU_ADLDO2_VSET_3V05 = 0xC,
    PMU_ADLDO2_VSET_3V1  = 0xD,
    PMU_ADLDO2_VSET_3V2  = 0xE,
    PMU_ADLDO2_VSET_3V3  = 0xF,
} pmu_adldo2_vset_t;

typedef enum pmu_clkldo_vset {
    PMU_CLKLDO_VSET_1V1  = 0x0,
    PMU_CLKLDO_VSET_0V98 = 0x1,
    PMU_CLKLDO_VSET_1V02 = 0x2,
    PMU_CLKLDO_VSET_1V06 = 0x3,
    PMU_CLKLDO_VSET_1V14 = 0x4,
    PMU_CLKLDO_VSET_1V18 = 0x5,
    PMU_CLKLDO_VSET_1V22 = 0x6,
    PMU_CLKLDO_VSET_1V3  = 0x7,
} pmu_clkldo_vset_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif