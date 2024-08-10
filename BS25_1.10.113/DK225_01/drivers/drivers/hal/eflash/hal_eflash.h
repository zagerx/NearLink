/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides HAL eflash \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-08, Create file. \n
 */
#ifndef HAL_EFLASH_H
#define HAL_EFLASH_H

#include <stdint.h>
#include "common_def.h"
#include "errcode.h"
#include "eflash_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_eflash_api EFLASH
 * @ingroup  drivers_hal_eflash
 * @{
 */

/**
 * @if Eng
 * @brief  Init device for hal eflash.
 * @param  [in]  region The eflash region. see @ref eflash_region_t.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  HAL层EFLASH的初始化接口。
 * @param  [in]  region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
typedef errcode_t (*hal_eflash_init_t)(eflash_region_t region);

/**
 * @if Eng
 * @brief  Deinit device for hal eflash.
 * @param  [in] region The eflash region. see @ref eflash_region_t.
 * @else
 * @brief  HAL层eflash去初始化接口。
 * @param  [in] region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @endif
 */
typedef void (*hal_eflash_deinit_t)(eflash_region_t region);

/**
 * @if Eng
 * @brief  Set freq register for hal eflash.
 * @param  [in] region The eflash region. see @ref eflash_region_t.
 * @param  [in] freq   Frequency of eflash. see @ref eflash_frequency_t.
 * @else
 * @brief  HAL层配置频率寄存器。
 * @param  [in] region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @param  [in] freq   eflash的频率. see @ref eflash_frequency_t 。
 * @endif
 */
typedef void (*hal_eflash_set_freq_t)(eflash_region_t region, eflash_frequency_t freq);

/**
 * @if Eng
 * @brief  Set NVR register for hal eflash.
 * @param  [in] region The eflash region. see @ref eflash_region_t.
 * @else
 * @brief  HAL层设置nvr寄存器。
 * @param  [in] region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @endif
 */
typedef void (*hal_eflash_set_nvr_t)(eflash_region_t region);

/**
 * @if Eng
 * @brief  Set erase verify register for hal eflash.
 * @param  [in] region The eflash region. see @ref eflash_region_t.
 * @else
 * @brief  HAL层设置erase verify寄存器。
 * @param  [in] region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @endif
 */
typedef void (*hal_eflash_set_erase_verify_t)(eflash_region_t region);

/**
 * @if Eng
 * @brief  Set NVR register for hal eflash.
 * @param  [in]  addr  The eflash read address.
 * @param  [out] data  Read data address
 * @param  [in]  len   Read data length.
 * @else
 * @brief  HAL层设置nvr寄存器。
 * @param  [in]  addr  eflash读取地址。
 * @param  [out] data  读取地址的数据。
 * @param  [in]  len   数据长度。
 * @endif
 */
typedef void (*hal_eflash_read_t)(uint32_t addr, uint32_t *data, uint32_t len);

/**
 * @if Eng
 * @brief  Set NVR register for hal eflash.
 * @param  [in] addr   The eflash read address.
 * @param  [in] data   Read data address.
 * @param  [in] len    Read data length.
 * @param  [in] region The eflash region. see @ref eflash_region_t.
 * @param  [in] freq   Frequency of eflash. see @ref eflash_frequency_t.
 * @else
 * @brief  HAL层设置nvr寄存器。
 * @param  [in] addr   eflash写入地址。
 * @param  [in] data   写入地址的数据。
 * @param  [in] len    数据长度。
 * @param  [in] region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @param  [in] freq   eflash的频率. see @ref eflash_frequency_t 。
 * @endif
 */
typedef void (*hal_eflash_write_t)(uint32_t addr, const uint32_t *data, uint32_t len, eflash_region_t region,
                                   eflash_frequency_t freq);

/**
 * @if Eng
 * @brief  Chip erase for hal eflash.
 * @param  [in] region The eflash region. see @ref eflash_region_t.
 * @else
 * @brief  HAL层eflash整块芯片擦除。
 * @param  [in] region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @endif
 */
typedef void (*hal_eflash_chip_erase_t)(eflash_region_t region);

/**
 * @if Eng
 * @brief  Sector erase for hal eflash.
 * @param  [in] region The eflash region. see @ref eflash_region_t.
 * @param  [in] id     The sector address when erasing.
 * @else
 * @brief  HAL层eflash扇区擦除。
 * @param  [in] region 选择两块eflash中的一块， 参考 @ref eflash_region_t 。
 * @param  [in] id     擦除时候sector地址。
 * @endif
 */
typedef void (*hal_eflash_sector_erase_t)(eflash_region_t region, uint32_t id);

/**
 * @if Eng
 * @brief  Suspend for hal eflash.
 * @else
 * @brief  HAL层eflash休眠。
 * @endif
 */
typedef void (*hal_eflash_suspend_t)(void);

/**
 * @if Eng
 * @brief  Resume for hal eflash.
 * @else
 * @brief  HAL层eflash休眠唤醒。
 * @endif
 */
typedef void (*hal_eflash_resume_t)(void);

/**
 * @if Eng
 * @brief  Interface between EFLASH driver and EFLASH hal.
 * @else
 * @brief  Driver层EFLASH和HAL层EFLASH的接口。
 * @endif
 */
typedef struct {
    hal_eflash_init_t   init;                               /*!< @if Eng Init device interface.
                                                             @else   HAL层Eflash的初始化接口 @endif */
    hal_eflash_deinit_t deinit;                             /*!< @if Eng Deinit device interface.
                                                             @else   HAL层Eflash去初始化接口 @endif */
    hal_eflash_set_freq_t set_freq;                         /*!< @if Eng Set freq device interface.
                                                             @else   HAL层Eflash设置频率接口 @endif */
    hal_eflash_set_nvr_t set_nvr;                           /*!< @if Eng Set nvr device interface.
                                                             @else   HAL层Eflash设置nvr接口 @endif */
    hal_eflash_set_erase_verify_t set_erase_verify;         /*!< @if Eng Set erase verify device interface.
                                                             @else   HAL层Eflash设置erase verify接口 @endif */
    hal_eflash_read_t    read;                              /*!< @if Eng Read address device interface.
                                                             @else   HAL层Eflash读取内容接口 @endif */
    hal_eflash_write_t   write;                             /*!< @if Eng Write address device interface.
                                                             @else   HAL层Eflash写入内容接口 @endif */
    hal_eflash_chip_erase_t   chip_erase;                   /*!< @if Eng Chip erase device interface.
                                                             @else   HAL层Eflash芯片擦除接口 @endif */
    hal_eflash_sector_erase_t   sector_erase;               /*!< @if Eng Sector erase device interface.
                                                             @else   HAL层Eflash扇区擦除接口 @endif */
    hal_eflash_suspend_t    suspend;                        /*!< @if Eng Enter sleep device interface.
                                                             @else   HAL层Eflash进入深睡接口 @endif */
    hal_eflash_resume_t     resume;                         /*!< @if Eng Exit sleep device interface.
                                                             @else   HAL层Eflash退出深睡接口 @endif */
} hal_eflash_funcs_t;

/**
 * @if Eng
 * @brief  Register @ref hal_eflash_funcs_t into the g_hal_eflash_funcs.
 * @param  [in]  region The eflash region. see @ref eflash_region_t.
 * @param  [in]  funcs  Interface between eflash driver and eflash hal.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  注册 @ref hal_eflash_funcs_t 到 g_hal_eflash_funcs 。
 * @param  [in]  region 串口号， 参考 @ref eflash_region_t 。
 * @param  [in]  funcs  Driver层EFLASH和HAL层EFLASH的接口实例。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_eflash_register_funcs(eflash_region_t region, hal_eflash_funcs_t *funcs);

/**
 * @if Eng
 * @brief  Unregister @ref hal_eflash_funcs_t from the g_hal_eflash_funcs.
 * @param  [in]  region The eflash region. see @ref eflash_region_t.
 * @return ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  从g_hal_eflash_funcs注销 @ref hal_eflash_funcs_t 。
 * @param  [in]  region 串口号， 参考 @ref eflash_region_t 。
 * @return ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_eflash_unregister_funcs(eflash_region_t region);

/**
 * @if Eng
 * @brief  Init the EFLASH which will set the base address of registers.
 * @param  [in]  region The eflash region. see @ref eflash_region_t.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  初始化EFLASH，设置寄存器的基地址。
 * @param  [in]  region 串口号， 参考 @ref eflash_region_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_eflash_regs_init(eflash_region_t region);

/**
 * @if Eng
 * @brief  Deinit the hal_eflash which will clear the base address of registers has been
 *         set by @ref hal_eflash_regs_init.
 * @param  [in]  region The eflash region. see @ref eflash_region_t.
 * @else
 * @brief  去初始化，然后清除在 @ref hal_eflash_regs_init 中设置的寄存器地址。
 * @param  [in]  region 串口号， 参考 @ref eflash_region_t 。
 * @endif
 */
void hal_eflash_regs_deinit(eflash_region_t region);

/**
 * @if Eng
 * @brief  Get interface between eflash driver and eflash hal, see @ref hal_eflash_funcs_t.
 * @param  [in]  region The eflash region. see @ref eflash_region_t.
 * @return Interface between eflash driver and eflash hal, see @ref hal_eflash_funcs_t.
 * @else
 * @brief  获取Driver层EFLASH和HAL层EFLASH的接口实例，参考 @ref hal_eflash_funcs_t 。
 * @param  [in]  region 串口号， 参考 @ref eflash_region_t 。
 * @return Driver层EFLASH和HAL层EFLASH的接口实例，参考 @ref hal_eflash_funcs_t 。
 * @endif
 */
hal_eflash_funcs_t *hal_eflash_get_funcs(eflash_region_t region);

/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif