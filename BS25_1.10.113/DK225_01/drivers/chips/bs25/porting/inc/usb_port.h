/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: USB port for project
 * Author: @CompanyNameTag
 * Create:  2021-09-27
 */
#ifndef USB_PORT_H
#define USB_PORT_H

#include "vectors.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define USB_BASE_ADDR   0x58000000                              /*!< The base address of USB. */
#define MAX_PERIO_FIFOS 15                                      /*!< Maximum number of Periodic FIFOs. */
#define MAX_TX_FIFOS    15                                      /*!< Maximum number of Periodic FIFOs. */
#define MAX_EPS_CHANNELS 4                                      /*!< Maximum number of Endpoints/HostChannels. */
#define WAIT_MAX_COUNT_10000 10000
#define WAIT_MAX_COUNT_100000 100000
#define WAIT_PERIOD_1_US 1
#define WAIT_PERIOD_10_US 10
#define WAIT_PERIOD_50_US 50
#define WAIT_PERIOD_100_US 100
#define WAIT_PERIOD_5_MS 5
#define LOCAL_INTERRUPT0 26
#define USB_IRQ (LOCAL_INTERRUPT0 + 63)
#define usb_remap_addr(addr_mcu_side)       (0x81000000 + (addr_mcu_side))

void usb_port_phy_init(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
