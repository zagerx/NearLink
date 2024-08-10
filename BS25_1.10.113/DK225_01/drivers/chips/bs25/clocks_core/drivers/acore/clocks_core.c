/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  CLOCKS CORE PRIVATE SOURCE.
 * Author: @CompanyNameTag
 * Create: 2021-06-16
 */

#include <stdint.h>
#include "osal_interrupt.h"
#include "non_os.h"
#include "clocks_switch.h"
#ifdef USE_CMSIS_OS
#include "pmu_cmu.h"
#endif
#include "hal_cpu_clocks.h"
#include "hal_clocks_application_private.h"
#include "product.h"
#include "clocks_config.h"
#include "clocks_core.h"

#if defined(BUILD_APPLICATION_SSB)
void clocks_ssb_clk_init(void)
{
}
#else
bool clocks_app_clk_init(void)
{
    uint32_t irq_sts = osal_irq_lock();
    clocks_clk_cfg_t const *system_clocks = clocks_system_all_clocks_get();
    for (uint8_t module = CLOCKS_CCRG_MODULE_MCU_CORE; module < CLOCKS_CCRG_MODULE_MAX; module++) {
        system_ccrg_clock_config(module, system_clocks[module].clk_src, system_clocks[module].clk_div);
    }
    osal_irq_restore(irq_sts);

    return true;
}
#endif

uint32_t clocks_get_spi_clock_value(void)
{
#if CHIP_FPGA || defined(BUILD_APPLICATION_ROM)
    return TCXO_1X_CLK;
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_CLOCKS_CORE) != true) {
        return TCXO_1X_CLK;
    }
    return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_MCU_PERP_SPI);
#endif
}

uint32_t clocks_get_xip_clock_value(void)
{
#if CHIP_FPGA || defined(BUILD_APPLICATION_ROM)
    return TCXO_1X_CLK;
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_CLOCKS_CORE) != true) {
        return TCXO_1X_CLK;
    }

    return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_XIP_QSPI);
#endif
}