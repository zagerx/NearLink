/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: usb vbus status detect(connect or disconnect) \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-24， Create file. \n
 */

#include "debug_print.h"
#include "chip_io.h"
#include "osal_interrupt.h"
#include "chip_core_irq.h"
#include "arch_port.h"
#include "usb_status.h"

static usb_status_callback_t g_usb_status_cb = NULL;

#define POWN_PMU_REG_ADDR       (0x5702e320)
#define POWN_VBUS_STATUS_ADDR   (0x5702e01c)
#define POWN_VBUS_CONNECT_BIT   (1)
#define POWN_VBUS_DISCONNECT_BIT   (2)

static void usb_status_pmu_irqn(void)
{
    if ((readl(POWN_PMU_REG_ADDR) & BIT(POWN_VBUS_CONNECT_BIT)) != 0) {
        // clear key down interrupt
        reg_setbit(POWN_PMU_REG_ADDR, 0, POWN_VBUS_CONNECT_BIT);
        if (g_usb_status_cb != NULL) {
            g_usb_status_cb(USB_CONNECTED);
        }
        return;
    }
    if ((readl(POWN_PMU_REG_ADDR) & BIT(POWN_VBUS_DISCONNECT_BIT)) != 0) {
        // clear key up interrupt
        reg_setbit(POWN_PMU_REG_ADDR, 0, POWN_VBUS_DISCONNECT_BIT);
        if (g_usb_status_cb != NULL) {
            g_usb_status_cb(USB_DISCONNECTED);
        }
        return;
    }
}

bool usb_is_vbus_connected(void)
{
    return (readl(POWN_VBUS_STATUS_ADDR) & BIT(POWN_VBUS_CONNECT_BIT)) != 0;
}

void usb_status_init(usb_status_callback_t cb)
{
    osal_irq_request(PMU_IRQ_IRQN, (osal_irq_handler)usb_status_pmu_irqn, NULL, NULL, NULL);
    osal_irq_set_priority(PMU_IRQ_IRQN,  irq_prio(PMU_IRQ_IRQN));
    osal_irq_enable(PMU_IRQ_IRQN);
    g_usb_status_cb = cb;
}