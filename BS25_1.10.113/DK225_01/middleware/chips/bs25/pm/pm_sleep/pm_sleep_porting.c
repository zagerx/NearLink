/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pm sleep port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-13， Create file. \n
 */

#include "chip_io.h"
#include "chip_core_irq.h"
#include "platform_core.h"
#include "common_def.h"
#include "los_task_pri.h"
#include "non_os.h"
#include "arch_barrier.h"
#include "debug_print.h"
#include "systick.h"
#include "rtc.h"
#include "tcxo.h"
#include "watchdog.h"
#include "pm_veto.h"
#include "pm_sleep_porting.h"

#define M_CTL_MCPU_POR_RST_PC_H_REG       (M_CTL_RB_BASE + 0xC0)
#define M_CTL_MCPU_POR_RST_PC_L_REG       (M_CTL_RB_BASE + 0xC4)

#define PMU1_CTL_LPM_MCU_ALW_TO_SLP_REG         (PMU1_CTL_RB_BASE + 0x200)

/* Ulp sleep event. */
#define ULP_AON_CTL_ULP_SLP_EVT_STS_REG         (ULP_AON_CTL_RB_ADDR + 0x1C8)
#define ULP_AON_CTL_ULP_SLP_EVT_CLR_REG         (ULP_AON_CTL_RB_ADDR + 0x1CC)
#define ULP_AON_CTL_ULP_SLP_EVT_EN_REG          (ULP_AON_CTL_RB_ADDR + 0x1D0)

/* Ulp sleep interrupt. */
#define ULP_AON_CTL_ULP_SLP_INT_STS_REG         (ULP_AON_CTL_RB_ADDR + 0x1C0)
#define ULP_AON_CTL_ULP_SLP_INT_CLR_REG         (ULP_AON_CTL_RB_ADDR + 0x1C4)
#define ULP_AON_CTL_ULP_SLP_INT_EN_REG          (ULP_AON_CTL_RB_ADDR + 0x1D4)

/* Ulp wakeup interrupt. */
#define ULP_AON_CTL_ULP_WKUP_INT_STS_REG        (ULP_AON_CTL_RB_ADDR + 0x1E0)
#define ULP_AON_CTL_ULP_WKUP_INT_CLR_REG        (ULP_AON_CTL_RB_ADDR + 0x1E4)
#define ULP_AON_CTL_ULP_WKUP_INT_EN_REG         (ULP_AON_CTL_RB_ADDR + 0x1F8)

/* Ulp wakeup event. */
#define ULP_AON_CTL_ULP_WKUP_EVT_STS_REG        (ULP_AON_CTL_RB_ADDR + 0x1E8)
#define ULP_AON_CTL_ULP_WKUP_EVT_CLR_REG        (ULP_AON_CTL_RB_ADDR + 0x1EC)
#define ULP_AON_CTL_ULP_WKUP_EVT_EN_REG         (ULP_AON_CTL_RB_ADDR + 0x1F0)

#define PM_ULP_RTC_WAKEUP                       2
#define PM_ULP_GPIO_WAKEUP                      1
#define PM_AON_WKUP_ULP_WAKEUP                  0
#define PM_ULP_WKUP_ALL_MASK                    0x7
#define PM_ULP_WKUP_MASK                        (BIT(PM_ULP_RTC_WAKEUP) | \
                                                BIT(PM_ULP_GPIO_WAKEUP) | \
                                                BIT(PM_AON_WKUP_ULP_WAKEUP))

/* Wakeup event. */
#define PMU1_CTL_LPM_MCPU_WKUP_EVT_CLR_REG      (PMU1_CTL_RB_BASE + 0x210)
#define PMU1_CTL_LPM_MCPU_WKUP_EVT_EN_REG       (PMU1_CTL_RB_BASE + 0x214)
#define PMU1_CTL_LPM_MCPU_WKUP_EVT_STS_REG      (PMU1_CTL_RB_BASE + 0x218)
/* Wakeup interrupt. */
#define PMU1_CTL_LPM_MCPU_WKUP_INT_CLR_REG      (PMU1_CTL_RB_BASE + 0x220)
#define PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG       (PMU1_CTL_RB_BASE + 0x224)
#define PMU1_CTL_LPM_MCPU_WKUP_INT_STS_REG      (PMU1_CTL_RB_BASE + 0x228)

