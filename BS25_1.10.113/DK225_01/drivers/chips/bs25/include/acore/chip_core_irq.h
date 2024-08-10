/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:   Chip core irq >= 0 define.
 * Author: @CompanyNameTag
 * Create:  2021-06-16
 */

#ifndef CHIP_CORE_IRQ_H
#define CHIP_CORE_IRQ_H

#define LOCAL_INTERRUPT0 26

typedef enum core_irq {
/* -------------------  Processor Interrupt Numbers  ------------------------------ */
    BT_INT0_IRQN                = LOCAL_INTERRUPT0 + 0,
    BT_INT1_IRQN                = LOCAL_INTERRUPT0 + 1,
    GADC_DONE_IRQN              = LOCAL_INTERRUPT0 + 2,
    GADC_ALARM_IRQN             = LOCAL_INTERRUPT0 + 3,
    HADC_DONE_IRQN              = LOCAL_INTERRUPT0 + 4,
    HADC_ALARM_IRQN             = LOCAL_INTERRUPT0 + 5,
    MCU_PCLR_LOCK_IRQN          = LOCAL_INTERRUPT0 + 6,
    ULP_GPIO_IRQN               = LOCAL_INTERRUPT0 + 7,
    GPIO_0_IRQN                 = LOCAL_INTERRUPT0 + 8,
    GPIO_1_IRQN                 = LOCAL_INTERRUPT0 + 9,
    BT_TOGGLE_POS_IRQN          = LOCAL_INTERRUPT0 + 10,
    BT_TOGGLE_NEG_IRQN          = LOCAL_INTERRUPT0 + 11,
    KEY_SCAN_LOW_POWER_IRQN     = LOCAL_INTERRUPT0 + 12,
    UART_0_IRQN                 = LOCAL_INTERRUPT0 + 13, // UARTL0
    MCU_SIMO1P1_VSET_INT_IRQN   = LOCAL_INTERRUPT0 + 14,
    UART_1_IRQN                 = LOCAL_INTERRUPT0 + 15, // UARTH0
    UART_2_IRQN                 = LOCAL_INTERRUPT0 + 16, // UARTL2
    QSPI0_2CS_IRQN              = LOCAL_INTERRUPT0 + 17,
    QSPI1_2CS_IRQN              = LOCAL_INTERRUPT0 + 18,
    SPI4_S_IRQN                 = LOCAL_INTERRUPT0 + 19,
    KEY_SCAN_IRQN               = LOCAL_INTERRUPT0 + 20,
    M_WAKEUP_IRQN               = LOCAL_INTERRUPT0 + 21,
    M_SLEEP_IRQN                = LOCAL_INTERRUPT0 + 22,
    RTC_0_IRQN                  = LOCAL_INTERRUPT0 + 23,
    RTC_1_IRQN                  = LOCAL_INTERRUPT0 + 24,
    RTC_2_IRQN                  = LOCAL_INTERRUPT0 + 25,
    RTC_3_IRQN                  = LOCAL_INTERRUPT0 + 26,
    TIMER_0_IRQN                = LOCAL_INTERRUPT0 + 27,
    TIMER_1_IRQN                = LOCAL_INTERRUPT0 + 28,
    TIMER_2_IRQN                = LOCAL_INTERRUPT0 + 29,
    TIMER_3_IRQN                = LOCAL_INTERRUPT0 + 30,
    M_SDMA_IRQN                 = LOCAL_INTERRUPT0 + 31,
    M_DMA_IRQN                  = LOCAL_INTERRUPT0 + 32,
    SPI_M_S_0_IRQN              = LOCAL_INTERRUPT0 + 33,
    SPI_M_S_1_IRQN              = LOCAL_INTERRUPT0 + 34,
    SPI_M_IRQN                  = LOCAL_INTERRUPT0 + 35,
    I2C_0_IRQN                  = LOCAL_INTERRUPT0 + 36,
    I2C_1_IRQN                  = LOCAL_INTERRUPT0 + 37,
    I2C_2_IRQN                  = LOCAL_INTERRUPT0 + 38,
    SPI3_MS_IRQN                = LOCAL_INTERRUPT0 + 39,
    EFLASH_INT_IRQN             = LOCAL_INTERRUPT0 + 40,
    I2S_IRQN                    = LOCAL_INTERRUPT0 + 41,
    I2C_3_IRQN                  = LOCAL_INTERRUPT0 + 42,
    I2C_4_IRQN                  = LOCAL_INTERRUPT0 + 43,
    SEC_INT_IRQN                = LOCAL_INTERRUPT0 + 44,
    PWM_0_IRQN                  = LOCAL_INTERRUPT0 + 45,
    PWM_1_IRQN                  = LOCAL_INTERRUPT0 + 46,
    PWM_2_IRQN                  = LOCAL_INTERRUPT0 + 47,
    PWM_3_IRQN                  = LOCAL_INTERRUPT0 + 48,
    PWM_4_IRQN                  = LOCAL_INTERRUPT0 + 49,
    PWM_5_IRQN                  = LOCAL_INTERRUPT0 + 50,
    MCU_SIMO0P9_VSET_INT_IRQN   = LOCAL_INTERRUPT0 + 51,
    PMU_CMU_ERR_IRQN            = LOCAL_INTERRUPT0 + 52,
    PMU_IRQ_IRQN                = LOCAL_INTERRUPT0 + 53,
    AUX_ADC_IRQN                = LOCAL_INTERRUPT0 + 54,
    MEM_SUB_MONITOR_INT_IRQN    = LOCAL_INTERRUPT0 + 55,
    B_SUB_MONITOR_INT_IRQN      = LOCAL_INTERRUPT0 + 56,
    COMRAM_MONITOR_IRQN         = LOCAL_INTERRUPT0 + 57,
    EH2H_BRG_IRQN               = LOCAL_INTERRUPT0 + 58,
    PMU2_CLK_32K_CALI_IRQN      = LOCAL_INTERRUPT0 + 59,
    B_WDT_IRQN                  = LOCAL_INTERRUPT0 + 60,
    TSENSOR_IRQN                = LOCAL_INTERRUPT0 + 61,
    QDEC_IRQN                   = LOCAL_INTERRUPT0 + 62,
    USB_IRQN                    = LOCAL_INTERRUPT0 + 63,
    BUTT_IRQN
} core_irq_t;

#endif
