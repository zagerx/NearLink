
/*
 * Copyright (c) @CompanyNameMagicTag. 2023. All rights reserved.
 *
 * Description: SLE Device Information Server module.
 */

/**
 * @defgroup SLE Device Information Server API
 * @ingroup
 * @{
 */
#ifndef SLE_DIS_SERVICE_H
#define SLE_DIS_SERVICE_H

#include "sle_ssap_server.h"

/**
 * @if Eng
 * @brief  SLE dis server init.
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
errcode_t sle_dis_server_add(void);
#endif