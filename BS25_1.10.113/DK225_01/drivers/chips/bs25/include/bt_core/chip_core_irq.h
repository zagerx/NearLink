/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Irq config interface \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#ifndef CHIP_CORE_IRQ_H
#define CHIP_CORE_IRQ_H

#define LOCAL_INTERRUPT0 26

typedef enum core_irq {
/* -------------------  Processor Interrupt Numbers  ------------------------------ */
    MCU_INT0_IRQN                = LOCAL_INTERRUPT0 + 0,
    MCU_INT1_IRQN                = LOCAL_INTERRUPT0 + 1,
    RTC_0_IRQN                   = LOCAL_INTERRUPT0 + 2,
    RTC_1_IRQN                   = LOCAL_INTERRUPT0 + 3,
    RTC_2_IRQN                   = LOCAL_INTERRUPT0 + 4,
    RTC_3_IRQN                   = LOCAL_INTERRUPT0 + 5,
    TIMER_0_IRQN                 = LOCAL_INTERRUPT0 + 6,
    TIMER_1_IRQN                 = LOCAL_INTERRUPT0 + 7,
    TIMER_2_IRQN                 = LOCAL_INTERRUPT0 + 8,
    TIMER_3_IRQN                 = LOCAL_INTERRUPT0 + 9,
    EFLASH_COMBINE_IRQN          = LOCAL_INTERRUPT0 + 10,
    GPIO_0_IRQN                  = LOCAL_INTERRUPT0 + 11,
    GPIO_1_IRQN                  = LOCAL_INTERRUPT0 + 12,
    BCPU_PCLR_LOCK_IRQN          = LOCAL_INTERRUPT0 + 13,
    UART_0_IRQN                  = LOCAL_INTERRUPT0 + 14, // UARTL1
    RF_PRT_IRQN                  = LOCAL_INTERRUPT0 + 15,
    BT_VSET_IRQN                 = LOCAL_INTERRUPT0 + 16,
    EH2H_B2C_IRQN                = LOCAL_INTERRUPT0 + 17,
    EH2H_C2B_IRQN                = LOCAL_INTERRUPT0 + 18,
    OSC_EN_WKUP_IRQN             = LOCAL_INTERRUPT0 + 19,
    OSC_EN_SLEEP_IRQN            = LOCAL_INTERRUPT0 + 20,
    B_WAKEUP_IRQN                = LOCAL_INTERRUPT0 + 21,
    B_SLEEP_IRQN                 = LOCAL_INTERRUPT0 + 22,
    COM_RAM_MONITOR_IRQN         = LOCAL_INTERRUPT0 + 23,
    C2B_COMRAM_MONITOR_IRQN      = LOCAL_INTERRUPT0 + 24,
    PMU2_CLK_32K_CALI_IRQN       = LOCAL_INTERRUPT0 + 25,
    BT_BB_BT_IRQ                 = LOCAL_INTERRUPT0 + 26,
    BT_BB_BLE_IRQ                = LOCAL_INTERRUPT0 + 27,
    BT_BB_GLE_IRQ                = LOCAL_INTERRUPT0 + 28,
    TSENSOR_IRQN                 = LOCAL_INTERRUPT0 + 29,
    RESERVED_30_IRQN             = LOCAL_INTERRUPT0 + 30,
    RESERVED_31_IRQN             = LOCAL_INTERRUPT0 + 31,
    RESERVED_32_IRQN             = LOCAL_INTERRUPT0 + 32,
    RESERVED_33_IRQN             = LOCAL_INTERRUPT0 + 33,
    RESERVED_34_IRQN             = LOCAL_INTERRUPT0 + 34,
    RESERVED_35_IRQN             = LOCAL_INTERRUPT0 + 35,
    RESERVED_36_IRQN             = LOCAL_INTERRUPT0 + 36,
    RESERVED_37_IRQN             = LOCAL_INTERRUPT0 + 37,
    RESERVED_38_IRQN             = LOCAL_INTERRUPT0 + 38,
    RESERVED_39_IRQN             = LOCAL_INTERRUPT0 + 39,
    RESERVED_40_IRQN             = LOCAL_INTERRUPT0 + 40,
    RESERVED_41_IRQN             = LOCAL_INTERRUPT0 + 41,
    RESERVED_42_IRQN             = LOCAL_INTERRUPT0 + 42,
    RESERVED_43_IRQN             = LOCAL_INTERRUPT0 + 43,
    RESERVED_44_IRQN             = LOCAL_INTERRUPT0 + 44,
    RESERVED_45_IRQN             = LOCAL_INTERRUPT0 + 45,
    RESERVED_46_IRQN             = LOCAL_INTERRUPT0 + 46,
    RESERVED_47_IRQN             = LOCAL_INTERRUPT0 + 47,
    BUTT_IRQN,
} core_irq_t;

#endif
