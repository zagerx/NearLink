/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  Hi11xx SEC MOUDLE CONFIG
 * Author: @CompanyNameTag
 * Create: 2022-04-18
 */
#include "sec_rsa_port.h"
#include "pal_sec_config_port.h"
#include "sec_rsav2.h"
#include "debug_print.h"

void sec_rsav2_cfg(uint32_t text_addr, uint32_t key_addr, uint32_t result_addr)
{
    sec_rsa_context_t *rsa_context = sec_rsa_get_context();
#if SEC_NEW_RSA == YES
    rsa_context->rsa_alg_cfg.mode.sec_rsa_mode = RSA_DECRYPT;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_key_width = KEY_LENGTH_4096;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data0_clr = SEC_RSA_RAM_CLEAR_DIS;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data1_clr = SEC_RSA_RAM_CLEAR_DIS;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data2_clr = SEC_RSA_RAM_CLEAR_DIS;
    rsa_context->rsa_alg_cfg.wpkt = text_addr;
    rsa_context->rsa_alg_cfg.wsec = key_addr;
    rsa_context->rsa_alg_cfg.rrslt = result_addr;
#else
    hal_sec_rsav2_cfg_ahb_timout(0);
    rsa_context->rsa_alg_cfg.rsa_otsk.rsav2_otsk_para_cfg.otask_len = OTASK_LENGTH_4096;
    rsa_context->rsa_alg_cfg.rsa_otsk.rsav2_otsk_para_cfg.plaintext_len_sel = DEFAULT_LENGTH;
    rsa_context->rsa_alg_cfg.otask_ptext_addr = text_addr;
    rsa_context->rsa_alg_cfg.otask_key_addr = key_addr;
    rsa_context->rsa_alg_cfg.otask_result_addr = result_addr;
#endif
}

bool sec_rsav2_final(void)
{
#if SEC_NEW_RSA == YES
    sec_rsa_context_t *rsa_context = sec_rsa_get_context();
    hal_sec_rsav2_copy_result(rsa_context->rsa_alg_cfg);
    rsa_context->rsa_alg_cfg.mode.sec_rsa_mode = RSA_RAM_CLR;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_key_width = KEY_LENGTH_4096;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data0_clr = SEC_RSA_RAM_CLEAR_EN;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data1_clr = SEC_RSA_RAM_CLEAR_EN;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data2_clr = SEC_RSA_RAM_CLEAR_EN;
    if (!hal_sec_rsav2_clear_ram(rsa_context->rsa_alg_cfg)) {
        PRINT("RSA ram clear error!!!"NEWLINE);
        return false;
    }

    rsa_context->rsa_alg_cfg.mode.sec_rsa_mode = RSA_CRC16_DEBUG;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_key_width = KEY_LENGTH_4096;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data0_clr = SEC_RSA_RAM_CLEAR_DIS;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data1_clr = SEC_RSA_RAM_CLEAR_DIS;
    rsa_context->rsa_alg_cfg.mode.sec_rsa_data2_clr = SEC_RSA_RAM_CLEAR_DIS;
    if (!hal_sec_rsav2_clear_key(rsa_context->rsa_alg_cfg)) {
        PRINT("RSA key clear error!!!"NEWLINE);
        return false;
    }
#else
    rsav2_interrupt_t int_type = hal_sec_rsav2_get_int_type();
    if (int_type != RSA_TRANS_DONE_INT) {
        PRINT("RSA calculate error!!! %d"NEWLINE, int_type);
        return false;
    }
#endif
    return true;
}
