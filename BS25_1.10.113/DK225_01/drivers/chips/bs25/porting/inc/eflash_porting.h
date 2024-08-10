/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides eflash port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-16， Create file. \n
 */
#ifndef EFLASH_PORTING_H
#define EFLASH_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "hal_eflash_v150_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_eflash EFLASH
 * @ingroup  drivers_port
 * @{
 */

#define XIP_CACHE_CTL  0xA3006000
#define EMBED_FLASH_REGION_0_BASE_ADDR  0x8C400000
#define EMBED_FLASH_REGION_1_BASE_ADDR  0x8C500000
#define PMU1_CTL_RB_BASE  0x57004000
#define EMBED_FLASH_0_TIMING_REG  (XIP_CACHE_CTL + 0x0C00)
#define EMBED_FLASH_1_TIMING_REG  (XIP_CACHE_CTL + 0x0D00)
#define XIP_CACHE_ENABLE_REG  (XIP_CACHE_CTL + 0x0100)
#define XIP_CACHE_MAN_ALL_REG  (XIP_CACHE_CTL + 0x017C)
#define EMBED_FLASH_STB_MUX  (XIP_CACHE_CTL + 0xE00)
#define EFLASH_POR_DPD_SEL  (PMU1_CTL_RB_BASE + 0x0920)
#define EFLASH_POR_DPD_MAN  (PMU1_CTL_RB_BASE + 0x0930)

#define EMBED_FLASH_0_CFG  (XIP_CACHE_CTL + 0x0F00)
#define EMBED_FLASH_1_CFG  (XIP_CACHE_CTL + 0x0F80)
#define EMBED_FLASH_0_NVR_CFG  0x8C3FFFB0
#define EMBED_FLASH_1_NVR_CFG  0x8C6FFFB0

#define EMBED_FLASH_REGION_LENGTH               0x100000
#define EMBED_FLASH_SECTOR_LENGTH               0x1000
#define EMBED_FLASH_REGION_MAX_NUM              2
#define EMBED_FLASH_SECTOR_MAX                  256

#define USER_EFLASH_START 0x100000
#define USER_EFLASH_END   0x1FC000

/**
 * @brief  Definition of embedded flash region.
 */
typedef enum {
    EMBED_FLASH_0 = 0,                                /*!< EFLASH Bus 0 */
    EMBED_FLASH_1,                                    /*!< EFLASH Bus 1 */
    EMBED_FLASH_REGION_MAX,
    EMBED_FLASH_NONE = EMBED_FLASH_REGION_MAX
} eflash_region_t;

/**
 * @if Eng
 * @brief  Definition basic information of embedded flash.
 * @else
 * @brief  Embedded flash基础信息的定义。
 * @endif
 */
typedef struct {
    uint32_t base;                                            /*!<@if Eng Base address of eflash
                                                                  @else   EFLASH的读写擦基地址      @endif */
    uint32_t flash_len;                                       /*!<@if Eng The length of eflash
                                                                  @else   每块eflash的长度      @endif */
    uint16_t sector_len;                                      /*!<@if Eng The sector length of eflash
                                                                  @else   eflash每块扇区的长度      @endif */
    uint16_t sector_num;                                      /*!<@if Eng Number of eflash sectors
                                                                  @else   eflash扇区的数量      @endif */
} eflash_adapter_t;

/**
 * @if Eng
 * @brief  Definition of embedded flash frequency.
 * @else
 * @brief  Embedded flash频率的定义。
 * @endif
 */
typedef enum {
    CLOCK_32M = 0,
    CLOCK_48M,
    CLOCK_64M,
    CLOCK_96M
} eflash_frequency_t;

/**
 * @if Eng
 * @brief  Definition of embedded flash frequency.
 * @else
 * @brief  Embedded flash的基础情况。
 * @endif
 */
typedef struct {
    uint32_t base_addr;                                       /*!<@if Eng Base address of eflash
                                                                  @else   EFLASH的读写擦基地址      @endif */
    uint32_t length;                                    /*!<@if Eng The length of eflash
                                                                  @else   每块eflash的长度      @endif */
    uint16_t sector_length;                                   /*!<@if Eng The sector length of eflash
                                                                  @else   eflash每块扇区的长度      @endif */
    uint16_t sector_num;                                      /*!<@if Eng Number of eflash sectors
                                                                  @else   eflash扇区的数量      @endif */
    bool flash_is_initialised;                                /*!<@if Eng Whether each eflash is initial.
                                                                  @else   每块eflash是否初始化      @endif */
    eflash_frequency_t frequency;                             /*!<@if Eng The frequency of eflash.
                                                                  @else   eflash的频率      @endif */
} eflash_context_t;

extern eflash_adapter_t g_eflash_adapter[EMBED_FLASH_REGION_MAX_NUM];

/**
 * @if Eng
 * @brief  Base address for XIP CACHE control register.
 * @else
 * @brief  XIP CACHE控制寄存器基地址。
 * @endif
 */
extern uintptr_t g_xip_cache_regs_base_addr;

/**
 * @if Eng
 * @brief Base address for XIP CACHE consistency control register.
 * @else
 * @brief  XIP CACHE一致性控制寄存器基地址。
 * @endif
 */
extern uintptr_t g_xip_cache_man_all_regs_base_addr;

/**
 * @if Eng
 * @brief  Base address for STB EFLASH MUX.
 * @else
 * @brief  STB EFLASH MUX首地址。
 * @endif
 */
extern uintptr_t g_eflash_stb_mux_regs_base_addr;

/**
 * @if Eng
 * @brief  Base address for EFLASH low-power configuration register.
 * @else
 * @brief  EFLASH低功耗配置。
 * @endif
 */
extern uintptr_t g_eflash_por_dpd_regs_base_addr;

/**
 * @if Eng
 * @brief  Get the base address of a specified eflash.
 * @param  [in]  region The region of embedded flash. see @ref eflash_region_t.
 * @retval The base address of specified eflash.
 * @else
 * @brief  获取指定的eflash的初始地址。
 * @param  [in]  region 指定的eflash块。
 * @retval eflash的初始地址。
 * @endif
 */
uintptr_t eflash_porting_base_addr_get(eflash_region_t region);

/**
 * @if Eng
 * @brief  Get the NVR_CFG address of a specified eflash.
 * @param  [in]  region The region of embedded flash. see @ref eflash_region_t.
 * @retval The NVR_CFG address of specified eflash.
 * @else
 * @brief  获取指定的NVR_CFG的初始地址。
 * @param  [in]  region 指定的eflash块。
 * @retval NVR_CFG的初始地址。
 * @endif
 */
uintptr_t eflash_porting_nvr_cfg_addr_get(eflash_region_t region);

/**
 * @if Eng
 * @brief  Get the flash_cfg address of a specified eflash.
 * @param  [in]  region The region of embedded flash. see @ref eflash_region_t.
 * @retval The flash_cfg address of specified eflash.
 * @else
 * @brief  获取指定的flash_cfg的初始地址。
 * @param  [in]  region 指定的eflash块。
 * @retval flash_cfg的初始地址。
 * @endif
 */
uintptr_t eflash_porting_flash_cfg_addr_get(eflash_region_t region);

/**
 * @if Eng
 * @brief  Register hal funcs objects into hal_eflash module.
 * @else
 * @brief  将hal funcs对象注册到hal_eflash模块。
 * @endif
 */
void eflash_port_register_hal_funcs(void);

/**
 * @if Eng
 * @brief  Unregister hal funcs objects from hal_pwm module.
 * @else
 * @brief  从hal_pwm模块取消注册hal funcs对象。
 * @endif
 */
void eflash_port_unregister_hal_funcs(void);

/**
 * @if Eng
 * @brief  Initialize eflash information.
 * @param  [in]  ctxt The context of Embedded flash. see @ref eflash_context_t.
 * @else
 * @brief  初始化eflash信息。
 * @param  [in]  ctxt eflash的基础信息，参考 @ref eflash_context_t 。
 * @endif
 */
void eflash_port_context_init(eflash_context_t *ctxt);

/**
 * @if Eng
 * @brief  Provide txco time delay.
 * @param  [in]  time Time required to delay.
 * @else
 * @brief  提供txco时间延迟。
 * @param  [in]  time 需要延迟的时间。
 * @endif
 */
void eflash_port_delay_us(uint32_t time);

/**
 * @brief  Eflash lock create.
 */
void eflash_porting_lock_create(void);

/**
 * @brief  Eflash lock delete.
 */
void eflash_porting_lock_delete(void);

/**
 * @if Eng
 * @brief  Lock interrupt for eflash operation.
 * @retval The status of the irq_lock applied for eflash.
 * @else
 * @brief  为eflash操作加锁中断。
 * @retval eflash 申请的锁中断状态号。
 * @endif
 */
uint32_t eflash_porting_lock(void);

/**
 * @if Eng
 * @brief  Free eflash interrupt lock.
 * @param  [in]  irq_status The status of eflash irq_lock to free.
 * @else
 * @brief  释放eflash锁中断。
 * @param  [in]  irq_status eflash要释放的锁中断状态号。
 * @endif
 */
void eflash_porting_unlock(uint32_t irq_status);

/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif