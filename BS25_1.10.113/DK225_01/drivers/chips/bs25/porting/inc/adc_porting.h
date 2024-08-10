/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides adc port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#ifndef ADC_PORTING_H
#define ADC_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "adc.h"
#include "common.h"
#include "errcode.h"
#include "osal_interrupt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_adc ADC
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief  device_bs25_port_adc ADC
 */

#define AFE_ENABLE_INTERRUPT 0
#define AIN_MAX_NUM 7

#define HADC_DEFAULT_CONFIG                                 \
{                                                           \
    .adldo_voltage = VOLTAGE_2_6,                           \
    .adldo_trim = 0,                                        \
    .simo_trim = 0x60,                                      \
    .sample_rate.b.cfg_adc_ana_div_th  = 0xf,               \
    .sample_rate.b.cfg_adc_ana_div_cyc = 0x1f,              \
    .cfg_adc_data0.b.data_num = 0,                          \
    .cfg_adc_data0.b.wait_num = 7,                          \
    .cfg_adc_data0.b.cont_mode = CONTIUNE_MODE,             \
    .cfg_adc_data1.b.osr_len = 7,                           \
    .cfg_adc_data1.b.osr_sel = 7,                           \
    .cfg_adc_data4.b.adc_cdac_fc_scale_div = 0x80,          \
    .cfg_adc_data4.b.adc_cdac_scale_div_sel = 1,            \
    .cfg_hadc_ctrl2.b.cfg_hadc_ia_gain = IA_GAIN_SIXTEEN,   \
    .cfg_hadc_ctrl2.b.cfg_hadc_pga_gain = PGA_GAIN_TWO,     \
    .cfg_dreg_data4 = 0x40D3,                               \
    .cfg_dreg_data5 = 0x1B13,                               \
    .cfg_dreg_data6 = 0x0E44,                               \
    .cfg_dreg_data7 = 0x1BC,                                \
    .cfg_dreg_data8 = 0,                                    \
    .cfg_dreg_data9 = 0x2000,                               \
    .cfg_dreg_data10 = 0x1200,                              \
    .cfg_dreg_data11 = 0x1300,                              \
    .cfg_dreg_data12 = 0x0,                                 \
    .cfg_dreg_data13 = 0x3,                                 \
    .diag_node = HADC_ACCUMULATIVE_AVERAGE_OUTPUT,          \
    .os_cali = true,                                        \
    .cdac_cali = true,                                      \
    .dcoc_cali = true,                                      \
}

#define GADC_DEFAULT_CONFIG                             \
{                                                       \
    .adldo_voltage = VOLTAGE_2_6,                       \
    .adldo_trim = 0,                                    \
    .simo_trim = 0x60,                                  \
    .sample_rate.b.cfg_adc_ana_div_th  = 0xf,           \
    .sample_rate.b.cfg_adc_ana_div_cyc = 0x1f,          \
    .cfg_adc_data0.b.data_num = 3,                      \
    .cfg_adc_data0.b.wait_num = 7,                      \
    .cfg_adc_data0.b.cont_mode = CONTIUNE_MODE,         \
    .cfg_adc_data1.b.osr_len = 3,                       \
    .cfg_adc_data1.b.osr_sel = 3,                       \
    .cfg_adc_data4.b.adc_cdac_fc_scale_div = 0x80,      \
    .cfg_adc_data4.b.adc_cdac_scale_div_sel = 1,        \
    .cfg_dreg_data0.b.diff_mode = 0,                    \
    .cfg_dreg_data1.b.en_buf_local = 1,                 \
    .cfg_dreg_data1.b.en_chop = 1,                      \
    .cfg_dreg_data1.b.res_tuning = 5,                   \
    .cfg_dreg_data1.b.cap_tuning = 6,                   \
    .cfg_dreg_data2.b.vcm_mode = 1,                     \
    .cfg_dreg_data3.b.en_os_range = 1,                  \
    .cfg_dreg_data10 = 0x1200,                          \
    .cfg_dreg_data11 = 0x1300,                          \
    .cfg_dreg_data12 = 0x0,                             \
    .cfg_dreg_data13 = 0x3,                             \
    .diag_node = GADC_ACCUMULATED_AVERAGE_OUTPUT,       \
    .os_cali = true,                                    \
    .cdac_cali = false,                                 \
}

/**
 * @brief  ADC channels definition.
 */
typedef enum adc_channel {
    GADC_CHANNEL_1,
    GADC_CHANNEL_2,
    GADC_CHANNEL_3,
    GADC_CHANNEL_4,
    HADC_CHANNEL_0,
    ADC_CHANNEL_MAX_NUM,
    ADC_CHANNEL_NONE = ADC_CHANNEL_MAX_NUM
} adc_channel_t;

/**
 * @if Eng
 * @brief  ADC auto scan frequency which is public used for all channel.
 * @else
 * @brief  adc自动扫描频率，用于所有通道。
 * @endif
 */
typedef enum {
    HAL_ADC_SCAN_FREQ_2HZ,
    HAL_ADC_SCAN_FREQ_4HZ,
    HAL_ADC_SCAN_FREQ_8HZ,
    HAL_ADC_SCAN_FREQ_16HZ,
    HAL_ADC_SCAN_FREQ_32HZ,
    HAL_ADC_SCAN_FREQ_64HZ,
    HAL_ADC_SCAN_FREQ_128HZ,
    HAL_ADC_SCAN_FREQ_256HZ,
    HAL_ADC_SCAN_FREQ_MAX,
    HAL_ADC_SCAN_FREQ_NONE = HAL_ADC_SCAN_FREQ_MAX
} port_adc_scan_freq_t;

/**
 * @brief  AFE interrupt source.
 */
typedef struct adc_irq {
    uint32_t done_irqn;
    uint32_t alarm_irqn;
} adc_irq_t;

/**
 * @brief  Get the base address of ADC.
 * @return uintptr_t The base address of ADC.
 */
uintptr_t adc_porting_base_addr_get(void);

/**
 * @brief  Get the base address of PMU registers associated with ADC.
 * @return uintptr_t The base address of PMU registers associated with ADC.
 */
uintptr_t adc_pmu_base_addr_get(void);

/**
 * @brief  Get the base address of always-on registers associated with ADC.
 * @return uintptr_t The base address of always-on registers associated with ADC.
 */
uintptr_t adc_aon_cfg_addr_get(void);

/**
 * @brief  Get the base address of DIAG registers associated with ADC.
 * @return uintptr_t The base address of DIAG registers associated with ADC.
 */
uintptr_t adc_diag_cfg_addr_get(void);

/**
 * @brief  Register hal funcs objects into hal_adc module.
 */
void adc_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_adc module.
 */
void adc_port_unregister_hal_funcs(void);

/**
 * @brief  Set the divider number of the peripheral device clock.
 * @param [in] clock The clock which is used for adc sample, adc source clock is 2MHz.
 */
void adc_port_init_clock(adc_clock_t clock);

/**
 * @brief  Set the divider number of the peripheral device clock.
 * @param [in] on Enable or disable.
 */
void adc_port_clock_enable(bool on);

/**
 * @brief  Register the interrupt of adc.
 * @param [in] afe_scan_mode AFE mode to use.
 */
void adc_port_register_irq(afe_scan_mode_t afe_scan_mode);

/**
 * @brief  Unregister the interrupt of adc.
 * @param [in] afe_scan_mode AFE mode which is using.
 */
void adc_port_unregister_irq(afe_scan_mode_t afe_scan_mode);

/**
 * @brief  Power on or power off the peripheral device.
 * @param [in] on Power on or Power off.
 */
void adc_port_power_on(bool on);

/**
 * @brief  Lock of the interrupt.
 * @return The irq status.
 */
inline uint32_t adc_irq_lock(void)
{
    return osal_irq_lock();
}

/**
 * @brief  Unlock of the interrupt.
 * @param [in] irq_sts The irq status to restore.
 */
inline void adc_irq_unlock(uint32_t irq_sts)
{
    osal_irq_restore(irq_sts);
}

/**
 * @brief ADC write calibration code into nv.
 * @param [in] afe_config calibration code.
 * @return errcode_t Write result.
 */
errcode_t adc_write_nv_cfg(afe_config_t *afe_config);

/**
 * @brief Get adc calibration code from nv.
 * @return afe_config_t* ADC calibration code recorded in NV.
 */
afe_config_t *adc_nv_config_get(void);

/**
 * @brief Set adc calibration code into ADC_BASE_ADDR.
 * @param [in] afe_scan_mode AFE mode to set calibration code.
 * @param [in] afe_config ADC calibration code.
 * @return errcode_t Write result.
 */
errcode_t adc_set_cali_code(afe_scan_mode_t afe_scan_mode, afe_config_t *afe_config);

/**
 * @brief Get adc calibration code source.
 * @param [in] afe_scan_mode AFE mode to check cali code source.
 * @return bool True if calibration code is recorded, false if code is calibration during current startup.
 */
bool adc_get_cali_code_source(afe_scan_mode_t afe_scan_mode);

/**
 * @brief ADC calibration api.
 * @param [in]  afe_scan_mode AFE mode to use.
 * @param [in]  os_cali OS calibration enable.
 * @param [in]  cdac_cali CDAC calibration enable.
 * @param [in]  dcoc_cali DCOC calibration enable(Only enable for HADC).
 * @return errcode_t  Calibration result.
 */
errcode_t adc_calibration(afe_scan_mode_t afe_scan_mode, bool os_cali, bool cdac_cali, bool dcoc_cali);

/**
 * @brief HADC PGA gain & IA gain config
 * @param [in]  pga_gain PGA gain multiple.
 * @param [in]  ia_gain IA gain multiple.
 */
void hadc_gain_set(uint8_t pga_gain, uint8_t ia_gain);

/**
 * @brief  Restart cpu trace after ADC sample done.
 */
void hal_cpu_trace_restart(void);
/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif