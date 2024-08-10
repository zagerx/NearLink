/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides watchdog port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "hal_watchdog_v100.h"
#include "watchdog.h"
#include "tcxo.h"
#include "platform_core.h"
#include "cpu_utils.h"
#include "debug_print.h"
#include "non_os.h"
#include "chip_io.h"
#include "soc_osal.h"
#include "preserve.h"
#include "uart.h"
#include "watchdog_porting.h"

#define PARAM       0
#define WAIT_APPS_DUMP_DSP_DELAY_MS  15000
#define WDT_PORTING_ENABLE_DELAY     60uLL
#define DEFAULT_WDT_TIME             8
#define ULP_WDT_EN                   0x5702E18C
#define ULP_WDT_EN_BIT               0
#define ULP_WDT_CTRL                 0x5702E194
#define ULP_WDT_TIME_LEN             2
#define ULP_WDT_DEBUG_LOCK           0x5702E880
#define ULP_WDT_KICK                 0x5702E190
#define ULP_WDT_KICK_VALUE           0xC5
#define WDT_IRT_BIT                  0x2
#define ULP_WDT_RST_RECORD           0x5702E348
#define ULP_WDT_BIT                  0x5

uintptr_t g_watchdog_base_addr = (uintptr_t)CHIP_WDT_BASE_ADDRESS;
hal_chip_watchdog_dump_callback_t g_hal_dsp_chip_watchdog_callback = NULL;
static int32_t g_watchdog_config = DEFAULT_WDT_TIME ;
static bool g_ulp_wdt_inited = false;

void watchdog_port_register_hal_funcs(void)
{
    hal_watchdog_register_funcs(hal_watchdog_v100_funcs_get());
}

void watchdog_port_unregister_hal_funcs(void)
{
    hal_watchdog_unregister_funcs();
}

void watchdog_port_register_irq(void)
{
    return;
}

void irq_wdt_handler(void)
{
    hal_watchdog_irq_handler((uintptr_t)PARAM);
}

#ifdef WDT_INVOKE_USER_CALLBACK
errcode_t watchdog_port_check_nmi_intr(uint16_t status)
{
    if ((status & WDT_IRT_BIT) == WDT_IRT_BIT) {
        return ERRCODE_SUCC;
    }
    return ERRCODE_FAIL;
}
#endif

void watchdog_func_adapt(uint32_t time)
{
    non_os_nmi_config(NMI_CWDT, true);
    watchdog_set_configtime(time);
}

void watchdog_turnon_clk(void)
{
    reg_setbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
    uapi_tcxo_delay_us(WDT_PORTING_ENABLE_DELAY);
}

void watchdog_turnoff_clk(void)
{
    reg_clrbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
}

void watchdog_set_configtime(uint32_t time)
{
    g_watchdog_config = time;
}

uint32_t watchdog_get_configtime(void)
{
    return g_watchdog_config;
}

errcode_t ulp_wdt_init(ulp_wdt_timeout_t timeout)
{
    if (timeout > ULP_WDT_TIMEOUT_MAX) {
        return ERRCODE_FAIL;
    }

    uint32_t irq_sts = osal_irq_lock();
    if (unlikely(g_ulp_wdt_inited == true)) {
        osal_irq_restore(irq_sts);
        return ERRCODE_SUCC;
    }
    reg8_clrbit(ULP_WDT_EN, ULP_WDT_EN_BIT);
    writeb(ULP_WDT_DEBUG_LOCK, 0x0);
    reg8_setbits(ULP_WDT_CTRL, 0, ULP_WDT_TIME_LEN, (uint8_t)timeout);
    reg8_setbit(ULP_WDT_EN, ULP_WDT_EN_BIT);
    g_ulp_wdt_inited = true;
    osal_irq_restore(irq_sts);
    return ERRCODE_SUCC;
}

errcode_t ulp_wdt_deinit(void)
{
    uint32_t irq_sts = osal_irq_lock();
    if (unlikely(g_ulp_wdt_inited == false)) {
        osal_irq_restore(irq_sts);
        return ERRCODE_SUCC;
    }
    reg8_clrbit(ULP_WDT_EN, ULP_WDT_EN_BIT);

    g_ulp_wdt_inited = false;
    osal_irq_restore(irq_sts);
    return ERRCODE_SUCC;
}

errcode_t ulp_wdt_kick(void)
{
    if (unlikely(g_ulp_wdt_inited == false)) {
        return ERRCODE_FAIL;
    }

    unsigned int irq_sts = osal_irq_lock();
    writeb(ULP_WDT_KICK, ULP_WDT_KICK_VALUE);
    osal_irq_restore(irq_sts);

    return ERRCODE_SUCC;
}

void watchdog_porting_pmu_reboot(void)
{
    uint16_t i = 0;
    uint16_t stat = 0;
    if (reg8_getbit(ULP_WDT_RST_RECORD, ULP_WDT_BIT) == true) {
        set_system_boot_status(REBOOT_ULP_WDGTIMEOUT_TRIGER_STATUS);
        set_cpu_utils_reset_cause(REBOOT_CAUSE_APPLICATION_STD_ULP_WDT_FRST);
        set_update_reset_cause_on_boot(false);
    }

    do {
        reg16_setbit(ULP_WDT_RST_RECORD, ULP_WDT_BIT);
        stat = reg16_getbit(ULP_WDT_RST_RECORD, ULP_WDT_BIT);
        i++;
    } while ((stat == 1) && (i < UINT16_MAX));
    if (stat == 1) {
        PRINT("ULP_WDT_RST clear fail = %x\r\n", readl(ULP_WDT_RST_RECORD));
    }
}