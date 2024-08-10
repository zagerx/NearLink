/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Pmu buck ldo src file. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-1-28, Create file. \n
 */

#include "common_def.h"
#include "platform_core.h"
#include "pmu_buck_ldo.h"

#define PMU_BUCK_LDO_0P9_STATE_REG              0x5702E03C
#define PMU_BUCK_LDO_1P8_STATE_REG              0x5702E040
#define PMU_BUCK_LDO_1P1_STATE_REG              0x5702E044
#define PMU_BUCK_LDO_1P3_STATE_REG              0x5702E048

#define PMU_BUCK_LDO_0P9_VSET_REG               0x5702E080
#define PMU_BUCK_LDO_1P8_VSET_REG               0x5702E088
#define PMU_BUCK_LDO_1P1_VSET_REG               0x5702E090
#define PMU_BUCK_LDO_1P3_VSET_REG               0x5702E098

#define PMU_BUCK_LDO_0P9_ECO_VSET_REG           0x5702E084
#define PMU_BUCK_LDO_1P8_ECO_VSET_REG           0x5702E08C
#define PMU_BUCK_LDO_1P1_ECO_VSET_REG           0x5702E094
#define PMU_BUCK_LDO_1P3_ECO_VSET_REG           0x5702E09C

#define PMU_BUCK_LDO_SLEEP_VSET_CTRL            0x5702E11C
#define SLEEP_VSET_CTRL_B2_1P3_BIT              3
#define SLEEP_VSET_CTRL_B2_1P1_BIT              4
#define SLEEP_VSET_CTRL_B1_1P8_BIT              5
#define SLEEP_VSET_CTRL_B1_0P9_BIT              6

#define PMU_BUCK_LDO_SLEEP_ONOFF_CTRL           0x5702E120
#define SLEEP_ONOFF_CTRL_B2_1P3_BIT             2
#define SLEEP_ONOFF_CTRL_B2_1P1_BIT             3
#define SLEEP_ONOFF_CTRL_B1_0P9_BIT             4

#define PMU_BUCK_LDO_SLEEP_ECO_CTRL             0x5702E124
#define SLEEP_ECO_CTRL_BIT                      3

#define PMU_D_CORE_WDT_EN_REG                   (PMU_D_CORE_RB_ADDR + 0x18C)
#define PMU_D_CORE_WDT_CTRL_REG                 (PMU_D_CORE_RB_ADDR + 0x194)
#define PMU_D_CORE_WDT_TEST_LEVEL               0x10

static const uintptr_t g_buck_ldo_state_regs[PMU_BUCK_LDO_MAX] = {
    PMU_BUCK_LDO_0P9_STATE_REG,
    PMU_BUCK_LDO_1P8_STATE_REG,
    PMU_BUCK_LDO_1P1_STATE_REG,
    PMU_BUCK_LDO_1P3_STATE_REG
};

static const uint32_t g_buck_ldo_vset_regs[PMU_BUCK_LDO_MAX] = {
    PMU_BUCK_LDO_0P9_VSET_REG,
    PMU_BUCK_LDO_1P8_VSET_REG,
    PMU_BUCK_LDO_1P1_VSET_REG,
    PMU_BUCK_LDO_1P3_VSET_REG
};

static const uint32_t g_buck_ldo_eco_vset_regs[PMU_BUCK_LDO_MAX] = {
    PMU_BUCK_LDO_0P9_ECO_VSET_REG,
    PMU_BUCK_LDO_1P8_ECO_VSET_REG,
    PMU_BUCK_LDO_1P1_ECO_VSET_REG,
    PMU_BUCK_LDO_1P3_ECO_VSET_REG
};

typedef union pmu_buck_ldo_state_ctrl {
    uint32_t u32;                       /*!< Raw register data. */
    struct {
        uint32_t pwr_en     : 1;        /*!< Power enable. */
        uint32_t pwr_sts    : 1;        /*!< Power status. */
        uint32_t reserved   : 2;        /*!< Reserved. */
        uint32_t eco_en     : 1;        /*!< Eco enable. */
        uint32_t eco_sts    : 1;        /*!< Eco status. */
    } b;                                /*!< Register bits. */
} pmu_buck_ldo_state_ctrl_t;

