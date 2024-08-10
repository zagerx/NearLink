/**
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved. \n
 *
 * Description: Application core main function for pm demo \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-28, Create file. \n
 */

#include "app_init.h"
#include "hal_reboot.h"
#include "pinctrl.h"
#include "gpio.h"
#include "preserve.h"
#include "cmsis_os2.h"
#include "watchdog.h"
#include "soc_osal.h"
#include "pm_ble_server.h"
#if defined(CONFIG_PM_SYS_SUPPORT)
#include "pm_sys.h"
#endif

#define TASK_COMMON_APP_DELAY_MS  5000

#if defined(CONFIG_PM_SYS_SUPPORT)
#define DURATION_MS_OF_WORK2STANDBY    30000
#define DURATION_MS_OF_STANDBY2SLEEP   60000

#define PM_STATE_WORK      0
#define PM_STATE_STANDBY   1
#define PM_STATE_SLEEP     2

#define PM_SAMPLE_GPIO_NUM  S_MGPIO18

static uint8_t g_pm_state = PM_STATE_WORK;

static void pm_gpio_irq_func(pin_t pin)
{
    if (g_pm_state == PM_STATE_WORK) {
        osal_printk("[pm_sys]gpio%d irq.\n", pin);
        uapi_gpio_clear_interrupt(pin);
        // 重置工作状态，睡眠定时器重新计时。
        uapi_pm_work_state_reset();
    } else {
        osal_printk("[pm_sys]The system wake up by gpio%d.\n", pin);
        uapi_gpio_clear_interrupt(pin);
        // 唤醒处理，后续将切换到工作状态。
        uapi_pm_wkup_process(0);
    }
}

static void pm_gpio_int_enable(void)
{
    // 睡眠时GPIO不下电，支持GPIO中断唤醒
    uapi_pin_set_mode(PM_SAMPLE_GPIO_NUM, 0);
    uapi_pin_set_pull(PM_SAMPLE_GPIO_NUM, PIN_PULL_UP);
    uapi_gpio_set_dir(PM_SAMPLE_GPIO_NUM, GPIO_DIRECTION_INPUT);
    uapi_gpio_register_isr_func(PM_SAMPLE_GPIO_NUM, GPIO_INTERRUPT_FALLING_EDGE, (gpio_callback_t)pm_gpio_irq_func);
}

static int32_t pm_state_work_to_standby(uintptr_t arg)
{
    unused(arg);
    uint32_t irq_status = osal_irq_lock();
    // 连接态：增大连接间隔；广播态：暂不操作。
    pm_ble_work2standby();
    g_pm_state = PM_STATE_STANDBY;
    osal_irq_restore(irq_status);
    return 0;
}

static int32_t pm_state_standby_to_sleep(uintptr_t arg)
{
    unused(arg);
    uint32_t irq_status = osal_irq_lock();
    // 断开连接、关闭广播
    pm_ble_standby2sleep();
    g_pm_state = PM_STATE_SLEEP;
    osal_irq_restore(irq_status);
    return 0;
}

static int32_t pm_state_standby_to_work(uintptr_t arg)
{
    unused(arg);
    uint32_t irq_status = osal_irq_lock();
    // 连接态：减小连接间隔；广播态：不操作。
    pm_ble_standby2work();
    g_pm_state = PM_STATE_WORK;
    osal_irq_restore(irq_status);
    return 0;
}

static int32_t pm_state_sleep_to_work(uintptr_t arg)
{
    unused(arg);
    uint32_t irq_status = osal_irq_lock();
    // 打开广播、蓝牙回连
    pm_ble_sleep2work();
    g_pm_state = PM_STATE_WORK;
    osal_irq_restore(irq_status);
    return 0;
}

void pm_low_power_entry(void)
{
    pm_state_trans_handler_t handler = {
        .work_to_standby = pm_state_work_to_standby,
        .standby_to_sleep = pm_state_standby_to_sleep,
        .standby_to_work = pm_state_standby_to_work,
        .sleep_to_work = pm_state_sleep_to_work,
    };
    uapi_pm_state_trans_handler_register(&handler);

    uapi_pm_work_state_reset();
    uapi_pm_set_state_trans_duration(DURATION_MS_OF_WORK2STANDBY, DURATION_MS_OF_STANDBY2SLEEP);
}

app_run(pm_low_power_entry);
#endif

void app_main(void *unused)
{
    UNUSED(unused);
    hal_reboot_clear_history();
    system_boot_reason_print();
    system_boot_reason_process();

    pm_gpio_int_enable();
    osal_msleep(1000);  // Delay 1000ms before ble_init.
    pm_ble_server_init();

    app_tasks_init();

    while (1) {  //lint !e716 Main Loop
        (void)osal_msleep(TASK_COMMON_APP_DELAY_MS);
        uapi_watchdog_kick();

        osal_printk("app_main!\n");
    }
}
