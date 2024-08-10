/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description:  bs25 Memory Map configuration
 * Author: @CompanyNameTag
 * Create: 2023-03-25
 */

#include "non_os.h"
#include "chip_io.h"
#include "panic.h"
#include "hal_reg_config.h"
#include "hal_memory_apps.h"

#define SDIO_MEM_CTRL_REG           (M_CTL_RB_BASE + 0x12C)
#define SDIO_MEM_PWR_REG            (M_CTL_RB_BASE + 0x130)
#define EMMC_MEM_SLP_BIT            0
#define EMMC_MEM_DSLP_BIT           1
#define EMMC_MEM_SD_BIT             2
#define SD_H_MEM_SLP_BIT            3
#define SD_H_MEM_DSLP_BIT           4
#define SD_H_MEM_SD_BIT             5
#define SD_D_MEM_SLP_BIT            6
#define SD_D_MEM_DSLP_BIT           7
#define SD_D_MEM_SD_BIT             8
#define MEM_MODE_LEN                3

#define ITCM_SLP_POWER_MODE_BUCK0   (M_CTL_RB_BASE + 0x0300)
#define ITCM_SLP_POWER_MODE_BUCK1   (M_CTL_RB_BASE + 0x0304)
#define ITCM_SLP_POWER_MODE_BUCK2   (M_CTL_RB_BASE + 0x0308)
#define ITCM_SLP_POWER_MODE_BUCK3   (M_CTL_RB_BASE + 0x030C)
#define ITCM_SLP_POWER_MODE_BUCK4   (M_CTL_RB_BASE + 0x0310)
#define ITCM_SLP_POWER_MODE_BUCK5   (M_CTL_RB_BASE + 0x0314)
#define ITCM_SLP_POWER_MODE_BUCK6   (M_CTL_RB_BASE + 0x0318)
#define ITCM_SLP_POWER_MODE_BUCK7   (M_CTL_RB_BASE + 0x031C)

#define DTCM_SLP_POWER_MODE_BUCK0_1 (M_CTL_RB_BASE + 0x0320)
#define DTCM_SLP_POWER_MODE_BUCK2_3 (M_CTL_RB_BASE + 0x0324)
#define DTCM_SLP_POWER_MODE_BUCK4_5 (M_CTL_RB_BASE + 0x0328)
#define DTCM_SLP_POWER_MODE_BUCK6_7 (M_CTL_RB_BASE + 0x032C)

#define CACHE_SLP_POWER_MODE_REGION (M_CTL_RB_BASE + 0x0330)

#define ITCM_NOR_POWER_MODE_BUCK0   (M_CTL_RB_BASE + 0x0340)
#define ITCM_NOR_POWER_MODE_BUCK1   (M_CTL_RB_BASE + 0x0344)
#define ITCM_NOR_POWER_MODE_BUCK2   (M_CTL_RB_BASE + 0x0348)
#define ITCM_NOR_POWER_MODE_BUCK3   (M_CTL_RB_BASE + 0x034C)
#define ITCM_NOR_POWER_MODE_BUCK4   (M_CTL_RB_BASE + 0x0350)
#define ITCM_NOR_POWER_MODE_BUCK5   (M_CTL_RB_BASE + 0x0354)
#define ITCM_NOR_POWER_MODE_BUCK6   (M_CTL_RB_BASE + 0x0358)
#define ITCM_NOR_POWER_MODE_BUCK7   (M_CTL_RB_BASE + 0x035C)

#define DTCM_NOR_POWER_MODE_BUCK0_1 (M_CTL_RB_BASE + 0x0360)
#define DTCM_NOR_POWER_MODE_BUCK2_3 (M_CTL_RB_BASE + 0x0364)
#define DTCM_NOR_POWER_MODE_BUCK4_5 (M_CTL_RB_BASE + 0x0368)
#define DTCM_NOR_POWER_MODE_BUCK6_7 (M_CTL_RB_BASE + 0x036C)

#define CACHE_POWER_MODE_REGION     (M_CTL_RB_BASE + 0x0370)

#define ITCM_POWER_MODE_BUCK0_STS   (M_CTL_RB_BASE + 0x0380)
#define ITCM_POWER_MODE_BUCK1_STS   (M_CTL_RB_BASE + 0x0384)
#define ITCM_POWER_MODE_BUCK2_STS   (M_CTL_RB_BASE + 0x0388)
#define ITCM_POWER_MODE_BUCK3_STS   (M_CTL_RB_BASE + 0x038C)
#define ITCM_POWER_MODE_BUCK4_STS   (M_CTL_RB_BASE + 0x0390)
#define ITCM_POWER_MODE_BUCK5_STS   (M_CTL_RB_BASE + 0x0394)
#define ITCM_POWER_MODE_BUCK6_STS   (M_CTL_RB_BASE + 0x0398)
#define ITCM_POWER_MODE_BUCK7_STS   (M_CTL_RB_BASE + 0x039C)

