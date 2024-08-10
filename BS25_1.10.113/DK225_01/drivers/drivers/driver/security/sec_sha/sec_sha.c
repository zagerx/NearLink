/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: SEC SHA
 * Author: @CompanyNameTag
 * Create: 2020-01-20
 */

#include "sec_sha.h"
#include "sec_common.h"
#include "chip_io.h"
#include "securec.h"
#include "oal_interface.h"
#include "tcxo.h"
#include "debug_print.h"
#include "soc_osal.h"

#define SHA256_BUFFER_WORDS_LEN   16
#define SHA512_BUFFER_WORDS_LEN   16
#define SHA256_BUFFER_BYTES_LEN   64
#define SHA512_BUFFER_BYTES_LEN   128
#define SHA256_HASH_WORDS_LEN     8
#define SHA512_HASH_WORDS_LEN     16

#define SHA_4BYTE             4U
#define SHA_SHIFT_2           2
#define SHA_SHIFT_3           3U
#define SHA_SHIFT_6           6
#define SHA_SHIFT_7           7
#define SHA_SHIFT_8           8
#define SHA_TIMEOUT_DELAY     1000UL

typedef struct {
#if SHA512_EXIST == YES
    union {
        uint64_t words[SHA512_BUFFER_WORDS_LEN];
        uint8_t bytes[SHA512_BUFFER_BYTES_LEN];
    } buffer;
    uint32_t hash[SHA512_HASH_WORDS_LEN];
#else
    union {
        uint32_t words[SHA256_BUFFER_WORDS_LEN];
        uint8_t bytes[SHA256_BUFFER_BYTES_LEN];
    } buffer;
    uint32_t hash[SHA256_HASH_WORDS_LEN];
#endif
    uint32_t total_length;
    uint32_t buffer_length;
} sec_sha_context_t  __attribute__((aligned(8)));

static sec_sha_context_t g_sec_sha_context;

static const uint8_t g_sec_sha256_null_value[] = {
    0xE3, 0xB0, 0xC4, 0x42, 0x98, 0xFC, 0x1C, 0x14, \
    0x9A, 0xFB, 0xF4, 0xC8, 0x99, 0x6F, 0xB9, 0x24, \
    0x27, 0xAE, 0x41, 0xE4, 0x64, 0x9B, 0x93, 0x4C, \
    0xA4, 0x95, 0x99, 0x1B, 0x78, 0x52, 0xB8, 0x55  \
};

#if SHA512_EXIST == YES
static const uint8_t g_sec_sha512_null_value[] = {
    0xCF, 0x83, 0xE1, 0x35, 0x7E, 0xEF, 0xB8, 0xBD, \
    0xF1, 0x54, 0x28, 0x50, 0xD6, 0x6D, 0x80, 0x07, \
    0xD6, 0x20, 0xE4, 0x05, 0x0B, 0x57, 0x15, 0xDC, \
    0x83, 0xF4, 0xA9, 0x21, 0xD3, 0x6C, 0xE9, 0xCE, \
    0x47, 0xD0, 0xD1, 0x3C, 0x5D, 0x85, 0xF2, 0xB0, \
    0xFF, 0x83, 0x18, 0xD2, 0x87, 0x7E, 0xEC, 0x2F, \
    0x63, 0xB9, 0x31, 0xBD, 0x47, 0x41, 0x7A, 0x81, \
    0xA5, 0x38, 0x32, 0x7A, 0xF9, 0x27, 0xDA, 0x3E  \
};
#endif

uint8_t *sec_sha_context_get(uint16_t* len)
{
    *len = sizeof(g_sec_sha_context);
    return (uint8_t *)&g_sec_sha_context;
}

sec_sha_ret_type_t sec_sha_init(void)
{
    if (sec_common_driver_initialised_get(SEC_SHA)) {
        return SEC_SHA_RET_INIT_ERROR;
    }

    memset_s((uint8_t *)&g_sec_sha_context, sizeof(g_sec_sha_context), 0, sizeof(g_sec_sha_context));

    uint32_t irq_sts = osal_irq_lock();
    hal_sec_sha_enable();
    hal_sec_comm_set_endian(SEC_SHA, BIG_ENDIAN_32BIT);
    sec_common_driver_initalised_set(SEC_SHA, true);
    osal_irq_restore(irq_sts);
    return SEC_SHA_RET_OK;
}

