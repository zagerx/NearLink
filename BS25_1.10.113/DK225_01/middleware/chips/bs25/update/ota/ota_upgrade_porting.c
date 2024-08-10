/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: ota upgrade porting.
 * This file should be changed only infrequently and with great care.
 */
#include "ota_upgrade_porting.h"
#include "test_suite_log.h"
#include "osal_interrupt.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#else
#include "flash.h"
#include "pinctrl.h"
#include "gpio.h"
#endif

uint32_t ota_port_flash_init(flash_id_t id)
{
    UNUSED(id);
#ifdef USE_EMBED_FLASH
    uapi_eflash_init(0);
    uapi_eflash_set_freq(0, CLOCK_32M);
    uapi_eflash_init(1);
    uapi_eflash_set_freq(1, CLOCK_32M);
#endif
    return 0;
}

uint32_t ota_port_flash_read_data(flash_id_t id, uint32_t addr, uint8_t *dst, uint32_t length)
{
    UNUSED(id);
    uint32_t ret = length;
    uint32_t irq = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_read(addr, (uint32_t *)dst, length) != ERRCODE_SUCC) {
        ret = 0;
    }
#else
    ret = uapi_flash_read_data(FLASH_0, addr, dst, length);
#endif
    osal_irq_restore(irq);
    return ret;
}

uint32_t ota_port_flash_block_erase(flash_id_t id, uint32_t addr, uint32_t length)
{
    UNUSED(id);
    uint32_t ret = length;
    uint32_t irq = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_erase(addr, length) != ERRCODE_SUCC) {
        ret = 0;
    }
#else
    ret = uapi_flash_block_erase(FLASH_0, addr, length, true);
#endif
    osal_irq_restore(irq);
    return ret;
}

uint32_t ota_port_flash_write_data(flash_id_t id, uint32_t addr, const uint8_t *src, uint32_t length)
{
    UNUSED(id);
    uint32_t ret = length;
    uint32_t irq = osal_irq_lock();
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_write(addr, (uint32_t *)src, length) != ERRCODE_SUCC) {
        ret = 0;
    }
#else
    ret = uapi_flash_write_data(FLASH_0, addr, src, length);
#endif
    osal_irq_restore(irq);
    return ret;
}