#define DTCM_POWER_MODE_BUCK0_1_STS (M_CTL_RB_BASE + 0x03A0)
#define DTCM_POWER_MODE_BUCK2_3_STS (M_CTL_RB_BASE + 0x03A4)
#define DTCM_POWER_MODE_BUCK4_5_STS (M_CTL_RB_BASE + 0x03A8)
#define DTCM_POWER_MODE_BUCK6_7_STS (M_CTL_RB_BASE + 0x03AC)

#define CACHE_POWER_MODE_REGION_STS (M_CTL_RB_BASE + 0x03B0)

#define MCPU_ROM_POWER_CFG          (M_CTL_RB_BASE + 0x03C8)
#define MCPU_ROM_SD_BIT             0

#define GPU_MEMORY_POWER_CTRL       (M_CTL_RB_BASE + 0xC50)

#define COM_RAM_MEM_CFG             (COM_CTL_RB_BASE + 0x400)
#define COM_RAM_MEM_MODE_BIT        10
#define COM_RAM_MEM_MODE_LEN        3

#define COM_MEM_MODE_CTRL_BIT       10
#define COM_MEM_MODE_CTRL_LEN       3
#define COM_EMMC_SDIO_HOST_RAM_CRTL (COM_CTL_RB_BASE + 0x10)
#define COM_CODEC_ROM_CFG_REG       (COM_CTL_RB_BASE + 0x170)
#define COM_CODEC_MEM_CFG_REG       (COM_CTL_RB_BASE + 0x174)

#define XIP_MEM_POWER_CFG           (COM_CTL_RB_BASE + 0x410)
#define XIP_CACHE_POWER_MODE        0
#define XIP_DIAG_POWER_MODE         3
#define XIP_MEM_POWER_MODE_LEN      3

#define DISPLAY_MEM_SD_CFG          (COM_CTL_RB_BASE + 0x504)
#define DISPLAY_DSI_MEM_SD          0
#define DISPLAY_DSS_MEM_SD          1

/* Power control registers */
#define DSP_MEM_POWER_EN_CFG        (PMU1_CTL_RB_BASE + 0x3C)
#define DSP_MEM0_POWER_EN           0
#define DSP_MEM1_POWER_EN           1
#define DSP_MEM0_POWER_ACK          4
#define DSP_MEM1_POWER_ACK          5
#define DSP_MEM0_ISO_EN             8
#define DSP_MEM1_ISO_EN             9

#define MCU_HS_POWER_EN_CFG         (PMU1_CTL_RB_BASE + 0x40)
#define DSUB_POWER_EN_CFG           (PMU1_CTL_RB_BASE + 0x48)
#define ASUB_POWER_EN_CFG           (PMU1_CTL_RB_BASE + 0x54)
#define PMU_MEM_POWER_ENABLE        0
#define PMU_MEM_POWER_ACK           4
#define PMU_MEM_ISO_ENABLE          8

#define HAL_MEM_APP_TIMEOUT         1000

/*
 * Simply initialise the pointers in the runtime configuration array. As yet, do not configure memory maps!
 */
void hal_memory_init(void)
{
    non_os_enter_critical();
    // To be add memory init code
    non_os_exit_critical();
}

void hal_memory_itcm_set_sleep_power_mode(hal_memory_itcm_region_type_t region,
                                          hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    switch (region) {
        case HAL_MEMORY_ITCM_REGION_0:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK0, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_1:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK1, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_2:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK2, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_3:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK3, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_4:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK4, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_5:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK5, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_6:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK6, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_7:
            reg16_write(ITCM_SLP_POWER_MODE_BUCK7, (uint16_t)mode);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            break; //lint !e527  unreachable code
    }
}

void hal_memory_dtcm_set_sleep_power_mode(hal_memory_dtcm_region_type_t region, hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    switch (region) {
        case HAL_MEMORY_DTCM_REGION0_1:
            reg16_write(DTCM_SLP_POWER_MODE_BUCK0_1, (uint16_t)mode);
            break;
        case HAL_MEMORY_DTCM_REGION2_3:
            reg16_write(DTCM_SLP_POWER_MODE_BUCK2_3, (uint16_t)mode);
            break;
        case HAL_MEMORY_DTCM_REGION4_5:
            reg16_write(DTCM_SLP_POWER_MODE_BUCK4_5, (uint16_t)mode);
            break;
        case HAL_MEMORY_DTCM_REGION6_7:
            reg16_write(DTCM_SLP_POWER_MODE_BUCK6_7, (uint16_t)mode);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            break; //lint !e527  unreachable code
    }
}

