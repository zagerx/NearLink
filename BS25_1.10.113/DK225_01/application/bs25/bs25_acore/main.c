/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Applicationcore main function for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#include "cmsis_os2.h"
#include "ipc.h"
#include "string.h"
#include "rtc.h"
#include "timer.h"
#include "app_os_init.h"
#include "log_common.h"
#include "gpio.h"
#include "chip_io.h"
#include "memory_core.h"
#include "i2c.h"
#include "clocks.h"
#include "uart.h"
#include "log_uart.h"
#include "log_memory_region.h"
#include "securec.h"
#include "pinctrl_porting.h"
#include "pinctrl.h"
#include "tcxo.h"
#include "cpu_trace.h"
#include "application_version.h"
#include "mem_monitor.h"
#include "eflash.h"
#include "flash.h"
#include "flash_porting.h"
#include "pmu.h"
#include "systick.h"
#include "watchdog.h"
#include "drv_pmp.h"
#if CHIP_ASIC
#include "pmu_cmu.h"
#include "clock_calibration.h"
#include "hal_clocks_glb.h"
#include "low_power_management.h"
#endif
#include "otp.h"
#ifdef CONFIG_SYSTEM_VIEW
#include "SEGGER_SYSVIEW_Conf.h"
#include "SEGGER_SYSVIEW.h"
#endif
#include "cpu_utils.h"
#include "exception.h"
#include "log_oml_exception.h"
#include "los_task_pri.h"
#include "oam_trace.h"
#if CHIP_FPGA
#include "test_usb.h"
#include "usb_port.h"
#if (ENABLE_LOW_POWER == YES)
#include "pmu_interrupt.h"
#endif
#endif
#include "pm.h"
#include "pm_pmu.h"
#include "watchdog_porting.h"
#include "dfx_system_init.h"
#include "ota_upgrade.h"
#include "irmalloc.h"
#include "nv.h"
#include "nv_upg.h"
#include "soc_osal.h"
#include "nv_btc_port.h"
#include "at_btc_product.h"
#include "hal_xip.h"
#ifdef SUPPORT_CFBB_UPG
#include "partition.h"
#include "upg.h"
#endif
#include "preserve.h"
#include "last_dump_adapt.h"

#ifdef SLEM_CARKEY
#include "eflash.h"
#include "lib_config.h"
#include "pm_clock.h"
#include "los_memory.h"
#include "los_cppsupport.h"
#endif

#ifdef CONFIG_SAMPLE_SUPPORT_SLE_VDT_WITH_DONGLE
#include "pm_clock.h"
#endif

#ifdef OS_DFX_SUPPORT
#include "os_dfx.h"
#endif

#define WDT_TIMEOUT_S 30
#define IMG_NUM_MAX   2
#define IMG_SEC       0
#define IMG_HIFI      1
#define IPC_SOFT_RST_N           (M_CTL_RB_BASE + 0x038)
#define CPU_TRACE_CLK_REG        0x5200410c
#define CPU_TRACE_CLK_VAL        0x1ff

typedef enum {
    PMP_ROM_ID1,
    PMP_ITCM_ID2,
    PMP_UNDEF_MEM_ID3,
    PMP_DTCM_ID4,
    MAX_REGION_NUM
} region_index_t;

typedef struct {
    uint16_t key_id;
    uint16_t klength;
} reserved_key_t;

static void app_mpu_enable(void)
{
    /*
     *  bs25 support pmp_attr：
     *  PMP_ATTR_DEVICE_NO_BUFFERABLE
     *  PMP_ATTR_NO_CACHEABLE_AND_BUFFERABLE
     *  PMP_ATTR_WRITEBACK_RALLOCATE
     *  PMP_ATTR_WRITEBACK_RWALLOCATE
     */
    pmp_conf_t region_attr[MAX_REGION_NUM] = {
        /* bootrom pmp */
        {
            PMP_ROM_ID1, 0x10000, 0,
            { PMPCFG_NO_ACCESS, PMPCFG_ADDR_MATCH_TOR, true, PMP_ATTR_DEVICE_NO_BUFFERABLE },
        },
        /* itcm: .ramtext .rodata */
        {
            PMP_ITCM_ID2, 0x30000, 0,
            { PMPCFG_READ_ONLY_EXECUTE, PMPCFG_ADDR_MATCH_TOR, true, PMP_ATTR_NO_CACHEABLE_AND_BUFFERABLE },
        },
        /* undef mem region */
        {
            PMP_UNDEF_MEM_ID3, 0x20000000, 0,
            { PMPCFG_RW_NEXECUTE, PMPCFG_ADDR_MATCH_TOR, true, PMP_ATTR_DEVICE_NO_BUFFERABLE },
        },
        /* dtcm: .data .bss .stack .heap */
        {
            PMP_DTCM_ID4, 0x20030000, 0,
            { PMPCFG_RW_NEXECUTE, PMPCFG_ADDR_MATCH_TOR, true, PMP_ATTR_NO_CACHEABLE_AND_BUFFERABLE },
        },
    };
    uapi_pmp_config((pmp_conf_t *)region_attr, MAX_REGION_NUM);
}

static void hardware_config_init(void)
{
    uapi_pin_init();
    uapi_gpio_init();
    size_t pin_num;
    hal_pio_config_t *pio_func = NULL;
    bool result = false;

    get_pio_func_config(&pin_num, &pio_func);
    for (pin_t i = S_MGPIO0; i < pin_num; i++) {
        if (pio_func[i].func != HAL_PIO_FUNC_INVALID) {
            if (pio_func[i].func == HAL_PIO_FUNC_DEFAULT_HIGH_Z) {
                result = uapi_pin_set_mode(i, HAL_PIO_FUNC_GPIO);
                uapi_gpio_set_dir(i, GPIO_DIRECTION_INPUT);
            } else {
                result = uapi_pin_set_mode(i, pio_func[i].func);
                result = uapi_pin_set_ds(i, pio_func[i].drive);
            }
            if (pio_func[i].pull != HAL_PIO_PULL_MAX) {
                uapi_pin_set_pull(i, pio_func[i].pull);
            }
            UNUSED(result);
        }
    }
}

static void hw_init(void)
{
    panic_init();
#ifdef SW_UART_DEBUG
    sw_debug_uart_init(SW_UART_BAUDRATE);    
#endif
    /* Initialise malloc */
    irmalloc_init_default();
    app_mpu_enable();
#if CHIP_ASIC
    otp_init();
#endif
    hardware_config_init();
#if CHIP_ASIC
    pmu_init();
    clocks_init();
#endif

    uapi_pm_lpc_init();
    uapi_rtc_init();
    uapi_rtc_adapter(RTC_0, RTC_0_IRQN, g_aucIntPri[RTC_0_IRQN]);
    writel(IPC_SOFT_RST_N, 1);
    ipc_init();
}

static bool get_bt_power_on_handler(ipc_action_t message,
                                    const volatile ipc_payload *payload_p, cores_t src, uint32_t id)
{
    if (message != IPC_ACTION_BT_POWER_ON) {
        return true;
    }

    UNUSED(message);
    UNUSED(id);
    UNUSED(src);

    flash_porting_set_bt_power_on_flag(payload_p->bt_power_on_flag.flag);
    return true;
}

static void app_get_bt_xip_access(void)
{
    (void)ipc_register_handler(IPC_ACTION_BT_POWER_ON, (ipc_action_handler)get_bt_power_on_handler);
}

static void watchdog_init(void)
{
    watchdog_turnon_clk();
    watchdog_func_adapt(CHIP_WDT_TIMEOUT_32S);
    uapi_watchdog_init(CHIP_WDT_TIMEOUT_32S);
    uapi_watchdog_enable(CHIP_WDT_MODE_INTERRUPT);

    ulp_wdt_init(ULP_WDT_TIMEOUT_40S);
#ifdef NO_TIMEOUT
    uapi_watchdog_disable();
    watchdog_turnoff_clk();
    ulp_wdt_deinit();
#endif
}

static void app_upg_init(void)
{
#ifdef SUPPORT_CFBB_UPG
    uapi_partition_init();
    upg_func_t upg_func = {0};
    upg_func.malloc = malloc;
    upg_func.free = free;
    upg_func.serial_putc = NULL;
    (void)uapi_upg_init(&upg_func);
#endif
    uapi_upgrade_init();
}

static void app_nv_init(void)
{
    uapi_nv_init();
    app_read_nv_customize_msg();
    ipc_app_read_nv_customize_msg();
    /* nv init must be before nv_upg_upgrade_task_process */
    (void)nv_upg_upgrade_task_process();
}

static void app_clocks_init(void)
{
    uapi_tcxo_init();
    uapi_systick_init();
    uapi_timer_init();
    uapi_timer_adapter(TIMER_INDEX_0, TIMER_0_IRQN, OS_HWI_PRIO_LOWEST - 1);
}

static void app_logs_init(void)
{
    log_memory_region_init();
    log_init();
    /* Tell the logging logging consumer/writer to switch to using a mutex (requires RTOS init). */
    log_init_after_rtos();
    log_uart_init_after_rtos();
    log_uart_init();
}

static void irq_bt_wdt_handler(void)
{
    osal_irq_clear(B_WDT_IRQN);
    dfx_last_dump();
    cpu_utils_reset_chip_with_cause(REBOOT_CAUSE_APPLICATION_STD_BT_WDT_FRST);
}

static void data_flash_pin_cfg(void)
{
#ifndef USE_EMBED_FLASH
    uapi_pin_set_mode(S_AGPIO5, HAL_PIO_FUNC_QSPI0);
    uapi_pin_set_mode(S_AGPIO6, HAL_PIO_FUNC_QSPI0);
    uapi_pin_set_mode(S_AGPIO8, HAL_PIO_FUNC_QSPI0);
    uapi_pin_set_mode(S_AGPIO9, HAL_PIO_FUNC_QSPI0);
    uapi_pin_set_mode(S_AGPIO12, HAL_PIO_FUNC_QSPI0);
    uapi_pin_set_mode(S_AGPIO13, HAL_PIO_FUNC_QSPI0);
#endif
}

#ifdef SLEM_CARKEY
static void key_hadm_init(void)
{
    /* key hadm mem pool init. */
    (void)LOS_MemInit((void *)OS_SYS_KEY_ADDR, OS_SYS_KEY_SIZE);
#ifdef BS25_SUPPORT_CPP
    /* liteos cpp init. */
    (void)LOS_CppSystemInit((uintptr_t)&__init_array_start, (uintptr_t)&__init_array_end, NO_SCATTER);
#endif
    /* work 64M */
    uapi_eflash_init(EMBED_FLASH_0);
    uapi_eflash_set_freq(EMBED_FLASH_0, CLOCK_64M);
    uapi_eflash_init(EMBED_FLASH_1);
    uapi_eflash_set_freq(EMBED_FLASH_1, CLOCK_64M);
    uapi_clock_crg_config(CLOCK_CRG_ID_COM_BUS, CLOCK_CLK_SRC_TCXO_2X, 1);
    uapi_clock_crg_config(CLOCK_CRG_ID_MEM_BUS, CLOCK_CLK_SRC_TCXO_2X, 1);
    uapi_clock_crg_config(CLOCK_CRG_ID_XIP_OPI, CLOCK_CLK_SRC_TCXO_2X, 1);
    uapi_clock_crg_config(CLOCK_CRG_ID_MCU_CORE, CLOCK_CLK_SRC_TCXO_2X, 1);
}
#endif

void prepare_config_of_main(void)
{
    watchdog_init();
    BoardConfig();
    (void)osKernelInitialize();
    ArchSetExcHook((EXC_PROC_FUNC)do_fault_handler);
    ArchSetNMIHook((NMI_PROC_FUNC)do_hard_fault_handler);
    app_clocks_init();
    hw_init();
    data_flash_pin_cfg();
#ifdef CONFIG_SAMPLE_SUPPORT_SLE_VDT_WITH_DONGLE
    uapi_clock_control(CLOCK_CONTROL_BT_CORE_FREQ_CONFIG, CLOCK_MCU_CTL_BT_64M);
#endif
    app_get_bt_xip_access();
}

void main(const void *startup_details_table)
{
    startup_table ssb_to_application_table;
    errno_t sec_ret;
    sec_ret = memcpy_s(&ssb_to_application_table, sizeof(ssb_to_application_table),
                       startup_details_table, sizeof(ssb_to_application_table));
    if (sec_ret != EOK) {
        panic(PANIC_MEMORY, __LINE__);
        return;  //lint !e527  unreachable code
    }
    prepare_config_of_main();
#ifdef USE_EMBED_FLASH
    uapi_eflash_init(0);
    uapi_eflash_set_freq(0, CLOCK_32M);
    uapi_eflash_init(1);
    uapi_eflash_set_freq(1, CLOCK_32M);
#else
    uint32_t flash_id = ssb_to_application_table.ssb_to_application_flash_id;
    hal_xip_init();
    flash_funcreg_adapt(0);
    flash_save_manufacturer(0, flash_id);
#endif
    app_logs_init();
#ifdef SLEM_CARKEY
    // osal_printk("test");
    key_hadm_init();
#endif
    cpu_utils_init();
    hal_register_exception_dump_callback(log_exception_dump);
#ifdef CONFIG_SYSTEM_VIEW
    SEGGER_SYSVIEW_Conf();
#endif
    /* Enable MCPU trace */
    if (get_cpu_utils_reset_cause() != REBOOT_CAUSE_APPLICATION_STD_CHIP_WDT_FRST) {
        cpu_trace_enable_mcpu_repeat();
#ifdef OS_DFX_SUPPORT
        os_dfx_trace_init();
        LOS_TaskSwitchHookReg(os_dfx_task_switch_trace);
        LOS_HwiPreHookReg(os_dfx_hwi_pre);
        LOS_HwiPostHookReg(os_dfx_hwi_post);
#endif
    } else { writel(CPU_TRACE_CLK_REG, CPU_TRACE_CLK_VAL); }

    /* Set the initialisation routine to run immediately as soon as RTOS starts */
    /* Note the cast, as uint32_t and uint32_t are treated as different on some compilers. */
    dfx_system_init();
    app_upg_init();
    app_nv_init();
    uapi_pmu_control(PMU_CONTROL_BT_POWER, PMU_CONTROL_POWER_ON);
    osal_irq_request(B_WDT_IRQN, (osal_irq_handler)irq_bt_wdt_handler, NULL, NULL, NULL);
    osal_irq_set_priority(B_WDT_IRQN, irq_prio(B_WDT_IRQN));
    osal_irq_enable(B_WDT_IRQN);

    app_os_init(NULL, 0);
    /* Start LiteOS */
    (void)osKernelStart();
    for (;;) { }
}