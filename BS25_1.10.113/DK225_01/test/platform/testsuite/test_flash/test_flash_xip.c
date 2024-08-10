#if SOCMN1_CHIP_V100 == 1
#include "test_flash.h"
#include "debug_print.h"
#include "test_suite_functions.h"
#include "test_suite_errors.h"
#include "cmsis_os2.h"
#include "los_status.h"
#include "tcxo.h"
#include "watchdog.h"
#include "systick.h"

typedef struct {
    uint32_t curr_idx;
    uint32_t err_cnt;
    uint32_t addr[XIP_FAIL_TRANCE_MAX];
    uint32_t value[XIP_FAIL_TRANCE_MAX];
} xip_read_fail_trance;

typedef struct {
    uint32_t cost_s;
    uint32_t start_s;
    uint32_t xip_threa1_loops;
    uint32_t xip_thread2_loops;
    uint32_t xip_thread3_loops;
    xip_read_fail_trance xip_fail_trance;
} flash_test_ctrl_t;

static osThreadAttr_t g_flash_xip_thread_attr1 = { "xip_thread_attr1", 0, NULL, 0, NULL, (0x400),
    (osPriority_t)(15), 0, 0 };
static osThreadAttr_t g_flash_xip_thread_attr2 = { "xip_thread_attr2", 0, NULL, 0, NULL, (0x400),
    (osPriority_t)(15), 0, 0 };
static osThreadAttr_t g_flash_xip_thread_attr3 = { "xip_thread_attr3", 0, NULL, 0, NULL, (0x400),
    (osPriority_t)(15), 0, 0 };

flash_test_ctrl_t g_flash_test_ctrl = {0};

static void dump_flash_test_info(void)
{
    if (g_flash_test_ctrl.xip_fail_trance.err_cnt != 0) {
        PRINT("XIP Test: Fatal Error !!! \r\r");
    } else {
        PRINT("XIP Test: processing !!! \r\r");
    }
    g_flash_test_ctrl.cost_s = (uint32_t)uapi_systick_get_s() - g_flash_test_ctrl.start_s;
    PRINT("Cost:[day:%d][hour:%d][min:%d]\r\n", g_flash_test_ctrl.cost_s / 60 / 60 / 24, /* 60 24 */
        (g_flash_test_ctrl.cost_s / 60 / 60) % 24, (g_flash_test_ctrl.cost_s / 60) % 60); /* 60  24 */
    PRINT("thread loops:%d, %d, %d \r\n", g_flash_test_ctrl.xip_threa1_loops,
        g_flash_test_ctrl.xip_thread2_loops, g_flash_test_ctrl.xip_thread3_loops);
    PRINT("xip curr_idx:%d, err_cnt:%d \r\n", g_flash_test_ctrl.xip_fail_trance.curr_idx,
        g_flash_test_ctrl.xip_fail_trance.err_cnt);
    uint32_t xip_cnt;
    for (xip_cnt = 0; xip_cnt < g_flash_test_ctrl.xip_fail_trance.curr_idx; xip_cnt++) {
        PRINT("xip fail %d, addr:0x%x, value:0x%x \r\n", xip_cnt, g_flash_test_ctrl.xip_fail_trance.addr[xip_cnt],
            g_flash_test_ctrl.xip_fail_trance.value[xip_cnt]);
    }
}

static void dump_trance_test_info(void)
{
    static uint64_t tick_pre = 0;
    uint64_t tick_curr = uapi_tcxo_get_ms();
    if ((tick_curr - tick_pre) < 5000) { /* 5000ms */
        return;
    }
    tick_pre = tick_curr;
    dump_flash_test_info();
}

