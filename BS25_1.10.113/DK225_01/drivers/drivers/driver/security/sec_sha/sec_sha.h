/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: SEC SHA
 * Author: @CompanyNameTag
 * Create: 2020-01-20
 */

#ifndef SEC_SHA_H
#define SEC_SHA_H

#include "hal_sec_common.h"
#include "hal_sec_sha.h"
#include "pal_sec_config_port.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @addtogroup connectivity_drivers_non_os_sec
 * @{
 */
#define SHA256_HASH_SIZE      32
#if SHA512_EXIST == YES
#define SHA512_HASH_SIZE      64
#endif

/**
 * @brief Definition of sec sha256 return type.
 */
typedef enum {
    SEC_SHA_RET_OK,           //!< Sec sha return ok.
    SEC_SHA_RET_TIMEOUT,      //!< Sec sha calculate timeout.
    SEC_SHA_RET_MEMCOPY_FAIL, //!< sec sha memcopy failed.
    SEC_SHA_RET_INIT_ERROR    //!< sec sha init state error.
} sec_sha_ret_type_t;

/**
 * @brief Get sec sha calculate context info.
 * @param  len  The length of context.
 * @return uint8_t* return the address of sha context.
 */
uint8_t *sec_sha_context_get(uint16_t *len);

/**
 * @brief  Initialise the sha calculate.
 * @return Ref sec_sha_ret_type_t.
 */
sec_sha_ret_type_t sec_sha_init(void);

/**
 * @brief DeInitialise the sha calculate.
 */
void sec_sha_deinit(void);

/**
 * @brief  Update the sha256 calculate.
 * @param  vdata  Valiad data to calulate sha value.
 * @param  len    The length of vdata.
 * @return Ref sec_sha_ret_type_t.
 */
sec_sha_ret_type_t sec_sha256_update(const void *vdata, uint32_t len);

/**
 * @brief  End sha256 calculate, and sec_sha_context->hash is the result
 * @param  result  Return calculate result.
 * @param  length  The length of result.
 * @return Ref sec_sha_ret_type_t.
 */
sec_sha_ret_type_t sec_sha256_final(uint8_t *result, uint8_t length);

#if SHA512_EXIST == YES
/**
 * @brief  Update the sha512 calculate.
 * @param  vdata  Valiad data to calulate sha value.
 * @param  len    The length of vdata.
 * @return Ref sec_sha_ret_type_t.
 */
sec_sha_ret_type_t sec_sha512_update(const void *vdata, uint32_t len);

/**
 * @brief  End sha512 calculate, and sec_sha_context->hash is the result
 * @param  result  Return calculate result.
 * @param  length  The length of result.
 * @return Ref sec_sha_ret_type_t.
 */
sec_sha_ret_type_t sec_sha512_final(uint8_t *result, uint8_t length);
#endif

/**
 * @brief  Get sha result.
 * @return True if sha check finish.
 */
bool sec_sha_get_result(void);

/**
 * @brief  Register callback of sha and enable interrupt.
 * @param  callback The function to register.
 * @return True register success, false failed.
 */
bool sec_sha_register_callback(SEC_CALLBACK callback);

/**
 * @brief  Unregister callback of sha, disable and clear interrupt.
 */
void sec_sha_unregister_callback(void);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
