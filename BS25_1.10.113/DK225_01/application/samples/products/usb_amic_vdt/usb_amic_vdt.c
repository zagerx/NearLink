/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: USB AMIC Sample Source. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-06, Create file. \n
 */
#include "soc_osal.h"
#include "app_init.h"
#include "hal_dma.h"
#include "pdm.h"
#include "dma.h"
#include "gpio.h"
#include "pinctrl.h"
#include "adc.h"
#include "adc_porting.h"
#include "watchdog.h"
#include "vdt_usb/vdt_usb.h"
#include "vdt_pdm/vdt_pdm.h"

#define ADC_GADC_CHANNEL7                   7
#define ADC_GADC_CHANNEL6                   6
#define USB_VDT_OFFSET_16                   16
#define USB_VDT_OFFSET_24                   24
#define PDM_DMA_TRANSFER_EVENT              1

#define USB_VDT_TASK_DURATION_MS            2000
#define USB_VDT_TASK_STACK_SIZE             0x1000
#define USB_VDT_TASK_PRIO                   24

static void usb_vdt_dma_transfer_done_callback(uint8_t intr, uint8_t channel, uintptr_t arg);

static uint32_t g_pdm_dma_data[CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA] = { 0 };
static uint8_t g_pdm_usb_buffer[CONFIG_USB_UAC_MAX_RECORD_SIZE] = { 0 };
static uint32_t g_pdm_usb_buffer_filled_count = 0;
static osal_event g_dma_event_id;
dma_channel_t channel = 0;

static void usb_vdt_dma_transfer_restart(void)
{
    for (uint32_t i = 0; i < CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA; i++) {
        g_pdm_usb_buffer[g_pdm_usb_buffer_filled_count++] = (uint8_t)(g_pdm_dma_data[i] >> USB_VDT_OFFSET_16);
        g_pdm_usb_buffer[g_pdm_usb_buffer_filled_count++] = (uint8_t)(g_pdm_dma_data[i] >> USB_VDT_OFFSET_24);
    }

    if (vdt_pdm_start_dma_transfer(g_pdm_dma_data, usb_vdt_dma_transfer_done_callback, &channel) != 0) {
        return;
    }
    if (g_pdm_usb_buffer_filled_count >= CONFIG_USB_UAC_MAX_RECORD_SIZE) {
        uint32_t ret = osal_event_write(&g_dma_event_id, PDM_DMA_TRANSFER_EVENT);
        if (ret != OSAL_SUCCESS) {
            osal_printk("(%d)osal event write fail, ret = %x\r\n", __LINE__, ret);
        }
    }
}

static void usb_vdt_dma_transfer_done_callback(uint8_t intr, uint8_t channel, uintptr_t arg)
{
    unused(channel);
    unused(arg);

    switch (intr) {
        case HAL_DMA_INTERRUPT_TFR:
            usb_vdt_dma_transfer_restart();
            break;
        case HAL_DMA_INTERRUPT_ERR:
            osal_printk("%s DMA transfer error.\r\n", VDT_USB_LOG);
            break;
        default:
            break;
    }
}

static void vdt_adc_set_io(pin_t pin)
{
    uapi_pin_set_mode(pin, CONFIG_ADC_PIN_MODE);
    uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT);
    uapi_pin_set_pull(pin, PIN_PULL_NONE);
#if defined(CONFIG_PINCTRL_SUPPORT_IE)
    uapi_pin_set_ie(pin, PIN_IE_1);
#endif
}

static void vdt_adc_init(void)
{
    uapi_pin_init();
    uapi_gpio_init();

    vdt_adc_set_io(CONFIG_ADC_USE_PIN1);
    vdt_adc_set_io(CONFIG_ADC_USE_PIN2);
    uapi_adc_init(ADC_CLOCK_NONE);
#ifdef CONFIG_ADC_SUPPORT_AMIC
    uapi_adc_power_en(AFE_AMIC_MODE, true);
    uapi_adc_open_differential_channel(ADC_GADC_CHANNEL7, ADC_GADC_CHANNEL6);
    adc_calibration(AFE_AMIC_MODE, true, true, true);
#endif
    return;
}

static void *usb_amic_vdt_task(const char *arg)
{
    unused(arg);
    if (osal_event_init(&g_dma_event_id) != OSAL_SUCCESS) {
        osal_printk("%s touch osal_event_init fail! \r\n", VDT_USB_LOG);
    }
    if (vdt_usb_uac_init() != 0) {
        osal_printk("%s Init the USB UAC fail.\r\n", VDT_USB_LOG);
    }

    vdt_adc_init();

    if (vdt_pdm_init() != 0) {
        osal_printk("%s Init the PDM fail.\r\n", VDT_USB_LOG);
    }

    if (uapi_pdm_start() != ERRCODE_SUCC) {
        osal_printk("%s Start the PDM fail.\r\n", VDT_USB_LOG);
    }

    uapi_dma_init();
    uapi_dma_open();

    if (vdt_pdm_start_dma_transfer(g_pdm_dma_data, usb_vdt_dma_transfer_done_callback, &channel) != 0) {
        return NULL;
    }

    while (1) {
        uapi_watchdog_kick();
        uint32_t ret = osal_event_read(&g_dma_event_id, PDM_DMA_TRANSFER_EVENT, OSAL_WAIT_FOREVER,
                                       OSAL_WAITMODE_AND | OSAL_WAITMODE_CLR);
        if (ret & PDM_DMA_TRANSFER_EVENT) {
            if ((vdt_usb_uac_send_data(g_pdm_usb_buffer, CONFIG_USB_UAC_MAX_RECORD_SIZE) != 0)) {
                osal_printk("%s Send UAV to USB fail.\r\n", VDT_USB_LOG);
                continue;
            }
            g_pdm_usb_buffer_filled_count = 0;
        }
    }

    return NULL;
}

static void usb_amic_vdt_entry(void)
{
    osal_task *task_handle;

    osal_kthread_lock();
    task_handle = osal_kthread_create((osal_kthread_handler)usb_amic_vdt_task, 0, "UsbAmicVdtTask",
                                      USB_VDT_TASK_STACK_SIZE);
    if (task_handle != NULL) {
        osal_kthread_set_priority(task_handle, USB_VDT_TASK_PRIO);
        free(task_handle);
    }
    osal_kthread_unlock();
}

/* Run the usb_amic_vdt_entry. */
app_run(usb_amic_vdt_entry);