/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: HAL CPU CORE MODULE
 * Author: @CompanyNameTag
 * Create: 2018-10-15
 */
#if !defined BUILD_APPLICATION_ROM && !defined BUILD_APPLICATION_SSB
#include "hal_cpu_hifi.h"
#include "memory_config.h"
#include "debug_print.h"
#include "securec.h"
#include "panic.h"
#ifdef BUILD_APPLICATION_STANDARD
#include "log_oam_logger.h"
#include "log_def.h"
#include "log_printf.h"
#endif

#define HAL_CPU_HIFI_CODE_ADDR_MASK 0x00FFFFFF

// bt mapping dsp0 address 0xe8xxxxxx - > 0x58xxxxxx
#define HAL_CPU_HIFI0_DRAM_START  0x58014000  // DSP stack heap start
#define HAL_CPU_HIFI0_DRAM_END    0x5803c000
#define HAL_CPU_HIFI0_IRAM_START  0x5803c000  // DSP code verctor start
#define HAL_CPU_HIFI0_IRAM_END    0x58048000
#define HAL_CPU_HIFI_OCRAM_START 0x5808c000  // DSP .text  .rodata .data .bss
#define HAL_CPU_HIFI_OCRAM_END   (0x5815c000 + DSP_EXTEND_OCRAM_SIZE)

// dsp0 map
#define HAL_HIFI_DRAM_START  0xe8058000  // DSP stack heap start
#define HAL_HIFI_DRAM_END    0xe8080000
#define HAL_HIFI_IRAM_START  0xe8080000  // DSP code verctor start
#define HAL_HIFI_IRAM_END    0xe808c000
#define HAL_HIFI0_IRAM_OFFSET 0x44000
#define HAL_HIFI0_DRAM_OFFSET 0x44000

// bt mapping dsp1 address 0xe8xxxxxx - > 0x58xxxxxx
#define HAL_CPU_HIFI1_DRAM_START  0x58341000  // DSP stack heap start
#define HAL_CPU_HIFI1_DRAM_END    0x58369000
#define HAL_CPU_HIFI1_IRAM_START  0x58369000  // DSP code verctor start
#define HAL_CPU_HIFI1_IRAM_END    0x58375000

// dsp1 map
#define HAL_HIFI1_IRAM_OFFSET 0x2e9000
#define HAL_HIFI1_DRAM_OFFSET 0x2e9000

#define HIFI_IMAGE_COPY_LEN 4 // DSP need copy 4 bytes one by one
#define writel_hifi(addr, d)   (*(volatile uint32_t*)(addr) = (uint32_t)(d))
#define readl_hifi(addr)    (*(volatile uint32_t*)(addr))

static uint32_t g_hifi_memset_len = 0;
static uint32_t g_hifi_memcpy_len = 0;

