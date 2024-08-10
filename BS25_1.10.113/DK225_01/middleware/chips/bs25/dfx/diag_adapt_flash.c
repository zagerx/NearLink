/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: diag adapt flash
 * This file should be changed only infrequently and with great care.
 */
#include "diag_adapt_flash.h"
#include "debug_print.h"
#include "diag.h"
#include "dfx_adapt_layer.h"
#include "flash.h"
#include "memory_config_common.h"

#ifdef USE_EMBED_FLASH
#define LOG_FILE_FLASH_START_ADDRESS    (OFFLINE_LOG_FLASH_START - EMBED_FLASH_START)
#else
#define LOG_FILE_FLASH_START_ADDRESS    (OFFLINE_LOG_FLASH_START - 0x90000000)
#endif

/* flash 读写 */
int32_t diag_adapt_flash_read(uint8_t opt_type, uint32_t offset, uint8_t *buf, uint32_t size)
{
    uint32_t addr = offset;
#ifndef USE_EMBED_FLASH
    flash_id_t flash_id = CHIP_FLASH_ID;
    uint32_t flash_start_addr = FLASH_START;
#endif
    if (opt_type == FLASH_OP_TYPE_OTA) {
        addr += TRANSMIT_OTA_DATA_START;
    } else if (opt_type == FLASH_OP_TYPE_LOG_FILE) {
        addr += LOG_FILE_FLASH_START_ADDRESS;
    } else if (opt_type == FLASH_OP_TYPE_FLASH_DATA) {
#ifdef USE_EMBED_FLASH
        addr -= EMBED_FLASH_START;
#else
        flash_porting_switch_flash_id(&flash_id, &flash_start_addr, addr);
        addr -= flash_start_addr;
#endif
    } else {
        return -1;
    }
    uint32_t length = size;
    uint32_t irq = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_read(addr, (uint32_t *)buf, size) != ERRCODE_SUCC) {
        length = 0;
    }
#else
    if (opt_type == FLASH_OP_TYPE_FLASH_DATA) {
        if (flash_id == CHIP_FLASH_ID) {
            if (memcpy_s(buf, size, (uint8_t *)(uintptr_t)(addr + flash_start_addr), size) != EOK) { length = 0; }
        } else {
            length = uapi_flash_read_data(flash_id, addr, buf, size);
        }
    } else {
        length = uapi_flash_read_data(flash_id, addr, buf, size);
    }
#endif
    osal_irq_restore(irq);
    return (int32_t)length;
}

int32_t diag_adapt_flash_write(uint8_t opt_type, uint32_t offset, const uint8_t *buf, uint32_t size,
    bool do_erase)
{
    unused(do_erase);
    uint32_t addr = offset;
    if (opt_type == FLASH_OP_TYPE_OTA) {
        addr += TRANSMIT_OTA_DATA_START;
    } else if (opt_type == FLASH_OP_TYPE_LOG_FILE) {
        addr += LOG_FILE_FLASH_START_ADDRESS;
    } else {
        return -1;
    }

    uint32_t length = size;
    uint32_t irq = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_write(addr, (uint32_t *)buf, size) != ERRCODE_SUCC) {
        length = 0;
    }
#else
    length = uapi_flash_write_data(FLASH_0, addr, buf, size);
#endif
    osal_irq_restore(irq);
    return (int32_t)length;
}

int32_t diag_adapt_flash_info_write(uint8_t opt_type, uint32_t offset, const uint8_t *buf, uint32_t size,
    bool do_erase)
{
    unused(do_erase);
    unused(buf);
    uint32_t addr = offset;
    if (opt_type == FLASH_OP_TYPE_OTA) {
        addr += 0;
    } else if (opt_type == FLASH_OP_TYPE_LOG_FILE) {
        addr += LOG_FILE_FLASH_START_ADDRESS;
    } else {
        return -1;
    }
    uint32_t length = size;
    uint32_t irq = osal_irq_lock();
#ifndef NO_EXTERN_FLASH
    length = uapi_flash_write_data(FLASH_0, addr, buf, size);
#endif
    osal_irq_restore(irq);
    return (int32_t)length;
}

/*
 * flash擦除数据接口
 */
errcode_t diag_adapt_flash_erase(uint8_t opt_type, const uint32_t offset, const uint32_t size)
{
    errcode_t ret = ERRCODE_SUCC;
    uint32_t addr = offset;
    if (opt_type == FLASH_OP_TYPE_OTA) {
        addr += TRANSMIT_OTA_DATA_START;
    } else if (opt_type == FLASH_OP_TYPE_LOG_FILE) {
        addr += LOG_FILE_FLASH_START_ADDRESS;
    } else {
        return ERRCODE_FAIL;
    }

    uint32_t irq = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    ret = uapi_eflash_erase(addr, size);
#else
    ret = uapi_flash_sector_erase(FLASH_0, addr, size);
#endif
    osal_irq_restore(irq);
    return ret;
}