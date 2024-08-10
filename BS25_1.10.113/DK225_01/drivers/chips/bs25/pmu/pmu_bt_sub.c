/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: hal pmu bt ldo src file. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-5-5, Create file. \n
 */
#include "non_os.h"
#include "flash.h"
#include "hal_xip.h"
#include "securec.h"
#include "chip_io.h"
#include "osal_interrupt.h"
#include "tcxo.h"
#include "pmu_bt_sub.h"

#ifdef USE_EMBED_FLASH
#define M3_LOAD_ADDR (BT_XIP_REMAP + 0x80000000)
#else
#define M3_LOAD_ADDR (BT_FLASH_REGION_START)
#endif

#define BT_POWER_EN_DELAY     30ULL

void pmu_bt_onoff_ctrl(bool en)
{
    if (en) {
        // BT_SUB 复位
        writew(CLB_CTL_M_SOFT_RST_2_REG, 0);
        // b_soc power on
        reg32_setbit(PMU1_B_SOC_PWR_EN_CFG_REG, B_SOC_PWR_EN);
        uapi_tcxo_delay_us(BT_POWER_EN_DELAY);
        while (reg32_getbit(PMU1_B_SOC_PWR_EN_CFG_REG, B_SOC_PWR_ACK) != 1) {}
        reg32_clrbit(PMU1_B_SOC_PWR_EN_CFG_REG, B_SOC_ISO_EN);

        // BMEM power on
        reg32_setbit(PMU1_BMEM_PWR_EN_CFG_REG, BMEM_PWR_EN);
        uapi_tcxo_delay_us(BT_POWER_EN_DELAY);
        while (reg32_getbit(PMU1_BMEM_PWR_EN_CFG_REG, BMEM_PWR_ACK) != 1) {}
        reg32_clrbit(PMU1_BMEM_PWR_EN_CFG_REG, BMEM_ISO_EN);

        reg32_setbit(CLB_CTL_M_SOFT_RST_2_REG, SOFT_RET_GLB_B_CRG_N_BIT);
        reg32_setbit(CLB_CTL_M_SOFT_RST_2_REG, SOFT_RET_GLB_B_LGC_N_BIT);
        uint32_t status = osal_irq_lock();
        if(memcpy_s((void *)(M3_SRAM_ORIGIN_APP_MAPPING), BT_VECTORS_LENGTH, \
                    (void *)((uintptr_t)(M3_LOAD_ADDR)), BT_VECTORS_LENGTH) == EOK) {
            osal_irq_restore(status);
            reg32_setbit(CLB_CTL_M_SOFT_RST_2_REG, SOFT_RET_GLB_B_CPU_N_BIT);
        } else {
            return;
        }
    } else {
        // BT_SUB 复位
        writew(CLB_CTL_M_SOFT_RST_2_REG, 0);
        // b_soc power off
        reg32_clrbit(PMU1_B_SOC_PWR_EN_CFG_REG, B_SOC_PWR_EN);
        reg32_setbit(PMU1_B_SOC_PWR_EN_CFG_REG, B_SOC_ISO_EN);
        // BMEM power off
        reg32_clrbit(PMU1_BMEM_PWR_EN_CFG_REG, BMEM_PWR_EN);
        reg32_setbit(PMU1_BMEM_PWR_EN_CFG_REG, BMEM_ISO_EN);
    }
}