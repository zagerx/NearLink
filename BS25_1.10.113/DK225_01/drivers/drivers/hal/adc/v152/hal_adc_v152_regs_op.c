/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides V152 adc register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-30， Create file. \n
 */

#include "common_def.h"
#include "hal_adc_v152_regs_op.h"

#define cdac_cali_c13_invalid(x)   (((x) <= 0x400) || ((x) >= 0x1000))
#define ADC_REG_LEN                16

adc_regs_t *g_adc_regs = NULL;
adc_pmu_regs_t *g_adc_pmu_regs = NULL;
adc_aon_regs_t *g_adc_aon_regs = NULL;
adc_diag_regs_t *g_adc_diag_regs = NULL;

errcode_t hal_adc_v152_regs_init(void)
{
    g_adc_regs = (adc_regs_t *)adc_porting_base_addr_get();
    g_adc_pmu_regs = (adc_pmu_regs_t *)adc_pmu_base_addr_get();
    g_adc_aon_regs = (adc_aon_regs_t *)adc_aon_cfg_addr_get();
    g_adc_diag_regs = (adc_diag_regs_t *)adc_diag_cfg_addr_get();
    return ERRCODE_SUCC;
}

void hal_afe_dig_clk_set(cfg_clken_data_t afe_dig_clk)
{
    g_adc_regs->cfg_clken = afe_dig_clk.d32;
}

void hal_gafe_sample_set(hal_gafe_sample_info_t *afe_sample)
{
    g_adc_regs->cfg_clk_div_0 = afe_sample->sample_rate.d32;
    g_adc_regs->cfg_gadc_data_0 = afe_sample->cfg_adc_data0.d32;
    g_adc_regs->cfg_gadc_data_1 = afe_sample->cfg_adc_data1.d32;
    g_adc_regs->cfg_gadc_data_4 = afe_sample->cfg_adc_data4.d32;
    /* The SOC is configured with multiple consecutive 16-bit registers, but only the 32-bit registers can be
       configured based on the bus constraints. */
    writel((uint32_t)&(g_adc_regs->cfg_dreg[0]),
           afe_sample->cfg_dreg_data0.d16 | (afe_sample->cfg_dreg_data1.d16 << ADC_REG_LEN));
    writel((uint32_t)&(g_adc_regs->cfg_dreg[2]),   // cfg_dreg_data2
           afe_sample->cfg_dreg_data2.d16 | (afe_sample->cfg_dreg_data3.d16 << ADC_REG_LEN));
    writel((uint32_t)&(g_adc_regs->cfg_dreg[10]),  // cfg_dreg_data10
           afe_sample->cfg_dreg_data10 | (afe_sample->cfg_dreg_data11 << ADC_REG_LEN));
    writel((uint32_t)&(g_adc_regs->cfg_dreg[12]),  // cfg_dreg_data12
           afe_sample->cfg_dreg_data12 | (afe_sample->cfg_dreg_data13 << ADC_REG_LEN));
    g_adc_regs->diag_cfg_tst[1] = afe_sample->diag_node;
}

void hal_hafe_sample_set(hal_hafe_sample_info_t *afe_sample)
{
    g_adc_regs->cfg_clk_div_1 = afe_sample->sample_rate.d32;
    g_adc_regs->cfg_hadc_data_0 = afe_sample->cfg_adc_data0.d32;
    g_adc_regs->cfg_hadc_data_1 = afe_sample->cfg_adc_data1.d32;
    g_adc_regs->cfg_hadc_data_4 = afe_sample->cfg_adc_data4.d32;
    g_adc_regs->cfg_hadc_ctrl_2 = afe_sample->cfg_hadc_ctrl2.d32;
    /* The SOC is configured with multiple consecutive 16-bit registers, but only the 32-bit registers can be
       configured based on the bus constraints. */
    writel((uint32_t)&(g_adc_regs->cfg_dreg[4]),   // cfg_dreg_data4
           afe_sample->cfg_dreg_data4 | (afe_sample->cfg_dreg_data5 << ADC_REG_LEN));
    writel((uint32_t)&(g_adc_regs->cfg_dreg[6]),   // cfg_dreg_data6
           afe_sample->cfg_dreg_data6 | (afe_sample->cfg_dreg_data7 << ADC_REG_LEN));
    writel((uint32_t)&(g_adc_regs->cfg_dreg[8]),   // cfg_dreg_data8
           afe_sample->cfg_dreg_data8 | (afe_sample->cfg_dreg_data9 << ADC_REG_LEN));
    writel((uint32_t)&(g_adc_regs->cfg_dreg[10]),  // cfg_dreg_data10
           afe_sample->cfg_dreg_data10 | (afe_sample->cfg_dreg_data11 << ADC_REG_LEN));
    writel((uint32_t)&(g_adc_regs->cfg_dreg[12]),  // cfg_dreg_data12
           afe_sample->cfg_dreg_data12 | (afe_sample->cfg_dreg_data13 << ADC_REG_LEN));
    g_adc_regs->diag_cfg_tst[1] = afe_sample->diag_node;
}

void hal_afe_power_on(hal_gafe_sample_info_t *gafe_sample, hal_hafe_sample_info_t *hafe_sample)
{
    cfg_clken_data_t afe_dig_clk;
    cfg_rstn_data_t cfg_rstn;
    cfg_iso_data_t cfg_iso;
    hal_afe_dig_clk_set(afe_dig_clk);
    hal_afe_dig_crg_set(cfg_rstn);
    afe_dig_clk.b.cfg_clken_tst = 1;
    afe_dig_clk.b.cfg_hadc_clken_bc = 1;
    afe_dig_clk.b.cfg_hadc_clken_fc = 1;
    afe_dig_clk.b.cfg_gadc_clken_fc = 1;
    afe_dig_clk.b.cfg_hadc_clken_ctrl = 1;
    afe_dig_clk.b.cfg_gadc_clken_ctrl = 1;
    hal_afe_dig_clk_set(afe_dig_clk);
    hal_gafe_sample_set(gafe_sample);
    hal_hafe_sample_set(hafe_sample);
    afe_dig_clk.b.cfg_hadc_clken_byp = 1;
    afe_dig_clk.b.cfg_gadc_clken_byp = 1;
    hal_afe_dig_clk_set(afe_dig_clk);
    cfg_rstn.b.cfg_rstn_tst = 1;
    cfg_rstn.b.cfg_hadc_rstn_bc = 1;
    cfg_rstn.b.cfg_hadc_rstn_fc = 1;
    cfg_rstn.b.cfg_gadc_rstn_fc = 1;
    cfg_rstn.b.cfg_hadc_rstn_ana = 1;
    cfg_rstn.b.cfg_gadc_rstn_ana = 1;
    hal_afe_dig_crg_set(cfg_rstn);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_50);
    cfg_rstn.b.cfg_hadc_rstn_data = 1;
    cfg_rstn.b.cfg_gadc_rstn_data = 1;
    hal_afe_dig_crg_set(cfg_rstn);
    cfg_iso.b.hadc_iso_en = 0;
    hal_afe_cfg_iso_set(cfg_iso);
}

