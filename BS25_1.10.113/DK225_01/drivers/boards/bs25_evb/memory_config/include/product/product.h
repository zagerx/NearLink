/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  bs25 product config
 * Author: @CompanyNameTag
 * Create: 2021-06-16
 */
#ifndef PRODUCT_H
#define PRODUCT_H

#ifndef YES
#define YES (1)
#endif

#ifndef NO
#define NO (0)
#endif

#if (defined BS25_PRODUCT_NONE)
#define APPLICATION_VERSION_STRING "BS25 1.10.113"
#include "product_none.h"
#endif

#if (defined BS25_PRODUCT_EVB)
#define APPLICATION_VERSION_STRING "BS25 1.10.113"
#include "product_evb_standard.h"
#endif

#if (defined BS25_PRODUCT_SAMPLE)
#define APPLICATION_VERSION_STRING "BS25 1.10.113"
#include "product_sample_standard.h"
#endif

#if (defined BS25_PRODUCT_HID)
#define APPLICATION_VERSION_STRING "BS25 1.10.113"
#include "product_hid_common.h"
#if (defined VERSION_DEBUG)
#include "product_hid_debug.h"
#elif (defined VERSION_FACTORY)
#include "product_hid_factory.h"
#elif (defined VERSION_STANDARD)
#include "product_hid_standard.h"
#else
#include "product_hid_end_user.h"
#endif
#endif

#define TWS_EAR_WEAR_NOTICE_SUPP            NO
#define TWS_A2DP_TX_PWR_REDUCE_SUPP         NO
#define TWS_PAGE_TX_PWR_REDUCE_SUPP         NO
#define WITH_SDIO_HOST                      NO
#define USE_FN_PLL                          YES
#define USE_KV_MODE                         NO
#define BT_EXIST                            YES
#define APP_EXIST                           YES
#define DSP_EXIST                           NO
#define DUAL_DSP_EXIST                      NO
#define GNSS_EXIST                          NO
#define SECURITY_EXIST                      NO
#define WIFI_EXIST                          NO
#define UWB_EXIST                           NO
#define NFC_EXIST                           NO
#define CORE_NUMS                           2
#define IPC_SHARE_NUMS                      2
#define OTP_SET_CLK_PERIOD                  NO
#define SHA512_EXIST                        YES
#define SM4_EXIST                           YES
#define SHA_LAST_PACKAGE_IS_BIG_ENDIAN      YES
#define OTP_FIRST_REGION_BITS               2048
#define OTP_SECOND_REGION_BITS              0
#define RPC_PAYLOAD_MAX_SIZE                1024
#define USE_RPC_MODE                        NO
#define SEC_NEW_RSA                         YES
#define EMBED_FLASH_EXIST                   YES
#define SSB_BACKUP                          YES
#define USE_LOAD_SWICH                      NO
#define USE_PMU_WDT                         YES
#define PIO_DRIVE0_IS_LOWEST                NO

#define MASTER_UART_PIO_DRIVE_VAL           HAL_PIO_DRIVE_15
#endif