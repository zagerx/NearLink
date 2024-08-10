/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides eflash port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-15， Create file. \n
 */
#include <common_def.h>
#include "tcxo.h"
#include "soc_osal.h"
#include "hal_eflash_v150.h"
#include "eflash_porting.h"

eflash_adapter_t g_eflash_adapter[EMBED_FLASH_REGION_MAX_NUM] = {
    { EMBED_FLASH_REGION_0_BASE_ADDR, EMBED_FLASH_REGION_LENGTH,
      EMBED_FLASH_SECTOR_LENGTH, EMBED_FLASH_SECTOR_MAX },
    { EMBED_FLASH_REGION_1_BASE_ADDR, EMBED_FLASH_REGION_LENGTH,
      EMBED_FLASH_SECTOR_LENGTH, EMBED_FLASH_SECTOR_MAX },
};

static uintptr_t g_eflash_base_addrs[EMBED_FLASH_REGION_MAX] = {
    (uintptr_t)EMBED_FLASH_0_TIMING_REG,
    (uintptr_t)EMBED_FLASH_1_TIMING_REG
};

static uintptr_t g_eflash_nvr_cfg_addrs[EMBED_FLASH_REGION_MAX] = {
    EMBED_FLASH_0_NVR_CFG,
    EMBED_FLASH_1_NVR_CFG
};

static uintptr_t g_eflash_flash_cfg_addrs[EMBED_FLASH_REGION_MAX] = {
    EMBED_FLASH_0_CFG,
    EMBED_FLASH_1_CFG
};

uintptr_t g_xip_cache_regs_base_addr = XIP_CACHE_ENABLE_REG;
uintptr_t g_xip_cache_man_all_regs_base_addr = XIP_CACHE_MAN_ALL_REG;
uintptr_t g_eflash_stb_mux_regs_base_addr = EMBED_FLASH_STB_MUX;
uintptr_t g_eflash_por_dpd_regs_base_addr = EFLASH_POR_DPD_SEL;

uintptr_t eflash_porting_base_addr_get(eflash_region_t region)
{
    return g_eflash_base_addrs[region];
}

uintptr_t eflash_porting_nvr_cfg_addr_get(eflash_region_t region)
{
    return g_eflash_nvr_cfg_addrs[region];
}

uintptr_t eflash_porting_flash_cfg_addr_get(eflash_region_t region)
{
    return g_eflash_flash_cfg_addrs[region];
}


void eflash_port_register_hal_funcs(void)
{
    hal_eflash_register_funcs(EMBED_FLASH_0, hal_eflash_v150_funcs_get());
    hal_eflash_register_funcs(EMBED_FLASH_1, hal_eflash_v150_funcs_get());
}

void eflash_port_unregister_hal_funcs(void)
{
    hal_eflash_unregister_funcs(EMBED_FLASH_0);
    hal_eflash_unregister_funcs(EMBED_FLASH_1);
}

void eflash_port_context_init(eflash_context_t *ctxt)
{
    for (int i = 0; i < EMBED_FLASH_REGION_MAX_NUM; i++) {
        ctxt[i].base_addr = g_eflash_adapter[i].base;
        ctxt[i].length = g_eflash_adapter[i].flash_len;
        ctxt[i].sector_length = g_eflash_adapter[i].sector_len;
        ctxt[i].sector_num = g_eflash_adapter[i].sector_num;
    }
}

void eflash_port_delay_us(uint32_t time)
{
    uapi_tcxo_delay_us(time);
}

void eflash_porting_lock_create(void)
{
}

void eflash_porting_lock_delete(void)
{
}

uint32_t eflash_porting_lock(void)
{
    return osal_irq_lock();
}

void eflash_porting_unlock(uint32_t irq_status)
{
    osal_irq_restore(irq_status);
}
