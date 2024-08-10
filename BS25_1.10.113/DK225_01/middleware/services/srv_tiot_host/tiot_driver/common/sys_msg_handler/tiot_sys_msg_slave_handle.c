/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description:  \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-29, Create file. \n
 */
#include "tiot_sys_msg_handle.h"
#include "tiot_sys_msg_types.h"
#include "tiot_slave_board_port.h"
#include "tiot_pm_slave.h"
#include "tiot_board_log.h"

void tiot_pkt_sys_msg_slave_handle(tiot_controller *ctrl, uint8_t code)
{
    switch (code) {
        case SYS_PL_BFGNI_HOST_ASKTO_SLP:
            tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_REQUEST_SLEEP);
            break;
        case SYS_PL_BFGNI_HOST_DISALLOW_DEVSLP:
            tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_WAKEUP_ACK);
            break;
        default:
            tiot_print_warning("[TIoT:pkt]recv unknown sys msg:0x%x\n", code);
            break;
    }
}
