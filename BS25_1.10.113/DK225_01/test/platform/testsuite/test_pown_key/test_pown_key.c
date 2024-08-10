/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: pown key demo \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-11， Create file. \n
 */

#include "pwm.h"
#include "test_suite.h"
#include "osal_interrupt.h"
#include "osal_timer.h"
#include "osal_debug.h"
#include "chip_core_irq.h"
#include "arch_port.h"
#include "gpio.h"
#include "tcxo.h"
#include "pinctrl.h"
#include "test_pown_key.h"

#define PWM_BASE_TIME 300

#define PWM_TIME_SPEED_1 30
#define PWM_TIME_SPEED_2 300
#define PWM_TIME_SPEED_3 900

#define PWM_CHANNEL   5

#define DOUBLE_CLICK_INTERVEL   400
#define POWN_KEY_DOWN_BIT       7
#define POWN_KEY_UP_BIT         6
#define POWN_PMU_REG_ADDR       (0x5702e320)
#define POWN_PMU_CLICK_NUM_REG_ADDR       (0x5702e258)

#define POWN_KEY_SET_SPEED_TIME 3
#define POWN_KEY_SET_SPEED_DIR  4

#define POWN_KEY_SPEED_NUM      3

static void pown_timer_handle(unsigned long data);

static uint32_t g_pown_spped_level = 1;
static bool g_pown_dir = 0;

static osal_timer g_pown_timer = {
    .timer = 0,
    .data = 0,
    .handler = pown_timer_handle,
    .interval = DOUBLE_CLICK_INTERVEL
};

static uint32_t pown_get_speed(void)
{
    uint32_t speed[POWN_KEY_SPEED_NUM] = {PWM_TIME_SPEED_1, PWM_TIME_SPEED_2, PWM_TIME_SPEED_3};
    if (g_pown_spped_level > POWN_KEY_SPEED_NUM) {
        return 0;
    }
    return speed[g_pown_spped_level - 1];
}

static void pown_pwm_init(void)
{
    uapi_pwm_init();
    pwm_config_t cfg_repeat = {0, 0, 0xFF, 0xFF, true};
    cfg_repeat.low_time = PWM_BASE_TIME;
    cfg_repeat.high_time = pown_get_speed();
    uapi_pin_set_mode(S_AGPIO0, HAL_PIO_FUNC_PWM_P);
    uapi_pwm_open(PWM_CHANNEL, &cfg_repeat);
    uapi_pwm_start(PWM_CHANNEL);
}

static void pown_refresh_revolve_speed(void)
{
    if (!g_pown_dir) {
        uapi_pwm_update_duty_ratio(PWM_CHANNEL, PWM_BASE_TIME, pown_get_speed());
    } else {
        uapi_pwm_update_duty_ratio(PWM_CHANNEL, pown_get_speed(), PWM_BASE_TIME);
    }
}

static void pown_revolve_reverse(void)
{
    g_pown_dir = !g_pown_dir;
    osal_printk("g_pown_dir:%d\r\n", g_pown_dir);
    uapi_gpio_set_val(S_MGPIO21, g_pown_dir);
    pown_refresh_revolve_speed();
}

static void pown_timer_handle(unsigned long data)
{
    UNUSED(data);
    uint8_t time = readl(POWN_PMU_CLICK_NUM_REG_ADDR);
    osal_printk("click time %d\r\n", time);

    if (time <= POWN_KEY_SET_SPEED_TIME) {
        g_pown_spped_level = time;
        osal_printk("g_pown_spped_level:%d\r\n", g_pown_spped_level);
        pown_refresh_revolve_speed();
    } else if (time == POWN_KEY_SET_SPEED_DIR) {
        pown_revolve_reverse();
    }
    writel(POWN_PMU_CLICK_NUM_REG_ADDR, 0);
    osal_timer_stop(&g_pown_timer);
}

static void pmu_irqn(void)
{
    if ((readl(POWN_PMU_REG_ADDR) & BIT(POWN_KEY_DOWN_BIT)) != 0) {
        // clear key down interrupt
        reg_setbit(POWN_PMU_REG_ADDR, 0, POWN_KEY_DOWN_BIT);
    }
    if ((readl(POWN_PMU_REG_ADDR) & BIT(POWN_KEY_UP_BIT)) != 0) {
        // clear key up interrupt
        reg_setbit(POWN_PMU_REG_ADDR, 0, POWN_KEY_UP_BIT);
    }
    uapi_tcxo_delay_ms(1);
    writel(POWN_PMU_REG_ADDR, 0xff);
    // restart timer
    osal_timer_mod(&g_pown_timer, DOUBLE_CLICK_INTERVEL);
    // clear pending
    osal_irq_clear(PMU_IRQ_IRQN);
}

static int pown_init(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    osal_irq_request(PMU_IRQ_IRQN, (osal_irq_handler)pmu_irqn, NULL, NULL, NULL);
    osal_irq_set_priority(PMU_IRQ_IRQN,  irq_prio(PMU_IRQ_IRQN));
    osal_irq_enable(PMU_IRQ_IRQN);
    uapi_pin_set_mode(S_MGPIO21, 0);
    uapi_gpio_set_dir(S_MGPIO21, GPIO_DIRECTION_OUTPUT);
    pown_pwm_init();
    osal_timer_init(&g_pown_timer);
    return 0;
}

void add_pown_testcase(void)
{
    uapi_test_suite_add_function("test_pown_start", "start pown testcase", pown_init);
}