/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Bt core platform definitions \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#ifndef PLATFORM_CORE_H
#define PLATFORM_CORE_H

#include "product.h"
#include "chip_core_definition.h"

/**
 * @defgroup DRIVER_PLATFORM_CORE Platform CORE Driver
 * @ingroup DRIVER_PLATFORM
 * @{
 */
#define GLB_CTL_M_RB_BASE   0x57000000
#define GLB_CTL_B_RB_BASE   0x57000400
#define GLB_CTL_D_RB_BASE   0x57000800
#define DISPLAY_CTL_RB_BASE 0x56000000

#define B_CTL_RB_BASE            0x59000000
#define M_CTL_RB_BASE            0x52000000
#define COM_CTL_RB_BASE          0x55000000
#define PMU1_CTL_RB_BASE         0x57004000
#define PMU2_CMU_CTL_RB_BASE     0x57008000
#define FUSE_CTL_RB_ADDR         0x57028000
#define XO_CORE_TRIM_REG         0x57028308
#define XO_CORE_CTRIM_REG        0x5702830c
#define ULP_AON_CTL_RB_ADDR      0x5702c000
#define FLASH_START_ADDR         0x10000000

#define UART0_BASE            0x52082000 /* UART_L0 */
#define UART1_BASE            0x52080000 /* UART_H0 */
#define UART2_BASE            0x52081000 /* UART_H1 */
#define DMA_BASE_ADDR         0x59800000 /* B_DMA */
#define SDMA_BASE_ADDR        0x59800000
#define RTC_TIMER_BASE_ADDR   0x57026000
#define SYSTICK_BASE_ADDR     0x57000780
#define TCXO_COUNT_BASE_ADDR  0x550005A0

#define WDT_BASE_ADDR         0x59005000
#define CHIP_WDT_BASE_ADDRESS 0x57034000

#define HAL_SOFT_RST_CTL_BASE               (GLB_CTL_M_RB_BASE)
#define HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET    0x168
#define HAL_CHIP_WDT_ATOP1_RST_BIT          2

#define NMI_CTL_REG_BASE_ADDR 0x59000300

/* spi reg base addr */
#define SPI_BUS_0_BASE_ADDR 0x5208A000 // SPI_M0
#define SPI_BUS_1_BASE_ADDR 0x52088000 // SPI_MS_1
#define SPI_BUS_2_BASE_ADDR 0x52089000 // SPI_MS_2
#define SPI_BUS_3_BASE_ADDR 0xA3000000 // QSPI_0
#define SPI_BUS_4_BASE_ADDR 0xA3002000 // QSPI_1
#define SPI_BUS_5_BASE_ADDR 0x52090000 // QSPI_2
#define SPI_BUS_6_BASE_ADDR 0xA3004000 // OSPI

#define HAL_SPI_DEVICE_MODE_SET_REG   (*(volatile unsigned short *)(0x52000828))

// GPIO regs
#define GPIO0_BASE_ADDR  0x57012000
#define GPIO1_BASE_ADDR  0x57016000
#define GPIO2_BASE_ADDR  0x5701A000
#define GPIO3_BASE_ADDR  0x5701E000
#define GPIO4_BASE_ADDR  0x57022000

// GPIO select core
#define HAL_GPIO_D_CORE_SET_REG             0x570001B0
#define HAL_GPIO_NON_D_CORE_SET_REG         0x57000180
#define HAL_GPIO_CORE_SET_CHANNEL_OFFSET    0x08
#define HAL_GPIO_CORE_SET_REG_OFFSET        2
#define HAL_GPIO_CORE_SET_GPIOS             16

// SEC BASE ADDR
#define SEC_CTL_RB_BASE 0x52009000
#define RSAV2_S_RB_BASE 0x52030000
#define TRNG_RB_BASE 0x52009800
// Timer reg base addr
#define TIMER_BASE_ADDR                         0x59004000
#define TIMER_0_BASE_ADDR                       (TIMER_BASE_ADDR + 0x00)
#define TIMER_1_BASE_ADDR                       (TIMER_BASE_ADDR + 0x14)
#define TIMER_2_BASE_ADDR                       (TIMER_BASE_ADDR + 0x28)
#define TIMER_3_BASE_ADDR                       (TIMER_BASE_ADDR + 0x3C)
#define TIMER_SYSCTL_BASE_ADDR                  (TIMER_BASE_ADDR + 0xA0)

