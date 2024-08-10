/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Application core main function for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-11-03, Create file. \n
 */

#include "securec.h"
#include "osal_debug.h"
#include "os_dfx.h"
#include "systick.h"

#define TSK_RECORD_MAX 32
#define HWI_RECORD_MAX 32
#define OS_HWI_PRE 0x5a
#define OS_HWI_POST 0xa5

typedef struct {
    uint8_t tid_r;
    uint8_t tid_n;
    uint64_t timestamp;
} tsk_record_info_t;

typedef struct {
    uint8_t irqnum;
    uint8_t in_out_flag; // 0x5a: irq enter, 0xa5: irq exits_info_t
    uint64_t timestamp;
} hwi_record_info_t;

typedef struct {
    hwi_record_info_t hwi_info[HWI_RECORD_MAX];
    tsk_record_info_t tsk_info[TSK_RECORD_MAX];
} os_info_t;

__attribute__((section("os_info")))os_info_t g_trace = {0}; // segment is in itcm
static uint8_t g_trace_index = 0;
static uint8_t g_hwi_index = 0;

void os_dfx_trace_init(void)
{
    memset_s((void*)&g_trace, sizeof(os_info_t), 0, sizeof(os_info_t));
}

void os_dfx_task_switch_trace(uint32_t tid_r, uint32_t tid_n)
{
    uint8_t cur_index = g_trace_index % TSK_RECORD_MAX;
    g_trace.tsk_info[cur_index].tid_r = tid_r;
    g_trace.tsk_info[cur_index].tid_n = tid_n;
    g_trace.tsk_info[cur_index].timestamp = uapi_systick_get_us();
    g_trace_index++;
}

void os_dfx_hwi_pre(uint32_t irq_num)
{
    if (irq_num == 0x7) {   // 7号中断（tick）不记录
        return;
    }
    uint8_t cur_index = g_hwi_index % HWI_RECORD_MAX;
    g_trace.hwi_info[cur_index].irqnum = irq_num;
    g_trace.hwi_info[cur_index].in_out_flag = OS_HWI_PRE;
    g_trace.hwi_info[cur_index].timestamp = uapi_systick_get_us();
    g_hwi_index++;
}

void os_dfx_hwi_post(uint32_t irq_num)
{
    if (irq_num == 0x7) {   // 7号中断（tick）不记录
        return;
    }
    uint8_t cur_index = g_hwi_index % HWI_RECORD_MAX;
    g_trace.hwi_info[cur_index].irqnum = irq_num;
    g_trace.hwi_info[cur_index].in_out_flag = OS_HWI_POST;
    g_trace.hwi_info[cur_index].timestamp = uapi_systick_get_us();
    g_hwi_index++;
}

void os_dfx_print_info(void)
{
    osal_printk("\n<Task id>:");
    for (int i = 0; i < TSK_RECORD_MAX; i++) {
        osal_printk(" %d", g_trace.tsk_info[(g_trace_index + i) % TSK_RECORD_MAX].tid_n);
    }
    osal_printk("\n<Interrupt num>:");
    for (int i = 0; i < HWI_RECORD_MAX; i++) {
        if (g_trace.hwi_info[(g_hwi_index + i) % HWI_RECORD_MAX].in_out_flag == OS_HWI_PRE) {
            osal_printk(" %d", g_trace.hwi_info[(g_hwi_index + i) % HWI_RECORD_MAX].irqnum);
        }
    }
    osal_printk("\n");
}