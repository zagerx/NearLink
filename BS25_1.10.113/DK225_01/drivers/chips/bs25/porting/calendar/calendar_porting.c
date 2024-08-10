/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides calendar port template. \n
 * The Systick module needs to be used for simulation, calendar port file is stored in this path.
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "hal_calendar.h"
#include "systick.h"
#include "calendar_porting.h"

#define CALENDAR_BASE_ADDR 0x00000000

/**
 * @brief  Definition of timestamp mock structure.
 */
typedef struct {
    uint64_t mock_timestamp;               /*!< Record input timestamp */
    uint64_t mock_counter;                 /*!< Record systick when store input timestamp */
} hal_datastamp_mock_t;

static hal_datastamp_mock_t g_hal_datastamp_count = { 0 };
uintptr_t g_calendar_base_addr = (uintptr_t)CALENDAR_BASE_ADDR;

static errcode_t hal_calendar_mock_set_timestamp(uint64_t timestamp)
{
    g_hal_datastamp_count.mock_timestamp = timestamp;
    g_hal_datastamp_count.mock_counter = uapi_systick_get_ms();
    return ERRCODE_SUCC;
}

static uint64_t hal_calendar_mock_get_timestamp(void)
{
    return g_hal_datastamp_count.mock_timestamp + uapi_systick_get_ms() - g_hal_datastamp_count.mock_counter;
}

static hal_calendar_funcs_t g_hal_calendar_mock_funcs = {
    .set_timestamp = hal_calendar_mock_set_timestamp,
    .get_timestamp = hal_calendar_mock_get_timestamp
};

static hal_calendar_funcs_t *hal_calendar_mock_funcs_get(void)
{
    return &g_hal_calendar_mock_funcs;
}

uintptr_t hal_calendar_hw_base_addr_get(void)
{
    return g_calendar_base_addr;
}

errcode_t calendar_port_register_hal_funcs(void)
{
    return hal_calendar_register_funcs(hal_calendar_mock_funcs_get());
}

errcode_t calendar_port_unregister_hal_funcs(void)
{
    return hal_calendar_unregister_funcs();
}