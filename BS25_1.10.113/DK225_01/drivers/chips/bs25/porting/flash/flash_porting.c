/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides flash porting \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-30， Create file. \n
 */
#include <common_def.h>
#include "flash.h"
#include "debug_print.h"
#if defined(BUILD_APPLICATION_SSB)
#include "bootloader_configuration.h"
#endif
#include "chip_io.h"
#include "hal_xip.h"
#include "soc_osal.h"
#include "pm_clock.h"
#include "pm_pmu.h"
#include "pinctrl.h"
#include "flash_porting.h"

static uint32_t g_flash_bus_clk = 40000000;
static uint32_t g_flash_enter_qspi_freq_mhz[FLASH_MAX] = { 4, 4 };
static uint32_t g_flash_exit_xip_freq_mhz[FLASH_MAX] = { 2, 2 };
static uint32_t g_flash_enter_xip_freq_mhz[FLASH_MAX] = { 20, 20 };

static volatile bool g_bt_power_on = true;

static flash_cfg_t g_flash_porting_config[FLASH_MAX] = {
    {
        .isinit = 0,
        .flash_manufacturer = FLASH_MANUFACTURER_MAX,
        .unique_id = 0,
        .bus = SPI_BUS_5,
        .mode = HAL_SPI_FRAME_FORMAT_STANDARD,
        .is_xip = true,
        .attr = {
            .is_slave = false,
            .slave_num = 1,
            .bus_clk = 40000000,
            .freq_mhz = 4,
            .clk_polarity = (uint32_t)SPI_CFG_CLK_CPOL_0,
            .clk_phase = (uint32_t)SPI_CFG_CLK_CPHA_0,
            .frame_format = SPI_CFG_FRAME_FORMAT_MOTOROLA_SPI,
            .spi_frame_format = (uint32_t)HAL_SPI_FRAME_FORMAT_STANDARD,
            .frame_size = (uint32_t)HAL_SPI_FRAME_SIZE_8,
            .sste = SPI_CFG_SSTE_DISABLE,
        },
        .extra_attr = {
            .tx_use_dma = false,
            .rx_use_dma = false,
            .qspi_param = { 0 },
        },
    },
    {
        .isinit = 0,
        .flash_manufacturer = FLASH_MANUFACTURER_MAX,
        .unique_id = 0,
        .bus = SPI_BUS_6,
        .mode = HAL_SPI_FRAME_FORMAT_STANDARD,
        .is_xip = true,
        .attr = {
            .is_slave = false,
            .slave_num = 1,
            .bus_clk = 40000000,
            .freq_mhz = 4,
            .clk_polarity = (uint32_t)SPI_CFG_CLK_CPOL_0,
            .clk_phase = (uint32_t)SPI_CFG_CLK_CPHA_0,
            .frame_format = SPI_CFG_FRAME_FORMAT_MOTOROLA_SPI,
            .spi_frame_format = (uint32_t)HAL_SPI_FRAME_FORMAT_STANDARD,
            .frame_size = (uint32_t)HAL_SPI_FRAME_SIZE_8,
            .sste = SPI_CFG_SSTE_DISABLE,
        },
        .extra_attr = {
            .tx_use_dma = false,
            .rx_use_dma = false,
            .qspi_param = { 0 },
        },
    },
};

void flash_porting_get_config(flash_id_t id, flash_cfg_t *config)
{
    config[id] = g_flash_porting_config[id];
}

void flash_porting_update_enter_qspi_baud(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz)
{
    g_flash_bus_clk = bus_clk;
    g_flash_enter_qspi_freq_mhz[id] = freq_mhz;
}

static void flash_0_enter_qspi_clk_div(flash_id_t id, spi_attr_t *attr)
{
    UNUSED(id);
    UNUSED(attr);
}

static void flash_1_enter_qspi_clk_div(flash_id_t id, spi_attr_t *attr)
{
    attr->bus_clk = g_flash_bus_clk;
    attr->freq_mhz = g_flash_enter_qspi_freq_mhz[id];
}

static set_clk_div_t g_flash_enter_qspi_clk_div[FLASH_MAX] = {
    flash_0_enter_qspi_clk_div,
    flash_1_enter_qspi_clk_div,
};

void flash_porting_set_enter_qspi_clk_div(flash_id_t id, spi_attr_t *attr)
{
    g_flash_enter_qspi_clk_div[id](id, attr);
}

