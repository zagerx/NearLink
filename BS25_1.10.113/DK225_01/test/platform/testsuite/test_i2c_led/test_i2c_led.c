/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test i2c_led source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-11, Create file. \n
 */
#include <stdint.h>
#include "common_def.h"
#include "securec.h"
#include "soc_osal.h"
#include "pinctrl.h"
#include "i2c.h"
#include "i2c_porting.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "test_suite_log.h"
#include "test_i2c_led.h"

#define I2C_RGB_LED_ADDR                0x5B
#define DATA_SEND_LEN                   2
#define BREATH_DATA_LEN                 16
#define BREATH_REGS_LEN                 8
#define WHITE_DATA_LEN                  6
#define WHITE_REGS_LEN                  3
#define COLOR_DATA_LEN                  18
#define COLOR_REGS_LEN                  9
#define I2C_TEST_MAX_NUM                4
#define TEST_I2C_INIT_ARGV_COUNT        4
#define TEST_I2C_DEINIT_ARGV_COUNT      1

typedef struct {
    uint8_t slave_flag;
    uint8_t reserved[3];
} i2c_test_ctrl_info;

i2c_test_ctrl_info g_i2c_test_info[I2C_TEST_MAX_NUM];

static int test_i2c_init(int argc, char *argv[])
{
    if (argc != TEST_I2C_INIT_ARGV_COUNT) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t param_index = 0;
    uint32_t slave_flag = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t i2c_bus_id = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t baudrate = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;

    /* hscode dev_addr use same param */
    uint8_t hscode = (uint8_t)strtol(argv[param_index], NULL, 0);
    uint16_t dev_addr = (uint16_t)strtol(argv[param_index], NULL, 0);

    g_i2c_test_info[i2c_bus_id].slave_flag = slave_flag;

    errcode_t ret;
    uapi_pin_set_mode(I2C_1_SCL_PIN, PIN_MODE_2);
    uapi_pin_set_mode(I2C_1_SDA_PIN, PIN_MODE_2);
    uapi_pin_set_mode(I2C_2_SCL_PIN, PIN_MODE_1);
    uapi_pin_set_mode(I2C_2_SDA_PIN, PIN_MODE_1);

    if (slave_flag == true) {
        test_suite_log_stringf("config i2c_bus %d as slave bus\r\n", i2c_bus_id);
        ret = uapi_i2c_slave_init(i2c_bus_id, baudrate, dev_addr);
    } else {
        test_suite_log_stringf("config i2c_bus %d as master bus\r\n", i2c_bus_id);
        ret = uapi_i2c_master_init(i2c_bus_id, baudrate, hscode);
    }

    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return TEST_SUITE_OK;
}

static int test_i2c_deinit(int argc, char *argv[])
{
    if (argc != TEST_I2C_DEINIT_ARGV_COUNT) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t param_index = 0;
    uint32_t i2c_bus_id = (uint32_t)strtol(argv[param_index], NULL, 0);

    errcode_t ret = uapi_i2c_deinit(i2c_bus_id);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int test_i2c_write(int argc, char *argv[])
{
    unused(argc);
    uint32_t param_index = 0;
    uint32_t reg_addr = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t value = (uint32_t)strtol(argv[param_index], NULL, 0);

    i2c_data_t data = { 0 };

    uint8_t tx_buff[DATA_SEND_LEN] = { reg_addr, value };
    uint8_t tx_len = DATA_SEND_LEN;
    data.send_buf = tx_buff;
    data.send_len = tx_len;

    uint16_t dev_addr = I2C_RGB_LED_ADDR;
    uapi_i2c_master_write(I2C_BUS_1, dev_addr, &data);

    return TEST_SUITE_OK;
}

static int test_i2c_read(int argc, char *argv[])
{
    unused(argc);
    uint32_t param_index = 0;
    uint8_t reg_addr = (uint8_t)strtol(argv[param_index], NULL, 0);

    i2c_data_t data = { 0 };

    uint8_t rx_buff;
    uint8_t rx_len = 1;
    data.send_buf = &reg_addr;
    data.send_len = rx_len;
    data.receive_buf = &rx_buff;
    data.receive_len = rx_len;

    uint16_t dev_addr = I2C_RGB_LED_ADDR;
    uapi_i2c_master_write(I2C_BUS_1, dev_addr, &data);
    uapi_i2c_master_read(I2C_BUS_1, dev_addr, &data);
    test_suite_log_stringf("master read buf is %x", rx_buff);

    return TEST_SUITE_OK;
}

int test_i2c_breath_led(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    i2c_data_t data = { 0 };

    uint8_t data_buff[BREATH_DATA_LEN] = { 0x12, 0x00, 0x13, 0x00, 0x14, 0x3F, 0x04, 0x03, 0x05, 0x0F, 0x15, 0x1B,
                                           0x16, 0x12, 0x11, 0x83 };
    uint8_t tx_buff[DATA_SEND_LEN] = { 0, 0 };
    uint8_t tx_len = DATA_SEND_LEN;
    data.send_buf = tx_buff;
    data.send_len = tx_len;
    errcode_t ret = ERRCODE_FAIL;
    uint16_t dev_addr = I2C_RGB_LED_ADDR;
    for (int i = 0; i < BREATH_REGS_LEN; i++) {
        tx_buff[0] = data_buff[DATA_SEND_LEN * i];
        tx_buff[1] = data_buff[DATA_SEND_LEN * i + 1];
        ret = uapi_i2c_master_write(I2C_BUS_1, dev_addr, &data);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("ERROR CODE IS 0x%x\r\n", ret);
            return ret;
        }
    }

    return TEST_SUITE_OK;
}

int test_i2c_white_led(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    i2c_data_t data = { 0 };

    uint8_t data_buff[WHITE_DATA_LEN] = { 0x11, 0x10, 0x02, 0x00, 0x03, 0x00 };
    uint8_t tx_buff[DATA_SEND_LEN] = { 0, 0 };
    uint8_t tx_len = DATA_SEND_LEN;
    data.send_buf = tx_buff;
    data.send_len = tx_len;
    errcode_t ret = ERRCODE_FAIL;
    uint16_t dev_addr = I2C_RGB_LED_ADDR;
    for (int i = 0; i < WHITE_REGS_LEN; i++) {
        tx_buff[0] = data_buff[DATA_SEND_LEN * i];
        tx_buff[1] = data_buff[DATA_SEND_LEN * i + 1];
        ret = uapi_i2c_master_write(I2C_BUS_1, dev_addr, &data);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("ERROR CODE IS 0x%x\r\n", ret);
            return ret;
        }
    }

    return TEST_SUITE_OK;
}

int test_i2c_colourful_led(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    i2c_data_t data = { 0 };

    uint8_t data_buff[COLOR_DATA_LEN] = { 0x12, 0x00, 0x13, 0x00, 0x11, 0x03, 0x20, 0x60, 0x21, 0x49, 0x22, 0x2a,
                                          0x23, 0xff, 0x24, 0xdb, 0x25, 0x32 };
    uint8_t tx_buff[DATA_SEND_LEN] = { 0, 0 };
    uint8_t tx_len = DATA_SEND_LEN;
    data.send_buf = tx_buff;
    data.send_len = tx_len;
    errcode_t ret = ERRCODE_FAIL;
    uint16_t dev_addr = I2C_RGB_LED_ADDR;
    for (int i = 0; i < COLOR_REGS_LEN; i++) {
        tx_buff[0] = data_buff[DATA_SEND_LEN * i];
        tx_buff[1] = data_buff[DATA_SEND_LEN * i + 1];
        ret = uapi_i2c_master_write(I2C_BUS_1, dev_addr, &data);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("ERROR CODE IS 0x%x\r\n", ret);
            return ret;
        }
    }

    return TEST_SUITE_OK;
}

void add_i2c_led_test_case(void)
{
    uapi_test_suite_add_function(
        "test_i2c_led_init", "Params: <slave_flag>, <i2c_bus_id>, <baudrate>, <hscode/dev_addr>",  test_i2c_init);
    uapi_test_suite_add_function("test_i2c_led_deinit", "Params: <i2c_bus_id>", test_i2c_deinit);
    uapi_test_suite_add_function("test_i2c_write", "Params: <dev_addr>, <data>", test_i2c_write);
    uapi_test_suite_add_function("test_i2c_read", "Params: <dev_addr>", test_i2c_read);
    uapi_test_suite_add_function("test_i2c_white_led", "Start the led with white light", test_i2c_white_led);
    uapi_test_suite_add_function(
        "test_i2c_colourful_led", "Start the led with colourful light", test_i2c_colourful_led);
    uapi_test_suite_add_function(
        "test_i2c_breath_led", "Start the led which can shine in a smooth frequence", test_i2c_breath_led);
}