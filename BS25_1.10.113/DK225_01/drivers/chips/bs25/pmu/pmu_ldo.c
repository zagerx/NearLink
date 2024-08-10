/*
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description:  PMU DRIVER
 * Author: @CompanyNameTag
 * Create: 2019-12-19
 */

#include "non_os.h"
#include "hal_reg_config.h"
#include "tcxo.h"
#include "pmu_ldo.h"

typedef union pmu_ldo1_cfg_data {
    uint32_t u32;                       /*!< Raw register data. */
    struct {
        uint32_t vset       : 4;
        uint32_t ictr       : 3;
        uint32_t reserved   : 1;
        uint32_t sel        : 2;
    } b;                                /*!< Register bits. */
} pmu_ldo1_cfg_data_t;

typedef union pmu_ldo1_en_data {
    uint32_t u32;                       /*!< Raw register data. */
    struct {
        uint32_t en         : 1;
        uint32_t en_delay   : 1;
        uint32_t ocp_set    : 1;
        uint32_t ocp_bypass : 1;
        uint32_t en_hiz     : 1;
        uint32_t en_sw      : 1;
    } b;                                /*!< Register bits. */
} pmu_ldo1_en_data_t;

typedef struct pmu_ldo1_ctrl_regs {
    pmu_ldo1_cfg_data_t cfg;
    pmu_ldo1_en_data_t en;
} pmu_ldo1_ctrl_regs_t;

typedef struct {
    uint32_t reg_addr;
    uint16_t value;
} reg_cfg_t;

#define PMU_SYS_BOOT_CNT_MASK 0x3FFF

/* SYS_STATUS registers */
#define PMU_SYS_BOOT_CNT_REG                    (PMU1_CTL_RB_BASE + 0x0020)
#define PMU_SYS_STATUS_REG                      (PMU1_CTL_RB_BASE + 0x0024)
#define PMU_SYS_CUR_STATUS_BIT                  0
#define PMU_SYS_CUR_STATUS_LEN                  3
#define PMU_SYS_CUR_POWER_SYNC_STS              4

#define PMU_AUXLDO_CFG_REG                      (PMU1_CTL_RB_BASE + 0x90)
#define PMU_AUXLDO_VSET_BIT                     0
#define PMU_AUXLDO_VSET_LEN                     4
#define PMU_AUXLDO_ICTR_BIT                     4
#define PMU_AUXLDO_ICTR_LEN                     3
#define PMU_AUXLDO_EN_REG                       (PMU1_CTL_RB_BASE + 0xCC)
#define PMU_AUXLDO_EN_BIT                       0
#define PMU_EN_AUXLDO_DELAY_BIT                 1
#define PMU_EN_AUXLDO_HIZ_BIT                   4
#define PMU_EN_AUXLDO_SW_BIT                    5

#define PMU_CLDO1_CFG_REG                       (PMU1_CTL_RB_BASE + 0x8C)
#define PMU_CLDO1_ICTR_BIT                      4
#define PMU_CLDO1_ICTR_LEN                      3
#define PMU_CLDO1_EN_REG                        (PMU1_CTL_RB_BASE + 0xAC)
#define PMU_EN_CLDO1_HIZ_BIT                    4

#define PMU_IOLDO_CFG_REG                       (PMU1_CTL_RB_BASE + 0x94)
#define PMU_IOLDO_VSET_BIT                      0
#define PMU_IOLDO_VSET_LEN                      4
#define PMU_IOLDO_ICTR_BIT                      4
#define PMU_IOLDO_ICTR_LEN                      2
#define PMU_IOLDO_OCP_BYPASS                    10
#define PMU_IOLDO_OCP_SET                       11
#define PMU_IOLDO_LRSET_BIT                     12
#define PMU_IOLDO_LRSET_LEN                     2
#define PMU_IOLDO_EN_REG                        (PMU1_CTL_RB_BASE + 0xB4)
#define PMU_EN_IOLDO_HIZ_BIT                    4
#define PMU_EN_IOLDO_SW_BIT                     5

