/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  Default memory configurations
 * Author: @CompanyNameTag
 * Create:  2021-06-16
 */

#ifndef MEMORY_CONFIG_COMMON_H
#define MEMORY_CONFIG_COMMON_H

#include "product.h"
#include "chip_core_definition.h"

/**
 * @defgroup connectivity_config_memory MEMORY
 * @ingroup  connectivity_config
 * @{
 */
/* Standard lengths */
#define BT_VECTORS_LENGTH     0x204
#define APP_VECTORS_LENGTH    360
#define VERSION_LENGTH        88
#define CHANGE_ID_LENGTH      16

#ifdef MASTER_LOAD_SLAVE
#define BUILD_INFO_LENGRH    408
#else
#define BUILD_INFO_LENGRH    0
#endif

#define FILL_4_BYTE           4

#define BCPU_RAM_START        0x0
/* ----------------------------------------------------------------------------------------------------------------- */
/* Share MEM defines, 64K in FPGA, 160K in ASIC
 * SHARE MEM                IPC Mailbox
 *                          SYSTEM CONFIG
 *                          SYSTEM STATUS
 *                          BT LOG
 *                          APP LOG
 *                          DSP LOG
 */
#define SHARED_MEM_START 0x87000000
#define SHARED_MEM_LENGTH 0x4000
#define SHARED_MEM_END    (SHARED_MEM_START + SHARED_MEM_LENGTH)

/*
 * ********************* ROM ALLOCATION ***********************
 *
 * Used solely by the BT Core ROM image built into the chip.
 */
/* 32K ROM */
#define ROM_START  0x00000000
#define ROM_LENGTH 0x10000

/* APP ITCM config */
#define APP_ITCM_ORIGIN 0x10000
#define APP_ITCM_LENGTH 0x20000

/* APP DTCM config */
#define APP_DTCM_ORIGIN 0x20000000
#define APP_DTCM_LENGTH 0x30000

/*
 * ********************* FLASH ALLOCATION *********************
 * Flash is shared between the cores, and also handles a limited number of
 * non-volatile storage areas.
 *
 * Flash is split into 2 main areas
 * IMAGE Area
 *  Managed by the BT core
 *  This contains, in order from the bottom of Flash:
 *      SSB - SSB image starting at the bottom of flash. Variable size depending on the SSB on the chip that
 *            the package is loaded on. The values in this file only apply to the SSB built with this file.
 *      RECOVERY_Image   - starts in the page after the SSB.
 *      BT_Image         - starts in the page after the RECOVERY_Image
 *      DSP_Image        - starts in the page after the BT_Image
 *      APP_Image        - starts in the page after the DSP_Image
 *      System On-demand region - dynamically allocated flash for FOTA etc
 * NV Area
 *  Ends at the last page of flash, is configurable in size above a minimum allocation.
 *  NV Area, of 12 pages, is allocated, from the lowest address:
 *      8 pages of general use (pages are not fixed and self-identifying), consisting of
 *          1 Page for Asset Store defragmentation
 *          3 pages for Asset Store
 *          2 Pages for App and DSP Core KV defragmentation
 *          1 page for DSP Core KV
 *          1 page for App Core KV
 *      2 pages for BT core use (last 2 pages in Flash)
 *          1 page for BT Core KV
 *          1 page for BT Core KV defragmentation
 */
#define FLASH_START     0x98000000
#ifdef USE_EMBED_FLASH
#define FLASH_LEN       0x200000
#else
#define FLASH_LEN       0x8000000
#endif
#define FLASH_MAX_END   (FLASH_START + FLASH_LEN)
#define EMBED_FLASH_START     0x8C400000
#define EMBED_FLASH_LEN       0x200000
#define EMBED_FLASH_MAX_END   (EMBED_FLASH_START + EMBED_FLASH_LEN)
#define DATA_FLASH_START        0x90000000
#define DATA_FLASH_LEN          0x8000000
#define DATA_FLASH_MAX_END      (DATA_FLASH_START + DATA_FLASH_LEN)
#define FLASH_PAGE_SIZE 4096

/* APP ITCM config */
#define MCU_ITCM_END    (APP_ITCM_ORIGIN + APP_ITCM_LENGTH)

#define SSB_BOOT_RAM_ORIGIN APP_ITCM_ORIGIN
#define SSB_BOOT_RAM_LENGTH APP_ITCM_LENGTH

#define DUMP_USE_EMBED_FLASH    0
/* dump save start addr. 0:no dump. others:start addr. */
#define FLASH_DUMP_START        0x0
#define FLASH_DUMP_SIZE         (0 * 0x1000) /* (n * 0x1000) */

/* ----------------------------------------------------------------------------------------------------------------- */
/* IMAGE Area Defines */
/* MCU SSB Region */
#if USE_KV_MODE == YES
#define SSB_FLASH_REGION_PAGES  (SSB_IMAGE_PAGES)
#define KV_PAGES 5
#else
#define SSB_FLASH_REGION_PAGES  (SSB_IMAGE_PAGES + 5)
#define KV_PAGES 0
#endif
#define SSB_FLASH_REGION_START  (FLASH_START)
#define SSB_FLASH_REGION_LENGTH (SSB_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)

#define SSB_VECTORS_LOAD_ORIGIN (SSB_FLASH_REGION_START)
#define SSB_VERSION_LOAD_ORIGIN (SSB_VECTORS_LOAD_ORIGIN + APP_VECTORS_LENGTH)
#define SSB_CHANGEID_LOAD_ORIGIN (SSB_VERSION_LOAD_ORIGIN + VERSION_LENGTH)

#define SSB_PROGRAM_ORIGIN (SSB_CHANGEID_LOAD_ORIGIN + CHANGE_ID_LENGTH)
#define SSB_PROGRAM_LENGTH ((SSB_FLASH_REGION_LENGTH) - (APP_VECTORS_LENGTH + VERSION_LENGTH + CHANGE_ID_LENGTH))

/* MCU SSB BACK-UP Region */
#define SSB_BACKUP_FLASH_REGION_PAGES  20
#define SSB_BACKUP_FLASH_REGION_START  (SSB_FLASH_REGION_START + SSB_FLASH_REGION_LENGTH + KV_PAGES * FLASH_PAGE_SIZE)
#define SSB_BACKUP_FLASH_REGION_LENGTH (SSB_BACKUP_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)

/* SEC_BOOT Region for Signature Certificate */
#ifdef SUPPORT_SEC_BOOT
#define SEC_BOOT_FLASH_REGION_PAGES  9
#else
#define SEC_BOOT_FLASH_REGION_PAGES  0
#endif
#define SEC_BOOT_FLASH_REGION_START  (SSB_BACKUP_FLASH_REGION_START + SSB_BACKUP_FLASH_REGION_LENGTH)
#define SEC_BOOT_FLASH_REGION_LENGTH (SEC_BOOT_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)

/* DTB Region */
#define DTB_FLASH_REGION_PAGES  (DTB_IMAGE_PAGES)
#define DTB_FLASH_REGION_START  (SEC_BOOT_FLASH_REGION_START + SEC_BOOT_FLASH_REGION_LENGTH)
#define DTB_FLASH_REGION_LENGTH (DTB_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)

/* RECOVERY Region */
#define RECOVERY_FLASH_REGION_PAGES  (RECOVERY_IMAGE_PAGES)
#define RECOVERY_FLASH_REGION_START  (DTB_FLASH_REGION_START + DTB_FLASH_REGION_LENGTH)
#define RECOVERY_FLASH_REGION_LENGTH (RECOVERY_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)
#define RECOVERY_FLASH_REGION_OFFSET (RECOVERY_FLASH_REGION_START - FLASH_START)

/* RESERVE Region */
#define RESERVE_FLASH_REGION_PAGES  (RESERVE_IMAGE_PAGES)
#define RESERVE_FLASH_REGION_START  (RECOVERY_FLASH_REGION_START + RECOVERY_FLASH_REGION_LENGTH)
#define RESERVE_FLASH_REGION_LENGTH (RESERVE_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)

/* BT image xip addr. */
#ifdef USE_EMBED_FLASH
#define BT_XIP_REMAP      (((BT_PROGRAM_ORIGIN) - 0x98000000) + (EMBED_FLASH_START) - 0x80000000)
#else
#define BT_XIP_REMAP      ((BT_PROGRAM_ORIGIN) - 0x80000000)
#endif

/* BT Region */
#define BT_FLASH_REGION_PAGES  (BT_IMAGE_PAGES)
#define BT_FLASH_REGION_START  (RESERVE_FLASH_REGION_START + RESERVE_FLASH_REGION_LENGTH)
#define BT_FLASH_REGION_LENGTH (BT_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)

/* Fill 4 byte to align flash 8 byte */
#define BT_PROGRAM_ORIGIN (BT_FLASH_REGION_START)
#define BT_PROGRAM_LENGTH (BT_FLASH_REGION_LENGTH)

#define BT_FLASH_REGION_OFFSET (BT_FLASH_REGION_START - FLASH_START)

/* DSP Region */
#define DSP_FLASH_REGION_PAGES  (HIFI0_IMAGE_PAGES)
#define DSP_FLASH_REGION_START  (BT_FLASH_REGION_START + BT_FLASH_REGION_LENGTH)
#define DSP_FLASH_REGION_LENGTH (DSP_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)

/* DSP1 Region */
#define DSP1_FLASH_REGION_PAGES  (HIFI1_IMAGE_PAGES)
#define DSP1_FLASH_REGION_START  (DSP_FLASH_REGION_START + DSP_FLASH_REGION_LENGTH)
#define DSP1_FLASH_REGION_LENGTH (DSP1_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)
/* APP Region */
#define APP_FLASH_REGION_PAGES  (APP_IMAGE_PAGES)
#define APP_FLASH_REGION_START  (DSP1_FLASH_REGION_START + DSP1_FLASH_REGION_LENGTH)
#define APP_FLASH_REGION_LENGTH (APP_FLASH_REGION_PAGES * FLASH_PAGE_SIZE)
#define APP_FLASH_REGION_OFFSET (APP_FLASH_REGION_START - FLASH_START)

#define APP_PROGRAM_ORIGIN (APP_FLASH_REGION_START)
#define APP_PROGRAM_LENGTH (APP_FLASH_REGION_LENGTH)

/* cores image flash region start */
#define FLASH_CORE_IMAGES_START (FLASH_START + SSB_FLASH_REGION_LENGTH + SSB_BACKUP_FLASH_REGION_LENGTH + \
                                 SEC_BOOT_FLASH_REGION_LENGTH + KV_PAGES * FLASH_PAGE_SIZE)
/* ----------------------------------------------------------------------------------------------------------------- */
/* KV Region Defines */
/* Minimum System Configuration pages in FLASH reserved */
#if USE_KV_MODE == YES
#define SYSTEM_RESERVED_FLASH_PAGES 4
#else
#define SYSTEM_RESERVED_FLASH_PAGES 0
#endif
/* Minimum reserved Non Volatile storage + Config Data. */
#define FLASH_RESERVED_LENGTH (SYSTEM_RESERVED_FLASH_PAGES * FLASH_PAGE_SIZE)

/*
 * ********************* SEC BOOT REGION *********************
 */
#define MCU_PARTITION_SIGN_START        SEC_BOOT_FLASH_REGION_START
#define MCU_BT_IMAGE_SIGN_START         (MCU_PARTITION_SIGN_START + FLASH_PAGE_SIZE)
#define MCU_APP_IMAGE_SIGN_START        (MCU_BT_IMAGE_SIGN_START + FLASH_PAGE_SIZE)
#define MCU_NV_SIGN_START               (MCU_APP_IMAGE_SIGN_START + FLASH_PAGE_SIZE)
#define MCU_ROOT_PUBKEY_START           (FLASH_START + 0x30000)
#define MCU_SUB_PUBKEY_START            (MCU_ROOT_PUBKEY_START + FLASH_PAGE_SIZE)
#define MCU_SSB_IMAGE_SIGN_START        (MCU_SUB_PUBKEY_START + FLASH_PAGE_SIZE)
#define MCU_SUB_PUBKEY_BACKUP_START     (MCU_SSB_IMAGE_SIGN_START + FLASH_PAGE_SIZE)
#define MCU_SSB_IMAGE_SIGN_BACKUP_START (MCU_SUB_PUBKEY_BACKUP_START + FLASH_PAGE_SIZE)
#define MCU_SEC_BOOT_SIGN_BACKUP_START  MCU_SUB_PUBKEY_BACKUP_START
#define MCU_SEC_BOOT_SIGN_BACKUP_LEN    (2 * FLASH_PAGE_SIZE)
#define MCU_RECOVERY_IMAGE_SIGN_START   (MCU_SEC_BOOT_SIGN_BACKUP_START + MCU_SEC_BOOT_SIGN_BACKUP_LEN)

#define PARTITION_TABLE_REGION_START    (FLASH_START + 0x1FB000)
#define PARTITION_TABLE_REGION_LENGTH   FLASH_PAGE_SIZE
#define PARTITION_FLASH_REGION_START    (FLASH_START + 0x1FB000)
#define PARTITION_FLASH_AREA_START      (FLASH_START + 0x1FB000)
#define PARTITION_FLASH_REGION_LENGTH   0x1000
#ifdef USE_EMBED_FLASH
#define MCU_FLASH_NV_START              (FLASH_START + 0x1FC000)
#else
#define MCU_FLASH_NV_START              (DATA_FLASH_START + 0xFC000)
#endif
#define MCU_FLASH_NV_LEN                (4 * FLASH_PAGE_SIZE)
#define MCU_FLASH_NV_END                (MCU_FLASH_NV_START + MCU_FLASH_NV_LEN)

#define OFFLINE_LOG_FLASH_LENTH (0x32000) // 200K
#ifdef USE_EMBED_FLASH
#define OFFLINE_LOG_FLASH_START (EMBED_FLASH_MAX_END - OFFLINE_LOG_FLASH_LENTH - MCU_FLASH_NV_LEN)
#else
#define OFFLINE_LOG_FLASH_START (0x90000000 + 0x700000 - OFFLINE_LOG_FLASH_LENTH - MCU_FLASH_NV_LEN)
#endif
/*
 * ********************* RAM ALLOCATION *********************
 *
 * Three main RAM areas, the 'BCPU' RAM, the 'APP' RAM, and 'share' RAM.
 *      'APP' RAM include 'ITCM' for code and 'DTCM' for DATA.
 * As the name implies, every core can access it's RAM and they all allowed
 * to access 'share' RAM
 *
 * BT core can access all RAM include APP RAM and DSP RAM.
 *
 * The 'shared' RAM is used by all cores, this area visible to them all
 * to exchange larger amounts of data.
 *
 * SHARED RAM               IPC mail box
 *                          LOG Area
 */
/* ----------------------------------------------------------------------------------------------------------------- */
/* BCPU RAM defines
 * BCPU has base 640K RAM and can set other 96K at the share RAM
 * as it's private, total 736K.
 * BCPU RAM                 VECTORS TABLE
 *                          STACK
 *                          RAM TEXT
 *                          RAM
 *                          PRESERVE
 *                          *VIRTUAL_OTP
 */
#define BCPU_RAM_START      0x0
#define BCPU_ROM_START      0x30000

#define BCPU_RAM_LENGTH     0x30000
#define BCPU_ROM_LENGTH     0x50000
#define BCPU_RAM_END        (BCPU_RAM_START + BCPU_RAM_LENGTH)

/* rom ram text */
#define BCPU_STARTUP_LENGTH       0x400
#define BCPU_PATCH_TBL_LENGTH     0x410
#define BCPU_ROM_RAM_START   (BCPU_RAM_START + BCPU_STARTUP_LENGTH + BCPU_PATCH_TBL_LENGTH)

/* APP Core can also access the BT core RAM, need mapping out. */
#define BT_RAM_ORIGIN_APP_MAPPING 0xA6000000
#define BT_RAM_ORIGIN_APP_MAPPING_LENGTH BCPU_RAM_LENGTH

/* 48 byte for BCPU system clocks status */
#define BCPU_SYSTEM_CLOCKS_LENGTH 0x30

/* 2048 bits for virtual otp */
#define OTP_SIZE_IN_BITS  2048 /* Size of OTP memory in bits */
#define OTP_SIZE_IN_BYTES ((OTP_SIZE_IN_BITS) / 8)

/* the minimum BT heap size (36kB) */
#define BT_HEAP_MINIMUM_SIZE 0x9000

/* the minimum APP heap size */
#define APP_HEAP_MINIMUM_SIZE (APP_HEAP_SIZE)

/* for update the updater */

/* 48 byte for BCPU system clocks status */
#define BCPU_SYSTEM_CLOCKS_LENGTH 0x30
#define BCPU_SYSTEM_CLOCKS_ORIGIN (BCPU_RAM_END - BCPU_SYSTEM_CLOCKS_LENGTH)

#define BCPU_PATCH_LENGTH   (0x4000 - BCPU_SYSTEM_CLOCKS_LENGTH)
#define BCPU_PATCH_START    (BCPU_SYSTEM_CLOCKS_ORIGIN - BCPU_PATCH_LENGTH)

#define BCPU_NEW_STACKS_LENGTH 0x10000
#define BCPU_NEW_STACKS_ORIGIN (BCPU_SYSTEM_CLOCKS_ORIGIN - BCPU_NEW_STACKS_LENGTH)

#define BCPU_USER_STACK_LEN      0x400
#define BCPU_IRQ_STACK_LEN       0x400
#define BCPU_EXCP_STACK_LEN      0x400
#define BCPU_NMI_STACK_LEN       0X400


/* ----------------------------------------------------------------------------------------------------------------- */
/* APP RAM defines
 * APP has base 512K ITCM (Instruction TCM) for code
 *              256K DTCM (Data TCM) for DATA.
 * APP ITCM                 VECTORS TABLE
 *                          RAM TEXT
 *
 * APP DTCM                 STACK
 *                          RAM
 */
/* 512K ITCM for APP core code, start at end of vectors table */
#define APP_RAMTEXT_ORIGIN (APP_ITCM_ORIGIN)
#define APP_RAMTEXT_LENGTH (APP_ITCM_LENGTH)

/* 256K DTCM for APP core data */
/* stack for normal 8k */
#define APP_USER_STACK_BASEADDR APP_DTCM_ORIGIN
#define APP_USER_STACK_LEN      0x800
#define APP_USER_STACK_LIMIT    (APP_USER_STACK_BASEADDR + APP_USER_STACK_LEN)

/* stack for irq 1k */
#define APP_IRQ_STACK_BASEADDR APP_USER_STACK_LIMIT
#define APP_IRQ_STACK_LEN      0x800
#define APP_IRQ_STACK_LIMIT    (APP_IRQ_STACK_BASEADDR + APP_IRQ_STACK_LEN)

/* stack for exception 1k */
#define APP_EXCP_STACK_BASEADDR APP_IRQ_STACK_LIMIT
#define APP_EXCP_STACK_LEN      0x800
#define APP_EXCP_STACK_LIMIT    (APP_EXCP_STACK_BASEADDR + APP_EXCP_STACK_LEN)

/* stack for nmi 1k */
#define APP_NMI_STACK_BASEADDR APP_EXCP_STACK_LIMIT
#define APP_NMI_STACK_LEN      0x800
#define APP_NMI_STACK_LIMIT    (APP_NMI_STACK_BASEADDR + APP_NMI_STACK_LEN)

#define APP_RAM_ORIGIN (APP_EXCP_STACK_LIMIT)
#define APP_RAM_END    (APP_DTCM_ORIGIN + APP_DTCM_LENGTH)
#define APP_RAM_LENGTH (APP_RAM_END - APP_RAM_ORIGIN)

/* BT Core can also access the APP core RAM, need mapping out. */
#define APP_ITCM_ORIGIN_BT_MAPPING 0x4C000000
#define APP_DTCM_ORIGIN_BT_MAPPING 0x4C000000

/* DSP RAM use and manage by DSP team, M7 just need to know the access mapping. */
#define DSP_CODE_ADDR_BT_MAPPING 0x58000000

/* 12*N bytes for cpu trace, trace line is 12 bytes, 8184 bytes */
#define MCPU_TRACE_MEM_REGION_START 0x52006000
#define BCPU_TRACE_MEM_REGION_START 0x5900E000
#define CPU_TRACE_MEM_REGION_LENGTH 0x1FF8

/* IPC Mail box region, every core need have a send mail box to other cores
 * so there is 6 mailbox totally, use (3KB+16B) * 3 share mem.
 */
#define IPC_MAILBOX_REGION_START  (SHARED_MEM_START)
#define IPC_MAILBOX_REGION_LENGTH 0x820

/* 176 bytes for BT core preserve region */
#define PRESERVED_REGION_ORIGIN (IPC_MAILBOX_REGION_START + IPC_MAILBOX_REGION_LENGTH)
#define PRESERVED_REGION_LENGTH 0xfc

/* 176 bytes for App core preserve region */
#define APP_PRESERVED_REGION_ORIGIN (PRESERVED_REGION_ORIGIN + PRESERVED_REGION_LENGTH)
#define APP_PRESERVED_REGION_LENGTH 0x0

/* fix the memory map layout */
#define IPC_MAILBOX_REGION_FIX_LENGTH (IPC_MAILBOX_REGION_LENGTH + \
                                       PRESERVED_REGION_LENGTH + APP_PRESERVED_REGION_LENGTH) // 0x49C0

/* System config region. */
#define SYSTEM_CFG_REGION_START  (APP_PRESERVED_REGION_ORIGIN + APP_PRESERVED_REGION_LENGTH)
#define SYSTEM_CFG_REGION_LENGTH 0x40

/* Reboot magic region. */
#define REBOOT_MAGIC_START  (SYSTEM_CFG_REGION_START + SYSTEM_CFG_REGION_LENGTH)
#define REBOOT_MAGIC_LENGTH 0x80

#define SYSTEM_CLK_REGION_START (REBOOT_MAGIC_START + REBOOT_MAGIC_LENGTH)
#define SYSTEM_CLK_REGION_LENGTH 0xC0

/* System status region. */
#define SYSTEM_STATUS_ORIGIN (SYSTEM_CLK_REGION_START + SYSTEM_CLK_REGION_LENGTH)
#define SYSTEM_STATUS_LENGTH 0xC0

/* Mass data region */
/* wear(BT - 1K, APP - 1K)  tws(BT - 1K, APP - 1K) */
#define MASSDATA_REGION_START   (SYSTEM_STATUS_ORIGIN + SYSTEM_STATUS_LENGTH)
#define MASSDATA_REGION_LENGTH  (BT_MASSDATA_LENGTH + APP_MASSDATA_LENGTH)

