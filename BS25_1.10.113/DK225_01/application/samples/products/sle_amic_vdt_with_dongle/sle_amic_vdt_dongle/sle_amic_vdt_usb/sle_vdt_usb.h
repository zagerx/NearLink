/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: SLE AMIC VDT USB Header. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-26, Create file. \n
 */
#ifndef SLE_VDT_USB_H
#define SLE_VDT_USB_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SLE_VDT_USB_LOG "[sle_vdt usb]"

int32_t sle_vdt_usb_uac_init(void);
int32_t sle_vdt_usb_uac_send_data(const uint8_t *buf, int len);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif