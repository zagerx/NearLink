/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: SEC RSA
 * Author: @CompanyNameTag
 * Create: 2020-01-20
 */

#include "sec_rsav2.h"
#include "sec_common.h"
#include "oal_interface.h"
#include "debug_print.h"
#include "tcxo.h"
#include "chip_io.h"

#define RSA_TIMEOUT_MS 2000ULL

static sec_rsa_context_t g_sec_rsa_context;

sec_rsa_context_t *sec_rsa_get_context(void)
{
    return &g_sec_rsa_context;
}

void sec_rsav2_init(void)
{
    if (sec_common_driver_initialised_get(SEC_RSAV2)) {
        return;
    }
    hal_sec_rsav2_enable();
    hal_sec_comm_set_endian(SEC_RSAV2, BIG_ENDIAN_32BIT);
    sec_common_driver_initalised_set(SEC_RSAV2, true);
}

void sec_rsav2_deinit(void)
{
    if (!sec_common_driver_initialised_get(SEC_RSAV2)) {
        return;
    }
    sec_rsav2_unregister_callback();
    hal_sec_rsav2_disable();
    sec_common_driver_initalised_set(SEC_RSAV2, false);
}

void sec_rsav2_start(rsav2_alg_cfg_t rsav2_alg_cfg)
{
    hal_sec_rsav2_cfg(rsav2_alg_cfg);
    hal_sec_rsav2_start();
}

bool sec_rsav2_get_result(void)
{
    return hal_sec_rsav2_is_finish();
}

#if SEC_NEW_RSA == NO
bool sec_rsav2_register_callback(SEC_CALLBACK callback)
{
    uint32_t irq_sts = osal_irq_lock();
    hal_sec_comm_intr_clear(SEC_RSAV2);
    if (hal_sec_comm_register_callback(callback, SEC_RSAV2) == false) {
        osal_irq_restore(irq_sts);
        return false;
    }
    sec_comm_enable_irq(SEC_RSAV2);
    hal_sec_comm_enable_intr(SEC_RSAV2);
    osal_irq_restore(irq_sts);
    return true;
}
#endif

void sec_rsav2_unregister_callback(void)
{
#if SEC_NEW_RSA == NO
    uint32_t irq_sts = osal_irq_lock();
    sec_comm_disable_irq(SEC_RSAV2);
    hal_sec_comm_disable_intr(SEC_RSAV2);
    hal_sec_comm_intr_clear(SEC_RSAV2);
    hal_sec_comm_unregister_callback(SEC_RSAV2);
    osal_irq_restore(irq_sts);
#endif
}

bool sec_rsav2_calculate(uint32_t text_addr, uint32_t key_addr, uint32_t result_addr)
{
    bool ret = true;
    sec_rsav2_init();
    sec_rsav2_cfg(text_addr, key_addr, result_addr);
    sec_rsav2_start(g_sec_rsa_context.rsa_alg_cfg);

    uint64_t start_time = uapi_tcxo_get_ms();
    while (!hal_sec_rsav2_is_finish()) {
        if ((uapi_tcxo_get_ms() - start_time) >= RSA_TIMEOUT_MS) {
            PRINT("RSA calculate timeout!!! %d"NEWLINE);
            ret = false;
            break;
        }
    }

    if (!sec_rsav2_final()) {
        PRINT("RSA calculate error.\r\n");
    }
    PRINT("RSA calculate finish.\r\n");
    sec_rsav2_deinit();
    return ret;
}
