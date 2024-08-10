/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: dfx adapt layer
 * This file should be changed only infrequently and with great care.
 */
#ifndef DFX_ADAPT_LAYER_H
#define DFX_ADAPT_LAYER_H

#include <stdbool.h>
#include "securec.h"
#include "dfx_feature_config.h"
#include "dfx_resource_id.h"
#include "soc_osal.h"
#include "soc_log.h"
#include "cmsis_os2.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#else
#include "flash.h"
#endif
#include "diag_adapt_flash.h"
#include "diag_debug.h"
#include "dfx_system_init.h"
#include "watchdog.h"
#include "tcxo.h"

#define PASSED_SECONDS_SINCE_1970           1663739960
#define LOGFILE_SAVE_TASK_SIZE              0x600
#define LOG_MIN_WATCHDOG_LEFT_TIME          2000
#define TCXO_1000MS                         1000

typedef enum {
    FLASH_OP_TYPE_OTA,
    FLASH_OP_TYPE_LOG_FILE,
    FLASH_OP_TYPE_FLASH_DATA,
    FLASH_OP_TYPE_USER,
    FLASH_OP_TYPE_MAX,
} dfx_flash_op_type_t;

static inline uint32_t dfx_int_lock(void)
{
    return osal_irq_lock();
}

static inline void dfx_int_restore(uint32_t state)
{
    osal_irq_restore(state);
}

static inline void dfx_sleep(uint32_t ms)
{
    osDelay(ms);
}

static inline void *dfx_malloc_permanent(uint32_t id, uint32_t size)
{
    unused(id);
    return osal_vmalloc(size);
}

static inline void *dfx_malloc(uint32_t id, uint32_t size)
{
    unused(id);
    return osal_vmalloc(size);
}

static inline void dfx_free(uint32_t id, void *addr)
{
    unused(id);
    osal_vfree(addr);
}

static inline uint32_t dfx_get_cur_second(void)
{
    return (uint32_t)(uapi_tcxo_get_ms() / TCXO_1000MS);
}

/* Event 接口 */
static inline errcode_t dfx_event_write(dfx_event_id_t event_id)
{
    unused(event_id);
    return ERRCODE_SUCC;
}

/* Msg 接口 */
static inline errcode_t dfx_msg_write(uint32_t msg_id, uint8_t *msg, uint16_t msg_len, bool wait)
{
    uint32_t timeout = (wait) ? OSAL_MSGQ_WAIT_FOREVER : 0;
    uint8_t msg_data[DFX_MSG_MAX_SIZE + DFX_MSG_ID_LEN];

    *(uint32_t*)msg_data = msg_id;

    if ((msg != NULL) && (memcpy_s(&msg_data[DFX_MSG_ID_LEN], DFX_MSG_MAX_SIZE, msg, msg_len) != EOK)) {
        return ERRCODE_FAIL;
    }
    return (errcode_t)osal_msg_queue_write_copy(dfx_get_osal_queue_id(), msg_data,
                                                DFX_MSG_MAX_SIZE + DFX_MSG_ID_LEN, timeout);
}

/*
 * 文件传输消息发送接口
 * 如文件传输未单独创建线程，可使用dfx_msg_write适配
 * 如文件传输单独创建线程，使用g_dfx_transmit_queue_id发送消息
 */
static inline errcode_t transmit_msg_write(uint32_t msg_id, uint8_t *msg, uint16_t msg_len, bool wait)
{
    return dfx_msg_write(msg_id, msg, msg_len, wait);
}

static inline int32_t dfx_msg_queue_is_full(unsigned long queue_id)
{
    return osal_msg_queue_is_full(queue_id);
}

/* TIMER接口 */
typedef osal_timer dfx_timer;
typedef void (*dfx_timer_handler)(uintptr_t data);
typedef void (*temp_osal_handler)(unsigned long data);
static inline errcode_t dfx_timer_init(dfx_timer *timer, dfx_timer_handler handler, uintptr_t data, uint32_t ms)
{
    timer->handler = (temp_osal_handler)handler;
    timer->data = data;
    timer->interval = ms;
    return (errcode_t)osal_timer_init(timer);
}

static inline errcode_t dfx_timer_start(dfx_timer *timer, uint32_t ms)
{
    unused(ms);
    return osal_timer_start(timer);
}

static inline errcode_t dfx_timer_stop(dfx_timer *timer)
{
    return (errcode_t)osal_timer_stop(timer);
}

static inline errcode_t dfx_timer_destroy(dfx_timer *timer)
{
    return (errcode_t)osal_timer_destroy(timer);
}

/* machine 接口 */
static inline uint32_t dfx_machine_get_id(void)
{
    return 0;
}

static inline char *dfx_machine_get_name(void)
{
    return "unknown";
}

/* fault 处理 */
static inline void dfx_fault_event_data(uint32_t event_id, uint8_t *data, uint16_t len)
{
    unused(event_id);
    unused(data);
    unused(len);
}

/* flash 读写 */
static inline int32_t dfx_flash_read(uint8_t opt_type, uint32_t offset, uint8_t *buf, uint32_t size)
{
    return diag_adapt_flash_read(opt_type, offset, buf, size);
}

static inline int32_t dfx_flash_info_write(uint8_t opt_type, uint32_t offset, const uint8_t *buf, uint32_t size,
    bool do_erase)
{
    return diag_adapt_flash_info_write(opt_type, offset, buf, size, do_erase);
}

static inline int32_t dfx_flash_write(uint8_t opt_type, uint32_t offset, const uint8_t *buf, uint32_t size,
    bool do_erase)
{
    return diag_adapt_flash_write(opt_type, offset, buf, size, do_erase);
}

/*
 * flash擦除数据接口
 */
static inline errcode_t dfx_flash_erase(uint8_t opt_type, const uint32_t offset, const uint32_t size)
{
    return diag_adapt_flash_erase(opt_type, offset, size);
}

static inline void dfx_watchdog_kick(void)
{
    uint32_t wdg_left_time;
    uapi_watchdog_get_left_time(&wdg_left_time);
    if (wdg_left_time < LOG_MIN_WATCHDOG_LEFT_TIME) {
        uapi_watchdog_kick();
    }
}

static inline errcode_t dfx_pm_add_sleep_veto(void)
{
    return ERRCODE_SUCC;
}

static inline errcode_t dfx_pm_remove_sleep_veto(void)
{
    return ERRCODE_SUCC;
}

static inline errcode_t dfx_log_get_level(uint8_t core, uint8_t *level)
{
    unused(core);
    unused(level);
    return ERRCODE_SUCC;
}

static inline errcode_t dfx_log_set_level(uint8_t core, uint8_t level)
{
    unused(core);
    unused(level);
    return ERRCODE_SUCC;
}

#define dfx_log_debug(fmt...)
#define dfx_log_info(fmt...)
#define dfx_log_err(fmt...) printf(fmt)

#define dfx_assert(x) ((void)0)

#endif /* DFX_ADAPT_LAYER_H */
