/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test flash source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-11-11, Create file. \n
 */
#include "securec.h"
#include "debug/osal_debug.h"
#include "interrupt/osal_interrupt.h"
#include "common_def.h"
#include "memory_config.h"
#include "flash.h"
#include "hal_spi.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "test_suite_log.h"
#include "hal_xip.h"
#include "test_flash.h"

#define TEST_PARAM_4_BYTES 4
#define TEST_READ_LEN  10

#define READ_MODE 0
#define WRITE_MODE 1
#define ERASE_MODE 2

#define TEST_LEFT_SHIFT_8   8
#define TEST_LEFT_SHIFT_16  16
#define TEST_LEFT_SHIFT_24  24

#if defined(CONFIG_SPI_USING_V151) && (CONFIG_SPI_USING_V151 == 1)
#define ERASE_LEN   0x100
#else
#define ERASE_LEN   0x4000
#endif
#define ERASE_32K   0x8000

#define TEST_FLASH_LENGTH    0x100000
#if defined(CONFIG_SPI_USING_V151) && (CONFIG_SPI_USING_V151 == 1)
#define TEST_S_ADDR          0x0
#define TEST_E_ADDR          0x1000
#else
#define TEST_S_ADDR          (APP_FLASH_REGION_START + APP_FLASH_REGION_LENGTH - FLASH_START)
#define TEST_E_ADDR          (TEST_S_ADDR + TEST_FLASH_LENGTH)
#endif

#define writel(addr, d)      (*(volatile uint32_t*)(uintptr_t)(addr) = (uint32_t)(d))
#define readl(addr)          (*(volatile uint32_t*)(uintptr_t)(addr))
#define reg(base, offset)    (*(volatile uint32_t *)(uintptr_t)((uint32_t)(base) + (offset)))

uint32_t g_testflash_src[ERASE_LEN / TEST_PARAM_4_BYTES];
uint32_t g_testflash_dst[ERASE_LEN / TEST_PARAM_4_BYTES];

#if defined(CONFIG_SPI_USING_V151) && (CONFIG_SPI_USING_V151 == 1)
#define FLASH_ID    FLASH_0
#else
#define FLASH_ID    FLASH_1
#endif

void flash_porting_pinmux_cfg(flash_id_t id) __attribute__((weak, alias("flash_porting_none")));

static void flash_porting_none(flash_id_t id)
{
    UNUSED(id);
}

#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
void flash_porting_xip_disable(void) __attribute__((weak, alias("flash_xip_none")));

static void flash_xip_none(void)
{
}

static int test_flash_xip_read(int argc, char *argv[])
{
    uint32_t test_time;
    uint32_t print_times = 0;

    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    test_time = (uint32_t)strtol(argv[0], NULL, 0);

    for (uint32_t i = 0; i < test_time; i++) {
        test_suite_log_stringf("test times: %d\r\n", i);
        for (uint32_t addr = FLASH_START + TEST_S_ADDR; addr < (FLASH_START + TEST_E_ADDR);
             addr += TEST_PARAM_4_BYTES) {
            print_times++;
            readl(addr);
            if (print_times >= 0x10000) {
                test_suite_log_stringf("check value addr:0x%x,actual:0x%x\r\n", addr - FLASH_START, readl(addr));
                print_times = 0;
            }
        }
    }
    test_suite_log_stringf("test finish...\r\n");

    return TEST_SUITE_OK;
}
#endif

static int test_flash_write_data(int argc, char *argv[])
{
    uint32_t test_addr;
    uint32_t write_value;
    if (argc >= 2) {    /* 2: Indicates the number of input parameters. */
        test_addr = (uint32_t)strtol(argv[0], NULL, 0);
        write_value = (uint32_t)strtol(argv[1], NULL, 0);
    } else {
        test_suite_log_stringf("test_flash_write_byte para num is err argc=%d.\r\n", argc);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    unsigned int irq = osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_ID);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x, and write value is 0x%x\r\n", test_addr,
                           readl(test_addr + FLASH_START), write_value);
#endif

#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_ID);
#endif
    uapi_flash_sector_erase(FLASH_ID, test_addr, true);
    uapi_flash_write_data(FLASH_ID, test_addr, (uint8_t *)&write_value, sizeof(write_value));
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_ID);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x\r\n", test_addr, readl(test_addr + FLASH_START));
#endif
    osal_irq_restore(irq);
    return TEST_SUITE_OK;
}