#define PMU_MIPILDO_CFG_REG                     (PMU1_CTL_RB_BASE + 0x98)
#define PMU_MIPILDO_VSET_BIT                    0
#define PMU_MIPILDO_VSET_LEN                    4
#define PMU_MIPILDO_ICTR_BIT                    4
#define PMU_MIPILDO_ICTR_LEN                    3
#define PMU_MIPILDO_EN_REG                      (PMU1_CTL_RB_BASE + 0xB8)
#define PMU_MIPILDO_EN_BIT                      0
#define PMU_EN_MIPILDO_DELAY_BIT                1
#define PMU_EN_MIPILDO_HIZ_BIT                  4
#define PMU_EN_MIPILDO_SW_BIT                   5

#define PMU_LOAD_SWITCH_REG                     (PMU1_CTL_RB_BASE + 0xBC)
#define PMU_EN_LOAD_SWITCH1_BIT                 0
#define PMU_EN_LOAD_SWITCH2_BIT                 1
#define PMU_EN_LOAD_SWITCH3_BIT                 2
#define PMU_EN_LOAD_SWITCH4_BIT                 3

/* PMU2 control register */
#define PMU_PMU2_MANUAL_CTRL_REG                (PMU1_CTL_RB_BASE + 0xD0)
#define PMU_EN_REF_BG2_BIT                      0
#define PMU_EN_REF_BUFFER_BIT                   1
#define PMU_EN_INTLDO2_BIT                      2
#define PMU_EN_INTLDO2_DELAY_BIT                3
#define PMU_EN_IBG2_BIT                         4
#define PMU_EN_IPOLY_BIT                        5
#define PMU_EN_ITUNE_BIT                        6

/* PMU0 RESET MANUAL & SLE Module */
#define PMU_MAN_0_REG                           (PMU1_CTL_RB_BASE + 0xD4)
#define PMU_SEL_0_REG                           (PMU1_CTL_RB_BASE + 0xE8)
#define PMU_STS_0_REG                           (PMU1_CTL_RB_BASE + 0xFC)
#define PMU_AON_SLP_RDY_BIT                     0

/* PMU1 RESET MANUAL & SLE Module */
#define PMU_MAN_1_REG                           (PMU1_CTL_RB_BASE + 0xD8)
#define PMU_SEL_1_REG                           (PMU1_CTL_RB_BASE + 0xEC)
#define PMU_STS_1_REG                           (PMU1_CTL_RB_BASE + 0x100)
#define PMU_POWER_CTRL_CONFIG                   0x7FF
#define PMU_SEL_EN_IBG1_BIT                     0
#define PMU_SEL_IOLDO_EN_BIT                    1
#define PMU_SEL_IOLDO_EN_DELAY_BIT              2
#define PMU_SEL_EN_CLDO_BIT                     3
#define PMU_SEL_EN_CLDO_DELAY_BIT               4
#define PMU_SEL_PMU_CMU_REF_VBG_BIT             5
#define PMU_SEL_PMU_CMU_REF_IBG_BIT             6
#define PMU_SEL_EN_XLDO_BIT                     7
#define PMU_SEL_EN_XLDO_DELAY_BIT               8

/* LDO RESET MANUAL & SLE Module */
#define PMU_LDO_MAN_REG                         (PMU1_CTL_RB_BASE + 0xDC)
#define PMU_LDO_SEL_REG                         (PMU1_CTL_RB_BASE + 0xF0)
#define PMU_LDO_STS_REG                         (PMU1_CTL_RB_BASE + 0x104)
#define PMU_LDO_C1_TO_SYS_ISO_EN_BIT            0
#define PMU_LDO_EN_CLDO1_SW_BIT                 1
#define PMU_LDO_CLDO1_VSET_WKUP_BIT             2
#define PMU_LDO_CTRL_CONFIG                     (BIT(PMU_LDO_C1_TO_SYS_ISO_EN_BIT) | \
                                                 BIT(PMU_LDO_EN_CLDO1_SW_BIT) | \
                                                 BIT(PMU_LDO_CLDO1_VSET_WKUP_BIT))   // Need check

/* CMU RESET MANUAL & SLE Module */
#define PMU_CMU_MAN_REG                         (PMU1_CTL_RB_BASE + 0xE0)
#define PMU_CMU_SEL_REG                         (PMU1_CTL_RB_BASE + 0xF4)
#define PMU_CMU_STS_REG                         (PMU1_CTL_RB_BASE + 0x108)
#define PMU_CMU_CTRL_CONFIG                     0xFFFF
#define PMU_CMU_XO_CORE_PD_BIT                  0
#define PMU_CMU_FAST_XO_ISO_BIT                 1
#define PMU_CMU_RC_PD_BIT                       2
#define PMU_CMU_RC_RSTN_BIT                     3
#define PMU_CMU_FAST_XO_LOOP_RSTN_BIT           4
#define PMU_CMU_TCXO2ANA_PD_BIT                 5
#define PMU_CMU_XO2DBB_PD_BIT                   6
#define PMU_CMU_CLK_XO2DBB_GT_BIT               7
#define PMU_CMU_TCXO_EN_BIT                     8

/* PMU1 RESET MANUAL & SLE Module */
#define PMU_RST_MAN_REG                         (PMU1_CTL_RB_BASE + 0xE4)
#define PMU_RST_SEL_REG                         (PMU1_CTL_RB_BASE + 0xF8)
#define PMU_RST_MAN_RST_PWR_C1_CPU_N_BIT        0
#define PMU_RST_MAN_RST_PWR_C1_LOGIC_N_BIT      1
#define PMU_RST_MAN_RST_PWR_C1_CRG_N_BIT        2
#define PMU_RST_MAN_GLB_CLK_FORCE_ON_BIT        4
#define PMU_RST_MAN_CONFIG                      (BIT(PMU_RST_MAN_RST_PWR_C1_CPU_N_BIT) | \
                                                 BIT(PMU_RST_MAN_RST_PWR_C1_LOGIC_N_BIT) | \
                                                 BIT(PMU_RST_MAN_RST_PWR_C1_CRG_N_BIT))

#define PMU_RST_STS_REG                         (PMU1_CTL_RB_BASE + 0x10C)
#define PMU_RST_PWR_C1_CPU_N_BIT                0
#define PMU_RST_PWR_C1_LOGIC_N_BIT              1
#define PMU_RST_PWR_C1_CRG_N_BIT                2
#define PMU_RST_AON_LOGIC_N_STS_BIT             3
#define PMU_RST_GLB_CLK_FORCE_ON_STS_BIT        4
#define PMU_RST_RST_AON_CRG_N_STS_BIT           5

/* System wakeup time config register */
#define PMU_AON_SLP_RDY_WKUP_TIME_SOC_REG       (PMU1_CTL_RB_BASE + 0x110)
#define PMU_EN_IBG1_WKUP_TIME_SOC_REG           (PMU1_CTL_RB_BASE + 0x11C)

#define PMU_EN_XLDO_DELAY_TIME_SOC_REG          (PMU1_CTL_RB_BASE + 0x1B0)
#define PMU_EN_CLDO1_DELAY_TIME_SOC_REG         (PMU1_CTL_RB_BASE + 0x1B4)
#define PMU_IOLDO_EN_DELAY_TIME_SOC_REG         (PMU1_CTL_RB_BASE + 0x1B8)

/* System sleep time config register */
#define PMU_AON_SLP_RDY_SLP_TIME_SOC_REG        (PMU1_CTL_RB_BASE + 0x2A0)
#define PMU_C1_TO_SYS_ISO_EN_SLP_TIME_REG       (PMU1_CTL_RB_BASE + 0x2BC)
#define PMU_CMU_REF_VBG_SLP_TIME_REG            (PMU1_CTL_RB_BASE + 0x2C0)
#define PMU_RST_REG_ADDR_OFFSET                 4

