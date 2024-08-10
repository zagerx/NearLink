/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  bs25 Memory Map configuration.
 * Author: @CompanyNameTag
 * Create: 2020-01-15
 */

#include "hal_memory_security.h"
#include "chip_io.h"
#include "panic.h"
#include "platform_core.h"

#define BT_MEM_CLK_FORCE_ON_REG         (B_CTL_RB_BASE + 0x30)

// BCPU RAM storage 640KB split to 5 region 128KB, to control power.
#define MEMORY_POWER_MODE_CTRL_BIT      10
#define MEMORY_POWER_MODE_CTRL_LEN      3
#define MEMORY_SLP_POWER_MODE_REGION0   (B_CTL_RB_BASE + 0x0210)
#define MEMORY_SLP_POWER_MODE_REGION1   (B_CTL_RB_BASE + 0x0214)
#define MEMORY_SLP_POWER_MODE_REGION2   (B_CTL_RB_BASE + 0x0218)
#define MEMORY_SLP_POWER_MODE_REGION3   (B_CTL_RB_BASE + 0x021C)
#define MEMORY_SLP_POWER_MODE_REGION4   (B_CTL_RB_BASE + 0x0220)

#define MEMORY_NOR_POWER_MODE_REGION0   (B_CTL_RB_BASE + 0x0228)
#define MEMORY_NOR_POWER_MODE_REGION1   (B_CTL_RB_BASE + 0x022C)
#define MEMORY_NOR_POWER_MODE_REGION2   (B_CTL_RB_BASE + 0x0230)
#define MEMORY_NOR_POWER_MODE_REGION3   (B_CTL_RB_BASE + 0x0234)
#define MEMORY_NOR_POWER_MODE_REGION4   (B_CTL_RB_BASE + 0x0238)

#define BCPU_TRACE_RAM_POWER_MODE       (B_CTL_RB_BASE + 0x23C)

#define MEMORY_POWER_MODE_STS_REGION0   (B_CTL_RB_BASE + 0x0240)
#define MEMORY_POWER_MODE_STS_REGION1   (B_CTL_RB_BASE + 0x0244)
#define MEMORY_POWER_MODE_STS_REGION2   (B_CTL_RB_BASE + 0x0248)
#define MEMORY_POWER_MODE_STS_REGION3   (B_CTL_RB_BASE + 0x024C)
#define MEMORY_POWER_MODE_STS_REGION4   (B_CTL_RB_BASE + 0x0250)

#define MEMORY_BRAM_SHARE_MODE_REG      (B_CTL_RB_BASE + 0x720)
#define MEMORY_BRAM_SHARE_MODE_BIT      0
#define MEMORY_BRAM_SHARE_MODE_LEN      2
#define MEMORY_BRAM_SHARE_MODE_GT       4


void hal_memory_set_sleep_power_mode(hal_memory_region_type_t region, hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    switch (region) {
        case HAL_MEMORY_REGION_0:
            reg16_setbits(MEMORY_SLP_POWER_MODE_REGION0, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        case HAL_MEMORY_REGION_1:
            reg16_setbits(MEMORY_SLP_POWER_MODE_REGION1, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        case HAL_MEMORY_REGION_2:
            reg16_setbits(MEMORY_SLP_POWER_MODE_REGION2, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        case HAL_MEMORY_REGION_3:
            reg16_setbits(MEMORY_SLP_POWER_MODE_REGION3, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            break; //lint !e527  unreachable code
    }
}

void hal_memory_set_normal_power_mode(hal_memory_region_type_t region, hal_memory_power_mode_type_t mode)
{
    if (mode >= HAL_MEMORY_POWER_NONE) {
        panic(PANIC_MEMORY, __LINE__);
        return; //lint !e527  unreachable code
    }

    switch (region) {
        case HAL_MEMORY_REGION_0:
            reg16_setbits(MEMORY_NOR_POWER_MODE_REGION0, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        case HAL_MEMORY_REGION_1:
            reg16_setbits(MEMORY_NOR_POWER_MODE_REGION1, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        case HAL_MEMORY_REGION_2:
            reg16_setbits(MEMORY_NOR_POWER_MODE_REGION2, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        case HAL_MEMORY_REGION_3:
            reg16_setbits(MEMORY_NOR_POWER_MODE_REGION3, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
            break;
        default:
            /* Should not be possible... */
            panic(PANIC_MEMORY, __LINE__);
            break; //lint !e527  unreachable code
    }
}

void hal_memory_bt_trace_ram_power_mode(hal_memory_power_mode_type_t mode)
{
    reg16_setbits(BCPU_TRACE_RAM_POWER_MODE, MEMORY_POWER_MODE_CTRL_BIT, MEMORY_POWER_MODE_CTRL_LEN, mode);
}

void hal_memory_bt_mem_clk_force_on_control(switch_type_t on)
{
    reg16_write(BT_MEM_CLK_FORCE_ON_REG, (uint16_t)on);
}

void hal_memory_set_rom_sleep_power_mode(hal_memory_rom_power_mode_t mode)
{
    reg16_write(MEMORY_SLP_POWER_MODE_REGION4, (uint16_t)mode);
}

void hal_memory_set_rom_normal_power_mode(hal_memory_rom_power_mode_t mode)
{
    reg16_write(MEMORY_NOR_POWER_MODE_REGION4, (uint16_t)mode);
}

void hal_memory_set_bram_share_config(hal_memory_bram_share_mode_t mode)
{
    reg16_clrbit(MEMORY_BRAM_SHARE_MODE_REG, MEMORY_BRAM_SHARE_MODE_GT);
    reg16_setbits(MEMORY_BRAM_SHARE_MODE_REG, MEMORY_BRAM_SHARE_MODE_BIT, MEMORY_BRAM_SHARE_MODE_LEN, mode);
    reg16_setbit(MEMORY_BRAM_SHARE_MODE_REG, MEMORY_BRAM_SHARE_MODE_GT);
}