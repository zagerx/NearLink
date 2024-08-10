/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test i2c source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-11, Create file. \n
 */

#include <stdint.h>
#include "common_def.h"
#include "securec.h"
#include "soc_osal.h"
#include "tcxo.h"
#include "i2c.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "test_i2c.h"

#define I2C_TEST_MAX_NUM                 4
#define TEST_I2C_INIT_ARGV_COUNT         4
#define TEST_I2C_DEINIT_ARGV_COUNT       1
#define TEST_I2C_SET_BAUDRATE_ARGV_COUNT 2
#define TEST_I2C_WRITE_READ_ARGV_COUNT   4
#define TEST_I2C_READ_REG_ARGV_COUNT     4
#if defined(CONFIG_I2C_SUPPORT_IN_CHIP_LOOPBACK)
#define TEST_I2C_LOOP_INIT_ARGV_COUNT    7
#define TEST_I2C_LOOP_INIT_WITH_IRQ      1
#define TEST_I2C_LOOP_BACK_ARGV_COUNT    6
#define TEST_I2C_LOOP_DEINIT_ARGV_COUNT  2
#endif

typedef struct {
    uint8_t slave_flag;
    uint8_t reserved[3];
} i2c_test_ctrl_info;

typedef enum {
    I2C_MODE_PULL,
    I2C_MODE_IT,
    I2C_MODE_DMA,
    I2C_MODE_MAX_BUTT
} i2c_op_mode;

typedef enum {
    I2C_MAP_WRITE = 1,
    I2C_MAP_READ,
    I2C_MAP_WRITEREAD,
    I2C_MAP_MAX_BUTT
} i2c_op_map;