#define PMU2_LDO_EN_CTL_REG                     (PMU2_CMU_CTL_RB_BASE + 0x110)
#define PMU2_PALDO2_0P9_EN_BIT                  0
#define PMU2_PALDO2_0P9_EN_DELAY_BIT            1
#define PMU2_PALDO2_1P8_EN_BIT                  4
#define PMU2_PALDO2_1P8_EEN_DELAY_BIT           5

#define PMU2_XLDO_CFG_REG                       (PMU2_CMU_CTL_RB_BASE + 0x12C)
#define PMU2_XLDO_SEL_BIT                       0
#define PMU2_XLDO_VSET_BIT                      2
#define PMU2_XLDO_VSET_LEN                      4
#define PMU2_XLDO_ICTR_BIT                      6
#define PMU2_XLDO_ICTR_LEN                      3
#define PMU2_EN_XLDO_SW_BIT                     9
#define PMU2_EN_XLDO_HIZ_BIT                    10

#define PMU2_INTLDO2_CFG_REG                    (PMU2_CMU_CTL_RB_BASE + 0x130)
#define PMU2_INTLDO2_HIZ_BIT                    1
#define PMU2_INTLDO2_VSET_BIT                   2
#define PMU2_INTLDO2_VSET_LEN                   3
#define PMU2_INTLDO2_ICTR_BIT                   5
#define PMU2_INTLDO2_ICTR_LEN                   3
#define PMU2_EN_INTLDO2_SW_BIT                  8

#define PMU_EN_SWITCH_REG           0x5702C250
#define PMU_TSENSOR_SWITCH_EN_BIT   0
#define PMU_SWITCH1_EN_BIT          2
#define PMU_EFUSE_EN_SWITCH_REG     0x5702C254
#define PMU_EFUSE_SWITCH_ON_VAL     0xA5A5
#define PMU_EFUSE_SWITCH_OFF_VAL    0

#define PMU_PHYLDO_CFG_REG          0x5702C220
#define PMU_ADLDO1_CFG_REG          0x5702C228
#define PMU_ADLDO2_CFG_REG          0x5702C230

#define PMU_LDO1_ENABLE_DELAY_US    30UL

#define PMU_ULP_CLKLDO_CFG_REG      0x5702C24C
#define PMU_ULP_CLKLDO_VSET_BIT     0
#define PMU_ULP_CLKLDO_VSET_LEN     4
#define PMU_ULP_CLKLDO_EN_BIT       12
#define PMU_ULP_CLKLDO_EN_DELAY_BIT 13

#define PMU_D_CORE_RB_AUXLDO1_EN_REG    (PMU_D_CORE_RB_ADDR + 0x50)
#define PMU_D_CORE_RB_AUXLDO1_EN_VAL    0x3
#define PMU_D_CORE_RB_AUXLDO1_VSET_REG  (PMU_D_CORE_RB_ADDR + 0xA8)

uint16_t hal_pmu_system_boot_count(void)
{
    uint16_t sys_reboot_cnt = 0;

    reg16_read(PMU_SYS_BOOT_CNT_REG, sys_reboot_cnt);
    return (PMU_SYS_BOOT_CNT_MASK & sys_reboot_cnt);
}

hal_pmu_system_current_status_type_t hal_pmu_system_current_status(void)
{
    return (hal_pmu_system_current_status_type_t)reg16_getbits(PMU_SYS_STATUS_REG,
                                                               PMU_SYS_CUR_STATUS_BIT, PMU_SYS_CUR_STATUS_LEN);
}

/*********************** PMU0 config ****************************/
void hal_aon_sleep_ready_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        reg16_setbit(PMU_MAN_0_REG, PMU_AON_SLP_RDY_BIT);
    } else {
        reg16_clrbit(PMU_MAN_0_REG, PMU_AON_SLP_RDY_BIT);
    }
}

void hal_aon_sleep_ready_manual_select(switch_type_t manual_switch)
{
    if (manual_switch != 0) {
        reg16_setbit(PMU_SEL_0_REG, PMU_AON_SLP_RDY_BIT);
    } else {
        reg16_clrbit(PMU_SEL_0_REG, PMU_AON_SLP_RDY_BIT);
    }
}

