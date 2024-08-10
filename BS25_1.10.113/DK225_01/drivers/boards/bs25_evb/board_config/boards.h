/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Boards Definition Header. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-03, Create file. \n
 */
#ifndef SAMPLE_BOARDS_H
#define SAMPLE_BOARDS_H

#include "platform_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup boards_board Board
 * @ingroup  boards
 * @{
 */

#define BSP_LED_0  S_AGPIO13
#define BSP_LED_1  S_MGPIO24
#define BSP_LED_2  S_MGPIO25

#define BUTTON_0 S_MGPIO28

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif