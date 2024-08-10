/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides xip porting template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-13， Create file. \n
 */
#include "hal_xip.h"
#include "hal_xip_v150_regs_op.h"
#include "xip_porting.h"
#include "platform_core.h"
#include "debug_print.h"
#include "common_def.h"

uintptr_t g_xip_base_addr = (uintptr_t)XIP_CACHE_CTL_RB_BASE;

uintptr_t xip_porting_base_addr_get(void)
{
    return g_xip_base_addr;
}

void xip_error_interrupt_enable(void)
{
    PRINT("xip_error_interrupt_enable");
#if CORE == MASTER_BY_ALL
#if CHIP_FPGA

#else
    non_os_nmi_config(NMI_XIP_CTRL, true);
    non_os_nmi_config(NMI_XIP_CACHE, true);
#endif
#endif
}

void hal_xip_auto_cg(bool en)
{
    if (en) {
        uapi_reg_setbit(0x55000908, 1); // close xip cache auto cg bypass
    } else {
        uapi_reg_clrbit(0x55000908, 1); // open xip cache auto cg bypass
    }
}