
/*
 * Copyright (c) @CompanyNameMagicTag. 2022. All rights reserved.
 *
 * Description: SLE MOUSE Server module.
 */

/**
 * @defgroup SLE MOUSE SERVER API
 * @ingroup
 * @{
 */
#ifndef SLE_MOUSE_SERVER_H
#define SLE_MOUSE_SERVER_H

#include "sle_ssap_server.h"

typedef struct {
    uint16_t handle_in;
    uint16_t handle_out;
} sle_item_handle_t;

typedef enum {
    HANDLE_IN,
    HANDLE_OUT,
    HANDLE_INVALID,
} sle_handle_t;

/**
 * @if Eng
 * @brief  SLE mouse server init.
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    Excute successfully
 * @retval ERRCODE_SLE_FAIL       Execute fail
 * @par Dependency:
 * @li sle_ssap_server.h
 * @else
 * @brief  SLE MOUSE服务器初始化。
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    执行成功
 * @retval ERRCODE_SLE_FAIL       执行失败
 * @par 依赖:
 * @li sle_ssap_server.h
 * @endif
 */

errcode_t sle_mouse_uuid_server_init(void);
/**
 * @if Eng
 * @brief  SLE get server .
 * @attention  NULL
 * @retval server id
 * @par Dependency:
 * @li sle_ssap_server.h
 * @else
 * @brief 获取服务器id。
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    执行成功
 * @retval ERRCODE_SLE_FAIL       执行失败
 * @par 依赖:
 * @li sle_ssap_server.h
 * @endif
 */
errcode_t sle_get_server_identifier(uint8_t *server_id);
/**
 * @if Eng
 * @brief  SLE register server.
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    Excute successfully
 * @retval ERRCODE_SLE_FAIL       Execute fail
 * @par Dependency:
 * @li sle_ssap_server.h
 * @else
 * @brief  SLE 注册服务.
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    执行成功
 * @retval ERRCODE_SLE_FAIL       执行失败
 * @par 依赖:
 * @li sle_ssap_server.h
 * @endif
 */
errcode_t sle_register_server(void);
/**
 * @if Eng
 * @brief  SLE set uuid.
 * @attention  NULL
 * @param  [in]  uuid             uuid.
 * @param  [in]  service_uuid     service uuid。
 * @retval ERRCODE_SLE_SUCCESS    Excute successfully
 * @retval ERRCODE_SLE_FAIL       Execute fail
 * @par Dependency:
 * @li sle_ssap_server.h
 * @else
 * @brief  SLE 设置uuid.
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    执行成功
 * @retval ERRCODE_SLE_FAIL       执行失败
 * @par 依赖:
 * @li sle_ssap_server.h
 * @endif
 */
errcode_t sle_sample_set_uuid(uint8_t *uuid, sle_uuid_t *service_uuid);
/**
 * @if Eng
 * @brief  SLE add descriptor.
 * @attention  NULL
 * @param  [in]  properties       properties of descriptor.
 * @param  [in]  service_handle   service handle.
 * @param  [in]  property_handle  property handle.
 * @param  [in]  len              length of data.
 * @param  [in]  data             data.
 * @retval ERRCODE_SLE_SUCCESS    Excute successfully
 * @retval ERRCODE_SLE_FAIL       Execute fail
 * @par Dependency:
 * @li sle_ssap_server.h
 * @else
 * @brief  SLE 设置uuid.
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    执行成功
 * @retval ERRCODE_SLE_FAIL       执行失败
 * @par 依赖:
 * @li sle_ssap_server.h
 * @endif
 */
errcode_t sle_sample_add_descriptor_interface(uint32_t properties, uint16_t service_handle, uint16_t property_handle,
    uint16_t len, uint8_t *data);
/**
 * @if Eng
 * @brief  SLE set property.
 * @attention  NULL
 * @param  [in]  properties       properties of property.
 * @param  [in]  uuid             service uuid.
 * @param  [in]  len              length of data.
 * @param  [in]  data             data.
 * @param  [in]  service_hdl      service handle.
 * @retval ERRCODE_SLE_SUCCESS    Excute successfully
 * @retval ERRCODE_SLE_FAIL       Execute fail
 * @par Dependency:
 * @li sle_ssap_server.h
 * @else
 * @brief  SLE 设置uuid.
 * @attention  NULL
 * @retval ERRCODE_SLE_SUCCESS    执行成功
 * @retval ERRCODE_SLE_FAIL       执行失败
 * @par 依赖:
 * @li sle_ssap_server.h
 * @endif
 */
errcode_t sle_sample_add_property_interface(uint32_t properties, uint8_t *uuid, uint16_t len, uint8_t *data,
    sle_item_handle_t* service_hdl);
#endif