void sec_sha_deinit(void)
{
    uint32_t irq_sts = osal_irq_lock();
    hal_sec_sha_disable();
    sec_sha_unregister_callback();
    sec_common_driver_initalised_set(SEC_SHA, false);
    osal_irq_restore(irq_sts);
}

static bool sec_sha_cal_with_timeout(void)
{
    uint64_t sha_start_time = uapi_tcxo_get_ms();
    while (!sec_sha_get_result()) {
        if ((uapi_tcxo_get_ms() - sha_start_time) >= SHA_TIMEOUT_DELAY) {
            g_sec_sha_context.buffer_length = 0;
            PRINT("sec_sha_cal_with_timeout!!!" NEWLINE);
            return false;
        }
    }
    return true;
}

static uint32_t sha_hash_words_len(sec_sha_compute_mode_t compute_mode)
{
    if (compute_mode == SEC_SHA512) {
        return SHA512_HASH_WORDS_LEN;
    } else {
        return SHA256_HASH_WORDS_LEN;
    }
}

static uint32_t sha_buffer_bytes_len(sec_sha_compute_mode_t compute_mode)
{
    if (compute_mode == SEC_SHA512) {
        return SHA512_BUFFER_BYTES_LEN;
    } else {
        return SHA256_BUFFER_BYTES_LEN;
    }
}

static void sec_sha_start(uint32_t data_in_addr, uint32_t data_len,
                          bool is_last_package, sec_sha_compute_mode_t compute_mode)
{
    if (g_sec_sha_context.hash[0] != 0) {
        hal_sec_sha_cfg_ivin(SEC_SHA_IVIN_USER_CFG, (uint32_t)g_sec_sha_context.hash);
    } else {
        hal_sec_sha_cfg_ivin(SEC_SHA_IVIN_DEFAULT_CFG, 0);
    }

    if (compute_mode == SEC_SHA512) {
        hal_sec_sha_compute_mode(SEC_SHA512);
    } else if (compute_mode == SEC_SHA256) {
        hal_sec_sha_compute_mode(SEC_SHA256);
    }

    if ((data_len % sha_buffer_bytes_len(compute_mode)) != 0 || is_last_package) {
        hal_sec_sha_cfg_padding(SEC_SHA_AUTO_PADDING);
    } else {
        hal_sec_sha_cfg_padding(SEC_SHA_NO_PADDING);
    }

    hal_sec_sha_cfg_data(data_in_addr, (uint32_t)g_sec_sha_context.hash,
                         data_len, g_sec_sha_context.total_length);
    hal_sec_sha_start();
}

/**
 * @brief  if data_addr is not 8byte aligned, copy data to g_sec_sha_context and calculate, this function need
 *         data is not the last package, used in sha continue calculate case
 * @param  data_addr the data addr
 * @param  len len is 64byte aligned
 * @return sec_sha_ret_type_t
 */
static sec_sha_ret_type_t sec_sha_data_copy_and_cal(uint32_t data_addr, uint32_t len,
                                                    sec_sha_compute_mode_t compute_mode)
{
    if ((data_addr & 0x7) == 0) {
        g_sec_sha_context.total_length += len;
        sec_sha_start(data_addr, len, false, compute_mode);
        if (!sec_sha_cal_with_timeout()) {
            return SEC_SHA_RET_TIMEOUT;
        }
        return SEC_SHA_RET_OK;
    }

    // data_addr is not 8bit aligned, we need copy to g_sec_sha_context.buffer, then calculate
    uint32_t cal_times;
    if (compute_mode == SEC_SHA256) {
        cal_times = (len >> SHA_SHIFT_6);
    } else {
        cal_times = (len >> SHA_SHIFT_7);
    }

    errno_t ret;
    for (uint8_t i = 0; i < cal_times; i++) {
        ret = memcpy_s(&g_sec_sha_context.buffer.bytes[0], sha_buffer_bytes_len(compute_mode),
                       (uint8_t *)(data_addr + (i * sha_buffer_bytes_len(compute_mode))),
                       sha_buffer_bytes_len(compute_mode));
        if (ret != EOK) {
            return SEC_SHA_RET_MEMCOPY_FAIL;
        }

        g_sec_sha_context.total_length += sha_buffer_bytes_len(compute_mode);
        sec_sha_start((uintptr_t)g_sec_sha_context.buffer.words,
                      sha_buffer_bytes_len(compute_mode), false, compute_mode);
        if (!sec_sha_cal_with_timeout()) { return SEC_SHA_RET_TIMEOUT; }
    }
    return SEC_SHA_RET_OK;
}

