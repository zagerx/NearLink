/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: HAL RSA.
 * Author: @CompanyNameTagTag
 * Create: 2022-04-11
 */
#include "hal_sec_rsa.h"
#include "hal_sec_common.h"
#include "chip_io.h"
#include "securec.h"

#define SEC_RSA_BUSY_REG          (PAL_RSAV2_REG_BASE + 0x50)
#define SEC_RSA_MOD_REG           (PAL_RSAV2_REG_BASE + 0x54)
#define SEC_RSA_WSEC_REG          (PAL_RSAV2_REG_BASE + 0x58)
#define SEC_RSA_WPKT_REG          (PAL_RSAV2_REG_BASE + 0x5C)
#define SEC_RSA_RPKT_REG          (PAL_RSAV2_REG_BASE + 0x60)
#define SEC_RSA_RRSLT_REG         (PAL_RSAV2_REG_BASE + 0x64)
#define SEC_RSA_START_REG         (PAL_RSAV2_REG_BASE + 0x68)
#define SEC_RSA_ADDR_REG          (PAL_RSAV2_REG_BASE + 0x6C)
#define SEC_RSA_ERROR_REG         (PAL_RSAV2_REG_BASE + 0x70)
#define SEC_RSA_CRC16_REG         (PAL_RSAV2_REG_BASE + 0x74)

#define SEC_RSA_START             1
#define SEC_RSA_IS_NOT_BUSY       0
#define SEC_RSA_INPUT_TIMES       32
#define SEC_RSA_4_BYTES_SHIFT     4
#define SEC_RSA_RAM_IS_CLEAR      0
#define SEC_RSA_CRC_SHIFT         8
#define SEC_RSA_CRC_MASK          0xff
#define SEC_RSA_KEY_ARRAY_LENGTH  0x100
#define SEC_RSA_CRC_START         0xFFFF

/* Table of CRC constants - implements x^16+x^12+x^5+1 */
const uint16_t g_crc16_tab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

static uint16_t hal_sec_rsav2_crc(uint16_t crc_start, uint8_t *buf, uint32_t len)
{
    if ((buf == NULL) || (len == 0)) {
        return 0;
    }

    uint16_t cksum = crc_start;
    for (uint32_t i = 0; i < len; i++) {
        cksum = g_crc16_tab[((cksum >> SEC_RSA_CRC_SHIFT) ^ *buf++) & SEC_RSA_CRC_MASK] ^ (cksum << SEC_RSA_CRC_SHIFT);
    }
    return cksum;
}

void hal_sec_rsav2_enable(void)
{
    hal_sec_comm_enable(SEC_RSAV2);
}

void hal_sec_rsav2_disable(void)
{
    hal_sec_comm_disable(SEC_RSAV2);
}

void hal_sec_rsav2_cfg(rsav2_alg_cfg_t rsa_alg_cfg)
{
    while (readl(SEC_RSA_BUSY_REG) != SEC_RSA_IS_NOT_BUSY) {}
    uint32_t mode = *(uint32_t *)&rsa_alg_cfg.mode;
    writel(SEC_RSA_MOD_REG, mode);
    uint16_t input_times = (SEC_RSA_INPUT_TIMES << rsa_alg_cfg.mode.sec_rsa_key_width);

    // plain text
    for (uint16_t i = 0; i < input_times; i++) {
        writel(SEC_RSA_WPKT_REG, readl(rsa_alg_cfg.wpkt));
        rsa_alg_cfg.wpkt += SEC_RSA_4_BYTES_SHIFT;
    }
    // N
    for (uint16_t i = 0; i < input_times; i++) {
        writel(SEC_RSA_WSEC_REG, readl(rsa_alg_cfg.wsec));
        rsa_alg_cfg.wsec += SEC_RSA_4_BYTES_SHIFT;
    }
    // D
    for (uint16_t i = 0; i < input_times; i++) {
        writel(SEC_RSA_WSEC_REG, readl(rsa_alg_cfg.wsec));
        rsa_alg_cfg.wsec += SEC_RSA_4_BYTES_SHIFT;
    }
}

void hal_sec_rsav2_start(void)
{
    writel(SEC_RSA_START_REG, SEC_RSA_START);
}

bool hal_sec_rsav2_is_finish(void)
{
    return (readl(SEC_RSA_BUSY_REG) == SEC_RSA_IS_NOT_BUSY);
}

void hal_sec_rsav2_copy_result(rsav2_alg_cfg_t rsa_alg_cfg)
{
    uint16_t input_times = (SEC_RSA_INPUT_TIMES << rsa_alg_cfg.mode.sec_rsa_key_width);

    for (uint16_t i = 0; i < input_times; i++) {
        uint32_t result = readl(SEC_RSA_RRSLT_REG);
        writel(rsa_alg_cfg.rrslt, result);
        rsa_alg_cfg.rrslt += SEC_RSA_4_BYTES_SHIFT;
    }
}

bool hal_sec_rsav2_clear_ram(rsav2_alg_cfg_t rsa_alg_cfg)
{
    while (readl(SEC_RSA_BUSY_REG) != 0) {}
    uint32_t mode = *(uint32_t *)&rsa_alg_cfg.mode;
    writel(SEC_RSA_MOD_REG, mode);
    hal_sec_rsav2_start();
    while (readl(SEC_RSA_BUSY_REG) != 0) {}
    uint8_t error_count = 0;
    if (readl(SEC_RSA_RPKT_REG) != 0) {
        error_count++;
    }
    if (readl(SEC_RSA_RRSLT_REG) != 0) {
        error_count++;
    }
    return (error_count == SEC_RSA_RAM_IS_CLEAR);
}

bool hal_sec_rsav2_clear_key(rsav2_alg_cfg_t rsa_alg_cfg)
{
    while (readl(SEC_RSA_BUSY_REG) != 0) {}
    uint32_t mode = *(uint32_t *)&rsa_alg_cfg.mode;
    writel(SEC_RSA_MOD_REG, mode);
    hal_sec_rsav2_start();
    while (readl(SEC_RSA_BUSY_REG) != 0) {}

    uint16_t array_length = (SEC_RSA_KEY_ARRAY_LENGTH << rsa_alg_cfg.mode.sec_rsa_key_width);
    uint16_t crc16 = SEC_RSA_CRC_START;
    memset_s((void *)rsa_alg_cfg.wsec, array_length, 0x0, array_length);
    crc16 = hal_sec_rsav2_crc(crc16, (uint8_t*)rsa_alg_cfg.wsec, array_length);
    return (crc16 == readl(SEC_RSA_CRC16_REG));
}