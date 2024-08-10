/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description:  Application Core Platform Definitions
 * Author: @CompanyNameTag
 * Create:  2021-06-16
 */

#ifndef PLATFORM_CORE_H
#define PLATFORM_CORE_H

#include "product.h"
#include "chip_core_definition.h"

/**
 * @defgroup DRIVER_PLATFORM_CORE CHIP Platform CORE Driver
 * @ingroup DRIVER_PLATFORM
 * @{
 */
#define GLB_CTL_M_RB_BASE        0x57000000
#define GLB_CTL_B_RB_BASE        0x57000400
#define GLB_CTL_D_RB_BASE        0x57000800
#define DISPLAY_CTL_RB_BASE      0x56000000
#define GPU_BASE_ADDR            0x56200000
#define DSS_BASE_ADDR            0x56100000

#define B_CTL_RB_BASE            0x59000000
#define M_CTL_RB_BASE            0x52000000
#define COM_CTL_RB_BASE          0x55000000
#define PMU1_CTL_RB_BASE         0x57004000
#define PMU2_CMU_CTL_RB_BASE     0x57008000
#define ULP_AON_CTL_RB_ADDR      0x5702c000
#define PMU_D_CORE_RB_ADDR       0x5702E000
#define FUSE_CTL_RB_ADDR         0x57028000
#define XO_CORE_TRIM_REG         0x57028308
#define XO_CORE_CTRIM_REG        0x5700819c
#define XIP_CACHE_CTL            0xA3006000
#define NMI_CTL_REG_BASE_ADDR    0x52000700

#define FLASH_START_ADDR 0x10000000

#define UART0_BASE     0x52082000 /* UART_L0 */
#define UART1_BASE     0x52080000 /* UART_H0 */
#define UART2_BASE     0x52081000 /* UART_H1 */
#define DMA_BASE_ADDR  0x52070000 /* M_DMA */
#define SDMA_BASE_ADDR 0x520A0000 /* S_DMA */

/* I2C reg base addr */
#define I2C_BUS_0_BASE_ADDR 0x52083000
#define I2C_BUS_1_BASE_ADDR 0x52084000
#define I2C_BUS_2_BASE_ADDR 0x52085000
#define I2C_BUS_3_BASE_ADDR 0x52086000
#define I2C_BUS_4_BASE_ADDR 0x52087000

/* spi reg base addr */
#define SPI_BUS_0_BASE_ADDR 0x52088000 // SPI_M0
#define SPI_BUS_1_BASE_ADDR 0x52089000 // SPI_MS_1
#define SPI_BUS_2_BASE_ADDR 0x5208A000 // SPI_MS_2
#define SPI_BUS_3_BASE_ADDR 0x5208B000 // SPI_MS_3
#define SPI_BUS_4_BASE_ADDR 0x5208C000 // SPI_S_4
#define SPI_BUS_5_BASE_ADDR 0xA3000000 // QSPI_0
#define SPI_BUS_6_BASE_ADDR 0xA3002000 // QSPI_1

#define I2S_BUS_0_BASE_ADDR 0x5203003c

#define DMA_HANDSHAKE_I2C_BUS_0_TX HAL_DMA_HANDSHAKING_I2C0_TX
#define DMA_HANDSHAKE_I2C_BUS_0_RX HAL_DMA_HANDSHAKING_I2C0_RX
#define DMA_HANDSHAKE_I2C_BUS_1_TX HAL_DMA_HANDSHAKING_I2C1_TX
#define DMA_HANDSHAKE_I2C_BUS_1_RX HAL_DMA_HANDSHAKING_I2C1_RX
#define DMA_HANDSHAKE_I2C_BUS_2_TX HAL_DMA_HANDSHAKING_I2C2_TX
#define DMA_HANDSHAKE_I2C_BUS_2_RX HAL_DMA_HANDSHAKING_I2C2_RX
#define DMA_HANDSHAKE_I2C_BUS_3_TX HAL_DMA_HANDSHAKING_I2C3_TX
#define DMA_HANDSHAKE_I2C_BUS_3_RX HAL_DMA_HANDSHAKING_I2C3_RX
#define DMA_HANDSHAKE_I2C_BUS_4_TX HAL_DMA_HANDSHAKING_I2C4_TX
#define DMA_HANDSHAKE_I2C_BUS_4_RX HAL_DMA_HANDSHAKING_I2C4_RX

#define HAL_SPI_DEVICE_MODE_SET_REG   (*(volatile unsigned short *)(0x52000828))
#define HAL_SPI3_MODE_SET_REG         (M_CTL_RB_BASE + 0x950)

/* PWM reg base addr */
#define PWM_0_BASE 0x52000840
#define PWM_1_BASE 0x52000860
#define PWM_2_BASE 0x52000880
#define PWM_3_BASE 0x520008A0
#define PWM_4_BASE 0x520008C0
#define PWM_5_BASE 0x520008E0

/* PWM INTR REG */
#define PWM_INTR_ENABLE_REG (*(volatile unsigned short *)0x52000900)
#define PWM_INTR_CLEAR_REG  (*(volatile unsigned short *)0x52000904)
#define PWM_INTR_STATUS_REG (*(volatile unsigned short *)0x52000908)

// GPIO regs
#define GPIO0_BASE_ADDR 0x57010000
#define GPIO1_BASE_ADDR 0x57014000
#define GPIO2_BASE_ADDR 0x57018000
#define GPIO3_BASE_ADDR 0x5701C000
#define GPIO4_BASE_ADDR 0x57020000
#define ULP_GPIO_BASE_ADDR 0x57030000 // ULP GPIO

// GPIO select core
#define HAL_GPIO_D_CORE_SET_REG             0x570001B0
#define HAL_GPIO_NON_D_CORE_SET_REG         0x57000180
#define HAL_GPIO_CORE_SET_CHANNEL_OFFSET    0x08
#define HAL_GPIO_CORE_SET_REG_OFFSET        2
#define HAL_GPIO_CORE_SET_GPIOS             16

// ULP GPIO int clk config
#define HAL_GPIO_ULP_AON_GP_REG                 0x5702C010
#define HAL_GPIO_ULP_AON_PCLK_INT_EN_BIT        0
#define HAL_GPIO_ULP_AON_PCLK_INT_CLK_SEL_BIT   1
#define HAL_GPIO_ULP_PCLK_INTR_STATUS_BITS      0x3

#define RTC_TIMER_BASE_ADDR 0x57024000

#define SYSTICK_BASE_ADDR 0x5702C330

#define TCXO_COUNT_BASE_ADDR 0x55000580
#define WDT_BASE_ADDR 0x52003000
#define CHIP_WDT_BASE_ADDRESS 0x57034000

#define HAL_SOFT_RST_CTL_BASE               (GLB_CTL_M_RB_BASE)
#define HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET    0x168
#define HAL_CHIP_WDT_ATOP1_RST_BIT          1

// SEC BASE ADDR
#define SEC_CTL_RB_BASE 0x52009000
#define RSAV2_S_RB_BASE 0x52009900
#define TRNG_RB_BASE 0x52009800

// Timer reg base addr.
#define TIMER_BASE_ADDR                         0x52002000
#define TIMER_0_BASE_ADDR                       (TIMER_BASE_ADDR + 0x00)
#define TIMER_1_BASE_ADDR                       (TIMER_BASE_ADDR + 0x14)
#define TIMER_2_BASE_ADDR                       (TIMER_BASE_ADDR + 0x28)
#define TIMER_3_BASE_ADDR                       (TIMER_BASE_ADDR + 0x3C)
#define TIMER_SYSCTL_BASE_ADDR                  (TIMER_BASE_ADDR + 0xA0)

#define TICK_TIMER_BASE_ADDR                    TIMER_3_BASE_ADDR

// cpu trace memory
#define TRACE_MEM_REGION_START                  MCPU_TRACE_MEM_REGION_START
#define TRACE_MEM_REGION_LENGTH                 CPU_TRACE_MEM_REGION_LENGTH

/*
 * Maximum UART buses
 * Defined here rather than in the uart_bus_t enum, due to needing to use it for conditional compilation
 */
#define UART_BUS_MAX_NUMBER 3  // !< Max number of UARTS available
#define I2C_BUS_MAX_NUMBER  5  // !< Max number of I2C available
#define TIMER_MAX_AVAILABLE_NUMBER    4  // !< Max number of timer available
#define RTC_MAX_AVAILABLE_NUMBER      4  // !< Max number of rtc available

#define SPI_BUS_MAX_NUMBER  7  // !< Max number of SPI available
#define GPIO_MAX_NUMBER     2  // !< Max number of GPIO available

#define I2S_MAX_NUMBER      1

#define PDM_PIN_DATA          S_MGPIO30
#define PDM_PIN_CLK           S_MGPIO31

#define PDM_PIO_FUNC_DATA     HAL_PIO_FUNC_PDM_M1
#define PDM_PIO_FUNC_CLK      HAL_PIO_FUNC_PDM_M1

#define QDEC_CONFIG_PIO_A     S_MGPIO27
#define QDEC_CONFIG_PIO_B     S_MGPIO28
#define QDEC_CONFIG_PIO_LED   S_MGPIO18

#define QDEC_CONFIG_PINMUX_A     HAL_PIO_FUNC_QDEC
#define QDEC_CONFIG_PINMUX_B     HAL_PIO_FUNC_QDEC
#define QDEC_CONFIG_PINMUX_LED   HAL_PIO_FUNC_QDEC

#define S_DMA_CHANNEL_MAX_NUM    4  // !< Max number of SM_DMA available
#define B_DMA_CHANNEL_MAX_NUM    8  // !< Max number of M_DMA available

#define DMA_CHANNEL_MAX_NUM      (S_DMA_CHANNEL_MAX_NUM + B_DMA_CHANNEL_MAX_NUM)

#define CHIP_BCPU_SWDDIO  0
#define CHIP_BCPU_SWDCLK  0

#ifdef SW_UART_DEBUG
#define DEBUG_UART_BUS   UART_BUS_0  // UART L0
#elif defined(TEST_SUITE)
#define DEBUG_UART_BUS   UART_BUS_1  // UART H0
#endif

#define TEST_SUITE_UART_BUS DEBUG_UART_BUS

#ifdef USE_EMBED_FLASH
/* Test Suite UART Transmission PIN to use */
#define TEST_SUITE_UART_TX_PIN S_AGPIO12
/* Test Suite UART Reception PIN to use */
#define TEST_SUITE_UART_RX_PIN S_AGPIO13
#else
/* Test Suite UART Transmission PIN to use */
#define TEST_SUITE_UART_TX_PIN S_MGPIO23
/* Test Suite UART Reception PIN to use */
#define TEST_SUITE_UART_RX_PIN S_MGPIO21
#endif

#ifdef PRE_ASIC
#ifdef SW_UART_DEBUG
#ifdef USE_EMBED_FLASH
    #define CHIP_FIXED_RX_PIN S_AGPIO13
    #define CHIP_FIXED_TX_PIN S_AGPIO12
#else
    #define CHIP_FIXED_RX_PIN S_MGPIO21
    #define CHIP_FIXED_TX_PIN S_MGPIO23
#endif
#endif

#else
    #define CHIP_FIXED_RX_PIN S_AGPIO14
    #define CHIP_FIXED_TX_PIN S_AGPIO15
#endif

#define SW_DEBUG_UART_BUS DEBUG_UART_BUS

#define CHIP_FIXED_UART_BUS UART_BUS_0

#define CODELOADER_UART_BUS UART_BUS_0
#define CODELOADER_UART_TX_PIN S_AGPIO15
#define CODELOADER_UART_RX_PIN S_AGPIO14

#define LOGR_UART_BUS UART_BUS_0
#define LOG_UART_TX_PIN S_AGPIO15
#define LOG_UART_RX_PIN S_AGPIO14

#define LOG_UART_BUS       UART_BUS_2

#define QSPI_MAX_NUMBER    2
#define FLASH_QSPI_ID      QSPI_BUS_1
#define QSPI_0_BASE_ADDR   0xA3000000
#define QSPI_1_BASE_ADDR   0xA3002000

/* !< QSPI bus */
typedef enum {
    QSPI_BUS_0 = 0,
    QSPI_BUS_1,
    QSPI_BUS_NONE = QSPI_MAX_NUMBER
} qspi_bus_t;

