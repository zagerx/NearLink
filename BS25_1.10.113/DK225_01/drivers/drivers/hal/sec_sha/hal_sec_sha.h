/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: HAL SEC SHA
 * Author: @CompanyNameTag
 * Create: 2020-01-20
 */

#ifndef HAL_SEC_SHA_H
#define HAL_SEC_SHA_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @addtogroup connectivity_drivers_hal_sec
 * @{
 */
typedef enum {
    SEC_SHA_AUTO_PADDING = 0,
    SEC_SHA_NO_PADDING = 1,
} sec_padding_type_t;

typedef enum {
    SEC_SHA_IVIN_DEFAULT_CFG = 0,
    SEC_SHA_IVIN_USER_CFG = 1,
} sec_ivin_type_t;
/**
 * @brief  Select sha compute mode.
 */
typedef enum {
    SEC_SHA512 = 0,
    SEC_SHA256 = 1,
} sec_sha_compute_mode_t;
/**
 * @brief  Enable sha module.
 */
void hal_sec_sha_enable(void);

/**
 * @brief  Disable sha module.
 */
void hal_sec_sha_disable(void);

/**
 * @brief  Config sha module padding parameter.
 * @param  sha_padding_dis  Auto padding or not, 1:not padding, 0:auto padding.
 */
void hal_sec_sha_cfg_padding(sec_padding_type_t sha_padding_dis);

/**
 * @brief  Config sha module padding parameter.
 * @param  sha_ivin_sel   Select ivin source, 0:default setting, 1:software setting.
 * @param  sha_iniv_addr  Software setting ivin start address.
 */
void hal_sec_sha_cfg_ivin(sec_ivin_type_t sha_ivin_sel, uint32_t sha_iniv_addr);

/**
 * @brief  Config sha module compute parameter.
 * @param  sec_sha_compute_mode   compute mode of sha.
 */
void hal_sec_sha_compute_mode(sec_sha_compute_mode_t sec_sha_compute_mode);
/**
 * @brief  Config sha module data parameter.
 * @param  data_in_addr   Input data address.
 * @param  data_out_addr  Output data address.
 * @param  data_length    The length of input data.
 * @param  total_length   All data length.
 */
void hal_sec_sha_cfg_data(uint32_t data_in_addr, uint32_t data_out_addr, uint32_t data_length, uint32_t total_length);

/**
 * @brief  Start sha check.
 */
void hal_sec_sha_start(void);

/**
 * @brief  Sha check has finished or not.
 * @return True if sha check has finished, otherwise false.
 */
bool hal_sec_sha_is_finish(void);
/**
 * @}
 */
#endif
