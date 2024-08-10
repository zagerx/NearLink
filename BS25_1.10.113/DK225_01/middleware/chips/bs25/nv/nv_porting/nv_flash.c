/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: NV on different flash
 */

#include "nv_porting.h"
#include "eflash.h"
#include "flash.h"
#include "soc_osal.h"
#include "uapi_crc.h"
#include "memory_config_common.h"

#define FLASH_WORD_ALIGN 4

static uint32_t flash_read_data_align(const uint32_t *addr, uint32_t *length, uint8_t *first_cover_count,
                                      uint8_t *last_cover_count)
{
    uint32_t dest_addr;
    /* Check if the address is 4 bytes aligned. */
    if (((*addr) & 0x3) != 0) {
        /* The actual send address requires 4 byte alignment. */
        dest_addr = ((*addr) & 0xFFFFFFFC);

        /* Fill the first 4 bytes of data. */
        *first_cover_count = (*addr) & 0x3;
        (*length) += (*first_cover_count);
    } else {
        dest_addr = (*addr);
    }
    /* Check if the last 4 bytes are aligned. */
    if (((*length) & 0x3) != 0) {
        /* Fill the first 4 bytes of data. */
        *last_cover_count = FLASH_WORD_ALIGN - (uint8_t)((*length) & 0x3);
        (*length) += (*last_cover_count);
    }
    return dest_addr;
}

errcode_t kv_flash_read(const uint32_t flash_offset, const uint32_t size, uint8_t *ram_data)
{
    errcode_t ret = ERRCODE_SUCC;
    uint32_t read_size = size;
    uint8_t first_cover_count = 0;
    uint8_t last_cover_count = 0;
    uint32_t dest_addr = flash_read_data_align(&flash_offset, &read_size, &first_cover_count, &last_cover_count);
    uint8_t *align_read_buff = (uint8_t *)kv_malloc(read_size);
    if (align_read_buff == NULL) { return ERRCODE_MALLOC; }
    uint32_t mstatus = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_read(dest_addr, (uint32_t *)align_read_buff, read_size) != ERRCODE_SUCC) {
#else
    if (uapi_flash_read_data(0, dest_addr, (uint8_t *)align_read_buff, read_size) != read_size) {
#endif
        ret = ERRCODE_FAIL;
    }
    osal_irq_restore(mstatus);
    if (memcpy_s((uint8_t *)ram_data, size, (uint8_t *)(align_read_buff + first_cover_count), size) != EOK) {
        ret = ERRCODE_FAIL;
    }

    kv_free(align_read_buff);
    return ret;
}

errcode_t kv_flash_write(const uint32_t flash_offset, uint32_t size, const uint8_t *ram_data, bool do_erase)
{
    (void)do_erase;
    errcode_t ret = ERRCODE_SUCC;

    uint32_t write_size = size;
    uint8_t first_cover_count = 0;
    uint8_t last_cover_count = 0;
    uint32_t dest_addr = flash_read_data_align(&flash_offset, &write_size, &first_cover_count, &last_cover_count);

    uint8_t *align_write_buff = (uint8_t *)kv_malloc(write_size);
    if (align_write_buff == NULL) { return ERRCODE_MALLOC; }
    uint32_t mstatus = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_read(dest_addr, (uint32_t *)align_write_buff, write_size) != ERRCODE_SUCC) {
#else
    if (uapi_flash_read_data(0, dest_addr, (uint8_t *)align_write_buff, write_size) != write_size) {
#endif
        ret = ERRCODE_FAIL;
    }
    osal_irq_restore(mstatus);
    if (memcpy_s((uint8_t *)(align_write_buff + first_cover_count), write_size, (uint8_t *)ram_data, size) != EOK) {
        ret = ERRCODE_FAIL;
    }

    mstatus = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_write(dest_addr, (uint32_t*)align_write_buff, write_size) != ERRCODE_SUCC) {
#else
    if (uapi_flash_write_data(0, dest_addr, (uint8_t*)align_write_buff, write_size) != write_size) {
#endif
        ret = ERRCODE_FAIL;
    }
    osal_irq_restore(mstatus);

    kv_free(align_write_buff);
    return ret;
}

errcode_t kv_flash_erase(const uint32_t flash_offset, uint32_t size)
{
    UNUSED(size);
    errcode_t ret = ERRCODE_SUCC;
    uint32_t mstatus = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_erase(flash_offset, FLASH_PAGE_SIZE) != ERRCODE_SUCC) {
#else
    if (uapi_flash_block_erase(0, flash_offset, FLASH_PAGE_SIZE, true) != ERRCODE_SUCC) {
#endif
        ret = ERRCODE_FAIL;
    }
    osal_irq_restore(mstatus);
    return ret;
}