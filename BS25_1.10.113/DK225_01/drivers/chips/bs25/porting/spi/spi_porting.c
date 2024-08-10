/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides spi port UT \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-18， Create file. \n
 */
#include "hal_spi_v100_regs_def.h"
#include "hal_spi_v100_regs_op.h"
#include "hal_spi_v100.h"
#include "hal_spi.h"
#include "osal_interrupt.h"
#include "pinctrl.h"
#include "lpm_dev_ops.h"
#include "chip_io.h"
#include "std_def.h"
#include "platform_core.h"
#include "osal_interrupt.h"
#include "arch_port.h"
#include "pm_clock.h"
#include "pm_lpc.h"
#include "spi_porting.h"

/** -----------------------------------------------------
 *          Reg Bit fild Max and Shift Value
 * ---------------------------------------------------- */
 /**
  * @brief  CTRLR0 : Control register 0.
  */
#define HAL_SPI_CTRLR0_REG_MAX       0xFFFFFFFF
#define HAL_SPI_CE_LIN_TOGGLE_ENABLE (BIT(24))

/**
 * @brief  FRF : Frame format.
 */
#define HAL_SPI_FRAME_FORMAT_MAX   0x03
#define HAL_SPI_FRAME_FORMAT_SHIFT 0x15

/**
 * @brief  DFS : Data frame size.
 */
#define HAL_SPI_FRAME_SIZE_MAX   0x1F
#define HAL_SPI_FRAME_SIZE_SHIFT 0x10
#define HAL_SPI_FRAME_SIZE_8     0x07
#define HAL_SPI_FRAME_SIZE_16    0x0F
#define HAL_SPI_FRAME_SIZE_32    0x1F

/**
 * @brief  TMOD : Trans mode.
 */
#define HAL_SPI_TRANS_MODE_MAX    0x03
#define HAL_SPI_TRANS_MODE_SHIFT  0x08
#define HAL_SPI_TRANS_MODE_TXRX   0x00
#define HAL_SPI_TRANS_MODE_TX     0x01
#define HAL_SPI_TRANS_MODE_RX     0x02
#define HAL_SPI_TRANS_MODE_EEPROM 0x03

/**
 * @brief  CLOCK : SCPOL and SCPH.
 */
#define HAL_SPI_CLKS_MODE_MAX   0x03
#define HAL_SPI_CLKS_MODE_SHIFT 0x06

/**
 * @brief  CTRLR1 : Control register 1.
 */
#define HAL_SPI_RECEIVED_DATA_REG_MAX 0xFFFF

/**
 * @brief  SSIENR : SSI enable register.
 */
#define HAL_SPI_ENABLE 0x01

/**
 * @brief  SER : Slave enable register.
 */
#define HAL_SPI_SLAVE_ENABLE_REG_MAX 0xFFFFFFFF

/**
 * @brief  BAUDR : Baud rate select.
 */
#define HAL_SPI_CLK_DIV_REG_MAX 0xFFFF

/**
 * @brief  SR : Status register.
 */
#define HAL_SPI_RX_FIFO_FULL_FLAG      (BIT(4))
#define HAL_SPI_RX_FIFO_NOT_EMPTY_FLAG (BIT(3))
#define HAL_SPI_TX_FIFO_EMPTY_FLAG     (BIT(2))
#define HAL_SPI_TX_FIFO_NOT_FULL_FLAG  (BIT(1))
#define HAL_SPI_BUSY_FLAG              (BIT(0))

/**
 * @brief IMR : Interrupt mask register.
 */
#define HAL_SPI_INTERRUPT_REG_MAX 0x3F


/**
 * @brief  SPI_CTRLR0:  SPI control register.
 */
#define HAL_QSPI_CTRLR0_REG_MAX 0xFFFFFFFF

/**
 * @brief  Wait cycles.
 */
#define HAL_QSPI_WAIT_CYCLE_MAX   0x1F
#define HAL_QSPI_WAIT_CYCLE_SHIFT 0x0B
#define HAL_QSPI_WAIT_CYCLE_6     0x06
#define HAL_QSPI_WAIT_CYCLE_4     0x04
#define HAL_QSPI_WAIT_CYCLE_2     0x02

