/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides V150 HAL eflash \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-08， Create file. \n
 */

#include "common_def.h"
#include "hal_eflash.h"
#include "hal_eflash_v150.h"

#define EMBED_FLASH_PREPG_OFFSET                1
#define EMBED_FLASH_FIRST_BYTE_MASK             0xFF
#define EMBED_FLASH_FIRST_2_BYTE_MASK           0xFFFF
#define EMBED_FLASH_FIRST_BIT_MASK              1U
#define EMBED_FLASH_4_BYTES_SHIFT               4
#define EMBED_FLASH_SHIFT_2                     2
#define EMBED_FLASH_SHIFT_8                     8

#define EMBED_FLASH_TIME_REG_32M                0x54
#define EMBED_FLASH_TIMING_TNVS_32M             0x43
#define EMBED_FLASH_TIMING_TERASE_32M           0x4779
#define EMBED_FLASH_TIMING_TRCV_32M             0x0668
#define EMBED_FLASH_TIMING_TSCE_32M             0x4779
#define EMBED_FLASH_TIMING_TRCV_CHIP_32M        0x199C
#define EMBED_FLASH_TIMING_TPGS_32M             0x43
#define EMBED_FLASH_TIMING_TPROG_32M            0x48
#define EMBED_FLASH_TIMING_TRCV_PROG_32M        0x54
#define EMBED_FLASH_TIMING_TCFL_32M             0x54
#define EMBED_FLASH_ERASE_NEXT_RW_32M           0x3
#define EMBED_FLASH_PROG_NEXT_RW_32M            0x3
#define EMBED_FLASH_RECALL_TIME_32M             0x7
#define EMBED_FLASH_LVCTL_TIME_32M              0x1
#define EMBED_FLASH_FREQUENCY_SEL_32M           0x0
#define EMBED_FLASH_TERASE_VER_32M              0x1C20

#define EMBED_FLASH_TIME_REG_48M                0x7D
#define EMBED_FLASH_TIMING_TNVS_48M             0x64
#define EMBED_FLASH_TIMING_TERASE_48M           0x6B35
#define EMBED_FLASH_TIMING_TRCV_48M             0x099C
#define EMBED_FLASH_TIMING_TSCE_48M             0x6B35
#define EMBED_FLASH_TIMING_TRCV_CHIP_48M        0x266A
#define EMBED_FLASH_TIMING_TPGS_48M             0x64
#define EMBED_FLASH_TIMING_TPROG_48M            0x6C
#define EMBED_FLASH_TIMING_TRCV_PROG_48M        0x7E
#define EMBED_FLASH_TIMING_TCFL_48M             0x7E
#define EMBED_FLASH_ERASE_NEXT_RW_48M           0x5
#define EMBED_FLASH_PROG_NEXT_RW_48M            0x5
#define EMBED_FLASH_RECALL_TIME_48M             0xA
#define EMBED_FLASH_LVCTL_TIME_48M              0x3
#define EMBED_FLASH_FREQUENCY_SEL_48M           0x0
#define EMBED_FLASH_TERASE_VER_48M              0x2A30

#define EMBED_FLASH_TIME_REG_64M                0xA8
#define EMBED_FLASH_TIMING_TNVS_64M             0x86
#define EMBED_FLASH_TIMING_TERASE_64M           0x8EF2
#define EMBED_FLASH_TIMING_TRCV_64M             0x0CD0
#define EMBED_FLASH_TIMING_TSCE_64M             0x8EF2
#define EMBED_FLASH_TIMING_TRCV_CHIP_64M        0x3338
#define EMBED_FLASH_TIMING_TPGS_64M             0x86
#define EMBED_FLASH_TIMING_TPROG_64M            0x90
#define EMBED_FLASH_TIMING_TRCV_PROG_64M        0xA8
#define EMBED_FLASH_TIMING_TCFL_64M             0xA8
#define EMBED_FLASH_ERASE_NEXT_RW_64M           0x6
#define EMBED_FLASH_PROG_NEXT_RW_64M            0x6
#define EMBED_FLASH_RECALL_TIME_64M             0xF
#define EMBED_FLASH_LVCTL_TIME_64M              0x3
#define EMBED_FLASH_FREQUENCY_SEL_64M           0x2
#define EMBED_FLASH_TERASE_VER_64M              0x3840

