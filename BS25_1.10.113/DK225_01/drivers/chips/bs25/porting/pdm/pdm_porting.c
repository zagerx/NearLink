/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides PDM port for bs25 \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-31, Create file. \n
 */
#include "hal_pdm.h"
#include "hal_pdm_v151.h"
#include "pm_lpc.h"
#include "pdm_porting.h"

#define PDM_BASE_ADDR 0x5208E000

uintptr_t pdm_porting_base_addr_get(void)
{
    return (uintptr_t)PDM_BASE_ADDR;
}

void pdm_port_register_hal_funcs(void)
{
    hal_pdm_register_funcs(hal_pdm_v151_funcs_get());
}

void pdm_port_unregister_hal_funcs(void)
{
    hal_pdm_unregister_funcs();
}

void pdm_port_clock_enable(bool on)
{
    if (on) {
        pm_lpc_ccrg_clk_pdm_enable(LPC_CCRG_CLK_PDM_USED_PDM, true);
    } else {
        pm_lpc_ccrg_clk_pdm_enable(LPC_CCRG_CLK_PDM_USED_PDM, false);
    }
}