#define HAL_SPI_RX_SAMPLE_DLY_MAX   0xFF
#define HAL_SPI_RX_SAMPLE_DLY_SHIFT 0

/**
 * @brief  Command length.
 */
#define HAL_QSPI_CMD_LENTH_MAX   0x03
#define HAL_QSPI_CMD_LENTH_SHIFT 0x08
#define HAL_QSPI_CMD_LENTH_8     0x02

/**
 * @brief  Address length.
 */
#define HAL_QSPI_ADDR_LENTH_MAX   0x0F
#define HAL_QSPI_ADDR_LENTH_SHIFT 0x02
#define HAL_QSPI_ADDR_LENTH_24    0x06

/**
 * @brief  Trans type.
 */
#define HAL_QSPI_TRANS_TYPES_MAX          0x03
#define HAL_QSPI_TRANS_TYPES_SHIFT        0x00
#define HAL_QSPI_TRANS_TYPES_CMD_S_ADDR_Q 0x01

#define HAL_SPI_BUS_MAX_NUM (SPI_BUS_MAX_NUMBER)

#define HAL_SPI_DR_REG_SIZE 36

/** -----------------------------------------------------
 *          SPI Register Address
 * ----------------------------------------------------
 */
#define HAL_SPI_BUS_3_DEVICE_MODE_SET_BIT       0x00
#define HAL_SPI_BUS_1_DEVICE_MODE_SET_BIT_MAX   0x01
#define HAL_SPI_BUS_1_DEVICE_MODE_SET_BIT_SHIFT 0x00

#define HAL_QSPI_DMA_CFG        (*(volatile unsigned short *)(0x5C000404))
#define HAL_QSPI_1_DMA_CFG_MASK 0x0FU
#define HAL_QSPI_1_DMA_SEC_CORE 0x05
#define HAL_QSPI_1_DMA_APP_CORE 0x0A
#define HAL_QSPI_1_DMA_DSP_CORE 0x0F
#define HAL_QSPI_2_DMA_CFG_MASK 0xF0U
#define HAL_QSPI_2_DMA_SEC_CORE 0x50
#define HAL_QSPI_2_DMA_APP_CORE 0xA0
#define HAL_QSPI_2_DMA_DSP_CORE 0xF0

#define HAL_QSPI_INT_CFG             (*(volatile unsigned short *)(0x5C000408))
#define HAL_QSPI_1_INT_SEC_CORE_MASK (BIT(0))
#define HAL_QSPI_1_INT_APP_CORE_MASK (BIT(2))
#define HAL_QSPI_2_INT_SEC_CORE_MASK (BIT(4))
#define HAL_QSPI_2_INT_APP_CORE_MASK (BIT(6))

#define HAL_SPI_DATA_FRAME_SIZE_BIT     16
#define HAL_SPI_DATA_FRAME_SIZE_BITFILD 5
#define hal_spi_frame_size_trans_to_frame_bytes(x) (((x) + 1) >> 0x03)

#define HAL_SPI_MINUMUM_CLK_DIV 2
#define HAL_SPI_MAXIMUM_CLK_DIV 65534

#define hal_spi_mhz_to_hz(x) ((x) * 1000000)

#define HAL_SPI_RXDS_EN     BIT(18)
#define HAL_SPI_INST_DDR_EN BIT(17)
#define HAL_SPI_DDR_EN      BIT(16)
#define HAL_SPI_INST_L_POSE 8
#define HAL_SPI_ADDR_L_POSE 2

#define HAL_SPI_RSVD_NONE   0
#define HAL_SPI_RSVD_X8     0
#define HAL_SPI_RSVD_X8_X8  1
#define HAL_SPI_RSVD_X16    0x11
#define HAL_SPI_CLK_DIV_2   2
#define HAL_SPI_CLK_DIV_4   4
#define HAL_SPI_CLK_DIV_20  20

#define SPI_DMA_TX_DATA_LEVEL_4     4
#define QSPI_DMA_TX_DATA_LEVEL_8    8