// cpu trace memory
#define HAL_CPU_TRACE_CTL_RB_BASE_ADDR              0x5900C000
#define HAL_CPU_TRACE_DIAG_EN_OFFSET                0x104
#define HAL_CPU_TRACE_DIAG_CLOCK_CFG_OFFSET         0x108
#define HAL_CPU_TRACE_DIAG_CLOCK_SELECT_OFFSET      0x10C
#define HAL_CPU_TRACE_DIAG_SAMPLE_MODE_CFG_OFFSET   0x200
#define TRACE_MEM_REGION_START                  BCPU_TRACE_MEM_REGION_START
#define TRACE_MEM_REGION_LENGTH                 CPU_TRACE_MEM_REGION_LENGTH
#define BCPU_TRACE_ENABLE_CFG                   YES
#define HAL_BCPU_TRACE_ENABLE                   0x59008500

/**
 * @brief  Maximum UART buses
 * Defined here rather than in the uart_bus_t enum, due to needing to use it for conditional compilation
 */
#define UART_BUS_MAX_NUMBER 1  // !< Max number of UARTS available
#define SPI_BUS_MAX_NUMBER  6  // !< Max number of SPI available
#define GPIO_MAX_NUMBER     2  // !< Max number of GPIO available
#define TIMER_MAX_AVAILABLE_NUMBER    4  // !< Max number of timer available
#define RTC_MAX_AVAILABLE_NUMBER      4  // !< Max number of rtc available

#define S_DMA_CHANNEL_MAX_NUM    0  // !< Max number of S_DMA available
#define B_DMA_CHANNEL_MAX_NUM    2  // !< Max number of B_DMA available
#define DMA_CHANNEL_MAX_NUM      (S_DMA_CHANNEL_MAX_NUM + B_DMA_CHANNEL_MAX_NUM)

/**********************************************************************/
/************************* MPU config base addr ***********************/
/**********************************************************************/
#define MPU_BT_TEXT1_BASE1_ADDR         0x0
#define MPU_BT_TEXT1_LEN1               0x80000
#define MPU_BT_TEXT1_BASE2_ADDR         0x80000
#define MPU_BT_TEXT1_LEN2               0x40000

#define MPU_BT_TEXT2_BASE1_ADDR         0x20000000
#define MPU_BT_TEXT2_LEN1               0x80000
#define MPU_BT_TEXT2_BASE2_ADDR         0x20080000
#define MPU_BT_TEXT2_LEN2               0x40000


#define MPU_XIP_PSRAM_RO_ADDR_BASE      0x08000000
#define MPU_XIP_PSRAM_RO_ADDR_LEN       0x04000000

#define MPU_XIP_PSRAM_RW_ADDR_BASE      0x0C000000
#define MPU_XIP_PSRAM_RW_ADDR_LEN       0x04000000

#define MPU_XIP_FLASE_RO_ADDR_BASE      0x10000000
#define MPU_XIP_FLASE_RO_ADDR_LEN       0x10000000

#define MPU_STACK_START_ADDR_MIN        (MPU_BT_TEXT1_BASE2_ADDR + MPU_BT_TEXT1_LEN2) /* 0x120000 */

/* TRNG reg base addr */
#define B_TRNG_CTL_RB_BASE              0x59011000
#define TRNG_0_BASE                     0x59011020
#define TRNG_1_BASE                     0x59011030

/* PATCH reg base addr */
#define B_PATCH_BASE               0xE0002000

#define CHIP_BCPU_SWDDIO  0
#define CHIP_BCPU_SWDCLK  0

#define TEST_SUITE_UART_BUS UART_BUS_0 // UART H0
/* Test Suite UART Transmission PIN to use */
#define TEST_SUITE_UART_TX_PIN S_AGPIO11
/* Test Suite UART Reception PIN to use */
#define TEST_SUITE_UART_RX_PIN S_AGPIO12

#ifdef PRE_ASIC
    #define CHIP_FIXED_RX_PIN L_MGPIO14
    #define CHIP_FIXED_TX_PIN L_MGPIO15
#else
    #define CHIP_FIXED_RX_PIN 0
    #define CHIP_FIXED_TX_PIN 0
#endif

#define SW_DEBUG_UART_BUS UART_BUS_0
#define CHIP_FIXED_UART_BUS UART_BUS_0
#define WVT_FIXED_UART_BAUDRATE 230400

#define QSPI_MAX_NUMBER    3
#define FLASH_QSPI_ID      QSPI_BUS_0
#define QSPI_0_BASE_ADDR   0xA3000000
#define QSPI_1_BASE_ADDR   0xA3002000
#define QSPI_2_BASE_ADDR   0x52090000

/* !< QSPI bus */
typedef enum {
    QSPI_BUS_0 = 0,
    QSPI_BUS_1,
    QSPI_BUS_2,
    QSPI_BUS_NONE = QSPI_MAX_NUMBER
} qspi_bus_t;

/**
 * @brief  UART buses
 */
typedef enum {
    UART_BUS_0 = 0,                      // !< UART L1
    UART_BUS_NONE = UART_BUS_MAX_NUMBER  // !< Value used as invalid/unused UART number
} uart_bus_t;

typedef enum {
    SPI_BUS_0 = 0,
    SPI_BUS_1,
    SPI_BUS_2,
    SPI_BUS_3,
    SPI_BUS_4,
    SPI_BUS_5,
    SPI_BUS_6,
    SPI_BUS_NONE = SPI_BUS_MAX_NUMBER
} spi_bus_t;

typedef enum {
    CLOCK_REFERENCE_CLOCK_RTC = 0,
    CLOCK_REFERENCE_CLOCK_CRYSTAL,
    CLOCK_REFERENCE_CLOCK_MAX_NUMBER
} clock_reference_clock_e;

