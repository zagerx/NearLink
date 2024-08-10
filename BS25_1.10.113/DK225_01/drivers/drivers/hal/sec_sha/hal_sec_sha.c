/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: HAL SEC SHA
 * Author: @CompanyNameTag
 * Create: 2020-01-20
 */

#include "hal_sec_sha.h"
#include "hal_sec_common.h"
#include "pal_sec_config_port.h"
#include "chip_io.h"

#define SHA_CTRL_REG                (PAL_SEC_CTRL_REG_BASE + 0x80)
#define SHA_CFG_REG                 (PAL_SEC_CTRL_REG_BASE + 0x84)
#define SHA_DATA_LEN_REG            (PAL_SEC_CTRL_REG_BASE + 0x88)
#define SHA_IVIN_START_ADDR_REG     (PAL_SEC_CTRL_REG_BASE + 0x8C)
#define SHA_DATA_IN_START_ADDR_REG  (PAL_SEC_CTRL_REG_BASE + 0x90)
#define SHA_DATA_OUT_START_ADDR_REG (PAL_SEC_CTRL_REG_BASE + 0x94)
#define SHA_ALL_DATA_LEN_REG        (PAL_SEC_CTRL_REG_BASE + 0x98)

#define SHA_CLKEN_OFFSET            2
#define SHA_END_OFFSET              2
#define SHA_START_OFFSET            1
#define SHA_EN_OFFSET               0
#define SHA_PADDING_DIS_OFFSET      1
#define SHA_IVIN_SEL_OFFSET         0
#define SHA_COMPUTE_MODE_OFFSET     4

void hal_sec_sha_enable(void)
{
    hal_sec_comm_enable(SEC_SHA);
    reg32_setbit(SHA_CTRL_REG, SHA_EN_OFFSET);
}

void hal_sec_sha_disable(void)
{
    writel(SHA_IVIN_START_ADDR_REG, 0);
    reg32_clrbit(SHA_CTRL_REG, SHA_EN_OFFSET);
    hal_sec_comm_disable(SEC_SHA);
}

void hal_sec_sha_cfg_padding(sec_padding_type_t sha_padding_dis)
{
    if (sha_padding_dis == SEC_SHA_NO_PADDING) {
        reg32_setbit(SHA_CFG_REG, SHA_PADDING_DIS_OFFSET);
    } else if (sha_padding_dis == SEC_SHA_AUTO_PADDING) {
        reg32_clrbit(SHA_CFG_REG, SHA_PADDING_DIS_OFFSET);
    }
}

void hal_sec_sha_cfg_ivin(sec_ivin_type_t sha_ivin_sel, uint32_t sha_iniv_addr)
{
    if (sha_ivin_sel == SEC_SHA_IVIN_USER_CFG) {
        reg32_setbit(SHA_CFG_REG, SHA_IVIN_SEL_OFFSET);
        writel(SHA_IVIN_START_ADDR_REG, sha_iniv_addr);
    } else if (sha_ivin_sel == SEC_SHA_IVIN_DEFAULT_CFG) {
        reg32_clrbit(SHA_CFG_REG, SHA_IVIN_SEL_OFFSET);
    }
}

void hal_sec_sha_compute_mode(sec_sha_compute_mode_t sec_sha_compute_mode)
{
#if SHA512_EXIST == YES
    if (sec_sha_compute_mode == SEC_SHA256) {
        reg32_setbit(SHA_CFG_REG, SHA_COMPUTE_MODE_OFFSET);
    } else if (sec_sha_compute_mode == SEC_SHA512) {
        reg32_clrbit(SHA_CFG_REG, SHA_COMPUTE_MODE_OFFSET);
    }
#else
    UNUSED(sec_sha_compute_mode);
#endif
}

void hal_sec_sha_cfg_data(uint32_t data_in_addr, uint32_t data_out_addr, uint32_t data_length, uint32_t total_length)
{
    writel(SHA_DATA_LEN_REG, data_length);
    writel(SHA_ALL_DATA_LEN_REG, total_length);
    writel(SHA_DATA_IN_START_ADDR_REG, data_in_addr);
    writel(SHA_DATA_OUT_START_ADDR_REG, data_out_addr);
}

void hal_sec_sha_start(void)
{
    reg32_setbit(SHA_CTRL_REG, SHA_START_OFFSET);
}

bool hal_sec_sha_is_finish(void)
{
    return reg32_getbit(SHA_CTRL_REG, SHA_END_OFFSET);
}