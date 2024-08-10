/*
 * Copyright (c) @CompanyNameMagicTag. 2023-2023. All rights reserved.
 * Description: UPG common functions header file
 */

#ifndef UPG_COMMON_PORTING_H
#define UPG_COMMON_PORTING_H

#include "memory_config.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define UPG_FILE_PATH                   "/update/"
#define UPG_FILE_NAME                   "/update/update.fwpkg"
#define UPG_RES_INDEX_PATH              "/update/res_index.bin"

#define UPG_FLASH_SIZE                  0x200000
#define UPG_FLASH_PAGE_SIZE             FLASH_PAGE_SIZE /* Bytes per sector */
#define FOTA_DATA_STATUS_AREA_LEN       UPG_FLASH_PAGE_SIZE
#define FOTA_DATA_BUFFER_AREA_LEN       UPG_FLASH_PAGE_SIZE
#define FOTA_DATA_FLAG_AREA_LEN         UPG_FLASH_PAGE_SIZE
#define UPG_UPGRADE_FLAG_LENGTH (FOTA_DATA_STATUS_AREA_LEN + FOTA_DATA_BUFFER_AREA_LEN + FOTA_DATA_FLAG_AREA_LEN)

#define UPG_META_DATA_LENGTH            0x0

#define PARAMS_PARTITION_IMAGE_ID       0x4B87A52D
#define PARAMS_PARTITION_START_ADDR     PARTITION_FLASH_REGION_START
#define PARAMS_PARTITION_LENGTH         PARTITION_FLASH_REGION_LENGTH

/**
* @ingroup  iot_update
* @brief Upgrade the backup area of each image before start the upgrade.
*        CNcomment:升级开始前对各个镜像的备区升级。CNend
*
* @retval #ERRCODE_SUCC         Success.
* @retval #Other     Failure. For details, see soc_errno.h.
*/
errcode_t upg_image_backups_update(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* UPG_COMMON_PORTING_H */