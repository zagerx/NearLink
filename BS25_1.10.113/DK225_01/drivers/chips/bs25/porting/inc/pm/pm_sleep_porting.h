/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pm sleep port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-13， Create file. \n
 */
#ifndef PM_SLEEP_PORTING_H
#define PM_SLEEP_PORTING_H

#include <stdint.h>
#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup middleware_chips_pm_sleep_port PM sleep port
 * @ingroup  middleware_chips_pm
 * @{
 */

#ifndef CONFIG_PM_LIGHT_SLEEP_THRESHOLD_MS
#define CONFIG_PM_LIGHT_SLEEP_THRESHOLD_MS 1
#endif

#ifndef CONFIG_PM_DEEP_SLEEP_THRESHOLD_MS
#define CONFIG_PM_DEEP_SLEEP_THRESHOLD_MS 20
#endif

extern VOID os_tick_timer_disable(VOID);
extern VOID os_tick_timer_enable(VOID);
extern UINT32 oal_get_sleep_ticks(VOID);
extern VOID oal_ticks_restore(UINT32 ticks);

/**
 * @if Eng
 * @brief  Wakeup rtc initialization.
 * @else
 * @brief  唤醒定时器初始化。
 * @endif
 */
void pm_wakeup_rtc_init(void);

/**
 * @if Eng
 * @brief  Wakeup rtc clocks and starts.
 * @param  [in]  time_ms The seted time to wakeup.
 * @else
 * @brief  唤醒定时器计时并启动。
 * @param  [in]  time_ms 设置的唤醒时间。
 * @endif
 */
void pm_wakeup_rtc_start(uint32_t time_ms);

/**
 * @if Eng
 * @brief  Stsrt the tickless.
 * @else
 * @brief  启动tickless。
 * @endif
 */
void pm_port_start_tickless(void);

/**
 * @if Eng
 * @brief  Stop the tickless.
 * @param  [in]  sleep_ms The sleeping time.
 * @else
 * @brief  关闭tickless。
 * @param  [in]  sleep_ms 睡眠时间。
 * @endif
 */
void pm_port_stop_tickless(uint32_t sleep_ms);

/**
 * @if Eng
 * @brief  Get sleep time.
 * @else
 * @brief  获取睡眠时间。
 * @endif
 */
uint32_t pm_port_get_sleep_ms(void);

/**
 * @if Eng
 * @brief  PM allows to sleep.
 * @param  [in]  allow Allow or reject to deep sleep.
 * @retval ERRCODE_SUCC Success.
 * @else
 * @brief  PM允许系统深睡。
 * @param  [in]  allow 允许或拒绝进入深睡。
 * @endif
 */
void pm_port_allow_deepsleep(bool allow);

/**
 * @if Eng
 * @brief  Enter wifi.
 * @else
 * @brief  进入wifi。
 * @endif
 */
void pm_port_enter_wfi(void);

/**
 * @if Eng
 * @brief  Start the timer to wakeup system.
 * @param  [in]  sleep_ms The sleeping time.
 * @else
 * @brief  开启唤醒系统的定时器
 * @param  [in]  sleep_ms 睡眠时长。
 * @endif
 */
void pm_port_start_wakeup_timer(uint32_t sleep_ms);

/**
 * @if Eng
 * @brief  Configuration the wakeup of light sleep.
 * @else
 * @brief  配置浅睡眠。
 * @endif
 */
void pm_port_lightsleep_config(void);

/**
 * @if Eng
 * @brief  Configuration the wakeup of light sleep.
 * @else
 * @brief  配置浅睡眠的唤醒。
 * @endif
 */
void pm_port_light_wakeup_config(void);

/**
 * @if Eng
 * @brief  Configuration of deep sleep.
 * @else
 * @brief  配置深睡眠。
 * @endif
 */
void pm_port_deepsleep_config(void);

/**
 * @if Eng
 * @brief  Configuration the wakeup of deep sleep.
 * @else
 * @brief  配置深睡眠的唤醒。
 * @endif
 */
void pm_port_deep_wakeup_config(void);

/**
 * @if Eng
 * @brief  Get the status of sleep event.
 * @else
 * @brief  获取睡眠事件的状态。
 * @endif
 */
uint16_t pm_port_get_sleep_event_status(void);

/**
 * @if Eng
 * @brief  Get the status of wakeup event.
 * @else
 * @brief  获取唤醒事件的状态。
 * @endif
 */
uint16_t pm_port_get_wakeup_event_status(void);

#define PM_GET_SLEEP_EVENT_STATUS   pm_port_get_sleep_event_status()
#define PM_GET_WKUP_EVENT_STATUS  pm_port_get_wakeup_event_status()

void pm_enable_uart_h0_wkup_int(bool en);

void pm_enable_uart_l0_wkup_int(bool en);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif

