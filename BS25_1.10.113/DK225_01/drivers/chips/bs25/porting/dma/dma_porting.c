/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides dma port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-16， Create file. \n
 */

#include "common_def.h"
#include "soc_osal.h"
#include "hal_dmac_v100.h"
#include "dma_porting.h"
#include "platform_core.h"
#include "chip_core_irq.h"

#define DMA_BASE_ADDR 0x52070000
#define SDMA_BASE_ADDR 0x520A0000

#define DMA_CHANNEL_HS_SELECT     0x52000920

#define HAL_DMA_CHANNEL_HANDSHAKING_CHS  2
#define HAL_DMA_CHANNEL_HANDSHAKING_BITS 8

#define hal_dma_channel_handshaking_select_offset(ch)  (((uint32_t)(ch)) / HAL_DMA_CHANNEL_HANDSHAKING_CHS)
#define hal_dma_channel_handshaking_select_pos(ch)     ((((uint32_t)(ch)) % HAL_DMA_CHANNEL_HANDSHAKING_CHS) * \
                                                        HAL_DMA_CHANNEL_HANDSHAKING_BITS)
#define DMA_HARDSHAKING_OFFSET_NUM 4

uintptr_t g_dma_base_addr =  (uintptr_t)DMA_BASE_ADDR;
uintptr_t g_sdma_base_addr =  (uintptr_t)SDMA_BASE_ADDR;

typedef struct dma_hs_regs {
    volatile uint32_t dma_hs_ch_config[DMA_HARDSHAKING_OFFSET_NUM];    /* DMA handshaking configuration. */
} dma_hs_regs_t;

static void irq_dma_handler(void)
{
    hal_dma_irq_handler();
    osal_irq_clear(M_DMA_IRQN);
}

#if defined(CONFIG_DMA_SUPPORT_SMDMA)
static void irq_sdma_handler(void)
{
    hal_sdma_irq_handler();
    osal_irq_clear(M_SDMA_IRQN);
}
#endif /* CONFIG_DMA_SUPPORT_SMDMA */

void dma_port_register_irq(void)
{
    osal_irq_request(M_DMA_IRQN, (osal_irq_handler)irq_dma_handler, NULL, NULL, NULL);
    osal_irq_enable(M_DMA_IRQN);
#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    osal_irq_request(M_SDMA_IRQN, (osal_irq_handler)irq_sdma_handler, NULL, NULL, NULL);
    osal_irq_enable(M_SDMA_IRQN);
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
}

void dma_port_unregister_irq(void)
{
    osal_irq_disable(M_DMA_IRQN + LOCAL_INTERRUPT0);
    osal_irq_free(M_DMA_IRQN + LOCAL_INTERRUPT0, NULL);
#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    osal_irq_disable(M_SDMA_IRQN + LOCAL_INTERRUPT0);
    osal_irq_free(M_SDMA_IRQN + LOCAL_INTERRUPT0, NULL);
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
}

void dma_port_set_handshaking_channel_status(hal_dma_handshaking_source_t channel, bool on)
{
    unused(channel);
    unused(on);
}

errcode_t dma_port_set_mux_channel(dma_channel_t ch, hal_dma_transfer_peri_config_t *per_cfg)
{
    unused(ch);
    unused(per_cfg);
    return ERRCODE_SUCC;
}

void dma_port_set_channel_handshaking_source(hal_dma_hardshaking_channel_t ch, hal_dma_handshaking_source_t source)
{
    dma_hs_regs_t *dma_hs_regs = (dma_hs_regs_t *)(uintptr_t)DMA_CHANNEL_HS_SELECT;
    uint32_t offset = hal_dma_channel_handshaking_select_offset(ch);
    uint32_t pos = hal_dma_channel_handshaking_select_pos(ch);
    dma_hs_regs->dma_hs_ch_config[offset] |= source << pos;
}

hal_dma_master_select_t dma_port_get_master_select(dma_channel_t ch, uint32_t addr)
{
    /* APP core mdma/smdma has ms0 and ms1. */
    if (((addr >= MEM_X2P_MEMORY_START) && (addr <= MEM_X2P_MEMORY_END)) ||
        ((addr >= L2RAM_MEMORY_START) && (addr <= L2RAM_MEMORY_END)) ||
        ((addr >= QSPI_XIP_MEMORY_START) && (addr <= QSPI_XIP_MEMORY_END))) {
        if (ch > DMA_CHANNEL_7) {
            return HAL_DMA_MASTER_SELECT_ERROR;
        } else {
            return HAL_DMA_MASTER_SELECT_0;
        }
    } else {
        if (ch > DMA_CHANNEL_7) {
            return HAL_DMA_MASTER_SELECT_0;
        } else {
            return HAL_DMA_MASTER_SELECT_1;
        }
    }
}

void dma_port_add_sleep_veto(void)
{
    return;
}

void dma_port_remove_sleep_veto(void)
{
    return;
}

