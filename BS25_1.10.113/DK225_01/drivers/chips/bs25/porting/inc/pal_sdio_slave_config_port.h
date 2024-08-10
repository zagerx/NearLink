/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: pal sec config port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-04， Create file. \n
 */

#ifndef PAL_SDIO_SLAVE_CONFIG_PORT_H
#define PAL_SDIO_SLAVE_CONFIG_PORT_H

#include "platform_core.h"

#define PAL_SDIO_BASE_ADDR SDIO_SLAVE_BASE_ADDR

/* a standard sd card information of cid, csd, scr */
#define PAL_SDIO_MEM_CID0_SET     (int32_t)(0x5600F239)
#define PAL_SDIO_MEM_CID1_SET     (int32_t)(0x1043CA28)
#define PAL_SDIO_MEM_CID2_SET     (int32_t)(0x30303030)
#define PAL_SDIO_MEM_CID3_SET     (int32_t)(0x1B534D30)
#define PAL_SDIO_MEM_CSD0_SET     (int32_t)(0x0A4000A3)
#define PAL_SDIO_MEM_CSD1_SET     (int32_t)(0x775D7F80)
#define PAL_SDIO_MEM_CSD2_SET     (int32_t)(0x5B58F000)
#define PAL_SDIO_MEM_CSD3_SET     (int32_t)(0x400E0011)
#define PAL_SDIO_MEM_SCR0_SET     (int32_t)(0x00000000)
#define PAL_SDIO_MEM_SCR1_SET     (int32_t)(0x02358003)
#define PAL_SDIO_MEM_CSD3_NEW_SET (int32_t)(0x400E005A)

#endif