/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: USB port for bs25 acore.
 * Author: @CompanyNameTag
 * Create:  2023-01-10
 */

#include "common_def.h"
#include "usb_phy.h"
#include "pm_clock.h"
#include "pm_pmu.h"
#include "pm_veto.h"

#include "tcxo.h"

#define REG_BASE_PERI_CTRL              0x20000
#define NMI_REG_BASE                    (REG_BASE_PERI_CTRL + 0xC18)
#define CRG_REG_BASE                    0x59003800
#define IO_REG_BASE                     0xF8A20000
#define USB2_PHY_REG_BASE               0xFF2F3000

// Timer reg base addr.
#define TIMER_BASE_ADDR                 0x52002000
#define TIMER_0_BASE_ADDR               (TIMER_BASE_ADDR + 0x00)
#define TIMER_1_BASE_ADDR               (TIMER_BASE_ADDR + 0x14)
#define TIMER_2_BASE_ADDR               (TIMER_BASE_ADDR + 0x28)
#define TIMER_3_BASE_ADDR               (TIMER_BASE_ADDR + 0x3C)
#define TIMER_SYSCTL_BASE_ADDR          (TIMER_BASE_ADDR + 0xA0)

/* Timer register OFFSET */
#define TIMER_LOAD_COUNT                0x00
#define TIMER_CONTROL_REG               0x08
#define TIMER_EOI                       0x0C

// RTC reg base addr
#define RTC_TIMER_BASE_ADDR             0x57024000

/* RTC register OFFSET */
#define RTC_LOAD_COUNT                  0x00
#define RTC_CONTROL_REG                 0x08
#define RTC_EOI                         0x0C

#define USB_PHY_RST_REG                         (CRG_REG_BASE + 0x300)
#define USB_PHY_RST_RESENT                      4
#define USB_PHY_MODE_REG                        (CRG_REG_BASE + 0x304)
#define USB_PHY_OUT_EN_REG                      (CRG_REG_BASE + 0x308)

#define BT_BASE_ADDR                            0x570000b8
#define BT_VALUE_1                              1
#define BT_VALUE_3                              3
#define BT_VALUE_7                              7

#define M3SRAM_ORIGIN_APP_MAPPING               0xa6000000
#define BT_VECTORS_MAX_LENGTH                   0x205
#define BT_VECTORS_LENGTH                       0x204
#define M3_LOAD_ADDR                            0x98040000

#define START_BT_WAIT_TIME                      300

#define PMU_CTL_GLB_CLKEN_ADDR                  0x52000548
#define PMU_CTL_GLB_CLKEN_USB_PHY_MSK           8
#define PMU_CTL_USB_PHY_CR_ADDR                 0x52000604
#define CR_DIV_NUM_MSK                          5
#define CR_DIV_NUM_LEN                          5
#define CR_DIV_EN_MSK                           4
#define CR_CLK_SEL_MSK                          1
#define CR_CLK_SEL_LEN                          3

#define PMU_CTL_SOFT_USB_GLB_RST_ADDR           0x5700478C
#define PMU_CTL_SOFT_USB_GLB_RST_CLDO_N_MSK     2
#define PMU_CTL_SOFT_USB_GLB_RST_AON_LGC_N      1

#define GLB_CTL_D_USB_SOFT_RST_ADDR             0x57000828
#define GLB_CTL_D_USB_SOFT_RST_AHB_N            1

#define GLB_CTL_D_POR_RESET_ADDR                0x57000900
#define GLB_CTL_D_UTMI_RESET_ADDR               0x57000904

#define CLOCKS_CLK_SRC_TCXO                     2

#define ANA_CFG0                                0x57018000
#define ENABLE_DEEMPHASIS_OPTIMIZE              5
#define RG_HSTX_DEEN_START                      8
#define RG_HSTX_DEEN_LEN                        4
#define DRIVE_CURRENT_OPTIMIZE                  5

#define ANA_CFG2                                0x57018008
#define ENABLE_HALF_BIT_DRIVE                   8

#define ANA_CFG4                                0x57018010
#define RXTHR_LEN                               2

