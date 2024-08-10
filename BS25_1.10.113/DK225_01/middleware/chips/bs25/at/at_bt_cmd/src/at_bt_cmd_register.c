/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: At bt register.c fun \n
 * Author: @CompanyNameTag \n
 */

#include "at_bt_cmd_register.h"
#include "at_bt.h"
#include "at.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_u32 uapi_at_bt_register_cmd(TD_CONST at_cmd_entry_t *cmd_tbl, td_u16 cmd_num)
{
    td_u32 ret = EXT_ERR_FAILURE;

    ret = uapi_at_cmd_table_register(cmd_tbl, (uint32_t)cmd_num, EXT_AT_BT_CMD_MAX_LEN);
    if (ret != 0) {
        uapi_at_report_to_single_channel(0, "uapi_at_bt_register_cmd \r\n");
    }
    return ret;
}

td_void at_bt_cmd_register(td_void)
{
    los_at_bt_cmd_register();
}
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
