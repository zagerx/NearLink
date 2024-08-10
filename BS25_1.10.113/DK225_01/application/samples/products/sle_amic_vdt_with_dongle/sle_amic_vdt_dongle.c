/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: SLE AMIC VDT Sample Source. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-26, Create file. \n
 */
#include "soc_osal.h"
#include "osal_debug.h"
#include "common_def.h"
#include "app_init.h"
#include "watchdog.h"
#include "pm_clock.h"
#include "securec.h"
#include "sle_amic_vdt_codec/sle_vdt_codec.h"
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_DONGLE)
#include "sle_amic_vdt_dongle/sle_amic_vdt_usb/sle_vdt_usb.h"
#include "sle_amic_vdt_dongle/sle_amic_vdt_client/sle_vdt_client.h"
#elif defined(CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_SERVER)
#include "adc.h"
#include "pdm.h"
#include "gpio.h"
#include "pinctrl.h"
#include "pm_pmu.h"
#include "hal_adc.h"
#include "hal_dma.h"
#include "sle_connection_manager.h"
#include "sle_amic_vdt/sle_amic_vdt_pdm/sle_vdt_pdm.h"
#include "sle_amic_vdt/sle_amic_vdt_server/sle_vdt_server.h"
#include "sle_amic_vdt/sle_amic_vdt_server/sle_vdt_server_adv.h"
#endif

#define SLE_VDT_TASK_STACK_SIZE         0x2000
#define SLE_VDT_TASK_DURATION_MS        2000
#define SLE_VDT_WAIT_SSAPS_READY        500
#define SLE_VDT_TASK_PRIO               24

#if defined(CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_DONGLE)
#define SLE_VDT_DONGLE_LOG              "[sle vdt dongle]"
#define RECV_BUFFER_LENGTH              4
#define SLE_VDT_DECODE_SIZE             1024
#define SLE_VDT_TRANSFER_SIZE           96

static uint8_t g_sle_usb_buffer[SLE_VDT_DECODE_SIZE] = { 0 };
static uint8_t g_out_decode_data1[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t g_out_decode_data2[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t g_out_decode_data3[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t g_out_decode_data4[SLE_VDT_TRANSFER_SIZE] = { 0 };
static uint8_t *g_out_decode_data[RECV_BUFFER_LENGTH] = {g_out_decode_data1, g_out_decode_data2,
    g_out_decode_data3, g_out_decode_data4};
uint8_t write_index = 0;
uint8_t read_index = 0;

static void sle_vdt_notification_cb(uint8_t client_id, uint16_t conn_id, ssapc_handle_value_t *data, errcode_t status)
{
    unused(client_id);
    unused(status);
    unused(conn_id);
    if (data == NULL || data->data == NULL) {
        return;
    }

    if ((write_index + 1) % RECV_BUFFER_LENGTH != read_index) {
        memcpy_s(g_out_decode_data[write_index], data->data_len, data->data, data->data_len);
        write_index = (write_index + 1) % RECV_BUFFER_LENGTH;
    }
}

static void sle_vdt_indication_cb(uint8_t client_id, uint16_t conn_id, ssapc_handle_value_t *data, errcode_t status)
{
    unused(client_id);
    unused(conn_id);
    unused(status);
    if (data == NULL || data->data == NULL) {
        return;
    }
    osal_printk("%s sle vdt indication recived data : %s\r\n", SLE_VDT_DONGLE_LOG, data->data);
}

static void *sle_amic_vdt_client_task(const char *arg)
{
    unused(arg);
    if (sle_vdt_usb_uac_init() != 0) {
        osal_printk("%s Init the USB UAC fail.\r\n", SLE_VDT_DONGLE_LOG);
    }
    sle_vdt_client_init(sle_vdt_notification_cb, sle_vdt_indication_cb);
    sle_vdt_codec_init();
    while (get_g_ssap_find_ready() == 0) {
        osal_msleep(SLE_VDT_WAIT_SSAPS_READY);
    }
    osal_printk("%s get_g_ssap_find_ready.\r\n", SLE_VDT_DONGLE_LOG);

    while (1) {
        uapi_watchdog_kick();
        if (read_index != write_index) {
            uint8_t *out_put_data = g_sle_usb_buffer;
            uint32_t decode_data_len = sle_vdt_codec_decode(g_out_decode_data[read_index], &out_put_data);
            if ((sle_vdt_usb_uac_send_data(out_put_data, decode_data_len) != 0)) {
                osal_printk("%s Send UAV to USB fail.\r\n", SLE_VDT_DONGLE_LOG);
            }
            read_index = (read_index + 1) % RECV_BUFFER_LENGTH;
        } else {
            osal_msleep(1);
        }
    }
    return NULL;
}

#elif defined(CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_SERVER)
#define SLE_VDT_SERVER_LOG              "[sle vdt server]"
#define ADC_GADC_CHANNEL7               7
#define ADC_GADC_CHANNEL6               6
#define SLE_VDT_MIC_OFFSET_16           16
#define SLE_VDT_MIC_OFFSET_24           24
#define SLE_VDT_ENCODE_SIZE             68
#define RING_BUFFER_NUMBER              4
#define PDM_DMA_TRANSFER_EVENT          1

static uint8_t g_sle_pdm_buffer[CONFIG_USB_UAC_MAX_RECORD_SIZE] = { 0 };
static uint32_t g_pdm_dma_data0[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t g_pdm_dma_data1[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t g_pdm_dma_data2[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t g_pdm_dma_data3[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint32_t *g_pdm_dma_data[RING_BUFFER_NUMBER] = {
    g_pdm_dma_data0, g_pdm_dma_data1, g_pdm_dma_data2, g_pdm_dma_data3};
static uint8_t g_out_encode_data[SLE_VDT_ENCODE_SIZE] = { 0 };
static uint8_t g_write_buffer_state = 0;
static uint8_t g_read_buffer_state = 0;
static osal_event g_dma_event_id;


static void sle_usb_vdt_dma_transfer_done_callback(uint8_t intr, uint8_t channel, uintptr_t arg);

static void ssaps_server_read_request_cbk(uint8_t server_id, uint16_t conn_id, ssaps_req_read_cb_t *read_cb_para,
                                          errcode_t status)
{
    osal_printk("%s ssaps read request cbk callback server_id:%x, conn_id:%x, handle:%x, status:%x\r\n",
        SLE_VDT_SERVER_LOG,
        server_id,
        conn_id,
        read_cb_para->handle,
        status);
}

static void ssaps_server_write_request_cbk(uint8_t server_id, uint16_t conn_id, ssaps_req_write_cb_t *write_cb_para,
                                           errcode_t status)
{
    unused(status);
    unused(conn_id);
    unused(server_id);
    if (write_cb_para == NULL || write_cb_para->value == NULL) {
        return;
    }
    osal_printk("sle vdt write recv data:%s\r\n", write_cb_para->value);
}

static void sle_usb_vdt_dma_transfer_restart(void)
{
    g_write_buffer_state = (g_write_buffer_state + 1) % RING_BUFFER_NUMBER;
    if (g_write_buffer_state != g_read_buffer_state) {
        uint32_t ret = osal_event_write(&g_dma_event_id, PDM_DMA_TRANSFER_EVENT);
        if (ret != OSAL_SUCCESS) {
            osal_printk("(%d)osal event write fail, ret = %x\r\n", __LINE__, ret);
        }
    }
    if (sle_vdt_pdm_start_dma_transfer(g_pdm_dma_data[g_write_buffer_state],
        sle_usb_vdt_dma_transfer_done_callback) != 0) {
        return;
    }
}

static void sle_usb_vdt_dma_transfer_done_callback(uint8_t intr, uint8_t channel, uintptr_t arg)
{
    unused(channel);
    unused(arg);

    switch (intr) {
        case HAL_DMA_INTERRUPT_TFR:
            sle_usb_vdt_dma_transfer_restart();
            break;
        case HAL_DMA_INTERRUPT_ERR:
            osal_printk("%s DMA transfer error.\r\n", SLE_VDT_SERVER_LOG);
            break;
        default:
            break;
    }
}

static void sle_vdt_set_phy_param(void)
{
    uint16_t conn_id = get_g_sle_conn_hdl();
    sle_set_phy_t param = { 0 };
    param.tx_format = 1;         // 无线帧类型2
    param.rx_format = 1;         // 无线帧类型2
    param.tx_phy = 0;            // 0 1M 1 2M 2 4M
    param.rx_phy = 0;
    param.tx_pilot_density = 0;  // 导频密度4:1
    param.rx_pilot_density = 0;  // 导频密度4:1
    param.g_feedback = 0;
    param.t_feedback = 0;
    if (sle_set_phy_param(conn_id, &param) != 0) {
        osal_printk("sle_set_phy_param fail!\r\n");
        return;
    }
    osal_printk("sle_vdt_set_phy_param ok!\r\n");
}

static void sle_vdt_adc_set_io(pin_t pin)
{
    uapi_pin_set_mode(pin, CONFIG_ADC_PIN_MODE);
    uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT);
    uapi_pin_set_pull(pin, PIN_PULL_NONE);
#if defined(CONFIG_PINCTRL_SUPPORT_IE)
    uapi_pin_set_ie(pin, PIN_IE_1);
#endif
}

static void sle_vdt_adc_init(void)
{
    uapi_pin_init();
    uapi_gpio_init();

    uapi_pmu_control(PMU_CONTROL_MICLDO_POWER, PMU_CONTROL_POWER_ON);
    uapi_pmu_ldo_set_voltage(PMU_LDO_ID_MICLDO, PMU_MICLDO_VSET_2V0);

    sle_vdt_adc_set_io(CONFIG_ADC_USE_PIN1);
    sle_vdt_adc_set_io(CONFIG_ADC_USE_PIN2);
    uapi_adc_init(ADC_CLOCK_NONE);
    uapi_adc_power_en(AFE_AMIC_MODE, true);
    uapi_adc_open_differential_channel(ADC_GADC_CHANNEL7, ADC_GADC_CHANNEL6);
    adc_calibration(AFE_AMIC_MODE, true, true, true);

    return;
}

static void *sle_amic_vdt_server_task(const char *arg)
{
    unused(arg);
    uint32_t buffer_filled_count = 0;
    if (osal_event_init(&g_dma_event_id) != OSAL_SUCCESS) {
        osal_printk("%s touch osal_event_init fail! \r\n", SLE_VDT_SERVER_LOG);
    }

    sle_vdt_server_init(ssaps_server_read_request_cbk, ssaps_server_write_request_cbk);
    while (get_g_ssaps_ready() == false) {
        osal_msleep(SLE_VDT_WAIT_SSAPS_READY);
    }
    while (get_g_conn_update() == 0) {
        osal_msleep(SLE_VDT_TASK_DURATION_MS);
    }
    sle_vdt_set_phy_param();

    sle_vdt_adc_init();
    if (sle_vdt_pdm_init() != 0) {
        osal_printk("%s Init the PDM fail.\r\n", SLE_VDT_SERVER_LOG);
    }
    if (uapi_pdm_start() != ERRCODE_SUCC) {
        osal_printk("%s Start the PDM fail.\r\n", SLE_VDT_SERVER_LOG);
    }

    sle_vdt_codec_init();

    if (sle_vdt_pdm_start_dma_transfer(g_pdm_dma_data[g_write_buffer_state],
        sle_usb_vdt_dma_transfer_done_callback) != 0) {
        return NULL;
    }

    while (1) {
        uapi_watchdog_kick();
        while (get_g_sle_amic_server_conn_state() == SLE_ACB_STATE_NONE ||
               get_g_sle_amic_server_conn_state() == SLE_ACB_STATE_DISCONNECTED || get_g_sle_amic_pair_state() != 0) {
            continue;
        }
        uint32_t ret = osal_event_read(&g_dma_event_id, PDM_DMA_TRANSFER_EVENT, OSAL_WAIT_FOREVER,
                                       OSAL_WAITMODE_AND | OSAL_WAITMODE_CLR);
        if (ret & PDM_DMA_TRANSFER_EVENT) {
            for (uint32_t i = 0; i < CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA; i++) {
                g_sle_pdm_buffer[buffer_filled_count++] =
                    (uint8_t)(g_pdm_dma_data[g_read_buffer_state][i] >> SLE_VDT_MIC_OFFSET_16);
                g_sle_pdm_buffer[buffer_filled_count++] =
                    (uint8_t)(g_pdm_dma_data[g_read_buffer_state][i] >> SLE_VDT_MIC_OFFSET_24);
            }
            g_read_buffer_state = (g_read_buffer_state + 1) % RING_BUFFER_NUMBER;
            buffer_filled_count = 0;
            uint8_t *out_data = g_out_encode_data;
            uint32_t encode_data_len = sle_vdt_codec_encode(g_sle_pdm_buffer, &out_data);
            sle_vdt_server_send_report_by_handle(out_data, encode_data_len);
        }
    }
    return NULL;
}
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_SERVER */

static void sle_amic_vdt_with_dongle_entry(void)
{
    osal_task *task_handle;
    if (uapi_clock_control(CLOCK_CLKEN_ID_MCU_CORE, CLOCK_FREQ_LEVEL_HIGH) == ERRCODE_SUCC) {
        osal_printk("Config succ.\r\n");
    } else {
        osal_printk("Config fail.\r\n");
    }
    osal_kthread_lock();
#if defined(CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_DONGLE)
    task_handle = osal_kthread_create((osal_kthread_handler)sle_amic_vdt_client_task, 0, "SleAmicVdtDongleTask",
                                      SLE_VDT_TASK_STACK_SIZE);
#elif defined(CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_SERVER)
    task_handle = osal_kthread_create((osal_kthread_handler)sle_amic_vdt_server_task, 0, "SleAmicVdtServerTask",
                                      SLE_VDT_TASK_STACK_SIZE);
#endif /* CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_DONGLE */
    if (task_handle != NULL) {
        osal_kthread_set_priority(task_handle, SLE_VDT_TASK_PRIO);
        osal_kfree(task_handle);
    }
    osal_kthread_unlock();
}

/* Run the sle_vdt_with_dongle_entry. */
app_run(sle_amic_vdt_with_dongle_entry);