i2c_test_ctrl_info g_i2c_test_info[I2C_TEST_MAX_NUM];
static i2c_data_t g_rtx_data = {0};
#if defined(CONFIG_I2C_SUPPORT_IN_CHIP_LOOPBACK)
static bool g_i2c_init = false;
#endif
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
    i2c_port_test_i2c_init_pin();

    if (slave_flag == true) {
#if defined(CONFIG_I2C_SUPPORT_SLAVE) && (CONFIG_I2C_SUPPORT_SLAVE == 1)
        osal_printk("config i2c_bus %d as slave bus\r\n", i2c_bus_id);
        ret = uapi_i2c_slave_init(i2c_bus_id, baudrate, dev_addr);
#else
        unused(dev_addr);
        osal_printk("not support slave! \r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
#endif
    } else {
        osal_printk("config i2c_bus %d as master bus\r\n", i2c_bus_id);
        ret = uapi_i2c_master_init(i2c_bus_id, baudrate, hscode);
    }

    if (ret != ERRCODE_SUCC) {
        osal_printk("init fail %x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
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
        return (int)ret;
    }

    return TEST_SUITE_OK;
}

static int test_i2c_set_baudrate(int argc, char *argv[])
{
    if (argc != TEST_I2C_SET_BAUDRATE_ARGV_COUNT) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t param_index = 0;
    uint32_t i2c_bus_id = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t baudrate = (uint32_t)strtol(argv[param_index], NULL, 0);

    errcode_t ret = uapi_i2c_set_baudrate(i2c_bus_id, baudrate);
    if (ret != ERRCODE_SUCC) {
        return (int)ret;
    }

    return TEST_SUITE_OK;
}

static void test_i2c_mode_print(i2c_data_t *data)
{
    unused(data);
#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
#if defined(CONFIG_I2C_SUPPORT_POLL_AND_DMA_AUTO_SWITCH) && (CONFIG_I2C_SUPPORT_POLL_AND_DMA_AUTO_SWITCH == 1)
    if (data->send_len > CONFIG_I2C_POLL_AND_DMA_AUTO_SWITCH_THRESHOLD ||
        data->receive_len > CONFIG_I2C_POLL_AND_DMA_AUTO_SWITCH_THRESHOLD) {
        osal_printk("i2c transfer mode : DMA\r\n");
    } else {
        osal_printk("i2c transfer mode : POLL\r\n");
    }
#endif  /* CONFIG_I2C_SUPPORT_DMA */
#endif  /* CONFIG_I2C_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */
}

static int test_i2c_write_read_pull_master(i2c_bus_t i2c_bus_id, i2c_data_t *data, int32_t dev_addr, uint32_t op_map)
{
    errcode_t ret = ERRCODE_SUCC;
    uint64_t time_before = 0;
    switch (op_map) {
        case I2C_MAP_WRITE:
            osal_printk("i2c master write\r\n");
            test_i2c_mode_print(data);
            time_before = uapi_tcxo_get_us();
            ret = uapi_i2c_master_write(i2c_bus_id, dev_addr, data);
            osal_printk("uapi i2c master write time: %uus, length: %u\r\n",
                        (uint32_t)(uapi_tcxo_get_us() - time_before), data->send_len);
            break;
        case I2C_MAP_READ:
            osal_printk("i2c master read\r\n");
            test_i2c_mode_print(data);
            time_before = uapi_tcxo_get_us();
            ret = uapi_i2c_master_read(i2c_bus_id, dev_addr, data);
            osal_printk("uapi i2c master read time: %uus, length: %u\r\n",
                        (uint32_t)(uapi_tcxo_get_us() - time_before), data->receive_len);
            break;
        case I2C_MAP_WRITEREAD:
            osal_printk("i2c master writeread\r\n");
            test_i2c_mode_print(data);
            time_before = uapi_tcxo_get_us();
            ret = uapi_i2c_master_writeread(i2c_bus_id, dev_addr, data);
            osal_printk("uapi i2c master writeread time: %uus, send length: %u, read length: %u\r\n",
                        (uint32_t)(uapi_tcxo_get_us() - time_before), data->send_len, data->receive_len);
            break;
        default:
            ret = ERRCODE_FAIL;
            break;
    }
    return ret;
}

#if defined(CONFIG_I2C_SUPPORT_SLAVE) && (CONFIG_I2C_SUPPORT_SLAVE == 1)
static int test_i2c_write_read_pull_slave(i2c_bus_t i2c_bus_id, i2c_data_t *data, uint32_t op_map)
{
    errcode_t ret = ERRCODE_SUCC;
    uint64_t time_before = 0;
    switch (op_map) {
        case I2C_MAP_WRITE:
            osal_printk("i2c slave write\r\n");
            test_i2c_mode_print(data);
            time_before = uapi_tcxo_get_us();
            ret = uapi_i2c_slave_write(i2c_bus_id, data);
            osal_printk("uapi i2c slave write time: %uus, length: %u\r\n",
                        (uint32_t)(uapi_tcxo_get_us() - time_before), data->send_len);
            break;
        case I2C_MAP_READ:
            osal_printk("i2c slave read\r\n");
            test_i2c_mode_print(data);
            time_before = uapi_tcxo_get_us();
            ret = uapi_i2c_slave_read(i2c_bus_id, data);
            osal_printk("uapi i2c slave read time: %uus, length: %u\r\n",
                        (uint32_t)(uapi_tcxo_get_us() - time_before), data->receive_len);
            break;
        default:
            ret = ERRCODE_FAIL;
            break;
    }
    return ret;
}
#endif

static int test_i2c_write_read_pull(i2c_bus_t i2c_bus_id, i2c_data_t *data, int32_t dev_addr, uint32_t op_map)
{
    errcode_t ret = ERRCODE_SUCC;
    if (g_i2c_test_info[i2c_bus_id].slave_flag == false) {
        ret = test_i2c_write_read_pull_master(i2c_bus_id, data, dev_addr, op_map);
    } else {
#if defined(CONFIG_I2C_SUPPORT_SLAVE) && (CONFIG_I2C_SUPPORT_SLAVE == 1)
        ret = test_i2c_write_read_pull_slave(i2c_bus_id, data, op_map);
#else
        osal_printk("not support slave! \r\n");
        ret = ERRCODE_FAIL;
#endif
    }

#if !defined(CONFIG_I2C_SUPPORT_IN_CHIP_LOOPBACK)
    if (op_map == I2C_MAP_READ) {
        for (uint32_t i = 0; i < data->receive_len; i++) {
            osal_printk("receive data is %x\r\n", data->receive_buf[i]);
        }
    }
#endif
    return (int)ret;
}

static int test_i2c_write_read_dispatch(
    uint32_t i2c_bus_id, i2c_data_t *data, int32_t dev_addr, uint32_t op_map, uint32_t mode)
{
    unused(mode);
    i2c_bus_t bus_id = (i2c_bus_t)i2c_bus_id;

    return test_i2c_write_read_pull(bus_id, data, dev_addr, op_map);
}

static void test_i2c_data_fill(uint8_t *data, uint32_t len)
{
    for (uint32_t loop = 0; loop < len; loop++) {
        data[loop] = (loop & 0xFF);
    }
}

static errcode_t test_i2c_data_check(uint8_t *data, uint32_t len)
{
    for (uint32_t loop = 0; loop < len; loop++) {
        if (data[loop] != (loop & 0xFF)) {
            return ERRCODE_FAIL;
        }
    }
    return ERRCODE_SUCC;
}

static errcode_t test_i2c_write_read_ext(uint32_t i2c_bus_id, uint32_t write_len, uint32_t read_len,
    uint32_t mode, uint32_t dev_addr)
{
    errcode_t ret;
    uint32_t op_map = 0;

    if (write_len != 0) {
        g_rtx_data.send_buf = (uint8_t *)osal_kmalloc(write_len, 0);
        if (g_rtx_data.send_buf == NULL) {
            ret = ERRCODE_MALLOC;
            goto exit;
        }
        test_i2c_data_fill(g_rtx_data.send_buf, write_len);
        g_rtx_data.send_len = write_len;
        op_map = op_map | I2C_MAP_WRITE;
    }

    if (read_len != 0) {
        g_rtx_data.receive_buf = (uint8_t *)osal_kmalloc(read_len, 0);
        if (g_rtx_data.receive_buf == NULL) {
            ret = ERRCODE_MALLOC;
            goto exit;
        }
        if (memset_s(g_rtx_data.receive_buf, read_len, 0, read_len) != EOK) {
            ret = ERRCODE_MEMSET;
            goto exit;
        }
        g_rtx_data.receive_len = read_len;
        op_map = op_map | I2C_MAP_READ;
    }

    ret = test_i2c_write_read_dispatch(i2c_bus_id, &g_rtx_data, dev_addr, op_map, mode);
    if (ret != ERRCODE_SUCC) { goto exit; }
#if !defined(CONFIG_I2C_SUPPORT_IN_CHIP_LOOPBACK)
    if ((op_map & I2C_MAP_READ) == I2C_MAP_READ) {
        ret = test_i2c_data_check(g_rtx_data.receive_buf, g_rtx_data.receive_len);
        goto exit;
    }

    ret = ERRCODE_SUCC;
#endif

#if defined(CONFIG_I2C_SUPPORT_IN_CHIP_LOOPBACK)
    return ERRCODE_SUCC;
#endif

exit:
    if (g_rtx_data.send_buf != NULL) {
        osal_kfree(g_rtx_data.send_buf);
        g_rtx_data.send_buf = NULL;
    }
    if (g_rtx_data.receive_buf != NULL) {
        osal_kfree(g_rtx_data.receive_buf);
        g_rtx_data.receive_buf = NULL;
    }
    return ret;
}

static int test_i2c_write_read(int argc, char *argv[])
{
    if (argc < TEST_I2C_WRITE_READ_ARGV_COUNT) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    errcode_t ret;

    uint32_t param_index = 0;
    uint32_t i2c_bus_id = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t write_len = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t read_len = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t mode = (uint32_t)strtol(argv[param_index], NULL, 0);
    uint32_t dev_addr = 0;

    if (g_i2c_test_info[i2c_bus_id].slave_flag == false) {
        param_index++;
        dev_addr = (uint32_t)strtol(argv[param_index], NULL, 0);
    }

    ret = test_i2c_write_read_ext(i2c_bus_id, write_len, read_len, mode, dev_addr);
    if (ret != ERRCODE_SUCC) {
        osal_printk("test_i2c_write_read fail, ret=[%x]. \r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
static int test_i2c_set_dma_mode(int argc, char *argv[])
{
    unused(argc);
    uint32_t param_index = 0;
    uint32_t bus = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t enable = (uint32_t)strtol(argv[param_index], NULL, 0);
    if (uapi_i2c_set_dma_mode(bus, enable) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}
#endif  /* CONFIG_I2C_SUPPORT_DMA */
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
static int test_i2c_set_irq_mode(int argc, char *argv[])
{
    unused(argc);
    uint32_t param_index = 0;
    uint32_t bus = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    uint32_t enable = (uint32_t)strtol(argv[param_index], NULL, 0);
    if (uapi_i2c_set_irq_mode(bus, enable) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}
#endif  /* CONFIG_I2C_SUPPORT_INT */

#if defined(CONFIG_I2C_SUPPORT_IN_CHIP_LOOPBACK)
static int test_i2c_loopback_init(int argc, char *argv[])
{
    errcode_t ret = ERRCODE_SUCC;
    if (argc != TEST_I2C_LOOP_INIT_ARGV_COUNT) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint32_t param_index = 0;
    uint32_t master_bus_id = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t m_slave_flag = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t slave_bus_id = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t s_slave_flag = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t baudrate = (uint32_t)strtol(argv[param_index++], NULL, 0);
    /* hscode dev_addr use same param */
    uint8_t hscode = (uint8_t)strtol(argv[param_index++], NULL, 0);
    uint16_t dev_addr = (uint16_t)strtol(argv[param_index], NULL, 0);
    if (!g_i2c_init) {
        g_i2c_test_info[master_bus_id].slave_flag = m_slave_flag;
        g_i2c_test_info[slave_bus_id].slave_flag = s_slave_flag;
        i2c_port_test_i2c_init_pin();
        ret |= uapi_i2c_master_init(master_bus_id, baudrate, hscode);
        ret |= uapi_i2c_slave_init(slave_bus_id, baudrate, dev_addr);
        ret |= uapi_i2c_set_irq_mode(master_bus_id, TEST_I2C_LOOP_INIT_WITH_IRQ);
        ret |= uapi_i2c_set_irq_mode(slave_bus_id, TEST_I2C_LOOP_INIT_WITH_IRQ);
        g_i2c_init = true;
    }
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int test_i2c_loopback(int argc, char *argv[])
{
    if (argc != TEST_I2C_LOOP_BACK_ARGV_COUNT) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (!g_i2c_init) {
        return TEST_SUITE_TEST_FAILED;
    }
    errcode_t ret = ERRCODE_SUCC;

    uint32_t param_index = 0;
    uint32_t i2c_master_id = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t write_len = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint8_t hscode = (uint8_t)strtol(argv[param_index++], NULL, 0);
    uint32_t i2c_slave_id = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t read_len = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint16_t dev_addr = (uint16_t)strtol(argv[param_index], NULL, 0);

    ret |= test_i2c_write_read_ext(i2c_slave_id, 0, read_len, 0, hscode);
    ret |= test_i2c_write_read_ext(i2c_master_id, write_len, 0, 0, dev_addr);
    ret |= test_i2c_data_check(g_rtx_data.receive_buf, g_rtx_data.receive_len);
    uapi_tcxo_delay_ms(1ULL);
    if (ret != ERRCODE_SUCC) {
        if (g_rtx_data.send_buf != NULL) {
            free(g_rtx_data.send_buf);
            g_rtx_data.send_buf = NULL;
        }
        if (g_rtx_data.receive_buf != NULL) {
            free(g_rtx_data.receive_buf);
            g_rtx_data.receive_buf = NULL;
        }
        return TEST_SUITE_TEST_FAILED;
    }
    for (uint32_t i = 0; i < g_rtx_data.receive_len; i++) {
        osal_printk("receive data is %x\r\n", g_rtx_data.receive_buf[i]);
    }
    if (g_rtx_data.send_buf != NULL) {
        free(g_rtx_data.send_buf);
        g_rtx_data.send_buf = NULL;
    }
    if (g_rtx_data.receive_buf != NULL) {
        free(g_rtx_data.receive_buf);
        g_rtx_data.receive_buf = NULL;
    }

    return TEST_SUITE_OK;
}

static int test_i2c_loopback_deinit(int argc, char *argv[])
{
    unused(argc);
    if (argc != TEST_I2C_LOOP_DEINIT_ARGV_COUNT) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    errcode_t ret = ERRCODE_SUCC;
    uint32_t param_index = 0;
    uint32_t master_bus_id = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t slave_bus_id = (uint32_t)strtol(argv[param_index], NULL, 0);
    ret |= uapi_i2c_deinit(master_bus_id);
    ret |= uapi_i2c_deinit(slave_bus_id);
    g_i2c_init = false;
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}
#endif

void add_i2c_test_case(void)
{
#if defined(CONFIG_I2C_SUPPORT_IN_CHIP_LOOPBACK)
    uapi_test_suite_add_function("test_i2c_loopback_init", "Params: <master_bus_id>, <master_slave_flag>,\
        <slave_bus_id>, <slave_slave_flag>, <baudrate>, <hscode>, <dev_addr>", test_i2c_loopback_init);
    uapi_test_suite_add_function("test_i2c_loop", "Params: <master_bus_id>, <write_len>, <hscode>, <slave_bus_id>,\
        <read_len>, <dev_addr>", test_i2c_loopback);
    uapi_test_suite_add_function("test_i2c_loopback_deinit", "Params: <master_bus_id>,<slave_bus_id>",
        test_i2c_loopback_deinit);
#endif
    uapi_test_suite_add_function("test_i2c_init", "Params: <slave_flag>, <i2c_bus_id>, <baudrate>, <hscode/dev_addr>",
        test_i2c_init);
    uapi_test_suite_add_function("test_i2c_deinit", "Params: <i2c_bus_id>",
        test_i2c_deinit);
    uapi_test_suite_add_function("test_i2c_set_baudrate", "Params: <i2c_bus_id>, <baudrate>",
        test_i2c_set_baudrate);
    uapi_test_suite_add_function("test_i2c_write_read",
        "Params: <i2c_bus_id>, <write_len>, <read_len>, <mode>, <dev_addr>", test_i2c_write_read);
#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
    uapi_test_suite_add_function("test_i2c_set_dma_mode", "Params: <i2c_bus_id>, <enable>", test_i2c_set_dma_mode);
#endif  /* CONFIG_I2C_SUPPORT_DMA */
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    uapi_test_suite_add_function("test_i2c_set_irq_mode", "Params: <i2c_bus_id>, <enable>", test_i2c_set_irq_mode);
#endif  /* CONFIG_I2C_SUPPORT_INT */
}
