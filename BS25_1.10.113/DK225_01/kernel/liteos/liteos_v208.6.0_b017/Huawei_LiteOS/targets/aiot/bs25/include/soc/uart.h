/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#ifndef _SOC_UART_H
#define _SOC_UART_H

#include "los_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define UART_WITH_LOCK    1
#define UART_WITHOUT_LOCK 0

extern VOID UartPuts(const CHAR *s, UINT32 len, BOOL isLock);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
