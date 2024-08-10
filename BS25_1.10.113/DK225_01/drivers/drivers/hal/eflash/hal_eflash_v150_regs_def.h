/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides V150 eflash register \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-16， Create file. \n
 */

#ifndef HAL_EFLASH_V150_REGS_DEF_H
#define HAL_EFLASH_V150_REGS_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_eflash_v150_regs_def EFLASH V150 Regs Definition
 * @ingroup  drivers_hal_eflash
 * @{
 */

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_tms_0_data {
    uint32_t d32;                                  /*!< Raw register data. */
    struct  {
        uint32_t tms_0                   : 8;      /*!< Mode of read operation, e.g. RECALL and LVCTL set-up time
                                                        before the rising edge of the clock.
                                                        -5us->125x20.345x2
                                                        -Count value = {tMS,1'h0}
                                                        -Recommended values:
                                                        -32M - 'h54 - 5.250us
                                                        -48M - 'h7D
                                                        -64M - 'hA8
                                                        -96M - 'hFA */
        uint32_t reserved                : 8;
    } b;                                           /*!< Register bits. */
} eflash_timing_tms_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_tnvs_0_data {
    uint32_t d32;                                   /*!< Raw register data. */
    struct  {
        uint32_t tnvs_0                  : 8;       /*!< When in prog & erase mode, setup time required for
                                                         PROG/ERASE/Ceb/ARRDN/NVR/NVR_CFG/CHIP/Ax signal to Web pull-up.
                                                         -Count value = {tNVS,1'b0}
                                                         -4us->100x20.345x2
                                                         -Manual requires a value of 4us.
                                                         -Recommended values:
                                                         -32M - 'h43 - 4.1875 us
                                                         -48M - 'h64
                                                         -64M - 'h86
                                                         -96M - 'hC8 */
        uint32_t reserved                : 8;
    } b;
} eflash_timing_tnvs_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_terase_0_data {
    uint32_t d32;                                  /*!< Raw register data. */
    struct  {
        uint32_t terase_0                : 16;     /*!< Time required for sector erase.
                                                        -8-10ms->27650x20.345x16
                                                        -Count value={tERASE,4'b0}
                                                        -Recommended values：
                                                        -32M - 'h4779 - 9.1485ms
                                                        -48M - 'h6B35
                                                        -64M - 'h8EF2
                                                        -96M - 'hD66B */
    } b;
} eflash_timing_terase_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_trcv_sector_0_data {
    uint32_t d32;                                 /*!< Raw register data. */
    struct  {
        uint32_t trcv_sector_0          : 16;     /*!< Recovery time for sector erase.
                                                       -50us->
                                                       -Count value={tRCV_SECTOR}
                                                       -Recommended values:
                                                       -32M - 'h668 - 51.25us
                                                       -48M - 'h99C
                                                       -64M - 'hCD0
                                                       -96M - 'h1338 */
    } b;
} eflash_timing_trcv_sector_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_tsce_0_data {
    uint32_t d32;                                /*!< Raw register data. */
    struct  {
        uint32_t tsce_0                 : 16;    /*!< Time required for chip erase.
                                                      -8-10ms->27650x20.345x16
                                                      -Count value={tERASE,4'b0}
                                                      -Recommended values：
                                                      -32M - 'h4779 - 9.1485ms
                                                      -48M - 'h6B35
                                                      -64M - 'h8EF2
                                                      -96M - 'hD66B */
    } b;
} eflash_timing_tsce_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_trcv_chip_0_data {
    uint32_t d32;                                /*!< Raw register data. */
    struct  {
        uint32_t trcv_chip_0            : 16;    /*!< Recovery time for chip erase.
                                                      -200us->
                                                      -Count value={tRCV_CHIP}
                                                      -Recommended values:
                                                      -32M - 'h199C - 204.875 us
                                                      -48M - 'h266A
                                                      -64M - 'h3338
                                                      -96M - 'h4CD4 */
    } b;
} eflash_timing_trcv_chip_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_tpgs_0_data {
    uint32_t d32;                                /*!< Raw register data. */
    struct  {
        uint32_t tpgs_0                 : 8;     /*!< WEb pull-down to PROG2 pull-up setup time.
                                                      -8us->100x20.345x4
                                                      -Count values={tPGS,2'b0}
                                                      -Manual requires a value of 8us
                                                      -Recommended values:
                                                      -32M - 'h43 - 8.375 us
                                                      -48M - 'h64
                                                      -64M - 'h86
                                                      -96M - 'hC8 */
        uint32_t reserved               : 8;
    } b;
} eflash_timing_tpgs_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_tprog_0_data {
    uint32_t d32;                                /*!< Raw register data. */
    struct  {
        uint32_t tprog_0                : 8;     /*!< 1. PROG pulse time.
                                                      -8-10us->112x20.345x4
                                                      -Count value = {tPROG,2'b0}
                                                      -Recommended values:
                                                      -32M - 'h48 - 9.03125 us
                                                      -48M - 'h6C
                                                      -64M - 'h90
                                                      -96M - 'hD8
                                                      2. preprogram pulse time.
                                                      -4-5us
                                                      -Recommended values:
                                                      -32M - 'h20 - 4 us
                                                      -48M - 'h30
                                                      -64M - 'h40
                                                      -96M - 'h60 */
        uint32_t reserved               : 8;
    } b;
} eflash_timing_tprog_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_trcv_prog_0_data {
    uint32_t d32;                                 /*!< Raw register data. */
    struct  {
        uint32_t trcv_prog_0            : 8;      /*!< Recovery time for PROG.
                                                       -Count value = {tRCV, 1'b0}
                                                       -Recommended values:
                                                       -32M - 'h54 - 5.250us
                                                       -48M - 'h7E
                                                       -64M - 'hA8
                                                       -96M - 'hFA */
        uint32_t reserved               : 8;
    } b;
} eflash_timing_trcv_prog_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_timing_tcfl_0_data {
    uint32_t d32;                                 /*!< Raw register data. */
    struct  {
        uint32_t tcfl_0                 : 8;      /*!< CONFEN recovery time.
                                                       -Count value={tCFL,1'b0}
                                                       -5us->
                                                       -Recommended values:
                                                       -32M - 'h54 - 5.25us
                                                       -48M - 'h7E
                                                       -64M - 'hA8
                                                       -96M - 'hFA */
        uint32_t reserved               : 8;
    } b;
} eflash_timing_tcfl_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_erase_next_rw_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_erase_next_rw_0     : 4;     /*!< Wait time 100ns after Erase pulls low.
                                                              -Counter = {tRW}
                                                              -Recommended values:
                                                              -32M - 'h3 - >125ns
                                                              -48M - 'h5
                                                              -64M - 'h6
                                                              -96M - 'h9 */
        uint32_t reserved                       : 12;
    } b;
} cfg_eflash_erase_next_rw_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_prog_next_rw_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_prog_next_rw_0     : 4;      /*!< Wait time 100ns after PROG pull low.
                                                              -Counter = {tRW}
                                                              -Recommended values:
                                                              -32M - 'h3 - >125ns
                                                              -48M - 'h5
                                                              -64M - 'h6
                                                              -96M - 'h9 */
        uint32_t reserved                      : 12;
    } b;
} cfg_eflash_prog_next_rw_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_recall_time_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_recall_time_0     : 5;       /*!< Recall reads need to wait 200ns.
                                                              -Count value = {tRECALL+1}
                                                              -Recommended values:
                                                              -32M - 'h7 - 218.75ns
                                                              -48M - 'hA
                                                              -64M - 'hF
                                                              -96M - 'h13 */
        uint32_t reserved                     : 11;
    } b;
} cfg_eflash_recall_time_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_lvctl_time_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_lvctl_time_0      : 4;       /*!< Lvctl read needs to wait 50ns.
                                                              -Count value = {tLVCTL+1}
                                                              -Recommended value:
                                                              -32M - 'h1 -64.5ns
                                                              -48M - 'h3
                                                              -64M - 'h3
                                                              -96M - 'h5 */
        uint32_t reserved                     : 12;
    } b;
} cfg_eflash_lvctl_time_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_frequency_sel_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_frequency_cnt_0   : 2;       /*!< 2'h0:
                                                              -32M/48M
                                                              2'h2:
                                                              -96M */
        uint32_t reserved                     : 14;
    } b;
} cfg_eflash_frequency_sel_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_write_read_en_0_data {
    uint32_t d32;                                       /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_write_read_en_0   : 1;      /*!< Enable register for eflash controller. */
        uint32_t reserved                     : 15;
    } b;
} cfg_eflash_write_read_en_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_write_read_protect_0_data {
    uint32_t d32;                                       /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_write_protect_0   : 1;      /*!< eflash write protection.
                                                             -1: eflash can be written.
                                                             -0: eflash write protection is not available. */
        uint32_t cfg_eflash_read_protect_0    : 1;      /*!< eflash read protection.
                                                             -1: eflash can be read.
                                                             -0: eflash cannot be read. */
        uint32_t reserved                     : 14;
    } b;
} cfg_eflash_write_read_protect_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_nvr_recall_0_data {
    uint32_t d32;                                       /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_nvr_recall_0      : 1;      /*!< eflash NVR read method options.
                                                             -1: Read using the recall method.
                                                             -2: Read using the normal method. */
        uint32_t reserved                     : 15;
    } b;
} cfg_eflash_nvr_recall_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_lck_cfg_0_data {
    uint32_t d32;                                       /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_lck_cfg_0         : 1;      /*!< eflash NVR_CFG write protection.
                                                             -1: No erase or write to NVR_CFG allowed.
                                                             -0: Erase or write to NVR_CFG is allowed. */
        uint32_t reserved                     : 15;
    } b;
} cfg_eflash_lck_cfg_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_lvctl_0_data {
    uint32_t d32;                                      /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_lvctl_0           : 1;     /*!< eflash low voltage enable register.
                                                            -1: low voltage enable, 0.8~1.21, read data slower.
                                                            -0: low voltage not enabled, 0.9~1.21. */
        uint32_t reserved                     : 15;
    } b;
} cfg_eflash_lvctl_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_overtime_cnt_high_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_overtime_cnt_high_0    : 4;  /*!< eflash read/write timeout threshold register high 2bit. */
        uint32_t reserved                          : 12;
    } b;
} cfg_eflash_overtime_cnt_high_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_overtime_cnt_low_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_overtime_cnt_low_0    : 16;  /*!< eflash read/write timeout threshold register low 16bit. */
    } b;
} cfg_eflash_overtime_cnt_low_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_erase_0_data {
    uint32_t d32;                                       /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_erase_enable_pulse_0  : 1;  /*!< eflash erase enable pulse register. */
        uint32_t cfg_eflash_erase_chip_0          : 1;  /*!< Chip erase indication signals.
                                                             -1: chip erase
                                                             -0: sector erase */
        uint32_t cfg_eflash_erase_nvr_0           : 1;  /*!< NVR signal at erase. */
        uint32_t cfg_eflash_erase_nvr_cfg_0       : 1;  /*!< NVR_CFG signal on erase. */
        uint32_t cfg_eflash_erase_arrdn_0         : 2;  /*!< ARRDN[1:0] at erase. */
        uint32_t reserved                         : 10;
    } b;
} cfg_eflash_erase_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_erase_addr_0_data {
    uint32_t d32;                                       /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_erase_addr_0          : 8;  /*!< The address of the sector at the time of erase,
                                                             valid only when the sector is erased, default is 0. */
        uint32_t reserved                         : 8;
    } b;
} cfg_eflash_erase_addr_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_confen_0_data {
    uint32_t d32;                                       /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_confen_enable_pulse_0 : 1;  /*!< eflash confen enable pulse register. */
        uint32_t cfg_eflash_confen_addr_0         : 3;  /*!< 3bit address for elfash confen. */
        uint32_t reserved                         : 12;
    } b;
} cfg_eflash_confen_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_confen_data_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_confen_data_0         : 16;  /*!< Data for elfash confen. */
    } b;
} cfg_eflash_confen_data_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_redun0_0_data {
    uint32_t d32;                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_redun0_en_0           : 1;   /*!< eflash block 1 redundant sector replaces
                                                              main array enable. */
        uint32_t cfg_eflash_redun0_addr_0         : 8;   /*!< eflash block 1 redundant sector replaces
                                                              main array address. */
        uint32_t reserved                         : 7;
    } b;
} cfg_eflash_redun0_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union cfg_eflash_redun1_0_data {
    uint32_t d32;                                         /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_redun1_en_0           : 1;    /*!< eflash block 2 redundant sector replaces
                                                              main array enable. */
        uint32_t cfg_eflash_redun1_addr_0         : 8;    /*!< eflash block 2 redundant sector replaces
                                                              main array enable. */
        uint32_t reserved                         : 7;
    } b;
} cfg_eflash_redun1_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_error_resp_mask_0_data {
    uint32_t d32;                                                      /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash_nvr_cfg_write_protect_resp_mask_0   : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_overtime_ahb_resp_mask_0            : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_confen_ahb_resp_mask_0              : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_erase_ahb_resp_mask_0               : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_wrong_size_resp_mask_0              : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_read_protect_ahb_resp_mask_0        : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_write_protect_ahb_resp_mask_0       : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_disable_ahb_resp_mask_0             : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t cfg_eflash_not_ready_resp_mask_0               : 1;   /*!< 1'b0:not shielded.
                                                                            1'b1:Shielded. */
        uint32_t reserved                                       : 7;
    } b;
} eflash_error_resp_mask_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cache_valid_0_data {
    uint32_t d32;                                                       /*!< Raw register data. */
    struct  {
        uint32_t cache_valid_0                                  : 1;    /*!< cache valid signal in eflash control. */
        uint32_t cache_addr_h_0                                 : 1;    /*!< cache addr bit 16 in eflash control. */
        uint32_t reserved                                       : 14;
    } b;
} eflash_cache_valid_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cache_addr_l_0_data {
    uint32_t d32;                                                        /*!< Raw register data. */
    struct  {
        uint32_t cache_addr_l_0                                 : 16;    /*!< cache addr signal in eflash control. */
    } b;
} eflash_cache_addr_l_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_erase_confen_state_0_data {
    uint32_t d32;                                                       /*!< Raw register data. */
    struct  {
        uint32_t chip_erasing_0                                 : 1;    /*!< Chip erase indication signal in
                                                                             eflash control. */
        uint32_t sector_erasing_0                               : 1;    /*!< Sector erase indication signal in
                                                                             eflash control. */
        uint32_t confen_writing_0                               : 1;    /*!< confen indication signal in
                                                                             eflash control. */
        uint32_t reserved                                       : 13;
    } b;
} eflash_erase_confen_state_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union xip_cache_en_0_data {
    uint32_t d32;                                                       /*!< Raw register data. */
    struct  {
        uint32_t xip_cache_en                                   : 1;    /*!< -1'b0：Close XIP CACHE.
                                                                             -1'b1：Open XIP CACHE. */
        uint32_t reserved                                       : 15;
    } b;
} xip_cache_en_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union man_all_0_data {
    uint32_t d32;                                                        /*!< Raw register data. */
    struct  {
        uint32_t man_all_req                                     : 1;    /*!< -1'b0: no INVALID request.
                                                                              -1'b1: INVALID request for CACHE
                                                                               full address. */
        uint32_t man_all_done                                    : 1;    /*!< -1'b0: CACHE full address INVALID
                                                                               operation not completed.
                                                                              -1'b1: CACHE full address INVALID
                                                                               operation completed. */
        uint32_t reserved                                        : 14;
    } b;
} man_all_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union stb_eflash_mux_data {
    uint32_t d32;                                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_stb_eflash0_test_en                         : 1;    /*!< -1'b0: Function branch.
                                                                              -1'b1: Test branch. */
        uint32_t cfg_stb_eflash1_test_en                         : 1;    /*!< -1'b0: Function branch.
                                                                              -1'b1: Test branch. */
        uint32_t reserved                                        : 14;
    } b;
} stb_eflash_mux_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_por_dpd_sel_data {
    uint32_t d32;                                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash0_por_dbg_sel                         : 1;    /*!< -0: Selects the signal coming from
                                                                               the state machine.
                                                                              -1: Selects the DBG signal. */
        uint32_t cfg_eflash0_dpd_dbg_sel                         : 1;    /*!< -0: Selects the signal coming from
                                                                               the state machine.
                                                                              -1: Selects the DBG signal. */
        uint32_t cfg_eflash1_por_dbg_sel                         : 1;    /*!< -0: Selects the signal coming from
                                                                               the state machine.
                                                                              -1: Selects the DBG signal. */
        uint32_t cfg_eflash1_dpd_dbg_sel                         : 1;    /*!< -0: Selects the signal coming from
                                                                               the state machine.
                                                                              -1: Selects the DBG signal. */
        uint32_t reserved                                        : 12;
    } b;
} eflash_por_dpd_sel_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_por_dpd_man_data {
    uint32_t d32;                                                        /*!< Raw register data. */
    struct  {
        uint32_t cfg_eflash0_por_dbg                             : 1;    /*!< -0: Reset EFLASH.
                                                                              -1: Unreset EFLASH. */
        uint32_t cfg_eflash0_dpd_dbg                             : 1;    /*!< -0: Exit low-power mode
                                                                              -1: Enter low-power mode */
        uint32_t cfg_eflash1_por_dbg                             : 1;    /*!< -0: Reset EFLASH.
                                                                              -1: Unreset EFLASH. */
        uint32_t cfg_eflash1_dpd_dbg                             : 1;    /*!< -0: Exit low-power mode
                                                                              -1: Enter low-power mode */
        uint32_t reserved                                        : 12;
    } b;
} eflash_por_dpd_man_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cfg_0_data {
    uint32_t d32;
    struct  {
        uint32_t eflash0_cfg_0                                   : 16;   /*!< [0]:eflash0_vdd11_pd:
                                                                              -1'b1:vdd11 power off
                                                                              -1'b0:vdd11 power on */
    } b;
} eflash_cfg_0_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cfg_1_data {
    uint32_t d32;
    struct  {
        uint32_t cfg_eflash_erase_verify_en                      : 1;   /*!< cfg_eflash_erase_verify_en:
                                                                             -1'b1: Run the erase verify mode for
                                                                             sector erase.
                                                                             -0'b0: Run the single pulse mode for
                                                                             sector erase. */
        uint32_t cfg_eflash_preprogram                           : 1;   /*!< cfg_eflash_preprogram:
                                                                             -1'b1: Pull up PREPG.
                                                                             -0'b0：Pull down PREPG. */
        uint32_t reserved                                        : 14;
    } b;
} eflash_cfg_1_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cfg_2_data {
    uint32_t d32;
    struct  {
        uint32_t eflash0_cfg_2                                   : 16;  /*!< [8:0]: cfg_eflash_verify_addr_step:
                                                                             Configures the accumulated step for
                                                                             each verify read address.
                                                                             -Recommended value:'h1. */
    } b;
} eflash_cfg_2_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cfg_3_data {
    uint32_t d32;
    struct  {
        uint32_t eflash0_cfg_3                                   : 16;  /*!< [8:0]: cfg_eflash_verify_addr_times:
                                                                             Configure the total number of verify reads.
                                                                             -Recommended value: 'h1FF */
    } b;
} eflash_cfg_3_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cfg_4_data {
    uint32_t d32;
    struct  {
        uint32_t eflash0_cfg_4                                   : 16;  /*!< [3:0]: cfg_eflash_eraver_erase_times:
                                                                             Configure the maximum number of cycles for
                                                                             erase_verify.
                                                                             Do not configure the value to exceed 'hA.
                                                                             -Recommended value: 'hA. */
    } b;
} eflash_cfg_4_data_t;

/**
 * @brief  This union represents the bit fields in the Control Register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union eflash_cfg_5_data {
    uint32_t d32;
    struct  {
        uint32_t eflash0_cfg_5                                   : 16;  /*!< [15:0]: tERASE_VER:
                                                                             Configure the pulse time of a single pulse
                                                                             in erase_verify. The required interval is
                                                                             0.8ms to 1ms.
                                                                             (The control granularity here is 1 for
                                                                              4 clock cycles)
                                                                             -Count value = {tERASE_VER,2'b0}
                                                                             -Recommended values:
                                                                             -32M - 'h1C20 Corresponding time
                                                                             approx. 0.9ms.
                                                                             -48M - 'h2A30.
                                                                             -64M - 'h3840.
                                                                             -96M - 'h5460. */
    } b;
} eflash_cfg_5_data_t;

/**
 * @brief  Eflash_L2 function registers.
 */
typedef struct eflash_l2_regs {
    volatile uint32_t cfg_0;                                           /*!< eflash_cfg_0.      <i>Offset: F00h</i>. */
    volatile uint32_t cfg_1;                                           /*!< eflash_cfg_1.      <i>Offset: F04h</i>. */
    volatile uint32_t cfg_2;                                           /*!< eflash_cfg_2.      <i>Offset: F08h</i>. */
    volatile uint32_t cfg_3;                                           /*!< eflash_cfg_3.      <i>Offset: F0ch</i>. */
    volatile uint32_t cfg_4;                                           /*!< eflash_cfg_4.      <i>Offset: F10h</i>. */
    volatile uint32_t cfg_5;                                           /*!< eflash_cfg_5.      <i>Offset: F14h</i>. */
} eflash_l2_regs_t;

/**
 * @brief  XIP CACHE control registers.
 */
typedef struct xip_cache_regs {
    volatile uint32_t xip_cache_en;                                  /*!< xip_cache_en.        <i>Offset: 100h</i>. */
} xip_cache_regs_t;

/**
 * @brief  XIP CACHE consistency control registers.
 */
typedef struct xip_cache_man_regs {
    volatile uint32_t man_all;                                       /*!< man_all.             <i>Offset: 17ch</i>. */
} xip_cache_man_regs_t;

/**
 * @brief  STB eflash mux registers.
 */
typedef struct stb_eflash_mux_regs {
    volatile uint32_t stb_mux;                                       /*!< stb_mux.             <i>Offset: E00h</i>. */
} stb_eflash_mux_regs_t;

/**
 * @brief  Eflash low power configuration registers.
 */
typedef struct eflash_low_power_regs {
    volatile uint32_t dpd_sel;                                       /*!< dpd_sel.             <i>Offset: 920h</i>. */
    volatile uint32_t reserved0;                                     /*!< reserved.            <i>Offset: 924h</i>. */
    volatile uint32_t reserved1;                                     /*!< reserved.            <i>Offset: 928h</i>. */
    volatile uint32_t reserved2;                                     /*!< reserved.            <i>Offset: 92ch</i>. */
    volatile uint32_t dpd_man;                                       /*!< dpd_man.             <i>Offset: 930h</i>. */
} eflash_low_power_regs_t;

/**
 * @brief  Eflash time registers.
 */
typedef struct eflash_regs {
    volatile uint32_t time_reg;                                  /*!< time_reg.           <i>Offset: 00h</i>. */
    volatile uint32_t timing_tnvs;                               /*!< timing_tnvs.        <i>Offset: 04h</i>. */
    volatile uint32_t timing_terase;                             /*!< timing_terase.      <i>Offset: 08h</i>. */
    volatile uint32_t timing_trcv_sector;                        /*!< timing_trcv_sector. <i>Offset: 0ch</i>. */
    volatile uint32_t timing_tsce;                               /*!< timing_tsce.        <i>Offset: 10h</i>. */
    volatile uint32_t timing_trcv_chip;                          /*!< timing_trcv_chip.   <i>Offset: 14h</i>. */
    volatile uint32_t timing_tpgs;                               /*!< timing_tpgs.        <i>Offset: 18h</i>. */
    volatile uint32_t timing_tprog;                              /*!< timing_tprog.       <i>Offset: 1ch</i>. */
    volatile uint32_t timing_trcv_prog;                          /*!< timing_trcv_prog.   <i>Offset: 20h</i>. */
    volatile uint32_t timing_tcfl;                               /*!< timing_tcfl.        <i>Offset: 24h</i>. */
    volatile uint32_t erase_next_rw;                             /*!< erase_next_rw.      <i>Offset: 28h</i>. */
    volatile uint32_t prog_next_rw;                              /*!< prog_next_rw.       <i>Offset: 2ch</i>. */
    volatile uint32_t recall_time;                               /*!< recall_time.        <i>Offset: 30h</i>. */
    volatile uint32_t lvctl_time;                                /*!< lvctl_time.         <i>Offset: 34h</i>. */
    volatile uint32_t frequency_sel;                             /*!< frequency_sel.      <i>Offset: 38h</i>. */
    volatile uint32_t reserved;                                  /*!< reserved.           <i>Offset: 3ch</i>. */
    volatile uint32_t write_read_reg;                            /*!< write_read_reg.     <i>Offset: 40h</i>. */
    volatile uint32_t write_read_protect;                        /*!< write_read_protect. <i>Offset: 44h</i>. */
    volatile uint32_t nvr_recall;                                /*!< nvr_recall.         <i>Offset: 48h</i>. */
    volatile uint32_t lck_cfg;                                   /*!< lck_cfg.            <i>Offset: 4ch</i>. */
    volatile uint32_t lvctl;                                     /*!< lvctl.              <i>Offset: 50h</i>. */
    volatile uint32_t overtime_cnt_high;                         /*!< overtime_cnt_high.  <i>Offset: 54h</i>. */
    volatile uint32_t overtime_cnt_low;                          /*!< overtime_cnt_low.   <i>Offset: 58h</i>. */
    volatile uint32_t erase_reg;                                 /*!< erase_reg.          <i>Offset: 5ch</i>. */
    volatile uint32_t erase_addr;                                /*!< erase_addr.         <i>Offset: 60h</i>. */
    volatile uint32_t confen;                                    /*!< confen.             <i>Offset: 64h</i>. */
    volatile uint32_t confen_data;                               /*!< confen_data.        <i>Offset: 68h</i>. */
    volatile uint32_t redun0;                                    /*!< redun0.             <i>Offset: 6ch</i>. */
    volatile uint32_t redun1;                                    /*!< redun1.             <i>Offset: 70h</i>. */
    volatile uint32_t reserved0;                                 /*!< reserved0.          <i>Offset: 74h</i>. */
    volatile uint32_t reserved1;                                 /*!< reserved1.          <i>Offset: 78h</i>. */
    volatile uint32_t reserved2;                                 /*!< reserved2.          <i>Offset: 7ch</i>. */
    volatile uint32_t reserved3;                                 /*!< reserved3.          <i>Offset: 80h</i>. */
    volatile uint32_t reserved4;                                 /*!< reserved4.          <i>Offset: 84h</i>. */
    volatile uint32_t reserved5;                                 /*!< reserved5.          <i>Offset: 88h</i>. */
    volatile uint32_t reserved6;                                 /*!< reserved6.          <i>Offset: 8ch</i>. */
    volatile uint32_t cache_valid;                               /*!< cache_valid.        <i>Offset: 90h</i>. */
    volatile uint32_t cache_addr;                                /*!< cache_addr.         <i>Offset: 94h</i>. */
    volatile uint32_t erase_confen_state;                        /*!< erase_confen_state. <i>Offset: 98h</i>. */
} eflash_regs_t;
/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif