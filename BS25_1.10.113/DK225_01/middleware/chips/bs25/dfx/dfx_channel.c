/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: dfx channel
 * This file should be changed only infrequently and with great care.
 */
#include <stdint.h>
#include "dfx_adapt_layer.h"
#include "diag.h"
#include "diag_ind_src.h"
#include "diag_msg.h"
#include "errcode.h"
#include "diag_adapt_layer.h"
#include "log_uart.h"
#include "dfx_channel.h"
#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_BLE == DFX_YES)
#include "bts_def.h"
#endif
#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_SLE == DFX_YES)
#include "sle_ota.h"
#endif

diag_channel_id_t g_diag_channel_id = DIAG_CHANNEL_ID_INVALID;
#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_BLE == DFX_YES)
errcode_t bth_ota_reg_cbk(void *data_report, void *status_report);
errcode_t bth_ota_data_send(uint8_t *data_ptr, uint16_t data_len);
#endif

void diag_uart_rx_proc(uint8_t *buffer, uint16_t length)
{
    uapi_diag_channel_rx_mux_char_data(g_diag_channel_id, buffer, length);
}

static int32_t diag_channel_uart_output(void *fd, dfx_data_type_t data_type, uint8_t *data[], uint16_t len[],
                                        uint8_t cnt)
{
    unused(fd);

    if (data_type == DFX_DATA_DIAG_PKT_CRITICAL) {
        for (uint8_t i = 0; i < cnt; i++) {
            log_uart_send_buffer(data[i], len[i]);
        }
    } else {
        for (uint8_t i = 0; i < cnt; i++) {
            if (len[i] != 0) {
                log_uart_write_blocking(data[i], len[i]);
            }
        }
    }
    return ERRCODE_SUCC;
}

#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_BLE == DFX_YES)
static int32_t diag_channel_bt_output(void *fd, dfx_data_type_t data_type, uint8_t *data[], uint16_t len[], uint8_t cnt)
{
    unused(fd);
    unused(data_type);
    unused(cnt);
    return (int32_t)bth_ota_data_send(data[0], len[0]);
}

static errcode_t diag_channel_rx_bt_data(uint8_t *data, uint16_t size)
{
    return uapi_diag_channel_rx_frame_data(DIAG_CHANNEL_ID_1, data, size);
}

static void diag_channel_register_bt_callback(void)
{
    bth_ota_reg_cbk(diag_channel_rx_bt_data, NULL);
}
#endif

#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_SLE == DFX_YES)
static int32_t diag_channel_sle_output(void *fd, dfx_data_type_t type, uint8_t *data[], uint16_t len[], uint8_t cnt)
{
    unused(fd);
    unused(type);
    unused(cnt);
    int32_t size = len[0];
    if (sle_ota_data_ack(len[0], (uint8_t *)data[0]) != ERRCODE_SUCC) {
        size = 0;
    }
    return size;
}

static void diag_channel_rx_sle_data(const uint8_t *data, const uint16_t size)
{
    (void)uapi_diag_channel_rx_frame_data(DIAG_CHANNEL_ID_2, (uint8_t *)data, (uint16_t)size);
}
 
static void diag_channel_register_sle_callback(void)
{
    sle_ota_reg_chan_data_report_cbk(diag_channel_rx_sle_data);
}
#endif

errcode_t diag_register_channel(void)
{
    g_diag_channel_id = DIAG_CHANNEL_ID_0;
    if (uapi_diag_channel_init(g_diag_channel_id, DIAG_CHANNEL_ATTR_NEED_RX_BUF) != (errcode_t)ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    if (uapi_diag_channel_set_connect_hso_addr(g_diag_channel_id, diag_adapt_get_default_dst()) !=
        (errcode_t)ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    if (uapi_diag_channel_set_tx_hook(g_diag_channel_id, diag_channel_uart_output) != (errcode_t)ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_BLE == DFX_YES)
    if (uapi_diag_channel_init(DIAG_CHANNEL_ID_1, DIAG_CHANNEL_ATTR_NONE) != (errcode_t)ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    if (uapi_diag_channel_set_tx_hook(DIAG_CHANNEL_ID_1, diag_channel_bt_output) != (errcode_t)ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    diag_channel_register_bt_callback();
#endif
#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_SLE == DFX_YES)
    if (uapi_diag_channel_init(DIAG_CHANNEL_ID_2, DIAG_CHANNEL_ATTR_NONE) != (errcode_t)ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
 
    if (uapi_diag_channel_set_tx_hook(DIAG_CHANNEL_ID_2, diag_channel_sle_output) != (errcode_t)ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    diag_channel_register_sle_callback();
#endif
    return ERRCODE_SUCC;
}