/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pm pmu port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-13， Create file. \n
 */

#include "chip_io.h"
#include "pmu_ldo.h"
#include "pmu_buck_ldo.h"
#include "pmu_bt_sub.h"
#include "pm_pmu.h"
#include "pm_pmu_porting.h"

typedef bool (*pmu_control_func_t)(uint8_t param);

static bool pmu_control_buck_ctrl(pmu_control_type_t type, pmu_buck_ctrl_t ctrl)
{
    switch (ctrl) {
        case PMU_BUCK_POWER_ON:
            pmu_buck_ldo_set_state((pmu_buck_ldo_t)type, PMU_BUCK_LDO_STATE_ON);
            break;

        case PMU_BUCK_POWER_OFF:
            pmu_buck_ldo_set_state((pmu_buck_ldo_t)type, PMU_BUCK_LDO_STATE_OFF);
            break;

        case PMU_BUCK_ENTER_ECO:
            pmu_buck_ldo_sleep_onoff_ctrl((pmu_buck_ldo_t)type, false);
            pmu_buck_ldo_set_state((pmu_buck_ldo_t)type, PMU_BUCK_LDO_STATE_ECO);
            break;

        case PMU_BUCK_POWER_OFF_WHEN_SLEEPING:
            pmu_buck_ldo_sleep_onoff_ctrl((pmu_buck_ldo_t)type, true);
            break;

        case PMU_BUCK_ENTER_ECO_WHEN_SLEEPING:
            pmu_buck_ldo_sleep_eco_ctrl((pmu_buck_ldo_t)type, true);
            break;

        case PMU_BUCK_SWITCH_TO_ECO_VSET_WHEN_SLEEPING:
            pmu_buck_ldo_sleep_vset_ctrl((pmu_buck_ldo_t)type, true);
            break;

        case PMU_BUCK_INTO_NORMAL_STATE_WHEN_SLEEPING:
            pmu_buck_ldo_sleep_onoff_ctrl((pmu_buck_ldo_t)type, false);
            pmu_buck_ldo_sleep_eco_ctrl((pmu_buck_ldo_t)type, false);
            pmu_buck_ldo_sleep_vset_ctrl((pmu_buck_ldo_t)type, false);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_auxldo1_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_auxldo1_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_phyldo_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_phyldo_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_adldo1_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_adldo1_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_adldo2_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_adldo2_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_ulp_clkldo_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_ulp_clkldo_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_efuse_switch_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_efuse_switch_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_tsensor_switch_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_tsensor_switch_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_switch1_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_switch1_enable((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_bt_power(pmu_control_power_t power)
{
    switch (power) {
        case PMU_CONTROL_POWER_OFF:
        case PMU_CONTROL_POWER_ON:
            pmu_bt_onoff_ctrl((bool)power);
            break;

        default:
            return false;
    }
    return true;
}

static bool pmu_control_pmu_wdt_reboot(uint8_t param)
{
    unused(param);
    pmu_trigger_pmu_wdt_reboot();
    return true;
}

static bool pmu_control_power_pull(pmu_power_pull_t pull_type)
{
    // 控制长按按键重启功能，默认使能（上拉：10uA@1.8v漏电），pull_none该功能失效。
    reg16_setbits(0x5702C5CC, 0x4, 0x1, (uint8_t)pull_type);
    return true;
}

static const pmu_control_func_t g_pmu_control_funcs[] = {
    pmu_control_auxldo1_power,
    pmu_control_phyldo_power,
    pmu_control_adldo1_power,
    pmu_control_adldo2_power,
    pmu_control_ulp_clkldo_power,
    pmu_control_efuse_switch_power,
    pmu_control_tsensor_switch_power,
    pmu_control_switch1_power,
    pmu_control_bt_power,
    pmu_control_pmu_wdt_reboot,
    pmu_control_power_pull,
};

errcode_t uapi_pmu_control(pmu_control_type_t type, uint8_t param)
{
    if (type >= PMU_CONTROL_TYPE_MAX) {
        return ERRCODE_INVALID_PARAM;
    }

    bool ret = false;
    if (type <= PMU_CONTROL_BUCK_1P3_CTRL) {
        ret = pmu_control_buck_ctrl(type, param);
    } else {
        ret = g_pmu_control_funcs[((uint8_t)type - PMU_CONTROL_AUXLDO1_POWER)](param);
    }

    if (ret) {
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}

errcode_t uapi_pmu_ldo_set_voltage(pmu_ldo_id_t id, uint8_t vset)
{
    switch (id) {
        case PMU_LDO_ID_BUCK_0P9:
        case PMU_LDO_ID_BUCK_1P8:
        case PMU_LDO_ID_BUCK_1P1:
        case PMU_LDO_ID_BUCK_1P3:
            pmu_buck_ldo_vset_cfg((pmu_buck_ldo_t)id, vset);
            break;

        case PMU_LDO_ID_BUCK_0P9_ECO:
        case PMU_LDO_ID_BUCK_1P8_ECO:
        case PMU_LDO_ID_BUCK_1P1_ECO:
        case PMU_LDO_ID_BUCK_1P3_ECO:
            pmu_buck_ldo_eco_vset_cfg((pmu_buck_ldo_t)((uint32_t)id - PMU_LDO_ID_BUCK_0P9_ECO), vset);
            break;

        case PMU_LDO_ID_AUXLDO1:
            pmu_auxldo1_vset_cfg(vset);
            break;

        case PMU_LDO_ID_PHYLDO:
            pmu_phyldo_vset_cfg(vset);
            break;

        case PMU_LDO_ID_ADLDO1:
            pmu_adldo1_vset_cfg(vset);
            break;

        case PMU_LDO_ID_ADLDO2:
            pmu_adldo2_vset_cfg(vset);
            break;

        case PMU_LDO_ID_ULP_CLKLDO:
            pmu_ulp_clkldo_vset_cfg(vset);
            break;

        default:
            return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

uint8_t uapi_pmu_ldo_get_voltage(pmu_ldo_id_t id)
{
    switch (id) {
        case PMU_LDO_ID_BUCK_0P9:
        case PMU_LDO_ID_BUCK_1P8:
        case PMU_LDO_ID_BUCK_1P1:
        case PMU_LDO_ID_BUCK_1P3:
            return pmu_buck_ldo_get_vset((pmu_buck_ldo_t)id);

        case PMU_LDO_ID_BUCK_0P9_ECO:
        case PMU_LDO_ID_BUCK_1P8_ECO:
        case PMU_LDO_ID_BUCK_1P1_ECO:
        case PMU_LDO_ID_BUCK_1P3_ECO:
            return pmu_buck_ldo_get_eco_vset((pmu_buck_ldo_t)((uint32_t)id - PMU_LDO_ID_BUCK_0P9_ECO));

        case PMU_LDO_ID_AUXLDO1:
            return pmu_auxldo1_get_vset();

        case PMU_LDO_ID_PHYLDO:
            return pmu_phyldo_get_vset();

        case PMU_LDO_ID_ADLDO1:
            return pmu_adldo1_get_vset();

        case PMU_LDO_ID_ADLDO2:
            return pmu_adldo2_get_vset();

        default:
            return 0;
    }
}