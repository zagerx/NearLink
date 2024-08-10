/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test flash header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-11-11, Create file. \n
 */
#ifndef TEST_CFBB_FLASH_H
#define TEST_CFBB_FLASH_H

#include "common_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define XIP_FAIL_TRANCE_MAX     128
#define XIP_FLASH_BASE_ADDR     0x10000000
#define XIP_CMP_VALUE_DATA      0x5a5a5a5a
#define XIP_READ_ADDR_START     0x680000  /* addr of flash addr, not contain soc offset */
#define XIP_READ_MAX_SIZE       0x80000
#define XIP_BYPASS_FLASH_BASE_ADDR 0x14000000
#define THREAD_RANDOM_DELAY_MAX 60 /* ms */
#define ERASE_TEST_ADDR_START  0x100000
#define ERASE_TEST_MAX_SIZE    0x100000
#define SUS_MAX_RAND_DELAY_US   1000
#define SUS_CHECK_REGS_TIMEOUT_US 10000

/**
 * @defgroup testcase_flash flash
 * @ingroup  testcase
 * @{
 */

/**
 * @if Eng
 * @brief  Add the testcase of flash.
 * @else
 * @brief  添加flash测试用例。
 * @endif
 */
void add_flash_test_case(void);
void test_flash_write_area(uint32_t addr_start, uint32_t addr_size);
void add_flash_xip_test_case(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif