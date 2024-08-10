/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  CLOCKS CORE COMMON SOURCE.
 * Author: @CompanyNameTag
 * Create: 2021-12-25
 */

#include "hal_clocks.h"
#include "non_os.h"
#if (CORE == MASTER_BY_ALL) && !defined(BUILD_APPLICATION_ROM)
#include "clocks_switch.h"
#endif
#include "clocks_core_common.h"

uint32_t uart_get_clock_value(uart_bus_t uart)
{
#if defined(BUILD_APPLICATION_ATE)
    UNUSED(uart);
    return TCXO_1X_CLK;
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_CLOCKS_CORE) != true) {
        return TCXO_1X_CLK;
    }
    switch (uart) {
        case UART_BUS_0:
            return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_MCU_PERP_LS);
        case UART_BUS_1:
        case UART_BUS_2:
            return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_MCU_PERP_UART);
        default:
            return 0; //lint !e527
    }
#endif
}

uint32_t get_mcu_core_clk(void)
{
    return TCXO_1X_CLK;
}