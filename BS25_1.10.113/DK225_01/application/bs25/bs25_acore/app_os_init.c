/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Application core os initialize interface header for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */

#include "cmsis_os2.h"
#include "app_os_init.h"
#include "log_common.h"
#include "log_def.h"
#include "log_uart.h"
#include "los_status.h"
#include "pmu_interrupt.h"
#include "log_reg_dump.h"
#include "watchdog.h"
#include "preserve.h"
#include "hal_reboot.h"
#include "pinctrl_porting.h"
#include "pinctrl.h"

#ifdef TEST_SUITE
#include "test_auxiliary.h"
#include "test_suite_uart.h"
#include "test_suite.h"
#include "test_keyscan.h"
#include "test_i2s.h"
#include "test_qdec.h"
#include "test_usb.h"
#include "test_pdm.h"
#include "test_adc.h"
#include "test_calendar.h"
#ifdef FTRACE
#include "test_ftrace.h"
#endif
#include "test_eflash.h"
#include "test_qdec.h"
#ifdef SLEM_CARKEY
#include "test_pos_dis.h"
#include "test_sle_hadm.h"
#include "sle_hadm_api.h"
#include "hadm_uapi.h"
#endif
#endif
#if (ENABLE_LOW_POWER == YES)
#include "pm_veto.h"
#endif
#ifdef OS_DFX_SUPPORT
#include "memory_info.h"
#include "os_dfx.h"
#endif

#if defined(CONFIG_SAMPLE_ENABLE)
#include "app_init.h"
#endif
#include "nv_btc_port.h"
#include "pm_pmu.h"
#include "board_evb_lp.h"
#include "nv_btc_port.h"
#include "pm_porting.h"
#include "pm_clock.h"

#ifdef AT_COMMAND
#include "at_product.h"
#include "at_bt_cmd_register.h"
#endif

#define TASK_COMMON_APP_DELAY_MS 5000
#define OSDELAY  1000

typedef void (*CreateQueueFunction_t)(void);

#define TASK_PRIORITY_APP (osPriority_t)(17)
#define TASK_PRIORITY_RPC (osPriority_t)(15)
#define TASK_PRIORITY_LOG (osPriority_t)(13)
#define TASK_PRIORITY_CMD ((osPriority_t)18)
#define TASK_PRIORITY_AT  ((osPriority_t)16)
#define TASK_PRIORITY_SDK ((osPriority_t)19)
#define TASK_PRIORITY_SRV ((osPriority_t)14)
#define TASK_PRIORITY_HADM ((osPriority_t)14)
#define TASK_PRIORITY_HADM_MSG ((osPriority_t)13)
#define TASK_PRIORITY_HADM_LOG ((osPriority_t)13)

#define AT_STACK_SIZE        0x800
#define CMD_STACK_SIZE       0xa00
#define APP_STACK_SIZE       0xa00
#define LOG_STACK_SIZE       0x600

#define STACK_SIZE_BASELINE     0x200
#define FS_EXTEND_STACK (STACK_SIZE_BASELINE * 3)
#ifdef SLEM_CARKEY
#define BTH_SDK_STACK_SIZE (STACK_SIZE_BASELINE * 14 + FS_EXTEND_STACK)
#define BTH_SERVICE_STACK_SIZE (STACK_SIZE_BASELINE * 80)
#define BTH_HADM_SERVICE_STACK_SIZE (STACK_SIZE_BASELINE * 4)
#define BTH_HADM_SERVICE_MSG_STACK_SIZE (STACK_SIZE_BASELINE * 4)
#define BTH_HADM_SERVICE_LOG_STACK_SIZE (STACK_SIZE_BASELINE * 4)
#else
#define BTH_SDK_STACK_SIZE (STACK_SIZE_BASELINE * 4 + FS_EXTEND_STACK)
#define BTH_SERVICE_STACK_SIZE (STACK_SIZE_BASELINE * 6)
#endif

#define BOOT_CTRL_MAGIC_FLAG        0x5A
#define FPGA_BT_BOOTUP_CTRL_REG     0x52000010
#define FPGA_DSP_BOOTUP_CTRL_REG    0x52000014
#define FPGA_DSP1_BOOTUP_CTRL_REG   0x52000018

typedef struct {
    osThreadAttr_t attr;
    osThreadFunc_t func;
    uint32_t *task_handle;
    CreateQueueFunction_t create_queue_fn_p;
} app_task_definition_t;

#define APP_MAIN_QUEUE_LEN  2

#define APP_MAIN_QUEUE_ITEM_SIZE (sizeof(uint32_t))
void cmd_processor_entry(void *args);
__attribute__((weak)) void app_main(void *unused);

void bt_acore_task_main(const void *pvParams);
void btsrv_task_body(const void *data);
static int8_t g_log_task_num = -1;
static int8_t g_at_task_num = -1;
static int8_t g_cmd_task_num = -1;

app_task_definition_t g_app_tasks[] = {
    { { "app", 0, NULL, 0, NULL, (APP_STACK_SIZE), TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)app_main, NULL, NULL },
    /* cmd loop must be g_app_tasks num 1 !!! */
#ifndef PRODUCT_VERSION
#ifdef TEST_SUITE
    { { "cmd_loop", 0, NULL, 0, NULL, CMD_STACK_SIZE, TASK_PRIORITY_CMD, 0, 0 },
      (osThreadFunc_t)cmd_processor_entry, NULL, (CreateQueueFunction_t)NULL },
#endif
#ifdef AT_COMMAND
    /* at loop must be g_app_tasks num 2 !!! */
    { { "at", 0, NULL, 0, NULL, AT_STACK_SIZE, TASK_PRIORITY_CMD, 0, 0 },
      (osThreadFunc_t)uapi_at_msg_main, NULL, NULL },
#endif
#endif

#if (USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == NO)
    { { "log", 0, NULL, 0, NULL, (LOG_STACK_SIZE), TASK_PRIORITY_LOG, 0, 0 },
      (osThreadFunc_t)log_main, NULL, NULL },
#endif
    { { "bt_sdk", 0, NULL, 0, NULL, BTH_SDK_STACK_SIZE, TASK_PRIORITY_SDK, 0, 0 },
      (osThreadFunc_t)bt_acore_task_main, NULL, (CreateQueueFunction_t)NULL },
    { { "bt_service", 0, NULL, 0, NULL, BTH_SERVICE_STACK_SIZE, TASK_PRIORITY_SRV, 0, 0 },
      (osThreadFunc_t)btsrv_task_body, NULL, (CreateQueueFunction_t)NULL },
    /* ---- user task defines from here ! ---- */
#ifdef SLEM_CARKEY
    {{"key", 0, NULL, 0, NULL, BTH_HADM_SERVICE_STACK_SIZE, TASK_PRIORITY_HADM, 0, 0},
     (osThreadFunc_t)key_main, NULL, (CreateQueueFunction_t)NULL},
    {{"key_msg", 0, NULL, 0, NULL, BTH_HADM_SERVICE_MSG_STACK_SIZE, TASK_PRIORITY_HADM_MSG, 0, 0},
     (osThreadFunc_t)sle_hadm_msg_proc, NULL, (CreateQueueFunction_t)NULL},
    {{"key_log", 0, NULL, 0, NULL, BTH_HADM_SERVICE_LOG_STACK_SIZE, TASK_PRIORITY_HADM_LOG, 0, 0},
     (osThreadFunc_t)sle_hadm_log_proc, NULL, (CreateQueueFunction_t)NULL},
#endif
};

#define M_NUM_TASKS (sizeof(g_app_tasks) / sizeof(app_task_definition_t))

void app_os_init(void *unused1, unsigned int unused2)
{
    UNUSED(unused1);
    UNUSED(unused2);
    // Create queues before tasks in case they are used during initialisation.
    for (uint8_t i = 0; i < M_NUM_TASKS; i++) {
        if (g_app_tasks[i].create_queue_fn_p) {
            g_app_tasks[i].create_queue_fn_p();
        }
        if (osal_strcmp(g_app_tasks[i].attr.name, "log") == 0) {
            g_log_task_num = i;
        } else if (osal_strcmp(g_app_tasks[i].attr.name, "at") == 0) {
            g_at_task_num = i;
        } else if (osal_strcmp(g_app_tasks[i].attr.name, "cmd_loop") == 0) {
            g_cmd_task_num = i;
        }
    }

    // Now create tasks
    for (uint8_t i = 0; i < M_NUM_TASKS; i++) {
        // You can't configure a task with a higher priority than the kernel
        // has been compiled to support. RTOS will clip the priority level,
        // which is probably not what you wanted.
        //
        // Similarly, if the watchdog kick is in the idle task and it's
        // intended to detect any other task being stuck, you don't want any
        // other task to have a priority <= the idle task.
        g_app_tasks[i].task_handle = osThreadNew(g_app_tasks[i].func, NULL, &(g_app_tasks[i].attr));
        if (g_app_tasks[i].task_handle == NULL) {
            panic(PANIC_TASK_CREATE_FAILED, i);
        }
    }

#ifdef AT_COMMAND
    if (g_at_task_num != -1 && g_cmd_task_num != -1) {
        set_task_id(g_app_tasks[g_at_task_num].task_handle, g_app_tasks[g_cmd_task_num].task_handle);
        osThreadSuspend(g_app_tasks[g_at_task_num].task_handle);
    }
#endif
#ifndef SLEM_CARKEY
#if defined(CONFIG_SAMPLE_ENABLE)
    system_init();
#endif
#endif
}

#ifdef TEST_SUITE

static int test_run_core(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    if (strcmp(argv[0], "bt") == 0) {
        PRINT("Boot bt.\r\n");
        uapi_pmu_control(PMU_CONTROL_BT_POWER, PMU_CONTROL_POWER_ON);
    } else {
        return -1;
    }
    return 0;
}

static int test_mcu_core_freq_config(int argc, char **argv)
{
    UNUSED(argc);
    uint8_t clk_src = (uint8_t)strtol(argv[0], NULL, 0);
    if (uapi_clock_control(CLOCK_CONTROL_FREQ_LEVEL_CONFIG, clk_src) == ERRCODE_SUCC) {
        PRINT("Config succ.\r\n");
        return 0;
    } else {
        PRINT("Config fail.\r\n");
        return -1;
    }
}

#if (ENABLE_LOW_POWER == YES)
static int test_mcu_vote_sleep(int argc, char* argv[])
{
    unused(argc);
    uint8_t vote = (uint8_t)strtol(argv[0], NULL, 0);
    if (vote == 0) {
        uapi_pm_add_sleep_veto(PM_VETO_MCU);
    } else {
        uapi_pm_remove_sleep_veto(PM_VETO_MCU);
    }
    return 0;
}
#endif

static void cmd_main_add_functions(void)
{
    PRINT("version: %s\r\n", APPLICATION_VERSION_STRING);
    add_auxiliary_functions();
#ifdef AT_COMMAND
    uapi_test_suite_add_function("testsuite_sw_at", "<at>", testsuite_sw_at);
#endif
    uapi_test_suite_add_function("run_cores", "<bt>", test_run_core);
    uapi_test_suite_add_function("btc_write_nv", "<bt>", app_write_config_nv_act);
    uapi_test_suite_add_function("mcu_core_freq", "<0:32M, 1:64M, 2:86M>", test_mcu_core_freq_config);
#if (ENABLE_LOW_POWER == YES)
    uapi_test_suite_add_function("mcu_vote_slp", "MCU vote to sleep or not Function", test_mcu_vote_sleep);
#endif
#ifdef SLEM_CARKEY
    add_pos_dis_test_case();
    add_sle_hadm_test_case();
    add_sle_hadm_nv_test_case();
#endif
#ifndef SIZE_MINI
    add_calendar_test_case();
    add_usb_test_case();
    add_adc_test_case();
    add_eflash_test_case();
    add_keyscan_test_case();
    add_qdec_test_case();
#endif

#ifdef FTRACE
    add_ftrace_test_case();
#endif
}

void cmd_processor_entry(void *args)
{
    UNUSED(args);
    // UART Initialisation
#ifdef USE_EMBED_FLASH
    uapi_pin_set_mode(S_AGPIO12, HAL_PIO_FUNC_UART_H0_M1);
    uapi_pin_set_mode(S_AGPIO13, HAL_PIO_FUNC_UART_H0_M1);
#else
    uapi_pin_set_mode(S_MGPIO23, HAL_PIO_FUNC_UART_H0_M2);
    uapi_pin_set_mode(S_MGPIO21, HAL_PIO_FUNC_UART_H0_M2);
#endif
    uapi_pin_set_mode(S_AGPIO14, HAL_PIO_FUNC_UART_L0_M1);
    uapi_pin_set_mode(S_AGPIO15, HAL_PIO_FUNC_UART_L0_M1);
    test_suite_uart_init();  // If this gets changed to a non-lp uart then a sleep veto should be added
    uapi_test_suite_init();
    cmd_main_add_functions();
}
#endif

static void pinctrl_low_power_config_for_evb(void)
{
    size_t pin_num = sizeof(g_pio_function_config_lp) / sizeof(g_pio_function_config_lp[0]);
    hal_pio_config_t *pio_func = g_pio_function_config_lp;

    for (uint32_t i = S_MGPIO0; i < (uint32_t)pin_num; i++) {
        uapi_pin_set_mode(i, pio_func[i].func);
        uapi_pin_set_ds(i, pio_func[i].drive);
        uapi_pin_set_pull(i, pio_func[i].pull);
    }
}

static void pm_low_power_config(void)
{
    pinctrl_low_power_config_for_evb();
}

__attribute__((weak)) void app_main(void *unused)
{
    UNUSED(unused);
    hal_reboot_clear_history();
    system_boot_reason_print();
    system_boot_reason_process();
#if (USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == NO)
    log_exception_dump_reg_check();
#endif
#ifndef HADM_CARKEY
#if defined(CONFIG_SAMPLE_ENABLE)
    app_tasks_init();
#endif
#endif
#ifdef OS_DFX_SUPPORT
    print_os_task_id_and_name();
#endif

    while (1) {  //lint !e716 Main Loop
        (void)osDelay(TASK_COMMON_APP_DELAY_MS);
        if (readl(0x52000014) == 0x5a) {
            uapi_pmu_control(PMU_CONTROL_BT_POWER, PMU_CONTROL_POWER_ON);
            writel(0x52000014, 0);
        }
        if (reg16_getbit(0x52000010, 0) != 0) { // bit0: gpio->low power
            pm_low_power_config();
            reg16_clrbit(0x52000010, 0);
        }
        if (reg16_getbit(0x52000010, 0x1) != 0 && g_log_task_num != -1) {   // bit1: Log suspend.
            osThreadSuspend(g_app_tasks[g_at_task_num].task_handle);
            reg16_clrbit(0x52000010, 0x1);
        } else if (reg16_getbit(0x52000010, 0x2) != 0 && g_log_task_num != -1) { // bit2: Log resume.
            osThreadResume(g_app_tasks[g_at_task_num].task_handle);
            reg16_clrbit(0x52000010, 0x2);
        }

#if defined(PM_MCPU_MIPS_STATISTICS_ENABLE) && (PM_MCPU_MIPS_STATISTICS_ENABLE == YES)
        oml_pf_log_print2(LOG_BCORE_PLT, LOG_NUM_DEBUG, LOG_LEVEL_INFO,
            "[Mcpu mips statistics] total work time: %dms, total idle time: %dms.\r\n",
            pm_get_total_work_time(), pm_get_total_idle_time());
#endif
#if CONFIG_NV_AUTO
        app_write_config_nv_sem();
#endif
        uapi_watchdog_kick();
#ifdef OS_DFX_SUPPORT
        os_dfx_print_info();
#endif
    }
}