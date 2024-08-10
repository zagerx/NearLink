/*
 * Copyright (c) @CompanyNameMagicTag. 2023-2023. All rights reserved.
 * Description: UPG product definitions header file
 */

#ifndef UPG_DEFINITIONS_PORTING_H
#define UPG_DEFINITIONS_PORTING_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup middleware_chips_brandy_update_api update
 * @ingroup  middleware_chips
 * @{
 */

/**
 * @if Eng
 * @brief  SSB signature image ids.
 * @else
 * @brief  SSB签名镜像ID
 * @endif
 */
#define SSB_SIGNATURE_IMAGE_ID                  0x4B1E3C1E

/**
 * @if Eng
 * @brief  Image ids.
 * @else
 * @brief  SSB镜像ID
 * @endif
 */
#define SSB_IMAGE_ID                           0x4B1E3C2D

/**
 * @if Eng
 * @brief  Application signature image ids.
 * @else
 * @brief  Application签名镜像ID
 * @endif
 */
#define APPLICATION_SIGNATURE_IMAGE_ID          0x4B0F2D1E

/**
 * @if Eng
 * @brief  Application image ids.
 * @else
 * @brief  Application镜像ID
 * @endif
 */
#define APPLICATION_IMAGE_ID                   0x4B0F2D2D

/**
 * @if Eng
 * @brief  Bt signature image ids.
 * @else
 * @brief  Bt签名镜像ID
 * @endif
 */
#define BT_SIGNATURE_IMAGE_ID                   0x4BF01E1E

/**
 * @if Eng
 * @brief  Bt image ids.
 * @else
 * @brief  Bt镜像ID
 * @endif
 */
#define BT_IMAGE_ID                            0x4BF01E2D

/**
 * @if Eng
 * @brief  key area reserved length.
 * @else
 * @brief  key区保留字段长度
 * @endif
 */
#define KEY_AREA_RESERVED_LEN 176

/**
 * @if Eng
 * @brief  info area user defined length.
 * @else
 * @brief  info区用户字段长度
 * @endif
 */
#define INFO_AREA_USER_LEN 176

/**
 * @if Eng
 * @brief  signature length.
 * @else
 * @brief  签名字段长度
 * @endif
 */
#define SIG_LEN         512

/**
 * @if Eng
 * @brief  FOTA external public key.
 * @else
 * @brief  FOTA二级公钥长度
 * @endif
 */
#define PUBLIC_KEY_LEN 516

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

/**
 * @}
 */

#endif /* UPG_DEFINITIONS_PORTING_H */