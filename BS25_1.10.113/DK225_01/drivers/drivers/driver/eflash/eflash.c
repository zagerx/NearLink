/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides eflash driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-16, Create file. \n
 */

#include "osal_addr.h"
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
#include "soc_osal.h"
#include "securec.h"
#include "eflash.h"

#define BYTE_TO_WORD                        2
#define EFLASH_PREPROGRAM_WRITE_LENGTH_MAX  0x400
#define EFLASH_TIME_AFTER_DPD_PORB_CFG      5
#define EFLASH_TIME_BEFORE_ENTER_SLEEP      1
#define EFLASH_TIME_AFTER_EXIT_SLEEP        5
#define ADDR_ALIGN                          0x3
#define BIT_TO_FLASH_LENGTH                 12
#define BIT_TO_FLASH_NUM                    8
#define get_pre_word_addr(addr)             ((addr) & 0xfffffffc)

#define num_to_pre_word(addr)               ((addr) & 0x3)        // num of byte to previous word align of addr
#define num_to_next_word(addr)              (4 - ((addr) & 0x3))  // num of byte to next word align of addr
#define EFLASH_CACHE_SIZE                   40
#define WORD_WIDTH                          4
#define FLASH_WRITE_INVALID_VALUE           0xffffffff

static hal_eflash_funcs_t *g_hal_eflash_funcs = NULL;
static eflash_context_t g_eflash_context[EMBED_FLASH_REGION_MAX_NUM] = {
    { 0, 0, 0, 0, 0, CLOCK_32M },
    { 0, 0, 0, 0, 0, CLOCK_32M },
};

static errcode_t eflash_region_check(eflash_region_t region)
{
    if (unlikely(region >= EMBED_FLASH_REGION_MAX_NUM)) {
        return ERRCODE_INVALID_PARAM;
    }
    if (unlikely(!g_eflash_context[region].flash_is_initialised)) {
        return ERRCODE_EFLASH_NOT_INIT;
    }

    return ERRCODE_SUCC;
}

static bool eflash_addr_check(uint32_t addr, uint32_t len)
{
    if (addr + len < addr) {
        return false;
    }
    if ((addr + len) > (g_eflash_context[0].length + g_eflash_context[1].length)) {
        return false;
    }
    return true;
}

static eflash_region_t eflash_get_region(uint32_t addr)
{
    uint32_t addr_tmp = addr;
    uint8_t i = 0;
    addr_tmp += g_eflash_context[0].base_addr;
    for (i = 0; i < EMBED_FLASH_REGION_MAX_NUM; i++) {
        if ((addr_tmp >= g_eflash_context[i].base_addr) && \
            (addr_tmp < (g_eflash_context[i].base_addr + g_eflash_context[i].length))) {
            break;
        }
    }
    return (eflash_region_t)i;
}