/**
 * @brief  UART bus.
 */
typedef enum {
    UART_BUS_0 = 0,  // !< UART L0
#if UART_BUS_MAX_NUMBER > 1
    UART_BUS_1 = 1,  // !< UART H0
#endif
#if UART_BUS_MAX_NUMBER > 2
    UART_BUS_2 = 2,  // !< UART L2
#endif
    UART_BUS_NONE = UART_BUS_MAX_NUMBER  // !< Value used as invalid/unused UART number
} uart_bus_t;

/**
 * @brief  I2C bus.
 */
typedef enum {
    I2C_BUS_0,               // !< I2C0
    I2C_BUS_1,               // !< I2C1
    I2C_BUS_2,               // !< I2C2
    I2C_BUS_3,
    I2C_BUS_4,
    I2C_BUS_NONE = I2C_BUS_MAX_NUMBER
} i2c_bus_t;

/***************************************
For BS25 config:
    SPI_M0,
    SPI_MS_1,
    SPI_MS_2,
    SPI_MS_3,
    SPI_S_4,
    QSPI_0,
    QSPI_1
***************************************/

/**
 * @brief SPI Bus.
 *  - SPI_M0,
 *  - SPI_MS_1,
 *  - SPI_MS_2,
 *  - SPI_MS_3,
 *  - SPI_S_4,
 *  - QSPI_0,
 *  - QSPI_1
 */
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

/**
 * @brief SIO(I2S/PCM) Bus.
 *
 */
typedef enum {
    SIO_BUS_0,
    SIO_NONE = I2S_MAX_NUMBER
} sio_bus_t;

/* !< SLAVE CPU */
typedef enum {
    SLAVE_CPU_BT,
    SLAVE_CPU_MAX_NUM,
} slave_cpu_t;

/**********************************************************************/
/************************* MPU config base addr ***********************/
/**********************************************************************/
// config register region
#define MPU_REG_ADDR0_BASE           0x50000000
#define MPU_REG_ADDR0_LEN            0x10000000

#define MPU_REG_ADDR1_BASE           0xA3000000
#define MPU_REG_ADDR1_LEN            0x01000000

// ROM region
#define MPU_ROM_ADDR_BASE               0x0
#define MPU_ROM_ADDR_LEN                0x8000

// ITCM region
#define MPU_ITCM_ADDR_BASE              0x80000
#define MPU_ITCM_ADDR_LEN               0x80000

// L2ram region
#define MPU_L2RAM_ADDR0_BASE            0x100000
#define MPU_L2RAM_ADDR0_LEN             0x100000
#define MPU_L2RAM_ADDR1_BASE            0x200000
#define MPU_L2RAM_ADDR1_LEN             0x100000

// XIP PSRAM read & execute region
#define MPU_XIP_PSRAM_RO_ADDR_BASE      0x08000000
#define MPU_XIP_PSRAM_RO_ADDR_LEN       0x04000000

// XIP PSRAM bypass(read/write) region
#define MPU_XIP_PSRAM_RW_ADDR_BASE      0x0C000000
#define MPU_XIP_PSRAM_RW_ADDR_LEN       0x04000000

// XIP NorFlash region
#define MPU_XIP_FLASE_RO_ADDR_BASE      0x10000000
#define MPU_XIP_FLASE_RO_ADDR_LEN       0x10000000

// Sharemem region
#define MPU_SHAREMEM_ADDR_BASE          0x87000000
#define MPU_SHAREMEM_ADDR_LEN           0x10000

// MDMA(m0) address judge
#define MEM_X2P_MEMORY_START    0xA3000000
#define MEM_X2P_MEMORY_END      0xA3008FFF
#define L2RAM_MEMORY_START      0x00100000
#define L2RAM_MEMORY_END        0x0035FFFF
#define QSPI_XIP_MEMORY_START   0x08000000
#define QSPI_XIP_MEMORY_END     0x1FFFFFFF

// CHIP RESET offset address
#define CHIP_RESET_OFF   0x600

// Power on the flash memory on the FPGA
#define PMU_RESERV1      0x570040C8

/**
 * @brief Definition of pin.
 */
