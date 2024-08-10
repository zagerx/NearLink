/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:HAL CPU CLOCK DRIVER.
 * Author: @CompanyNameTag
 * Create: 2020-01-13
 */

#include "hal_cpu_clocks.h"
#include "non_os.h"
#include "chip_io.h"
#include "clocks.h"
#include "tcxo.h"

/************************ C_CRG config **************************/
#define HAL_AON_SUB_32K_DELAY   65ULL
#define CLOCK_DIV_1 1

void hal_aon_sub_rc_tcxo_clock_switch(hal_clocks_aon_clk_t clksel, uint8_t aon_bus_div,
                                      uint8_t aon_com_div, uint8_t acs_div, uint8_t hf_div)
{
    if (clksel == HAL_CLOCK_AON_TCXO) {
        // Select TCXO clock
        hal_clocks_xo_rc_clk_sel(HAL_CLOCK_AON_TCXO);
        // Set aon bus div
        hal_clocks_set_aon_bus_div(aon_bus_div);
        // Config aon 32M common select TCOX clock
        hal_clocks_a32m_common_clken(TURN_OFF);
        hal_clocks_a32m_common_sel(HAL_CLOCK_AON_TCXO);
        hal_clocks_a32m_common_clken(TURN_ON);
        // Set aon 32M common clock div
        hal_clocks_set_a32m_common_clk_div(aon_com_div);
        // Set cap sensor clock div
        hal_clocks_glb_config_cap_sensor_clkdiv(acs_div);
        // Set hf 32k clock div
        hal_clocks_set_hf_32k_div(hf_div);
    } else {
        // Set aon bus div
        hal_clocks_set_aon_bus_div(aon_bus_div);
        // Select RC clock
        hal_clocks_xo_rc_clk_sel(HAL_CLOCK_AON_RC);
        // Set aon 32M common clock div
        hal_clocks_a32m_common_clken(TURN_ON);
        hal_clocks_set_a32m_common_clk_div(aon_com_div);
        // Set cap sensor clock div
        hal_clocks_glb_config_cap_sensor_clkdiv(acs_div);
        // Set hf 32k clock div
        hal_clocks_set_hf_32k_div(hf_div);
        // Config aon 32M common select RC clock
        hal_clocks_a32m_common_clken(TURN_OFF);
        hal_clocks_a32m_common_sel(HAL_CLOCK_AON_RC);
        hal_clocks_a32m_common_clken(TURN_ON);
    }
}

void hal_aon_sub_32k_clock_switch(switch_type_t clksel)
{
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_GLB_32K, TURN_OFF);
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_CAP_SENSOR, TURN_OFF);
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_EFUSE, TURN_OFF);
    uapi_tcxo_delay_us(HAL_AON_SUB_32K_DELAY);
    hal_clocks_xo_32k_clk_sel(clksel);
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_GLB_32K, TURN_ON);
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_CAP_SENSOR, TURN_ON);
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_EFUSE, TURN_ON);
}