#define PM_LPM_MCPU_CWDT_INT_WAKEUP             14
#define PM_LPM_MCPU_ULP_INT_WAKEUP              13
#define PM_LPM_MCPU_DAP_WAKEUP                  11
#define PM_LPM_MCPU_SSI_WAKEUP                  10
#define PM_LPM_MCPU_GPIO_WAKEUP                 9
#define PM_LPM_MCPU_M_RTC_WAKEUP                8
#define PM_LPM_MCPU_SPI3_INT_WAKEUP             7
#define PM_LPM_MCPU_UART_L2_RX_WAKEUP           6
#define PM_LPM_MCPU_UART_H0_RX_WAKEUP           5
#define PM_LPM_MCPU_UART_L0_RX_WAKEUP           4
#define PM_LPM_MCPU_SPI4_INT_WAKEUP             3
#define PM_LPM_MCPU_QDEC_INT_WAKEUP             2
#define PM_LPM_MCPU_B2M_IPC_INT_WAKEUP          1
#define PM_LPM_MCPU_KEYSCAN_INT_WAKEUP          0
#define PM_LPM_MCPU_WKUP_ALL_MASK               0xFFFF
#define PM_LPM_MCPU_WKUP_MASK                   (BIT(PM_LPM_MCPU_CWDT_INT_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_ULP_INT_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_DAP_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_SSI_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_GPIO_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_M_RTC_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_SPI3_INT_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_UART_L2_RX_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_UART_H0_RX_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_UART_L0_RX_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_SPI4_INT_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_QDEC_INT_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_B2M_IPC_INT_WAKEUP) | \
                                                BIT(PM_LPM_MCPU_KEYSCAN_INT_WAKEUP))

/* Sleep event. */
#define PMU1_CTL_LPM_MCPU_SLP_EVT_CLR_REG       (PMU1_CTL_RB_BASE + 0x240)
#define PMU1_CTL_LPM_MCPU_SLP_EVT_EN_REG        (PMU1_CTL_RB_BASE + 0x244)
#define PMU1_CTL_LPM_MCPU_SLP_EVT_STS_REG       (PMU1_CTL_RB_BASE + 0x248)
/* Sleep int. */
#define PMU1_CTL_LPM_MCPU_SLP_INT_CLR_REG       (PMU1_CTL_RB_BASE + 0x250)
#define PMU1_CTL_LPM_MCPU_SLP_INT_EN_REG        (PMU1_CTL_RB_BASE + 0x254)
#define PMU1_CTL_LPM_MCPU_SLP_INT_STS_REG       (PMU1_CTL_RB_BASE + 0x258)

#define PM_LPM_MCPU_SLEEPING_SLP                1
#define PM_LPM_MCPU_SLEEPDEEP_SLP               0
#define PM_LPM_MCPU_SLP_ALL_MASK                0x3
#define PM_LPM_MCPU_SLP_MASK                    (BIT(PM_LPM_MCPU_SLEEPING_SLP) | \
                                                BIT(PM_LPM_MCPU_SLEEPDEEP_SLP))

#define PMU1_CTL_USB_WKUP_EVT_SEL_REG           (PMU1_CTL_RB_BASE + 0x700)
#define PMU1_CTL_USB_WKUP_EVT_EN_REG            (PMU1_CTL_RB_BASE + 0x704)
#define PMU1_CTL_USB_WKUP_EVT_CLR_REG           (PMU1_CTL_RB_BASE + 0x708)
#define PMU1_CTL_USB_WKUP_EVT_STS_REG           (PMU1_CTL_RB_BASE + 0x70C)

#define PMU1_CTL_USB_WKUP_INT_EN_REG            (PMU1_CTL_RB_BASE + 0x710)
#define PMU1_CTL_USB_WKUP_INT_CLR_REG           (PMU1_CTL_RB_BASE + 0x714)
#define PMU1_CTL_USB_WKUP_INT_STS_REG           (PMU1_CTL_RB_BASE + 0x718)

#define GLB_CTL_M_GLB_GP_M_REG2_CFG_REG         (GLB_CTL_M_RB_BASE + 0x18)

#define PM_OS_TICKS_PER_S   LOSCFG_BASE_CORE_TICK_PER_SECOND
#define PM_OS_TICKS_PER_MS  (PM_OS_TICKS_PER_S / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#define PM_NO_SLEEP_TIME_AFTER_UART_WKUP        5000

