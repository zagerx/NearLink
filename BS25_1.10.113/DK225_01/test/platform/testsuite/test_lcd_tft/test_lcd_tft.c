/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd tft testcase \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-18， Create file. \n
 */

#include "std_def.h"
#include "stdlib.h"

#include "lcd_draw.h"
#include "lcd_tft.h"
#include "lcd_picture.h"
#include "test_suite.h"

#include "cmsis_os2.h"
#include "soc_osal.h"
#include "flash.h"

#define RECTANGLE_X1  10
#define RECTANGLE_Y1  10
#define RECTANGLE_X2  50
#define RECTANGLE_Y2  50
#define LCD_STACK_SIZE 0x600
#define LCD_STACK_PRI  13
#define LCD_GIF_WAIT_FOREVER  0xFFFFFFFF
#define LOOP_FOREVER  0

#define LCD_MSG_QUEUE_LEN      5
#define LCD_MSG_QUEUE_MAX_SIZE 10U
#define LCD_MSG_QUEUE_WAITE_MS 10
#define LCD_SHOW_GIF_DELAY_MS 20
#define LCD_GIF_WELCOME_FRAME_NUM 10

static int lcd_stack(void *para);

typedef struct {
    osThreadAttr_t attr;
    osThreadFunc_t func;
} app_task_definition_t;

typedef struct {
    uint32_t first_fram_addr;
    uint32_t delay_ms;
    uint8_t  frame_num;
    uint8_t  current_fram;
} lcd_image_t;

typedef enum {
    LCD_DO_NOTHING,
    LCD_SHOW_GIF,
    LCD_SHOW_PICTRUE,
} lcd_command_type_t;

typedef struct {
    lcd_command_type_t command_type;
    uint8_t image_id;
} lcd_command_t;

typedef enum {
    LCD_GIF_WELCOME,
    LCD_PIC_LOGO,
} lcd_image_name_t;

unsigned long g_lcd_msgqueue_id;
lcd_image_t g_images[] = {
    {FLASH_START, LCD_SHOW_GIF_DELAY_MS, LCD_GIF_WELCOME_FRAME_NUM, 0},
    {(uint32_t)&g_test_image, LCD_GIF_WAIT_FOREVER, 1, 0}
};

static void init_lcd_thread(void)
{
    if (osal_msg_queue_create("lcd_que", LCD_MSG_QUEUE_LEN, \
        &g_lcd_msgqueue_id, 0, sizeof(lcd_command_t)) != OSAL_SUCCESS) {
        print("lcd message queue create failed!\n");
        return;
    }
    osal_task *task = osal_kthread_create(lcd_stack, NULL, "lcd_task", LCD_STACK_SIZE);
    osal_kthread_set_priority(task, LCD_STACK_PRI);
}

static void lcd_show_gif(uint32_t gif_id, bool fresh)
{
    lcd_image_t *gif = &g_images[gif_id];
    if (fresh) {
        gif->current_fram = 0;
    }
    //  Displaying an image takes about 14 ms.
    lcd_show_pictrue((uint8_t *)(gif->first_fram_addr + gif->current_fram * IMAGE_ARRAY_LENGTH), IMAGE_ARRAY_LENGTH);
    gif->current_fram += 1;
    if (gif->current_fram == gif->frame_num) {
        gif->current_fram = 0;
    }
}

static int lcd_stack(void *para)
{
    lcd_command_t msg;
    uint32_t msg_size = sizeof(lcd_command_t);
    lcd_command_t current_command = {0};
    lcd_image_t current_image = {0};
    uint32_t msg_queue_waite_ms = LCD_GIF_WAIT_FOREVER;
    bool is_fresh = true;
    UNUSED(para);
    for (;;) {
        int ret = osal_msg_queue_read_copy(g_lcd_msgqueue_id, &msg, &msg_size, msg_queue_waite_ms);
        if (ret == OSAL_SUCCESS) {
            current_command = msg;
            current_image = g_images[current_command.image_id];
            is_fresh = true;
            msg_queue_waite_ms = current_image.delay_ms;
        }
        switch (current_command.command_type) {
            case LCD_DO_NOTHING:
                print("show nothing\n");
                break;
            case LCD_SHOW_GIF:
                print("show gif\n");
                lcd_show_gif(current_command.image_id, is_fresh);
                is_fresh = false;
                break;
            case LCD_SHOW_PICTRUE:
                print("show pictrue\n");
                lcd_show_pictrue((uint8_t *)(current_image.first_fram_addr), IMAGE_ARRAY_LENGTH);
                break;
        }
    }
    return 0;
}

int32_t test_lcd_tft(int32_t argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    lcd_tft_init();
    init_lcd_thread();
    print("lcd init done\r\n");
    lcd_draw_rec(RECTANGLE_X1, RECTANGLE_Y1, RECTANGLE_X2, RECTANGLE_Y2, RED);
    return 0;
}

int32_t test_lcd_tft_showimage(int32_t argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint8_t image_id = (uint8_t)strtol(argv[0], NULL, 0);
    uint8_t image_type = (uint8_t)strtol(argv[1], NULL, 0);
    lcd_command_t cmd = {
        .image_id = image_id,
        .command_type = image_type,
    };
    osal_msg_queue_write_copy(g_lcd_msgqueue_id, &cmd, sizeof(lcd_command_t), 0);
    return 0;
}

void add_lcd_tft_testcase(void)
{
    uapi_test_suite_add_function("test_lcd_tft", "test_lcd_tft", test_lcd_tft);
    uapi_test_suite_add_function(
        "test_lcd_tft_showimage", "test_lcd_tft_showimage <image_id> <image_type>", test_lcd_tft_showimage);
}