void flash_porting_update_exit_xip_baud(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz)
{
    g_flash_bus_clk = bus_clk;
    g_flash_exit_xip_freq_mhz[id] = freq_mhz;
}

static void flash_0_exit_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    UNUSED(id);
    UNUSED(attr);
}

static void flash_1_exit_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    attr->bus_clk = g_flash_bus_clk;
    attr->freq_mhz = g_flash_exit_xip_freq_mhz[id];
}

static set_clk_div_t g_flash_exit_xip_clk_div[FLASH_MAX] = {
    flash_0_exit_xip_clk_div,
    flash_1_exit_xip_clk_div,
};

void flash_porting_set_exit_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    g_flash_exit_xip_clk_div[id](id, attr);
}

void flash_porting_update_enter_xip_baud(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz)
{
    g_flash_bus_clk = bus_clk;
    g_flash_enter_xip_freq_mhz[id] = freq_mhz;
}

static void flash_0_enter_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    UNUSED(id);
    UNUSED(attr);
}

static void flash_1_enter_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    attr->bus_clk = g_flash_bus_clk;
    attr->freq_mhz = g_flash_enter_xip_freq_mhz[id];
}

static set_clk_div_t g_flash_enter_xip_clk_div[FLASH_MAX] = {
    flash_0_enter_xip_clk_div,
    flash_1_enter_xip_clk_div,
};

void flash_porting_set_enter_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    g_flash_enter_xip_clk_div[id](id, attr);
}

static void flash_0_power_on(flash_id_t id)
{
    unused(id);
}

static void flash_1_power_on(flash_id_t id)
{
    unused(id);
}

static power_on_t g_flash_power_on_array[FLASH_MAX] = {
    flash_0_power_on,
    flash_1_power_on,
};

void flash_porting_power_on(flash_id_t id)
{
    g_flash_power_on_array[id](id);
}

void flash_porting_spi_lock_create(flash_id_t id)
{
    unused(id);
}

void flash_porting_spi_lock_delete(flash_id_t id)
{
    unused(id);
}

uint32_t flash_porting_spi_lock(flash_id_t id)
{
    unused(id);
    return osal_irq_lock();
}

void flash_porting_spi_unlock(flash_id_t id, uint32_t status)
{
    unused(id);
    osal_irq_restore(status);
}

void flash_porting_wait_exit_xip_mode(void)
{
    while (flash_porting_get_bt_power_on_flag() == false) { }
    return;
}

void flash_porting_set_bt_power_on_flag(bool flag)
{
    g_bt_power_on = flag;
}

bool flash_porting_get_bt_power_on_flag(void)
{
    return g_bt_power_on;
}

// 适配
#define KV_PAGE_OFFSET 0
static uint32_t g_flash_size = 0;

void flash_config_save_info(void)
{
#if defined(BUILD_APPLICATION_SSB)
    flash_info_t flash_info;
    uapi_flash_get_info(CHIP_FLASH_ID, &flash_info);
    g_flash_size = flash_info.flash_size;
    PRINT("PRINT Eflash id:%x, size:0x%x\r\n", flash_info.flash_id, flash_info.flash_size);
#endif
}

uint32_t flash_config_get_end(void)
{
#if !defined(BUILD_APPLICATION_ROM)
#if (EMBED_FLASH_EXIST == YES) && defined(BUILD_APPLICATION_SSB)
    if (bootloader_configuration_get_flash_mode() == EMBED_FLASH_XIP) {
        return (EMBED_FLASH_LEN + EMBED_FLASH_START);
    }
#endif
    flash_info_t flash_info;
    if (g_flash_size == 0) {
        uapi_flash_get_info(0, &flash_info);
        g_flash_size = flash_info.flash_size;
    }
    return (g_flash_size + FLASH_START);
#else
    return FLASH_END;
#endif
}

uint16_t flash_config_get_pages(void)
{
#if !defined(BUILD_APPLICATION_ROM)
#if (EMBED_FLASH_EXIST == YES) && defined(BUILD_APPLICATION_SSB)
    if (bootloader_configuration_get_flash_mode() == EMBED_FLASH_XIP) {
        return EMBED_FLASH_LEN / FLASH_PAGE_SIZE;
    }
#endif
    flash_info_t flash_info;
    if (g_flash_size == 0) {
        uapi_flash_get_info(0, &flash_info);
        g_flash_size = flash_info.flash_size;
    }
    return (uint16_t)(g_flash_size / FLASH_PAGE_SIZE);
#else
    return FLASH_PAGES;
#endif
}

uint32_t flash_config_get_start_addr(void)
{
#if (EMBED_FLASH_EXIST == YES) && defined(BUILD_APPLICATION_SSB)
    if (bootloader_configuration_get_flash_mode() == EMBED_FLASH_XIP) {
        return EMBED_FLASH_START;
    }
#endif
    return FLASH_START;
}

uint16_t flash_get_page(uint32_t address)
{
    return (uint16_t)((address - FLASH_START) / FLASH_PAGE_SIZE);
}

uint16_t flash_config_get_kv_pages(void)
{
    return KV_PAGE_OFFSET;
}

extern flash_cfg_t g_flash_config[FLASH_MAX];
errcode_t test_uapi_flash_init(flash_id_t id)
{
    if (g_flash_config[id].isinit == 1) {
        return ERRCODE_SUCC;
    }

    flash_porting_get_config(id, g_flash_config);

    /* spi钩子和基地址 */
    uapi_spi_init(g_flash_config[id].bus, &(g_flash_config[id].attr), &(g_flash_config[id].extra_attr));
    g_flash_config[id].isinit = 1;
    return ERRCODE_SUCC;
}

uint16_t flash_funcreg_adapt(flash_id_t id)
{
    if (g_flash_config[id].isinit) {
        return ERRCODE_SUCC;
    }
    flash_porting_get_config(id, g_flash_config);
    hal_xip_set_cur_mode((xip_id_t)id, XIP_MODE_NORMAL);
    g_flash_config[id].mode = HAL_SPI_FRAME_FORMAT_QUAD;

    spi_funcreg_adapt(g_flash_config[id].bus);

    g_flash_config[id].isinit = 1;
    return ERRCODE_SUCC;
}

void flash_porting_xip_disable(void)
{
    writel(0xa3006100, 0); /* 0xa3006100 disable xip cache en reg for xip init */
}

bool flash_porting_switch_flash_id(flash_id_t *id, uint32_t *base_addr, uint32_t addr)
{
    if (addr >= DATA_FLASH_START && addr < DATA_FLASH_MAX_END) {
        *id = (flash_id_t)CHIP_DATA_FLASH_ID;
        *base_addr = (uint32_t)DATA_FLASH_START;
        return true;
    }
    return false;
}

void flash_port_clock_enable(bool on)
{
    if (on) {
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_ENABLE, CLOCK_APERP_QSPI_1CS_CLKEN);
        uapi_clock_crg_config(CLOCK_CRG_ID_XIP_QSPI, CLOCK_CLK_SRC_TCXO, 0x1);
    } else {
        uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_QSPI_1CS_CLKEN);
        uapi_clock_crg_config(CLOCK_CRG_ID_XIP_QSPI, CLOCK_CLK_SRC_MAX, 0x0);
    }
}

void flash_port_power_on(bool on)
{
    if (on) {
        uapi_pmu_control(PMU_CONTROL_SWITCH1_POWER, PMU_CONTROL_POWER_ON);
    } else {
        uapi_pmu_control(PMU_CONTROL_SWITCH1_POWER, PMU_CONTROL_POWER_OFF);
    }
}

void flash_porting_pinmux_cfg(flash_id_t id)
{
    if (id >= FLASH_MAX) {
        return;
    }

    if (id == FLASH_0) {
        uapi_pin_set_mode(S_AGPIO5, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_AGPIO6, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_AGPIO8, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_AGPIO9, HAL_PIO_FUNC_QSPI0);
#if !defined(CONFIG_FLASH_SUPPORT_SPI_SINGLE_LINE_MODE)
        uapi_pin_set_mode(S_AGPIO12, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_AGPIO13, HAL_PIO_FUNC_QSPI0);
#endif
    } else {
        uapi_pin_set_mode(S_MGPIO0, HAL_PIO_FUNC_QSPI1);
        uapi_pin_set_mode(S_MGPIO1, HAL_PIO_FUNC_QSPI1);
#if !defined(CONFIG_FLASH_SUPPORT_SPI_SINGLE_LINE_MODE)
        uapi_pin_set_mode(S_MGPIO2, HAL_PIO_FUNC_QSPI1);
        uapi_pin_set_mode(S_MGPIO3, HAL_PIO_FUNC_QSPI1);
#endif
        uapi_pin_set_mode(S_MGPIO4, HAL_PIO_FUNC_QSPI1);
        uapi_pin_set_mode(S_MGPIO5, HAL_PIO_FUNC_QSPI1);
    }
}