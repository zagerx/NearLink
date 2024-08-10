/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 *
 * Description: SLE Server header
 * Author: @CompanyNameTag
 * History:
 * 2024-01-25, Create file.
 */
#ifndef SLE_SERVER_H
#define SLE_SERVER_H

#include "sle_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    uint16_t handle_in;
    uint16_t handle_out;
} sle_item_handle_t;

void sle_save_remote_addr(const sle_addr_t *addr);
errcode_t sle_register_server(void);
errcode_t sle_server_mouse_service_add(void);
errcode_t sle_server_dis_service_add(void);
void sle_server_adv_init(void);
void sle_server_direct_adv_init(void);

void sle_dd_register_cbks(void);
void sle_cm_register_cbks(void);
void sle_ssaps_register_cbks(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SLE_SERVER_H */