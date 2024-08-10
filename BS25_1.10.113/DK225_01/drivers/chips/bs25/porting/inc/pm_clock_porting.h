/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pm clock port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-01-12， Create file. \n
 */
#ifndef PM_CLOCK_PORTING_H
#define PM_CLOCK_PORTING_H


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_pm_clock PM clock
 * @ingroup  drivers_port_pm
 * @{
 */

typedef enum clock_crg_id {
    CLOCK_CRG_ID_MCU_CORE,
    CLOCK_CRG_ID_MCU_PERP_LS,
    CLOCK_CRG_ID_MCU_PERP_UART,
    CLOCK_CRG_ID_MCU_PERP_SPI,
    CLOCK_CRG_ID_COM_BUS,
    CLOCK_CRG_ID_MEM_BUS,
    CLOCK_CRG_ID_USB_PHY_BUS,
    CLOCK_CRG_ID_PDM,
    CLOCK_CRG_ID_XIP_OPI,
    CLOCK_CRG_ID_XIP_QSPI,
    CLOCK_CRG_ID_PAD_OUT0,
    CLOCK_CRG_ID_MAX,
} clock_crg_id_t;

#define CLOCK_CRG_ID_BT_CPU CLOCK_CRG_ID_MAX

typedef enum clock_clken_id {
    CLOCK_CLKEN_ID_MCU_CORE,
    CLOCK_CLKEN_ID_MCU_PERP_LS,
    CLOCK_CLKEN_ID_MCU_PERP_UART,
    CLOCK_CLKEN_ID_MCU_PERP_SPI,
    CLOCK_CLKEN_ID_COM_BUS,
    CLOCK_CLKEN_ID_MEM_BUS,
    CLOCK_CLKEN_ID_USB_PHY_BUS,
    CLOCK_CLKEN_ID_PDM,
    CLOCK_CLKEN_ID_XIP_OPI,
    CLOCK_CLKEN_ID_XIP_QSPI,
    CLOCK_CLKEN_ID_PAD_OUT0,
    CLOCK_CLKEN_ID_MAX,
} clock_clken_id_t;

typedef enum clock_clksel_id {
    CLOCK_CLKSEL_ID_MAX,
} clock_clksel_id_t;

typedef enum clock_clkdiv_id {
    CLOCK_CLKDIV_ID_MAX,
} clock_clkdiv_id_t;

typedef enum clock_clk_src {
    CLOCK_CLK_SRC_RC = 0,
    CLOCK_CLK_SRC_32K = 1,
    CLOCK_CLK_SRC_TCXO = 2,
    CLOCK_CLK_SRC_TCXO_2X = 3,
    CLOCK_CLK_SRC_ULPFLL_MCU_CORE = 4,
    CLOCK_CLK_SRC_FNPLL_MCU_HS = 5,
    CLOCK_CLK_SRC_PAD_CLKIN = 6,
    CLOCK_CLK_SRC_MAX,
} clock_clk_src_t;

typedef enum clock_control_type {
    CLOCK_CONTROL_FREQ_LEVEL_CONFIG,    /* For clock frequency configuration, see @ref clock_freq_level_t */
    CLOCK_CONTROL_MCLKEN_ENABLE,        /* For the parameter types, see @ref clock_mclken_aperp_type_t */
    CLOCK_CONTROL_MCLKEN_DISABLE,       /* For the parameter types, see @ref clock_mclken_aperp_type_t */
    CLOCK_CONTROL_XO_CLKOUT_ENABLE,     /* For the parameter types, see @ref clock_xo_clkout_type_t */
    CLOCK_CONTROL_XO_CLKOUT_DISABLE,    /* For the parameter types, see @ref clock_xo_clkout_type_t */
    CLOCK_CONTROL_GLB_CLKEN_ENABLE,     /* For the parameter types, see @ref clock_glb_clken_type_t */
    CLOCK_CONTROL_GLB_CLKEN_DISABLE,    /* For the parameter types, see @ref clock_glb_clken_type_t */
    CLOCK_CONTROL_BTH_FREQ_CONFIG,      /* For clock frequency configuration, see @ref clock_freq_level_t */
    CLOCK_CONTROL_BT_CORE_FREQ_CONFIG,  /* For clock frequency configuration, see @ref clock_mcu_ctl_bt_t */
    CLOCK_CONTROL_TYPE_MAX,
} clock_control_type_t;

typedef enum clock_glb_clken_type {
    CLOCK_GLB_CLKEN_MCU_PERP_LS     = 1,
    CLOCK_GLB_CLKEN_MCU_PERP_UART   = 2,
    CLOCK_GLB_CLKEN_MCU_PERP_SPI    = 3,
    CLOCK_GLB_CLKEN_MEM_BUS         = 5,
    CLOCK_GLB_CLKEN_XIP_OPI         = 6,
    CLOCK_GLB_CLKEN_XIP_QSPI        = 7,
    CLOCK_GLB_CLKEN_USB_PHY         = 8,
    CLOCK_GLB_CLKEN_USB_BUS         = 9,
    CLOCK_GLB_CLKEN_PDM             = 10,
} clock_glb_clken_type_t;

typedef enum clock_xo_clkout_type {
    CLOCK_D_CLK2ADPLL_EN = 0,
    CLOCK_D_CLK2FNPLL_EN = 1,
    CLOCK_D_CLK2RFADC_EN = 2,
    CLOCK_D_CLKN2AFE_EN  = 3,
    CLOCK_D_CLKN2OUT_EN  = 4,
    CLOCK_D_CLKP2AFE_EN  = 5,
    CLOCK_D_CLKP2OUT_EN  = 6,
} clock_xo_clkout_type_t;

typedef enum clock_mclken_aperp_type {
    // M_CLKEN0
    CLOCK_APERP_SEC_CLKEN               = 0,
    CLOCK_APERP_MDIAG_CLKEN             = 1,
    CLOCK_APERP_I2S_CLKEN               = 2,
    CLOCK_APERP_DISPLAY_BUS_CLKEN       = 3,
    CLOCK_APERP_MTOP_GLUE_TRIGGER_CLKEN = 6,
    CLOCK_APERP_PWM_CLKEN               = 7,
    CLOCK_APERP_IMG_DEC_CLKEN           = 8,
    CLOCK_APERP_UART_L0_CLKEN           = 9,
    CLOCK_APERP_SDIO_DEV_CLKEN          = 11,
    CLOCK_APERP_IR_APB_CLKEN            = 12,
    CLOCK_APERP_X2H_MEM_BUS_CLKEN       = 13,
    // M_CLKEN1
    CLOCK_APERP_MTIMER_CLKEN            = 16,
    CLOCK_APERP_SPI0_M_CLKEN            = 17,
    CLOCK_APERP_SPI1_M_CLKEN            = 18,
    CLOCK_APERP_SPI2_M_CLKEN            = 19,
    CLOCK_APERP_UART_H0_CLKEN           = 20,
    CLOCK_APERP_UART_L2_CLKEN           = 21,
    CLOCK_APERP_I2C0_CLKEN              = 22,
    CLOCK_APERP_I2C1_CLKEN              = 23,
    CLOCK_APERP_I2C2_CLKEN              = 24,
    CLOCK_APERP_I2C3_CLKEN              = 25,
    CLOCK_APERP_QSPI_1CS_CLKEN          = 26,
    CLOCK_APERP_SPI3_MS_CLKEN           = 27,
    CLOCK_APERP_SPI4_S_CLKEN            = 28,
    CLOCK_APERP_XIP_BUS_CLKEN           = 29,
    CLOCK_APERP_I2C4_CLKEN              = 30,
    CLOCK_APERP_BT_TGTWS_CLKEN          = 31,
} clock_mclken_aperp_type_t;

typedef enum clock_freq_level {
    CLOCK_FREQ_LEVEL_LOW_POWER,         // MCU: 32M
    CLOCK_FREQ_LEVEL_NORMAL,            // MCU: 64M
    CLOCK_FREQ_LEVEL_HIGH_PERFORMANCE,  // MCU: 86M
} clock_freq_level_t;

typedef enum clock_vset_level {
    CLOCK_VSET_LEVEL_0V9,
    CLOCK_VSET_LEVEL_1V0,
    CLOCK_VSET_LEVEL_1V1,
} clock_vset_level_t;

typedef enum clock_mcu_ctl_bt {
    CLOCK_MCU_CTL_BT_AUTO  = 0x0,
    CLOCK_MCU_CTL_BT_32M   = 0x20,
    CLOCK_MCU_CTL_BT_64M   = 0x40,
} clock_mcu_ctl_bt_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif

