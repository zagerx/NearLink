/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: At bt register.c fun \n
 * Author: @CompanyNameTag \n
 */

#ifndef AT_BT_CMD_REGISTER_H
#define AT_BT_CMD_REGISTER_H

#include <td_base.h>
#include "cmsis_os2.h"
#include "at.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define EXT_AT_BT_CMD_MAX_LEN   128

td_u32 uapi_at_bt_register_cmd(TD_CONST at_cmd_entry_t *cmd_tbl, td_u16 cmd_num);
td_void at_bt_cmd_register(td_void);
int testsuite_sw_at(int argc, char **argv);
void set_task_id(uint32_t *at_task_id, uint32_t *testsuite_task_id);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif
