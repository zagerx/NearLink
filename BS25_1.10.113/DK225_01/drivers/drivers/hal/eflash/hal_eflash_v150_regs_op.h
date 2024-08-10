/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides V150 EFLASH register \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-08， Create file. \n
 */

#ifndef HAL_EFLASH_V150_REGS_OP_H
#define HAL_EFLASH_V150_REGS_OP_H

#include <stdint.h>
#include "errcode.h"
#include "eflash_porting.h"
#include "hal_eflash_v150_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_eflash_v150_regs_op EFLASH V150 Regs Operation
 * @ingroup  drivers_hal_eflash
 * @{
 */

extern uintptr_t g_eflash_regs[EMBED_FLASH_REGION_MAX_NUM];
extern uintptr_t g_xip_cache_regs;
extern uintptr_t g_xip_cache_man_all_regs;
extern uintptr_t g_eflash_stb_mux_regs;
extern uintptr_t g_eflash_por_dpd_regs;
extern uintptr_t g_eflash_nvr_cfg_regs[EMBED_FLASH_REGION_MAX_NUM];
extern uintptr_t g_eflash_flash_cfg_regs[EMBED_FLASH_REGION_MAX_NUM];

/**
 * @brief  Set the value of @ref eflash_timing_tms_0_data_t.tms_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_tms_0_data_t.tms_0.
 */
static inline void hal_eflash_timing_tms_0_set_tms_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_tms_0_data_t timing_tms;
    timing_tms.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->time_reg;
    timing_tms.b.tms_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->time_reg = timing_tms.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_tms_0_data_t.tms_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_tms_0_data_t.tms_0.
 */
static inline uint32_t hal_eflash_timing_tms_0_get_tms_0(eflash_region_t bus)
{
    eflash_timing_tms_0_data_t timing_tms;
    timing_tms.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->time_reg;
    return timing_tms.b.tms_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_tnvs_0_data_t.tnvs_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_tnvs_0_data_t.tnvs_0.
 */
static inline void hal_eflash_timing_tnvs_0_set_tnvs_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_tnvs_0_data_t timing_tnvs_0;
    timing_tnvs_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tnvs;
    timing_tnvs_0.b.tnvs_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_tnvs = timing_tnvs_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_tnvs_0_data_t.tnvs_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_tnvs_0_data_t.tnvs_0.
 */
static inline uint32_t hal_eflash_timing_tnvs_0_get_tnvs_0(eflash_region_t bus)
{
    eflash_timing_tnvs_0_data_t timing_tnvs_0;
    timing_tnvs_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tnvs;
    return timing_tnvs_0.b.tnvs_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_terase_0_data_t.terase_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_terase_0_data_t.terase_0.
 */
static inline void hal_eflash_timing_terase_0_set_terase_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_terase_0_data_t timing_terase_0;
    timing_terase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_terase;
    timing_terase_0.b.terase_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_terase = timing_terase_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_terase_0_data_t.terase_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_terase_0_data_t.terase_0.
 */
static inline uint32_t hal_eflash_timing_terase_0_get_terase_0(eflash_region_t bus)
{
    eflash_timing_terase_0_data_t timing_terase_0;
    timing_terase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_terase;
    return timing_terase_0.b.terase_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_trcv_sector_0_data_t.trcv_sector_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_trcv_sector_0_data_t.trcv_sector_0.
 */
static inline void hal_eflash_timing_trcv_sector_0_set_trcv_sector_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_trcv_sector_0_data_t timing_trcv_sector_0;
    timing_trcv_sector_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_sector;
    timing_trcv_sector_0.b.trcv_sector_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_sector = timing_trcv_sector_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_trcv_sector_0_data_t.trcv_sector_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_trcv_sector_0_data_t.trcv_sector_0.
 */
static inline uint32_t hal_eflash_timing_trcv_sector_0_get_trcv_sector_0(eflash_region_t bus)
{
    eflash_timing_trcv_sector_0_data_t timing_trcv_sector_0;
    timing_trcv_sector_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_sector;
    return timing_trcv_sector_0.b.trcv_sector_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_tsce_0_data_t.tsce_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_tsce_0_data_t.tsce_0.
 */
static inline void hal_eflash_timing_tsce_0_set_tsce_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_tsce_0_data_t timing_tsce_0;
    timing_tsce_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tsce;
    timing_tsce_0.b.tsce_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_tsce = timing_tsce_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_tsce_0_data_t.tsce_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_tsce_0_data_t.tsce_0.
 */
static inline uint32_t hal_eflash_timing_tsce_0_get_tsce_0(eflash_region_t bus)
{
    eflash_timing_tsce_0_data_t timing_tsce_0;
    timing_tsce_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tsce;
    return timing_tsce_0.b.tsce_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_trcv_chip_0_data_t.trcv_chip_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_trcv_chip_0_data_t.trcv_chip_0.
 */
static inline void hal_eflash_timing_trcv_chip_0_set_trcv_chip_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_trcv_chip_0_data_t timing_trcv_chip_0;
    timing_trcv_chip_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_chip;
    timing_trcv_chip_0.b.trcv_chip_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_chip = timing_trcv_chip_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_tsce_0_data_t.tsce_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_trcv_chip_0_data_t.trcv_chip_0.
 */
static inline uint32_t hal_eflash_timing_trcv_chip_0_get_trcv_chip_0(eflash_region_t bus)
{
    eflash_timing_trcv_chip_0_data_t timing_trcv_chip_0;
    timing_trcv_chip_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_chip;
    return timing_trcv_chip_0.b.trcv_chip_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_tpgs_0_data_t.tpgs_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_tpgs_0_data_t.tpgs_0.
 */
static inline void hal_eflash_timing_tpgs_0_set_tpgs_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_tpgs_0_data_t timing_tpgs_0;
    timing_tpgs_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tpgs;
    timing_tpgs_0.b.tpgs_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_tpgs = timing_tpgs_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_tpgs_0_data_t.tpgs_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_tpgs_0_data_t.tpgs_0.
 */
static inline uint32_t hal_eflash_timing_tpgs_0_get_tpgs_0(eflash_region_t bus)
{
    eflash_timing_tpgs_0_data_t timing_tpgs_0;
    timing_tpgs_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tpgs;
    return timing_tpgs_0.b.tpgs_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_tprog_0_data_t.tprog_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_tprog_0_data_t.tprog_0.
 */
static inline void hal_eflash_timing_tprog_0_set_tprog_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_tprog_0_data_t timing_tprog_0;
    timing_tprog_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tprog;
    timing_tprog_0.b.tprog_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_tprog = timing_tprog_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_tprog_0_data_t.tprog_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_tprog_0_data_t.tprog_0.
 */
static inline uint32_t hal_eflash_timing_tprog_0_get_tprog_0(eflash_region_t bus)
{
    eflash_timing_tprog_0_data_t timing_tprog_0;
    timing_tprog_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tprog;
    return timing_tprog_0.b.tprog_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_trcv_prog_0_data_t.trcv_prog_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_trcv_prog_0_data_t.trcv_prog_0.
 */
static inline void hal_eflash_timing_trcv_prog_0_set_trcv_prog_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_trcv_prog_0_data_t timing_trcv_prog_0;
    timing_trcv_prog_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_prog;
    timing_trcv_prog_0.b.trcv_prog_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_prog = timing_trcv_prog_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_trcv_prog_0_data_t.trcv_prog_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_trcv_prog_0_data_t.trcv_prog_0.
 */
static inline uint32_t hal_eflash_timing_trcv_prog_0_get_trcv_prog_0(eflash_region_t bus)
{
    eflash_timing_trcv_prog_0_data_t timing_trcv_prog_0;
    timing_trcv_prog_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_trcv_prog;
    return timing_trcv_prog_0.b.trcv_prog_0;
}

/**
 * @brief  Set the value of @ref eflash_timing_tcfl_0_data_t.tcfl_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_timing_tcfl_0_data_t.tcfl_0.
 */
static inline void hal_eflash_timing_tcfl_0_set_tcfl_0(eflash_region_t bus, uint32_t val)
{
    eflash_timing_tcfl_0_data_t timing_tcfl_0;
    timing_tcfl_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tcfl;
    timing_tcfl_0.b.tcfl_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->timing_tcfl = timing_tcfl_0.d32;
}

/**
 * @brief  Get the value of @ref eflash_timing_tcfl_0_data_t.tcfl_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_timing_tcfl_0_data_t.tcfl_0.
 */
static inline uint32_t hal_eflash_timing_tcfl_0_get_tcfl_0(eflash_region_t bus)
{
    eflash_timing_tcfl_0_data_t timing_tcfl_0;
    timing_tcfl_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->timing_tcfl;
    return timing_tcfl_0.b.tcfl_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_erase_next_rw_0_data_t.cfg_eflash_erase_next_rw_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_erase_next_rw_0_data_t.cfg_eflash_erase_next_rw_0.
 */
static inline void hal_cfg_eflash_erase_next_rw_0_set_cfg_eflash_erase_next_rw_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_erase_next_rw_0_data_t cfg_eflash_erase_next_rw;
    cfg_eflash_erase_next_rw.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_next_rw;
    cfg_eflash_erase_next_rw.b.cfg_eflash_erase_next_rw_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->erase_next_rw = cfg_eflash_erase_next_rw.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_erase_next_rw_0_data_t.cfg_eflash_erase_next_rw_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_erase_next_rw_0_data_t.cfg_eflash_erase_next_rw_0.
 */
static inline uint32_t hal_cfg_eflash_erase_next_rw_0_get_cfg_eflash_erase_next_rw_0(eflash_region_t bus)
{
    cfg_eflash_erase_next_rw_0_data_t cfg_eflash_erase_next_rw;
    cfg_eflash_erase_next_rw.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_next_rw;
    return cfg_eflash_erase_next_rw.b.cfg_eflash_erase_next_rw_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_prog_next_rw_0_data_t.cfg_eflash_prog_next_rw_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_prog_next_rw_0_data_t.cfg_eflash_prog_next_rw_0.
 */
static inline void hal_cfg_eflash_prog_next_rw_0_set_cfg_eflash_prog_next_rw_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_prog_next_rw_0_data_t cfg_eflash_prog_next_rw;
    cfg_eflash_prog_next_rw.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->prog_next_rw;
    cfg_eflash_prog_next_rw.b.cfg_eflash_prog_next_rw_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->prog_next_rw = cfg_eflash_prog_next_rw.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_prog_next_rw_0_data_t.cfg_eflash_prog_next_rw_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_prog_next_rw_0_data_t.cfg_eflash_prog_next_rw_0.
 */
static inline uint32_t hal_cfg_eflash_prog_next_rw_0_get_cfg_eflash_prog_next_rw_0(eflash_region_t bus)
{
    cfg_eflash_prog_next_rw_0_data_t cfg_eflash_prog_next_rw;
    cfg_eflash_prog_next_rw.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->prog_next_rw;
    return cfg_eflash_prog_next_rw.b.cfg_eflash_prog_next_rw_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_recall_time_0_data_t.cfg_eflash_recall_time_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_recall_time_0_data_t.cfg_eflash_recall_time_0.
 */
static inline void hal_cfg_eflash_recall_time_0_set_cfg_eflash_recall_time_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_recall_time_0_data_t cfg_eflash_recall_time;
    cfg_eflash_recall_time.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->recall_time;
    cfg_eflash_recall_time.b.cfg_eflash_recall_time_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->recall_time = cfg_eflash_recall_time.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_recall_time_0_data_t.cfg_eflash_recall_time_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_recall_time_0_data_t.cfg_eflash_recall_time_0.
 */
static inline uint32_t hal_cfg_eflash_recall_time_0_get_cfg_eflash_recall_time_0(eflash_region_t bus)
{
    cfg_eflash_recall_time_0_data_t cfg_eflash_recall_time;
    cfg_eflash_recall_time.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->recall_time;
    return cfg_eflash_recall_time.b.cfg_eflash_recall_time_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_lvctl_time_0_data_t.cfg_eflash_lvctl_time_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_lvctl_time_0_data_t.cfg_eflash_lvctl_time_0.
 */
static inline void hal_cfg_eflash_lvctl_time_0_set_cfg_eflash_lvctl_time_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_lvctl_time_0_data_t lvctl_time_0;
    lvctl_time_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->lvctl_time;
    lvctl_time_0.b.cfg_eflash_lvctl_time_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->lvctl_time = lvctl_time_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_lvctl_time_0_data_t.cfg_eflash_lvctl_time_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_lvctl_time_0_data_t.cfg_eflash_lvctl_time_0.
 */
static inline uint32_t hal_cfg_eflash_lvctl_time_0_get_cfg_eflash_lvctl_time_0(eflash_region_t bus)
{
    cfg_eflash_lvctl_time_0_data_t lvctl_time_0;
    lvctl_time_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->lvctl_time;
    return lvctl_time_0.b.cfg_eflash_lvctl_time_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_frequency_sel_0_data_t.cfg_eflash_frequency_cnt_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_frequency_sel_0_data_t.cfg_eflash_frequency_cnt_0.
 */
static inline void hal_cfg_eflash_frequency_sel_0_set_cfg_eflash_frequency_cnt_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_frequency_sel_0_data_t frequency_sel_0;
    frequency_sel_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->frequency_sel;
    frequency_sel_0.b.cfg_eflash_frequency_cnt_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->frequency_sel = frequency_sel_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_frequency_sel_0_data_t.cfg_eflash_frequency_cnt_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_frequency_sel_0_data_t.cfg_eflash_frequency_cnt_0.
 */
static inline uint32_t hal_cfg_eflash_frequency_sel_0_get_cfg_eflash_frequency_cnt_0(eflash_region_t bus)
{
    cfg_eflash_frequency_sel_0_data_t frequency_sel_0;
    frequency_sel_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->frequency_sel;
    return frequency_sel_0.b.cfg_eflash_frequency_cnt_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_write_read_en_0_data_t.cfg_eflash_write_read_en_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_write_read_en_0_data_t.cfg_eflash_write_read_en_0.
 */
static inline void hal_cfg_eflash_write_read_en_0_set_cfg_eflash_write_read_en_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_write_read_en_0_data_t write_read_en_0;
    write_read_en_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->write_read_reg;
    write_read_en_0.b.cfg_eflash_write_read_en_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->write_read_reg = write_read_en_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_write_read_en_0_data_t.cfg_eflash_write_read_en_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_write_read_en_0_data_t.cfg_eflash_write_read_en_0.
 */
static inline uint32_t hal_cfg_eflash_write_read_en_0_get_cfg_eflash_write_read_en_0(eflash_region_t bus)
{
    cfg_eflash_write_read_en_0_data_t write_read_en_0;
    write_read_en_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->write_read_reg;
    return write_read_en_0.b.cfg_eflash_write_read_en_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_write_protect_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_write_protect_0.
 */
static inline void hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_write_protect_0(eflash_region_t bus,
                                                                                      uint32_t val)
{
    cfg_eflash_write_read_protect_0_data_t write_read_protect_0;
    write_read_protect_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->write_read_protect;
    write_read_protect_0.b.cfg_eflash_write_protect_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->write_read_protect = write_read_protect_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_write_protect_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_write_protect_0.
 */
static inline uint32_t hal_cfg_eflash_write_read_protect_0_get_cfg_eflash_write_protect_0(eflash_region_t bus)
{
    cfg_eflash_write_read_protect_0_data_t write_read_protect_0;
    write_read_protect_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->write_read_protect;
    return write_read_protect_0.b.cfg_eflash_write_protect_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_read_protect_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_read_protect_0.
 */
static inline void hal_cfg_eflash_write_read_protect_0_set_cfg_eflash_read_protect_0(eflash_region_t bus,
                                                                                     uint32_t val)
{
    cfg_eflash_write_read_protect_0_data_t wirte_read_protect_0;
    wirte_read_protect_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->write_read_protect;
    wirte_read_protect_0.b.cfg_eflash_read_protect_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->write_read_protect = wirte_read_protect_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_read_protect_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_write_read_protect_0_data_t.cfg_eflash_read_protect_0.
 */
static inline uint32_t hal_cfg_eflash_write_read_protect_0_get_cfg_eflash_read_protect_0(eflash_region_t bus)
{
    cfg_eflash_write_read_protect_0_data_t wirte_read_protect_0;
    wirte_read_protect_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->write_read_protect;
    return wirte_read_protect_0.b.cfg_eflash_read_protect_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_nvr_recall_0_data_t.cfg_eflash_nvr_recall_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_nvr_recall_0_data_t.cfg_eflash_nvr_recall_0.
 */
static inline void hal_cfg_eflash_nvr_recall_0_set_cfg_eflash_nvr_recall_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_nvr_recall_0_data_t nvr_recall_0;
    nvr_recall_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->nvr_recall;
    nvr_recall_0.b.cfg_eflash_nvr_recall_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->nvr_recall = nvr_recall_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_nvr_recall_0_data_t.cfg_eflash_nvr_recall_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_nvr_recall_0_data_t.cfg_eflash_nvr_recall_0.
 */
static inline uint32_t hal_cfg_eflash_nvr_recall_0_get_cfg_eflash_nvr_recall_0(eflash_region_t bus)
{
    cfg_eflash_nvr_recall_0_data_t nvr_recall_0;
    nvr_recall_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->nvr_recall;
    return nvr_recall_0.b.cfg_eflash_nvr_recall_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_lck_cfg_0_data_t.cfg_eflash_lck_cfg_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_lck_cfg_0_data_t.cfg_eflash_lck_cfg_0.
 */
static inline void hal_cfg_eflash_lck_cfg_0_set_cfg_eflash_lck_cfg_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_lck_cfg_0_data_t lck_cfg_0;
    lck_cfg_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->lck_cfg;
    lck_cfg_0.b.cfg_eflash_lck_cfg_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->lck_cfg = lck_cfg_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_lck_cfg_0_data_t.cfg_eflash_lck_cfg_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_lck_cfg_0_data_t.cfg_eflash_lck_cfg_0.
 */
static inline uint32_t hal_cfg_eflash_lck_cfg_0_get_cfg_eflash_lck_cfg_0(eflash_region_t bus)
{
    cfg_eflash_lck_cfg_0_data_t lck_cfg_0;
    lck_cfg_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->lck_cfg;
    return lck_cfg_0.b.cfg_eflash_lck_cfg_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_lvctl_0_data_t.cfg_eflash_lvctl_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_lvctl_0_data_t.cfg_eflash_lvctl_0.
 */
static inline void hal_cfg_eflash_lvctl_0_set_cfg_eflash_lvctl_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_lvctl_0_data_t lvctl_0;
    lvctl_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->lvctl;
    lvctl_0.b.cfg_eflash_lvctl_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->lvctl = lvctl_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_lvctl_0_data_t.cfg_eflash_lvctl_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_lvctl_0_data_t.cfg_eflash_lvctl_0.
 */
static inline uint32_t hal_cfg_eflash_lvctl_0_get_cfg_eflash_lvctl_0(eflash_region_t bus)
{
    cfg_eflash_lvctl_0_data_t lvctl_0;
    lvctl_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->lvctl;
    return lvctl_0.b.cfg_eflash_lvctl_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_overtime_cnt_high_0_data_t.cfg_eflash_overtime_cnt_high_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_overtime_cnt_high_0_data_t.cfg_eflash_overtime_cnt_high_0.
 */
static inline void hal_cfg_eflash_overtime_cnt_high_0_set_cfg_eflash_overtime_cnt_high_0(eflash_region_t bus,
                                                                                         uint32_t val)
{
    cfg_eflash_overtime_cnt_high_0_data_t overtime_cnt_high_0;
    overtime_cnt_high_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->overtime_cnt_high;
    overtime_cnt_high_0.b.cfg_eflash_overtime_cnt_high_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->overtime_cnt_high = overtime_cnt_high_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_overtime_cnt_high_0_data_t.cfg_eflash_overtime_cnt_high_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_overtime_cnt_high_0_data_t.cfg_eflash_overtime_cnt_high_0.
 */
static inline uint32_t hal_cfg_eflash_overtime_cnt_high_0_get_cfg_eflash_overtime_cnt_high_0(eflash_region_t bus)
{
    cfg_eflash_overtime_cnt_high_0_data_t overtime_cnt_high_0;
    overtime_cnt_high_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->overtime_cnt_high;
    return overtime_cnt_high_0.b.cfg_eflash_overtime_cnt_high_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_overtime_cnt_low_0_data_t.cfg_eflash_overtime_cnt_low_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_overtime_cnt_low_0_data_t.cfg_eflash_overtime_cnt_low_0.
 */
static inline void hal_cfg_eflash_overtime_cnt_low_0_set_cfg_eflash_overtime_cnt_low_0(eflash_region_t bus,
                                                                                       uint32_t val)
{
    cfg_eflash_overtime_cnt_low_0_data_t overtime_cnt_low_0;
    overtime_cnt_low_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->overtime_cnt_low;
    overtime_cnt_low_0.b.cfg_eflash_overtime_cnt_low_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->overtime_cnt_low = overtime_cnt_low_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_overtime_cnt_low_0_data_t.cfg_eflash_overtime_cnt_low_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_overtime_cnt_low_0_data_t.cfg_eflash_overtime_cnt_low_0.
 */
static inline uint32_t hal_cfg_eflash_overtime_cnt_low_0_get_cfg_eflash_overtime_cnt_low_0(eflash_region_t bus)
{
    cfg_eflash_overtime_cnt_low_0_data_t overtime_cnt_low_0;
    overtime_cnt_low_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->overtime_cnt_low;
    return overtime_cnt_low_0.b.cfg_eflash_overtime_cnt_low_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_enable_pulse_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_enable_pulse_0.
 */
static inline void hal_cfg_eflash_erase_0_set_cfg_eflash_erase_enable_pulse_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    erase_0.b.cfg_eflash_erase_enable_pulse_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg = erase_0.d32;
}

/**
 * @brief  Set the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_chip_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_chip_0.
 */
static inline void hal_cfg_eflash_erase_0_set_cfg_eflash_erase_chip_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    erase_0.b.cfg_eflash_erase_chip_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg = erase_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_chip_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_chip_0.
 */
static inline uint32_t hal_cfg_eflash_erase_0_get_cfg_eflash_erase_chip_0(eflash_region_t bus)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    return erase_0.b.cfg_eflash_erase_chip_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_0.
 */
static inline void hal_cfg_eflash_erase_0_set_cfg_eflash_erase_nvr_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    erase_0.b.cfg_eflash_erase_nvr_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg = erase_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_0.
 */
static inline uint32_t hal_cfg_eflash_erase_0_get_cfg_eflash_erase_nvr_0(eflash_region_t bus)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    return erase_0.b.cfg_eflash_erase_nvr_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_cfg_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_cfg_0.
 */
static inline void hal_cfg_eflash_erase_0_set_cfg_eflash_erase_nvr_cfg_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    erase_0.b.cfg_eflash_erase_nvr_cfg_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg = erase_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_cfg_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_nvr_cfg_0.
 */
static inline uint32_t hal_cfg_eflash_erase_0_get_cfg_eflash_erase_nvr_cfg_0(eflash_region_t bus)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    return erase_0.b.cfg_eflash_erase_nvr_cfg_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_arrdn_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_arrdn_0.
 */
static inline void hal_cfg_eflash_erase_0_set_cfg_eflash_erase_arrdn_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    erase_0.b.cfg_eflash_erase_arrdn_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg = erase_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_arrdn_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_erase_0_data_t.cfg_eflash_erase_arrdn_0.
 */
static inline uint32_t hal_cfg_eflash_erase_0_get_cfg_eflash_erase_arrdn_0(eflash_region_t bus)
{
    cfg_eflash_erase_0_data_t erase_0;
    erase_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_reg;
    return erase_0.b.cfg_eflash_erase_arrdn_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_erase_addr_0_data_t.cfg_eflash_erase_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_erase_addr_0_data_t.cfg_eflash_erase_addr_0.
 */
static inline void hal_cfg_eflash_erase_addr_0_set_cfg_eflash_erase_addr_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_erase_addr_0_data_t erase_addr_0;
    erase_addr_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_addr;
    erase_addr_0.b.cfg_eflash_erase_addr_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->erase_addr = erase_addr_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_erase_addr_0_data_t.cfg_eflash_erase_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_erase_addr_0_data_t.cfg_eflash_erase_addr_0.
 */
static inline uint32_t hal_cfg_eflash_erase_addr_0_get_cfg_eflash_erase_addr_0(eflash_region_t bus)
{
    cfg_eflash_erase_addr_0_data_t erase_addr_0;
    erase_addr_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_addr;
    return erase_addr_0.b.cfg_eflash_erase_addr_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_confen_0_data_t.cfg_eflash_confen_enable_pulse_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_confen_0_data_t.cfg_eflash_confen_enable_pulse_0.
 */
static inline void hal_cfg_eflash_confen_0_set_cfg_eflash_confen_enable_pulse_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_confen_0_data_t confen_0;
    confen_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->confen;
    confen_0.b.cfg_eflash_confen_enable_pulse_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->confen = confen_0.d32;
}

/**
 * @brief  Set the value of @ref cfg_eflash_confen_0_data_t.cfg_eflash_confen_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_confen_0_data_t.cfg_eflash_confen_addr_0.
 */
static inline void hal_cfg_eflash_confen_0_set_cfg_eflash_confen_addr_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_confen_0_data_t confen_0;
    confen_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->confen;
    confen_0.b.cfg_eflash_confen_addr_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->confen = confen_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_confen_0_data_t.cfg_eflash_confen_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_confen_0_data_t.cfg_eflash_confen_addr_0.
 */
static inline uint32_t hal_cfg_eflash_confen_0_get_cfg_eflash_confen_addr_0(eflash_region_t bus)
{
    cfg_eflash_confen_0_data_t confen_0;
    confen_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->confen;
    return confen_0.b.cfg_eflash_confen_addr_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_confen_data_0_data_t.cfg_eflash_confen_data_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_confen_data_0_data_t.cfg_eflash_confen_data_0.
 */
static inline void hal_cfg_eflash_confen_data_0_set_cfg_eflash_confen_data_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_confen_data_0_data_t confen_data_0;
    confen_data_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->confen_data;
    confen_data_0.b.cfg_eflash_confen_data_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->confen_data = confen_data_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_confen_data_0_data_t.cfg_eflash_confen_data_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_confen_data_0_data_t.cfg_eflash_confen_data_0.
 */
static inline uint32_t hal_cfg_eflash_confen_data_0_get_cfg_eflash_confen_data_0(eflash_region_t bus)
{
    cfg_eflash_confen_data_0_data_t confen_data_0;
    confen_data_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->confen_data;
    return confen_data_0.b.cfg_eflash_confen_data_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_en_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_en_0.
 */
static inline void hal_cfg_eflash_redun0_0_set_cfg_eflash_redun0_en_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_redun0_0_data_t redun0_0;
    redun0_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun0;
    redun0_0.b.cfg_eflash_redun0_en_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->redun0 = redun0_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_en_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_en_0.
 */
static inline uint32_t hal_cfg_eflash_redun0_0_get_cfg_eflash_redun0_en_0(eflash_region_t bus)
{
    cfg_eflash_redun0_0_data_t redun0_0;
    redun0_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun0;
    return redun0_0.b.cfg_eflash_redun0_en_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_addr_0.
 */
static inline void hal_cfg_eflash_redun0_0_set_cfg_eflash_redun0_addr_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_redun0_0_data_t redun0_0;
    redun0_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun0;
    redun0_0.b.cfg_eflash_redun0_addr_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->redun0 = redun0_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_addr_0.
 */
static inline uint32_t hal_cfg_eflash_redun0_0_get_cfg_eflash_redun0_addr_0(eflash_region_t bus)
{
    cfg_eflash_redun0_0_data_t redun0_0;
    redun0_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun0;
    return redun0_0.b.cfg_eflash_redun0_addr_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_redun1_0_data_t.cfg_eflash_redun1_en_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_redun1_0_data_t.cfg_eflash_redun1_en_0.
 */
static inline void hal_cfg_eflash_redun1_0_set_cfg_eflash_redun1_en_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_redun1_0_data_t redun1_0;
    redun1_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun1;
    redun1_0.b.cfg_eflash_redun1_en_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->redun1 = redun1_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_redun0_0_data_t.cfg_eflash_redun0_addr_0.
 */
static inline uint32_t hal_cfg_eflash_redun1_0_get_cfg_eflash_redun1_en_0(eflash_region_t bus)
{
    cfg_eflash_redun1_0_data_t redun1_0;
    redun1_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun1;
    return redun1_0.b.cfg_eflash_redun1_en_0;
}

/**
 * @brief  Set the value of @ref cfg_eflash_redun1_0_data_t.cfg_eflash_redun1_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref cfg_eflash_redun1_0_data_t.cfg_eflash_redun1_addr_0.
 */
static inline void hal_cfg_eflash_redun1_0_set_cfg_eflash_redun1_addr_0(eflash_region_t bus, uint32_t val)
{
    cfg_eflash_redun1_0_data_t redun1_0;
    redun1_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun1;
    redun1_0.b.cfg_eflash_redun1_addr_0 = val;
    ((eflash_regs_t *)g_eflash_regs[bus])->redun1 = redun1_0.d32;
}

/**
 * @brief  Get the value of @ref cfg_eflash_redun1_0_data_t.cfg_eflash_redun1_addr_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref cfg_eflash_redun1_0_data_t.cfg_eflash_redun1_addr_0.
 */
static inline uint32_t hal_cfg_eflash_redun1_0_get_cfg_eflash_redun1_addr_0(eflash_region_t bus)
{
    cfg_eflash_redun1_0_data_t redun1_0;
    redun1_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->redun1;
    return redun1_0.b.cfg_eflash_redun1_addr_0;
}

/**
 * @brief  Get the value of @ref eflash_cache_valid_0_data_t.cache_valid_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_cache_valid_0_data_t.cache_valid_0.
 */
static inline uint32_t hal_eflash_cache_valid_0_get_cache_valid_0(eflash_region_t bus)
{
    eflash_cache_valid_0_data_t cache_valid_0;
    cache_valid_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->cache_valid;
    return cache_valid_0.b.cache_valid_0;
}

/**
 * @brief  Get the value of @ref eflash_cache_valid_0_data_t.cache_addr_h_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_cache_valid_0_data_t.cache_addr_h_0.
 */
static inline uint32_t hal_eflash_cache_valid_0_get_cache_addr_h_0(eflash_region_t bus)
{
    eflash_cache_valid_0_data_t cache_valid_0;
    cache_valid_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->cache_valid;
    return cache_valid_0.b.cache_addr_h_0;
}

/**
 * @brief  Get the value of @ref eflash_cache_addr_l_0_data_t.cache_addr_l_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_cache_addr_l_0_data_t.cache_addr_l_0.
 */
static inline uint32_t hal_eflash_cache_addr_l_0_get_cache_addr_l_0(eflash_region_t bus)
{
    eflash_cache_addr_l_0_data_t cache_addr_0;
    cache_addr_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->cache_addr;
    return cache_addr_0.b.cache_addr_l_0;
}

/**
 * @brief  Get the value of @ref eflash_erase_confen_state_0_data_t.chip_erasing_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_erase_confen_state_0_data_t.chip_erasing_0.
 */
static inline uint32_t hal_eflash_erase_confen_state_0_get_chip_erasing_0(eflash_region_t bus)
{
    eflash_erase_confen_state_0_data_t erase_confen_state_0;
    erase_confen_state_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_confen_state;
    return erase_confen_state_0.b.chip_erasing_0;
}

/**
 * @brief  Get the value of @ref eflash_erase_confen_state_0_data_t.sector_erasing_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_erase_confen_state_0_data_t.sector_erasing_0.
 */
static inline uint32_t hal_eflash_erase_confen_state_0_get_sector_erasing_0(eflash_region_t bus)
{
    eflash_erase_confen_state_0_data_t erase_confen_state_0;
    erase_confen_state_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_confen_state;
    return erase_confen_state_0.b.sector_erasing_0;
}

/**
 * @brief  Get the value of @ref eflash_erase_confen_state_0_data_t.confen_writing_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @return The value of @ref eflash_erase_confen_state_0_data_t.confen_writing_0.
 */
static inline uint32_t hal_eflash_erase_confen_state_0_get_confen_writing_0(eflash_region_t bus)
{
    eflash_erase_confen_state_0_data_t erase_confen_state_0;
    erase_confen_state_0.d32 = ((eflash_regs_t *)g_eflash_regs[bus])->erase_confen_state;
    return erase_confen_state_0.b.confen_writing_0;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_sel_data_t.cfg_eflash0_por_dbg_sel.
 * @param  [in]  val The value of @ref eflash_por_dpd_sel_data_t.cfg_eflash0_por_dbg_sel.
 */
static inline void hal_eflash_por_dpd_sel_set_cfg_eflash0_por_dbg_sel(uint32_t val)
{
    eflash_por_dpd_sel_data_t eflash_dpd_sel;
    eflash_dpd_sel.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel;
    eflash_dpd_sel.b.cfg_eflash0_por_dbg_sel = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel = eflash_dpd_sel.d32;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_sel_data_t.cfg_eflash0_dpd_dbg_sel.
 * @param  [in]  val The value of @ref eflash_por_dpd_sel_data_t.cfg_eflash0_dpd_dbg_sel.
 */
static inline void hal_eflash_por_dpd_sel_set_cfg_eflash0_dpd_dbg_sel(uint32_t val)
{
    eflash_por_dpd_sel_data_t eflash_dpd_sel;
    eflash_dpd_sel.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel;
    eflash_dpd_sel.b.cfg_eflash0_dpd_dbg_sel = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel = eflash_dpd_sel.d32;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_sel_data_t.cfg_eflash1_por_dbg_sel.
 * @param  [in]  val The value of @ref eflash_por_dpd_sel_data_t.cfg_eflash1_por_dbg_sel.
 */
static inline void hal_eflash_por_dpd_sel_set_cfg_eflash1_por_dbg_sel(uint32_t val)
{
    eflash_por_dpd_sel_data_t eflash_dpd_sel;
    eflash_dpd_sel.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel;
    eflash_dpd_sel.b.cfg_eflash1_por_dbg_sel = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel = eflash_dpd_sel.d32;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_sel_data_t.cfg_eflash1_dpd_dbg_sel.
 * @param  [in]  val The value of @ref eflash_por_dpd_sel_data_t.cfg_eflash1_dpd_dbg_sel.
 */
static inline void hal_eflash_por_dpd_sel_set_cfg_eflash1_dpd_dbg_sel(uint32_t val)
{
    eflash_por_dpd_sel_data_t eflash_dpd_sel;
    eflash_dpd_sel.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel;
    eflash_dpd_sel.b.cfg_eflash1_dpd_dbg_sel = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_sel = eflash_dpd_sel.d32;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_man_data_t.cfg_eflash0_por_dbg.
 * @param  [in]  val The value of @ref eflash_por_dpd_man_data_t.cfg_eflash0_por_dbg.
 */
static inline void hal_eflash_por_dpd_man_set_cfg_eflash0_por_dbg(uint32_t val)
{
    eflash_por_dpd_man_data_t eflash_dpd_man;
    eflash_dpd_man.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man;
    eflash_dpd_man.b.cfg_eflash0_por_dbg = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man = eflash_dpd_man.d32;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_man_data_t.cfg_eflash0_dpd_dbg.
 * @param  [in]  val The value of @ref eflash_por_dpd_man_data_t.cfg_eflash0_dpd_dbg.
 */
static inline void hal_eflash_por_dpd_man_set_cfg_eflash0_dpd_dbg(uint32_t val)
{
    eflash_por_dpd_man_data_t eflash_dpd_man;
    eflash_dpd_man.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man;
    eflash_dpd_man.b.cfg_eflash0_dpd_dbg = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man = eflash_dpd_man.d32;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_man_data_t.cfg_eflash1_por_dbg.
 * @param  [in]  val The value of @ref eflash_por_dpd_man_data_t.cfg_eflash1_por_dbg.
 */
static inline void hal_eflash_por_dpd_man_set_cfg_eflash1_por_dbg(uint32_t val)
{
    eflash_por_dpd_man_data_t eflash_dpd_man;
    eflash_dpd_man.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man;
    eflash_dpd_man.b.cfg_eflash1_por_dbg = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man = eflash_dpd_man.d32;
}

/**
 * @brief  Set the value of @ref eflash_por_dpd_man_data_t.cfg_eflash1_dpd_dbg.
 * @param  [in]  val The value of @ref eflash_por_dpd_man_data_t.cfg_eflash1_dpd_dbg.
 */
static inline void hal_eflash_por_dpd_man_set_cfg_eflash1_dpd_dbg(uint32_t val)
{
    eflash_por_dpd_man_data_t eflash_dpd_man;
    eflash_dpd_man.d32 = ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man;
    eflash_dpd_man.b.cfg_eflash1_dpd_dbg = val;
    ((eflash_low_power_regs_t *)g_eflash_por_dpd_regs)->dpd_man = eflash_dpd_man.d32;
}

/**
 * @brief  Set the value of @ref xip_cache_en_0_data_t.xip_cache_en.
 * @param  [in]  val The value of @ref xip_cache_en_0_data_t.xip_cache_en.
 */
static inline void hal_xip_cache_en_set_xip_cache_en(uint32_t val)
{
    xip_cache_en_0_data_t xip_cache_ctl;
    xip_cache_ctl.d32 = ((xip_cache_regs_t *)g_xip_cache_regs)->xip_cache_en;
    xip_cache_ctl.b.xip_cache_en = val;
    ((xip_cache_regs_t *)g_xip_cache_regs)->xip_cache_en = xip_cache_ctl.d32;
}

/**
 * @brief  Set the value of @ref man_all_0_data_t.man_all_req.
 * @param  [in]  val The value of @ref man_all_0_data_t.man_all_req.
 */
static inline void hal_man_all_set_man_all_req(uint32_t val)
{
    man_all_0_data_t man_all;
    man_all.d32 = ((xip_cache_man_regs_t *)g_xip_cache_man_all_regs)->man_all;
    man_all.b.man_all_req = val;
    ((xip_cache_man_regs_t *)g_xip_cache_man_all_regs)->man_all = man_all.d32;
}

/**
 * @brief  Get the value of @ref man_all_0_data_t.man_all_done.
 * @return The value of @ref man_all_0_data_t.man_all_done.
 */
static inline uint32_t hal_man_all_get_man_all_done(void)
{
    man_all_0_data_t man_all;
    man_all.d32 = ((xip_cache_man_regs_t *)g_xip_cache_man_all_regs)->man_all;
    return man_all.b.man_all_done;
}

/**
 * @brief  Set the value of @ref stb_eflash_mux_data_t.cfg_stb_eflash0_test_en.
 * @param  [in]  val The value of @ref stb_eflash_mux_data_t.cfg_stb_eflash0_test_en.
 */
static inline void hal_stb_eflash_mux_set_cfg_stb_eflash0_test_en(uint32_t val)
{
    stb_eflash_mux_data_t mux;
    mux.d32 = ((stb_eflash_mux_regs_t *)g_eflash_stb_mux_regs)->stb_mux;
    mux.b.cfg_stb_eflash0_test_en = val;
    ((stb_eflash_mux_regs_t *)g_eflash_stb_mux_regs)->stb_mux = mux.d32;
}

/**
 * @brief  Set the value of @ref stb_eflash_mux_data_t.cfg_stb_eflash1_test_en.
 * @param  [in]  val The value of @ref stb_eflash_mux_data_t.cfg_stb_eflash1_test_en.
 */
static inline void hal_stb_eflash_mux_set_cfg_stb_eflash1_test_en(uint32_t val)
{
    stb_eflash_mux_data_t mux;
    mux.d32 = ((stb_eflash_mux_regs_t *)g_eflash_stb_mux_regs)->stb_mux;
    mux.b.cfg_stb_eflash1_test_en = val;
    ((stb_eflash_mux_regs_t *)g_eflash_stb_mux_regs)->stb_mux = mux.d32;
}

/**
 * @brief  Set the value of @ref eflash_cfg_0_data_t.eflash0_cfg_0.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_cfg_0_data_t.eflash0_cfg_0.
 */
static inline void hal_eflash0_cfg_0_set_eflash0_cfg_0(eflash_region_t bus, uint32_t val)
{
    eflash_cfg_0_data_t eflash_cfg_0;
    eflash_cfg_0.d32 = ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_0;
    eflash_cfg_0.b.eflash0_cfg_0 = val;
    ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_0 = eflash_cfg_0.d32;
}

/**
 * @brief  Set the value of @ref eflash_cfg_1_data_t.cfg_eflash_erase_verify_en.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_cfg_1_data_t.cfg_eflash_erase_verify_en.
 */
static inline void hal_eflash0_cfg_1_set_cfg_eflash_erase_verify_en(eflash_region_t bus, uint32_t val)
{
    eflash_cfg_1_data_t eflash_cfg_1;
    eflash_cfg_1.d32 = ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_1;
    eflash_cfg_1.b.cfg_eflash_erase_verify_en = val;
    ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_1 = eflash_cfg_1.d32;
}

/**
 * @brief  Set the value of @ref eflash_cfg_1_data_t.cfg_eflash_preprogram.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_cfg_1_data_t.cfg_eflash_preprogram.
 */
static inline void hal_eflash0_cfg_1_set_cfg_eflash_preprogram(eflash_region_t bus, uint32_t val)
{
    eflash_cfg_1_data_t eflash_cfg_1;
    eflash_cfg_1.d32 = ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_1;
    eflash_cfg_1.b.cfg_eflash_preprogram = val;
    ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_1 = eflash_cfg_1.d32;
}

/**
 * @brief  Set the value of @ref eflash_cfg_2_data_t.eflash0_cfg_2.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_cfg_2_data_t.eflash0_cfg_2.
 */
static inline void hal_eflash0_cfg_2_set_eflash0_cfg_2(eflash_region_t bus, uint32_t val)
{
    eflash_cfg_2_data_t eflash_cfg_2;
    eflash_cfg_2.d32 = ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_2;
    eflash_cfg_2.b.eflash0_cfg_2 = val;
    ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_2 = eflash_cfg_2.d32;
}

/**
 * @brief  Set the value of @ref eflash_cfg_3_data_t.eflash0_cfg_3.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_cfg_3_data_t.eflash0_cfg_3.
 */
static inline void hal_eflash0_cfg_3_set_eflash0_cfg_3(eflash_region_t bus, uint32_t val)
{
    eflash_cfg_3_data_t eflash_cfg_3;
    eflash_cfg_3.d32 = ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_3;
    eflash_cfg_3.b.eflash0_cfg_3 = val;
    ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_3 = eflash_cfg_3.d32;
}

/**
 * @brief  Set the value of @ref eflash_cfg_4_data_t.eflash0_cfg_4.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_cfg_4_data_t.eflash0_cfg_4.
 */
static inline void hal_eflash0_cfg_4_set_eflash0_cfg_4(eflash_region_t bus, uint32_t val)
{
    eflash_cfg_4_data_t eflash_cfg_4;
    eflash_cfg_4.d32 = ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_4;
    eflash_cfg_4.b.eflash0_cfg_4 = val;
    ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_4 = eflash_cfg_4.d32;
}

/**
 * @brief  Set the value of @ref eflash_cfg_5_data_t.eflash0_cfg_5.
 * @param  [in]  bus The value of @ref eflash_region_t.
 * @param  [in]  val The value of @ref eflash_cfg_5_data_t.eflash0_cfg_5.
 */
static inline void hal_eflash0_cfg_5_set_eflash0_cfg_5(eflash_region_t bus, uint32_t val)
{
    eflash_cfg_5_data_t eflash_cfg_5;
    eflash_cfg_5.d32 = ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_5;
    eflash_cfg_5.b.eflash0_cfg_5 = val;
    ((eflash_l2_regs_t *)g_eflash_flash_cfg_regs[bus])->cfg_5 = eflash_cfg_5.d32;
}

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif