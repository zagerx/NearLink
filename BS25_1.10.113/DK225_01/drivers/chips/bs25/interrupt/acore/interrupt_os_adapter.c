/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Interrupt driver \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#include "stdint.h"
#include "chip_core_irq.h"
#include "los_hwi.h"

uint8_t m_aucIntPri[BUTT_IRQN] = {
    LOSCFG_HWI_PRIO_LIMIT,   // USER_SOFTWARE_INT_IRQn            = 0,
    LOSCFG_HWI_PRIO_LIMIT,   // SUPERVISOR_SOFTWARE_INT_IRQn      = 1,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT2_IRQn                = 2,
    OS_HWI_PRIO_LOWEST,      // MACHINE_SOFTWARE_INT_IRQn         = 3,
    LOSCFG_HWI_PRIO_LIMIT,   // USER_TIMER_INT_IRQn               = 4,
    LOSCFG_HWI_PRIO_LIMIT,   // SUPERVISOR_TIMER_INT_IRQn         = 5,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT6_IRQn                = 6,
    LOSCFG_HWI_PRIO_LIMIT,   // MACHINE_TIMER_INT_IRQn            = 7,
    LOSCFG_HWI_PRIO_LIMIT,   // USER_EXTERNAL_INT_IRQn            = 8,
    LOSCFG_HWI_PRIO_LIMIT,   // SUPERVISOR_EXTERNAL_INT_IRQn      = 9,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT10_IRQn               = 10,
    OS_HWI_PRIO_LOWEST,      // MACHINE_EXTERNAL_INT_IRQn         = 11,
    OS_HWI_PRIO_HIGHEST,     // NON_MASKABLE_INT_IRQn             = 12,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT13_IRQn               = 13,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT14_IRQn               = 14,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT15_IRQn               = 15,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT16_IRQn               = 16,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT17_IRQn               = 17,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT18_IRQn               = 18,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT19_IRQn               = 19,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT20_IRQn               = 20,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT21_IRQn               = 21,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT22_IRQn               = 22,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT23_IRQn               = 23,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT24_IRQn               = 23,
    LOSCFG_HWI_PRIO_LIMIT,   // RESERVED_INT25_IRQn               = 25,

    OS_HWI_PRIO_LOWEST,      // RESERVED_INT0_IRQn                =  0,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT1_IRQn                =  1,
    OS_HWI_PRIO_LOWEST - 1,  // MCU_INT0_IRQn                     =  2,
    OS_HWI_PRIO_LOWEST,      // MCU_INT1_IRQn                     =  3,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT4_IRQn                =  4,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT5_IRQn                =  5,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT6_IRQn                =  6,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT7_IRQn                =  7,
    OS_HWI_PRIO_LOWEST - 3,      // GPIO_0_IRQn                       =  8,
    OS_HWI_PRIO_LOWEST - 3,      // GPIO_1_IRQn                       =  9,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT10_IRQn               = 10,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT11_IRQn               = 11,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT12_IRQn               = 12,
    OS_HWI_PRIO_LOWEST,      // UART_L0_IRQn                      = 13,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT14_IRQn               = 14,
    OS_HWI_PRIO_LOWEST - 2,  // UART_H0_IRQn                      = 15,
    OS_HWI_PRIO_LOWEST,      // UART_H1_IRQn                      = 16,
    OS_HWI_PRIO_LOWEST,      // QSPI0_2CS_IRQn                    = 17,
    OS_HWI_PRIO_LOWEST,      // QSPI1_2CS_IRQn                    = 18,
    OS_HWI_PRIO_LOWEST,      // SPI4_S_IRQn                       = 19,
    OS_HWI_PRIO_LOWEST,      // KEY_SCAN_IRQn                     = 20,
    OS_HWI_PRIO_LOWEST,      // M_WAKEUP_IRQn                     = 21,
    OS_HWI_PRIO_LOWEST,      // M_SLEEP_IRQn                      = 22,
    OS_HWI_PRIO_LOWEST,      // RTC_0_IRQn                        = 23,
    OS_HWI_PRIO_LOWEST,      // RTC_1_IRQn                        = 24,
    OS_HWI_PRIO_LOWEST,      // RTC_2_IRQn                        = 25,
    OS_HWI_PRIO_LOWEST,      // RTC_3_IRQn                        = 26,
    OS_HWI_PRIO_LOWEST - 1,  // TIMER_0_IRQn                      = 27,
    OS_HWI_PRIO_LOWEST - 1,  // TIMER_1_IRQn                      = 28,
    OS_HWI_PRIO_LOWEST - 1,  // TIMER_2_IRQn                      = 29,
    OS_HWI_PRIO_LOWEST,      // TIMER_3_IRQn                      = 30,
    OS_HWI_PRIO_LOWEST,      // M_SDMA_IRQn                       = 31,
    OS_HWI_PRIO_LOWEST,      // M_DMA_IRQn                        = 32,
    OS_HWI_PRIO_LOWEST,      // SPI_M_S_0_IRQn                    = 33,
    OS_HWI_PRIO_LOWEST,      // SPI_M_S_1_IRQn                    = 34,
    OS_HWI_PRIO_LOWEST,      // SPI_M_IRQn                        = 35,
    OS_HWI_PRIO_LOWEST,      // I2C_0_IRQn                        = 36,
    OS_HWI_PRIO_LOWEST,      // I2C_1_IRQn                        = 37,
    OS_HWI_PRIO_LOWEST,      // I2C_2_IRQn                        = 38,
    OS_HWI_PRIO_LOWEST,      // SPI3_MS_IRQn                      = 39,
    OS_HWI_PRIO_LOWEST,      // EFLASH_INT_IRQn                   = 40,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT41_IRQn               = 41,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT42_IRQn               = 42,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT43_IRQn               = 43,
    OS_HWI_PRIO_LOWEST,      // SEC_INT_IRQn                      = 44,
    OS_HWI_PRIO_LOWEST,      // PWM_0_IRQn                        = 45,
    OS_HWI_PRIO_LOWEST,      // PWM_1_IRQn                        = 46,
    OS_HWI_PRIO_LOWEST,      // PWM_2_IRQn                        = 47,
    OS_HWI_PRIO_LOWEST,      // PWM_3_IRQn                        = 48,
    OS_HWI_PRIO_LOWEST,      // PWM_4_IRQn                        = 49,
    OS_HWI_PRIO_LOWEST,      // PWM_5_IRQn                        = 50,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT51_IRQn               = 51,
    OS_HWI_PRIO_LOWEST,      // PMU_CMU_ERR_IRQn                  = 52,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT53_IRQn               = 53,
    OS_HWI_PRIO_LOWEST,      // RESERVED_INT54_IRQn               = 54,
    OS_HWI_PRIO_LOWEST,      // MEM_SUB_MONITOR_INT_IRQn          = 55,
    OS_HWI_PRIO_LOWEST,      // B_SUB_MONITOR_INT_IRQn            = 56,
    OS_HWI_PRIO_LOWEST,      // COMRAM_MONITOR_IRQn               = 57,
    OS_HWI_PRIO_LOWEST,      // EH2H_BRG_IRQn                     = 58,
    OS_HWI_PRIO_LOWEST,      // PMU2_CLK_32K_CALI_IRQn            = 59,
    OS_HWI_PRIO_LOWEST,      // B_WDT_IRQn                        = 60,
    OS_HWI_PRIO_LOWEST,      // TSENSOR_IRQn                      = 61,
    OS_HWI_PRIO_HIGHEST,      // QDEC_IRQn                         = 62,
    OS_HWI_PRIO_LOWEST - 2,  // USB_IRQn                          = 63,
};