typedef enum {
    S_MGPIO0  = 0,         // GPIO0_0   C_PINMUX_CTL offset: 0x0300
    S_MGPIO1  = 1,         // GPIO0_1   C_PINMUX_CTL
    S_MGPIO2  = 2,         // GPIO0_2   C_PINMUX_CTL
    S_MGPIO3  = 3,         // GPIO0_3   C_PINMUX_CTL
    S_MGPIO4  = 4,         // GPIO0_4   C_PINMUX_CTL
    S_MGPIO5  = 5,         // GPIO0_5   C_PINMUX_CTL
    S_MGPIO6  = 6,         // GPIO0_6   C_PINMUX_CTL
    S_MGPIO7  = 7,         // GPIO0_7   C_PINMUX_CTL
    S_MGPIO8  = 8,         // GPIO0_8   C_PINMUX_CTL
    S_MGPIO9  = 9,         // GPIO0_9   C_PINMUX_CTL
    S_MGPIO10 = 10,        // GPIO0_10  C_PINMUX_CTL
    S_MGPIO11 = 11,        // GPIO0_11  C_PINMUX_CTL
    S_MGPIO12 = 12,        // GPIO0_12  C_PINMUX_CTL
    S_MGPIO13 = 13,        // GPIO0_13  C_PINMUX_CTL
    S_MGPIO14 = 14,        // GPIO0_14  C_PINMUX_CTL
    S_MGPIO15 = 15,        // GPIO0_15  C_PINMUX_CTL
    S_MGPIO16 = 16,        // GPIO0_16  C_PINMUX_CTL
    S_MGPIO17 = 17,        // GPIO0_17  C_PINMUX_CTL
    S_MGPIO18 = 18,        // GPIO0_18  C_PINMUX_CTL
    S_MGPIO19 = 19,        // GPIO0_19  C_PINMUX_CTL
    S_MGPIO20 = 20,        // GPIO0_20  C_PINMUX_CTL
    S_MGPIO21 = 21,        // GPIO0_21  C_PINMUX_CTL
    S_MGPIO22 = 22,        // GPIO0_22  C_PINMUX_CTL
    S_MGPIO23 = 23,        // GPIO0_23  C_PINMUX_CTL
    S_MGPIO24 = 24,        // GPIO0_24  C_PINMUX_CTL
    S_MGPIO25 = 25,        // GPIO0_25  C_PINMUX_CTL
    S_MGPIO26 = 26,        // GPIO0_26  C_PINMUX_CTL
    S_MGPIO27 = 27,        // GPIO0_27  C_PINMUX_CTL
    S_MGPIO28 = 28,        // GPIO0_28  C_PINMUX_CTL offset: 0x031C
    S_MGPIO29 = 29,        // GPIO0_29  C_PINMUX_CTL
    S_MGPIO30 = 30,        // GPIO0_30  C_PINMUX_CTL
    S_MGPIO31 = 31,        // GPIO0_31  C_PINMUX_CTL

    S_AGPIO0 =  32,        // GPIO1_0   PINMUX_CTL   offset: 0x0320
    S_AGPIO1 =  33,        // GPIO1_1   PINMUX_CTL
    S_AGPIO2 =  34,        // GPIO1_2   PINMUX_CTL
    S_AGPIO3 =  35,        // GPIO1_3   PINMUX_CTL
    S_AGPIO4 =  36,        // GPIO1_4   PINMUX_CTL   offset: 0x0324
    S_AGPIO5 =  37,        // GPIO1_5   PINMUX_CTL
    S_AGPIO6 =  38,        // GPIO1_6   PINMUX_CTL
    S_AGPIO7 =  39,        // GPIO1_7   PINMUX_CTL
    S_AGPIO8 =  40,        // GPIO1_8   PINMUX_CTL   offset: 0x0328
    S_AGPIO9 =  41,        // GPIO1_9   PINMUX_CTL
    S_AGPIO10 = 42,        // GPIO1_10  PINMUX_CTL
    S_AGPIO11 = 43,        // GPIO1_11  PINMUX_CTL
    S_AGPIO12 = 44,        // GPIO1_12  PINMUX_CTL   offset: 0x032C
    S_AGPIO13 = 45,        // GPIO1_13  PINMUX_CTL
    S_AGPIO14 = 46,        // GPIO1_14  PINMUX_CTL
    S_AGPIO15 = 47,        // GPIO1_15  PINMUX_CTL
    S_AGPIO16 = 48,        // GPIO1_16  PINMUX_CTL   offset: 0x0330
    S_AGPIO17 = 49,        // GPIO1_17  PINMUX_CTL
    S_AGPIO18 = 50,        // GPIO1_18  PINMUX_CTL
    S_AGPIO19 = 51,        // GPIO1_19  PINMUX_CTL

    S_HGPIO0 = 52,         // GPIO1_20  C_PINMUX_CTL offset: 0x0334
    S_HGPIO1 = 53,         // GPIO1_21  C_PINMUX_CTL
    S_HGPIO2 = 54,         // GPIO1_22  C_PINMUX_CTL
    S_HGPIO3 = 55,         // GPIO1_23  C_PINMUX_CTL
    S_HGPIO4 = 56,         // GPIO1_24  C_PINMUX_CTL offset: 0x0338
    S_HGPIO5 = 57,         // GPIO1_25  C_PINMUX_CTL

    L_HGPIO0 = 64,         // RESERVED
    L_HGPIO11 = 75,        // RESERVED
                           // RESERVED 58 ~ 75
    L_AGPIO0 = 76,         // GPIO2_12  PINMUX_CTL   offset: 0x034C
    L_AGPIO1 = 77,         // GPIO2_13  PINMUX_CTL
    L_AGPIO2 = 78,         // GPIO2_14  PINMUX_CTL
    L_AGPIO3 = 79,         // GPIO2_15  PINMUX_CTL
    L_AGPIO8 = 84,         // RESERVED
                           // RESERVED 80 ~ 84
    L_MGPIO0 = 85,         // GPIO2_21  C_PINMUX_CTL offset: 0x0354
    L_MGPIO1 = 86,         // GPIO2_22  C_PINMUX_CTL
    L_MGPIO2 = 87,         // GPIO2_23  C_PINMUX_CTL
    L_MGPIO3 = 88,         // GPIO2_24  C_PINMUX_CTL
    L_MGPIO4 = 89,         // GPIO2_25  C_PINMUX_CTL
    L_MGPIO5 = 90,         // GPIO2_26  C_PINMUX_CTL
    L_MGPIO6 = 91,         // GPIO2_27  C_PINMUX_CTL
    L_MGPIO7 = 92,         // GPIO2_28  C_PINMUX_CTL
    L_MGPIO8 = 93,         // GPIO2_29  C_PINMUX_CTL
    L_MGPIO9 = 94,         // GPIO2_30  C_PINMUX_CTL
    L_MGPIO10 = 95,        // GPIO2_31  C_PINMUX_CTL
    L_MGPIO11 = 96,        // GPIO3_0   C_PINMUX_CTL
    L_MGPIO12 = 97,        // GPIO3_1   C_PINMUX_CTL
    L_MGPIO13 = 98,        // GPIO3_2   C_PINMUX_CTL
    L_MGPIO14 = 99,        // GPIO3_3   C_PINMUX_CTL
    L_MGPIO15 = 100,       // GPIO3_4   C_PINMUX_CTL
    L_MGPIO16 = 101,       // GPIO3_5   C_PINMUX_CTL
    L_MGPIO17 = 102,       // GPIO3_6   C_PINMUX_CTL
    L_MGPIO18 = 103,       // GPIO3_7   C_PINMUX_CTL
    L_MGPIO19 = 104,       // GPIO3_8   C_PINMUX_CTL
    L_MGPIO31 = 104,       // RESERVED
    L_MGPIO47 = 104,       // RESERVED
    L_MGPIO57 = 104,       // RESERVED

    SYS_RSTN =  105,
    RTC_CLK =   106,
    ULP_GPIO0 = 107,       // ULP_GPIO0
    ULP_GPIO1 = 108,       // ULP_GPIO1
    ULP_GPIO2 = 109,       // ULP_GPIO2
    ULP_GPIO3 = 110,       // ULP_GPIO3
    PWR_HOLD =  111,       // ULP_GPIO4
    HRESET =    112,       // ULP_GPIO5
    SLEEP_N =   113,       // ULP_GPIO6
    PMUIC_IRQ = 114,       // ULP_GPIO7
    PIN_NONE =  115,       // used as invalid/unused PIN number
    ULP_PIN = SYS_RSTN,
} pin_t;

