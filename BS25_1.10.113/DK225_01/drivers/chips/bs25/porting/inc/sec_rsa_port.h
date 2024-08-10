/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  Hi11xx SEC MOUDLE CONFIG
 * Author: @CompanyNameTag
 * Create: 2022-04-18
 */
#ifndef SEC_RSA_PORT_H
#define SEC_RSA_PORT_H

#include <stdint.h>
#include <stdbool.h>
#include "product.h"
#include "platform_core.h"

/**
 * @brief  Set rsa config.
 * @param  text_addr The address of plain text.
 * @param  key_addr  The address of rsa key.
 * @param  result_addr The address of rsa result.
 */
void sec_rsav2_cfg(uint32_t text_addr, uint32_t key_addr, uint32_t result_addr);

/**
 * @brief  Rsa has finished or not.
 */
bool sec_rsav2_final(void);

#endif
