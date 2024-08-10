/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides calendar hardware instance \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-03， Create file. \n
 */
#include "systick.h"
#include "hal_calendar_hw_op.h"
#include "hal_calendar_hw.h"

#define HAL_CLC_H_OFFSET 32

static inline uint32_t git_high_32bit(uint64_t val)
{
    return (uint32_t)(val >> HAL_CLC_H_OFFSET);
}

static inline uint32_t git_low_32bit(uint64_t val)
{
    return (uint32_t)(val & 0xFFFFFFFFLL);
}

static errcode_t hal_calendar_set_timestamp(uint64_t timestamp)
{
    uint64_t counter = uapi_systick_get_ms();
    hal_calendar_set_cnt_h(git_high_32bit(counter));
    hal_calendar_set_cnt_l(git_low_32bit(counter));

    hal_calendar_set_data_h(git_high_32bit(timestamp));
    hal_calendar_set_data_l(git_low_32bit(timestamp));
    return ERRCODE_SUCC;
}

static uint64_t hal_calendar_get_timestamp(void)
{
    uint64_t data_l, data_h, timestamp, cnt_l, cnt_h, counter;

    data_l = hal_calendar_get_data_l();
    data_h = hal_calendar_get_data_h();
    cnt_l = hal_calendar_get_cnt_l();
    cnt_h = hal_calendar_get_cnt_h();
    counter = (cnt_h << HAL_CLC_H_OFFSET) | cnt_l;
    timestamp = ((data_h << HAL_CLC_H_OFFSET) | data_l) + uapi_systick_get_ms() - counter;
    return timestamp;
}

hal_calendar_funcs_t g_hal_calendar_hw_funcs = {
    .set_timestamp = hal_calendar_set_timestamp,
    .get_timestamp = hal_calendar_get_timestamp
};

hal_calendar_funcs_t *hal_calendar_hw_funcs_get(void)
{
    return &g_hal_calendar_hw_funcs;
}