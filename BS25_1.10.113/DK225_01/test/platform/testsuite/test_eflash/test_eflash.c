/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Test flash source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-01, Create file. \n
 */
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "securec.h"
#include "debug/osal_debug.h"
#include "interrupt/osal_interrupt.h"
#include "common_def.h"
#include "memory_config.h"
#include "errcode.h"
#include "eflash.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "test_eflash.h"

#define TEST_READ_LEN      10
#define PARAM_2    2
#define PARAM_3    3
#define PARAM_4    4
#define writel(addr, d)      (*(volatile uint32_t*)(uintptr_t)(addr) = (uint32_t)(d))
#define readl(addr)          (*(volatile uint32_t*)(uintptr_t)(addr))

uint32_t g_test_eflash_write_data[20];
uint32_t g_test_eflash_read_data[20];

static int32_t test_eflash_read_data(int argc, char *argv[])
{
    uint32_t addr;
    uint32_t offset;
    uint32_t len;
    uint32_t dst[TEST_READ_LEN] = { 0 };

    if (argc == PARAM_3) {
        uint32_t param_index = 0;
        addr = (uint32_t)strtol(argv[param_index++], NULL, 0);
        offset = (uint32_t)strtol(argv[param_index++], NULL, 0);
        len = (uint32_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (offset + len > sizeof(dst)) {
        osal_printk("read length out of range\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint8_t *p = (uint8_t *)(((uint32_t)&dst) + offset);
    uint32_t irq_sts = osal_irq_lock();
    uapi_eflash_read(addr, (uint32_t *)p, len);
    for (uint32_t i = 0; i < len; i++) {
        osal_printk("%02d = 0x%x\r\n", i, p[i]);
    }
    osal_irq_restore(irq_sts);
    return TEST_SUITE_OK;
}

static int32_t  test_eflash_write_data(int argc, char *argv[])
{
    uint32_t addr;
    uint32_t offset;
    uint32_t len;

    if (argc == PARAM_3) {
        uint32_t param_index = 0;
        addr = (uint32_t)strtol(argv[param_index++], NULL, 0);
        offset = (uint32_t)strtol(argv[param_index++], NULL, 0);
        len = (uint32_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (offset + len > sizeof(g_test_eflash_write_data)) {
        osal_printk("write length out of range\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    osal_printk("++++++++++++++++ EFlash init success +++++++++++++++++ \r\n");
    uint32_t *value = (uint32_t *)(((uint32_t)&g_test_eflash_write_data) + offset);
    uapi_eflash_write(addr, value, len);
    return TEST_SUITE_OK;
}

static int32_t test_eflash_init(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    osal_printk("---------------------\r\n");
    uapi_eflash_init(0);
    uapi_eflash_set_freq(0, CLOCK_32M);
    uapi_eflash_init(1);
    uapi_eflash_set_freq(1, CLOCK_32M);
    osal_printk("++++++++++++++++ EFlash init success +++++++++++++++++ \r\n");
    return TEST_SUITE_OK;
}

static int32_t test_eflash_deinit(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_eflash_deinit(0);
    uapi_eflash_deinit(1);
    osal_printk("++++++++++++++++ EFlash deinit success +++++++++++++++++ \r\n");
    return TEST_SUITE_OK;
}


static int32_t test_eflash_erase(int argc, char *argv[])
{
    eflash_region_t  bus;
    if (argc == 1) {
        uint32_t param_index = 0;
        bus = (eflash_region_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    osal_printk("++++++++++++++++start erase+++++++++++++++++ \r\n");
    uint32_t irq_sts = osal_irq_lock();
    uapi_eflash_chip_erase(bus);
    osal_irq_restore(irq_sts);
    return TEST_SUITE_OK;
}

static int32_t test_eflash_sector_erase(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    uint32_t  addr, len;

    if (argc == PARAM_2) {
        uint32_t param_index = 0;
        addr = (uint32_t)strtol(argv[param_index++], NULL, 0);
        len = (uint32_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    osal_printk("sector erase: addr:%x, len:%x \r\n", addr, len);
    uint32_t irq_sts = osal_irq_lock();
    uapi_eflash_erase(addr, len);
    osal_irq_restore(irq_sts);
    return TEST_SUITE_OK;
}

static void init_test_eflash_write_data(void)
{
    uint8_t *p = (uint8_t *)&g_test_eflash_write_data;
    for (uint8_t i = 0; i < sizeof(g_test_eflash_write_data); i++) {
        *p = i;
        p++;
    }
}

void add_eflash_test_case(void)
{
    init_test_eflash_write_data();
    uapi_test_suite_add_function("eflash_read_data", "params: <addr>,<offset>,<len>", test_eflash_read_data);
    uapi_test_suite_add_function("eflash_write_data", "params: <addr>,<offset>,<len>", test_eflash_write_data);
    uapi_test_suite_add_function("eflash_init", "params: no param", test_eflash_init);
    uapi_test_suite_add_function("eflash_deinit", "params: no param", test_eflash_deinit);
    uapi_test_suite_add_function("eflash_erase", "params: <eflash_id>", test_eflash_erase);
    uapi_test_suite_add_function("eflash_sector_erase", "params: <addr>,<len>", test_eflash_sector_erase);
}