#if defined(PRE_FPGA)
static void usb_pd_pwr_up(void)
{
    uint32_t value;
    uapi_reg_read(PMU_CTL_SOFT_USB_GLB_RST_ADDR, value);
    value |= ((uint32_t)((1U) << (uint32_t)(PMU_CTL_SOFT_USB_GLB_RST_CLDO_N_MSK)));
    value |= (uint32_t)(1U);
    value |= ((uint32_t)((1U) << (uint32_t)(PMU_CTL_SOFT_USB_GLB_RST_AON_LGC_N)));
    uapi_reg_write(PMU_CTL_SOFT_USB_GLB_RST_ADDR, value);

    uapi_reg_read(GLB_CTL_D_USB_SOFT_RST_ADDR, value);
    value |= ((uint32_t)((1U) << (uint32_t)(GLB_CTL_D_USB_SOFT_RST_AHB_N)));
    value |= (uint32_t)(1U);
    uapi_reg_write(GLB_CTL_D_USB_SOFT_RST_ADDR, value);
}

static void clk_usb_phy_cfg(uint32_t clk_src, uint32_t div_num)
{
    /* Close CG. */
    uint32_t value, pos;
    uapi_reg_read(PMU_CTL_GLB_CLKEN_ADDR, value);
    value &= ~((uint32_t)((1U) << (uint32_t)(PMU_CTL_GLB_CLKEN_USB_PHY_MSK)));
    uapi_reg_write(PMU_CTL_GLB_CLKEN_ADDR, value);

    /* Close div_en. */
    uapi_reg_read(PMU_CTL_USB_PHY_CR_ADDR, value);
    value &= ~((uint32_t)((1U) << (uint32_t)(CR_DIV_EN_MSK)));
    value &= ~((uint32_t)(1U));
    uapi_reg_write(PMU_CTL_USB_PHY_CR_ADDR, value);

    /* Select clk_source. */
    uapi_reg_read(PMU_CTL_USB_PHY_CR_ADDR, value);
    value &= ~((((uint32_t)(1U) << (uint32_t)(CR_CLK_SEL_LEN)) - 1) << (uint32_t)(CR_CLK_SEL_MSK));
    pos = (uint32_t)((uint32_t)(clk_src) & (((uint32_t)(1U) << (uint32_t)(CR_CLK_SEL_LEN)) - 1)) <<
          (uint32_t)(CR_CLK_SEL_MSK);
    value |= pos;
    uapi_reg_write(PMU_CTL_USB_PHY_CR_ADDR, value);

    /* Select div_num. */
    if (div_num != 1) {
        uapi_reg_read(PMU_CTL_USB_PHY_CR_ADDR, value);
        value &= ~((((uint32_t)(1U) << (uint32_t)(CR_DIV_NUM_LEN)) - 1) << (uint32_t)(CR_DIV_NUM_MSK));
        pos = (uint32_t)((uint32_t)(div_num) & (((uint32_t)(1U) << (uint32_t)(CR_DIV_NUM_LEN)) - 1)) <<
              (uint32_t)(CR_DIV_NUM_MSK);
        value |= pos;
        uapi_reg_write(PMU_CTL_USB_PHY_CR_ADDR, value);

        uapi_reg_read(PMU_CTL_USB_PHY_CR_ADDR, value);
        value |= ((uint32_t)((1U) << (uint32_t)(CR_DIV_EN_MSK)));
        uapi_reg_write(PMU_CTL_USB_PHY_CR_ADDR, value);
    }
    /* Open enable and CG. */
    uapi_reg_read(PMU_CTL_USB_PHY_CR_ADDR, value);
    value |= (uint32_t)(1U);
    uapi_reg_write(PMU_CTL_USB_PHY_CR_ADDR, value);
    uapi_reg_read(PMU_CTL_GLB_CLKEN_ADDR, value);
    value |= ((uint32_t)((1U) << (uint32_t)(PMU_CTL_GLB_CLKEN_USB_PHY_MSK)));
    uapi_reg_write(PMU_CTL_GLB_CLKEN_ADDR, value);
}
#endif

