/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: ota upgrade porting header file
 */

#ifndef OTA_UPGRADE_PORTING_H
#define OTA_UPGRADE_PORTING_H

#include "ota_upgrade_porting.h"
#include "flash.h"
#include "eflash.h"

#ifdef USE_EMBED_FLASH
#define OTA_FOTA_DATA_START        0x101000
#define OTA_TRANSMIT_INFO_START    0x100000
#define OTA_HASH_START             (0x101000 - 0x20)
#else
#define OTA_FOTA_DATA_START        0x1000
#define OTA_TRANSMIT_INFO_START    0x0
#define OTA_HASH_START             (0x1000 - 0x20)
#endif
#define OTA_TRANSMIT_INFO_LEN      0x1000
#define OTA_HASH_INFO_SIZE         0x20
#define OTA_UPGRAD_RESET_REG       0x57004600

uint32_t ota_port_flash_init(flash_id_t id);
uint32_t ota_port_flash_read_data(flash_id_t id, uint32_t addr, uint8_t *dst, uint32_t length);

uint32_t ota_port_flash_block_erase(flash_id_t id, uint32_t addr, uint32_t length);

uint32_t ota_port_flash_write_data(flash_id_t id, uint32_t addr, const uint8_t *src, uint32_t length);

#endif