static size_t hifi_dest_max_size(uint32_t des, drv_hifi_e hifi_num)
{
    size_t dest_max_size = 0;

    if (hifi_num == DRV_HIFI_0) {
        if ((des >= HAL_CPU_HIFI0_IRAM_START) && (des < HAL_CPU_HIFI0_IRAM_END)) {
            dest_max_size = HAL_CPU_HIFI0_IRAM_END - des;
        } else if ((des >= HAL_CPU_HIFI0_DRAM_START) && (des < HAL_CPU_HIFI0_DRAM_END)) {
            dest_max_size = HAL_CPU_HIFI0_DRAM_END - des;
        } else if ((des >= HAL_CPU_HIFI_OCRAM_START) && (des < HAL_CPU_HIFI_OCRAM_END)) {
            dest_max_size = HAL_CPU_HIFI_OCRAM_END - des;
        } else {
            PRINT("des is error ,des:0x%x,%s,%d\r\n", des, __func__, __LINE__);
#ifdef BUILD_APPLICATION_STANDARD
            oml_pf_log_print1(LOG_BCORE_PLT_LPM_DSP_POWER, LOG_NUM_LPM_DSP_POWER, LOG_LEVEL_INFO, \
                "[LOW_POWER]des is error ,des:0x%x,%s,%d\r\n", des);
#endif
            panic(PANIC_CPU_HIFI, __LINE__);
            return 0;   //lint !e527 Unreachable code
        }
    } else {
        if ((des >= HAL_CPU_HIFI1_IRAM_START) && (des < HAL_CPU_HIFI1_IRAM_END)) {
            dest_max_size = HAL_CPU_HIFI1_IRAM_END - des;
        } else if ((des >= HAL_CPU_HIFI1_DRAM_START) && (des < HAL_CPU_HIFI1_DRAM_END)) {
            dest_max_size = HAL_CPU_HIFI1_DRAM_END - des;
        } else if ((des >= HAL_CPU_HIFI_OCRAM_START) && (des < HAL_CPU_HIFI_OCRAM_END)) {
            dest_max_size = HAL_CPU_HIFI_OCRAM_END - des;
        } else {
            PRINT("des is error ,des:0x%x,%s,%d\r\n", des, __func__, __LINE__);
#ifdef BUILD_APPLICATION_STANDARD
            oml_pf_log_print1(LOG_BCORE_PLT_LPM_DSP_POWER, LOG_NUM_LPM_DSP_POWER, LOG_LEVEL_INFO, \
                "[LOW_POWER]des is error ,des:0x%x,%s,%d\r\n", des);
#endif
            panic(PANIC_CPU_HIFI, __LINE__);
            return 0;   //lint !e527 Unreachable code
        }
    }
    return dest_max_size;
}
static void hifi_img_sec_dl_reg(const uint32_t *src, uint32_t des, uint32_t size, uint8_t type, drv_hifi_e hifi_num)
{
    if ((src == NULL) || ((size & 0x3) != 0) || (size == 0)) {
        PRINT("img download size:0x%x is not 4 byte aligned, type:%d\r\n", size, type);
#ifdef BUILD_APPLICATION_STANDARD
        oml_pf_log_print2(LOG_BCORE_PLT_LPM_DSP_POWER, LOG_NUM_LPM_DSP_POWER, LOG_LEVEL_INFO, \
            "[LOW_POWER]img download size:0x%x is not 4 byte aligned, type:%d\r\n", size, type);
#endif
        return;
    }

    size_t dest_max_size;
    uint32_t offset;

    dest_max_size = hifi_dest_max_size(des, hifi_num);
    if (size > dest_max_size) {
        PRINT("Memory process error\r\n");
#ifdef BUILD_APPLICATION_STANDARD
        oml_pf_log_print0(LOG_BCORE_PLT_LPM_CODEC_POWER, LOG_NUM_LPM_CODEC_POWER, LOG_LEVEL_INFO, \
            "[LOW_POWER]Memory process error");
#endif
        panic(PANIC_CPU_HIFI, __LINE__);
    }

    switch (type) {
        case DRV_HIFI_IMAGE_SEC_TYPE_BSS:
            PRINT("memset,size:0x%x,%s,%d\r\n", size, __func__, __LINE__);
            for (offset = 0; offset < size; offset += HIFI_IMAGE_COPY_LEN) {
                writel_hifi(des + offset, 0);
            }
            g_hifi_memset_len += size;
            break;
        case DRV_HIFI_IMAGE_SEC_TYPE_CODE:
        case DRV_HIFI_IMAGE_SEC_TYPE_DATA:
            for (offset = 0; offset < size; offset += HIFI_IMAGE_COPY_LEN) {
                writel_hifi(des + offset, readl_hifi((uintptr_t)src + offset));
            }
            g_hifi_memcpy_len += size;
            break;
        default:
            break;
    }
}

void hifi_download(const struct drv_hifi_image_head *data, drv_hifi_e hifi_num)
{
    struct drv_hifi_image_head *head = NULL;
    uint32_t dest_addr;
    uint32_t *src_addr = NULL;
    uint32_t i;

    head = (struct drv_hifi_image_head *)data;
    if (head == NULL) {
        PRINT("img head is null\n");
        return;
    }

    if (hifi_num >= DRV_HIFI_BUTT) { return; }

    for (i = 0; i < head->sections_num; i++) {
        src_addr = (uint32_t *)((char *)head + head->sections[i].src_offset);
        dest_addr = head->sections[i].des_addr;
        PRINT("copy sections num = %x\r\n", i);
        PRINT("image_size = %x\r\n", head->image_size);
        PRINT("sections_num = %x\r\n", head->sections_num);
        PRINT("load_attib = %d\r\n", head->sections[i].load_attib);
        PRINT("size = 0x%x\r\n", head->sections[i].size);
        PRINT("sn = %d\r\n", head->sections[i].sn);
        PRINT("src_offset = 0x%x\r\n", head->sections[i].src_offset);
        PRINT("type = %d\r\n", head->sections[i].type);

        if ((dest_addr >= HAL_HIFI_IRAM_START) && (dest_addr < HAL_HIFI_IRAM_END)) {
            if (hifi_num == DRV_HIFI_0) {
                dest_addr -= HAL_HIFI0_IRAM_OFFSET;
            } else {
                dest_addr += HAL_HIFI1_IRAM_OFFSET;
            }
        } else if ((dest_addr >= HAL_HIFI_DRAM_START) && (dest_addr < HAL_HIFI_DRAM_END)) {
            if (hifi_num == DRV_HIFI_0) {
                dest_addr -= HAL_HIFI0_DRAM_OFFSET;
            } else {
                dest_addr += HAL_HIFI1_DRAM_OFFSET;
            }
        }
        dest_addr &= HAL_CPU_HIFI_CODE_ADDR_MASK;
        dest_addr |= DSP_CODE_ADDR_BT_MAPPING;

        hifi_img_sec_dl_reg(src_addr, dest_addr, head->sections[i].size, head->sections[i].type, hifi_num);
    }

    PRINT("[LOW_POWER]DSP img memset size:0x%x , memcpy size:0x%xr\n", g_hifi_memset_len, g_hifi_memcpy_len);
    g_hifi_memset_len = 0;
    g_hifi_memcpy_len = 0;
    UNUSED(g_hifi_memset_len);
    UNUSED(g_hifi_memcpy_len);
    PRINT("img dl--r\n");
}
#endif
