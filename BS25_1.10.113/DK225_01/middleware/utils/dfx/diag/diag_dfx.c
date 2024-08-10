/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description: zdiag dfx
 * This file should be changed only infrequently and with great care.
 */

#include "diag_dfx.h"

static diag_dfx_stat_t g_zdiag_dfx_stat;

diag_dfx_stat_t *uapi_diag_get_dfx_stat(void)
{
    return &g_zdiag_dfx_stat;
}