void pmu_buck_ldo_set_state(pmu_buck_ldo_t ldo, pmu_buck_ldo_state_t state)
{
    pmu_buck_ldo_state_ctrl_t *p_state = (pmu_buck_ldo_state_ctrl_t *)g_buck_ldo_state_regs[ldo];
    switch (state) {
        case PMU_BUCK_LDO_STATE_OFF:
            p_state->u32 = 0;
            break;

        case PMU_BUCK_LDO_STATE_ON:
            p_state->b.eco_en = 0;
            p_state->b.pwr_en = 1;
            break;

        case PMU_BUCK_LDO_STATE_ECO:
            p_state->b.eco_en = 1;
            p_state->b.pwr_en = 1;
            break;
    }
    return;
}

pmu_buck_ldo_state_t pmu_buck_ldo_get_status(pmu_buck_ldo_t ldo)
{
    pmu_buck_ldo_state_ctrl_t *p_state = (pmu_buck_ldo_state_ctrl_t *)g_buck_ldo_state_regs[ldo];
    if (p_state->b.pwr_sts == 0) {
        return PMU_BUCK_LDO_STATE_OFF;
    } else if (p_state->b.eco_sts == 0) {
        return PMU_BUCK_LDO_STATE_ON;
    } else {
        return PMU_BUCK_LDO_STATE_ECO;
    }
}

void pmu_buck_ldo_vset_cfg(pmu_buck_ldo_t ldo, uint8_t vset)
{
    writeb(g_buck_ldo_vset_regs[ldo], vset);
}

uint8_t pmu_buck_ldo_get_vset(pmu_buck_ldo_t ldo)
{
    return readb(g_buck_ldo_vset_regs[ldo]);
}

void pmu_buck_ldo_eco_vset_cfg(pmu_buck_ldo_t ldo, uint8_t vset)
{
    writeb(g_buck_ldo_eco_vset_regs[ldo], vset);
}

uint8_t pmu_buck_ldo_get_eco_vset(pmu_buck_ldo_t ldo)
{
    return readb(g_buck_ldo_eco_vset_regs[ldo]);
}

void pmu_buck_ldo_sleep_vset_ctrl(pmu_buck_ldo_t ldo, bool en)
{
    uint8_t bit = 0;
    switch (ldo) {
        case PMU_BUCK_LDO_0P9:
            bit = SLEEP_VSET_CTRL_B1_0P9_BIT;
            break;

        case PMU_BUCK_LDO_1P8:
            bit = SLEEP_VSET_CTRL_B1_1P8_BIT;
            break;

        case PMU_BUCK_LDO_1P1:
            bit = SLEEP_VSET_CTRL_B2_1P1_BIT;
            break;

        case PMU_BUCK_LDO_1P3:
            bit = SLEEP_VSET_CTRL_B2_1P3_BIT;
            break;
    }
    if (en) {
        reg32_setbit(PMU_BUCK_LDO_SLEEP_VSET_CTRL, bit);
    } else {
        reg32_clrbit(PMU_BUCK_LDO_SLEEP_VSET_CTRL, bit);
    }
}


void pmu_buck_ldo_sleep_onoff_ctrl(pmu_buck_ldo_t ldo, bool en)
{
    uint8_t bit;
    switch (ldo) {
        case PMU_BUCK_LDO_0P9:
            bit = SLEEP_ONOFF_CTRL_B1_0P9_BIT;
            break;

        case PMU_BUCK_LDO_1P1:
            bit = SLEEP_ONOFF_CTRL_B2_1P1_BIT;
            break;

        case PMU_BUCK_LDO_1P3:
            bit = SLEEP_ONOFF_CTRL_B2_1P3_BIT;
            break;

        default:
            return;
    }
    if (en) {
        reg32_setbit(PMU_BUCK_LDO_SLEEP_ONOFF_CTRL, bit);
    } else {
        reg32_clrbit(PMU_BUCK_LDO_SLEEP_ONOFF_CTRL, bit);
    }
}

void pmu_buck_ldo_sleep_eco_ctrl(pmu_buck_ldo_t ldo, bool en)
{
    unused(ldo);
    if (en) {
        reg32_setbit(PMU_BUCK_LDO_SLEEP_ECO_CTRL, SLEEP_ECO_CTRL_BIT);
    } else {
        reg32_clrbit(PMU_BUCK_LDO_SLEEP_ECO_CTRL, SLEEP_ECO_CTRL_BIT);
    }
}

void pmu_trigger_pmu_wdt_reboot(void)
{
    writew(PMU_D_CORE_WDT_EN_REG, 0);   // Disable.
    writew(PMU_D_CORE_WDT_CTRL_REG, PMU_D_CORE_WDT_TEST_LEVEL);  // Watchdog time: 1ms.
    writew(PMU_D_CORE_WDT_EN_REG, 1);   // Enable.
}