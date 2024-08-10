/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: SLE AMIC VDT sample of client. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-26, Create file. \n
 */
#ifndef SLE_VDT_CLIENT_H
#define SLE_VDT_CLIENT_H

#include "sle_ssap_client.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

ssapc_write_param_t *get_g_sle_vdt_send_param(void);
uint8_t get_g_ssap_find_ready(void);
void sle_vdt_client_init(ssapc_notification_callback notification_cb, ssapc_indication_callback indication_cb);
void sle_vdt_start_scan(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif