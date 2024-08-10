/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides i2c port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15， Create file. \n
 */

#include "i2c_porting.h"
#include "pinctrl.h"
#include "hal_i2c.h"
#include "hal_i2c_v100.h"
#include "hal_i2c_v100_comm.h"
#include "osal_interrupt.h"
#include "chip_core_irq.h"
#include "pm_clock.h"

#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
#include "dma_porting.h"
#endif

#define BUS_CLOCK_TIME_32M 32000000UL
#define BUS_CLOCK_TIME_16M 16000000UL

typedef void (*i2c_porting_irq_handler)(void);

typedef struct i2c_irq_handler {
    uint32_t irq_id;
    i2c_porting_irq_handler irq_handler;
} i2c_irq_handler_t;


uintptr_t g_i2c_base_addrs[I2C_BUS_MAX_NUM] = {
    (uintptr_t)I2C_BUS_0_BASE_ADDR,
    (uintptr_t)I2C_BUS_1_BASE_ADDR,
    (uintptr_t)I2C_BUS_2_BASE_ADDR,
    (uintptr_t)I2C_BUS_3_BASE_ADDR,
    (uintptr_t)I2C_BUS_4_BASE_ADDR,
};

uintptr_t i2c_porting_base_addr_get(i2c_bus_t bus)
{
    return g_i2c_base_addrs[bus];
}

static void irq_i2c0_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_0);
    osal_irq_clear(I2C_0_IRQN);
}

static void irq_i2c1_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_1);
    osal_irq_clear(I2C_1_IRQN);
}

static void irq_i2c2_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_2);
    osal_irq_clear(I2C_2_IRQN);
}

static void irq_i2c3_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_3);
    osal_irq_clear(I2C_3_IRQN);
}

static void irq_i2c4_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_4);
    osal_irq_clear(I2C_4_IRQN);
}

static i2c_irq_handler_t g_i2c_irq_id[I2C_BUS_MAX_NUM] = {
    {
        I2C_0_IRQN,
        irq_i2c0_handler,
    },
    {
        I2C_1_IRQN,
        irq_i2c1_handler,
    },
    {
        I2C_2_IRQN,
        irq_i2c2_handler,
    },
    {
        I2C_3_IRQN,
        irq_i2c3_handler,
    },
    {
        I2C_4_IRQN,
        irq_i2c4_handler,
    },
};

uint32_t i2c_port_get_clock_value(i2c_bus_t bus)
{
    if (bus >= I2C_BUS_MAX_NUM) {
        return 0;
    }
    return BUS_CLOCK_TIME_16M;
}

void i2c_port_register_irq(i2c_bus_t bus)
{
    i2c_irq_handler_t irq = g_i2c_irq_id[bus];
    osal_irq_request(irq.irq_id, (osal_irq_handler)irq.irq_handler, NULL, NULL, NULL);
    osal_irq_enable(irq.irq_id);
}

void i2c_port_unregister_irq(i2c_bus_t bus)
{
    i2c_irq_handler_t irq = g_i2c_irq_id[bus];
    osal_irq_free(irq.irq_id, NULL);
}

void i2c_port_test_i2c_init_pin(void)
{
    // I2c1 pinmux
    uapi_pin_set_mode(S_MGPIO6, HAL_PIO_FUNC_I2C1_M3);
    uapi_pin_set_mode(S_MGPIO7, HAL_PIO_FUNC_I2C1_M3);
    // I2c2 pinmux
    uapi_pin_set_mode(S_MGPIO8, HAL_PIO_FUNC_I2C2_M3);
    uapi_pin_set_mode(S_MGPIO9, HAL_PIO_FUNC_I2C2_M3);
}

uint32_t i2c_porting_lock(i2c_bus_t bus)
{
    unused(bus);
    return osal_irq_lock();
}

void i2c_porting_unlock(i2c_bus_t bus, uint32_t irq_sts)
{
    unused(bus);
    osal_irq_restore(irq_sts);
}

void i2c_port_clock_enable(i2c_bus_t bus, bool on)
{
    clock_control_type_t control_type;
    clock_mclken_aperp_type_t aperp_type;

    switch (bus) {
        case I2C_BUS_0:
            aperp_type = CLOCK_APERP_I2C0_CLKEN;
            break;

        case I2C_BUS_1:
            aperp_type = CLOCK_APERP_I2C1_CLKEN;
            break;

        case I2C_BUS_2:
            aperp_type = CLOCK_APERP_I2C2_CLKEN;
            break;

        case I2C_BUS_3:
            aperp_type = CLOCK_APERP_I2C3_CLKEN;
            break;
        case I2C_BUS_4:
            aperp_type = CLOCK_APERP_I2C4_CLKEN;
            break;

        default:
            return;
    }

    if (on) {
        control_type = CLOCK_CONTROL_MCLKEN_ENABLE;
    } else {
        control_type = CLOCK_CONTROL_MCLKEN_DISABLE;
    }

    uapi_clock_control(control_type, aperp_type);
}

#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
uint8_t i2c_port_get_dma_trans_dest_handshaking(i2c_bus_t bus)
{
    switch (bus) {
        case I2C_BUS_0:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_0_TX;
        case I2C_BUS_1:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_1_TX;
        case I2C_BUS_2:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_2_TX;
        case I2C_BUS_3:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_3_TX;
        case I2C_BUS_4:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_4_TX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}

uint8_t i2c_port_get_dma_trans_src_handshaking(i2c_bus_t bus)
{
    switch (bus) {
        case I2C_BUS_0:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_0_RX;
        case I2C_BUS_1:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_1_RX;
        case I2C_BUS_2:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_2_RX;
        case I2C_BUS_3:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_3_RX;
        case I2C_BUS_4:
            return (uint8_t)DMA_HANDSHAKE_I2C_BUS_4_RX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}
#endif