/* LOG Region */
/* BT - 5K, APP - 5K */
#define BT_LOGGING_LENGTH  0x1400

#define LOGGING_REGION_START  (MASSDATA_REGION_START + MASSDATA_REGION_LENGTH)
#define LOGGING_REGION_LENGTH (BT_LOGGING_LENGTH + APP_LOGGING_LENGTH + DSP_LOGGING_LENGTH)

/* Trng data shared memory 512byte */
#define TRNG_DATA_REGION_START  (LOGGING_REGION_START + LOGGING_REGION_LENGTH)
#define TRNG_DATA_REGION_LENGTH 0

/* Prompt tone region 10K for tws and 5K for wear */
#define PROMPT_TONE_REGION_START  (TRNG_DATA_REGION_START + TRNG_DATA_REGION_LENGTH)

/* Audio data stream region 8K for tws and 20K for wear */
#define AUDIO_DATA_STREAM_REGION_START  (PROMPT_TONE_REGION_START + PROMPT_TONE_REGION_LENGTH)

/* AFE DIAG MEM */
#define AFE_SHARE_MEM_ORIGIN (SHARED_MEM_START + SHARED_MEM_LENGTH - 0x200)
#define AFE_SHARE_MEM_LENGTH (0x200)
/* RESERVED share mem */
#define RESERVED_SHARE_MEM_ORIGIN (AUDIO_DATA_STREAM_REGION_START + AUDIO_DATA_STREAM_REGION_LENGTH)
#define RESERVED_SHARE_MEM_LENGTH (SHARED_MEM_START + SHARED_MEM_LENGTH - \
                                   AFE_SHARE_MEM_LENGTH - RESERVED_SHARE_MEM_ORIGIN)

#if (RESERVED_SHARE_MEM_LENGTH < 0)
#error Share memory overflow
#endif

#ifndef BUILD_APPLICATION_ATE
/* BT DIAG region, Use L2RAM  */
#define BT_DIAG_REGION_MEMORY_START  0x80100000
#define BT_DIAG_REGION_MEMORY_LENGTH 0x20000
#define BT_DIAG_REGION_MEMORY_END    (BT_DIAG_REGION_MEMORY_START + BT_DIAG_REGION_MEMORY_LENGTH)
#endif

#ifdef USE_EMBED_FLASH
#define OTA_FOTA_DATA_START        0x101000
#define OTA_TRANSMIT_INFO_START    0x100000
#define OTA_HASH_START             (0x101000 - 0x20)
#define TRANSMIT_OTA_INFO_START    0x100000
#define TRANSMIT_OTA_INFO_END      (0x101000 - 0x20) // 1M flash
#define TRANSMIT_OTA_DATA_START    0x101000 // 1M flash
#else
#define OTA_FOTA_DATA_START        0x1000
#define OTA_TRANSMIT_INFO_START    0x0
#define OTA_HASH_START             (0x1000 - 0x20)
#define TRANSMIT_OTA_INFO_START    0x0
#define TRANSMIT_OTA_INFO_END      (0x1000 - 0x20)
#define TRANSMIT_OTA_DATA_START    0x1000
#endif
#define OTA_TRANSMIT_INFO_LEN      0x1000
#define OTA_HASH_INFO_SIZE         0x20
#define TRANSMIT_OTA_INFO_SIZE     (0x1000 - 0x20)

/*
 * ********************* ADDITIONAL MEMORY CONFIGURATION DEFINITIONS *********************
 * The offset in the BCPU (either flash or ROM) to where the version information starts.
 */
#define VERSION_INFORMATION_OFFSET (BT_VECTORS_LENGTH)

#ifndef HADM_POOL_SIZE
#define HADM_POOL_SIZE          0
#endif

/**
 * @}
 */
#endif