typedef enum {
    S_MGPIO0  = 0, // GPIO0_0   0 = GPIO0_0, 1 = QSPI1_D0, 3 = SPI0_DO, 7 = DIAG0
    S_MGPIO1  = 1, // GPIO0_1   0 = GPIO0_1, 1 = QSPI1_D1, 3 = SPI0_DI, 7 = DIAG1
    S_MGPIO2  = 2, // GPIO0_2   0 = GPIO0_2, 1 = QSPI1_D2, 2 = SPI0_DI, 7 = DIAG2
    S_MGPIO3  = 3, // GPIO0_3   0 = GPIO0_3, 1 = QSPI1_D3, 2 = SPI0_DO, 7 = DIAG3
    S_MGPIO4  = 4, // GPIO0_4   0 = GPIO0_4, 1 = QSPI1_CLK, 2 = SPI0_CLK, 7 = DIAG4
    S_MGPIO5  = 5, // GPIO0_5   0 = GPIO0_5, 1 = QSPI1_CS0, 2 = SPI0_CS0, 7 = DIAG5
    S_MGPIO6  = 6, // GPIO0_6   0 = GPIO0_6, 1 = SPI0_DI, 2 = SPI1_DI, 3 = I2C1_SCL, 5 = SPI4_DI, 7 = DIAG6
    S_MGPIO7  = 7, // GPIO0_7   0 = GPIO0_7, 1 = SPI0_DO, 2 = SPI1_DO, 3 = I2C1_SDA, 5 = SPI4_DO, 7 = DIAG7
    S_MGPIO8  = 8, // GPIO0_8   0 = GPIO0_8, 1 = SPI0_CLK, 2 = SPI1_CLK, 3 = I2C2_SCL, 5 = SPI4_CLK, 7 = DIAG8
    S_MGPIO9  = 9, // GPIO0_9   0 = GPIO0_9, 1 = SPI0_CS0, 2 = SPI0_CS0, 3 = I2C2_SDA, 5 = SPI4_CS0, 7 = DIAG9
    S_MGPIO10 = 10, // GPIO0_10 0 = GPIO0_10, 1 = SPI0_CS1, 2 = UART_L0_RXD, 7 = DIAG10
    S_MGPIO11 = 11, // GPIO0_11 0 = GPIO0_11, 1 = SPI0_CS2, 2 = UART_L0_TXD, 6 = PULSE_CAPTURE, 7 = DIAG11
    S_MGPIO12 = 12, // GPIO0_12 0 = GPIO0_12, 1 = SPI1_DI, 2 = I2C3_SCL, 3 = UART_L1_RXD, 4 = KEY_OUT_0, 5 = SPI4_DI,
                    // 6 = BT_ACTIVE, 7= DIAG12
    S_MGPIO13 = 13, // GPIO0_13 0 = GPIO0_13, 1 = SPI1_DO, 2 = I2C3_SDA, 3 = UART_L1_TXD, 4 = KEY_OUT_1, 5 = SPI4_DO,
                    // 6 = BT_STATUS_0, 7= DIAG13
    S_MGPIO14 = 14, // GPIO0_14 0 = GPIO0_14, 1 = SPI1_CLK, 2 = I2C4_SCL, 3 = I2C2_SCL, 4 = KEY_OUT_2, 5 = SPI4_CLK,
                    // 6 = BT_STATUS_1, 7= DIAG14
    S_MGPIO15 = 15, // GPIO0_15 0 = GPIO0_15, 1 = SPI1_CS0, 2 = I2C4_SDA, 3 = I2C2_SDA, 4 = KEY_OUT_3, 5 = SPI4_CS0,
                    // 6 = BT_FREQ, 7= DIAG15
    S_MGPIO16 = 16, // GPIO0_16 0 = GPIO0_16, 1 = SPI2_DI, 2 = I2S0_CLK, 3 = QDEC_A, 4 = KEY_OUT_4, 5 = RF_TX_ASSOCLK,
                    // 6 = WLAN_ACTIVE, 7= DIAG16
    S_MGPIO17 = 17, // GPIO0_17 0 = GPIO0_17, 1 = SPI2_DO, 2 = I2S0_WS, 3 = QDEC_B, 4 = KEY_OUT_5,
                    // 5 = BT_RF_TDC_SIGN_TEST, 6 = BT_SP_SAMP_P, 7= DIAG0
    S_MGPIO18 = 18, // GPIO0_18 0 = GPIO0_18, 1 = SPI2_CLK, 2 = I2S0_DO, 3 = QDEC_LED, 4 = KEY_OUT_6, 5 = I2C2_SDA,
                    // 6 = BT_BT_FRAME_SYNC, 7= DIAG1
    S_MGPIO19 = 19, // GPIO0_19 0 = GPIO0_19, 1 = SPI2_CS0, 2 = I2S0_DI, 4 = KEY_OUT_7, 5 = I2C2_SCL,
                    // 6 = BTS_SAMPLE, 7= DIAG2
    S_MGPIO20 = 20, // GPIO0_20 0 = GPIO0_20, 1 = SPI3_DI, 2 = UART_H0_CTS, 3 = PWM0P, 4 = PWM0N, 5 = SPI4_DI,
                    // 6 = BTS_SAMPLE, 7= DIAG3
    S_MGPIO21 = 21, // GPIO0_21 0 = GPIO0_21, 1 = SPI3_DO, 2 = UART_H0_RXD, 3 = PWM1P, 4 = PWM1N, 5 = SPI4_DO,
                    // 6 = BT_WIFI_SW, 7= DIAG4
    S_MGPIO22 = 22, // GPIO0_22 0 = GPIO0_22, 1 = SPI3_CLK, 2 = UART_H0_RTS, 3 = PWM2P, 4 = PWM2N, 5 = SPI4_CLK,
                    // 6 = EXTLNA_CTRL, 7= DIAG5
    S_MGPIO23 = 23, // GPIO0_23 0 = GPIO0_23, 1 = SPI3_CS0, 2 = UART_H0_TXD, 3 = PWM3P, 4 = PWM3N, 5 = SPI4_CS0,
                    // 6 = EXTLNA_RX_EN, 7= DIAG6
    S_MGPIO24 = 24, // GPIO0_24 0 = GPIO0_24, 1 = I2C0_SCL, 6 = STB0_SPI_DI, 7 = DIAG3,
    S_MGPIO25 = 25, // GPIO0_25 0 = GPIO0_25, 1 = I2C0_SDA, 6 = STB0_SPI_DO, 7 = DIAG4,
    S_MGPIO26 = 26, // GPIO0_26 0 = GPIO0_26, 1 = I2C0_SCL, 2 = UART_L2_TXD, 4 = PDM_CLK0, 6 = STB0_SPI_CLK 7 = DIAG8,
    S_MGPIO27 = 27, // GPIO0_27 0 = GPIO0_27, 1 = I2C1_SDA, 2 = UART_L2_RXD, 3 = QDEC_A, 4 = PDM_D0,
                    // 6 = STB0_SPI_CS 7 = DIAG9,
    S_MGPIO28 = 28, // GPIO0_28 0 = GPIO0_28, 1 = I2C1_SCL, 2 = KEY_IN_0, 3 = QDEC_B, 4 = UART_L0_RXD, 7 = DIAG10,
    S_MGPIO29 = 29, // GPIO0_29 0 = GPIO0_29, 1 = I2C1_SDA, 2 = KEY_IN_1, 3 = QDEC_LED, 4 = UART_L0_TXD, 7 = DIAG11,
    S_MGPIO30 = 30, // GPIO0_30 0 = GPIO0_30, 1 = PDM_D0, 2 = KEY_IN_2, 3 = PWM3P, 4 = PWM3N, 7 = DIAG8,
    S_MGPIO31 = 31, // GPIO0_31 0 = GPIO0_31, 1 = PDM_CLK0, 2 = KEY_IN_3, 3 = PWM4P, 4 = PWM4N, 7 = DIAG9,

    S_AGPIO0 =  32, // GPIO0_32 0 = GPIO1_0, 1 = CLKIN, 2 = KEY_IN_4, 3 = PWM5P, 4 = PWM5N,
                    // 6 = PULSE_CAPTURE, 7 = DIAG10,
    S_AGPIO1 =  33, // GPIO0_33 0 = GPIO1_1, 1 = I2S0_CLK, 2 = KEY_IN_5, 3 = UART_H0_TXD, 4 = PDM_CLK0, 7 = DIAG11,
    S_AGPIO2 =  34, // GPIO0_34 0 = GPIO1_2, 1 = I2S0_WS, 2 = KEY_IN_6, 3 = UART_H0_RXD, 4 = PDM_D0, 7 = DIAG12,
    S_AGPIO3 =  35, // GPIO0_35 0 = GPIO1_3, 1 = I2S0_DO, 2 = KEY_IN_7, 3 = UART_H0_RTS, 4 = BT_GLP_SYNC, 5 = PWM0N,
                    // 6 = PULSE_CAPTURE, 7 = DIAG13,
    S_AGPIO4 =  36, // GPIO0_36 0 = GPIO1_4, 1 = I2S0_DI, 2 = KEY_IN_8, 3 = UART_H0_CTS, 5 = PWM1N,
                    // 6 = PULSE_CAPTURE, 7 = DIAG14,
    S_AGPIO5 =  37, // GPIO0_37 0 = GPIO1_5, 1 = UART_H0_RTS, 2 = KEY_IN_9, 3 = PWM0P, 4 = SPI4_DI, 5 = QSPI0_D0
                    // 6 = PULSE_CAPTURE, 7 = DIAG15,
    S_AGPIO6 =  38, // GPIO0_38 0 = GPIO1_6, 1 = UART_H0_CTS, 2 = KEY_IN_10, 3 = PWM1P, 4 = SPI4_DO, 5 = QSPI0_D1
                    // 6 = PULSE_CAPTURE, 7 = DIAG4,
    S_AGPIO7 =  39, // GPIO0_39 0 = GPIO1_7, 1 = CLKOUT0, 2 = KEY_IN_11, 3 = PWM2P, 4 = PWM2N
                    // 6 = JTAG_nTRST, 7 = DIAG0,
    S_AGPIO8 =  40, // GPIO0_40 0 = GPIO1_8, 1 = UART_L2_TXD, 2 = KEY_IN_12, 3 = PWM2P, 4 = PWM2N, 5 = QSPI0_CLK
                    // 6 = JTAG_TDO, 7 = DIAG15,
    S_AGPIO9 =  41, // GPIO0_41 0 = GPIO1_9, 1 = UART_L2_RXD, 2 = KEY_IN_13, 3 = PWM3P, 4 = PWM3N, 5 = QSPI0_CS0
                    // 6 = JTAG_TDI, 7 = DIAG17,
    S_AGPIO10 = 42, // GPIO0_42 0 = GPIO1_10, 2 = SWDIO, 6 = JTAG_TMS, 7 = DIAG1,
    S_AGPIO11 = 43, // GPIO0_43 0 = GPIO1_11, 2 = SWDCLK, 6 = JTAG_TCK, 7 = DIAG2,
    S_AGPIO12 = 44, // GPIO0_44 0 = GPIO1_12, 1 = UART_H0_TXD, 2 = KEY_IN_14, 3 = UART_L1_RTS, 4 = SPI4_CLK,
                    // 5 = QSPI0_D2, 6 = UART_L1_TXD, 7 = DIAG15,
    S_AGPIO13 = 45, // GPIO0_45 0 = GPIO1_13, 1 = UART_H0_RXD, 2 = KEY_IN_15, 3 = UART_L1_CTS, 4 = SPI4_CS0,
                    // 5 = QSPI0_D3, 6 = UART_L1_RXD, 7 = DIAG12,
    S_AGPIO14 = 46, // GPIO0_46 0 = GPIO1_14, 1 = UART_L0_RXD, 2 = KEY_IN_16, 3 = UART_L1_RXD, 7 = DIAG15,
    S_AGPIO15 = 47, // GPIO0_47 0 = GPIO1_15, 1 = UART_L0_TXD, 2 = KEY_IN_17, 3 = UART_L1_TXD, 7 = DIAG0,
    PIN_NONE =  48, // used as invalid/unused PIN number
} pin_t;

#define TCXO_COUNT_ENABLE                 YES

#define PIN_MAX_NUMBER                    PIN_NONE // value USED to iterate in arrays

#define WATCHDOG_ROM_ENABLE               YES

#define GPIO_WITH_ULP                     NO
#define GPIO_FUNC                         HAL_PIO_FUNC_GPIO

#define I2C_AUTO_SEND_STOP_CMD            NO
#define I2C_WITH_BUS_RECOVERY             YES

#define SPI_WITH_OPI                      NO
#define SPI_DMA_TRANSFER_NUM_BY_BYTE      NO

#define DMA_TRANSFER_DEBUG                YES
#define DMA_USE_HIDMA                     NO
#define DMA_WITH_MDMA                     YES // dma
#define DMA_WITH_SMDMA                    YES // Small dma
#define DMA_TRANS_BY_LLI                  NO

#define XIP_WITH_OPI                      YES
#define XIP_INT_BY_NMI                    YES
#define FLASH_SLAVE_NOTIFY_MASTER_BOOTUP YES

#define ENABLE_CPU_TRACE                  2
#define ADC_WITH_AUTO_SCAN                YES

#define OTP_HAS_READ_PERMISSION           YES
#define OTP_HAS_WRITE_PERMISSION          YES
#define OTP_HAS_CLKLDO_VSET               NO

#define SEC_IAMGE_AES_DECRYPT_EN          NO
#define SEC_BOOT_SIGN_CHECK_EN            YES
#define SEC_SUB_RST_BY_SECURITY_CORE      NO
#define TRNG_WITH_SEC_COMMON              YES
#define IS_MAIN_CORE                      YES
#define EXTERNAL_CLOCK_CALIBRATION        YES
#define AON_SPECIAL_PIO                   YES

#define SUPPORT_HI_EMMC_PHY               NO

#define OPI_PIN_FIX_DM1_DRIVER            NO
#define OPI_USE_MCU_HS_CLK                NO
#define QSPI0_FUNC  HAL_PIO_FUNC_QSPI0
#define QSPI0_D0    S_MGPIO0
#define QSPI0_D1    S_MGPIO1
#define QSPI0_D2    S_MGPIO2
#define QSPI0_D3    S_MGPIO3
#define QSPI0_CLK   S_MGPIO4
#define QSPI0_CS    S_MGPIO5

#ifdef ATE_FLASH_CHECK
#define QSPI1_FUNC  HAL_PIO_FUNC_QSPI0
#define QSPI1_D0    S_MGPIO0
#define QSPI1_D1    S_MGPIO1
#define QSPI1_D2    S_MGPIO2
#define QSPI1_D3    S_MGPIO3
#define QSPI1_CLK   S_MGPIO4
#define QSPI1_CS    S_MGPIO5
#else
#define QSPI1_FUNC  HAL_PIO_FUNC_QSPI1
#define QSPI1_D0    S_AGPIO5
#define QSPI1_D1    S_AGPIO6
#define QSPI1_D2    S_AGPIO12
#define QSPI1_D3    S_AGPIO13
#define QSPI1_CLK   S_AGPIO8
#define QSPI1_CS    S_AGPIO9
#endif

#define XIP_EXIST                         YES
#define CHIP_FLASH_ID                     1
#define CHIP_DATA_FLASH_ID                0
#define USE_XIP_INDEX                     1
#define BCPU_INT0_ID                      26
#define UART_BAUD_RATE_DIV_8              NO
#define FIXED_IN_ROM                      NO
#define PMU_LPM_WAKEUP_SRC_NUM            16
#define ENABLE_GPIO_INTERRUPT             YES
#define CLK_AUTO_CG_ENABLE                NO
#if defined(BUILD_APPLICATION_ROM)
#define BOOT_ROM_DFR_PRINT                YES
#else
#define BOOT_ROM_DFR_PRINT                NO
#endif
#define FLASH_WRITE_CLK_DIV_AUTO_ADJ     NO
#define DMA_WITH_MUX_CHANNEL              YES
#define CRITICAL_INT_RESTORE              YES
#define SUPPORT_SEC_TRNG                  NO
#define TCXO_CLK_DYN_ADJUST              NO
#define DCACHE_IS_ENABLE                  NO
#define CODELOADER_SINGLE_PARTITION_EXP   YES
#define AUXLDO_ENABLE_FLASH               NO
/**
 * @}
 */
#endif
