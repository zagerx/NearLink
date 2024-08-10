/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides V151 HAL PDM \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-02, Create file. \n
 */
#include <stdbool.h>
#include "securec.h"
#include "errcode.h"
#include "common_def.h"
#include "pdm_porting.h"
#include "hal_pdm_v151_regs_op.h"
#include "hal_pdm_v151.h"

#define PDM_TWO_FREQUENCY_DIVIDER     0x2

static hal_pdm_config_t g_hal_pdm_config[HAL_PDM_MIC_MAX_NUM] = { 0 };

static void hal_pdm_v151_soft_reset(bool reset)
{
    if (reset) {
        hal_pdm_v151_codec_sw_rst_n_set_codec_sw_rst_n(0);
        hal_pdm_v151_codec_sw_rst_n_set_rst_2dmic_access_irq(1);
    } else {
        hal_pdm_v151_codec_sw_rst_n_set_codec_sw_rst_n(1);
        hal_pdm_v151_codec_sw_rst_n_set_rst_2dmic_access_irq(0);
    }
}

static void hal_pdm_v151_clk_enbable(hal_pdm_mic_t mic)
{
    if (mic == HAL_PDM_DMIC_4) {
        hal_pdm_v151_codec_clk_en0_set_mic4_up_afifo_clken(1);
        hal_pdm_v151_codec_clk_en0_set_mic4_pga_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic4_srcdn_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic4_adc_clken(1);
    } else if (mic == HAL_PDM_DMIC_5) {
        hal_pdm_v151_codec_clk_en0_set_mic5_up_afifo_clken(1);
        hal_pdm_v151_codec_clk_en0_set_mic5_pga_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic5_srcdn_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic5_adc_clken(1);
    } else {
        hal_pdm_v151_codec_clk_en0_set_mic4_up_afifo_clken(1);
        hal_pdm_v151_codec_clk_en0_set_mic4_pga_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic4_srcdn_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic4_adc_clken(1);

        hal_pdm_v151_codec_clk_en0_set_mic5_up_afifo_clken(1);
        hal_pdm_v151_codec_clk_en0_set_mic5_pga_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic5_srcdn_clken(1);
        hal_pdm_v151_codec_clk_en1_set_mic5_adc_clken(1);
    }
    hal_pdm_v151_codec_clk_en2_set_dmic0_clken(1);
}

static void hal_pdm_v151_clk_disable(void)
{
    hal_pdm_v151_codec_clk_en0_set(0);
    hal_pdm_v151_codec_clk_en1_set(0);
    hal_pdm_v151_codec_clk_en2_set(0);
}

static void hal_pdm_v151_config_mic_pga_internal(uint8_t mic_ll, hal_pdm_config_t *attr)
{
    hal_pdm_v151_mic_pga_ctrl_set_pga_linear_sel(mic_ll, attr->linear_select);
    hal_pdm_v151_mic_pga_ctrl_set_pga_zero_num(mic_ll, attr->zero_num);
    hal_pdm_v151_mic_pga_ctrl_set_pga_thre_id(mic_ll, attr->threshold_id);
    hal_pdm_v151_mic_pga_ctrl_set_pga_noise_en(mic_ll, attr->noise_enable);
    hal_pdm_v151_mic_pga_ctrl_set_pga_bypass(mic_ll, attr->pga_bypass_enable);
    hal_pdm_v151_mic_pga_ctrl_set_pga_fade_out(mic_ll, attr->fade_out_time);
    hal_pdm_v151_mic_pga_ctrl_set_pga_fade_in(mic_ll, attr->fade_in_time);
    hal_pdm_v151_mic_pga_ctrl_set_pga_cfg_small_sig_en(mic_ll, attr->little_signal);
    hal_pdm_v151_mic_pga_ctrl_set_pga_cfg_anti_clip_en(mic_ll, attr->anti_clip);
    hal_pdm_v151_mic_pga_ctrl_set_pga_cfg_fade_en(mic_ll, attr->fade_in_out);
    hal_pdm_v151_mic_pga_ctrl_set_pga_gain(mic_ll, attr->pga_gain);
}

static void hal_pdm_v151_config_mic_pga(hal_pdm_mic_t mic, hal_pdm_config_t *attr)
{
    if (mic == HAL_PDM_DMIC_4) {
        hal_pdm_v151_config_mic_pga_internal(PDM_V151_DMIC4, attr);
    } else if (mic == HAL_PDM_DMIC_5) {
        hal_pdm_v151_config_mic_pga_internal(PDM_V151_DMIC5, attr);
    } else {
        hal_pdm_v151_config_mic_pga_internal(PDM_V151_DMIC4, attr);
        hal_pdm_v151_config_mic_pga_internal(PDM_V151_DMIC5, attr);
    }
}

