
/*
 * Copyright (c) @CompanyNameMagicTag. 2022. All rights reserved.
 *
 * Description: SLE Discovery Server module.
 */

/**
 * @defgroup SLE MOUSE ADV API
 * @ingroup
 * @{
 */
#ifndef SLE_MOUSE_ADV_H
#define SLE_MOUSE_ADV_H

#include "sle_ssap_server.h"

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
errcode_t sle_sample_mouse_discovery_init(void);
#endif