ssi_regs_t * g_spi_base_addrs[SPI_BUS_MAX_NUM] = {
    (ssi_regs_t *)SPI_BUS_0_BASE_ADDR,
    (ssi_regs_t *)SPI_BUS_1_BASE_ADDR,
    (ssi_regs_t *)SPI_BUS_2_BASE_ADDR,
    (ssi_regs_t *)SPI_BUS_3_BASE_ADDR,
    (ssi_regs_t *)SPI_BUS_4_BASE_ADDR,
    (ssi_regs_t *)SPI_BUS_5_BASE_ADDR,
    (ssi_regs_t *)SPI_BUS_6_BASE_ADDR,
};

#if defined(CONFIG_SPI_SUPPORT_INTERRUPT) && (CONFIG_SPI_SUPPORT_INTERRUPT == 1)
static int irq_spi0_handler(int i, void *p);
static int irq_spi1_handler(int i, void *p);
static int irq_spi2_handler(int i, void *p);
static int irq_spi3_handler(int i, void *p);
static int irq_spi4_handler(int i, void *p);
static int irq_spi5_handler(int i, void *p);
static int irq_spi6_handler(int i, void *p);

typedef struct spi_interrupt {
    core_irq_t irq_num;
    osal_irq_handler irq_func;
}
hal_spi_interrupt_t;

static const hal_spi_interrupt_t g_spi_interrupt_lines[SPI_BUS_MAX_NUMBER] = {
    { SPI_M_S_0_IRQN, irq_spi0_handler },
    { SPI_M_S_1_IRQN, irq_spi1_handler },
    { SPI_M_IRQN, irq_spi2_handler },
    { SPI3_MS_IRQN, irq_spi3_handler },
    { SPI4_S_IRQN, irq_spi4_handler },
    { QSPI0_2CS_IRQN, irq_spi5_handler },
    { QSPI1_2CS_IRQN, irq_spi6_handler },
};

static int irq_spi0_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_spi_v100_irq_handler(SPI_BUS_0);
    osal_irq_clear(g_spi_interrupt_lines[SPI_BUS_0].irq_num);
    return 0;
}

static int irq_spi1_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_spi_v100_irq_handler(SPI_BUS_1);
    osal_irq_clear(g_spi_interrupt_lines[SPI_BUS_1].irq_num);
    return 0;
}

static int irq_spi2_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_spi_v100_irq_handler(SPI_BUS_2);
    osal_irq_clear(g_spi_interrupt_lines[SPI_BUS_2].irq_num);
    return 0;
}

static int irq_spi3_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_spi_v100_irq_handler(SPI_BUS_3);
    osal_irq_clear(g_spi_interrupt_lines[SPI_BUS_3].irq_num);
    return 0;
}

static int irq_spi4_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_spi_v100_irq_handler(SPI_BUS_4);
    osal_irq_clear(g_spi_interrupt_lines[SPI_BUS_4].irq_num);
    return 0;
}

static int irq_spi5_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_spi_v100_irq_handler(SPI_BUS_5);
    osal_irq_clear(g_spi_interrupt_lines[SPI_BUS_5].irq_num);
    return 0;
}

static int irq_spi6_handler(int i, void *p)
{
    unused(i);
    unused(p);
    hal_spi_v100_irq_handler(SPI_BUS_6);
    osal_irq_clear(g_spi_interrupt_lines[SPI_BUS_6].irq_num);
    return 0;
}
#endif

#if defined(CONFIG_SPI_SUPPORT_INTERRUPT) && (CONFIG_SPI_SUPPORT_INTERRUPT == 1)
void spi_port_register_irq(spi_bus_t bus)
{
    osal_irq_request(g_spi_interrupt_lines[bus].irq_num, g_spi_interrupt_lines[bus].irq_func, NULL, NULL, NULL);
    osal_irq_set_priority(g_spi_interrupt_lines[bus].irq_num, irq_prio(g_spi_interrupt_lines[bus].irq_num));
    osal_irq_enable(g_spi_interrupt_lines[bus].irq_num);
}

void spi_port_unregister_irq(spi_bus_t bus)
{
    osal_irq_disable(g_spi_interrupt_lines[bus].irq_num);
    osal_irq_free(g_spi_interrupt_lines[bus].irq_num, NULL);
}
#endif

uintptr_t spi_porting_base_addr_get(spi_bus_t index)
{
    return (uintptr_t)g_spi_base_addrs[index];
}

uint32_t spi_porting_max_slave_select_get(spi_bus_t bus)
{
    unused(bus);
    return (uint32_t)SPI_SLAVE0;
}

void spi_porting_set_device_mode(spi_bus_t bus, spi_mode_t mode)
{
    if (bus == SPI_BUS_3) {
        reg16_write(HAL_SPI3_MODE_SET_REG, mode);
    }
}

spi_mode_t spi_porting_get_device_mode(spi_bus_t bus)
{
    spi_mode_t mode = SPI_MODE_NONE;

    if (bus == SPI_BUS_3) {
        reg16_read(HAL_SPI3_MODE_SET_REG, mode);
        mode = (((uint16_t)mode & (uint16_t)1) == 1) ? SPI_MODE_SLAVE : SPI_MODE_MASTER;
    } else if (bus == SPI_BUS_4) {
        mode = SPI_MODE_SLAVE;
    } else {    // Other SPI are fixed as master.
        mode = SPI_MODE_MASTER;
    }

    return mode;
}

/* BS25 SPI testsuit pinctrl config, here we use bus_2 by default. */
void spi_porting_test_spi_init_pin(void)
{
    uapi_pin_set_mode(S_MGPIO16, HAL_PIO_FUNC_SPI2_M1);
    uapi_pin_set_mode(S_MGPIO17, HAL_PIO_FUNC_SPI2_M1);
    uapi_pin_set_mode(S_MGPIO18, HAL_PIO_FUNC_SPI2_M1);
    uapi_pin_set_mode(S_MGPIO19, HAL_PIO_FUNC_SPI2_M1);
}

void spi_porting_set_rx_mode(spi_bus_t bus, uint16_t num)
{
    g_spi_base_addrs[bus]->ssienr = 0;
    hal_spi_v100_ctrlr0_set_tmod(bus, HAL_SPI_TRANS_MODE_EEPROM);
    hal_spi_v100_ctrlr1_set_ndf(bus, num - 1);
    g_spi_base_addrs[bus]->ssienr = 1;
}

void spi_porting_set_tx_mode(spi_bus_t bus)
{
    g_spi_base_addrs[bus]->ssienr = 0;
    hal_spi_v100_ctrlr0_set_tmod(bus, HAL_SPI_TRANS_MODE_TX);
    g_spi_base_addrs[bus]->ssienr = 1;
}


// opi使用cfbb_spi接口适配
bool hal_opi_set_fifo_threshold(spi_bus_t bus, uint32_t threshold)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    g_spi_base_addrs[bus]->txftlr = threshold;
    g_spi_base_addrs[bus]->rxftlr = threshold;
    return true;
}

bool hal_opi_reset_config(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    // reset register
    g_spi_base_addrs[bus]->ctrlr0 &= ~HAL_SPI_CTRLR0_REG_MAX;
    g_spi_base_addrs[bus]->spi_ctrlr0 &= ~HAL_QSPI_CTRLR0_REG_MAX;
    return true;
}

bool hal_opi_set_frame_size(spi_bus_t bus, uint32_t frame_size)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    reg32_setbits(&g_spi_base_addrs[bus]->ctrlr0, HAL_SPI_DATA_FRAME_SIZE_BIT,
                  HAL_SPI_DATA_FRAME_SIZE_BITFILD, frame_size);

    return true;
}

bool hal_opi_set_frame_format(spi_bus_t bus, hal_spi_frame_format_t frame_format)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    switch (frame_format) {
        case HAL_SPI_FRAME_FORMAT_STANDARD:
            g_spi_base_addrs[bus]->ctrlr0 |= (HAL_SPI_FRAME_FORMAT_STANDARD << HAL_SPI_FRAME_FORMAT_SHIFT);
            g_spi_base_addrs[bus]->rsvd = HAL_SPI_RSVD_NONE;
            break;
        case HAL_SPI_FRAME_FORMAT_QUAD:
            g_spi_base_addrs[bus]->ctrlr0 |= (HAL_SPI_FRAME_FORMAT_QUAD << HAL_SPI_FRAME_FORMAT_SHIFT);
            g_spi_base_addrs[bus]->spi_ctrlr0 |= (HAL_QSPI_CMD_LENTH_8 << HAL_QSPI_CMD_LENTH_SHIFT);
            g_spi_base_addrs[bus]->spi_ctrlr0 |= (HAL_QSPI_ADDR_LENTH_24 << HAL_QSPI_ADDR_LENTH_SHIFT);
            g_spi_base_addrs[bus]->spi_ctrlr0 |= (HAL_QSPI_TRANS_TYPES_CMD_S_ADDR_Q << HAL_QSPI_TRANS_TYPES_SHIFT);
            g_spi_base_addrs[bus]->rsvd = HAL_SPI_RSVD_NONE;
            break;
#if (SPI_WITH_OPI == YES)
        case HAL_SPI_FRAME_FORMAT_OCTAL:
            g_spi_base_addrs[bus]->ctrlr0 |= (HAL_SPI_FRAME_FORMAT_OCTAL << HAL_SPI_FRAME_FORMAT_SHIFT);
            g_spi_base_addrs[bus]->rsvd = HAL_SPI_RSVD_X8;
            break;
        case HAL_SPI_FRAME_FORMAT_DOUBLE_OCTAL:
            g_spi_base_addrs[bus]->ctrlr0 |= (HAL_SPI_FRAME_FORMAT_OCTAL << HAL_SPI_FRAME_FORMAT_SHIFT);
            g_spi_base_addrs[bus]->rsvd = HAL_SPI_RSVD_X8_X8;
            break;
        case HAL_SPI_FRAME_FORMAT_SIXT:
            g_spi_base_addrs[bus]->ctrlr0 |= (HAL_SPI_FRAME_FORMAT_OCTAL << HAL_SPI_FRAME_FORMAT_SHIFT);
            g_spi_base_addrs[bus]->rsvd = HAL_SPI_RSVD_X16;
            break;
#endif
        default:
            return false;
    }
    return true;
}

bool hal_opi_set_trans_mode(spi_bus_t bus, uint32_t tmod)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    g_spi_base_addrs[bus]->ctrlr0 &= ~(HAL_SPI_TRANS_MODE_MAX << HAL_SPI_TRANS_MODE_SHIFT);
    g_spi_base_addrs[bus]->ctrlr0 |= (tmod << HAL_SPI_TRANS_MODE_SHIFT);
    return true;
}

bool hal_opi_set_freq(spi_bus_t bus, uint32_t clk_in_mhz)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    uint32_t clk_div;
    uint32_t bus_clk;
    bus_clk = (uint32_t)lpm_dev_get_freq(DEV_SPI);

    clk_div = (uint32_t)(bus_clk / hal_spi_mhz_to_hz(clk_in_mhz));
    if (clk_div < HAL_SPI_MINUMUM_CLK_DIV) {
        clk_div = HAL_SPI_MINUMUM_CLK_DIV;
    }

    if (clk_div > HAL_SPI_MAXIMUM_CLK_DIV) {
        clk_div = HAL_SPI_MAXIMUM_CLK_DIV;
    }

    g_spi_base_addrs[bus]->baudr &= ~HAL_SPI_CLK_DIV_REG_MAX;
#ifdef PRE_ASIC
    g_spi_base_addrs[bus]->baudr |= clk_div;
#else
    g_spi_base_addrs[bus]->baudr |= HAL_SPI_CLK_DIV_20;
    UNUSED(clk_div);
#endif

#if (SPI_WITH_OPI == YES)
    if (bus == OPI_BUS) {
        g_spi_base_addrs[bus]->baudr = HAL_SPI_CLK_DIV_2;
    }
#endif

    return true;
}

void hal_opi_set_inst_len(spi_bus_t bus, hal_spi_inst_len_t inst_len)
{
    g_spi_base_addrs[bus]->spi_ctrlr0 &= (~(HAL_SPI_INST_LEN_MAX << HAL_SPI_INST_L_POSE));
    g_spi_base_addrs[bus]->spi_ctrlr0 |= ((uint32_t)inst_len << HAL_SPI_INST_L_POSE);
}

void hal_opi_set_addr_len(spi_bus_t bus, hal_spi_addr_len_t addr_len)
{
    g_spi_base_addrs[bus]->spi_ctrlr0 &= (~(0xf << HAL_SPI_ADDR_L_POSE));
    g_spi_base_addrs[bus]->spi_ctrlr0 |= ((uint32_t)addr_len << HAL_SPI_ADDR_L_POSE);
}

void hal_opi_cmd_trans_mode(spi_bus_t bus, uint32_t mode)
{
    g_spi_base_addrs[bus]->spi_ctrlr0 &= (~0x3);
    g_spi_base_addrs[bus]->spi_ctrlr0 |= (uint32_t)mode;
}

bool hal_opi_set_received_data_num(spi_bus_t bus, uint32_t number)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    number--;
    g_spi_base_addrs[bus]->ctrlr1 &= ~HAL_SPI_RECEIVED_DATA_REG_MAX;
    g_spi_base_addrs[bus]->ctrlr1 = number;
    return true;
}

bool hal_opi_set_tx_fifo_threshold(spi_bus_t bus, uint32_t threshold)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    g_spi_base_addrs[bus]->txftlr = threshold;
    return true;
}

void hal_opi_ddr_en(spi_bus_t bus, bool on)
{
    if (on) {
        g_spi_base_addrs[bus]->spi_ctrlr0 |= (HAL_SPI_DDR_EN | HAL_SPI_RXDS_EN | HAL_SPI_INST_DDR_EN);
    } else {
        g_spi_base_addrs[bus]->spi_ctrlr0 &= (~(HAL_SPI_DDR_EN | HAL_SPI_RXDS_EN | HAL_SPI_INST_DDR_EN));
    }
}

bool hal_qspi_set_wait_cycles(spi_bus_t bus, uint32_t wait_cyc)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    g_spi_base_addrs[bus]->spi_ctrlr0 &= ~(HAL_QSPI_WAIT_CYCLE_MAX << HAL_QSPI_WAIT_CYCLE_SHIFT);
    g_spi_base_addrs[bus]->spi_ctrlr0 |= (wait_cyc << HAL_QSPI_WAIT_CYCLE_SHIFT);
    return true;
}

bool hal_opi_set_trans_type(spi_bus_t bus, hal_spi_trans_type_t trans_type)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    g_spi_base_addrs[bus]->spi_ctrlr0 &= (~0x3);
    g_spi_base_addrs[bus]->spi_ctrlr0 |= (uint32_t)trans_type;
    return true;
}

void hal_opi_disable_slave(spi_bus_t bus)
{
    // Disable all slave
    g_spi_base_addrs[bus]->ser &= ~HAL_SPI_SLAVE_ENABLE_REG_MAX;
}

uint32_t hal_opi_read_data(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    return (g_spi_base_addrs[bus]->dr[0]);
}

bool hal_opi_write_data(spi_bus_t bus, uint32_t data)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    g_spi_base_addrs[bus]->dr[0] = data;
    return true;
}

bool hal_opi_select_slave(spi_bus_t bus, uint32_t slave_num)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    // Disable all slave
    g_spi_base_addrs[bus]->ser &= ~HAL_SPI_SLAVE_ENABLE_REG_MAX;
    g_spi_base_addrs[bus]->ser = BIT(slave_num);

    return true;
}

bool hal_opi_disable(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    g_spi_base_addrs[bus]->ssienr &= ~HAL_SPI_ENABLE;
    return true;
}


bool hal_opi_enable(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    g_spi_base_addrs[bus]->ctrlr0 &= ~HAL_SPI_CE_LIN_TOGGLE_ENABLE;
    g_spi_base_addrs[bus]->ssienr |= HAL_SPI_ENABLE;
    return true;
}

bool hal_opi_tx_fifo_is_not_full(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    return (bool)hal_spi_v100_sr_get_tfnf(bus);
}

bool hal_opi_tx_fifo_is_empty(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    return (g_spi_base_addrs[bus]->sr & HAL_SPI_TX_FIFO_EMPTY_FLAG);
}

bool hal_opi_rx_fifo_is_full(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    return (g_spi_base_addrs[bus]->sr & HAL_SPI_RX_FIFO_FULL_FLAG);
}

bool hal_opi_rx_fifo_is_not_empty(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    return (g_spi_base_addrs[bus]->sr & HAL_SPI_RX_FIFO_NOT_EMPTY_FLAG);
}

bool hal_opi_is_busy(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }
    return (g_spi_base_addrs[bus]->sr & HAL_SPI_BUSY_FLAG);
}

bool hal_opi_set_rx_sample_dly(spi_bus_t bus, uint8_t delay)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    g_spi_base_addrs[bus]->rx_sample_dly &= ~(HAL_SPI_RX_SAMPLE_DLY_MAX << HAL_SPI_RX_SAMPLE_DLY_SHIFT);
    g_spi_base_addrs[bus]->rx_sample_dly |= (delay << HAL_SPI_RX_SAMPLE_DLY_SHIFT);
    return true;
}

bool hal_opi_set_dma_rx_data_level(spi_bus_t bus, uint32_t data_level)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    g_spi_base_addrs[bus]->dmardlr = data_level;
    return true;
}

bool hal_opi_dma_control(spi_bus_t bus, hal_spi_dma_control_t operation)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return false;
    }

    if (operation >= HAL_SPI_DMA_CONTROL_MAX_NUM) {
        return false;
    }

    g_spi_base_addrs[bus]->dmacr = (uint32_t)operation;
    return true;
}

uint32_t spi_porting_lock(spi_bus_t bus)
{
    unused(bus);
    return osal_irq_lock();
}

void spi_porting_unlock(spi_bus_t bus, uint32_t irq_sts)
{
    unused(bus);
    osal_irq_restore(irq_sts);
}

uint8_t spi_port_get_dma_trans_dest_handshaking(spi_bus_t bus)
{
    switch (bus) {
        case SPI_BUS_0:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_0_TX;
        case SPI_BUS_1:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_1_TX;
        case SPI_BUS_2:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_2_TX;
        case SPI_BUS_3:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_3_TX;
        case SPI_BUS_4:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
        case SPI_BUS_5:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_5_TX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}

uint8_t spi_port_get_dma_trans_src_handshaking(spi_bus_t bus)
{
    switch (bus) {
        case SPI_BUS_0:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_0_RX;
        case SPI_BUS_1:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_1_RX;
        case SPI_BUS_2:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_2_RX;
        case SPI_BUS_3:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_3_RX;
        case SPI_BUS_4:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_4_RX;
        case SPI_BUS_5:
            return (uint8_t)DMA_HANDSHAKE_SPI_BUS_5_RX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}

uint8_t spi_port_tx_data_level_get(spi_bus_t bus)
{
    switch (bus) {
        case SPI_BUS_0:
        case SPI_BUS_1:
        case SPI_BUS_2:
        case SPI_BUS_3:
            return (uint8_t)SPI_DMA_TX_DATA_LEVEL_4;
        case SPI_BUS_4:
        case SPI_BUS_5:
            return (uint8_t)QSPI_DMA_TX_DATA_LEVEL_8;
        default:
            return 0;
    }
}

uint8_t spi_port_rx_data_level_get(spi_bus_t bus)
{
    unused(bus);
    return 0;
}

void spi_funcreg_adapt(spi_bus_t bus)
{
    if (bus >= HAL_SPI_BUS_MAX_NUM) {
        return ;
    }

    (void)hal_spi_regs_init();
    return;
}

void spi_port_clock_enable(spi_bus_t bus, bool on)
{
    clock_control_type_t control_type;
    clock_mclken_aperp_type_t aperp_type;

    switch (bus) {
        case SPI_BUS_0:
            aperp_type = CLOCK_APERP_SPI0_M_CLKEN;
            break;

        case SPI_BUS_1:
            aperp_type = CLOCK_APERP_SPI1_M_CLKEN;
            break;

        case SPI_BUS_2:
            aperp_type = CLOCK_APERP_SPI2_M_CLKEN;
            break;

        case SPI_BUS_3:
            aperp_type = CLOCK_APERP_SPI3_MS_CLKEN;
            break;

        case SPI_BUS_4:
            aperp_type = CLOCK_APERP_SPI4_S_CLKEN;
            break;
        default:
            return;
    }

    if (on) {
        control_type = CLOCK_CONTROL_MCLKEN_ENABLE;
    } else {
        control_type = CLOCK_CONTROL_MCLKEN_DISABLE;
    }
    pm_lpc_ccrg_clk_spi_enable((lpc_ccrg_clk_spi_used_t)(bus), on);
    uapi_clock_control(control_type, aperp_type);
}