void hal_memory_cache_set_sleep_power_mode(hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    reg16_write(CACHE_SLP_POWER_MODE_REGION, (uint16_t)mode);
}

void hal_memory_itcm_set_normal_power_mode(hal_memory_itcm_region_type_t region, hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    switch (region) {
        case HAL_MEMORY_ITCM_REGION_0:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK0, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_1:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK1, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_2:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK2, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_3:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK3, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_4:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK4, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_5:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK5, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_6:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK6, (uint16_t)mode);
            break;
        case HAL_MEMORY_ITCM_REGION_7:
            reg16_write(ITCM_NOR_POWER_MODE_BUCK7, (uint16_t)mode);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            break; //lint !e527  unreachable code
    }
}

void hal_memory_dtcm_set_normal_power_mode(hal_memory_dtcm_region_type_t region, hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    switch (region) {
        case HAL_MEMORY_DTCM_REGION0_1:
            reg16_write(DTCM_NOR_POWER_MODE_BUCK0_1, (uint16_t)mode);
            break;
        case HAL_MEMORY_DTCM_REGION2_3:
            reg16_write(DTCM_NOR_POWER_MODE_BUCK2_3, (uint16_t)mode);
            break;
        case HAL_MEMORY_DTCM_REGION4_5:
            reg16_write(DTCM_NOR_POWER_MODE_BUCK4_5, (uint16_t)mode);
            break;
        case HAL_MEMORY_DTCM_REGION6_7:
            reg16_write(DTCM_NOR_POWER_MODE_BUCK6_7, (uint16_t)mode);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            break; //lint !e527  unreachable code
    }
}

void hal_memory_cache_set_normal_power_mode(hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    reg16_write(CACHE_POWER_MODE_REGION, (uint16_t)mode);
}

hal_memory_power_mode_type_t hal_memory_itcm_get_power_mode(hal_memory_itcm_region_type_t region)
{
    hal_memory_power_mode_type_t power_id;
    switch (region) {
        case HAL_MEMORY_ITCM_REGION_0:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK0_STS);
            break;
        case HAL_MEMORY_ITCM_REGION_1:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK1_STS);
            break;
        case HAL_MEMORY_ITCM_REGION_2:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK2_STS);
            break;
        case HAL_MEMORY_ITCM_REGION_3:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK3_STS);
            break;
        case HAL_MEMORY_ITCM_REGION_4:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK4_STS);
            break;
        case HAL_MEMORY_ITCM_REGION_5:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK5_STS);
            break;
        case HAL_MEMORY_ITCM_REGION_6:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK6_STS);
            break;
        case HAL_MEMORY_ITCM_REGION_7:
            power_id = (hal_memory_power_mode_type_t)readw(ITCM_POWER_MODE_BUCK7_STS);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            return HAL_MEMORY_POWER_NONE;  //lint !e527  unreachable code
    }

    return power_id;
}

hal_memory_power_mode_type_t hal_memory_dtcm_get_power_mode(hal_memory_dtcm_region_type_t region)
{
    hal_memory_power_mode_type_t power_id;
    switch (region) {
        case HAL_MEMORY_DTCM_REGION0_1:
            power_id = (hal_memory_power_mode_type_t)readw(DTCM_POWER_MODE_BUCK0_1_STS);
            break;
        case HAL_MEMORY_DTCM_REGION2_3:
            power_id = (hal_memory_power_mode_type_t)readw(DTCM_POWER_MODE_BUCK2_3_STS);
            break;
        case HAL_MEMORY_DTCM_REGION4_5:
            power_id = (hal_memory_power_mode_type_t)readw(DTCM_POWER_MODE_BUCK4_5_STS);
            break;
        case HAL_MEMORY_DTCM_REGION6_7:
            power_id = (hal_memory_power_mode_type_t)readw(DTCM_POWER_MODE_BUCK6_7_STS);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            return HAL_MEMORY_POWER_NONE;  //lint !e527  unreachable code
    }

    return power_id;
}

hal_memory_power_mode_type_t hal_memory_cache_get_power_mode(void)
{
    hal_memory_power_mode_type_t power_id;
    power_id = (hal_memory_power_mode_type_t)readw(CACHE_POWER_MODE_REGION_STS);
    return power_id;
}

