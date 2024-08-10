/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:   CLOCKS DRIVER.
 * Author: @CompanyNameTag
 * Create:
 */

#include "clock_calibration.h"
#include "clocks_core.h"
#include "clocks_switch.h"
#include "pm_clock.h"
#include "clocks.h"

#define PMU_D_CORE_NP_CLK_OSC32K_CTRL0_REG          (PMU_D_CORE_RB_ADDR + 0x834)
#define PM_OSC32K_DISABLE_VAL0                      0x5A
#define PMU_D_CORE_NP_CLK_OSC32K_CTRL1_REG          (PMU_D_CORE_RB_ADDR + 0x838)
#define PM_OSC32K_DISABLE_VAL1                      0xF0
#define PMU_D_CORE_RC_32K_NOR_LP_SEL_REG            (PMU_D_CORE_RB_ADDR + 0x89C)
#define PM_NP_RC_32K_NOR_LP_SEL_BIT                 0

#if (RC_CLOCK_ON == YES)
#define PM_ULP_AON_CTL_RC_32K_TRIM_REG  (ULP_AON_CTL_RB_ADDR + 0x324)
#define PM_RC32K_MAX_TRIM_VAL           0x3F
// 32K时钟周期数，0x40即2ms
#define PM_CLK_32K_DET_VAL              0x40
#define PM_RC32K_MIN_CALI_FREQ          3104000
#define PM_RC32K_MAX_CALI_FREQ          3296000

static void clocks_rc32k_calibration(void)
{
    volatile uint32_t freq = calibration_get_clock_frq(PM_CLK_32K_DET_VAL);
    if ((freq >= PM_RC32K_MIN_CALI_FREQ) && (freq <= PM_RC32K_MAX_CALI_FREQ)) { // 误差范围内
        return;
    }
    volatile uint16_t trim_val = 0;
    volatile uint16_t trim_val_record = readw(PM_ULP_AON_CTL_RC_32K_TRIM_REG);

    do { // 手动校准
        writew(PM_ULP_AON_CTL_RC_32K_TRIM_REG, trim_val++);
        freq = calibration_get_clock_frq(PM_CLK_32K_DET_VAL);
        if ((freq >= PM_RC32K_MIN_CALI_FREQ) && (freq <= PM_RC32K_MAX_CALI_FREQ)) { // 误差范围内
            return;
        }
    } while (trim_val <= PM_RC32K_MAX_TRIM_VAL);
    // 没找到正确校准值
    writew(PM_ULP_AON_CTL_RC_32K_TRIM_REG, trim_val_record);
}
#endif

#if defined(BUILD_APPLICATION_STANDARD)
void clocks_system_init(void)
{
    clocks_app_clk_init();
}
#else
void clocks_system_init(void)
{
    clocks_ssb_clk_init();
}
#endif

void clocks_hardware_sub_init(void)
{
    // MCU enter sleep, disable DMA/mbus/mcpu_h/mcpu clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_CODEC_CH_SEL_SLP, TURN_OFF);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_HIFI_CH_SEL_SLP, TURN_OFF);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_COM_BUS_CH_SEL_SLP, TURN_OFF);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MCU_CORE_CH_SEL_SLP, TURN_OFF); // when MCU sleep, enable mcu core clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_COM_BUS_CLKEN_SLP, TURN_ON); // when MCU sleep, enable com bus clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_SDMA_CLKEN_SLP, TURN_OFF); // when MCU sleep, enable sdma clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MDMA_CLKEN_SLP, TURN_OFF); // when MCU sleep, disable DMA clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MBUS_CLKEN_SLP, TURN_ON); // when MCU sleep, enable mbus clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MCPU_HCLKEN_SLP, TURN_OFF); // when MCU sleep, enable mcpu_hclk clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MCPU_CLKEN_SLP, TURN_OFF); // when MCU sleep, enable mcpu_clk clk
}

#if defined(BUILD_APPLICATION_STANDARD)
static void clocks_mclken_config_init(void)
{
    // BS25 Reserved.
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_DISPLAY_BUS_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_IMG_DEC_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SDIO_DEV_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_IR_APB_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_BT_TGTWS_CLKEN);
    // Re-enable when in use.
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_PWM_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_I2S_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SEC_CLKEN);
    // SPI.
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SPI0_M_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SPI1_M_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SPI2_M_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SPI3_MS_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_SPI4_S_CLKEN);
    // I2C.
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_I2C0_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_I2C1_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_I2C2_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_I2C3_CLKEN);
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_I2C4_CLKEN);
#if defined(USE_EMBED_FLASH)
    uapi_clock_control(CLOCK_CONTROL_MCLKEN_DISABLE, CLOCK_APERP_QSPI_1CS_CLKEN);
#endif
}

void clocks_init(void)
{
    // com_dll2 close
    writew(0x52000540, 0x6);
#if (RC_CLOCK_ON == YES)
    pm_clock_switch_to_rc32k();
    clocks_rc32k_calibration();
#endif

    clocks_system_init();
    clocks_mclken_config_init();
    clocks_hardware_sub_init();
    // AFE_DIG_CFG_CLKEN close
    writew(0x57036004, 0x0);
}
#else
void clocks_init(void)
{
    return;
}
#endif

void pm_clock_switch_to_rc32k(void)
{
    writew(PMU_D_CORE_NP_CLK_OSC32K_CTRL0_REG, PM_OSC32K_DISABLE_VAL0); // Close xo32k.
    writew(PMU_D_CORE_NP_CLK_OSC32K_CTRL1_REG, PM_OSC32K_DISABLE_VAL1);
    reg16_clrbit(PMU_D_CORE_RC_32K_NOR_LP_SEL_REG, PM_NP_RC_32K_NOR_LP_SEL_BIT); // Switch to normal rc32k.
}