/* !< SLAVE CPU */
typedef enum {
    SLAVE_CPU_BT,
    SLAVE_CPU_MAX_NUM,
} slave_cpu_t;

#define PIN_MAX_NUMBER                    PIN_NONE // value USED to iterate in arrays

#define TCXO_COUNT_ENABLE                 NO

#define WATCHDOG_ROM_ENABLE               NO

#define GPIO_WITH_ULP                     NO
#define AON_SPECIAL_PIO                   YES

#define SPI_WITH_OPI                      NO
#define SPI_DMA_TRANSFER_NUM_BY_BYTE      NO

#define DMA_TRANSFER_DEBUG                NO
#define DMA_USE_HIDMA                     NO
#define DMA_WITH_SMDMA                    NO // Small dma
#define DMA_TRANS_BY_LLI                  NO
#define XIP_WITH_OPI                      NO
#define XIP_INT_BY_NMI                    YES
#define EFLASH_SLAVE_NOTIFY_MASTER_BOOTUP NO

#define ENABLE_CPU_TRACE                  1
#define ADC_WITH_AUTO_SCAN                NO

#define OTP_HAS_READ_PERMISSION           YES
#define OTP_HAS_WRITE_PERMISSION          NO
#define OTP_HAS_CLKLDO_VSET               NO

#define SEC_IAMGE_AES_DECRYPT_EN          NO
#define SEC_SUB_RST_BY_SECURITY_CORE      NO
#define TRNG_WITH_SEC_COMMON              NO
#define IS_MAIN_CORE                      NO
#define EXTERNAL_CLOCK_CALIBRATION        YES

#define XIP_EXIST                         NO
#define USE_XIP_INDEX                     1
#define MCPU_INT0_ID                      26
#define UART_BAUD_RATE_DIV_8              NO
#define FIXED_IN_ROM                      NO
#define ENABLE_GPIO_INTERRUPT             YES
#define CLK_AUTO_CG_ENABLE                NO
#define BOOT_ROM_DFR_PRINT                NO
#define CRITICAL_INT_RESTORE              YES
#define TCXO_CLK_DYN_ADJUST               NO

/**
 * @}
 */
#endif
