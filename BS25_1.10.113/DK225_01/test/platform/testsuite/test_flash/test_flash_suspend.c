#if SOCMN1_CHIP_V100 == 1
#include "test_flash.h"
#include "debug_print.h"
#include "test_suite_functions.h"
#include "test_suite_errors.h"
#include "cmsis_os2.h"
#include "los_status.h"
#include "tcxo.h"
#include "watchdog.h"
#include "flash.h"
#include "securec.h"
#include "flash_common_config.h"
#include "systick.h"

typedef enum {
    FLASH_IDLE,  // 0
    FLASH_BUSY,  // 1
    FLASH_SUSPEND,  // 2
    FLASH_RESUME,  // 3
    FLASH_CHECK_REG_TIMEOUT, // 4
    FLASH_ERROR,  // 5
} flash_status;

typedef struct {
    flash_info_t flash_info;
    uint32_t status_protect;
    uint32_t process_delay_us;
    uint32_t erase_addr;
    uint32_t  erase_sus_mask;
    uint32_t  write_sus_mask;
    uint8_t  is_write_sus;
    uint32_t erase_nowwait_cost_max;
    uint32_t suspend_process_cost_max;
    uint32_t suspend_check_reg_cost_max;
    uint32_t suspend_process_not_suspend;
    uint32_t resume_process_cost_max;
    uint32_t resume_check_reg_cost_max;
    uint32_t suspend_process_cnt;
    uint32_t suspend_process_cnt_max;
    uint32_t xip_read_cnt;
    uint32_t xip_read_fail_cnt;
    uint32_t all_process_cost;
    uint32_t all_process_cost_max;
    uint32_t loop_cnt;
    uint32_t skip_reg_check;
    uint32_t reg0_timeout_sts;
    uint32_t reg1_timeout_sts;
    uint32_t reinit_flash_fail_cnt;
    uint32_t reinit_flash_cnt;
    uint32_t last_status;
    uint32_t erase_intf_fail_cnt;
    uint32_t write_intf_fail_cnt;
    uint32_t erase_suspend_check_reg_fail_cnt;
    uint32_t erase_resume_check_reg_fail_cnt;
    uint32_t write_suspend_check_reg_fail_cnt;
    uint32_t write_resume_check_reg_fail_cnt;
    uint32_t cost_s;
    uint32_t start_s;
} flash_times_info;

flash_times_info g_flash_times_info = {0};
static uint8_t g_write_test_buf[256] = {0};

static void dump_flash_info(void)
{
    PRINT("Flash chip id:0x%x \r\n", g_flash_times_info.flash_info.flash_id);
    PRINT("Flash size:0x%x \r\n", g_flash_times_info.flash_info.flash_size);
}
static void test_result_check_info(void)
{
    if (g_flash_times_info.status_protect == FLASH_ERROR) {
        PRINT("Suspend Resume Test: Fatal Error !!!\r\n");
    } else {
        PRINT("Suspend Resume Test: no fatal \r\n");
    }
    if ((g_flash_times_info.reinit_flash_cnt != 0) || (g_flash_times_info.erase_intf_fail_cnt != 0) ||
        (g_flash_times_info.write_intf_fail_cnt != 0) ||
        (g_flash_times_info.erase_suspend_check_reg_fail_cnt != 0) ||
        (g_flash_times_info.erase_resume_check_reg_fail_cnt != 0) ||
        (g_flash_times_info.write_suspend_check_reg_fail_cnt != 0) ||
        (g_flash_times_info.write_resume_check_reg_fail_cnt != 0)) {
        PRINT("Suspend Resume Test: Fair Warning !!!\r\n");
    } else {
        PRINT("Suspend Resume Test: not fair \r\n");
    }
    PRINT("Cost:[day:%d][hour:%d][min:%d][second:%d]\r\n", g_flash_times_info.cost_s / 60 / 60 / 24, /* 60 24 */
        (g_flash_times_info.cost_s / 60 / 60) % 24, (g_flash_times_info.cost_s / 60) % 60, /* 60  24 */
        g_flash_times_info.cost_s % 60); /* 60 */
}

static void dump_flash_times_info(void)
{
    test_result_check_info();
    dump_flash_info();
    PRINT("status_protect:%d \r\n", g_flash_times_info.status_protect);
    PRINT("last_status:%d \r\n", g_flash_times_info.last_status);
    PRINT("process_delay_us:%d \r\n", g_flash_times_info.process_delay_us);
    PRINT("erase_sus_mask:0x%x, write_sus_mask:0x%x, is_write:%d \r\n", g_flash_times_info.erase_sus_mask,
        g_flash_times_info.write_sus_mask, g_flash_times_info.is_write_sus);
    PRINT("erase_nowwait_cost_max:%d \r\n", g_flash_times_info.erase_nowwait_cost_max);
    PRINT("suspend_process_cost_max:%d \r\n", g_flash_times_info.suspend_process_cost_max);
    PRINT("suspend_check_reg_cost_max:%d \r\n", g_flash_times_info.suspend_check_reg_cost_max);
    PRINT("suspend_process_not_suspend:%d \r\n", g_flash_times_info.suspend_process_not_suspend);
    PRINT("resume_process_cost_max:%d \r\n", g_flash_times_info.resume_process_cost_max);
    PRINT("resume_check_reg_cost_max:%d \r\n", g_flash_times_info.resume_check_reg_cost_max);
    PRINT("suspend_process_cnt:%d \r\n", g_flash_times_info.suspend_process_cnt);
    PRINT("suspend_process_cnt_max:%d \r\n", g_flash_times_info.suspend_process_cnt_max);
    PRINT("xip_read_cnt:%d \r\n", g_flash_times_info.xip_read_cnt);
    PRINT("xip_read_fail_cnt:%d \r\n", g_flash_times_info.xip_read_fail_cnt);
    PRINT("all_process_cost:%d \r\n", g_flash_times_info.all_process_cost);
    PRINT("all_process_cost_max:%d \r\n", g_flash_times_info.all_process_cost_max);
    PRINT("loop_cnt:%d \r\n", g_flash_times_info.loop_cnt);
    PRINT("skip_reg_check:%d \r\n", g_flash_times_info.skip_reg_check);
    PRINT("reg0_timeout_sts :0x%x \r\n", g_flash_times_info.reg0_timeout_sts);
    PRINT("reg1_timeout_sts :0x%x\r\n", g_flash_times_info.reg1_timeout_sts);
    PRINT("Error Check:reinit_flash_fail_cnt  :%u \r\n", g_flash_times_info.reinit_flash_fail_cnt);
    PRINT("Error Check:reinit_flash_cnt  :%u \r\n", g_flash_times_info.reinit_flash_cnt);
    PRINT("Error Check:erase_intf_fail_cnt:%d \r\n", g_flash_times_info.erase_intf_fail_cnt);
    PRINT("Error Check:write_intf_fail_cnt:%d \r\n", g_flash_times_info.write_intf_fail_cnt);
    PRINT("Error Check:erase_suspend_reg_fail:%d \r\n", g_flash_times_info.erase_suspend_check_reg_fail_cnt);
    PRINT("Error Check:erase_resume_reg_fail:%d \r\n", g_flash_times_info.erase_resume_check_reg_fail_cnt);
    PRINT("Error Check:write_suspend_reg_fail:%d \r\n", g_flash_times_info.write_suspend_check_reg_fail_cnt);
    PRINT("Error Check:write_resume_reg_fail:%d \r\n", g_flash_times_info.write_resume_check_reg_fail_cnt);
}

void flash_erase_nowait_test(void)
{
    uint64_t t1 = uapi_tcxo_get_us();
    g_flash_times_info.erase_addr += 0x1000;
    g_flash_times_info.erase_addr = (g_flash_times_info.erase_addr % ERASE_TEST_MAX_SIZE) + ERASE_TEST_ADDR_START;
    errcode_t ret = uapi_flash_block_erase(0, g_flash_times_info.erase_addr, 0x1000, false);
    uint32_t nowwait_cost = (uint32_t)(uapi_tcxo_get_us() - t1);
    g_flash_times_info.erase_nowwait_cost_max = (g_flash_times_info.erase_nowwait_cost_max <=
        nowwait_cost) ? nowwait_cost : g_flash_times_info.erase_nowwait_cost_max;
    if (ret != ERRCODE_SUCC) {
        g_flash_times_info.status_protect = FLASH_ERROR;
        g_flash_times_info.erase_intf_fail_cnt++;
        PRINT("flash_erase_nowait fail:%d \r\n", ret);
    }
}

static void flash_write_nowait_test(void)
{
    uint32_t addr = g_flash_times_info.erase_addr - 0x1000;
    addr = (addr % ERASE_TEST_MAX_SIZE) + ERASE_TEST_ADDR_START;

    /* should repleace to nowait flash_write interface, but no available function */
    uint32_t cnt = uapi_flash_write_data(0, addr, g_write_test_buf, sizeof(g_write_test_buf));
    if (cnt != sizeof(g_write_test_buf)) {
        g_flash_times_info.status_protect = FLASH_ERROR;
        g_flash_times_info.write_intf_fail_cnt++;
        PRINT("flash_write_nowait_test fail:%d \r\n", cnt);
    }
}

void flash_delay_process(void)
{
    if (g_flash_times_info.process_delay_us != 0) {
        uapi_tcxo_delay_us(g_flash_times_info.process_delay_us);
    } else {
        uapi_tcxo_delay_us(uapi_tcxo_get_us() % SUS_MAX_RAND_DELAY_US);
    }
}

static void flash_read_sus_busy_regs(uint32_t *sus_reg, uint32_t *busy_reg)
{
    errcode_t ret;
    ret = uapi_flash_read_security_status(0, (uint8_t *)sus_reg);
    if (ret != ERRCODE_SUCC) {
        PRINT("read sus reg fail:%d, reg:0x%x \r\n", ret, *sus_reg);
    }
    ret = uapi_flash_read_status(0, (uint8_t *)busy_reg);
    if (ret != ERRCODE_SUCC) {
        PRINT("read busy reg fail:%d, reg:0x%x \r\n", ret, *busy_reg);
    }
}

static uint8_t is_wait_suspend_done(uint8_t is_sus, uint32_t sus_reg)
{
    uint32_t mask_value;
    uint8_t wait_done;
    if (g_flash_times_info.is_write_sus != 0) {
        mask_value = sus_reg & g_flash_times_info.write_sus_mask;
        wait_done = (mask_value == g_flash_times_info.write_sus_mask) ? 0x1 : 0;
    } else {
        mask_value = sus_reg & g_flash_times_info.erase_sus_mask;
        wait_done = (mask_value == g_flash_times_info.erase_sus_mask) ? 0x1 : 0;
    }
    return wait_done == is_sus;
}

static void flash_check_sus_timeout(uint8_t is_sus, uint8_t is_busy, uint32_t sus_reg, uint32_t busy_reg)
{
    if ((is_sus == 1) && (is_busy == 0)) {
        if (is_wait_suspend_done(0, sus_reg)) {
            g_flash_times_info.suspend_process_not_suspend++;
            return;
        }
    }
    PRINT("flash_check_sus_timeout is_sus:%d, timeout regs:%x - %x \r\n", is_sus, sus_reg, busy_reg);
    g_flash_times_info.reg0_timeout_sts = busy_reg;
    g_flash_times_info.reg1_timeout_sts = sus_reg;
    g_flash_times_info.status_protect = FLASH_CHECK_REG_TIMEOUT;
    if (is_sus) {
        if (g_flash_times_info.is_write_sus != 0) {
            g_flash_times_info.write_suspend_check_reg_fail_cnt++;
        } else {
            g_flash_times_info.erase_suspend_check_reg_fail_cnt++;
        }
    } else {
        if (g_flash_times_info.is_write_sus != 0) {
            g_flash_times_info.write_resume_check_reg_fail_cnt++;
        } else {
            g_flash_times_info.erase_resume_check_reg_fail_cnt++;
        }
    }
}

static void flash_wait_check_sus_busy(uint8_t is_sus, uint8_t is_busy)
{
    uint32_t sus_reg = 0;
    uint32_t busy_reg = 0;
    uint64_t t2;
    if (g_flash_times_info.skip_reg_check != 0) {
        return;
    }
    uint64_t t1 = uapi_tcxo_get_us();
    do {
        t2 = uapi_tcxo_get_us();
        flash_read_sus_busy_regs(&sus_reg, &busy_reg);
        if ((is_wait_suspend_done(is_sus, sus_reg)) && ((busy_reg & 0x1) == is_busy)) {
            break;
        }
        if (t2 > (t1 + SUS_CHECK_REGS_TIMEOUT_US)) {
            flash_check_sus_timeout(is_sus, is_busy, sus_reg, busy_reg);
            return;
        }
        if ((is_wait_suspend_done(is_sus, sus_reg)) && (is_sus == 0)) {
            break;
        }
    } while ((is_wait_suspend_done(is_sus, sus_reg) != 0x1) || ((busy_reg & 0x1) != is_busy));
    uint32_t cost_time = (uint32_t)(t2 - t1);
    if ((is_sus == 1) && (is_busy == 0)) {
        g_flash_times_info.suspend_check_reg_cost_max = (g_flash_times_info.suspend_check_reg_cost_max < cost_time) ?
            cost_time : g_flash_times_info.suspend_check_reg_cost_max;
    } else {
        g_flash_times_info.resume_check_reg_cost_max = (g_flash_times_info.resume_check_reg_cost_max < cost_time) ?
            cost_time : g_flash_times_info.resume_check_reg_cost_max;
    }
}

static void flash_suspend_process_test(void)
{
    uint64_t t1 = uapi_tcxo_get_us();
    errcode_t ret = uapi_flash_suspend(0);
    flash_delay_process();
    uint32_t process_cost = (uint32_t)(uapi_tcxo_get_us() - t1);
    g_flash_times_info.suspend_process_cost_max = (g_flash_times_info.suspend_process_cost_max <=
        process_cost) ? process_cost : g_flash_times_info.suspend_process_cost_max;
    if (ret != ERRCODE_SUCC) {
        g_flash_times_info.last_status = g_flash_times_info.status_protect;
        g_flash_times_info.status_protect = FLASH_ERROR;
        PRINT("flash_suspend fail:%d \r\n", ret);
        return;
    }
    g_flash_times_info.status_protect = FLASH_SUSPEND;
    flash_wait_check_sus_busy(1, 0);
}

static void flash_test_resume_process(void)
{
    uint64_t t1 = uapi_tcxo_get_us();
    errcode_t ret = uapi_flash_resume(0);
    flash_delay_process();
    uint32_t process_cost = (uint32_t)(uapi_tcxo_get_us() - t1);
    g_flash_times_info.resume_process_cost_max = (g_flash_times_info.resume_process_cost_max <=
        process_cost) ? process_cost : g_flash_times_info.resume_process_cost_max;
    if (ret != ERRCODE_SUCC) {
        g_flash_times_info.last_status = g_flash_times_info.status_protect;
        g_flash_times_info.status_protect = FLASH_ERROR;
        PRINT("flash_resume fail:%d \r\n", ret);
        return;
    }
    g_flash_times_info.status_protect = FLASH_RESUME;
    flash_wait_check_sus_busy(0, 1);
    if (g_flash_times_info.status_protect == FLASH_CHECK_REG_TIMEOUT) {
        return;
    }
    if (uapi_flash_is_processing(0) != true) {
        g_flash_times_info.status_protect = FLASH_IDLE;
    } else {
        g_flash_times_info.status_protect = FLASH_BUSY;
    }
}

static void flash_erase_suspend_test(void)
{
    if (g_flash_times_info.status_protect == FLASH_IDLE) {
        if (g_flash_times_info.is_write_sus != 0) {
            flash_write_nowait_test();
        } else {
            flash_erase_nowait_test();
        }
        if (g_flash_times_info.status_protect == FLASH_IDLE) {
            g_flash_times_info.status_protect = FLASH_BUSY;
        }
        flash_delay_process();
    } else if (g_flash_times_info.status_protect == FLASH_BUSY) {
        g_flash_times_info.suspend_process_cnt++;
        flash_suspend_process_test();
        if (g_flash_times_info.status_protect == FLASH_ERROR) {
            return;
        }
        flash_delay_process();
    } else if (g_flash_times_info.status_protect == FLASH_SUSPEND) {
        flash_test_resume_process();
        if (g_flash_times_info.status_protect == FLASH_ERROR) {
            return;
        }
    } else {
    }
    if (g_flash_times_info.status_protect == FLASH_CHECK_REG_TIMEOUT) {
        g_flash_times_info.reinit_flash_cnt++;
        if (uapi_flash_reset(0) != ERRCODE_SUCC) {
            g_flash_times_info.reinit_flash_fail_cnt++;
        }
        g_flash_times_info.status_protect = FLASH_IDLE;
    }
}

static void flash_clear_run_info(void)
{
    g_flash_times_info.suspend_process_cnt = 0;
    g_flash_times_info.xip_read_cnt = 0;
    g_flash_times_info.suspend_process_cnt = 0;
}

static void flash_erase_suspend_resume_xip_test(void)
{
    errcode_t ret;
    uint32_t val, val2;
    uint64_t t1 = uapi_tcxo_get_us();
    flash_clear_run_info();
    uapi_flash_exit_from_xip_mode(0);
    flash_erase_suspend_test();
    while ((g_flash_times_info.status_protect != FLASH_IDLE)) {
        if (g_flash_times_info.status_protect == FLASH_ERROR) {
            break;
        }
        ret = uapi_flash_exit_from_xip_mode(0);
        if (ret != ERRCODE_SUCC) {
            PRINT("flash_exit_from_xip_mode fail:%d \r\n", ret);
            break;
        }
        flash_erase_suspend_test();
        if ((g_flash_times_info.status_protect == FLASH_IDLE) ||
            (g_flash_times_info.status_protect == FLASH_SUSPEND)) {
            ret = uapi_flash_switch_to_xip_mode(0);
            if (ret != ERRCODE_SUCC) {
                PRINT("flash_switch_to_xip_mode fail:%d \r\n", ret);
                break;
            }
            val = readl(0x10000000);
            val2 = readl(0x14000000);
            g_flash_times_info.xip_read_cnt++;
            if ((val != 0x20002FFC) || (val != val2)) {
                g_flash_times_info.xip_read_fail_cnt++;
                g_flash_times_info.last_status = g_flash_times_info.status_protect;
                g_flash_times_info.status_protect = FLASH_ERROR;
                PRINT("xip read fail:0x%x - 0x%x \r\n", val, val2);
                break;
            }
        }
    }
    uint32_t t2 = (uint32_t)(uapi_tcxo_get_us() - t1);
    g_flash_times_info.all_process_cost_max = (g_flash_times_info.all_process_cost_max < t2) ?
        t2 : g_flash_times_info.all_process_cost_max;
    ret = uapi_flash_switch_to_xip_mode(0);
    if (ret != ERRCODE_SUCC) {
        PRINT("flash_switch_to_xip_mode11 fail:%d \r\n", ret);
    }
}

static void test_flash_sus_resume_early_init(void)
{
    /*  need read flash id and adapt mash value */
    uapi_flash_get_info(0, &g_flash_times_info.flash_info);
    if (g_flash_times_info.flash_info.flash_id == FLASH_MANUFACTURER_ELITE_EN25SX128A) {
        PRINT("FLASH: EN25SX128A \r\n");
        g_flash_times_info.erase_sus_mask = 0x80;
        g_flash_times_info.write_sus_mask = 0x04;
    } else if (g_flash_times_info.flash_info.flash_id == FLASH_MANUFACTURER_DOSILICON_DS25M4AB) {
        PRINT("FLASH:  DS25M4AB \r\n");
        g_flash_times_info.erase_sus_mask = 0x80;
        g_flash_times_info.write_sus_mask = 0x80;
    } else {
        PRINT("Flash id:0x%x, suspend Not adapt!!! \r\n", g_flash_times_info.flash_info.flash_id);
        g_flash_times_info.loop_cnt = 0;
        g_flash_times_info.erase_sus_mask = 0x80;
        g_flash_times_info.write_sus_mask = 0x80;
    }
    g_flash_times_info.is_write_sus = 0;
    g_flash_times_info.status_protect = FLASH_IDLE;
}

static void test_flash_erase_sus_resume_loops(void)
{
    uint32_t cost_s = 0;
    uint32_t t1, t2;
    if (g_flash_times_info.loop_cnt == 0) {
        return;
    }
    g_flash_times_info.start_s = (uint32_t)uapi_systick_get_s();
    t1 = (uint32_t)uapi_systick_get_s();
    for (; cost_s < g_flash_times_info.loop_cnt;) {
        g_flash_times_info.is_write_sus = 0;
        flash_erase_suspend_resume_xip_test();
        uapi_watchdog_kick();
        g_flash_times_info.is_write_sus = 1;
        flash_erase_suspend_resume_xip_test();
        uapi_watchdog_kick();
        g_flash_times_info.suspend_process_cnt_max = (g_flash_times_info.suspend_process_cnt_max <=
            g_flash_times_info.suspend_process_cnt) ? g_flash_times_info.suspend_process_cnt :
            g_flash_times_info.suspend_process_cnt_max;
        if (g_flash_times_info.status_protect != FLASH_IDLE) {
            break;
        }
        t2 = (uint32_t)uapi_systick_get_s();
        if (t2 > (t1 + 0x5)) {
            g_flash_times_info.cost_s += t2 - t1;
            t1 = t2;
            PRINT("Loop cost[%d ~ %d]S \r\n", cost_s, g_flash_times_info.loop_cnt);
            dump_flash_times_info();
        }
        if (g_flash_times_info.status_protect == FLASH_ERROR) {
            break;
        }
        cost_s = (uint32_t)uapi_systick_get_s() - g_flash_times_info.start_s;
    }
    PRINT("Loop cost[%d ~ %d]S \r\n", cost_s, g_flash_times_info.loop_cnt);
    dump_flash_times_info();
    g_flash_times_info.loop_cnt = 0;
}

static void test_flash_sus_resume_thread(void *unused)
{
    UNUSED(unused);
    PRINT("test_flash_sus_resume_thread start \r\n");
    while (1) {  //lint !e716 Main Loop
        test_flash_erase_sus_resume_loops();
        osDelay(50); /* 50ms */
    }
    PRINT("test_flash_sus_resume_thread end \r\n");
}

static osThreadAttr_t g_flash_sus_resume_thread_attr = { "sus_resume_thread_attr", 0, NULL, 0, NULL, (0x400),
    (osPriority_t)(15), 0, 0 };
static int test_flash_sus_resume_test(int argc, char *argv[])
{
    uint32_t delay_us;
    uint32_t loop_cnt = 1;
    uint32_t skip_reg_check;
    static uint8_t thread_flag = 0;
    if (argc < 0x3) {
        PRINT("para fail, need <delay_us><loops><skip_reg_check> ... \r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_us = (uint32_t)strtol(argv[0], NULL, 0);
    loop_cnt = (uint32_t)strtol(argv[0x1], NULL, 0);
    skip_reg_check = (uint32_t)strtol(argv[0x2], NULL, 0);
    if (g_flash_times_info.loop_cnt != 0) {
        PRINT("working, ignor cfg !!! \r\n");
        return TEST_SUITE_OK;
    }
    g_flash_times_info.process_delay_us = delay_us;
    g_flash_times_info.loop_cnt = loop_cnt;
    g_flash_times_info.skip_reg_check = skip_reg_check;
    if (thread_flag == 0) {
        thread_flag = 1;
        test_flash_sus_resume_early_init();
        PRINT("test_flash_sus_resume_creat ... \r\n");
        (void)osThreadNew((osThreadFunc_t)test_flash_sus_resume_thread, NULL, &(g_flash_sus_resume_thread_attr));
    }
    return TEST_SUITE_OK;
}

void add_flash_suspend_test_case(void)
{
    uapi_test_suite_add_function("flash_sus_resume_test", "arg:<delay, cost_s, skip_reg_check>",
        test_flash_sus_resume_test);
}
#endif