#define EMBED_FLASH_TIME_REG_96M                0xFA
#define EMBED_FLASH_TIMING_TNVS_96M             0xC8
#define EMBED_FLASH_TIMING_TERASE_96M           0xD66B
#define EMBED_FLASH_TIMING_TRCV_96M             0x1338
#define EMBED_FLASH_TIMING_TSCE_96M             0xD66B
#define EMBED_FLASH_TIMING_TRCV_CHIP_96M        0x4CD4
#define EMBED_FLASH_TIMING_TPGS_96M             0xC8
#define EMBED_FLASH_TIMING_TPROG_96M            0xD8
#define EMBED_FLASH_TIMING_TRCV_PROG_96M        0xFA
#define EMBED_FLASH_TIMING_TCFL_96M             0xFA
#define EMBED_FLASH_ERASE_NEXT_RW_96M           0x9
#define EMBED_FLASH_PROG_NEXT_RW_96M            0x9
#define EMBED_FLASH_RECALL_TIME_96M             0x13
#define EMBED_FLASH_LVCTL_TIME_96M              0x5
#define EMBED_FLASH_FREQUENCY_SEL_96M           0x2
#define EMBED_FLASH_TERASE_VER_96M              0x5460

#define EMBED_FLASH_RR_NUM                      2
#define EMBED_FLASH_CBD_OFFSET                  16
#define EMBED_FLASH_CBD_NUM                     8
#define EMBED_FLASH_8_BYTES_SHIFT               8

#define EMBED_FLASH_CFG_1_OFFSET               0x04
#define EMBED_FLASH_CFG_2_OFFSET               0x08
#define EMBED_FLASH_CFG_3_OFFSET               0x0C
#define EMBED_FLASH_CFG_4_OFFSET               0x10
#define EMBED_FLASH_CFG_5_OFFSET               0x14

#define EMBED_FLASH_ERASE_VERIFY_EN             0x1
#define EMBED_FLASH_VERIFY_ADDR_STEP            0x1
#define EMBED_FLASH_VERIFY_ADDR_TIMES           0x1FF
#define EMBED_FLASH_ERAVER_ERASE_TIMES          0xA

#define EMBED_FLASH_TIMING_TPROG_PREPROGRAM_32M  0x20
#define EMBED_FLASH_TIMING_TPROG_PREPROGRAM_48M  0x30
#define EMBED_FLASH_TIMING_TPROG_PREPROGRAM_64M  0x40
#define EMBED_FLASH_TIMING_TPROG_PREPROGRAM_96M  0x60

static errcode_t hal_eflash_v150_init(eflash_region_t bus)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    if (hal_eflash_regs_init(bus) != ERRCODE_SUCC) {
        return ERRCODE_EFLASH_REG_ADDR_INVALID;
    }

    /* xip_cache_enable */
    hal_xip_cache_en_set_xip_cache_en(1);
    hal_man_all_set_man_all_req(1);
    while (hal_man_all_get_man_all_done() != 1) {}

    /* 1.xip_cache_ctl */
    hal_stb_eflash_mux_set_cfg_stb_eflash0_test_en(0);
    hal_stb_eflash_mux_set_cfg_stb_eflash1_test_en(0);
    /* 2.DPD, PORb */
    hal_eflash_por_dpd_sel_set_cfg_eflash0_por_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash0_dpd_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash1_por_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash1_dpd_dbg_sel(1);
    hal_eflash_por_dpd_man_set_cfg_eflash0_por_dbg(1);
    hal_eflash_por_dpd_man_set_cfg_eflash0_dpd_dbg(0);
    hal_eflash_por_dpd_man_set_cfg_eflash1_por_dbg(1);
    hal_eflash_por_dpd_man_set_cfg_eflash1_dpd_dbg(0);
    /* 3.xip_cache_ctl_rb */
    hal_cfg_eflash_write_read_en_0_set_cfg_eflash_write_read_en_0(bus, 1);
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
    return ERRCODE_SUCC;
}

static void hal_eflash_v150_deinit(eflash_region_t bus)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    /* De-enabled */
    hal_xip_cache_en_set_xip_cache_en(0);
    hal_eflash_regs_deinit(bus);
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
}

static void hal_eflash_v150_set_clock_32m(eflash_region_t bus)
{
    hal_eflash_timing_tms_0_set_tms_0(bus, EMBED_FLASH_TIME_REG_32M);
    hal_eflash_timing_tnvs_0_set_tnvs_0(bus, EMBED_FLASH_TIMING_TNVS_32M);
    hal_eflash_timing_terase_0_set_terase_0(bus, EMBED_FLASH_TIMING_TERASE_32M);
    hal_eflash_timing_trcv_sector_0_set_trcv_sector_0(bus, EMBED_FLASH_TIMING_TRCV_32M);
    hal_eflash_timing_tsce_0_set_tsce_0(bus, EMBED_FLASH_TIMING_TSCE_32M);
    hal_eflash_timing_trcv_chip_0_set_trcv_chip_0(bus, EMBED_FLASH_TIMING_TRCV_CHIP_32M);
    hal_eflash_timing_tpgs_0_set_tpgs_0(bus, EMBED_FLASH_TIMING_TPGS_32M);
    hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_32M);
    hal_eflash_timing_trcv_prog_0_set_trcv_prog_0(bus, EMBED_FLASH_TIMING_TRCV_PROG_32M);
    hal_eflash_timing_tcfl_0_set_tcfl_0(bus, EMBED_FLASH_TIMING_TCFL_32M);
    hal_cfg_eflash_erase_next_rw_0_set_cfg_eflash_erase_next_rw_0(bus, EMBED_FLASH_ERASE_NEXT_RW_32M);
    hal_cfg_eflash_prog_next_rw_0_set_cfg_eflash_prog_next_rw_0(bus, EMBED_FLASH_PROG_NEXT_RW_32M);
    hal_cfg_eflash_recall_time_0_set_cfg_eflash_recall_time_0(bus, EMBED_FLASH_RECALL_TIME_32M);
    hal_cfg_eflash_lvctl_time_0_set_cfg_eflash_lvctl_time_0(bus, EMBED_FLASH_LVCTL_TIME_32M);
    hal_cfg_eflash_frequency_sel_0_set_cfg_eflash_frequency_cnt_0(bus, EMBED_FLASH_FREQUENCY_SEL_32M);
    hal_eflash0_cfg_5_set_eflash0_cfg_5(bus, EMBED_FLASH_TERASE_VER_32M);
}

static void hal_eflash_v150_set_clock_48m(eflash_region_t bus)
{
    hal_eflash_timing_tms_0_set_tms_0(bus, EMBED_FLASH_TIME_REG_48M);
    hal_eflash_timing_tnvs_0_set_tnvs_0(bus, EMBED_FLASH_TIMING_TNVS_48M);
    hal_eflash_timing_terase_0_set_terase_0(bus, EMBED_FLASH_TIMING_TERASE_48M);
    hal_eflash_timing_trcv_sector_0_set_trcv_sector_0(bus, EMBED_FLASH_TIMING_TRCV_48M);
    hal_eflash_timing_tsce_0_set_tsce_0(bus, EMBED_FLASH_TIMING_TSCE_48M);
    hal_eflash_timing_trcv_chip_0_set_trcv_chip_0(bus, EMBED_FLASH_TIMING_TRCV_CHIP_48M);
    hal_eflash_timing_tpgs_0_set_tpgs_0(bus, EMBED_FLASH_TIMING_TPGS_48M);
    hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_48M);
    hal_eflash_timing_trcv_prog_0_set_trcv_prog_0(bus, EMBED_FLASH_TIMING_TRCV_PROG_48M);
    hal_eflash_timing_tcfl_0_set_tcfl_0(bus, EMBED_FLASH_TIMING_TCFL_48M);
    hal_cfg_eflash_erase_next_rw_0_set_cfg_eflash_erase_next_rw_0(bus, EMBED_FLASH_ERASE_NEXT_RW_48M);
    hal_cfg_eflash_prog_next_rw_0_set_cfg_eflash_prog_next_rw_0(bus, EMBED_FLASH_PROG_NEXT_RW_48M);
    hal_cfg_eflash_recall_time_0_set_cfg_eflash_recall_time_0(bus, EMBED_FLASH_RECALL_TIME_48M);
    hal_cfg_eflash_lvctl_time_0_set_cfg_eflash_lvctl_time_0(bus, EMBED_FLASH_LVCTL_TIME_48M);
    hal_cfg_eflash_frequency_sel_0_set_cfg_eflash_frequency_cnt_0(bus, EMBED_FLASH_FREQUENCY_SEL_48M);
    hal_eflash0_cfg_5_set_eflash0_cfg_5(bus, EMBED_FLASH_TERASE_VER_48M);
}

static void hal_eflash_v150_set_clock_64m(eflash_region_t bus)
{
    hal_eflash_timing_tms_0_set_tms_0(bus, EMBED_FLASH_TIME_REG_64M);
    hal_eflash_timing_tnvs_0_set_tnvs_0(bus, EMBED_FLASH_TIMING_TNVS_64M);
    hal_eflash_timing_terase_0_set_terase_0(bus, EMBED_FLASH_TIMING_TERASE_64M);
    hal_eflash_timing_trcv_sector_0_set_trcv_sector_0(bus, EMBED_FLASH_TIMING_TRCV_64M);
    hal_eflash_timing_tsce_0_set_tsce_0(bus, EMBED_FLASH_TIMING_TSCE_64M);
    hal_eflash_timing_trcv_chip_0_set_trcv_chip_0(bus, EMBED_FLASH_TIMING_TRCV_CHIP_64M);
    hal_eflash_timing_tpgs_0_set_tpgs_0(bus, EMBED_FLASH_TIMING_TPGS_64M);
    hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_64M);
    hal_eflash_timing_trcv_prog_0_set_trcv_prog_0(bus, EMBED_FLASH_TIMING_TRCV_PROG_64M);
    hal_eflash_timing_tcfl_0_set_tcfl_0(bus, EMBED_FLASH_TIMING_TCFL_64M);
    hal_cfg_eflash_erase_next_rw_0_set_cfg_eflash_erase_next_rw_0(bus, EMBED_FLASH_ERASE_NEXT_RW_64M);
    hal_cfg_eflash_prog_next_rw_0_set_cfg_eflash_prog_next_rw_0(bus, EMBED_FLASH_PROG_NEXT_RW_64M);
    hal_cfg_eflash_recall_time_0_set_cfg_eflash_recall_time_0(bus, EMBED_FLASH_RECALL_TIME_64M);
    hal_cfg_eflash_lvctl_time_0_set_cfg_eflash_lvctl_time_0(bus, EMBED_FLASH_LVCTL_TIME_64M);
    hal_cfg_eflash_frequency_sel_0_set_cfg_eflash_frequency_cnt_0(bus, EMBED_FLASH_FREQUENCY_SEL_64M);
    hal_eflash0_cfg_5_set_eflash0_cfg_5(bus, EMBED_FLASH_TERASE_VER_64M);
}

