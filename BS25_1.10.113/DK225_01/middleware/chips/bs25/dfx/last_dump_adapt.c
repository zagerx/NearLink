/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: last dump
 * This file should be changed only infrequently and with great care.
 */
#include "diag.h"
#include "last_dump.h"
#include "last_dump_st.h"
#include "memory_config.h"
#include "securec.h"
#include "debug_print.h"
#if (DUMP_USE_EMBED_FLASH == 1)
#include "eflash.h"
#else
#include "flash.h"
#endif
#if defined(CONFIG_WATCHDOG_SUPPORT_ULP_WDT) && (CONFIG_WATCHDOG_SUPPORT_ULP_WDT == 1)
#include "watchdog_porting.h"
#endif
#include "last_dump_adapt.h"

#define FLASH_DUMP_MAX_COUNT 50

typedef struct {
    char *name;
    uintptr_t start_addr;
    uintptr_t end_addr;
} diag_reg_dump_t;

typedef struct {
    char *name;
    uintptr_t start_addr;
    uint32_t len;
} diag_mem_dump_t;

typedef struct {
    uintptr_t addr;
    uint32_t len;
} flash_mem_dump_t;

#ifdef DUMP_REG_SUPPORT
static diag_reg_dump_t g_reg_dump_info[] = {
    { "PMU1_CTL", 0x57004000, 0x570047e0 },      // PMU1_CTL
    { "PMU2_CTL", 0x57008000, 0x57008490 },      // PMU2_CTL
    { "ULP_AON_CTL", 0x5702c000, 0x5702c418 },   // ULP_AON_CTL
};

static void dfx_dump_reg(void)
{
    uint16_t count = sizeof(g_reg_dump_info) / sizeof(diag_reg_dump_t);
    for (uint8_t i = 0; i < count; i++) {
        dfx_last_dump_data(g_reg_dump_info[i].name, g_reg_dump_info[i].start_addr,
                           g_reg_dump_info[i].end_addr - g_reg_dump_info[i].start_addr);
    }
}
#endif /* DUMP_REG_SUPPORT */

#ifdef DUMP_MEM_SUPPORT
static diag_mem_dump_t g_mem_dump_info[] = {
    { "SHARED_MEM", SHARED_MEM_START, SHARED_MEM_LENGTH },
    { "APP_ITCM_ORIGIN", APP_ITCM_ORIGIN, APP_ITCM_LENGTH },
    { "APP_DTCM_ORIGIN", APP_DTCM_ORIGIN, APP_DTCM_LENGTH },
    { "MCPU_TRACE_MEM_REGION", MCPU_TRACE_MEM_REGION_START, CPU_TRACE_MEM_REGION_LENGTH },
    { "BT_RAM_ORIGIN_APP_MAPPING", BT_RAM_ORIGIN_APP_MAPPING, BT_RAM_ORIGIN_APP_MAPPING_LENGTH },
    { "BCPU_TRACE_MEM_REGION", BCPU_TRACE_MEM_REGION_START, CPU_TRACE_MEM_REGION_LENGTH },
    { "BT_EM_MAP_MEMORY", 0x59410000, 64 * 1024 }, // 0x59410000 蓝牙em地址，一共64 * 1024 (64K)
};

static diag_mem_dump_t g_flash_dump_info[] = {
    { "SHARED_MEM", SHARED_MEM_START, SHARED_MEM_LENGTH },
    { "MCPU_TRACE_MEM_REGION", MCPU_TRACE_MEM_REGION_START, CPU_TRACE_MEM_REGION_LENGTH },
    { "BCPU_TRACE_MEM_REGION", BCPU_TRACE_MEM_REGION_START, CPU_TRACE_MEM_REGION_LENGTH },
};

static void dfx_dump_mem(void)
{
    uint16_t count = (uint16_t)(sizeof(g_mem_dump_info) / sizeof(diag_mem_dump_t));
    for (uint8_t i = 0; i < count; i++) {
#if defined(CONFIG_WATCHDOG_SUPPORT_ULP_WDT) && (CONFIG_WATCHDOG_SUPPORT_ULP_WDT == 1)
        ulp_wdt_kick();
#endif
        dfx_last_dump_data(g_mem_dump_info[i].name, g_mem_dump_info[i].start_addr,
                           g_mem_dump_info[i].len);
    }
}
#endif /* DUMP_MEM_SUPPORT */

void dfx_last_dump(void)
{
    uint32_t file_num = 0;
#ifdef DUMP_MEM_SUPPORT
    file_num += (uint32_t)(sizeof(g_mem_dump_info) / sizeof(diag_mem_dump_t));
#endif
#ifdef DUMP_REG_SUPPORT
    file_num += (sizeof(g_reg_dump_info) / sizeof(diag_reg_dump_t));
#endif

#if (defined(DUMP_MEM_SUPPORT) || defined(DUMP_REG_SUPPORT))
    dfx_last_dump_start(file_num);
#endif

#ifdef DUMP_MEM_SUPPORT
    dfx_dump_mem();
#endif
#ifdef DUMP_REG_SUPPORT
    dfx_dump_reg();
#endif

#if (defined(DUMP_MEM_SUPPORT) || defined(DUMP_REG_SUPPORT))
    dfx_last_dump_end(file_num);
#endif
    unused(file_num);
}

static errcode_t dump_flash_erase(uint32_t addr, uint32_t len)
{
#if (DUMP_USE_EMBED_FLASH == 1)
    return uapi_eflash_erase(addr - EMBED_FLASH_START, len);
#else
    uint32_t ret = ERRCODE_FAIL;
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
    ret = uapi_flash_block_erase(FLASH_1, addr - FLASH_START, len, true);
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_1);
#endif
    return ret;
#endif
}

void dfx_last_dump2flash(uint32_t flash_dump_addr, uint32_t flash_dump_size)
{
    uint32_t dump_addr = flash_dump_addr;
    uint32_t unused_dump_size = flash_dump_size;
    flash_mem_dump_t *dump_info_addr = (flash_mem_dump_t *)(uintptr_t)dump_addr;

    if (dump_flash_erase(flash_dump_addr, flash_dump_size) != ERRCODE_SUCC) {
        return;
    }

    dump_addr += FLASH_DUMP_MAX_COUNT * (uint32_t)sizeof(flash_mem_dump_t);
    unused_dump_size -= FLASH_DUMP_MAX_COUNT * (uint32_t)sizeof(flash_mem_dump_t);
#ifdef DUMP_MEM_SUPPORT
    PRINT("DUMP TO FLASH START.\r\n");
    uint16_t count = (uint16_t)(sizeof(g_flash_dump_info) / sizeof(diag_mem_dump_t));
    if (count > FLASH_DUMP_MAX_COUNT) {
        return;
    }
    for (uint8_t i = 0; i < count; i++) {
        if (g_flash_dump_info[i].len > unused_dump_size) {
            break;
        }
#if defined(CONFIG_WATCHDOG_SUPPORT_ULP_WDT) && (CONFIG_WATCHDOG_SUPPORT_ULP_WDT == 1)
        ulp_wdt_kick();
#endif
#if (DUMP_USE_EMBED_FLASH == 1)
        dump_info_addr[i].addr = dump_addr;
        dump_info_addr[i].len = g_flash_dump_info[i].len;
        PRINT("DUMP ADDR=0x%x, SIZE=0x%x (%s).\r\n", dump_addr, g_flash_dump_info[i].len, g_flash_dump_info[i].name);
        (void)memcpy_s((void *)dump_addr, unused_dump_size,
                       (void *)g_flash_dump_info[i].start_addr, g_flash_dump_info[i].len);
#else
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
        uapi_flash_write_data(FLASH_1, ((uint32_t)(uintptr_t)(&(dump_info_addr[i].addr)) - FLASH_START),
                              (uint8_t *)&dump_addr, sizeof(dump_addr));
        uapi_flash_write_data(FLASH_1, ((uint32_t)(uintptr_t)(&(dump_info_addr[i].len)) - FLASH_START),
                              (uint8_t *)&(g_flash_dump_info[i].len), sizeof(g_flash_dump_info[i].len));
        PRINT("DUMP ADDR=0x%x, SIZE=0x%x (%s).\r\n", dump_addr, g_flash_dump_info[i].len, g_flash_dump_info[i].name);
        uapi_flash_write_data(FLASH_1, dump_addr - FLASH_START, (uint8_t *)g_flash_dump_info[i].start_addr,
                              g_flash_dump_info[i].len);
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_switch_to_xip_mode(FLASH_1);
#endif
#endif
        dump_addr += g_flash_dump_info[i].len;
        unused_dump_size -= g_flash_dump_info[i].len;
    }
    PRINT("DUMP TO FLASH END.\r\n");
#endif
}