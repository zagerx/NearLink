/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  HAL CPU CORE HEADER
 * Author: @CompanyNameTag
 * Create: 2018-10-15
 */

#ifndef HAL_CPU_HIFI_H
#define HAL_CPU_HIFI_H

/** @defgroup connectivity_drivers_hal_cpu_hifi CPU Hifi
  * @ingroup  connectivity_drivers_hal
  * @{
  */
#if !defined BUILD_APPLICATION_ROM && !defined BUILD_APPLICATION_SSB

#define HIFI_SEC_MAX_NUM             32
#define HIFI_TIME_STAMP_MAX_CHAR_NUM 120

/**
 * @brief  dsp img download
 */
enum DRV_HIFI_IMAGE_SEC_TYPE_ENUM {
    DRV_HIFI_IMAGE_SEC_TYPE_CODE = 0, /* code section */
    DRV_HIFI_IMAGE_SEC_TYPE_DATA,     /* data section */
    DRV_HIFI_IMAGE_SEC_TYPE_BSS,      /* bss section */
    DRV_HIFI_IMAGE_SEC_TYPE_BUTT,
};

enum DRV_HIFI_IMAGE_SEC_LOAD_ENUM {
    DRV_HIFI_IMAGE_SEC_LOAD_STATIC = 0, /* before dsp reset  download one time */
    DRV_HIFI_IMAGE_SEC_LOAD_DYNAMIC,    /* maybe need download dynamic */
    DRV_HIFI_IMAGE_SEC_UNLOAD,          /* the section do not need download */
    DRV_HIFI_IMAGE_SEC_LOAD_BUTT,
};

struct drv_hifi_image_sec {
    unsigned short sn;        /* section serial number */
    unsigned char type;       /* section type :code, data, bss    */
    unsigned char load_attib; /* download attribute:static,dynmic,unload */
    unsigned int src_offset;  /* offset of down file, bytes */
    unsigned int des_addr;    /* des addr , bytes */
    unsigned int size;        /* section length, bytes */
};

struct drv_hifi_image_head {
    char time_stamp[HIFI_TIME_STAMP_MAX_CHAR_NUM];        /* image time stamp */
    unsigned int image_size;                              /* image size, bytes */
    unsigned int sections_num;                            /* section number */
    struct drv_hifi_image_sec sections[HIFI_SEC_MAX_NUM]; /* section head include section infomation */
};

typedef enum {
    DRV_HIFI_0 = 0, /* hifi1 */
    DRV_HIFI_1,    /* hifi1 */
    DRV_HIFI_BUTT,
} drv_hifi_e;

void hifi_download(const struct drv_hifi_image_head *data, drv_hifi_e hifi_num);
#endif

/**
  * @}
  */
#endif