static void hal_eflash_v150_set_clock_96m(eflash_region_t bus)
{
    hal_eflash_timing_tms_0_set_tms_0(bus, EMBED_FLASH_TIME_REG_96M);
    hal_eflash_timing_tnvs_0_set_tnvs_0(bus, EMBED_FLASH_TIMING_TNVS_96M);
    hal_eflash_timing_terase_0_set_terase_0(bus, EMBED_FLASH_TIMING_TERASE_96M);
    hal_eflash_timing_trcv_sector_0_set_trcv_sector_0(bus, EMBED_FLASH_TIMING_TRCV_96M);
    hal_eflash_timing_tsce_0_set_tsce_0(bus, EMBED_FLASH_TIMING_TSCE_96M);
    hal_eflash_timing_trcv_chip_0_set_trcv_chip_0(bus, EMBED_FLASH_TIMING_TRCV_CHIP_96M);
    hal_eflash_timing_tpgs_0_set_tpgs_0(bus, EMBED_FLASH_TIMING_TPGS_96M);
    hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_96M);
    hal_eflash_timing_trcv_prog_0_set_trcv_prog_0(bus, EMBED_FLASH_TIMING_TRCV_PROG_96M);
    hal_eflash_timing_tcfl_0_set_tcfl_0(bus, EMBED_FLASH_TIMING_TCFL_96M);
    hal_cfg_eflash_erase_next_rw_0_set_cfg_eflash_erase_next_rw_0(bus, EMBED_FLASH_ERASE_NEXT_RW_96M);
    hal_cfg_eflash_prog_next_rw_0_set_cfg_eflash_prog_next_rw_0(bus, EMBED_FLASH_PROG_NEXT_RW_96M);
    hal_cfg_eflash_recall_time_0_set_cfg_eflash_recall_time_0(bus, EMBED_FLASH_RECALL_TIME_96M);
    hal_cfg_eflash_lvctl_time_0_set_cfg_eflash_lvctl_time_0(bus, EMBED_FLASH_LVCTL_TIME_96M);
    hal_cfg_eflash_frequency_sel_0_set_cfg_eflash_frequency_cnt_0(bus, EMBED_FLASH_FREQUENCY_SEL_96M);
    hal_eflash0_cfg_5_set_eflash0_cfg_5(bus, EMBED_FLASH_TERASE_VER_96M);
}

static void hal_eflash_v150_set_freq(eflash_region_t bus, eflash_frequency_t freq)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    switch (freq) {
        case CLOCK_32M:
            hal_eflash_v150_set_clock_32m(bus);
            break;
        case CLOCK_48M:
            hal_eflash_v150_set_clock_48m(bus);
            break;
        case CLOCK_64M:
            hal_eflash_v150_set_clock_64m(bus);
            break;
        default:
            hal_eflash_v150_set_clock_96m(bus);
            break;
    }
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
    unused(freq);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
}

#if defined(CONFIG_SET_NVR_FUNCTION)
static void hal_eflash_v150_trim(eflash_region_t bus)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    /* 1.read flash */
    /* 2.trim */
    uint64_t addr = g_eflash_nvr_cfg_regs[bus] + EMBED_FLASH_CBD_OFFSET;
    for (uint8_t i = 0; i < EMBED_FLASH_CBD_NUM; i++) {
        hal_cfg_eflash_confen_0_set_cfg_eflash_confen_addr_0(bus, i);
        hal_cfg_eflash_confen_data_0_set_cfg_eflash_confen_data_0(bus, uapi_reg_read_val32(addr) &
                                                                  (uint32_t)EMBED_FLASH_FIRST_2_BYTE_MASK);
        hal_cfg_eflash_confen_0_set_cfg_eflash_confen_enable_pulse_0(bus, 1);
        if (bus == EMBED_FLASH_1) {
            hal_cfg_eflash_confen_0_set_cfg_eflash_confen_enable_pulse_0(EMBED_FLASH_1, 0);
        }
        while (hal_eflash_erase_confen_state_0_get_confen_writing_0(bus) == 1) {}
        addr += EMBED_FLASH_8_BYTES_SHIFT;
    }

    /* 3.repair */
    uint32_t rrx;
    addr = g_eflash_nvr_cfg_regs[bus];
    for (uint8_t i = 0; i < EMBED_FLASH_RR_NUM; i++) {
        uint32_t value = uapi_reg_read_val32(addr);
        rrx = (value >> EMBED_FLASH_SHIFT_8) & (uint32_t)EMBED_FLASH_FIRST_BIT_MASK;
        if (rrx == 0) {
            rrx = value & (uint32_t)EMBED_FLASH_FIRST_BYTE_MASK;
            if (i == 0) {
                hal_cfg_eflash_redun0_0_set_cfg_eflash_redun0_addr_0(bus, rrx);
                hal_cfg_eflash_redun0_0_set_cfg_eflash_redun0_en_0(bus, 1);
            } else if (i == 1) {
                hal_cfg_eflash_redun1_0_set_cfg_eflash_redun1_addr_0(bus, rrx);
                hal_cfg_eflash_redun1_0_set_cfg_eflash_redun1_en_0(bus, 1);
            }
        }
        addr += EMBED_FLASH_8_BYTES_SHIFT;
    }
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
}

static void hal_eflash_v150_set_nvr(eflash_region_t bus)
{
    hal_eflash_v150_trim(bus);
}
#endif /* CONFIG_SET_NVR_FUNCTION */

static void hal_eflash_v150_read(uint32_t addr, uint32_t *data, uint32_t len)
{
    uint32_t *tmp_data = data;
    for (uint32_t i = addr; i < addr + len; i += EMBED_FLASH_4_BYTES_SHIFT) {
        *tmp_data = uapi_reg_read_val32(i);
        tmp_data++;
    }
}

static void hal_eflash_v150_set_erase_verify(eflash_region_t bus)
{
#if defined(CONFIG_ERASE_VERIFY)
    hal_eflash0_cfg_1_set_cfg_eflash_erase_verify_en(bus, EMBED_FLASH_ERASE_VERIFY_EN);
    hal_eflash0_cfg_2_set_eflash0_cfg_2(bus, EMBED_FLASH_VERIFY_ADDR_STEP);
    hal_eflash0_cfg_3_set_eflash0_cfg_3(bus, EMBED_FLASH_VERIFY_ADDR_TIMES);
    hal_eflash0_cfg_4_set_eflash0_cfg_4(bus, EMBED_FLASH_ERAVER_ERASE_TIMES);
#else
    unused(bus);
#endif /* CONFIG_ERASE_VERIFY */
}


static void hal_eflash_v150_chip_erase(eflash_region_t bus)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    hal_cfg_eflash_write_read_en_0_set_cfg_eflash_write_read_en_0(bus, 1);
    hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_write_protect_0(bus, 1);
    hal_cfg_eflash_erase_0_set_cfg_eflash_erase_chip_0(bus, 1);
    hal_cfg_eflash_erase_0_set_cfg_eflash_erase_enable_pulse_0(bus, 1);
    /* chip_erasing */
    while (hal_eflash_erase_confen_state_0_get_chip_erasing_0(bus) == 1) {}
    hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_write_protect_0(bus, 0);
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
}

static void hal_eflash_v150_sector_erase(eflash_region_t bus, uint32_t id)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    hal_cfg_eflash_write_read_en_0_set_cfg_eflash_write_read_en_0(bus, 1);
    hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_write_protect_0(bus, 1);
    /* chip erase indication signal: 0(sector erase) 1(chip erase) */
    hal_cfg_eflash_erase_0_set_cfg_eflash_erase_chip_0(bus, 0);
    hal_cfg_eflash_erase_addr_0_set_cfg_eflash_erase_addr_0(bus, id);
    hal_cfg_eflash_erase_0_set_cfg_eflash_erase_enable_pulse_0(bus, 1);
    /* sector_erasing, verify sector erase is complete. */
    while (hal_eflash_erase_confen_state_0_get_sector_erasing_0(bus) == 1) {}
    hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_write_protect_0(bus, 0);
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
    unused(id);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
}

static void hal_eflash_v150_write_normal(uint32_t addr, const uint32_t *data, uint32_t len)
{
    uint32_t write_addr = addr;
    uint32_t temp = len >> EMBED_FLASH_SHIFT_2;
    for (uint32_t i = 0; i < temp; i++) {
        uapi_reg_write(write_addr, data[i]);
        write_addr += EMBED_FLASH_4_BYTES_SHIFT;
    }
}