static int test_flash_power(int argc, char *argv[])
{
    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    bool mode = (uint32_t)strtol(argv[0], NULL, 0);
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_ID);
#endif
    if (mode) {
        uapi_flash_switch_to_deeppower(FLASH_ID);
        test_suite_log_stringf("switch to power down.\r\n");
    } else {
        uapi_flash_resume_from_deeppower(FLASH_ID);
        test_suite_log_stringf("switch to power down resume.\r\n");
    }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_ID);
#endif
    return TEST_SUITE_OK;
}

static int test_flash_read_data(int argc, char *argv[])
{
    unused(argc);
    uint32_t test_addr;
    test_addr = (uint32_t)strtol(argv[0], NULL, 0);
    uint32_t dst[TEST_READ_LEN] = { 0 };

    unsigned int irq = osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_ID);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x\r\n", test_addr,
                           readl(test_addr + FLASH_START));
#endif
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_ID);
#endif
    uapi_flash_read_data(FLASH_ID, test_addr, (uint8_t*)dst, sizeof(dst));
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_ID);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x\r\n", test_addr, readl(test_addr + FLASH_START));
#endif
    osal_irq_restore(irq);
    for (int i = 0; i < TEST_READ_LEN; i++) {
        test_suite_log_stringf("dst = 0x%x\r\n", dst[i]);
    }
    return TEST_SUITE_OK;
}

static void test_flash_print(uint32_t mode, uint32_t* src, uint32_t length)
{
    uint32_t i = 0;
    if (mode == READ_MODE) {
        test_suite_log_stringf("read value:");
    }
    if (mode == WRITE_MODE) {
        test_suite_log_stringf("write value:");
    }
    for (i = 0; i < (length / TEST_PARAM_4_BYTES); i++) {
        test_suite_log_stringf("%x ", src[i]);
    }
    test_suite_log_stringf("\r\n");
}

static void test_flash_erase(void)
{
    uint32_t irq_sts = osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_ID);
#endif
    for (uint32_t i = TEST_S_ADDR; i < TEST_E_ADDR; i += ERASE_32K) {
        uapi_flash_block_erase(FLASH_ID, i, ERASE_32K, true);
    }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_ID);
#endif
    osal_irq_restore(irq_sts);
}

static int test_flash_press(int argc, char *argv[])
{
    uint32_t mode, value;

    if (argc >= 2) { /* 2: Indicates the number of input parameters. */
        mode = (uint32_t)strtol(argv[0], NULL, 0);
        value  = (uint32_t)strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    for (uint32_t j = 0; j < ERASE_LEN / TEST_PARAM_4_BYTES; j++) {
        g_testflash_src[j] = 0xffffffff & ((value << TEST_LEFT_SHIFT_24) | (value << TEST_LEFT_SHIFT_16) |
                             (value << TEST_LEFT_SHIFT_8) | value);
        g_testflash_dst[j] = 0xffffffff;
    }

    if (mode == READ_MODE) {
        uint32_t irq_sts =  osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_exit_from_xip_mode(FLASH_ID);
#endif
        for (uint32_t i = TEST_S_ADDR; i < TEST_E_ADDR; i += ERASE_32K) {
            uapi_flash_read_data(FLASH_ID, i, (uint8_t*)g_testflash_dst, ERASE_LEN);
            uapi_flash_read_data(FLASH_ID, (i + ERASE_LEN), (uint8_t*)g_testflash_dst, ERASE_LEN);
        }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_switch_to_xip_mode(FLASH_ID);
#endif
        test_flash_print(mode, g_testflash_dst, sizeof(g_testflash_dst));
        osal_irq_restore(irq_sts);
    } else if (mode == ERASE_MODE) {
        test_flash_erase();
    } else {
        uint32_t irq_sts =  osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_exit_from_xip_mode(FLASH_ID);
#endif
        for (uint32_t i = TEST_S_ADDR; i < TEST_E_ADDR; i += ERASE_32K) {
            uapi_flash_block_erase(FLASH_ID, i, ERASE_32K, true);
            uapi_flash_write_data(FLASH_ID, i, (uint8_t*)g_testflash_src, ERASE_LEN);
            uapi_flash_write_data(FLASH_ID, (i + ERASE_LEN), (uint8_t*)g_testflash_src, ERASE_LEN);
        }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_switch_to_xip_mode(FLASH_ID);
#endif
        osal_irq_restore(irq_sts);
    }
    test_suite_log_stringf("test finish...\r\n");
    return TEST_SUITE_OK;
}

#if SOCMN1_CHIP_V100 == 1
#include "tcxo.h"
#include "watchdog.h"
#include "gpio.h"
#include "securec.h"
#include "pinctrl.h"
#include "chip_io.h"
#include "hal_gpio.h"
#include "qspi.h"
#include "non_os_reboot.h"
#include "flash_common_config.h"

#define FLASH_PWROFF_WAIT_MS   15
#define FLASH_PWRON_WAIT_MS    5
#define EN_LOAD_SWITCH3        0x2
#define PUM1_LOAD_SWITCH       0x570040BC

static void triger_flash_gpio(void)
{
    uapi_gpio_set_val(S_MGPIO17, GPIO_LEVEL_LOW);
    uapi_tcxo_delay_us(10); /* 10us */
    uapi_gpio_set_val(S_MGPIO17, GPIO_LEVEL_HIGH);
    uapi_tcxo_delay_us(10); /* 10us */
    uapi_gpio_set_val(S_MGPIO17, GPIO_LEVEL_LOW);
}

static void test_flash_gpio_triger_init(void)
{
    static uint32_t init_flag = 0;
    if (init_flag == 0) {
        uapi_pin_set_mode(S_MGPIO17, HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_MGPIO17, GPIO_DIRECTION_OUTPUT);
        uapi_gpio_set_val(S_MGPIO17, GPIO_LEVEL_LOW);
        init_flag = 1;
        PRINT("S_MGPIO17 INIT \r\n");
    }
}

void flash_set_power_on(uint8_t is_power)
{
    if (is_power) {
        reg16_setbit(PUM1_LOAD_SWITCH, EN_LOAD_SWITCH3);
        uapi_pin_set_mode(S_MGPIO0, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_MGPIO1, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_MGPIO2, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_MGPIO3, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_MGPIO4, HAL_PIO_FUNC_QSPI0);
        uapi_pin_set_mode(S_MGPIO5, HAL_PIO_FUNC_QSPI0);
        uapi_tcxo_delay_ms(FLASH_PWRON_WAIT_MS);
    } else {
        uapi_pin_set_mode(S_MGPIO0, HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_MGPIO0, GPIO_DIRECTION_INPUT);
        uapi_pin_set_pull(S_MGPIO0, HAL_PIO_PULL_NONE);
        uapi_pin_set_mode(S_MGPIO1, HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_MGPIO1, GPIO_DIRECTION_INPUT);
        uapi_pin_set_pull(S_MGPIO1, HAL_PIO_PULL_NONE);
        uapi_pin_set_mode(S_MGPIO2, HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_MGPIO2, GPIO_DIRECTION_INPUT);
        uapi_pin_set_pull(S_MGPIO2, HAL_PIO_PULL_NONE);
        uapi_pin_set_mode(S_MGPIO3, HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_MGPIO3, GPIO_DIRECTION_INPUT);
        uapi_pin_set_pull(S_MGPIO3, HAL_PIO_PULL_NONE);
        uapi_pin_set_mode(S_MGPIO4, HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_MGPIO4, GPIO_DIRECTION_INPUT);
        uapi_pin_set_pull(S_MGPIO4, HAL_PIO_PULL_NONE);
        uapi_pin_set_mode(S_MGPIO5, HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_MGPIO5, GPIO_DIRECTION_INPUT);
        uapi_pin_set_pull(S_MGPIO5, HAL_PIO_PULL_NONE);
        reg16_clrbit(PUM1_LOAD_SWITCH, EN_LOAD_SWITCH3);
        uapi_tcxo_delay_ms(FLASH_PWROFF_WAIT_MS);
    }
}

static void set_flash_qe(uint32_t flash_set_qe_index)
{
    errcode_t ret;
    uint32_t i = 0;

    do {
        i++;
        ret = uapi_flash_send_cmd_exe(0, g_flash_need_set_qe[flash_set_qe_index].flash_set_qe_cmd);
        if (i > SUS_MAX_RAND_DELAY_US) {
            break;
        }
    } while (ret != ERRCODE_SUCC);

    if (i > SUS_MAX_RAND_DELAY_US) {
        PRINT("SET QE FAIL!\r\n");
    }
    PRINT("SET QE Success\r\n");
}

static void check_enable_qe_if_need(uint32_t manufacture_id)
{
    uint32_t set_qe_num = sizeof(g_flash_need_set_qe) / sizeof(g_flash_need_set_qe[0]);
    for (uint32_t i = 0; i < set_qe_num; i++) {
        if (manufacture_id == g_flash_need_set_qe[i].manufacturer_id) {
            set_flash_qe(i);
            break;
        }
    }
}

errcode_t test_flash_reinit(void)
{
    flash_set_power_on(0);
    uapi_flash_deinit(0);
    flash_set_power_on(1);
    uapi_flash_init(0);
    uint32_t manufacture_id = 0;
    uint8_t unique_id = 0;
    errcode_t ret = uapi_flash_read_id(0, &manufacture_id);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = uapi_flash_read_unique_id(0, &unique_id);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    PRINT("flash id read ret = 0x%x, manufacture_id = 0x%x, unique_id=0x%x\r\n", ret, manufacture_id, unique_id);
    uapi_flash_config_cmd_at_xip_mode(0, unique_id);
    check_enable_qe_if_need(manufacture_id);

    ret = uapi_flash_switch_to_qspi_init(0);
    if (ret != ERRCODE_SUCC) {
        PRINT("Switch to qspi init fail\r\n");
        return ret;
    }

    ret = uapi_flash_switch_to_cache_mode(0);
    if (ret != ERRCODE_SUCC) {
        PRINT("Switch to cache mode\r\n");
        return ret;
    }

    uint32_t val = readl(XIP_FLASH_BASE_ADDR);
    if (val != 0x20002ffc) {
        PRINT("xip read val error, error val = 0x%x\r\n", val);
        return ERRCODE_FLASH_FALSE_MODE;
    }

    return ret;
}

static int test_flash_reinit_test(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint64_t t1 = uapi_tcxo_get_us();
    errcode_t ret = test_flash_reinit();
    uint64_t t2 = uapi_tcxo_get_us();
    PRINT("flash reinit:0x%x\r\n", ret);
    PRINT("flash_reinit cost:%d us \r\n", (uint32_t)(t2 - t1));
    PRINT("%x == %x \r\n", readl(0x10000000), readl(0x14000000));
    return TEST_SUITE_OK;
}

static uint8_t g_test_page_buf[0x100];
static void dump_flash_qspi_regs(void)
{
    volatile hal_qspi_regs_t *qspi_reg = (volatile hal_qspi_regs_t *)QSPI_0_BASE_ADDR;
    PRINT("qspi_ctrlr0: 0x%x \r\n", qspi_reg->qspi_ctrlr0);
    PRINT("qspi_ctrlr1: 0x%x \r\n", qspi_reg->qspi_ctrlr1);
    PRINT("qspi_ssiner: 0x%x \r\n", qspi_reg->qspi_ssiner);
    PRINT("qspi_mwcr: 0x%x \r\n", qspi_reg->qspi_mwcr);
    PRINT("qspi_ser: 0x%x \r\n", qspi_reg->qspi_ser);
    PRINT("qspi_baudr: 0x%x \r\n", qspi_reg->qspi_baudr);
    PRINT("qspi_txftlr: 0x%x \r\n", qspi_reg->qspi_txftlr);
    PRINT("qspi_rxftlr: 0x%x \r\n", qspi_reg->qspi_rxftlr);
    PRINT("qspi_txflr: 0x%x \r\n", qspi_reg->qspi_txflr);
    PRINT("qspi_rxflr: 0x%x \r\n", qspi_reg->qspi_rxflr);
    PRINT("qspi_sr: 0x%x \r\n", qspi_reg->qspi_sr);
    PRINT("qspi_imr: 0x%x \r\n", qspi_reg->qspi_imr);
    PRINT("qspi_isr: 0x%x \r\n", qspi_reg->qspi_isr);
    PRINT("qspi_risr: 0x%x \r\n", qspi_reg->qspi_risr);
    PRINT("qspi_rx_sample_dly: 0x%x \r\n", qspi_reg->qspi_rx_sample_dly);
    PRINT("qspi_spi_ctrlr0: 0x%x \r\n", qspi_reg->qspi_spi_ctrlr0);
    PRINT("qspi_txd_drive_edge: 0x%x \r\n", qspi_reg->qspi_txd_drive_edge);
}

static int test_flash_write_traverse_page(int argc, char *argv[])
{
#define WRITE_PAGE_TEST_ADDR 0x988141
    UNUSED(argc);
    UNUSED(argv);
    uint32_t len;
    uint32_t addr = WRITE_PAGE_TEST_ADDR;
    if (argc >= 0x1) {
        addr = (uint32_t)strtol(argv[0], NULL, 0);
    }
    PRINT("test_flash_write_traverse_page start addr:%x  \r\n", addr);
    for (len = 0; len < 0x100; len++) {
        g_test_page_buf[len] = len;
    }
    test_flash_gpio_triger_init();
    non_os_enter_critical();
    uapi_flash_exit_from_xip_mode(0);
    for (len = 1; len < 0x100; len++) {
        uapi_watchdog_kick();
        uint32_t send_len = uapi_flash_write_data(0, addr, g_test_page_buf, len);
        if (send_len != len) {
            PRINT("send_len[%d] != len[%d] \r\n", send_len, len);
            triger_flash_gpio();
            dump_flash_qspi_regs();
            break;
        }
        if (qspi_trans_complete_wait_timeout(FLASH_QSPI_ID, QSPI_WAIT_FIFO_DEFAULT_TIMEOUT) != QSPI_RET_OK) {
            triger_flash_gpio();
            PRINT("wait qspi tx timeout fail \r\n");
            dump_flash_qspi_regs();
            break;
        }
    }
    uapi_flash_switch_to_xip_mode(0);
    non_os_exit_critical();
    PRINT("test_flash_write_traverse_page end \r\n");
    return TEST_SUITE_OK;
}

static uint8_t g_flash_ram_cmp_test[0x1000];
void test_flash_write_area(uint32_t addr_start, uint32_t addr_size)
{
    uint32_t i;
    uint32_t addr;
    for (i = 0; i < 0x1000; i++) {
        g_flash_ram_cmp_test[i] = 0x5A;
    }
    non_os_enter_critical();
    uapi_flash_exit_from_xip_mode(0);
    uapi_watchdog_kick();
    for (addr = addr_start; addr < (addr_start + addr_size); addr += 0x1000) {
        uapi_watchdog_kick();
        uapi_flash_block_erase(0, addr, 0x1000, true);
        uapi_flash_write_data(0, addr, (uint8_t *)g_flash_ram_cmp_test, sizeof(g_flash_ram_cmp_test));
    }
    uapi_flash_switch_to_xip_mode(0);
    non_os_exit_critical();
}

static int test_flash_reboot_chip(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    PRINT("flash_reboot_chip...  \r\n");
    uapi_tcxo_delay_ms(10); /* 10ms */
    reboot_chip();
    return TEST_SUITE_OK;
}

static void test_flash_cmd_read_consumption(void)
{
    uint64_t tick_curr = uapi_tcxo_get_ms();
    uint64_t tick_pre = uapi_tcxo_get_ms();
    uint32_t read_addr = 0;
    uint32_t val;

    (void)uapi_flash_exit_from_xip_mode(0);
    while (tick_curr < (tick_pre + 1000)) { /* 1000 ms */
        read_addr = (read_addr % XIP_READ_MAX_SIZE);
        uapi_flash_read_data(0, read_addr, (uint8_t *)&val, 0x4);
        tick_curr = uapi_tcxo_get_ms();
        read_addr += 0x4;
        if (val == 0x55aaccff) {
            PRINT("val:0x%x \r\n", val);
        }
    }

    uapi_tcxo_delay_ms(2000); /* 2000 ms */
    flash_set_power_on(0);
    uapi_tcxo_delay_ms(3000); /* 3000 ms */
    (void)test_flash_reinit();
}

static void test_flash_erase_consumption(void)
{
    uint64_t tick_curr = uapi_tcxo_get_ms();
    uint64_t tick_pre = uapi_tcxo_get_ms();
    uint32_t erase_addr = 0;
    non_os_enter_critical();
    (void)uapi_flash_exit_from_xip_mode(0);
    while (tick_curr < (tick_pre + 1000)) { /* 1000 ms */
        erase_addr = (erase_addr % ERASE_TEST_MAX_SIZE) + ERASE_TEST_ADDR_START;
        (void)uapi_flash_block_erase(0, erase_addr, 0x1000, true);
        tick_curr = uapi_tcxo_get_ms();
        erase_addr += 0x1000;
    }
    uapi_tcxo_delay_ms(2000); /* 2000 ms */
    flash_set_power_on(0);
    uapi_tcxo_delay_ms(3000); /* 3000 ms */
    (void)test_flash_reinit();
    non_os_exit_critical();
}

static void test_flash_write_consumption(void)
{
    uint64_t tick_curr = uapi_tcxo_get_ms();
    uint64_t tick_pre = uapi_tcxo_get_ms();
    uint32_t write_addr = 0;
    for (uint32_t i = 0; i < 0x1000; i++) {
        g_flash_ram_cmp_test[i] = 0x5A;
    }
    non_os_enter_critical();
    (void)uapi_flash_exit_from_xip_mode(0);
    while (tick_curr < (tick_pre + 1000)) { /* 1000 ms */
        write_addr = (write_addr % ERASE_TEST_MAX_SIZE) + ERASE_TEST_ADDR_START;
        (void)uapi_flash_write_data(0, write_addr, (uint8_t *)g_flash_ram_cmp_test, sizeof(g_flash_ram_cmp_test));
        tick_curr = uapi_tcxo_get_ms();
        write_addr += 0x1000;
    }
    uapi_tcxo_delay_ms(2000); /* 2000 ms */
    flash_set_power_on(0);
    uapi_tcxo_delay_ms(3000); /* 3000 ms */
    (void)test_flash_reinit();
    non_os_exit_critical();
}

static void flash_send_xip_cmd_test(void)
{
    hal_spi_interface_ctrlr0_reg ctrl0 = { 0 };
    hal_spi_interface_spi_ctrlr0_reg spi_ctrl0 = { 0 };

    ctrl0.d.spi_frf = SPI_QUAD_SPI_FRF;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;
    ctrl0.d.dfs_32 = DFR_32_FRAME_32BITS;
    spi_ctrl0.d.wait_cycles = WAIT_CYCLES_4;
    spi_ctrl0.d.inst_l = SPI_INST_L_8_BITS;
    spi_ctrl0.d.addr_l = QSPI_ADDR_L_32;
    spi_ctrl0.d.trans_type = QSPI_TRANS_MODE2;
    hal_qspi_disable(FLASH_QSPI_ID);
    hal_qspi_ctlr0_cfg(FLASH_QSPI_ID, ctrl0.d32);
    hal_qspi_spi_ctlr0_cfg(FLASH_QSPI_ID, spi_ctrl0.d32);
    hal_qspi_ctlr1_cfg(FLASH_QSPI_ID, FLASH_DATA_ONE_BYTE);
    hal_qspi_enable(FLASH_QSPI_ID);
    // When use 24bit addr, send 0xEB
    hal_qspi_write_data(FLASH_QSPI_ID, FLASH_QRD_CMD);
    /* cmd as dummy cycle */
    hal_qspi_write_data(FLASH_QSPI_ID, 0x20);
}

static errcode_t uapi_flash_switch_to_xip_mode_test(void)
{
    uint32_t recv_data[1];
    hal_spi_interface_spi_ctrlr0_reg spi_ctrl0 = { 0 };
    if (hal_xip_is_enable(0)) {
        PRINT("xip already enable in %d(line)\r\n", __LINE__);
        return ERRCODE_SUCC;
    }
    if (hal_xip_get_cur_mode(0) == XIP_MODE_DISABLE) {
        PRINT("xip mode is disable, switch xip failed.\r\n");
        return ERRCODE_FAIL;
    }

    non_os_enter_critical();
    flash_send_xip_cmd_test();
    /* flash qspi mode set succ */
    if (qspi_recv_data_by_word(FLASH_QSPI_ID, recv_data, 1) != QSPI_RET_OK) {
        PRINT("switch to xip error\r\n");
        non_os_exit_critical();
        return ERRCODE_FAIL;
    }
    spi_ctrl0.d.wait_cycles = WAIT_CYCLES_4;
    spi_ctrl0.d.inst_l = HAL_SPI_INST_LEN_0;
    spi_ctrl0.d.addr_l = HAL_SPI_ADDR_LEN_32;
    spi_ctrl0.d.trans_type = HAL_SPI_TRANS_TYPE_INST_Q_ADDR_Q;

    hal_qspi_disable(FLASH_QSPI_ID);
    hal_qspi_spi_ctlr0_cfg(FLASH_QSPI_ID, spi_ctrl0.d32);
    hal_qspi_baud_set_clk_div(FLASH_QSPI_ID, 0x2);
    /* set dma cfg for xip data convert in hardware */
    hal_qspi_dmacr_cfg(FLASH_QSPI_ID, FLASH_QSPI_TRANSMIT_DMA_ENABLE);
    hal_qspi_dmardlr_cfg(FLASH_QSPI_ID, 0x1);
    hal_qspi_ctlr1_cfg(FLASH_QSPI_ID, FLASH_XIP_TRANS_8_BYTE);
    hal_qspi_enable(FLASH_QSPI_ID);
    /* open xip en */
    hal_xip_enable(0, 0x20, false, false, false);
    non_os_exit_critical();
    return ERRCODE_SUCC;
}

extern flash_cfg_t g_flash_config[FLASH_MAX];

static bool flash_xip_use_cmd_mode(flash_id_t id)
{
    flash_info_t flash_info;
    uapi_flash_get_info(id, &flash_info);
    uint32_t xip_use_cmd_num = (uint32_t)sizeof(g_xip_use_cmd_mode) / (uint32_t)sizeof(g_xip_use_cmd_mode[0]);
    for (uint32_t i = 0; i < xip_use_cmd_num; i++) {
        if (flash_info.flash_id == g_xip_use_cmd_mode[i]) {
            return true;
        }
    }
    if (g_flash_config[id].need_cmd_at_xip_mode) {
        return true;
    }
    return false;
}

static void test_flash_continue_read_consumption(void)
{
    uint64_t tick_curr = uapi_tcxo_get_ms();
    uint64_t tick_pre = uapi_tcxo_get_ms();
    uint32_t read_addr = 0;
    uint32_t val;

    PRINT("1val:0x%x \r\n", readl(XIP_FLASH_BASE_ADDR));
    if (flash_xip_use_cmd_mode(0)) {
        /* local flash xip use cmd mode, use reset spi config for continue read */
        (void)uapi_flash_exit_from_xip_mode(0);
        errcode_t ret = uapi_flash_switch_to_xip_mode_test();
        PRINT("reset spi config for continue read ret:0x%x \r\n", ret);
    }

    PRINT("2val:0x%x \r\n", readl(XIP_FLASH_BASE_ADDR));
    while (tick_curr < (tick_pre + 1000)) { /* 1000 ms */
        read_addr = (read_addr % XIP_READ_MAX_SIZE) + XIP_FLASH_BASE_ADDR;
        val =  readl(read_addr);
        tick_curr = uapi_tcxo_get_ms();
        read_addr += 0x20;
        if (val == 0x55aaccff) {
            PRINT("val:0x%x \r\n", val);
        }
    }
    PRINT("val3:0x%x \r\n", readl(XIP_FLASH_BASE_ADDR));
    uapi_tcxo_delay_ms(2000); /* 2000 ms */
    (void)uapi_flash_exit_from_xip_mode(0);
    flash_set_power_on(0);
    uapi_tcxo_delay_ms(3000); /* 3000 ms */
    (void)test_flash_reinit();
}

static int test_flash_power_consumption(int argc, char *argv[])
{
    uint32_t test_case;
    if (argc < 0x1) {
        PRINT("para fail, need <0:read, 1:read_c 2:erase, 3: write> ... \r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_case = (uint32_t)strtol(argv[0], NULL, 0);
    uapi_watchdog_kick();
    if (test_case == 0x0) {
        PRINT("Flash CMD read power consumption test...  \r\n");
        test_flash_cmd_read_consumption();
    } else if (test_case == 0x1) {
        PRINT("Flash continue read power consumption test...  \r\n");
        test_flash_continue_read_consumption();
    } else if (test_case == 0x2) {
        PRINT("Flash erase power consumption test...  \r\n");
        test_flash_erase_consumption();
    } else {
        PRINT("Flash write power consumption test...  \r\n");
        test_flash_write_consumption();
    }
    return TEST_SUITE_OK;
}

static int test_flash_io_drv_set(int argc, char *argv[])
{
    uint32_t drv_0, drv_1, drv_2, drv_3, drv_4, drv_5;
    if (argc < 0x6) {
        PRINT("para fail, drv need <smgpio0 smgpio1 smgpio2 smgpio3 smgpio4 smgpio5> set 0~15\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    drv_0 = (uint32_t)strtol(argv[0], NULL, 0);
    drv_1 = (uint32_t)strtol(argv[0x1], NULL, 0);
    drv_2 = (uint32_t)strtol(argv[0x2], NULL, 0);
    drv_3 = (uint32_t)strtol(argv[0x3], NULL, 0);
    drv_4 = (uint32_t)strtol(argv[0x4], NULL, 0);
    drv_5 = (uint32_t)strtol(argv[0x5], NULL, 0);
    PRINT("SMGPIO0~15 drv set:%d-%d-%d-%d-%d \r\n", drv_0, drv_1, drv_2, drv_3, drv_4, drv_5);
    /* User Guarantee Validity:0~15 */
    uapi_pin_set_ds(S_MGPIO0, (pin_drive_strength_t)drv_0);
    uapi_pin_set_ds(S_MGPIO1, (pin_drive_strength_t)drv_1);
    uapi_pin_set_ds(S_MGPIO2, (pin_drive_strength_t)drv_2);
    uapi_pin_set_ds(S_MGPIO3, (pin_drive_strength_t)drv_3);
    uapi_pin_set_ds(S_MGPIO4, (pin_drive_strength_t)drv_4);
    uapi_pin_set_ds(S_MGPIO5, (pin_drive_strength_t)drv_5);
    return TEST_SUITE_OK;
}
#endif

void add_flash_test_case(void)
{
    flash_porting_pinmux_cfg(FLASH_ID);
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    hal_xip_init();
    flash_porting_xip_disable();
#endif
#if !defined(CONFIG_FLASH_ALREADY_START)
    uint32_t manufacture_id;
    uapi_flash_init(FLASH_ID);
    uapi_flash_read_id(FLASH_ID, &manufacture_id);
    test_suite_log_stringf("manufacture_id = %x\r\n", manufacture_id);
    flash_save_manufacturer(FLASH_ID, manufacture_id);
#if !defined(CONFIG_FLASH_SUPPORT_SPI_SINGLE_LINE_MODE)
    uapi_flash_switch_to_qspi_init(FLASH_ID);
#endif
#endif
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_cache_mode(FLASH_ID);  // need remove
    uapi_test_suite_add_function("flash_xip_read", "Parmas: <test times>", test_flash_xip_read);
#endif
    uapi_test_suite_add_function("flash_write_data", "Parmas: <addr>, <value>", test_flash_write_data);
    uapi_test_suite_add_function("flash_power_down", "Parmas: <power 1: power dowen, 0: resume>", test_flash_power);
    uapi_test_suite_add_function("flash_read_data", "Parmas: <addr>", test_flash_read_data);
    uapi_test_suite_add_function(
        "flash_press", "Parmas: <mode  0: read, 1: write, 2: erase>, <value>", test_flash_press);

#if SOCMN1_CHIP_V100 == 1
    uapi_test_suite_add_function("flash_reinit_test", "flash_reinit_test", test_flash_reinit_test);
    uapi_test_suite_add_function("flash_write_traverse_page",
        "flash_write_traverse_page", test_flash_write_traverse_page);
    uapi_test_suite_add_function("flash_reboot_chip", "flash_reboot_chip", test_flash_reboot_chip);
    uapi_test_suite_add_function("flash_power_consumption", "power_consumption <0:read, 1:read_c 2:erase, 3: write>",
        test_flash_power_consumption);
    uapi_test_suite_add_function("flash_io_drv_set",
        "io_drv_set:<smgpio0 smgpio1 smgpio2 smgpio3 smgpio4 smgpio5> set 0~15", test_flash_io_drv_set);
    add_flash_xip_test_case();
#endif
}
