/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: UPG common functions for different chip
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"
#include "common_def.h"
#include "upg_debug.h"
#include "partition.h"
#include "securec.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#else
#include "flash.h"
#endif
#include "upg_porting.h"
#include "hal_reboot.h"
#include "tcxo.h"
#include "non_os_reboot.h"
#include "non_os.h"
#include "cpu_utils.h"
#if (UPG_CFG_VERIFICATION_HARDWARE_ID == YES)
#include "otp.h"
#include "bootloader_configuration.h"
#endif
#include "upg_alloc.h"
#include "upg_config.h"
#include "watchdog.h"
#include "osal_interrupt.h"
#include "upg_definitions_porting.h"
#include "upg_common.h"
#include "upg.h"
#include "upg_common_porting.h"

#define MS_ID_ADDR   0x30
#define CHIP_ID_ADDR 0x108
#define TCXO_REBOOT_DELAY 500ULL

#define uapi_array_size(_array)  (sizeof(_array) / sizeof((_array)[0]))

static upg_image_partition_ids_map_t g_img_partition_map[] = {
    {SSB_IMAGE_ID, PARTITION_SSB_IMAGE},
    {SSB_SIGNATURE_IMAGE_ID, PARTITION_SSB_IMAGE_SIGNATURE},
    {BT_IMAGE_ID, PARTITION_BT_IMAGE},
    {BT_SIGNATURE_IMAGE_ID, PARTITION_BT_IMAGE_SIGNATURE},
    {APPLICATION_IMAGE_ID, PARTITION_ACPU_IMAGE},
    {APPLICATION_SIGNATURE_IMAGE_ID, PARTITION_ACPU_IMAGE_SIGNATURE},
};

/* 获取镜像ID和分区ID的map表
 * map: 镜像ID和分区ID的映射表
 * 返回map表中映射数量
 */
uint32_t upg_get_ids_map(upg_image_partition_ids_map_t **map)
{
    *map = g_img_partition_map;
    return uapi_array_size(g_img_partition_map);
}

/*
 * recovery/APP支持升级的镜像ID
 * 注意增删ID时同步更新升级ID的数量
 */
#define UPDATE_IMAGE_SET \
    { SSB_SIGNATURE_IMAGE_ID, SSB_IMAGE_ID, \
      APPLICATION_SIGNATURE_IMAGE_ID, APPLICATION_IMAGE_ID, \
      BT_SIGNATURE_IMAGE_ID, BT_IMAGE_ID, \
      UPG_IMAGE_ID_NV }
#define UPDATE_IMAGE_SET_CNT 7 /* 镜像ID列表长度 */

static uint32_t g_bs25_support_upg_id[] = UPDATE_IMAGE_SET;
static upg_image_collections_t g_bs25_upg_collect = { g_bs25_support_upg_id, UPDATE_IMAGE_SET_CNT };
/* 获取当前程序支持的升级镜像 */
upg_image_collections_t *uapi_upg_get_image_id_collection(void)
{
    return &g_bs25_upg_collect;
}

/* 获取升级包路径 */
char *upg_get_pkg_file_path(void)
{
    return UPG_FILE_NAME;
}

/* 获取资源索引路径 */
char *upg_get_res_file_index_path(void)
{
    return UPG_RES_INDEX_PATH;
}

/* 获取升级包所在目录 */
char *upg_get_pkg_file_dir(void)
{
    return UPG_FILE_PATH;
}

/* 本次支持升级的镜像ID集合，若为空则无镜像限制 */
bool upg_img_in_set(uint32_t img_id)
{
    upg_image_collections_t *collect = uapi_upg_get_image_id_collection();
    if (collect == NULL || collect->img_ids_cnt == 0) {
        return true;
    }

    for (uint32_t i = 0; i < collect->img_ids_cnt; i++) {
        if (collect->img_ids[i] == img_id) {
            return true;
        }
    }
    return false;
}

/*
 * 获取FOTA升级标记区的Flash起始地址，该地址为在flash上的相对地址，是相对flash基地址的偏移
 * start_address 返回升级标记区的起始地址
 */
errcode_t upg_get_upgrade_flag_flash_start_addr(uint32_t *start_address)
{
    errcode_t ret_val;
    partition_information_t info;

    ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC) {
        return ret_val;
    }

    /* FOTA升级标记区在FOTA分区的最后 */
    *start_address = info.part_info.addr_info.addr + info.part_info.addr_info.size - FOTA_DATA_FLAG_AREA_LEN;
    return ERRCODE_SUCC;
}

/*
 * 获取FOTA升级进度恢复标记区的Flash起始地址，该地址为在flash上的相对地址，是相对flash基地址的偏移
 * start_address 返回标记区的起始地址
 * size 返回包含标记区和flag区的总长度
 */
errcode_t upg_get_progress_status_start_addr(uint32_t *start_address, uint32_t *size)
{
    partition_information_t info;
    errcode_t ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC) {
        return ret_val;
    }

    *start_address = info.part_info.addr_info.addr + info.part_info.addr_info.size - UPG_UPGRADE_FLAG_LENGTH;
    *size = UPG_UPGRADE_FLAG_LENGTH;
    return ERRCODE_SUCC;
}

/*
 * 获取在Flash上预留的FOTA分区的地址和长度，该地址为在flash上的相对地址，是相对flash基地址的偏移
 * start_address 返回FOTA分区的起始地址
 * size          返回FOTA分区大小（包含升级包存储区、升级标记区和缓存区、状态区）
 */
errcode_t upg_get_fota_partiton_area_addr(uint32_t *start_address, uint32_t *size)
{
    partition_information_t info;
    errcode_t ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC) {
        return ret_val;
    }
    *start_address = info.part_info.addr_info.addr;
    *size = info.part_info.addr_info.size;
    return ERRCODE_SUCC;
}

/*
 * 重启
 */
void upg_reboot(void)
{
    /* 重启前睡眠500ms */
    uapi_tcxo_delay_ms((uint64_t)TCXO_REBOOT_DELAY);
    cpu_utils_set_system_status_by_cause(REBOOT_CAUSE_UPG_COMPLETION);
    hal_reboot_chip();
}

/*
 * 防止看门狗超时，踢狗
 */
void upg_watchdog_kick(void)
{
    uapi_watchdog_kick();
}

uint32_t upg_get_flash_base_addr(void)
{
#ifdef USE_EMBED_FLASH
    return EMBED_FLASH_START;
#else
    return FLASH_START;
#endif
}

uint32_t upg_get_flash_size(void)
{
    return UPG_FLASH_SIZE;
}

// 升级读flash接口需支持跨页读取
errcode_t upg_flash_read(const uint32_t flash_offset, const uint32_t size, uint8_t *ram_data)
{
    errcode_t ret = ERRCODE_FAIL;
    uint32_t read_len = 0;
    uint32_t tail_size = 0;
    uint32_t remain_size = 0;
    uint32_t status = 0;
    status = osal_irq_lock();
#ifndef USE_EMBED_FLASH
    if ((errcode_t)uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        goto end;
    }
#endif

    tail_size = UPG_FLASH_PAGE_SIZE - (flash_offset + UPG_FLASH_PAGE_SIZE) % UPG_FLASH_PAGE_SIZE;
    if (tail_size < size) { /* 跨页时，按页读取 */
#ifdef USE_EMBED_FLASH
        if (uapi_eflash_read(flash_offset, (uint32_t *)(uintptr_t)ram_data, tail_size) != ERRCODE_SUCC) {
            goto end;
        }
#else
        read_len = uapi_flash_read_data(0, flash_offset, ram_data, tail_size);
        if (read_len != tail_size) {
            goto end;
        }
#endif
    }

    remain_size = size - read_len;
#ifdef USE_EMBED_FLASH
    if (uapi_eflash_read((flash_offset + read_len), (uint32_t *)(uintptr_t)(ram_data + read_len),
                         remain_size) != ERRCODE_SUCC) {
        goto end;
    }
#else
    read_len = uapi_flash_read_data(0, flash_offset + read_len, ram_data + read_len, remain_size);
    if (read_len != remain_size) { goto end; }
#endif

    ret = ERRCODE_SUCC;
end:
#ifndef USE_EMBED_FLASH
    uapi_flash_switch_to_xip_mode(0);
#endif
    osal_irq_restore(status);
    return ret;
}

STATIC errcode_t upg_erase_before_write(const uint32_t flash_offset, uint32_t size)
{
    uint32_t erase_offset = flash_offset;
    uint32_t erase_size = size;
    uint32_t fst_page_tail_size = UPG_FLASH_PAGE_SIZE - (flash_offset + UPG_FLASH_PAGE_SIZE) % UPG_FLASH_PAGE_SIZE;
    bool not_page_start = flash_offset % UPG_FLASH_PAGE_SIZE != 0 ? true : false;
    bool not_mult_page = fst_page_tail_size >= size ? true : false;
    if (not_page_start && not_mult_page) {
        return ERRCODE_SUCC;
    }

    if (not_page_start) {
        erase_size -= fst_page_tail_size;
        erase_offset += fst_page_tail_size;
    }

    return upg_flash_erase(erase_offset, erase_size);
}

// 升级写flash接口需支持跨页写入和写前擦功能
errcode_t upg_flash_write(const uint32_t flash_offset, uint32_t size, const uint8_t *ram_data, bool do_erase)
{
    errcode_t ret = ERRCODE_FAIL;
    uint32_t write_len = 0;
    uint32_t read_len = 0;
    uint32_t status = 0;
    uint8_t *cmp_data = upg_malloc(size);
    if (cmp_data == NULL) {
        return ERRCODE_FAIL;
    }

    if (do_erase) {
        if (upg_erase_before_write(flash_offset, size) != ERRCODE_SUCC) {
            goto erase_end;
        }
    }

    status = osal_irq_lock();
#ifndef USE_EMBED_FLASH
    uapi_flash_exit_from_xip_mode(0);
    write_len = uapi_flash_write_data(0, flash_offset, ram_data, size);
    if (write_len != size) {
        goto write_end;
    }

    read_len = uapi_flash_read_data(0, flash_offset, cmp_data, size);
    if (read_len != size) {
        goto write_end;
    }
#else
    if (uapi_eflash_write(flash_offset, (uint32_t *)(uintptr_t)ram_data, size) != ERRCODE_SUCC) {
        goto write_end;
    }
    if (uapi_eflash_read(flash_offset, (uint32_t *)(uintptr_t)cmp_data, size) != ERRCODE_SUCC) {
        goto write_end;
    }
    UNUSED(write_len);
    UNUSED(read_len);
#endif
    ret = ERRCODE_SUCC;
write_end:
#ifndef USE_EMBED_FLASH
    uapi_flash_switch_to_xip_mode(0);
#endif
    osal_irq_restore(status);
    if (memcmp(cmp_data, ram_data, size) != 0) {
        ret = ERRCODE_FAIL;
    }
erase_end:
    upg_free(cmp_data);
    return ret;
}

errcode_t upg_flash_erase(const uint32_t flash_offset, const uint32_t size)
{
    errcode_t ret = ERRCODE_FAIL;
    uint32_t status = 0;
    status = osal_irq_lock();
#ifndef USE_EMBED_FLASH
    if ((errcode_t)uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        goto end;
    }
    uapi_flash_block_erase(0, flash_offset, size, true);
#else
    if (uapi_eflash_erase(flash_offset, size) != ERRCODE_SUCC) { goto end; }
#endif
    ret = ERRCODE_SUCC;
end:
#ifndef USE_EMBED_FLASH
    uapi_flash_switch_to_xip_mode(0);
#endif
    osal_irq_restore(status);
    return ret;
}

#if (UPG_CFG_VERIFICATION_SUPPORT == YES)
/*
 * 获取校验用的root_public_key
 */
uint8_t *upg_get_root_public_key(void)
{
#if (UPG_CFG_DIRECT_FLASH_ACCESS == YES)
    /* 使用Upgrader_External_Public_Key校验Key Area的签名  */
    uint32_t upgrader_address = 0;
    partition_information_t info;
    errcode_t ret_val = uapi_partition_get_info(PARTITION_UPGRADER_PRIMARY, &info);
    if (ret_val != ERRCODE_SUCC) {
        return NULL;
    }
    upgrader_address = info.part_info.addr_info.addr;
    image_key_area *upgrader_key = (image_key_area *)(uintptr_t)(upgrader_address + upg_get_flash_base_addr());
    return upgrader_key->ext_pulic_key_area;
#else
    upg_package_header_t *pkg_header = NULL;
    errcode_t              ret;
    ret = upg_get_package_header(&pkg_header);
    if (ret != ERRCODE_SUCC || pkg_header == NULL) {
        upg_msg0("upg_get_package_header fail");
        return NULL;
    }
    static uint8_t public_key[PUBLIC_KEY_LEN];
    memcpy_s(public_key, sizeof(public_key), (pkg_header->key_area.fota_external_public_key), PUBLIC_KEY_LEN);
    return public_key;
#endif
}

STATIC errcode_t check_fota_msid(const uint32_t msid_ext, const uint32_t mask_msid_ext)
{
    unused(msid_ext);
    unused(mask_msid_ext);
    return ERRCODE_SUCC;
}

#if (UPG_CFG_VERIFICATION_HARDWARE_ID == YES)
STATIC errcode_t check_fota_hardwareid(const uint32_t hardwareid)
{
    flash_mode_t flash_mode = bootloader_configuration_get_flash_mode();
    if (hardwareid != flash_mode) {
        upg_msg0("fota flash version does not match!\n");
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}
#endif
/*
 * 检查FOTA升级包中的信息与板端是否匹配
 * pkg_header 升级包包头指针
 * 检查成功，返回 ERRCODE_SUCC
 */
errcode_t upg_check_fota_information(const upg_package_header_t *pkg_header)
{
    upg_key_area_data_t *key_area = (upg_key_area_data_t *)&(pkg_header->key_area);
    upg_fota_info_data_t *fota_info = (upg_fota_info_data_t *)&(pkg_header->info_area);
    errcode_t ret;

    ret = check_fota_msid(key_area->msid_ext, key_area->mask_msid_ext);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = check_fota_msid(fota_info->msid_ext, fota_info->mask_msid_ext);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#if (UPG_CFG_VERIFICATION_HARDWARE_ID == YES)
    ret = check_fota_hardwareid(fota_info->hardware_id);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif
    return ERRCODE_SUCC;
}
#endif