/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Test serial source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-10-23, Create file. \n
 */

#include <stdint.h>
#include "console.h"
#include "osal_debug.h"
#include "securec.h"
#include "cmsis_os2.h"
#include "tcxo.h"
#include "test_suite_uart.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#endif
#include "implementation/usb_init.h"
#include "memory_config_common.h"
#include "cpu_utils.h"
#include "gadget/usbd_acm.h"


#define SERIAL_MANUFACTURER  { 'H', 0, 'H', 0, 'H', 0, 'H', 0, 'l', 0, 'i', 0, 'c', 0, 'o', 0, 'n', 0 }
#define SERIAL_MANUFACTURER_LEN   20
#define SERIAL_PRODUCT  { 'H', 0, 'H', 0, '6', 0, '6', 0, '6', 0, '6', 0, ' ', 0, 'U', 0, 'S', 0, 'B', 0 }
#define SERIAL_PRODUCT_LEN        22
#define SERIAL_SERIAL   { '2', 0, '0', 0, '2', 0, '0', 0, '0', 0, '6', 0, '2', 0, '4', 0 }
#define SERIAL_SERIAL_LEN         16
#define SERIAL_RET_ERR            (-1)
#define SERIAL_DELAY_MS           (1000)
#define FRAME_DOWNLOAD_OFFSET  0x101000
#define CRC_VERIFY_BUFFER_LEN  25
#define SERIAL_RECV_DATA_MAX_LEN  64
#define USB_RECV_DATA_FAIL_DELAY  50
#define USB_TOGGLE_TASK_SCHEDULE  100
#define USB_SERIAL_INIT_DELAY     2000
#define TEST_STR_LEN              (sizeof(g_test_suite) - 1)
#define TEST_BREAK_LEN            (5)

static char g_usb_serial_recv_data[SERIAL_RECV_DATA_MAX_LEN];
static char g_test_suite[] = "test_suite";

char g_serial_test_str[] = "serial send seccuse\n";

int usb_serial_init(void)
{
    int ret;
    const char manufacturer[SERIAL_MANUFACTURER_LEN] = SERIAL_MANUFACTURER;
    struct device_string str_manufacturer = {
        .str = manufacturer,
        .len = SERIAL_MANUFACTURER_LEN
    };

    const char product[SERIAL_PRODUCT_LEN] = SERIAL_PRODUCT;
    struct device_string str_product = {
        .str = product,
        .len = SERIAL_PRODUCT_LEN
    };

    const char serial[SERIAL_SERIAL_LEN] = SERIAL_SERIAL;
    struct device_string str_serial_number = {
        .str = serial,
        .len = SERIAL_SERIAL_LEN
    };

    struct device_id dev_id = {
        .vendor_id = 0x1111,
        .product_id = 0xd,
        .release_num = 0x0119
    };

    ret = usbd_set_device_info(DEV_SERIAL, &str_manufacturer, &str_product, &str_serial_number, dev_id);
    if (ret != 0) {
        osal_printk("set device info fail!\n");
        return -1;
    }

    ret = usb_init(DEVICE, DEV_SERIAL);
    if (ret != 0) {
        osal_printk("usb_init failed!\n");
        return -1;
    }
    osDelay(USB_SERIAL_INIT_DELAY);
    usb_serial_write(0, g_serial_test_str, sizeof(g_serial_test_str) - 1);
    test_suite_switch_serial_mode(TEST_USB_SERIAL);
    osal_printk("switch_to_usb_serial_mode\n");
    return 0;
}

int8_t usb_serial_recv_data(void)
{
    usb_serial_ioctl(0, CONSOLE_CMD_RD_BLOCK_SERIAL, 1);
    for (;;) {
        uint32_t recv_len = usb_serial_read(0, g_usb_serial_recv_data, SERIAL_RECV_DATA_MAX_LEN);
        if (recv_len <= 0) {
            osDelay(USB_RECV_DATA_FAIL_DELAY);
            printf("serial recv fail\n");
            continue;
        }
        printf("serial recv: %s\n", g_usb_serial_recv_data);

        usb_serial_write(0, g_usb_serial_recv_data, recv_len);
#ifdef TEST_SUITE
        if (recv_len > TEST_STR_LEN && memcmp(g_usb_serial_recv_data, g_test_suite, TEST_STR_LEN) == 0) {
            test_suite_uart_rx_callback(&g_usb_serial_recv_data[TEST_STR_LEN + 1], recv_len - TEST_STR_LEN - 1, 0);
            osDelay(USB_TOGGLE_TASK_SCHEDULE);
            continue;
        }
#endif
        if (memcmp(g_usb_serial_recv_data, "break", TEST_BREAK_LEN) == 0) {
            break;
        }
    }
    return -1;
}