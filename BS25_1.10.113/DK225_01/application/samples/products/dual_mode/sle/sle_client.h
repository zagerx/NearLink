/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: SLE Client header
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */
#ifndef SLE_CLIENT_H
#define SLE_CLIENT_H

#include "sle_ssap_client.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    uint8_t client_id;
    uint16_t conn_id;
    ssapc_find_service_result_t service;
} app_ssapc_find_service_t;

typedef struct {
    uint8_t client_id;
    uint16_t conn_id;
    ssapc_find_property_result_t property;
} app_ssapc_find_property_t;

void sle_save_remote_addr(const sle_addr_t *addr);
void sle_mouse_client_init(void);
errcode_t sle_register_client(void);
void sle_client_start_scan(void);
void sle_set_default_conn_param(void);
void sle_client_find_service(void);
void sle_client_find_property(app_ssapc_find_service_t *result);

uint16_t get_g_sle_mouse_client_conn_id(void);

uint8_t get_g_sle_mouse_client_conn_state(void);


void sle_dd_register_cbks(void);
void sle_cm_register_cbks(void);
void sle_ssapc_register_cbks(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SLE_CLIENT_H */