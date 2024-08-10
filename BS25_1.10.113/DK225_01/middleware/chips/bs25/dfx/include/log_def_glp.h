/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description:  common logging producer interface - need to change name of log.h in all the protocol core files
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef _LOG_DEF_GLP_H_
#define _LOG_DEF_GLP_H_

typedef enum {
// only for glp extern part
    _GLP_FILE_ID_EXT_START = 901,
    NB_ASSEMBLE_ACK_MSG_C,
    NB_PARSE_ACK_MSG_C,
    ASSEMBLE_KM_MSG_C,
    NB_ASSEMBLE_MEAS_MSG_C,
    NB_PARSE_MEAS_MSG_C,
    ASSEMBLE_RM_MSG_C,
    PARSE_RSLT_MSG_C,
    PARSE_SLEEP_MSG_C,
    ASSEMBLE_START_MSG_C,
    ASSEMBLE_STOP_MSG_C,
    NB_PARSE_TDOA_AT_CTRL_MSG_C,
    NB_PARSE_TDOA_AT_TIMEDIFF_MSG_C,
    NB_PARSE_TDOA_TA_RMARKER_MSG_C,
    ASSEMBLE_TS_MSG_C,
    GLP_CLIENT_ATT_C,
    GLP_SERVICES_C,
    GLP_SERVER_C,
    GLP_CLIENT_C,
    ASSEMBLE_BASE_IE_C,
    ASSEMBLE_CTSC_IE_C,
    PARSE_CTSC_IE_C,
    ASSEMBLE_RAR_IE_C,
    PARSE_RAR_IE_C,
    ASSEMBLE_RMD_IE_C,
    PARSE_RMD_IE_C,
    ASSEMBLE_RPRR_IE_C,
    PARSE_RPRR_IE_C,
    ASSEMBLE_RDM_IE_C,
    PARSE_RDM_IE_C,
    GLP_CLIENT_MAIN_C,
    ASSEMBLE_NB_PAYLOAD_C,
    NB_PAYLOAD_ATT_C,
    PARSE_NB_PAYLOAD_C,
    GLP_INTERFACE_C,
    FPGA_SEND_MSG_C,
    FPGA_GLP_SERVICE_C,
    ASSEMBLE_RADAR_MSG_C,
    SIMULATE_API_COMMAND_C,
    GLP_CLIENT_RDDT_C,
    ASSEMBLE_PR_IE_C,
    PARSE_PR_IE_C,
    PARSE_RM_DONE_MSG_C,
    PARSE_ABNORMAL_MSG_C,
    GLP_AT_C,
    _GLP_FILE_ID_EXT_END = 1023,
} log_file_list_enum_glp_t;
#endif