#define PM_MCLKEN0_CFG_REG              0x52000040
#define PM_MCLKEN1_CFG_REG              0x52000044
#define PM_MCLKEN0_VAL_WHEN_SLEEP       0x2040
#define PM_MCLKEN1_VAL_WHEN_SLEEP       0x2000
#define PM_GLB_CLKEN_CFG_REG            0x52000548
#if defined(USE_EMBED_FLASH)
#define PM_GLB_CLKEN_VAL_WHEN_SLEEP     0x60
#else
#define PM_GLB_CLKEN_VAL_WHEN_SLEEP     0xE0
#endif
#define PM_AON_BUS_CFG_REG              0x570000A8
#define PM_AON_BUS_VAL_WHEN_SLEEP       0x184
#define PM_MCU_CORE_CH0_CFG_REG         0x5200054C
#define PM_MCU_CORE_CH0_VAL_WHEN_SLEEP  0x13

#ifdef USE_EMBED_FLASH
#define PM_UART_H0_TX_PIN_MODE_REG      0x52008098
#else
#define PM_UART_H0_TX_PIN_MODE_REG      0x5200805C
#endif
#define PM_UART_L0_TX_PIN_MODE_REG      0x520080A4

#define PM_MSYS_SLP_HISTORY_REG         0x57004260
#define PM_SYS_SLP_HISTORY_REG          0x57004264

#define PM_XO_CORE_LOADCAP_REG          0x5700819C

#define PM_EARLY_WKUP_TIME_TO_KICK_WDT  1000

static uint16_t g_mclken0_record = 0;
static uint16_t g_mclken1_record = 0;
static uint16_t g_glb_clken_record = 0;
static uint16_t g_aon_bus_record = 0;
static uint16_t g_mcu_core_ch0_record = 0;
static uint16_t g_xo_core_loadcap_record = 0;
#if !defined(CONFIG_KEYSCAN_SUPPORT_SLEEP)
static uint16_t  g_uart_h0_tx_pin_mode = 0;
#endif
static uint16_t  g_uart_l0_tx_pin_mode = 0;

static uint64_t g_entry_sleep_time = 0;
static uint64_t g_exit_sleep_time = 0;
static uint64_t g_32k_time_calibrate = 0;
static rtc_handle_t g_pm_rtc = NULL;

static void pm_rtc1_irq(uintptr_t data)
{
    unused(data);
}

void pm_wakeup_rtc_init(void)
{
    uapi_rtc_init();
    uapi_rtc_adapter(RTC_1, RTC_1_IRQN, g_aucIntPri[RTC_1_IRQN]);
    uapi_rtc_create(RTC_1, &g_pm_rtc);
}

void pm_wakeup_rtc_start(uint32_t time_ms)
{
    if (g_pm_rtc == NULL) {
        uapi_rtc_create(RTC_1, &g_pm_rtc);
    } else {
        uapi_rtc_start(g_pm_rtc, time_ms, pm_rtc1_irq, 1);
    }
}

void pm_port_start_tickless(void)
{
    g_entry_sleep_time = uapi_systick_get_count();
    os_tick_timer_disable();
}

static void pm_port_tickless_compensation(uint32_t ticks)
{
    g_exit_sleep_time = uapi_systick_get_count();
    uint64_t sleep_time = ((g_exit_sleep_time - g_entry_sleep_time) * PM_OS_TICKS_PER_S) + g_32k_time_calibrate;
    uint32_t actual_sleep_time = (uint32_t)(sleep_time >> 0xF); /* Divide 32768. */
    g_32k_time_calibrate = actual_sleep_time & 0x7FFF; /* Store the divisible part for the next calibration. */

    if (actual_sleep_time > ticks) {
        oal_ticks_restore(ticks);
    } else {
        oal_ticks_restore(actual_sleep_time);
    }

#if defined(PM_SLEEP_DEBUG_ENABLE) && (PM_SLEEP_DEBUG_ENABLE == YES)
    PRINT("Wkup event is 0x%x\r\n", readw(PMU1_CTL_LPM_MCPU_WKUP_EVT_STS_REG));
    PRINT("The mcu sleep history is: %d, The whole system sleep history is: %d, The mcu enter wfi time is %d ms\r\n", \
        readw(PM_MSYS_SLP_HISTORY_REG), readw(PM_SYS_SLP_HISTORY_REG), actual_sleep_time);
    writew(PM_MSYS_SLP_HISTORY_REG, 0x2); // Clear sleep history.
    writew(PM_SYS_SLP_HISTORY_REG, 0x2);  // Clear sleep history.

    pm_veto_t *veto_info = uapi_pm_veto_get_info();
    uint8_t count;
    for (int i = 0; i < PM_VETO_ID_MAX; i++) {
        count = veto_info->veto_counts.sub_counts[i];
        if (count != 0) {
            PRINT("PM veto id is: %d, count is: %d\r\n", i, count);
        }
    }
#endif
}

void pm_port_stop_tickless(uint32_t sleep_ms)
{
    uint32_t ticks = 0;
    /* Avoid overcompensation. */
    if (PM_OS_TICKS_PER_MS == 1) {
        ticks = (sleep_ms - 1) * PM_OS_TICKS_PER_MS;
    } else {
        ticks = (sleep_ms - (PM_OS_TICKS_PER_MS >> 1)) * PM_OS_TICKS_PER_MS;
    }
    os_tick_timer_enable();
    pm_port_tickless_compensation(ticks);
}

uint32_t pm_port_get_sleep_ms(void)
{
    uapi_watchdog_kick();
    uint32_t sleep_ms = oal_get_sleep_ticks();
    uint32_t wdt_timeout;
    if (uapi_watchdog_get_left_time(&wdt_timeout) == ERRCODE_SUCC) {
        if (sleep_ms + PM_EARLY_WKUP_TIME_TO_KICK_WDT >= wdt_timeout) {
            sleep_ms = wdt_timeout - PM_EARLY_WKUP_TIME_TO_KICK_WDT; // 提前唤醒去喂狗
        }
    }

    return sleep_ms;
}

void pm_port_allow_deepsleep(bool allow)
{
    if (!allow) {
        reg16_clrbit(0x5702E044, 0x4);   // 手动退出eco
        writew(0x5702C408, 0xB);
        writew(0x5702C408, 0x200B);
    }
    writew(PMU1_CTL_LPM_MCU_ALW_TO_SLP_REG, (uint16_t)allow);
    if (allow && readw(0x57000014) == 0x5a) {   // B核当前处于睡眠状态
        writew(0x5702C408, 0xB);
        writew(0x5702C408, 0x400B);
        reg16_setbit(0x5702E044, 0x4); // 手动进入eco
    }
}

void pm_port_enter_wfi(void)
{
    dsb();
    wfi();
    isb();
    nop();
    nop();
    nop();
    nop();
}

void pm_port_start_wakeup_timer(uint32_t sleep_ms)
{
    pm_wakeup_rtc_start(sleep_ms);
}

void pm_port_lightsleep_config(void)
{
    return;
}

void pm_port_light_wakeup_config(void)
{
    return;
}

static void pm_port_handle_before_enter_deepsleep(void)
{
    /* Aon sleep event. */
    writew(PMU1_CTL_LPM_MCPU_SLP_EVT_EN_REG, 0);                            // Aon sleep event disable.
    writew(PMU1_CTL_LPM_MCPU_SLP_EVT_CLR_REG, PM_LPM_MCPU_SLP_ALL_MASK);    // Aon sleep event clear status.
    writew(PMU1_CTL_LPM_MCPU_SLP_EVT_EN_REG, PM_LPM_MCPU_SLP_MASK);         // Aon sleep event enable.
    /* Aon wakeup event. */
    writew(PMU1_CTL_LPM_MCPU_WKUP_EVT_EN_REG, 0);                           // Aon wakeup event disable.
    writew(PMU1_CTL_LPM_MCPU_WKUP_EVT_CLR_REG, PM_LPM_MCPU_WKUP_MASK);      // Aon wakeup event clear status.
    writew(PMU1_CTL_LPM_MCPU_WKUP_EVT_EN_REG, PM_LPM_MCPU_WKUP_MASK);       // Aon wakeup event enable.

#if defined(CONFIG_PM_ENABLE_WAKEUP_INTERRUPT)
    /* Aon wakeup interrupt. */
    writew(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, 0);                           // Aon wakeup intterrupt disable.
    writew(PMU1_CTL_LPM_MCPU_WKUP_INT_CLR_REG, PM_LPM_MCPU_WKUP_MASK);      // Aon wakeup intterrupt clear status.
    writew(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_WKUP_MASK);       // Aon wakeup intterrupt enable.
#endif

#if defined(PM_UART_H0_RX_WKUP_ENABLE)
    /* Uart h0 interrupt. */
    reg16_clrbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_H0_RX_WAKEUP);
    reg16_setbit(PMU1_CTL_LPM_MCPU_WKUP_INT_CLR_REG, PM_LPM_MCPU_UART_H0_RX_WAKEUP);
    reg16_setbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_H0_RX_WAKEUP);
#endif

#if defined(PM_UART_L0_RX_WKUP_ENABLE)
    /* Uart l0 interrupt. */
    reg16_clrbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_L0_RX_WAKEUP);
    reg16_setbit(PMU1_CTL_LPM_MCPU_WKUP_INT_CLR_REG, PM_LPM_MCPU_UART_L0_RX_WAKEUP);
    reg16_setbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_L0_RX_WAKEUP);
#endif

    /* Ulp sleep event. */
    writew(ULP_AON_CTL_ULP_SLP_EVT_EN_REG, 0);  // Ulp sleep event disable.
    writew(ULP_AON_CTL_ULP_SLP_EVT_CLR_REG, 1); // Ulp sleep event clear status.
    writew(ULP_AON_CTL_ULP_SLP_EVT_EN_REG, 1);  // Ulp sleep event enable.
    /* Ulp wakeup event. */
    writew(ULP_AON_CTL_ULP_WKUP_EVT_EN_REG, 0);                     // Ulp wakeup event disable.
    writew(ULP_AON_CTL_ULP_WKUP_EVT_CLR_REG, PM_ULP_WKUP_ALL_MASK); // Ulp wakeup event clear status.
    writew(ULP_AON_CTL_ULP_WKUP_EVT_EN_REG, PM_ULP_WKUP_MASK);      // Ulp wakeup event enable.
}

#if defined(USE_EMBED_FLASH)
static void pm_eflash_resume(uint8_t eflash_region)
{
    uint32_t cbd0_addr;
    uint32_t confen_addr;
    uint32_t confen_data_addr;
    uint32_t read_write_en_addr;
    uint32_t erase_confen_state_addr;

    if (eflash_region == 0) {
        cbd0_addr = 0x8C3FFFC0;
        confen_addr = 0xA3006C64;
        confen_data_addr = 0xA3006C68;
        read_write_en_addr = 0xA3006C40;
        erase_confen_state_addr = 0xA3006C98;
    } else {
        cbd0_addr = 0x8C6FFFC0;
        confen_addr = 0xA3006D64;
        confen_data_addr = 0xA3006D68;
        read_write_en_addr = 0xA3006D40;
        erase_confen_state_addr = 0xA3006D98;
    }

    uint32_t addr = cbd0_addr;
    for (uint8_t i = 0; i < 0x8; i++) {
        writel(confen_addr, (uint32_t)(i << 1));
        writel(confen_data_addr, (readl(addr) & 0xFFFF));
        writel(read_write_en_addr, 0x1);
        reg32_setbit(confen_addr, 0);
        if (eflash_region == 1) {
            reg32_clrbit(confen_addr, 0);
        }
        while (readw(erase_confen_state_addr) == 0x4) {}
        addr += 0x8;
    }
}

static void pm_open_eflash_after_sleep(void)
{
    uint8_t mul_cnt = 0x1;
    if ((readw(0x52000550) & 0xFF) == 0x5) { mul_cnt = 0x2; }   // 64M: loop_numbers x 2

    writew(0x5702C224, 0x11);       // phyldo en
    writew(0x5702C24C, 0x1000);     // ulp_clkldo en

    for (uint32_t i = 0; i < (uint32_t)(1200 * mul_cnt); i++) { nop(); } // 1200:delay 120us

    writew(0x5702C224, 0x13);       // phyldo en_delay
    writew(0x5702C24C, 0x3000);     // ulp_clkldo en_delay

    for (uint32_t i = 0; i < (uint32_t)(10 * mul_cnt); i++) { nop(); } // 10:delay 1us

    writew(0x57004930, 0x5);    // 退出dpd

    for (uint32_t i = 0; i < (uint32_t)(100 * mul_cnt); i++) { nop(); } // 100:delay 10us

    pm_eflash_resume(0);
    pm_eflash_resume(1);
}
#endif

static void pm_close_phyldo_before_sleep(void)
{
    while (readw(0x57004014) == 0x5a) { // 软件锁：对端正在操作
        dsb();
        nop();
        nop();
        nop();
        nop();
    }

    writew(0x57004010, 0x5a); // 添加软件锁
    dsb();
    for (int i = 0; i < 50; i++) { nop(); } // 50:delay 5us, 防止获取锁的操作撞在一起引起问题
    dsb();

    while (1) { // 等待对端 “确定未占用锁”
        if (readw(0x57004014) != 0x5a) {    // 对端确定未占用锁
            break;
        }
        dsb();
        nop();
        nop();
        nop();
        nop();
    }

    if (readw(0x57000014) != 0x5a) {    // 对端未睡眠，不需要关闭phyldo
        writew(0x57000010, 0x5a);   // 进入睡眠
        dsb();
        writew(0x57004010, 0); // 释放软件锁
        return;
    }

    /*------------------------------对端睡眠------------------------------*/
#if defined(USE_EMBED_FLASH)
    writew(0x57004930, 0xA);    // 进入dpd
    writew(0x5702C24C, 0x0);    // 关闭ulp_clkldo
    writew(0x5702C224, 0x10);   // 关闭phyldo
#endif

    writew(0x57000010, 0x5a);   // 进入睡眠
    g_xo_core_loadcap_record = readw(PM_XO_CORE_LOADCAP_REG);   // 保存loadcap
    writew(PM_XO_CORE_LOADCAP_REG, 0x0);    // 系统深睡时loadcap配置成0，加快起振

    dsb();
    writew(0x57004010, 0); // 释放软件锁
}

static void pm_open_phyldo_after_sleep(void)
{
    while (readw(0x57004014) == 0x5a) { // 软件锁：对端正在操作
        dsb();
        nop();
        nop();
        nop();
        nop();
    }

    writew(0x57004010, 0x5a); // 添加软件锁
    reg16_clrbit(0x5702E044, 0x4);  // 退出eco
    writew(0x5702C408, 0x200B);
    dsb();
    for (int i = 0; i < 50; i++) { nop(); } // 50:delay 5us, 防止获取锁的操作撞在一起引起问题
    dsb();
    reg16_clrbit(0x5702E044, 0x4);  // 退出eco
    writew(0x5702C408, 0x200B);
    while (1) { // 等待对端 “确定未占用锁”
        if (readw(0x57004014) != 0x5a) { break; }  // 对端确定未占用锁
        dsb();
        nop();
        nop();
        nop();
        nop();
    }

    if (readw(0x57000014) != 0x5a) {    // 对端未睡眠，不需要打开phyldo
        writew(0x57000010, 0x0);   // 退出睡眠
        dsb();
        writew(0x57004010, 0); // 释放软件锁
        return;
    }

    /*------------------------------对端睡眠------------------------------*/
    reg16_clrbit(0x5702E044, 0x4); // 退出eco
    writew(0x5702C408, 0x200B);
    writew(PM_XO_CORE_LOADCAP_REG, g_xo_core_loadcap_record); // 恢复loadcap
    writew(0x57000010, 0x0);   // 退出睡眠

#if defined(USE_EMBED_FLASH)
    pm_open_eflash_after_sleep();
#endif

    dsb();
    writew(0x57004010, 0); // 释放软件锁
}

void pm_port_deepsleep_config(void)
{
    pm_close_phyldo_before_sleep();
    pm_port_handle_before_enter_deepsleep();

    g_mclken0_record = readw(PM_MCLKEN0_CFG_REG);
    writew(PM_MCLKEN0_CFG_REG, PM_MCLKEN0_VAL_WHEN_SLEEP);
    g_mclken1_record = readw(PM_MCLKEN1_CFG_REG);
    writew(PM_MCLKEN1_CFG_REG, PM_MCLKEN1_VAL_WHEN_SLEEP);
    g_glb_clken_record = readw(PM_GLB_CLKEN_CFG_REG);
    writew(PM_GLB_CLKEN_CFG_REG, PM_GLB_CLKEN_VAL_WHEN_SLEEP);
    g_mcu_core_ch0_record = readw(PM_MCU_CORE_CH0_CFG_REG);
    writew(PM_MCU_CORE_CH0_CFG_REG, PM_MCU_CORE_CH0_VAL_WHEN_SLEEP);
    g_aon_bus_record = readw(PM_AON_BUS_CFG_REG);
    writew(PM_AON_BUS_CFG_REG, PM_AON_BUS_VAL_WHEN_SLEEP);
#if !defined(CONFIG_KEYSCAN_SUPPORT_SLEEP)
    g_uart_h0_tx_pin_mode = readw(PM_UART_H0_TX_PIN_MODE_REG);
#endif
    g_uart_l0_tx_pin_mode = readw(PM_UART_L0_TX_PIN_MODE_REG);
#if !defined(CONFIG_KEYSCAN_SUPPORT_SLEEP)
    writew(PM_UART_H0_TX_PIN_MODE_REG, 0);  // uart h0 tx -> gpio
#endif
    writew(PM_UART_L0_TX_PIN_MODE_REG, 0);  // uart l0 tx -> gpio
}

void pm_port_deep_wakeup_config(void)
{
    pm_open_phyldo_after_sleep();
    uint16_t wkup_evt_sts = readw(PMU1_CTL_LPM_MCPU_WKUP_EVT_STS_REG);
#if defined(PM_UART_H0_RX_WKUP_ENABLE) && (PM_UART_H0_RX_WKUP_ENABLE == YES)
    if ((wkup_evt_sts & BIT(PM_LPM_MCPU_UART_H0_RX_WAKEUP)) != 0) {
        uapi_pm_add_sleep_veto_with_timeout(PM_VETO_UART_H0, PM_NO_SLEEP_TIME_AFTER_UART_WKUP);
    }
#endif

#if defined(PM_UART_L0_RX_WKUP_ENABLE) && (PM_UART_L0_RX_WKUP_ENABLE == YES)
    if ((wkup_evt_sts & BIT(PM_LPM_MCPU_UART_L0_RX_WAKEUP)) != 0) {
        uapi_pm_add_sleep_veto_with_timeout(PM_VETO_UART_L0, PM_NO_SLEEP_TIME_AFTER_UART_WKUP);
    }
#endif

#if defined(PM_JLINK_WKUP_ENABLE)
    if ((wkup_evt_sts & BIT(PM_LPM_MCPU_DAP_WAKEUP)) != 0) {
        uapi_pm_add_sleep_veto_with_timeout(PM_VETO_DAP, PM_NO_SLEEP_TIME_AFTER_UART_WKUP);
    }
#endif

    writew(PM_MCLKEN0_CFG_REG, g_mclken0_record);
    writew(PM_MCLKEN1_CFG_REG, g_mclken1_record);
    writew(PM_GLB_CLKEN_CFG_REG, g_glb_clken_record);
    writew(PM_MCU_CORE_CH0_CFG_REG, g_mcu_core_ch0_record);
    writew(PM_AON_BUS_CFG_REG, g_aon_bus_record);
#if !defined(CONFIG_KEYSCAN_SUPPORT_SLEEP)
    writew(PM_UART_H0_TX_PIN_MODE_REG, g_uart_h0_tx_pin_mode);
#endif
    writew(PM_UART_L0_TX_PIN_MODE_REG, g_uart_l0_tx_pin_mode);

#if defined(PM_UART_H0_RX_WKUP_ENABLE) && (PM_UART_H0_RX_WKUP_ENABLE == YES)
    reg16_clrbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_H0_RX_WAKEUP);
#endif
#if defined(PM_UART_L0_RX_WKUP_ENABLE) && (PM_UART_L0_RX_WKUP_ENABLE == YES)
    reg16_clrbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_L0_RX_WAKEUP);
#endif

    return;
}

uint16_t pm_port_get_sleep_event_status(void)
{
    return readw(PMU1_CTL_LPM_MCPU_SLP_EVT_STS_REG);
}

uint16_t pm_port_get_wakeup_event_status(void)
{
    return readw(ULP_AON_CTL_ULP_WKUP_EVT_STS_REG);
}

void pm_enable_uart_h0_wkup_int(bool en)
{
    if (en) {
        reg16_setbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_H0_RX_WAKEUP);
    } else {
        reg16_clrbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_H0_RX_WAKEUP);
    }
}

void pm_enable_uart_l0_wkup_int(bool en)
{
    if (en) {
        reg16_setbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_L0_RX_WAKEUP);
    } else {
        reg16_clrbit(PMU1_CTL_LPM_MCPU_WKUP_INT_EN_REG, PM_LPM_MCPU_UART_L0_RX_WAKEUP);
    }
}