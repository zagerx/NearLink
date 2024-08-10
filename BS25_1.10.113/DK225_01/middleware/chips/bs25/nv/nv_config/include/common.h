/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: NV common header file.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include "stdint.h"
/* 修改此文件后需要先编译A核任意版本生成中间文件application.etypes后才能在编译nv.bin时生效 */
#define BT_CUSTOMIZE_NV_RESERVED 86
#define BT_CUSTOMIZE_CHNL_MAP_LEN 10
/* 基础类型无需在此文件中定义，直接引用即可，对应app.json中的sample0 */

#define AFE_INFO_RESERVED 51
// AFE config total 128 bytes
// #pragma pack(1)
typedef struct {
    uint32_t    magic;                              // 4B  : const value 0x8AA855DDUL
    uint32_t    crc32;                              // 4B  : start : end of crc32, end : structure end.
    uint8_t     product_name;                       // 1B  : afe product name
    uint8_t     hadc_ia_gain;                       // 1B  : adc ia gain: cal 0x0F8
    uint8_t     hadc_pga_gain;                      // 1B  : adc pga gain: cal == 2 0x0F8

    uint8_t     gadc_os_code;                       // 1B  : gadc os cal  0x240
    uint32_t    gadc_wt_b14;                        // 4B  : gadc cap cal 0x0C0
    uint16_t    gadc_wt_b13;                        // 2B  : gadc cap cal 0x0C4
    uint16_t    gadc_wt_b12;                        // 2B  : gadc cap cal 0x0C8
    uint16_t    gadc_wt_b11;                        // 2B  : gadc cap cal 0x0CC
    uint16_t    gadc_wt_b10;                        // 2B  : gadc cap cal 0x0D0
    uint16_t    gadc_wt_b9;                         // 2B  : gadc cap cal 0x0D4
    uint16_t    gadc_wt_b8;                         // 2B  : gadc cap cal 0x0D8
    uint16_t    gadc_wt_b7;                         // 2B  : gadc cap cal 0x0DC
    uint16_t    gadc_wt_b6;                         // 2B  : gadc cap cal 0x0E0
    uint16_t    gadc_gain_coeff;                    // 2B  : gadc gain cal 0x06C, RB 0x070

    uint8_t     hadc_os_code;                       // 1B  : hadc os cal  0x244
    uint8_t     reserved0;                          // 1B  : put here to keep struct align
    uint32_t    hadc_wt_b22;                        // 4B  : hadc cap cal 0x1B0
    uint32_t    hadc_wt_b21;                        // 4B  : hadc cap cal 0x1B4
    uint32_t    hadc_wt_b20;                        // 4B  : hadc cap cal 0x1B8
    uint32_t    hadc_wt_b19;                        // 4B  : hadc cap cal 0x1BC
    uint16_t    hadc_wt_b18;                        // 2B  : hadc cap cal 0x1C0
    uint16_t    hadc_wt_b17;                        // 2B  : hadc cap cal 0x1C4
    uint16_t    hadc_wt_b16;                        // 2B  : hadc cap cal 0x1C8
    uint16_t    hadc_wt_b15;                        // 2B  : hadc cap cal 0x1CC
    uint16_t    hadc_wt_b14;                        // 2B  : hadc cap cal 0x1D0
    uint16_t    hadc_wt_b13;                        // 2B  : hadc cap cal 0x1D4
    uint16_t    hadc_wt_b12;                        // 2B  : hadc cap cal 0x1D8
    uint16_t    hadc_wt_b11;                        // 2B  : hadc cap cal 0x1DC
    uint16_t    hadc_wt_b10;                        // 2B  : hadc cap cal 0x1E0
    uint16_t    hadc_wt_b9;                         // 2B  : hadc cap cal 0x1E4
    uint16_t    hadc_gain_coeff;                    // 2B  : hadc gain cal 0x140, RB 0x144

    int16_t     dac_code1;                          // 2B  : dac Code 0x3D4, RB 0x3D8
    int16_t     dac_code2;                          // 2B  : dac Code backup
    uint8_t     reserved[AFE_INFO_RESERVED - 1];        // 51B : reserved data fill 0xFF
} afe_config_t;
// #pragma pack()

/* 蓝牙地址长度 */
#define BD_ADDR_LEN 6
/* 蓝牙名称长度 */
#define BD_NAME_MAX_LEN       32
/* 蓝牙本端配对地址长度 */
#define BTH_BLE_OWN_ADDR_LEN   24
/* 蓝牙预留长度 */
#define BTH_BLE_RESERVE_LEN   128
/* HOST NV配置数据长度 */
#define BT_CONFIG_DATA_LEN 541
/* HOST NV配置数据长度 */
#define SLE_CONFIG_SYNC_DATA_SIZE 108
#define SLE_SM_OWN_ADDR_LEN 24
#define SLE_NV_RESERV_LEN   128
/* SLDM NV配置主锚点+从锚点数量 */
#define SLDM_NV_ANCHOR_NUM  8
#define SLDM_NV_NUM_OF_FEATURES 8
#define SLDM_NV_NUM_OF_R 12
#define SLDM_NV_HALF_NUM_OF_R 6

/* HOST NV配置结构 */
typedef struct {
    uint8_t bd_addr[BD_ADDR_LEN];
    uint8_t phone_bd_addr[BD_ADDR_LEN];
    uint8_t bd_name[BD_NAME_MAX_LEN];
    uint8_t config_data[BT_CONFIG_DATA_LEN];
} bth_product_data_config_t;
/* HOST NV配置结构 */
typedef struct {
    uint8_t bd_addr[BTH_BLE_OWN_ADDR_LEN];
    uint8_t reserve[BTH_BLE_RESERVE_LEN];
} bth_own_addr_store_stru_t;

/* SLE HOST NV配置结构 */
typedef struct {
    uint8_t sle_addr[BD_ADDR_LEN];
    uint8_t sle_name[BD_NAME_MAX_LEN];
    uint8_t config_data[SLE_CONFIG_SYNC_DATA_SIZE];
} sle_product_data_config_stru_t;

typedef struct {
    uint8_t loc_addr[SLE_SM_OWN_ADDR_LEN];
    uint8_t reserv[SLE_NV_RESERV_LEN];
} sle_product_own_addr_t;

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
} bt_nv_cali_info_type_t;

typedef struct {
    uint32_t nv_customize_config;
    uint32_t nv_customize_config_reserved;
    uint8_t nv_max_ble_tx_power_cfg_val;
    uint8_t nv_max_gle_tx_power_cfg_val;
    uint8_t fcc_flag;
    uint8_t customize_four_k_support;
    uint8_t customize_freq_map[BT_CUSTOMIZE_CHNL_MAP_LEN];
    uint8_t reserved[BT_CUSTOMIZE_NV_RESERVED];
} bt_nv_customize_type_t;

/* sle hadm 配置结构 */
typedef struct {
    uint16_t debug_flag;
    uint8_t posalg_freq;
    uint8_t hadm_addrm[BD_ADDR_LEN];  /* master */
    uint8_t hadm_addrk1[BD_ADDR_LEN];  /* key */
    uint8_t hadm_addrk2[BD_ADDR_LEN];  /* key */
    uint8_t hadm_addr[7][BD_ADDR_LEN];  /* slave0 */
    uint8_t hadm_role;
    uint8_t slave_num;
    uint8_t slave_index;
    uint16_t reserve;
    uint32_t offset[SLDM_NV_ANCHOR_NUM];
    uint32_t x[SLDM_NV_ANCHOR_NUM];
    uint32_t y[SLDM_NV_ANCHOR_NUM];
    uint32_t z[SLDM_NV_ANCHOR_NUM];
    uint32_t threshold_cond2;
    uint16_t rssi_limit;
    uint8_t r_start;
    uint8_t anchor_used_num_fusion;
} sle_hadm_data_config_t;


typedef struct {
    uint32_t e_r[SLDM_NV_NUM_OF_R];
    uint8_t enable_in_out_alg;
    uint32_t calib_val_pos;
} sle_hadm_pos_in_out_config_t;

/* sle hadm 配置结构 */
typedef struct {
    uint32_t intercepts;
    uint32_t index[SLDM_NV_NUM_OF_FEATURES];
    uint32_t mean[SLDM_NV_NUM_OF_FEATURES];
    uint32_t scale[SLDM_NV_NUM_OF_FEATURES];
    uint32_t coef[SLDM_NV_NUM_OF_FEATURES];
} sle_hadm_posalg_config_t;

#endif /* __COMMON_H__ */