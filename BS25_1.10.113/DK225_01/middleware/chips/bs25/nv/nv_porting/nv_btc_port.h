/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: NON VOLATILE STORAGE MODULE -- CPU CONFIG
 */

#ifndef NV_STORAGE_CORE_H
#define NV_STORAGE_CORE_H

#include "ipc.h"
#include "osal_semaphore.h"

#define BT_CUSTOMIZE_NV_VALID 21
#define BT_NV_START_OFFSET 8
#define BT_NV_CUSTOMIZE_PARAM 2
#define CONFIG_NV_AUTO 0 // 校准值通过单条命令写NV，产测收集校准值阶段暂时关闭
extern osal_semaphore sem_config_nv; // 校准写NV信号量，校准命令后释放，写入NV后回收

typedef struct {
    uint16_t config_nv_1;
    uint16_t config_nv_2;
    uint16_t config_nv_3;
    uint16_t config_nv_4;
    uint16_t config_nv_5[5];
    uint16_t config_nv_6;
    uint16_t config_nv_7;
    uint16_t config_nv_8;
    uint16_t config_nv_9;
    uint8_t config_nv_10;
    int8_t config_nv_11;
    int8_t config_nv_12;
    uint8_t flag;
} config_nv_info_struct;

typedef struct {
    uint32_t nv_customize_config;
    uint32_t nv_customize_config_reserved;
    uint8_t nv_max_ble_tx_power_cfg_val;
    uint8_t nv_max_gle_tx_power_cfg_val;
    uint8_t fcc_flag;
    uint8_t customize_four_k_support;
    uint8_t customize_freq_map[BT_CUSTOMIZE_CHNL_MAP_LEN];
    uint8_t reserved[BT_CUSTOMIZE_NV_RESERVED];
} bt_nv_customize_info_struct;


void app_read_nv_msg(void);
void app_read_nv_customize_msg(void);
void ipc_app_read_nv_msg(void);
void ipc_app_write_nv_msg(void);
void ipc_app_read_nv_customize_msg(void);
void btc_chnl_75_disable_reduction(uint8_t flag);
#if CONFIG_NV_AUTO
void app_write_config_nv_sem(void);
#endif
int app_write_config_nv_act(int argc, char **argv);
#endif
