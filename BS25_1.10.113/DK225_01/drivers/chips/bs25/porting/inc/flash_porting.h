/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides flash port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-30， Create file. \n
 */
#ifndef FLASH_PORTING_H
#define FLASH_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "flash_config_info.h"
#include "memory_config.h"
#include "hal_qspi.h"
#include "debug_print.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup  drivers_port_flash
 * @{
 */
#define flash_print(x)

typedef struct flash_qspi_cfg_s {
    spi_frf_m_t mode;
    uint16_t clk_div;
} flash_qspi_cfg_t;

extern flash_qspi_cfg_t g_flash_qspi_cfg;
/**
 * @brief  FLASH ID.
 */
typedef enum flash_id {
    FLASH_0,
    FLASH_1,
    FLASH_MAX
} flash_id_t;

/**
 * @brief  Get the configuration of the flash.
 * @param  [in]  id The flash ID.
 * @param  [out]  config The configuration of flash.
 */
void flash_porting_get_config(flash_id_t id, flash_cfg_t *config);

/**
 * @brief  Update exit xip baud.
 * @param  [in]  id The flash ID.
 * @param  [in]  bus_clk The bus clock.
 * @param  [in]  freq_mhz The frequency of SPI.
 */
void flash_porting_update_exit_xip_baud(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz);

/**
 * @brief  Update enter xip baud.
 * @param  [in]  id The flash ID.
 * @param  [in]  bus_clk The bus clock.
 * @param  [in]  freq_mhz The frequency of SPI.
 */
void flash_porting_update_enter_xip_baud(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz);

/**
 * @brief  Update enter qspi baud.
 * @param  [in]  id The flash ID.
 * @param  [in]  bus_clk The bus clock.
 * @param  [in]  freq_mhz The frequency of SPI.
 */
void flash_porting_update_enter_qspi_baud(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz);

/**
 * @brief  Set the baud.
 * @param  [in]  id The flash ID.
 * @param  [out]  attr The spi attr.
 */
typedef void (*set_clk_div_t)(flash_id_t id, spi_attr_t *attr);

/**
 * @brief  Set the baud when flash exit xip mode.
 * @param  [in]  id The flash ID.
 * @param  [out]  attr The spi attr.
 */
void flash_porting_set_exit_xip_clk_div(flash_id_t id, spi_attr_t *attr);

/**
 * @brief  Set the baud when flash enter xip mode.
 * @param  [in]  id The flash ID.
 * @param  [out]  attr The spi attr.
 */
void flash_porting_set_enter_xip_clk_div(flash_id_t id, spi_attr_t *attr);

/**
 * @brief  Set the baud when flash enter qspi mode.
 * @param  [in]  id The flash ID.
 * @param  [out]  attr The spi attr.
 */
void flash_porting_set_enter_qspi_clk_div(flash_id_t id, spi_attr_t *attr);

/**
 * @brief  Power on.
 * @param  [in]  id The flash ID.
 */
typedef void (*power_on_t)(flash_id_t id);

/**
 * @brief  Flash power on.
 * @param  [in]  id The flash ID.
 */
void flash_porting_power_on(flash_id_t id);

/**
 * @brief  Flash spi lock create.
 * @param  [in]  id The flash ID.
 */
void flash_porting_spi_lock_create(flash_id_t id);

/**
 * @brief  Flash spi lock delete.
 * @param  [in]  id The flash ID.
 */
void flash_porting_spi_lock_delete(flash_id_t id);

/**
 * @brief  Flash spi lock.
 * @param  [in]  id The flash ID.
 * @return the lock status before lock.
 */
uint32_t flash_porting_spi_lock(flash_id_t id);

/**
 * @brief  Flash spi unlock.
 * @param  [in]  id The flash ID.
 * @param  [in]  status The lock status.
 */
void flash_porting_spi_unlock(flash_id_t id, uint32_t status);

/**
 * @brief  Wait xip mode exit.
 */
void flash_porting_wait_exit_xip_mode(void);

/**
 * @brief  Set power off when app load bt.
 * @param  [in]  flag types power on or power off.
 */
void flash_porting_set_bt_power_on_flag(bool flag);

/**
 * @brief  Return bt is power on or off.
 * @retval true power on.
 * @retval false power off.
 */
bool flash_porting_get_bt_power_on_flag(void);

/**
 * @brief  Save flash id information to kv
 * @return none
 */
void flash_config_save_info(void);

/**
 * @brief  Return current flash end addr
 * @return current flash end addr.
 */
uint32_t flash_config_get_end(void);

/**
 * @brief  Return current flash pages
 * @return current flash pages.
 */
uint16_t flash_config_get_pages(void);

/**
 * @brief  Return current flash kv pages
 * @return current flash kv pages.
 */
uint16_t flash_config_get_kv_pages(void);

/**
 * @brief  Return flash start addr.
 * @return none
 */
uint32_t flash_config_get_start_addr(void);

/**
 * @brief  Get flash page based on address.
 * @return flash page.
 */
uint16_t flash_get_page(uint32_t address);

errcode_t test_uapi_flash_init(flash_id_t id);

uint16_t flash_funcreg_adapt(flash_id_t id);

bool flash_porting_switch_flash_id(uint8_t *id, uint32_t *base_addr, uint32_t addr);

/**
 * @brief  Flash clock enable or disable.
 * @param  [in]  on Enable or disable.
 */
void flash_port_clock_enable(bool on);

/**
 * @brief  Flash power on or power off.
 * @param  [in]  on Power on or power off.
 */
void flash_port_power_on(bool on);

/**
 * @brief  Flash pinmux cfg.
 * @param  [in]  id The flash ID.
 */
void flash_porting_pinmux_cfg(flash_id_t id);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