/*********************** PMU1 config ****************************/
void hal_pmu_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        writew(PMU_MAN_1_REG, PMU_POWER_CTRL_CONFIG);
    } else {
        writew(PMU_MAN_1_REG, 0);
    }
}

void hal_pmu_manual_config_set(uint16_t pmu_manual_config)
{
    writew(PMU_MAN_1_REG, pmu_manual_config);
}

void hal_pmu_manual_config_control(hal_pmu_man_sel_t pmu_man_control, switch_type_t manual_en)
{
    // Enable or disable pmu manual.
    hal_reg_config_bit(PMU_MAN_1_REG, manual_en, (REG16_POS)pmu_man_control);
}

void hal_pmu_manual_select(switch_type_t manual_switch)
{
    if (manual_switch != 0) {
        writew(PMU_SEL_1_REG, PMU_POWER_CTRL_CONFIG);
    } else {
        writew(PMU_SEL_1_REG, 0);
    }
}

void hal_pmu_manual_select_set(uint16_t pmu_manual_select)
{
    writew(PMU_SEL_1_REG, pmu_manual_select);
}

void hal_pmu_manual_select_control(hal_pmu_man_sel_t pmu_sel_control, switch_type_t select_en)
{
    // Enable or disable pmu manual select.
    hal_reg_config_bit(PMU_SEL_1_REG, select_en, (REG16_POS)pmu_sel_control);
}

/*********************** CMU config ****************************/
void hal_cmu_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        writew(PMU_CMU_MAN_REG, PMU_CMU_CTRL_CONFIG);
    } else {
        writew(PMU_CMU_MAN_REG, 0);
    }
}

void hal_cmu_manual_config_set(uint16_t cmu_man_set)
{
    writew(PMU_CMU_MAN_REG, cmu_man_set);
}

void hal_cmu_manual_config_control(hal_cmu_man_sel_t cmu_man_control, switch_type_t manual_en)
{
    // Enable or disable cmu manual.
    hal_reg_config_bit(PMU_CMU_MAN_REG, manual_en, (REG16_POS)cmu_man_control);
}

void hal_cmu_manual_select(switch_type_t manual_switch)
{
    if (manual_switch != 0) {
        writew(PMU_CMU_SEL_REG, PMU_CMU_CTRL_CONFIG);
    } else {
        writew(PMU_CMU_SEL_REG, 0);
    }
}

void hal_cmu_manual_select_set(uint16_t cmu_sel_set)
{
    writew(PMU_CMU_SEL_REG, cmu_sel_set);
}

void hal_cmu_manual_select_control(hal_cmu_man_sel_t cmu_sel_control, switch_type_t select_en)
{
    // Enable or disable cmu select.
    hal_reg_config_bit(PMU_CMU_SEL_REG, select_en, (REG16_POS)cmu_sel_control);
}

/*********************** RESET config ****************************/
void hal_pmu_reset_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        writew(PMU_RST_MAN_REG, PMU_RST_MAN_CONFIG);
    } else {
        writew(PMU_RST_MAN_REG, 0);
    }
}

/* XLDO control */
void hal_pmu_xldo_set_voltage(hal_pmu_xldo_level_type_t level)
{
    reg16_setbits(PMU2_XLDO_CFG_REG, PMU2_XLDO_VSET_BIT, PMU2_XLDO_VSET_LEN, level);
}

void hal_pmu_xldo_enable_bypass(switch_type_t enable_bypass)
{
    hal_reg_config_bit(PMU2_XLDO_CFG_REG, enable_bypass, (REG16_POS)PMU2_EN_XLDO_SW_BIT);
}

void hal_pmu_xldo_enable_high_resistance(switch_type_t enable_high_z)
{
    if (enable_high_z != 0) {
        reg16_setbit(PMU2_XLDO_CFG_REG, PMU2_EN_XLDO_HIZ_BIT);
    } else {
        reg16_clrbit(PMU2_XLDO_CFG_REG, PMU2_EN_XLDO_HIZ_BIT);
    }
}

void hal_pmu_xldo_set_ictr(uint8_t ictr)
{
    reg16_setbits(PMU2_XLDO_CFG_REG, PMU2_XLDO_ICTR_BIT, PMU2_XLDO_ICTR_LEN, ictr);
}

/* INTLDO control */
void hal_pmu_intldo_set_voltage(hal_pmu_intldo_level_type_t level)
{
    reg16_setbits(PMU2_INTLDO2_CFG_REG, PMU2_INTLDO2_VSET_BIT, PMU2_INTLDO2_VSET_LEN, level);
}

void hal_pmu_intldo_enable_bypass(switch_type_t enable_bypass)
{
    hal_reg_config_bit(PMU2_INTLDO2_CFG_REG, enable_bypass, (REG16_POS)PMU2_EN_INTLDO2_SW_BIT);
}

void hal_pmu_intldo_enable_high_resistance(switch_type_t enable_high_z)
{
    if (enable_high_z == TURN_ON) {
        reg16_setbit(PMU2_INTLDO2_CFG_REG, PMU2_INTLDO2_HIZ_BIT);
    } else {
        reg16_clrbit(PMU2_INTLDO2_CFG_REG, PMU2_INTLDO2_HIZ_BIT);
    }
}

static inline void hal_pmu_ldo1_cfg_reg_vset(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->cfg.b.vset = val;
}

static inline uint8_t hal_pmu_ldo1_cfg_reg_vset_get(pmu_ldo1_ctrl_regs_t *regs)
{
    return regs->cfg.b.vset;
}

static inline void hal_pmu_ldo1_cfg_reg_ictr(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->cfg.b.ictr = val;
}

static inline void hal_pmu_ldo1_en_reg_en(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->en.b.en = val;
}

static inline void hal_pmu_ldo1_en_reg_en_delay(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->en.b.en_delay = val;
}

static inline void hal_pmu_ldo1_en_reg_ocp_set(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->en.b.ocp_set = val;
}

static inline void hal_pmu_ldo1_en_reg_ocp_bypass(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->en.b.ocp_bypass = val;
}

static inline void hal_pmu_ldo1_en_reg_hiz(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->en.b.en_hiz = val;
}

static inline void hal_pmu_ldo1_en_reg_sw(pmu_ldo1_ctrl_regs_t *regs, uint8_t val)
{
    regs->en.b.en_sw = val;
}

void pmu_phyldo_vset_cfg(uint8_t vset)
{
    hal_pmu_ldo1_cfg_reg_vset((pmu_ldo1_ctrl_regs_t *)PMU_PHYLDO_CFG_REG, vset);
}

uint8_t pmu_phyldo_get_vset(void)
{
    return hal_pmu_ldo1_cfg_reg_vset_get((pmu_ldo1_ctrl_regs_t *)PMU_PHYLDO_CFG_REG);
}

void pmu_phyldo_enable(bool en)
{
    hal_pmu_ldo1_en_reg_en((pmu_ldo1_ctrl_regs_t *)PMU_PHYLDO_CFG_REG, (uint8_t)en);
    if (en) {
        uapi_tcxo_delay_us(PMU_LDO1_ENABLE_DELAY_US);
    }
    hal_pmu_ldo1_en_reg_en_delay((pmu_ldo1_ctrl_regs_t *)PMU_PHYLDO_CFG_REG, (uint8_t)en);
}

void pmu_phyldo_enable_bypass(bool en)
{
    hal_pmu_ldo1_en_reg_sw((pmu_ldo1_ctrl_regs_t *)PMU_PHYLDO_CFG_REG, (uint8_t)en);
}

void pmu_adldo1_vset_cfg(uint8_t vset)
{
    hal_pmu_ldo1_cfg_reg_vset((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO1_CFG_REG, vset);
}

uint8_t pmu_adldo1_get_vset(void)
{
    return hal_pmu_ldo1_cfg_reg_vset_get((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO1_CFG_REG);
}

void pmu_adldo1_enable(bool en)
{
    hal_pmu_ldo1_en_reg_en((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO1_CFG_REG, (uint8_t)en);
    if (en) {
        uapi_tcxo_delay_us(PMU_LDO1_ENABLE_DELAY_US);
    }
    hal_pmu_ldo1_en_reg_en_delay((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO1_CFG_REG, (uint8_t)en);
}

void pmu_adldo1_enable_bypass(bool en)
{
    hal_pmu_ldo1_en_reg_sw((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO1_CFG_REG, (uint8_t)en);
}

void pmu_adldo2_vset_cfg(uint8_t vset)
{
    hal_pmu_ldo1_cfg_reg_vset((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO2_CFG_REG, vset);
}

uint8_t pmu_adldo2_get_vset(void)
{
    return hal_pmu_ldo1_cfg_reg_vset_get((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO2_CFG_REG);
}

void pmu_adldo2_enable(bool en)
{
    hal_pmu_ldo1_en_reg_en((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO2_CFG_REG, (uint8_t)en);
    if (en) {
        uapi_tcxo_delay_us(PMU_LDO1_ENABLE_DELAY_US);
    }
    hal_pmu_ldo1_en_reg_en_delay((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO2_CFG_REG, (uint8_t)en);
}

void pmu_adldo2_enable_bypass(bool en)
{
    hal_pmu_ldo1_en_reg_sw((pmu_ldo1_ctrl_regs_t *)PMU_ADLDO2_CFG_REG, (uint8_t)en);
}

void pmu_tsensor_switch_enable(bool en)
{
    if (en) {
        reg16_setbit(PMU_EN_SWITCH_REG, PMU_TSENSOR_SWITCH_EN_BIT);
    } else {
        reg16_clrbit(PMU_EN_SWITCH_REG, PMU_TSENSOR_SWITCH_EN_BIT);
    }
}

void pmu_switch1_enable(bool en)
{
    if (en) {
        reg16_setbit(PMU_EN_SWITCH_REG, PMU_SWITCH1_EN_BIT);
    } else {
        reg16_clrbit(PMU_EN_SWITCH_REG, PMU_SWITCH1_EN_BIT);
    }
}

void pmu_efuse_switch_enable(bool en)
{
    if (en) {
        writew(PMU_EFUSE_EN_SWITCH_REG, PMU_EFUSE_SWITCH_ON_VAL);
    } else {
        writew(PMU_EFUSE_EN_SWITCH_REG, PMU_EFUSE_SWITCH_OFF_VAL);
    }
}

void pmu_ulp_clkldo_enable(bool en)
{
    if (en) {
        reg16_setbit(PMU_ULP_CLKLDO_CFG_REG, PMU_ULP_CLKLDO_EN_BIT);
        uapi_tcxo_delay_us(PMU_LDO1_ENABLE_DELAY_US);
        reg16_setbit(PMU_ULP_CLKLDO_CFG_REG, PMU_ULP_CLKLDO_EN_DELAY_BIT);
    } else {
        reg16_clrbit(PMU_ULP_CLKLDO_CFG_REG, PMU_ULP_CLKLDO_EN_BIT);
        reg16_clrbit(PMU_ULP_CLKLDO_CFG_REG, PMU_ULP_CLKLDO_EN_DELAY_BIT);
    }
}

void pmu_ulp_clkldo_vset_cfg(uint8_t vset)
{
    reg16_setbits(PMU_ULP_CLKLDO_CFG_REG, PMU_ULP_CLKLDO_VSET_BIT, PMU_ULP_CLKLDO_VSET_LEN, vset);
}

void pmu_auxldo1_enable(bool en)
{
    if (en) {
        writew(PMU_D_CORE_RB_AUXLDO1_EN_REG, PMU_D_CORE_RB_AUXLDO1_EN_VAL);
    } else {
        writew(PMU_D_CORE_RB_AUXLDO1_EN_REG, 0);
    }
}

void pmu_auxldo1_vset_cfg(uint8_t vset)
{
    writeb(PMU_D_CORE_RB_AUXLDO1_VSET_REG, vset);
}

uint8_t pmu_auxldo1_get_vset(void)
{
    return readb(PMU_D_CORE_RB_AUXLDO1_VSET_REG);
}