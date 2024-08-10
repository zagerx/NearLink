/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides low power control source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-5-25， Create file. \n
 */

#include "chip_io.h"
#include "tcxo.h"
#include "pm_clock.h"
#include "pm_pmu.h"
#include "pm_lpc.h"

static uint16_t g_lpc_ccrg_clk_spi_used = 0;
static uint16_t g_lpc_mclken_sec_used = 0;
static uint16_t g_lpc_ccrg_clk_pdm_used = 0;
static uint16_t g_lpc_ulp_clkldo_used = 0;

#define lpc_setbit(src, pos)   ((src) |= ((uint16_t)((1U) << (uint8_t)(pos))))
#define lpc_clrbit(src, pos)   ((src) &= ~((uint16_t)((1U) << (uint8_t)(pos))))

void pm_lpc_ccrg_clk_spi_enable(lpc_ccrg_clk_spi_used_t dev, bool en)
{
    if (en) {
        if (g_lpc_ccrg_clk_spi_used == 0) {
            uapi_clock_crg_config(CLOCK_CRG_ID_MCU_PERP_SPI, CLOCK_CLK_SRC_TCXO, 0x2); // 2分频：16M
        }
        lpc_setbit(g_lpc_ccrg_clk_spi_used, dev);
    } else {
        lpc_clrbit(g_lpc_ccrg_clk_spi_used, dev);
        if (g_lpc_ccrg_clk_spi_used == 0) {
            uapi_clock_crg_config(CLOCK_CRG_ID_MCU_PERP_SPI, CLOCK_CLK_SRC_MAX, 0x2);
        }
    }
}

void pm_lpc_mclken_sec_enable(lpc_sec_clken_used_t dev, bool en)
{
    if (en) {
        if (g_lpc_mclken_sec_used == 0) {
            uapi_clock_control(CLOCK_CONTROL_MCLKEN_ENABLE, CLOCK_APERP_SEC_CLKEN);
        }
        lpc_setbit(g_lpc_mclken_sec_used, dev);
    } else {
        lpc_clrbit(g_lpc_mclken_sec_used, dev);
        if (g_lpc_mclken_sec_used == 0) {
            uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SEC_CLKEN);
        }
    }
}

void pm_lpc_ccrg_clk_pdm_enable(lpc_ccrg_clk_pdm_used_t dev, bool en)
{
    if (en) {
        if (g_lpc_ccrg_clk_pdm_used == 0) {
            uapi_clock_crg_config(CLOCK_CRG_ID_PDM, CLOCK_CLK_SRC_ULPFLL_MCU_CORE, 0x7); // 7分频：12.288M
        }
        lpc_setbit(g_lpc_ccrg_clk_pdm_used, dev);
    } else {
        lpc_clrbit(g_lpc_ccrg_clk_pdm_used, dev);
        if (g_lpc_ccrg_clk_pdm_used == 0) {
            uapi_clock_crg_config(CLOCK_CRG_ID_PDM, CLOCK_CLK_SRC_MAX, 0x0);
        }
    }
}

void pm_lpc_ulp_clkldo_enable(lpc_ulp_clkldo_used_t dev, bool en)
{
    if (en) {
        if (g_lpc_ulp_clkldo_used == 0) {
#ifndef USE_EMBED_FLASH
            writew(0x5702C224, 0x11);   // phyldo en
            writew(0x5702C24C, 0x1000); // ulp_clkldo en
            uapi_tcxo_delay_us(120);    // delay 120us
            writew(0x5702C224, 0x13);   // phyldo en_delay
            writew(0x5702C24C, 0x3000); // ulp_clkldo en_delay
#endif
        }
        lpc_setbit(g_lpc_ulp_clkldo_used, dev);
    } else {
        lpc_clrbit(g_lpc_ulp_clkldo_used, dev);
        if (g_lpc_ulp_clkldo_used == 0) {
#ifndef USE_EMBED_FLASH
            writew(0x5702C24C, 0x0);    // 关闭ulp_clkldo
            writew(0x5702C224, 0x10);   // 关闭phyldo
#endif
        }
    }
}