static void test_suite_usb_phy_reset(void)
{
#if defined(PRE_FPGA)
    uint32_t value;

    uapi_reg_write(USB_PHY_RST_REG, 0);
    uapi_tcxo_delay_us(100);  /* 100: delay time */
    uapi_reg_write(USB_PHY_MODE_REG, 0x10);
    uapi_reg_write(USB_PHY_OUT_EN_REG, 0x1);

    uapi_reg_read(USB_PHY_RST_REG, value);
    value |= ((uint32_t)((1U) << (uint32_t)(USB_PHY_RST_RESENT)));
    value |= (uint32_t)(1U);
    uapi_reg_write(USB_PHY_RST_REG, value);
#else
    uapi_pm_add_sleep_veto(PM_VETO_USB);
    uapi_pmu_control(PMU_CONTROL_PHYLDO_POWER, PMU_CONTROL_POWER_ON);
    uapi_clock_crg_config(CLOCK_CRG_ID_USB_PHY_BUS, CLOCK_CLK_SRC_FNPLL_MCU_HS, 0x4); // 4分频:24M
    uapi_reg_write16(0x5700478C, 0x0);
    uapi_reg_write16(0x57004780, 0x3);
    uapi_tcxo_delay_us(30); /* 30: delay time */
    uapi_reg_write16(0x57004788, 0x0);
    uapi_reg_write16(0x5700478C, 0x7);
    uapi_reg_write16(0x52000604, 0x9D);
    uapi_reg_write16(0x57000910, 0x4);
    uapi_reg_write16(0x57000828, 0x2);
    uapi_reg_write16(0x57000828, 0x3);
    uapi_reg_write16(0x57000900, 0x2);
    uapi_reg_write16(0x57000904, 0x0);
    uapi_reg_write16(0x57000824, 0x2);
    uapi_reg_write16(0x57000824, 0x3);
    uapi_reg_write(0x57018004, 0x95000000);

    uapi_reg_setbit(ANA_CFG0, ENABLE_DEEMPHASIS_OPTIMIZE);
    uapi_reg_setbit(ANA_CFG2, ENABLE_HALF_BIT_DRIVE);
    uapi_reg_setbits(ANA_CFG0, RG_HSTX_DEEN_START, RG_HSTX_DEEN_LEN, DRIVE_CURRENT_OPTIMIZE);

    uapi_reg_setbits(ANA_CFG4, 0, RXTHR_LEN, 0);
    uapi_tcxo_delay_us(300);  /* 300: delay time */
#endif
}

#if defined(PRE_FPGA)
static void low_power_control(void)
{
    uapi_reg_write(BT_BASE_ADDR, BT_VALUE_1);
    uapi_reg_write(BT_BASE_ADDR, BT_VALUE_3);
    int ret = memcpy_s((void *)(M3SRAM_ORIGIN_APP_MAPPING),
                       BT_VECTORS_MAX_LENGTH,
                       (void *)((uintptr_t)(M3_LOAD_ADDR)),
                       BT_VECTORS_LENGTH);
    if (ret != 0) {
        usb_err("%s failed!\n", __FUNCTION__);
        return;
    }

    uapi_reg_write(BT_BASE_ADDR, BT_VALUE_1);
    uapi_reg_write(BT_BASE_ADDR, BT_VALUE_3);
    uapi_reg_write(BT_BASE_ADDR, BT_VALUE_7);
}
#endif

static void usb_phy_power_off(void)
{
    uapi_reg_write16(PMU_CTL_SOFT_USB_GLB_RST_ADDR, 0);
    uapi_reg_write16(GLB_CTL_D_USB_SOFT_RST_ADDR, 0);
    uapi_reg_write16(GLB_CTL_D_POR_RESET_ADDR, 1);
    uapi_reg_write16(GLB_CTL_D_UTMI_RESET_ADDR, 1);
    uapi_reg_write16(0x57000824, 0);
    uapi_reg_write16(0x57004788, 0x3);
    uapi_reg_write16(0x57004780, 0);

    uapi_clock_crg_config(CLOCK_CRG_ID_USB_PHY_BUS, CLOCK_CLK_SRC_MAX, 0x0);
    uapi_pm_remove_sleep_veto(PM_VETO_USB);
}

static void usb_phy_power_on(void)
{
#if defined(PRE_FPGA)
    low_power_control();
#endif
    test_suite_usb_phy_reset();

#if defined(PRE_FPGA)
    usb_pd_pwr_up();
    clk_usb_phy_cfg(CLOCKS_CLK_SRC_TCXO, 2); /* 2: div num */
    osal_mdelay(START_BT_WAIT_TIME);

    uapi_reg_write16(GLB_CTL_D_POR_RESET_ADDR, 0);
    uapi_reg_write16(GLB_CTL_D_UTMI_RESET_ADDR, 0);
#endif
}

void usb_start_hcd(void)
{
    usb_phy_power_on();
}

void usb_stop_hcd(void)
{
    usb_phy_power_off();
}

static const usb_phy_ops_t g_usb_phy_ops = {
    .usbStartHcd            = usb_start_hcd,
    .usbStopHcd             = usb_stop_hcd,
    .usbHost2Device         = NULL,
    .usbDevice2Host         = NULL,
    .usbSuspend             = NULL,
    .usbResume              = NULL,
};

void usb_phy_init(void)
{
    usb_phy_reg(&g_usb_phy_ops);
}