static void test_flash_xip_read_init(void)
{
    PRINT("test_flash_xip_read_init start:0x%x, size:0x%x \r\n", XIP_READ_ADDR_START, XIP_READ_MAX_SIZE);
    uint32_t val1 = readl(XIP_FLASH_BASE_ADDR + XIP_READ_ADDR_START);
    uint32_t val2 = readl(XIP_FLASH_BASE_ADDR + XIP_READ_ADDR_START + XIP_READ_MAX_SIZE - 0x1000);
    if ((val1 == val2) && (val1 == XIP_CMP_VALUE_DATA)) {
        PRINT("already init xip zoon \r\n");
        return;
    }
    test_flash_write_area(XIP_READ_ADDR_START, XIP_READ_MAX_SIZE);
}

static void test_xip_read_fail_trance(uint32_t addr, uint32_t value)
{
    g_flash_test_ctrl.xip_fail_trance.err_cnt++;
    uint32_t idx = g_flash_test_ctrl.xip_fail_trance.curr_idx;
    if (idx < XIP_FAIL_TRANCE_MAX) {
        g_flash_test_ctrl.xip_fail_trance.addr[idx] = addr;
        g_flash_test_ctrl.xip_fail_trance.value[idx] = value;
        g_flash_test_ctrl.xip_fail_trance.curr_idx += 1;
    }
}

static void test_flash_xip_read_test(void)
{
    static uint32_t addr = 0;
    uint32_t offset;
    uint32_t value;
    for (offset = 0; offset < 0x8000; offset += 0x4) {
        addr = ((addr + offset) % XIP_READ_MAX_SIZE) + XIP_READ_ADDR_START;
        value = readl(addr + XIP_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_FLASH_BASE_ADDR, value);
        }
        value = readl(addr + XIP_BYPASS_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_BYPASS_FLASH_BASE_ADDR, value);
        }
    }
}

#define MAX_RAND_DELAY_US   1000
static void test_flash_xip_read_test_random_delay(void)
{
    static uint32_t addr = 0;
    uint32_t offset;
    uint32_t value;
    for (offset = 0; offset < 0x8000; offset += 0x4) {
        addr = ((addr + offset) % XIP_READ_MAX_SIZE) + XIP_READ_ADDR_START;
        value = readl(addr + XIP_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_FLASH_BASE_ADDR, value);
        }
        value = readl(addr + XIP_BYPASS_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_BYPASS_FLASH_BASE_ADDR, value);
        }
        uapi_watchdog_kick();
        uapi_tcxo_delay_us(uapi_tcxo_get_us() % MAX_RAND_DELAY_US);
    }
}

static void test_flash_xip_read_test2(void)
{
    static uint32_t addr = 0;
    uint32_t offset;
    uint32_t value;
    for (offset = 0; offset < 0x40000; offset += 0x4) {
        addr = ((addr + offset) % XIP_READ_MAX_SIZE) + XIP_READ_ADDR_START;
        value = readl(addr + XIP_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_FLASH_BASE_ADDR, value);
        }
        value = readl(addr + XIP_BYPASS_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_BYPASS_FLASH_BASE_ADDR, value);
        }
    }
}

/* return flash inner addr */
static uint32_t test_get_random_read_addr(void)
{
    uint32_t addr = (uint32_t)uapi_tcxo_get_count();
    addr &= ~(0x3);
    uint32_t itcm_value = ((addr % 0x20000) + 0x80000) & (~0x3);
    itcm_value = readl(itcm_value);
    addr += itcm_value;
    addr = ((addr % XIP_READ_MAX_SIZE) + XIP_READ_ADDR_START) & (~0x3);
    return addr;
}

static void test_flash_xip_random_read_test(void)
{
    uint32_t cnt;
    uint32_t addr;
    uint32_t value;
    for (cnt = 0; cnt < 0x4000; cnt++) {
        addr = test_get_random_read_addr();
        value = readl(addr + XIP_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_FLASH_BASE_ADDR, value);
        }
        value = readl(addr + XIP_BYPASS_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_BYPASS_FLASH_BASE_ADDR, value);
        }
    }
}

#define RECORD_RANDOM_ADDR_LENGTH 2048
static uint32_t g_record_random_addr[RECORD_RANDOM_ADDR_LENGTH];
static void test_flash_xip_random_read_test_record(void)
{
    uint32_t cnt;
    uint32_t addr;
    uint32_t value;
    static uint32_t record_addr_cnt = 0;
    for (cnt = 0; cnt < RECORD_RANDOM_ADDR_LENGTH; cnt++) {
        if (record_addr_cnt < RECORD_RANDOM_ADDR_LENGTH) {
            addr = test_get_random_read_addr();
            g_record_random_addr[record_addr_cnt] = addr;
            record_addr_cnt++;
        } else {
            addr = g_record_random_addr[cnt];
        }
        value = readl(addr + XIP_BYPASS_FLASH_BASE_ADDR);
        if (value != XIP_CMP_VALUE_DATA) {
            test_xip_read_fail_trance(addr + XIP_BYPASS_FLASH_BASE_ADDR, value);
        }
    }
}

static void test_random_os_delay(void)
{
    uint32_t delay_ms = (uint32_t)uapi_tcxo_get_count();
    (void)osDelay(delay_ms % THREAD_RANDOM_DELAY_MAX);
}

static void test_flash_xip_thread1(void *unused)
{
    unused(unused);
    PRINT("test_flash_xip_thread1 start \r\n");
    while (1) {  //lint !e716 Main Loop
        g_flash_test_ctrl.xip_threa1_loops++;
        uapi_watchdog_kick();
        test_flash_xip_read_test2();
        test_flash_xip_random_read_test();
        test_flash_xip_read_test();
        test_flash_xip_random_read_test();
        test_random_os_delay();
        osDelay(50); /* 50ms */
        dump_trance_test_info();
    }
    PRINT("test_flash_xip_thread1 end \r\n");
}

static void test_flash_xip_thread2(void *unused)
{
    unused(unused);
    PRINT("test_flash_xip_thread2 start \r\n");
    while (1) {  //lint !e716 Main Loop
        g_flash_test_ctrl.xip_thread2_loops++;
        uapi_watchdog_kick();
        test_flash_xip_random_read_test_record();
        test_flash_xip_read_test();
        test_flash_xip_random_read_test();
        test_random_os_delay();
        dump_trance_test_info();
    }
    PRINT("test_flash_xip_thread2 end \r\n");
}

static void test_flash_xip_thread3(void *unused)
{
    unused(unused);
    PRINT("test_flash_xip_thread3 start \r\n");
    while (1) {  //lint !e716 Main Loop
        g_flash_test_ctrl.xip_thread3_loops++;
        test_flash_xip_random_read_test_record();
        uapi_watchdog_kick();
        test_flash_xip_read_test();
        test_flash_xip_random_read_test();
        test_flash_xip_read_test_random_delay();
        test_random_os_delay();
        osDelay(30); /* 30ms */
        dump_trance_test_info();
    }
    PRINT("test_flash_xip_thread3 end\r\n");
}

static int test_flash_xip_read_creat(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    static uint8_t xip_thread_flag = 0;
    test_flash_xip_read_init();
    if (xip_thread_flag == 0) {
        xip_thread_flag = 1;
        PRINT("test_flash_xip_read_creat ... \r\n");
        g_flash_test_ctrl.start_s = (uint32_t)uapi_systick_get_s();
        (void)osThreadNew((osThreadFunc_t)test_flash_xip_thread1, NULL, &(g_flash_xip_thread_attr1));
        (void)osThreadNew((osThreadFunc_t)test_flash_xip_thread2, NULL, &(g_flash_xip_thread_attr2));
        (void)osThreadNew((osThreadFunc_t)test_flash_xip_thread3, NULL, &(g_flash_xip_thread_attr3));
    } else {
        PRINT("test_flash_xip_read_creat already creat !!! \r\n");
    }
    return TEST_SUITE_OK;
}

void add_flash_xip_test_case(void)
{
    uapi_test_suite_add_function("test_flash_xip_read_creat", "test_flash_xip_read_creat", test_flash_xip_read_creat);
}

#endif