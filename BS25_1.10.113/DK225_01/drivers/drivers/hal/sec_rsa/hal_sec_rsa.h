/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: HAL RSA.
 * Author: @CompanyNameTagTag
 * Create: 2022-04-11
 */

#ifndef HAL_SEC_RSA_H
#define HAL_SEC_RSA_H

#include <stdint.h>
#include <stdbool.h>
#include "pal_sec_config_port.h"
#include "sec_rsa_port.h"

#define SEC_RSA_RAM_CLEAR_EN       1
#define SEC_RSA_RAM_CLEAR_DIS      0

/** @addtogroup connectivity_drivers_hal_sec
  * @{
  */
typedef enum {
    KEY_LENGTH_1024 = 0,
    KEY_LENGTH_2048,
    KEY_LENGTH_4096
} rsa_key_width_t;

typedef enum {
    RSA_DECRYPT = 0,
    RSA_UPDATE_KEY,
    RSA_RAM_CLR,
    RSA_CRC16_DEBUG
} rsa_mode_t;

typedef struct {
    uint32_t sec_rsa_mode : 2;
    uint32_t sec_rsa_key_width : 2;
    uint32_t sec_rsa_data0_clr : 1;
    uint32_t sec_rsa_data1_clr : 1;
    uint32_t sec_rsa_data2_clr : 1;
    uint32_t reserved : 25;
} rsa_mod_reg_t;

typedef struct {
    uint32_t addr_n : 8;
    uint32_t addre_d : 8;
    uint32_t addre_pkt : 8;
    uint32_t addere_rslt : 8;
} rsa_addr_reg_t;

typedef struct {
    uint32_t addr_n : 8;
    uint32_t addre_d : 8;
    uint32_t addre_pkt : 8;
    uint32_t addere_rslt : 8;
} rsa_error_reg_t;

typedef struct {
    rsa_mod_reg_t mode;
    rsa_addr_reg_t addr;
    rsa_error_reg_t error;
    uint32_t wsec;
    uint32_t wpkt;
    uint32_t rrslt;
} rsav2_alg_cfg_t;

/**
 * @brief  Enable rsav2 module.
 */
void hal_sec_rsav2_enable(void);

/**
 * @brief  Disable rsav2 module.
 */
void hal_sec_rsav2_disable(void);

/**
 * @brief  Config rsav2 module parameter.
 * @param  rsa_alg_cfg  Input parameter for rsav2.
 */
void hal_sec_rsav2_cfg(rsav2_alg_cfg_t rsa_alg_cfg);

/**
 * @brief  Start rsav2.
 */
void hal_sec_rsav2_start(void);

/**
 * @brief  Rsav2 has finished or not.
 * @return True if rsav2 has finished, otherwise false.
 */
bool hal_sec_rsav2_is_finish(void);

/**
 * @brief  Copy rsa result.
 * @param  rsa_alg_cfg  Input parameter for rsav2.
 */
void hal_sec_rsav2_copy_result(rsav2_alg_cfg_t rsa_alg_cfg);

/**
 * @brief  Clear plain text, key and rsa result in ram.
 * @param  rsa_alg_cfg  Input parameter for rsa.
 */
bool hal_sec_rsav2_clear_ram(rsav2_alg_cfg_t rsa_alg_cfg);

/**
 * @brief  CRC for rsa key.
 * @param  rsa_alg_cfg  Input parameter for rsa.
 */
bool hal_sec_rsav2_clear_key(rsav2_alg_cfg_t rsa_alg_cfg);
/**
  * @}
  */
#endif
