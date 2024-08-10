/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: SEC RSAV2
 * Author: @CompanyNameTag
 * Create: 2020-01-20
 */

#ifndef SEC_RSAV2_H
#define SEC_RSAV2_H

#include "hal_sec_common.h"
#include "pal_sec_config_port.h"
#include "sec_rsa_port.h"
#if SEC_NEW_RSA == YES
#include "hal_sec_rsa.h"
#else
#include "hal_sec_rsav2.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @addtogroup connectivity_drivers_non_os_sec
 * @{
 */

/**
 * @brief  The definition of security rsa context.
 */
typedef struct {
    uint32_t is_inited;
    rsav2_alg_cfg_t rsa_alg_cfg;
} sec_rsa_context_t;

/**
 * @brief  Get the context of security rsa.
 * @return The context of security rsa.
 */
sec_rsa_context_t *sec_rsa_get_context(void);

/**
 * @brief  Initialise the rsa driver module.
 */
void sec_rsav2_init(void);

/**
 * @brief  De-initialise the rsa driver module.
 */
void sec_rsav2_deinit(void);

/**
 * @brief  Start rsa.
 * @param  rsav2_alg_cfg  Input parameter for rsav2.
 */
void sec_rsav2_start(rsav2_alg_cfg_t rsav2_alg_cfg);

/**
 * @brief  Get rsa result.
 * @return True if rsa check finish.
 */
bool sec_rsav2_get_result(void);

/**
 * @brief  Register callback of rsa and enable interrupt.
 * @param  callback The function to register.
 */
bool sec_rsav2_register_callback(SEC_CALLBACK callback);

/**
 * @brief  Unregister callback of rsa, disable and clear interrupt.
 */
void sec_rsav2_unregister_callback(void);

/**
 * @brief  Rsa calulate function.
 * @param  text_addr The address of plainotask_ptext_addr_text.
 * @param  key_addr  The address of otask_key_addr.
 * @param  result_addr The address of otask_result_addr.
 * @return True if success.
 */
bool sec_rsav2_calculate(uint32_t text_addr, uint32_t key_addr, uint32_t result_addr);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
