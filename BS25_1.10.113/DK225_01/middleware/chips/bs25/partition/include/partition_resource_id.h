/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides configuration definitions related to partitions. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-08, Create file. \n
 */
#ifndef PARTITION_RESOURCE_ID_H
#define PARTITION_RESOURCE_ID_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* partition image id, must be same as flash partition bin */
#define PARTITION_IMAGE_ID  0x4B87A52D

/**
 * @defgroup middleware_chips_brandy_partition Partition
 * @ingroup  middleware_chips_brandy
 * @{
 */

/**
 * @if Eng
 * @brief  Partition ids.
 * @else
 * @brief  分区ID
 * @endif
 */
typedef enum {
    PARTITION_SSB_IMAGE = 0x0,
    PARTITION_SSB_BACKUP = 0x1,
    PARTITION_ROOTPUBKEY_SIGNATURE = 0x10,
    PARTITION_SIGNATURE_CERTIFICATE = 0x11,
    PARTITION_SSB_IMAGE_SIGNATURE = 0x12,
    PARTITION_SIGNATURE_CERTIFICATE_BACKUP = 0x13,
    PARTITION_SSB_IMAGE_SIGNATURE_BACKUP = 0x14,
    PARTITION_BT_IMAGE_SIGNATURE = 0x15,
    PARTITION_ACPU_IMAGE_SIGNATURE = 0x16,
    PARTITION_BT_IMAGE = 0x20,
    PARTITION_ACPU_IMAGE = 0x21,
    PARTITION_NV_DATA = 0x30,
    PARTITION_FOTA_DATA = 0x31,

    PARTITION_MAX_CNT = 13 /*!< @if Eng Maximum number of partitions.
        The value is recorded only as the number of partitions and is not used to determine the ID validity.
                                @else   分区的数量，这个枚举只用来记录分区的数量，不作为分区ID有效性的判断 @endif */
} partition_ids_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* PARTITION_RESOURCE_ID_H */