static uint32_t sec_sha_fix(uint32_t len, sec_sha_compute_mode_t compute_mode)
{
    if (compute_mode == SEC_SHA512) {
        return (((len) >> SHA_SHIFT_7) << SHA_SHIFT_7);
    } else {
        return (((len) >> SHA_SHIFT_6) << SHA_SHIFT_6);
    }
}

static uint32_t sec_sha_mod(uint32_t len, sec_sha_compute_mode_t compute_mode)
{
    if (compute_mode == SEC_SHA512) {
        return ((len) - (((len) >> SHA_SHIFT_7) << SHA_SHIFT_7));
    } else {
        return ((len) - (((len) >> SHA_SHIFT_6) << SHA_SHIFT_6));
    }
}

static sec_sha_ret_type_t sec_sha_update(const void *vdata, uint32_t len, sec_sha_compute_mode_t compute_mode)
{
    if (!sec_common_driver_initialised_get(SEC_SHA)) { return SEC_SHA_RET_INIT_ERROR; }

    const uint8_t *data = vdata;
    uint32_t irq_sts = osal_irq_lock();
    uint32_t buffer_bytes_len = sha_buffer_bytes_len(compute_mode);
    if (g_sec_sha_context.buffer_length != 0) {
        uint32_t bytes_copy = MIN((buffer_bytes_len - g_sec_sha_context.buffer_length), len);
        if (bytes_copy > 0) {
            if (memcpy_s(&g_sec_sha_context.buffer.bytes[g_sec_sha_context.buffer_length],
                         (buffer_bytes_len - g_sec_sha_context.buffer_length), data, bytes_copy) != EOK) {
                osal_irq_restore(irq_sts);
                return SEC_SHA_RET_MEMCOPY_FAIL;
            }

            g_sec_sha_context.total_length += bytes_copy;
            g_sec_sha_context.buffer_length += bytes_copy;
            data += bytes_copy;
            len -= bytes_copy;
        }

        if ((g_sec_sha_context.buffer_length == buffer_bytes_len) && (len != 0)) {
            sec_sha_start((uintptr_t)g_sec_sha_context.buffer.words, buffer_bytes_len, false, compute_mode);
            if (!sec_sha_cal_with_timeout()) {
                osal_irq_restore(irq_sts);
                return SEC_SHA_RET_TIMEOUT;
            }
            g_sec_sha_context.buffer_length = 0;
        }
    }

    if (len > buffer_bytes_len) {
        uint32_t cal_length = (sec_sha_mod(len, compute_mode) == 0) ? (len - buffer_bytes_len) : \
                               sec_sha_fix(len, compute_mode);
        if (sec_sha_data_copy_and_cal((uint32_t)data, cal_length, compute_mode) != SEC_SHA_RET_OK) {
            osal_irq_restore(irq_sts);
            return SEC_SHA_RET_TIMEOUT;
        }
        data += cal_length;
        len -= cal_length;
    }

    if (len > 0) {
        if (memcpy_s(&g_sec_sha_context.buffer.bytes[g_sec_sha_context.buffer_length],
                     (buffer_bytes_len - g_sec_sha_context.buffer_length), data, len) != EOK) {
            osal_irq_restore(irq_sts);
            return SEC_SHA_RET_MEMCOPY_FAIL;
        }
        g_sec_sha_context.total_length += len;
        g_sec_sha_context.buffer_length += len;
    }
    osal_irq_restore(irq_sts);

    return SEC_SHA_RET_OK;
}

#if SHA_LAST_PACKAGE_IS_BIG_ENDIAN == NO
static uint32_t little2big(uint32_t number, uint8_t remaind)
{
    return number << ((uint8_t)(SHA_4BYTE - remaind) << SHA_SHIFT_3);
}
#endif

static void sec_sha_4byte_align(void)
{
#if SHA_LAST_PACKAGE_IS_BIG_ENDIAN == NO
    if (hal_sec_comm_get_endian() == BIG_ENDIAN_32BIT) {
        uint8_t remaind = g_sec_sha_context.buffer_length % SHA_4BYTE;
        if (remaind != 0) {
            uint32_t temp = g_sec_sha_context.buffer.words[g_sec_sha_context.buffer_length >> SHA_SHIFT_2];
            g_sec_sha_context.buffer.words[g_sec_sha_context.buffer_length >> SHA_SHIFT_2] = little2big(temp, remaind);
        }
    }
#endif
}

static void sec_sha_get_null_input_result(sec_sha_compute_mode_t compute_mode)
{
    if (compute_mode == SEC_SHA256) {
        if (memcpy_s((void *)(uintptr_t)g_sec_sha_context.hash, SHA256_HASH_WORDS_LEN * sizeof(uint32_t),
            g_sec_sha256_null_value, sizeof(g_sec_sha256_null_value)) != EOK) {
            PRINT("sec sha memcpy failed!!!" NEWLINE);
        }
    }
#if SHA512_EXIST == YES
    if (compute_mode == SEC_SHA512) {
        if (memcpy_s((void *)(uintptr_t)g_sec_sha_context.hash, SHA512_HASH_WORDS_LEN * sizeof(uint32_t),
            g_sec_sha512_null_value, sizeof(g_sec_sha512_null_value)) != EOK) {
            PRINT("sec sha memcpy failed!!!" NEWLINE);
        }
    }
#endif
}

static sec_sha_ret_type_t sec_sha_final(uint8_t *result, uint8_t length, sec_sha_compute_mode_t compute_mode)
{
    sec_sha_ret_type_t sec_ret = SEC_SHA_RET_OK;
    if (!sec_common_driver_initialised_get(SEC_SHA)) {
        return SEC_SHA_RET_INIT_ERROR;
    }

    uint32_t irq_sts = osal_irq_lock();
    sec_sha_4byte_align();
    if (g_sec_sha_context.buffer_length != 0) {
        sec_sha_start((uintptr_t)g_sec_sha_context.buffer.words, g_sec_sha_context.buffer_length, true, compute_mode);
        if (!sec_sha_cal_with_timeout()) {
            sec_ret = SEC_SHA_RET_TIMEOUT;
        }
    }

    if (g_sec_sha_context.total_length == 0) {
        sec_sha_get_null_input_result(compute_mode);
    }

    errno_t ret = memcpy_s(result, length, g_sec_sha_context.hash, sha_hash_words_len(compute_mode) * sizeof(uint32_t));
    if (ret != EOK) {
        sec_ret = SEC_SHA_RET_MEMCOPY_FAIL;
    }
    hal_sec_sha_disable();
    sec_sha_unregister_callback();
    sec_common_driver_initalised_set(SEC_SHA, false);
    osal_irq_restore(irq_sts);
    return sec_ret;
}

sec_sha_ret_type_t sec_sha256_update(const void *vdata, uint32_t len)
{
    return sec_sha_update(vdata, len, SEC_SHA256);
}

sec_sha_ret_type_t sec_sha256_final(uint8_t *result, uint8_t length)
{
    return sec_sha_final(result, length, SEC_SHA256);
}

#if SHA512_EXIST == YES
sec_sha_ret_type_t sec_sha512_update(const void *vdata, uint32_t len)
{
    if (vdata == NULL) {
        return 0;
    }
    return sec_sha_update(vdata, len, SEC_SHA512);
}

sec_sha_ret_type_t sec_sha512_final(uint8_t *result, uint8_t length)
{
    return sec_sha_final(result, length, SEC_SHA512);
}
#endif

bool sec_sha_get_result(void)
{
    return hal_sec_sha_is_finish();
}

bool sec_sha_register_callback(SEC_CALLBACK callback)
{
    uint32_t irq_sts = osal_irq_lock();
    hal_sec_comm_intr_clear(SEC_SHA);
    if (hal_sec_comm_register_callback(callback, SEC_SHA) == false) {
        osal_irq_restore(irq_sts);
        return false;
    }
    sec_comm_enable_irq(SEC_SHA);
    hal_sec_comm_enable_intr(SEC_SHA);
    osal_irq_restore(irq_sts);
    return true;
}

void sec_sha_unregister_callback(void)
{
    uint32_t irq_sts = osal_irq_lock();
    sec_comm_disable_irq(SEC_SHA);
    hal_sec_comm_disable_intr(SEC_SHA);
    hal_sec_comm_intr_clear(SEC_SHA);
    hal_sec_comm_unregister_callback(SEC_SHA);
    osal_irq_restore(irq_sts);
}