static void hal_eflash_v150_pull_up_preprog(eflash_region_t bus, eflash_frequency_t freq)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    hal_eflash0_cfg_1_set_cfg_eflash_preprogram(bus, 1);
    switch (freq) {
        case CLOCK_32M:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_PREPROGRAM_32M);
            break;
        case CLOCK_48M:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_PREPROGRAM_48M);
            break;
        case CLOCK_64M:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_PREPROGRAM_64M);
            break;
        default:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_PREPROGRAM_96M);
            break;
    }
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
    unused(freq);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
}

static void hal_eflash_v150_pull_down_preprog(eflash_region_t bus, eflash_frequency_t freq)
{
#if defined(CONFIG_REAL_EMBED_FLASH)
    hal_eflash0_cfg_1_set_cfg_eflash_preprogram(bus, 0);
    switch (freq) {
        case CLOCK_32M:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_32M);
            break;
        case CLOCK_48M:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_48M);
            break;
        case CLOCK_64M:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_64M);
            break;
        default:
            hal_eflash_timing_tprog_0_set_tprog_0(bus, EMBED_FLASH_TIMING_TPROG_96M);
            break;
    }
#endif /* CONFIG_REAL_EMBED_FLASH */
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    unused(bus);
    unused(freq);
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
}

static void hal_eflash_v150_write(uint32_t addr, const uint32_t *data, uint32_t len, eflash_region_t bus,
                                  eflash_frequency_t freq)
{
    hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_write_protect_0(bus, 1);
#if defined(CONFIG_PULL_UP_PULL_DOWN_WRITE)
    hal_eflash_v150_pull_up_preprog(bus, freq);
    hal_eflash_v150_write_normal(addr, data, len);
    hal_eflash_v150_pull_down_preprog(bus, freq);
#endif /* CONFIG_PULL_UP_PULL_DOWN_WRITE */
    hal_eflash_v150_write_normal(addr, data, len);
    hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_write_protect_0(bus, 0);
}

static void hal_eflash_v150_suspend(void)
{
    /* set DPD SEL */
    hal_eflash_por_dpd_sel_set_cfg_eflash0_por_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash0_dpd_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash1_por_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash1_dpd_dbg_sel(1);
    /* set DPD MAN */
    hal_eflash_por_dpd_man_set_cfg_eflash0_por_dbg(1);
    hal_eflash_por_dpd_man_set_cfg_eflash0_dpd_dbg(1);
    hal_eflash_por_dpd_man_set_cfg_eflash1_por_dbg(1);
    hal_eflash_por_dpd_man_set_cfg_eflash1_dpd_dbg(1);
}

static void hal_eflash_v150_resume(void)
{
    /* set DPD SEL */
    hal_eflash_por_dpd_sel_set_cfg_eflash0_por_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash0_dpd_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash1_por_dbg_sel(1);
    hal_eflash_por_dpd_sel_set_cfg_eflash1_dpd_dbg_sel(1);
    /* set DPD MAN */
    hal_eflash_por_dpd_man_set_cfg_eflash0_por_dbg(1);
    hal_eflash_por_dpd_man_set_cfg_eflash0_dpd_dbg(0);
    hal_eflash_por_dpd_man_set_cfg_eflash1_por_dbg(1);
    hal_eflash_por_dpd_man_set_cfg_eflash1_dpd_dbg(0);
}

static hal_eflash_funcs_t g_hal_eflash_v150_funcs = {
    .init = hal_eflash_v150_init,
    .deinit = hal_eflash_v150_deinit,
    .set_freq = hal_eflash_v150_set_freq,
#if defined(CONFIG_SET_NVR_FUNCTION)
    .set_nvr = hal_eflash_v150_set_nvr,
#endif /* CONFIG_SET_NVR_FUNCTION */
    .set_erase_verify = hal_eflash_v150_set_erase_verify,
    .read = hal_eflash_v150_read,
    .write = hal_eflash_v150_write,
    .chip_erase = hal_eflash_v150_chip_erase,
    .sector_erase = hal_eflash_v150_sector_erase,
    .suspend = hal_eflash_v150_suspend,
    .resume = hal_eflash_v150_resume,
};

hal_eflash_funcs_t *hal_eflash_v150_funcs_get(void)
{
    return &g_hal_eflash_v150_funcs;
}