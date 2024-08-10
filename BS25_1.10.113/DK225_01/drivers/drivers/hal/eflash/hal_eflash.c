/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides HAL eflash \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-08， Create file. \n
 */
#include "common_def.h"
#include "hal_eflash.h"

uintptr_t g_eflash_regs[EMBED_FLASH_REGION_MAX_NUM] = { 0 };
uintptr_t g_xip_cache_regs = NULL;
uintptr_t g_xip_cache_man_all_regs = NULL;
uintptr_t g_eflash_stb_mux_regs = NULL;
uintptr_t g_eflash_por_dpd_regs = NULL;
hal_eflash_funcs_t *g_hal_eflash_funcs[EMBED_FLASH_REGION_MAX_NUM] = { 0 };
uintptr_t g_eflash_nvr_cfg_regs[EMBED_FLASH_REGION_MAX_NUM] = { 0 };
uintptr_t g_eflash_flash_cfg_regs[EMBED_FLASH_REGION_MAX_NUM] = { 0 };

errcode_t hal_eflash_regs_init(eflash_region_t region)
{
    if (eflash_porting_base_addr_get(region) == 0 || g_xip_cache_regs_base_addr == 0 ||
        g_xip_cache_man_all_regs_base_addr == 0 || g_eflash_stb_mux_regs_base_addr == 0 ||
        g_eflash_por_dpd_regs_base_addr == 0) {
        return ERRCODE_EFLASH_REG_ADDR_INVALID;
    }
    g_eflash_regs[region] = eflash_porting_base_addr_get(region);
    g_xip_cache_regs = g_xip_cache_regs_base_addr;
    g_xip_cache_man_all_regs = g_xip_cache_man_all_regs_base_addr;
    g_eflash_stb_mux_regs = g_eflash_stb_mux_regs_base_addr;
    g_eflash_por_dpd_regs = g_eflash_por_dpd_regs_base_addr;
    g_eflash_nvr_cfg_regs[region] = eflash_porting_nvr_cfg_addr_get(region);
    g_eflash_flash_cfg_regs[region] = eflash_porting_flash_cfg_addr_get(region);

    return ERRCODE_SUCC;
}

void hal_eflash_regs_deinit(eflash_region_t region)
{
    g_eflash_regs[region] = 0;
    g_xip_cache_regs = 0;
    g_xip_cache_man_all_regs = 0;
    g_eflash_stb_mux_regs = 0;
    g_eflash_por_dpd_regs = 0;
    g_eflash_nvr_cfg_regs[region] = 0;
    g_eflash_flash_cfg_regs[region] = 0;
}

errcode_t hal_eflash_register_funcs(eflash_region_t region, hal_eflash_funcs_t *funcs)
{
    if (region >= EMBED_FLASH_REGION_MAX || funcs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    g_hal_eflash_funcs[region] = funcs;
    return ERRCODE_SUCC;
}

errcode_t hal_eflash_unregister_funcs(eflash_region_t region)
{
    if (region >= EMBED_FLASH_REGION_MAX) {
        return ERRCODE_INVALID_PARAM;
    }
    g_hal_eflash_funcs[region] = NULL;
    return ERRCODE_SUCC;
}

hal_eflash_funcs_t *hal_eflash_get_funcs(eflash_region_t region)
{
    if (region >= EMBED_FLASH_REGION_MAX) {
        return NULL;
    }
    return g_hal_eflash_funcs[region];
}