errcode_t uapi_eflash_init(eflash_region_t region)
{
    if (region >= EMBED_FLASH_REGION_MAX_NUM) {
        return ERRCODE_INVALID_PARAM;
    }
    eflash_port_context_init(g_eflash_context);
    eflash_port_register_hal_funcs();
    g_hal_eflash_funcs = hal_eflash_get_funcs(region);
    if (g_eflash_context[region].flash_is_initialised == true) {
        return ERRCODE_SUCC;
    }
    errcode_t ret = g_hal_eflash_funcs->init(region);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#if defined(CONFIG_SET_NVR_FUNCTION)
    eflash_port_delay_us(EFLASH_TIME_AFTER_DPD_PORB_CFG);
    g_hal_eflash_funcs->set_nvr(region);
#endif /* CONFIG_SET_NVR_FUNCTION */
    eflash_porting_lock_create();
    g_eflash_context[region].flash_is_initialised = true;
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_deinit(eflash_region_t region)
{
    if (region >= EMBED_FLASH_REGION_MAX_NUM) {
        return ERRCODE_INVALID_PARAM;
    }
    if (g_eflash_context[region].flash_is_initialised == false) {
        return ERRCODE_SUCC;
    }
    g_hal_eflash_funcs->deinit(region);
    eflash_port_unregister_hal_funcs();
    eflash_porting_lock_delete();
    g_eflash_context[region].flash_is_initialised = false;
    return ERRCODE_SUCC;
}

void uapi_eflash_set_freq(eflash_region_t region, eflash_frequency_t freq)
{
    g_eflash_context[region].frequency = freq;
    g_hal_eflash_funcs->set_freq(region, freq);
}

static errcode_t eflash_read_word_align(uint32_t addr, uint32_t *value, uint32_t len)
{
    g_hal_eflash_funcs->read(addr + g_eflash_context[0].base_addr, value, len);
    return ERRCODE_SUCC;
}

static errcode_t eflash_read_previous_four_bytes(uint32_t addr, const uint32_t *value, uint32_t len)
{
    uint32_t last_four_bytes;
    eflash_read_word_align(get_pre_word_addr(addr), &last_four_bytes, WORD_WIDTH);
    uint8_t *temp = (uint8_t *)&last_four_bytes;
    if (memcpy_s((uint8_t *)value, len, temp + num_to_pre_word(addr), len) != EOK) {
        return ERRCODE_MEMCPY;
    }
    return ERRCODE_SUCC;
}

static errcode_t eflash_read_last_four_bytes(uint32_t addr, uint32_t *value, uint32_t len)
{
    uint32_t *read_value = value + (len >> BYTE_TO_WORD);
    uint32_t last_four_bytes;
    eflash_read_word_align(get_pre_word_addr(addr + len), &last_four_bytes, WORD_WIDTH);
    uint8_t read_size = num_to_pre_word(addr + len);
    if (memcpy_s((uint8_t *)read_value, read_size, &last_four_bytes, read_size) != EOK) {
        return ERRCODE_MEMCPY;
    }
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_read(uint32_t addr, uint32_t *value, uint32_t len)
{
    uint32_t len_tmp = len;
    uint32_t addr_tmp = addr;
    if ((!eflash_addr_check(addr_tmp, len_tmp) || (value == NULL))) {
        return ERRCODE_INVALID_PARAM;
    }
    errcode_t ret = ERRCODE_SUCC;
    uint32_t *read_value = value;
    uint32_t read_len = 0;
    uint32_t irq_sts = eflash_porting_lock();
    if (num_to_pre_word(addr_tmp) != 0) {
        read_len = len_tmp < num_to_next_word(addr_tmp) ? len_tmp : num_to_next_word(addr_tmp);
        ret = eflash_read_previous_four_bytes(addr_tmp, value, read_len);
        if (ret != ERRCODE_SUCC) {
            eflash_porting_unlock(irq_sts);
            return ret;
        }
        read_value = (uint32_t *)(uintptr_t)((uint32_t)(uintptr_t)read_value + read_len);
        len_tmp -= read_len;
        addr_tmp += read_len;
    }
    if (len_tmp > 0 && num_to_pre_word(addr_tmp + len_tmp) != 0) {
        ret = eflash_read_last_four_bytes(addr_tmp, read_value, len_tmp);
        if (ret != ERRCODE_SUCC) {
            eflash_porting_unlock(irq_sts);
            return ret;
        }
        len_tmp -= num_to_pre_word(addr_tmp + len_tmp);
    }
    if (len_tmp == 0) {
        eflash_porting_unlock(irq_sts);
        return ERRCODE_SUCC;
    }
    // now, addr and len is word align, but read_value is not sure.
    if (num_to_pre_word((uint32_t)(uintptr_t)read_value) == 0) {
        eflash_read_word_align(addr_tmp, read_value, len_tmp);
        eflash_porting_unlock(irq_sts);
        return ERRCODE_SUCC;
    }

    // if read_value is not word align, memcpy it into eflash_cache
    uint32_t *eflash_cache = (uint32_t *)osal_kmalloc_align(EFLASH_CACHE_SIZE, 0, WORD_WIDTH);
    if (eflash_cache == NULL) {
        eflash_porting_unlock(irq_sts);
        return ERRCODE_MALLOC;
    }
    while (len_tmp > 0) {
        read_len = len_tmp > EFLASH_CACHE_SIZE ? EFLASH_CACHE_SIZE : len_tmp;
        eflash_read_word_align(addr_tmp, eflash_cache, read_len);
        if (memcpy_s(read_value, len_tmp, eflash_cache, read_len) != EOK) {
            osal_kfree(eflash_cache);
            eflash_porting_unlock(irq_sts);
            return ERRCODE_MEMCPY;
        }
        read_value = (uint32_t *)(uintptr_t)((uint32_t)(uintptr_t)read_value + read_len);
        len_tmp -= read_len;
        addr_tmp += read_len;
    }
    osal_kfree(eflash_cache);
    eflash_porting_unlock(irq_sts);
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_chip_erase(eflash_region_t region)
{
    errcode_t ret = eflash_region_check(region);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    uint32_t irq_sts = eflash_porting_lock();
#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    if (memset_s((void *)g_eflash_context[region].base_addr, g_eflash_context[region].length,
        0xff, g_eflash_context[region].length) != EOK) {
        eflash_porting_unlock(irq_sts);
        return ERRCODE_FAIL;
    }
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
#if defined(CONFIG_REAL_EMBED_FLASH)
    g_hal_eflash_funcs->chip_erase(region);
#endif /* CONFIG_REAL_EMBED_FLASH */
    eflash_porting_unlock(irq_sts);
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_erase(uint32_t addr, uint32_t len)
{
    if (!eflash_addr_check(addr, len)) {
        return ERRCODE_INVALID_PARAM;
    }
    eflash_region_t begin = eflash_get_region(addr);
    eflash_region_t end = eflash_get_region(addr + len);
    uint32_t irq_sts = eflash_porting_lock();
    for (uint32_t i = begin; i <= end; i++) {
        g_hal_eflash_funcs->set_erase_verify((eflash_region_t)i);
    }

#if defined(CONFIG_VIRTUAL_EMBED_FLASH)
    if (memset_s((void *)(g_eflash_context[0].base_addr + addr), len, 0xff, len) != EOK) {
        eflash_porting_unlock(irq_sts);
        return ERRCODE_FAIL;
    }
#endif /* CONFIG_VIRTUAL_EMBED_FLASH */
#if defined(CONFIG_REAL_EMBED_FLASH)
    uint32_t sector_start_id = addr / g_eflash_context[0].sector_length;
    uint32_t sector_num = (addr + len) / g_eflash_context[0].sector_length;
    uint32_t sector_end_id = ((addr + len) % g_eflash_context[0].sector_length) != 0 ?
                             (sector_num + 1) : sector_num;
    for (uint32_t i = sector_start_id; i < sector_end_id; i++) {
        g_hal_eflash_funcs->sector_erase(i / g_eflash_context[0].sector_num, i % g_eflash_context[0].sector_num);
    }
#endif /* CONFIG_REAL_EMBED_FLASH */
    eflash_porting_unlock(irq_sts);
    return ERRCODE_SUCC;
}

static errcode_t eflash_write_word_align(uint32_t addr, const uint32_t *value, uint32_t len)
{
    eflash_region_t begin = eflash_get_region(addr);

    uint32_t one_process_len;
    uint32_t write_addr = addr;
    const uint32_t *write_value = value;
    uint32_t write_len = len;
    while (write_len > 0) {
        if (write_addr % EFLASH_PREPROGRAM_WRITE_LENGTH_MAX != 0) {
            one_process_len = (((write_addr % EFLASH_PREPROGRAM_WRITE_LENGTH_MAX) + write_len) <=
                EFLASH_PREPROGRAM_WRITE_LENGTH_MAX) ? write_len : (EFLASH_PREPROGRAM_WRITE_LENGTH_MAX - \
                (write_addr % EFLASH_PREPROGRAM_WRITE_LENGTH_MAX));
        } else {
            one_process_len = (write_len <= EFLASH_PREPROGRAM_WRITE_LENGTH_MAX) ?
                write_len : EFLASH_PREPROGRAM_WRITE_LENGTH_MAX;
        }
        g_hal_eflash_funcs->write(write_addr + g_eflash_context[0].base_addr,
                                  write_value, one_process_len, begin, g_eflash_context[begin].frequency);
        write_addr += one_process_len;
        write_value += (one_process_len >> BYTE_TO_WORD);
        write_len -= one_process_len;
    }
    return ERRCODE_SUCC;
}

static errcode_t eflash_write_previous_four_bytes(uint32_t addr, const uint32_t *value, uint32_t len)
{
    uint32_t last_four_bytes = FLASH_WRITE_INVALID_VALUE;
    uint8_t *temp = (uint8_t *)&last_four_bytes;
    if (memcpy_s(temp + num_to_pre_word(addr), len, (uint8_t *)value, len) != EOK) {
        return ERRCODE_MEMCPY;
    }
    eflash_write_word_align(get_pre_word_addr(addr), &last_four_bytes, WORD_WIDTH);
    return ERRCODE_SUCC;
}

static errcode_t eflash_write_last_four_bytes(uint32_t addr, const uint32_t *value, uint32_t len)
{
    const uint32_t *write_value = value + (len >> BYTE_TO_WORD);
    uint32_t last_four_bytes = FLASH_WRITE_INVALID_VALUE;
    uint8_t write_size = num_to_pre_word(addr + len);
    if (memcpy_s(&last_four_bytes, write_size, (uint8_t *)write_value, write_size) != EOK) {
        return ERRCODE_MEMCPY;
    }
    eflash_write_word_align(get_pre_word_addr(addr + len), &last_four_bytes, WORD_WIDTH);
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_write(uint32_t addr, const uint32_t *value, uint32_t len)
{
    uint32_t len_tmp = len;
    uint32_t addr_tmp = addr;
    errcode_t ret = ERRCODE_SUCC;
    if ((!eflash_addr_check(addr_tmp, len_tmp)) || (value == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }
    uint32_t write_len = 0;
    const uint32_t *write_value = value;
    uint32_t irq_sts = eflash_porting_lock();
    if (num_to_pre_word(addr_tmp) != 0) {
        write_len = len_tmp < num_to_next_word(addr_tmp) ? len_tmp : num_to_next_word(addr_tmp);
        ret = eflash_write_previous_four_bytes(addr_tmp, value, write_len);
        if (ret != ERRCODE_SUCC) {
            eflash_porting_unlock(irq_sts);
            return ret;
        }
        write_value = (uint32_t *)(uintptr_t)((uint32_t)(uintptr_t)write_value + write_len);
        len_tmp -= write_len;
        addr_tmp += write_len;
    }
    if (len_tmp > 0 && num_to_pre_word(addr_tmp + len_tmp) != 0) {
        ret = eflash_write_last_four_bytes(addr_tmp, write_value, len_tmp);
        if (ret != ERRCODE_SUCC) {
            eflash_porting_unlock(irq_sts);
            return ret;
        }
        len_tmp -= num_to_pre_word(addr_tmp + len_tmp);
    }
    // now, addr and len is word align, but write_value is not sure.
    // if write_value is word align, write it into flash addr
    if (len_tmp == 0) {
        eflash_porting_unlock(irq_sts);
        return ERRCODE_SUCC;
    }
    if (num_to_pre_word((uint32_t)(uintptr_t)write_value) == 0) {
        eflash_write_word_align(addr_tmp, write_value, len_tmp);
        eflash_porting_unlock(irq_sts);
        return ERRCODE_SUCC;
    }
    // if write_value is not word align, memcpy it into eflash_cache

    uint32_t *eflash_cache = (uint32_t *)osal_kmalloc_align(EFLASH_CACHE_SIZE, 0, WORD_WIDTH);
    if (eflash_cache == NULL) {
        eflash_porting_unlock(irq_sts);
        return ERRCODE_MALLOC;
    }
    while (len_tmp > 0) {
        write_len = len_tmp > EFLASH_CACHE_SIZE ? EFLASH_CACHE_SIZE : len_tmp;
        if (memcpy_s(eflash_cache, EFLASH_CACHE_SIZE, write_value, write_len) != EOK) {
            osal_kfree(eflash_cache);
            eflash_porting_unlock(irq_sts);
            return ERRCODE_MEMCPY;
        }
        write_value = (uint32_t *)(uintptr_t)((uint32_t)(uintptr_t)write_value + write_len);
        eflash_write_word_align(addr_tmp, eflash_cache, write_len);
        len_tmp -= write_len;
        addr_tmp += write_len;
    }
    osal_kfree(eflash_cache);
    eflash_porting_unlock(irq_sts);
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_suspend(uintptr_t arg)
{
    unused(arg);
    eflash_port_delay_us(EFLASH_TIME_BEFORE_ENTER_SLEEP);
    g_hal_eflash_funcs->suspend();
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_resume(uintptr_t arg)
{
    unused(arg);
    g_hal_eflash_funcs->resume();
    eflash_port_delay_us(EFLASH_TIME_AFTER_EXIT_SLEEP);
    return ERRCODE_SUCC;
}

errcode_t uapi_eflash_user_read(uint32_t addr, uint32_t *value, uint32_t len)
{
    if ((addr >= USER_EFLASH_START) && ((addr + len) <= USER_EFLASH_END)) {
        return uapi_eflash_read(addr, value, len);
    }
    return ERRCODE_INVALID_PARAM;
}

errcode_t uapi_eflash_user_write(uint32_t addr, const uint32_t *value, uint32_t len)
{
    if ((addr >= USER_EFLASH_START) && ((addr + len) <= USER_EFLASH_END)) {
        return uapi_eflash_write(addr, value, len);
    }
    return ERRCODE_INVALID_PARAM;
}

errcode_t uapi_eflash_user_erase(uint32_t addr, uint32_t len)
{
    if ((addr >= USER_EFLASH_START) && ((addr + len) <= USER_EFLASH_END)) {
        return uapi_eflash_erase(addr, len);
    }
    return ERRCODE_INVALID_PARAM;
}