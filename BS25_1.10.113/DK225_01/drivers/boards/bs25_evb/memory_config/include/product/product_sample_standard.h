/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: bs25 product EVB standard config
 * Author: @CompanyNameTag
 * Create:  2021-06-16
 */
#ifndef PRODUCT_EVB_STANDARD_CONFIG_H
#define PRODUCT_EVB_STANDARD_CONFIG_H

/**
 * @addtogroup connectivity_config_product PRODUCT
 * @{
 */
#define BTH_WITH_SMART_WEAR                 YES
#define BT_MANAGER_DEPLOYED                 2
#define BT_CODEC_TID                        2
#define PM_UART_H0_RX_WKUP_ENABLE           YES
#define PM_UART_L0_RX_WKUP_ENABLE           YES
#define SSB_IMAGE_PAGES                     19
#define DTB_IMAGE_PAGES                     0
#define RECOVERY_IMAGE_PAGES                0
#define RESERVE_IMAGE_PAGES                 0
#define BT_IMAGE_PAGES                      80
#define HIFI0_IMAGE_PAGES                   0
#define HIFI1_IMAGE_PAGES                   0
#define APP_IMAGE_PAGES                     132
#ifdef USE_EMBED_FLASH
#define FOTA_IMAGE_START                    0x102000
#define SSB_FOTA_OFFSET                     0x101000
#else
#define FOTA_IMAGE_START                    0x0
#define SSB_FOTA_OFFSET                     0x1000
#endif
#define SSB_FOTA_LENGTH                     0xD0000
#define DSP_EXTEND_OCRAM_SIZE               1310720 // Unit byte, = 1280 KB
#define USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG NO
#define LOG_LEVEL_APP_DEFAULT_CONFIG        3   // Info level
#define LOG_LEVEL_BT_DEFAULT_CONFIG         3   // Info level
#define LOG_LEVEL_DSP_DEFAULT_CONFIG        3   // Info level
#define BT_MASSDATA_LENGTH                  0x400
#define APP_MASSDATA_LENGTH                 0x400
#define APP_LOGGING_LENGTH                  0xC00
#define DSP_LOGGING_LENGTH                  0
#ifdef CFBB_TEST
#define RC_CLOCK_ON                         NO
#else
#define RC_CLOCK_ON                         YES
#endif
#define SMART_WATCH_BT_COEX_PIN_SWITCH      NO   // pinmux switch in the Coexistence of BT and WiFi of smart watch
#define CAP_SENSOR_BUFF_LEN                 128

/********************Other module board config********************/
#define BTC_SWITCH_COEX                     NO
#define BTC_SMART_WEAR_WLCOEX               NO
#define BTC_WITH_SMART_WEAR                 NO
#define BTC_TWS_MONITOR                     YES
#define BTC_ICO_SUPPORT                     YES
#define BTC_DUAL_ANT_SWITCH_SUPPORT         NO
#define BTC_GET_EMC_FROM_PLATFORM           NO // 耳机产品通过HCI传递NV数据实现后，该选项需要置为NO
#define BTSNOOP_ENABLE                      YES
#define TWS_USER_CHR_RECORD                 NO
#define BTH_WEAR_ENABLE_AUDIO_SINK          NO
#define BTH_WEAR_ENABLE_AUDIO_GATEWAY       NO
#define BTH_WEAR_BREDR_DOUBLE_CONNECT       NO
#define BTC_DFX_LOG_HELP_SUPPORT            NO
#define FACTORY_CALI                        NO
#define BTH_WEAR_ENABLE_CONNECT_MANAGER     YES
#define BTH_WSTP_CMD_SUPPORT                YES
#define BTH_WEAR_ENABLE_BLE_FEATURES        YES
#define BTH_WEAR_ENABLE_HFP_FEATURES        NO
#define WEAR_USER_CONFIG                    NO
#define BTH_HIGH_POWER                      YES
#define MULTI_CONNECT                       YES
#define BTH_CONFIG_HDAP                     NO
#define DEVICE_MANAGE_FEATURE               YES
#define BTH_CALL_LC3_32K                    NO
#define BTH_ENABLE_LC3_CODEC                YES
#define ENABLE_CHANGE_DEVICE_NAME           YES
#define BTH_DIP_PRODUCT_ID                  0x4108
#define ENABLE_BT_XO_16M                    NO
#define BTH_KEYBOARD_MANAGER_SUPPORT        YES
#define BTH_MOUSE_MANAGER_SUPPORT           YES

/********************Other module board config********************/
#ifndef CONFIG_PM_SUPPORT_LPC
#define CONFIG_PM_SUPPORT_LPC               YES
#endif

/**
 * @}
 */
#endif