void hal_memory_rom_shut_down_control(switch_type_t power_control)
{
    non_os_enter_critical();
    hal_reg_config_bit(MCPU_ROM_POWER_CFG, power_control, (REG16_POS)MCPU_ROM_SD_BIT);
    non_os_exit_critical();
}

void hal_memory_gpu_set_power_mode(hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    non_os_enter_critical();
    reg16_write(GPU_MEMORY_POWER_CTRL, (uint16_t)mode);
    non_os_exit_critical();
}

void hal_common_ram_set_power_mode(hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    non_os_enter_critical();
    reg16_setbits(COM_RAM_MEM_CFG, COM_RAM_MEM_MODE_BIT, COM_RAM_MEM_MODE_LEN, mode);
    non_os_exit_critical();
}

void hal_sdio_memory_control(uint16_t mem_ctrl)
{
    non_os_enter_critical();
    writew(SDIO_MEM_CTRL_REG, mem_ctrl);
    non_os_exit_critical();
}

void hal_sdio_memory_power_control(hal_sdio_mem_t mem_t, hal_sdio_pwr_t mem_pwr_mode)
{
    uint8_t offset;
    switch (mem_t) {
        case HAL_EMMC_MEM:
            offset = EMMC_MEM_SLP_BIT;
            break;
        case HAL_SD_H_MEM:
            offset = SD_H_MEM_SLP_BIT;
            break;
        case HAL_SD_D_MEM:
            offset = SD_D_MEM_SLP_BIT;
            break;
        default:
            return;
    }

    reg16_setbits(SDIO_MEM_PWR_REG, offset, MEM_MODE_LEN, mem_pwr_mode);
}

void hal_emmc_sdio_host_ram_ctrl(uint8_t ram_ctrl)
{
    non_os_enter_critical();
    writew(COM_EMMC_SDIO_HOST_RAM_CRTL, ram_ctrl);
    non_os_exit_critical();
}

void hal_codec_mem_ctrl(hal_memory_power_mode_type_t mem_mode)
{
    non_os_enter_critical();
    reg16_setbits(COM_CODEC_MEM_CFG_REG, COM_MEM_MODE_CTRL_BIT, COM_MEM_MODE_CTRL_LEN, mem_mode);
    non_os_exit_critical();
}

void hal_memory_xip_set_power_mode(hal_memory_xip_region_type_t region,
                                   hal_memory_power_mode_type_t mode)
{
    uint16_t reg_offset = 0;
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    switch (region) {
        case HAL_MEMORY_XIP_CACHE_REGION:
            reg_offset = XIP_CACHE_POWER_MODE;
            break;
        case HAL_MEMORY_XIP_DIAG_REGION:
            reg_offset = XIP_DIAG_POWER_MODE;
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            break; //lint !e527  unreachable code
    }

    non_os_enter_critical();
    reg16_setbits(XIP_MEM_POWER_CFG, reg_offset, XIP_MEM_POWER_MODE_LEN, mode);
    non_os_exit_critical();
}

void hal_memory_dsi_shut_down_control(switch_type_t power_control)
{
    hal_reg_config_bit(DISPLAY_MEM_SD_CFG, power_control, (REG16_POS)DISPLAY_DSI_MEM_SD);
}

void hal_memory_dss_shut_down_control(switch_type_t power_control)
{
    hal_reg_config_bit(DISPLAY_MEM_SD_CFG, power_control, (REG16_POS)DISPLAY_DSS_MEM_SD);
}

static void hal_pmu_dsp_mem_iso_enable(hal_pmu_dsp_mem_type_t hal_dsp_mem, switch_type_t iso_switch)
{
    UNUSED(hal_dsp_mem);
    UNUSED(iso_switch);
    return;
}

static void hal_pmu_wait_dsp_mem_power_ack(hal_pmu_dsp_mem_type_t hal_dsp_mem)
{
    switch (hal_dsp_mem) {
        case HAL_PMU_DSP_MEM0:
            return;
        case HAL_PMU_DSP_MEM1:
            break;
        default:
            return;
    }
}

void hal_pmu_dsp_mem_power_enable(hal_pmu_dsp_mem_type_t hal_dsp_mem, switch_type_t power_switch)
{
    uint16_t offset;
    switch (hal_dsp_mem) {
        case HAL_PMU_DSP_MEM0:
            offset = DSP_MEM0_POWER_EN;
            break;
        case HAL_PMU_DSP_MEM1:
            offset = DSP_MEM1_POWER_EN;
            break;
        default:
            return;
    }

    non_os_enter_critical();
    if (power_switch) {
        hal_reg_config_bit(DSP_MEM_POWER_EN_CFG, power_switch, (REG16_POS)offset); // Power on
        // ASIC do not support ack, so just use the delay
        hal_pmu_wait_dsp_mem_power_ack(hal_dsp_mem); // Wait power on complete
        hal_pmu_dsp_mem_iso_enable(hal_dsp_mem, TURN_OFF); // Turn off dsp mem iso
    } else {
        hal_pmu_dsp_mem_iso_enable(hal_dsp_mem, TURN_ON); // Turn on dsp mem iso
        hal_reg_config_bit(DSP_MEM_POWER_EN_CFG, power_switch, (REG16_POS)offset); // Power off
    }
    non_os_exit_critical();
}

static void hal_pmu_mcu_hs_power_ack(void)
{
    hal_reg_status_check_timeout(MCU_HS_POWER_EN_CFG, PMU_MEM_POWER_ACK, TURN_ON, HAL_MEM_APP_TIMEOUT);
}

static void hal_pmu_mcu_hs_iso_enable(switch_type_t iso_switch)
{
    hal_reg_config_bit(MCU_HS_POWER_EN_CFG, iso_switch, (REG16_POS)PMU_MEM_ISO_ENABLE);
}

void hal_pmu_mcu_hs_power_enable(switch_type_t power_switch)
{
    non_os_enter_critical();
    if (power_switch) {
        hal_reg_config_bit(MCU_HS_POWER_EN_CFG, power_switch, (REG16_POS)PMU_MEM_POWER_ENABLE);
        hal_pmu_mcu_hs_power_ack();
        hal_pmu_mcu_hs_iso_enable(TURN_OFF);
    } else {
        hal_pmu_mcu_hs_iso_enable(TURN_ON);
        hal_reg_config_bit(MCU_HS_POWER_EN_CFG, power_switch, (REG16_POS)PMU_MEM_POWER_ENABLE);
    }
    non_os_exit_critical();
}

static void hal_pmu_dsub_power_ack(void)
{
    hal_reg_status_check_timeout(DSUB_POWER_EN_CFG, PMU_MEM_POWER_ACK, TURN_ON, HAL_MEM_APP_TIMEOUT);
}

static void hal_pmu_dsub_iso_enable(switch_type_t iso_switch)
{
    hal_reg_config_bit(DSUB_POWER_EN_CFG, iso_switch, (REG16_POS)PMU_MEM_ISO_ENABLE);
}

void hal_pmu_dsub_power_enable(switch_type_t power_switch)
{
    non_os_enter_critical();
    if (power_switch) {
        hal_reg_config_bit(DSUB_POWER_EN_CFG, power_switch, (REG16_POS)PMU_MEM_POWER_ENABLE);
        hal_pmu_dsub_power_ack();
        hal_pmu_dsub_iso_enable(TURN_OFF);
    } else {
        hal_pmu_dsub_iso_enable(TURN_ON);
        hal_reg_config_bit(DSUB_POWER_EN_CFG, power_switch, (REG16_POS)PMU_MEM_POWER_ENABLE);
    }
    non_os_exit_critical();
}

bool hal_pmu_get_dsub_power_status(void)
{
    return reg16_getbit(DSUB_POWER_EN_CFG, (REG16_POS)PMU_MEM_POWER_ENABLE);
}

static void hal_pmu_asub_power_ack(void)
{
    hal_reg_status_check_timeout(ASUB_POWER_EN_CFG, PMU_MEM_POWER_ACK, TURN_ON, HAL_MEM_APP_TIMEOUT);
}

static void hal_pmu_asub_iso_enable(switch_type_t iso_switch)
{
    hal_reg_config_bit(ASUB_POWER_EN_CFG, iso_switch, (REG16_POS)PMU_MEM_ISO_ENABLE);
}

void hal_pmu_asub_power_enable(switch_type_t power_switch)
{
    non_os_enter_critical();
    if (power_switch) {
        hal_reg_config_bit(ASUB_POWER_EN_CFG, power_switch, (REG16_POS)PMU_MEM_POWER_ENABLE);
        hal_pmu_asub_power_ack();
        hal_pmu_asub_iso_enable(TURN_OFF);
    } else {
        hal_pmu_asub_iso_enable(TURN_ON);
        hal_reg_config_bit(ASUB_POWER_EN_CFG, power_switch, (REG16_POS)PMU_MEM_POWER_ENABLE);
    }
    non_os_exit_critical();
}