void hal_gafe_power_off(void)
{
    afe_gadc_cfg_data_t gadc_cfg;
    gadc_cfg.d32 = hal_gadc_cfg_get();
    cfg_rstn_data_t cfg_rstn;
    cfg_rstn.d32 = hal_afe_dig_crg_get();
    gadc_cfg.b.s2d_gadc_en = 0;
    gadc_cfg.b.s2d_gadc_iso_en = 1;
    hal_gadc_cfg_set(gadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    gadc_cfg.b.s2d_gadc_mux_en = 0;
    hal_gadc_cfg_set(gadc_cfg);
    cfg_rstn.b.cfg_gadc_rstn_ana = 0;
    hal_afe_dig_crg_set(cfg_rstn);
}

void hal_hafe_power_off(void)
{
    afe_hadc_cfg_data_t hadc_cfg;
    hadc_cfg.d32 = hal_hadc_cfg_get();
    cfg_rstn_data_t cfg_rstn;
    cfg_rstn.d32 = hal_afe_dig_crg_get();
    hadc_cfg.b.s2d_hadc_iso_en = 1;
    hadc_cfg.b.s2d_hadc_en = 0;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    hadc_cfg.b.s2d_hadc_dcoc_dac_en = 0;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    hadc_cfg.b.s2d_hadc_pga_en = 0;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    hadc_cfg.b.s2d_hadc_ia_en_dly = 0;
    hadc_cfg.b.s2d_hadc_ia_en = 0;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    hadc_cfg.b.s2d_hadc_sensor_en = 0;
    hal_hadc_cfg_set(hadc_cfg);
    cfg_rstn.b.cfg_hadc_rstn_ana = 0;
    hal_afe_dig_crg_set(cfg_rstn);
}

void hal_gafe_iso_on(cfg_dreg_data1_t cfg_dreg_data1)
{
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    afe_gadc_cfg_data_t gadc_cfg;
    gadc_cfg.d32 = hal_gadc_cfg_get();
    gadc_cfg.b.s2d_gadc_mux_en = 1;
    hal_gadc_cfg_set(gadc_cfg);
    gadc_cfg.b.s2d_gadc_iso_en = 0;
    hal_gadc_cfg_set(gadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_50);
    gadc_cfg.b.s2d_gadc_en = 1;
    hal_gadc_cfg_set(gadc_cfg);
    g_adc_regs->cfg_dreg[1] = cfg_dreg_data1.d16;
}

void hal_gafe_iso_off(void)
{
    afe_gadc_cfg_data_t gadc_cfg;
    gadc_cfg.d32 = hal_gadc_cfg_get();
    gadc_cfg.b.s2d_gadc_en = 0;
    hal_gadc_cfg_set(gadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    gadc_cfg.b.s2d_gadc_iso_en = 1;
    gadc_cfg.b.s2d_gadc_mux_en = 0;
    hal_gadc_cfg_set(gadc_cfg);
}

void hal_adc_adldo_pwron(void)
{
    adldo_en_cfg_data_t adldo1_cfg = { 0 };
    adldo_en_cfg_data_t adldo2_cfg = { 0 };
    afe_ldo_cfg_data_t ldo_cfg = { 0 };

    adldo1_cfg.b.en_adldo = 1;
    hal_afe_adldo1_set(adldo1_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_150);
    adldo2_cfg.b.en_adldo = 1;
    hal_afe_adldo2_set(adldo2_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_150);
    adldo1_cfg.b.en_adldo_delay = 1;
    adldo2_cfg.b.en_adldo_delay = 1;
    hal_afe_adldo1_set(adldo1_cfg);
    hal_afe_adldo2_set(adldo2_cfg);
    ldo_cfg.b.s2d_afeldo3_en = 1;
    ldo_cfg.b.s2d_afeldo2_en = 1;
    ldo_cfg.b.s2d_afeldo1_en = 1;
    ldo_cfg.b.s2d_afeldo_iso_en_n = 1;
    hal_afe_ldo_set(ldo_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    ldo_cfg.b.s2d_afeldo3_en_dly = 1;
    ldo_cfg.b.s2d_afeldo2_en_dly = 1;
    ldo_cfg.b.s2d_afeldo1_en_dly = 1;
    hal_afe_ldo_set(ldo_cfg);
}

void hal_adc_gadc_enable(void)
{
    afe_gadc_cfg_data_t gadc_cfg;
    gadc_cfg.b.s2d_gadc_iso_en = 1;
    gadc_cfg.b.s2d_gadc_mux_en = 1;
    hal_gadc_cfg_set(gadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_50);
    gadc_cfg.b.s2d_gadc_iso_en = 0;
    hal_gadc_cfg_set(gadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_50);
    gadc_cfg.b.s2d_gadc_en = 1;
    hal_gadc_cfg_set(gadc_cfg);
    if (adc_get_cali_code_source(AFE_GADC_MODE)) {
        cfg_gadc_data0_t cfg_adc_data0;
        cfg_adc_data0.d32 = g_adc_regs->cfg_gadc_data_0;
        cfg_adc_data0.b.cont_mode = COUNT_MODE;
        g_adc_regs->cfg_gadc_data_0 = cfg_adc_data0.d32;
    }
}

void hal_adc_hadc_enable(void)
{
    afe_hadc_cfg_data_t hadc_cfg;
    hadc_cfg.b.s2d_hadc_iso_en = 1;
    hadc_cfg.b.s2d_hadc_sensor_en = 1;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    hadc_cfg.b.s2d_hadc_ia_en = 1;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_50);
    hadc_cfg.b.s2d_hadc_ia_en_dly = 1;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_5);
    hadc_cfg.b.s2d_hadc_pga_en = 1;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    hadc_cfg.b.s2d_hadc_dcoc_dac_en = 1;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
    hadc_cfg.b.s2d_hadc_iso_en = 0;
    hal_hadc_cfg_set(hadc_cfg);
    hadc_cfg.b.s2d_hadc_en = 1;
    hal_hadc_cfg_set(hadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_30);
}

static bool hal_afe_os_cali_get_sts(afe_scan_mode_t afe_scan_mode)
{
    uint32_t os_cali_status = 0;
    if (afe_scan_mode == AFE_GADC_MODE) {
        os_cali_status = g_adc_regs->rpt_cmp_os_0;
    } else {
        os_cali_status = g_adc_regs->rpt_cmp_os_1;
    }
    os_cali_status = os_cali_status & BIT_0;
    return ((os_cali_status == BIT_0) ? true : false);
}

static bool hal_afe_cdac_cali_get_sts(afe_scan_mode_t afe_scan_mode)
{
    uint32_t cdac_cali_status = 0;
    if (afe_scan_mode == AFE_GADC_MODE) {
        cdac_cali_status = g_adc_regs->rpt_cdac_fc0_0;
    } else {
        cdac_cali_status = g_adc_regs->rpt_cdac_fc0_1;
    }
    cdac_cali_status = cdac_cali_status & BIT_0;
    return ((cdac_cali_status == BIT_0) ? true : false);
}

void hal_adc_v152_os_cali(afe_scan_mode_t afe_scan_mode)
{
    cfg_cmp_os_data_t os_cali;
    if (afe_scan_mode == AFE_GADC_MODE) {
        cfg_adc_cali_ctrl_t gadc_cali;
        gadc_cali.b.cfg_adc_cal_en = 1;
        hal_gafe_cali_ctrl(gadc_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        bool extrenum_symbol = hal_gafe_os_cali_extremum_symbol_get();
        os_cali.b.cfg_gadc_cmp_os_start = 1;
        hal_afe_os_cali_ctrl(os_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        while (!hal_afe_os_cali_get_sts(AFE_GADC_MODE)) { }
        os_cali.b.cfg_gadc_cmp_os_start = 0;
        hal_afe_os_cali_ctrl(os_cali);
        bool extrenum_protect = hal_gafe_os_cali_extremum_protect_status();
        if (extrenum_protect) {
            if (extrenum_symbol) {
                hal_gafe_os_cali_set_extremum_max();
            } else {
                hal_gafe_os_cali_set_extremum_min();
            }
        }
        gadc_cali.b.cfg_adc_cal_en = 0;
        hal_gafe_cali_ctrl(gadc_cali);
    } else {
        cfg_adc_cali_ctrl_t hadc_cali;
        hadc_cali.b.cfg_adc_cal_en = 1;
        hal_hafe_cali_ctrl(hadc_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        bool extrenum_symbol = hal_hafe_os_cali_extremum_symbol_get();
        os_cali.b.cfg_hadc_cmp_os_start = 1;
        hal_afe_os_cali_ctrl(os_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        while (!hal_afe_os_cali_get_sts(AFE_HADC_MODE)) { }
        os_cali.b.cfg_hadc_cmp_os_start = 0;
        hal_afe_os_cali_ctrl(os_cali);
        bool extrenum_protect = hal_hafe_os_cali_extremum_protect_status();
        if (extrenum_protect) {
            if (extrenum_symbol) {
                hal_hafe_os_cali_set_extremum_max();
            } else {
                hal_hafe_os_cali_set_extremum_min();
            }
        }
        hadc_cali.b.cfg_adc_cal_en = 0;
        hal_hafe_cali_ctrl(hadc_cali);
    }
}

static void hal_hafe_cdac_cali_check(void)
{
    if (cdac_cali_c13_invalid(hal_hafe_cdac_cali_get_code16(5))) {  // Cdac cali value C13 is the 5th 16bit value.
        hal_hafe_reduced_cali_accuracy();
        hal_hafe_cdec_cali_set_default_value();
        hal_hafe_cdac_cali_sync();
    }
}

void hal_adc_v152_cdac_cali(afe_scan_mode_t afe_scan_mode)
{
    cfg_cdac_fc0_data_t cdac_cali;
    if (afe_scan_mode == AFE_GADC_MODE) {
        cfg_adc_cali_ctrl_t gadc_cali;
        gadc_cali.b.cfg_adc_cal_en = 1;
        hal_gafe_cali_ctrl(gadc_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        cdac_cali.b.cfg_gadc_cdac_fc_start = 1;
        hal_afe_cdac_cali_ctrl(cdac_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        while (!hal_afe_cdac_cali_get_sts(AFE_GADC_MODE)) { }
        cdac_cali.b.cfg_gadc_cdac_fc_start = 0;
        hal_afe_cdac_cali_ctrl(cdac_cali);
        gadc_cali.b.cfg_adc_cal_en = 0;
        hal_gafe_cali_ctrl(gadc_cali);
    } else {
        cfg_adc_cali_ctrl_t hadc_cali;
        hadc_cali.b.cfg_adc_cal_en = 1;
        hal_hafe_cali_ctrl(hadc_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        hal_hafe_cdac_cali_check();
        cdac_cali.b.cfg_hadc_cdac_fc_start = 1;
        hal_afe_cdac_cali_ctrl(cdac_cali);
        uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_50);
        while (!hal_afe_cdac_cali_get_sts(AFE_HADC_MODE)) { }
        cdac_cali.b.cfg_hadc_cdac_fc_start = 0;
        hal_afe_cdac_cali_ctrl(cdac_cali);
        hadc_cali.b.cfg_adc_cal_en = 0;
        hal_hafe_cali_ctrl(hadc_cali);
    }
}

void hal_adc_v152_dcoc_cali(void)
{
    cfg_dcoc_cal_data2_t dcoc_cali;
    dcoc_cali.b.cfg_hadc_dcoc_cal_iter_cyc = 0x29;
    dcoc_cali.b.cfg_hadc_dcoc_cal_iter_polar = 1;
    hal_afe_dcoc_cali_cfg(dcoc_cali);
    hal_afe_dcoc_compensation_mode_sel(STATIC_CONFIGURATION);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_1000);
    hal_afe_dcoc_cali_ctrl(true);
    uapi_tcxo_delay_ms(HAL_ADC_V152_CFG_DELAY_1000);
    while (!hal_afe_dcoc_cali_get_sts()) { }
    hal_afe_dcoc_cali_ctrl(false);
}

void hal_adc_v152_adldo_vset(void)
{
    hal_afe_adldo1_vset(VOLTAGE_2_6);
    hal_afe_adldo2_vset(VOLTAGE_2_6);
}

void hal_adc_v152_sample_start(adc_channel_t channel, uint32_t sample_data_addr, uint32_t sample_data_len)
{
    hal_afe_diag_set();
    hal_afe_diag_send2aix();
    hal_afe_diag_clk_enable();
    hal_afe_diag_source_sel();
    if (channel == HADC_CHANNEL_0) {
        hal_afe_diag_sample_len(sample_data_len);
        hal_afe_diag_sample_start_addr(sample_data_addr);
        hal_afe_diag_sample_end_addr(sample_data_addr + (sample_data_len * sizeof(uint32_t)));
    }
    hal_afe_diag_set_full_done_mode();
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_150);
    hal_afe_diag_sample_sync();
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_150);
    hal_afe_diag_sample_en();
    if (channel == HADC_CHANNEL_0) {
        while (!hal_afe_diag_get_sts()) {}
    } else {
        while (!hal_gafe_single_sample_get_sts()) {}
    }
}

void hal_gafe_channel_sel(uint8_t ch)
{
    cfg_dreg_data0_t cfg_dreg_data0;
    cfg_dreg_data0.d16 = g_adc_regs->cfg_dreg[0];
    cfg_dreg_data0.b.diff_mode = 0;
    cfg_dreg_data0.b.channel_sel = ch;
    g_adc_regs->cfg_dreg[0] = cfg_dreg_data0.d16;
    afe_gadc_cfg_data_t gadc_cfg;
    gadc_cfg.d32 = hal_gadc_cfg_get();
    gadc_cfg.b.s2d_gadc_en = 0;
    hal_gadc_cfg_set(gadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_10);
    gadc_cfg.b.s2d_gadc_en = 1;
    hal_gadc_cfg_set(gadc_cfg);
    uapi_tcxo_delay_us(HAL_ADC_V152_CFG_DELAY_10);
}