static void hal_pdm_v151_config_srcdn(hal_pdm_mic_t mic, hal_pdm_config_t *attr)
{
    if (mic == HAL_PDM_DMIC_4) {
        hal_pdm_v151_srcdn_ctrl0_set_mic4_srcdn_src_mode((uint32_t)attr->srcdn_src_mode);
        hal_pdm_v151_srcdn_ctrl0_set_mic4_srcdn_fifo_clr(0);
    } else if (mic == HAL_PDM_DMIC_5) {
        hal_pdm_v151_srcdn_ctrl0_set_mic5_srcdn_src_mode((uint32_t)attr->srcdn_src_mode);
        hal_pdm_v151_srcdn_ctrl0_set_mic5_srcdn_fifo_clr(0);
    } else {
        hal_pdm_v151_srcdn_ctrl0_set_mic4_srcdn_src_mode((uint32_t)attr->srcdn_src_mode);
        hal_pdm_v151_srcdn_ctrl0_set_mic4_srcdn_fifo_clr(0);

        hal_pdm_v151_srcdn_ctrl0_set_mic5_srcdn_src_mode((uint32_t)attr->srcdn_src_mode);
        hal_pdm_v151_srcdn_ctrl0_set_mic5_srcdn_fifo_clr(0);
    }
}

static void hal_pdm_v151_config_up_afifo(hal_pdm_mic_t mic)
{
    if (mic == HAL_PDM_DMIC_4) {
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_upfifo_aempty_th(CONFIG_PDM_AFIFO_AEMPTY_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_upfifo_afull_th(CONFIG_PDM_AFIFO_AFULL_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_up_fifo_clr(0);
    } else if (mic == HAL_PDM_DMIC_5) {
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_upfifo_aempty_th(CONFIG_PDM_AFIFO_AEMPTY_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_upfifo_afull_th(CONFIG_PDM_AFIFO_AFULL_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_up_fifo_clr(0);
    } else {
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_upfifo_aempty_th(CONFIG_PDM_AFIFO_AEMPTY_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_upfifo_afull_th(CONFIG_PDM_AFIFO_AFULL_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic4_up_fifo_clr(0);

        hal_pdm_v151_mic_up_afifo_ctrl_set_mic5_upfifo_aempty_th(CONFIG_PDM_AFIFO_AEMPTY_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic5_upfifo_afull_th(CONFIG_PDM_AFIFO_AFULL_TH);
        hal_pdm_v151_mic_up_afifo_ctrl_set_mic5_up_fifo_clr(0);
    }
}

static void hal_pdm_v151_config_fs_ctrl(hal_pdm_mic_t mic, hal_pdm_config_t *attr)
{
    if (mic == HAL_PDM_DMIC_4) {
        hal_pdm_v151_fs_ctrl0_set_fs_mic4_afifo((uint32_t)attr->fs_ctrl_freq);
        hal_pdm_v151_fs_ctrl1_set_fs_mic4_srcdn_dout((uint32_t)attr->fs_ctrl_freq);
    } else if (mic == HAL_PDM_DMIC_5) {
        hal_pdm_v151_fs_ctrl0_set_fs_mic5_afifo((uint32_t)attr->fs_ctrl_freq);
        hal_pdm_v151_fs_ctrl1_set_fs_mic5_srcdn_dout((uint32_t)attr->fs_ctrl_freq);
    } else {
        hal_pdm_v151_fs_ctrl0_set_fs_mic4_afifo((uint32_t)attr->fs_ctrl_freq);
        hal_pdm_v151_fs_ctrl1_set_fs_mic4_srcdn_dout((uint32_t)attr->fs_ctrl_freq);

        hal_pdm_v151_fs_ctrl0_set_fs_mic5_afifo((uint32_t)attr->fs_ctrl_freq);
        hal_pdm_v151_fs_ctrl1_set_fs_mic5_srcdn_dout((uint32_t)attr->fs_ctrl_freq);
    }
}

static void hal_pdm_v151_config_adc_offset(hal_pdm_mic_t mic, uint32_t adc_dc_offset)
{
    if (mic == HAL_PDM_DMIC_4) {
        hal_pdm_v151_codec_adc_dc_offset_set_adc_dc_offset(PDM_V151_DMIC4, adc_dc_offset);
    } else if (mic == HAL_PDM_DMIC_5) {
        hal_pdm_v151_codec_adc_dc_offset_set_adc_dc_offset(PDM_V151_DMIC5, adc_dc_offset);
    } else {
        hal_pdm_v151_codec_adc_dc_offset_set_adc_dc_offset(PDM_V151_DMIC4, adc_dc_offset);
        hal_pdm_v151_codec_adc_dc_offset_set_adc_dc_offset(PDM_V151_DMIC5, adc_dc_offset);
    }
}

static void hal_pdm_v151_config_cic_gain(hal_pdm_mic_t mic, uint8_t cic_gain, uint8_t lp_cic_gain)
{
    if (mic == HAL_PDM_DMIC_4) {
        hal_pdm_v151_codec_adc_cic_gain_set_adc4_cic_gain(cic_gain);
        hal_pdm_v151_lp_cic_gain1_set_cicdn_adc4_gain(lp_cic_gain);
    } else if (mic == HAL_PDM_DMIC_5) {
        hal_pdm_v151_codec_adc_cic_gain_set_adc5_cic_gain(cic_gain);
        hal_pdm_v151_lp_cic_gain1_set_cicdn_adc5_gain(lp_cic_gain);
    } else {
        hal_pdm_v151_codec_adc_cic_gain_set_adc4_cic_gain(cic_gain);
        hal_pdm_v151_lp_cic_gain1_set_cicdn_adc4_gain(lp_cic_gain);

        hal_pdm_v151_codec_adc_cic_gain_set_adc5_cic_gain(cic_gain);
        hal_pdm_v151_lp_cic_gain1_set_cicdn_adc5_gain(lp_cic_gain);
    }
}

static errcode_t hal_pdm_v151_init(void)
{
    return hal_pdm_regs_init();
}

static void hal_pdm_v151_deinit(void)
{
    hal_pdm_regs_deinit();
}

static errcode_t hal_pdm_v151_set_attr(uint8_t mic, hal_pdm_config_t *attr)
{
    if (unlikely(attr == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    hal_pdm_v151_soft_reset(true);
    hal_pdm_v151_codec_clk_div0_set_clk_6144k_div_ratio(PDM_TWO_FREQUENCY_DIVIDER);
    hal_pdm_v151_clk_enbable((hal_pdm_mic_t)mic);
    hal_pdm_v151_config_mic_pga((hal_pdm_mic_t)mic, attr);

    hal_pdm_v151_config_srcdn((hal_pdm_mic_t)mic, attr);
    hal_pdm_v151_dmic_div_set_fs_dmic0(HALF_CLK);
    hal_pdm_v151_config_up_afifo((hal_pdm_mic_t)mic);
    hal_pdm_v151_config_fs_ctrl((hal_pdm_mic_t)mic, attr);
    hal_pdm_v151_config_adc_offset((hal_pdm_mic_t)mic, CONFIG_PDM_ADC_DC_OFFSET);
    hal_pdm_v151_config_cic_gain((hal_pdm_mic_t)mic, CONFIG_PDM_CIC_GAIN, CONFIG_PDM_LP_CIC_GAIN);

    (void)memcpy_s(&g_hal_pdm_config[mic], sizeof(hal_pdm_config_t), attr, sizeof(hal_pdm_config_t));

    return ERRCODE_SUCC;
}

static void hal_pdm_v151_get_attr(uint8_t mic, hal_pdm_config_t *attr)
{
    if (attr == NULL) {
        return;
    }

    (void)memcpy_s(attr, sizeof(hal_pdm_config_t), &g_hal_pdm_config[mic], sizeof(hal_pdm_config_t));
}

static errcode_t hal_pdm_v151_start(void)
{
    hal_pdm_v151_soft_reset(false);
    return ERRCODE_SUCC;
}

static errcode_t hal_pdm_v151_stop(void)
{
    hal_pdm_v151_clk_disable();
    return ERRCODE_SUCC;
}

static uintptr_t hal_pdm_v151_get_fifo_addr(void)
{
    return hal_pdm_v151_fifo_get_addr();
}

static uint32_t hal_pdm_v151_get_fifo_deepth(void)
{
    return hal_pdm_v151_fifo_get_deepth();
}

static hal_pdm_funcs_t g_hal_pdm_funcs = {
    .init               = hal_pdm_v151_init,
    .deinit             = hal_pdm_v151_deinit,
    .set_attr           = hal_pdm_v151_set_attr,
    .get_attr           = hal_pdm_v151_get_attr,
    .start              = hal_pdm_v151_start,
    .stop               = hal_pdm_v151_stop,
    .get_fifo_addr      = hal_pdm_v151_get_fifo_addr,
    .get_fifo_deepth    = hal_pdm_v151_get_fifo_deepth
};

hal_pdm_funcs_t *hal_pdm_v151_funcs_get(void)
{
    return &g_hal_pdm_funcs;
}