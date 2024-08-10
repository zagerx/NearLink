/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: AMIC PDM Source. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-06, Create file. \n
 */
#include "osal_debug.h"
#include "pdm.h"
#include "hal_pdm.h"
#include "hal_dma.h"
#include "vdt_pdm.h"

#define PDM_FIFO_ADDR            (0x5208E080)
#define VDT_PDM_DMA_PRIORITY     3

#define VDT_PDM_LOG              "[vdt pdm]"

int32_t vdt_pdm_init(void)
{
    if (uapi_pdm_init() != ERRCODE_SUCC) {
        return 1;
    }

#ifdef CONFIG_ADC_SUPPORT_AMIC
    pdm_config_t config = { 0 };
    config.srcdn_src_mode = TRIPLE_EXTRACT;
    errcode_t ret = uapi_pdm_set_attr(HAL_PDM_AMIC, &config);
    if (ret != ERRCODE_SUCC) {
        osal_printk("%s Configure the PDM fail. %x\r\n", VDT_PDM_LOG, ret);
        return 1;
    }
#endif
    dma_port_clock_enable();
    osal_printk("%s PDM init config success.\r\n", VDT_PDM_LOG);

    return 0;
}

int32_t vdt_pdm_start_dma_transfer(uint32_t *pcm_buffer, dma_transfer_cb_t trans_done, dma_channel_t *channel)
{
    dma_ch_user_peripheral_config_t transfer_config;

    transfer_config.src = (uint32_t)(uintptr_t)PDM_FIFO_ADDR;
    transfer_config.dest = (uint32_t)(uintptr_t)pcm_buffer;
    transfer_config.transfer_num = CONFIG_USB_PDM_TRANSFER_LEN_BY_DMA;
    transfer_config.src_handshaking = 0x4;  /* MIC45_UPLINK_REQ 握手通道 */
    transfer_config.dest_handshaking = 0;
    transfer_config.trans_type = HAL_DMA_TRANS_PERIPHERAL_TO_MEMORY_DMA;
    transfer_config.trans_dir = HAL_DMA_TRANSFER_DIR_PERIPHERAL_TO_MEM;
    transfer_config.priority = VDT_PDM_DMA_PRIORITY;
    transfer_config.src_width = HAL_DMA_TRANSFER_WIDTH_32;
    transfer_config.dest_width = HAL_DMA_TRANSFER_WIDTH_32;
    transfer_config.burst_length = 1;
    transfer_config.src_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config.dest_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config.protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;

    errcode_t ret = uapi_dma_configure_peripheral_transfer_single(&transfer_config, channel,
                                                                  trans_done, (uintptr_t)NULL);
    if (ret != ERRCODE_SUCC) {
        osal_printk("%s Configure the DMA fail. %x\r\n", VDT_PDM_LOG, ret);
        return 1;
    }

    ret = uapi_dma_start_transfer((uint8_t)*channel);
    if (ret != ERRCODE_SUCC) {
        osal_printk("%s Start the DMA fail. %x\r\n", VDT_PDM_LOG, ret);
        return 1;
    }

    return 0;
}

uint32_t vdt_pdm_get_fifo_deepth(void)
{
    return uapi_pdm_get_fifo_deepth();
}