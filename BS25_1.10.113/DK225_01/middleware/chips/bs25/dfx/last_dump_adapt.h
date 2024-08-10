/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: last dump
 * This file should be changed only infrequently and with great care.
 */

#ifndef LAST_DUMP_ADAPT_H
#define LAST_DUMP_ADAPT_H

#include <stdint.h>

void dfx_last_dump(void);
void dfx_last_dump2flash(uint32_t flash_dump_addr, uint32_t flash_dump_size);
#endif