#include "user_task.h"
#include "common_def.h"
#include "osal_debug.h"
#include "cmsis_os2.h"
#include "app_init.h"
#include "gpio.h"
#include "pinctrl.h"
#include "boards.h"

#define TASKS_TEST_TASK_STACK_SIZE    0x1000
#define TASKS_TEST_TASK_PRIO          (osPriority_t)(17)
#define TASKS_TEST_DURATION_MS        1000



static void *task_thread(const char *arg)
{
    unused(arg);
    osal_printk("Hello World DK225\r\n");
    uapi_pin_set_mode(BSP_LED_2, HAL_PIO_FUNC_GPIO);
    uapi_pin_set_mode(BSP_LED_3, HAL_PIO_FUNC_GPIO);
    uapi_pin_set_mode(BSP_LED_4, HAL_PIO_FUNC_GPIO);

    uapi_gpio_set_dir(BSP_LED_2, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_dir(BSP_LED_3, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_dir(BSP_LED_4, GPIO_DIRECTION_OUTPUT);

    // uapi_gpio_set_val(BSP_LED_1, GPIO_LEVEL_LOW);
    uapi_gpio_set_val(BSP_LED_2, GPIO_LEVEL_LOW);
    uapi_gpio_set_val(BSP_LED_3, GPIO_LEVEL_LOW);
    uapi_gpio_set_val(BSP_LED_4, GPIO_LEVEL_LOW);
    while (1) {
        osDelay(TASKS_TEST_DURATION_MS);
        uapi_gpio_toggle(BSP_LED_4);
    }
    return NULL;
}

static void usertask_entry(void)
{
    osThreadAttr_t attr;

    attr.name = "UserTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = TASKS_TEST_TASK_STACK_SIZE;
    attr.priority = TASKS_TEST_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)task_thread, NULL, &attr) == NULL) {
        /* Create task fail. */
    }
}

/* Run the usertask_entry. */
app_run(usertask_entry);
