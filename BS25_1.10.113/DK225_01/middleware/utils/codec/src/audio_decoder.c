/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Application core os initialize interface header for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#include "common_def.h"
#include "audio_dec_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern void *overlay_dec_entry(void);
static audio_adec_inst g_adec_inst;
static uint8_t g_dec_private_data[32];
static uint32_t g_consume_data_size = 256;
static uint32_t g_product_data_size = 0;

static inline audio_adec_inst *adec_get_inst(void)
{
    return &g_adec_inst;
}

uint32_t audio_get_decoder_consume_data_size(void)
{
    return g_consume_data_size;
}

uint32_t audio_get_decoder_product_data_size(void)
{
    return g_product_data_size;
}

void audio_set_decoder_consume_data_size(uint32_t data_size)
{
#ifdef ERROR_DETECT
    printf("dec consume data is %d\n", data_size);
#endif
    g_consume_data_size = data_size;
}

void audio_dec_set_product_data_size(uint32_t data_size)
{
#ifdef ERROR_DETECT
    printf("dec product data is %d\n", data_size);
#endif
    g_product_data_size = data_size;
}

static ha_codec_decode *adec_get_lib_inst(void)
{
    ha_codec_decode *dev = overlay_dec_entry();
    return dev;
}

static int32_t adec_get_default_private_attr(ha_codec_decode *codec, acodec_get_private_param *arg, adec_attr *attr)
{
    int32_t ret;

    if (codec->send_cmd == NULL) {
        return ERR_HACODEC_NULL_PTR;
    }

    arg->codec_id = attr->codec_id;
    arg->private_data = attr->open_param.private_data;
    arg->private_data_size = attr->open_param.private_data_size;

    arg->private_attr = (void *)&g_dec_private_data;
    arg->private_attr_size = sizeof(g_dec_private_data);

    attr->open_param.private_data = arg;
    attr->open_param.private_data_size = sizeof(*arg);

    ret = codec->send_cmd(NULL, ACODEC_PRIVATE_PARAM_GET_CMD, (void *)arg);
    if (ret != AUDIO_SUCCESS) {
        printf("adec no need to get private para, return %x\n", ret);
    }

    return AUDIO_SUCCESS;
}

int32_t adec_attr_set(int32_t codec_id, adec_attr *attr, ha_codec_dec_param *open_param)
{
    attr->codec_id = codec_id;
    attr->open_param.channels = open_param->channels;
    attr->open_param.bit_depth = open_param->bit_depth;
    attr->open_param.sample_rate = open_param->sample_rate;
    attr->open_param.private_data = open_param->private_data;
    attr->open_param.private_data_size = open_param->private_data_size;
    return AUDIO_SUCCESS;
}

int32_t adec_open_codec(int32_t codec_id, ha_codec_dec_param *open_param)
{
    int32_t ret;
    acodec_get_private_param arg;
    adec_attr attr;
    audio_adec_inst *adec_inst = adec_get_inst();
    (void)memset_s(adec_inst, sizeof(audio_adec_inst), 0, sizeof(audio_adec_inst));
    adec_inst->codec_id = codec_id;
    adec_inst->codec = adec_get_lib_inst();

    if (adec_inst->codec == NULL) {
        return ERR_HACODEC_NOT_SUPPORT;
    }

    ring_buffer_init(HA_CODEC_DECODER);
    cps_profile_init(&adec_inst->profile);

    adec_attr_set(codec_id, &attr, open_param);
    ret = adec_get_default_private_attr(adec_inst->codec, &arg, &attr);
    if (ret != AUDIO_SUCCESS) {
        printf("adec get private para err %x\n", ret);
        return ret;
    }

    ret = adec_inst->codec->init(&adec_inst->h_dec, &attr.open_param);
    if (ret != AUDIO_SUCCESS) {
        printf("adec init err %x\n", ret);
        return ret;
    }

    ret = adec_inst->codec->get_max_pcm_out_size(adec_inst->h_dec, &adec_inst->max_pcm_out_size);
    if (ret != AUDIO_SUCCESS) {
        printf("adec get pcm outsize err %x\n", ret);
        adec_inst->codec->deinit(adec_inst->h_dec);
        return ret;
    }

    if (adec_inst->max_pcm_out_size == 0 || adec_inst->max_pcm_out_size > AUDIO_FIFO_SIZE) {
        adec_inst->codec->deinit(adec_inst->h_dec);
        return ERR_HACODEC_NOT_ENOUGH_OUT_BUFFER_SIZE;
    }

    return AUDIO_SUCCESS;
}

void adec_close_codec()
{
    audio_adec_inst *adec_inst = adec_get_inst();
    if (adec_inst->codec != NULL) {
        if (adec_inst->h_dec != NULL) {
            adec_inst->codec->deinit(adec_inst->h_dec);
            adec_inst->h_dec = NULL;
        }
        adec_inst->max_pcm_out_size = 0;
        adec_inst->codec = NULL;
    }
}

static int32_t adec_decode_frame_proc(audio_adec_inst *adec_inst, uint32_t pkg_loss)
{
    int32_t ret;
    ring_buffer_t *in_fifo = audio_get_in_fifo(HA_CODEC_DECODER);
    uint8_t *input_data = NULL;
    uint8_t output_data[AUDIO_DEC_OUT_FIFO_READ_DATA_LEN];
    uint32_t consume_data_size = audio_get_decoder_consume_data_size();
    ret = read_data_from_fifo(&input_data, in_fifo, consume_data_size);
    if (ret != AUDIO_SUCCESS) {
        adec_inst->acquire_fail_cnt++;
        return ret;
    }

    adec_inst->in_pkt.data = input_data;
    adec_inst->in_pkt.size = AUDIO_FIFO_SIZE;
    adec_inst->in_pkt.pkg_loss = pkg_loss; /* use stream->pkg_loss when ready */
    adec_inst->out_pkt.pcm_out_buf_size = AUDIO_DEC_OUT_FIFO_READ_DATA_LEN;
    adec_inst->out_pkt.pcm_out_buf = (int32_t *)output_data;
    cps_profile_start(&adec_inst->profile);
    ret = adec_inst->codec->decode_frame(adec_inst->h_dec, &adec_inst->in_pkt, &adec_inst->out_pkt);
    if (ret == AUDIO_SUCCESS) {
        cps_profile_stop(&adec_inst->profile, adec_inst->out_pkt.out_sample_rate, adec_inst->out_pkt.pcm_out_samples);
    }

    consume_data_size = AUDIO_FIFO_SIZE - adec_inst->in_pkt.size;
    audio_set_decoder_consume_data_size(consume_data_size);
    read_data_from_fifo_finish(in_fifo, consume_data_size);

    if (ret != AUDIO_SUCCESS) {
        if (ret == (int32_t)ERR_HACODEC_NOT_ENOUGH_DATA) {
            adec_inst->not_enough_data_cnt++;
        } else {
            adec_inst->dec_fail_cnt++;
        }
        return ret;
    }

    adec_inst->frame_size = codec_max(adec_inst->frame_size, adec_inst->out_pkt.pcm_out_buf_size);
    adec_inst->dec_success_cnt++;
    return ret;
}

static int32_t adec_output_frame(audio_adec_inst *adec_inst)
{
    ring_buffer_t *out_fifo = audio_get_out_fifo(HA_CODEC_DECODER);
    audio_dec_set_product_data_size(adec_inst->out_pkt.pcm_out_buf_size);
    adec_inst->frame_cnt++;
    return write_data_into_fifo((uint8_t *)adec_inst->out_pkt.pcm_out_buf, out_fifo,
                                adec_inst->out_pkt.pcm_out_buf_size);
}

static void update_decode_time_interval(audio_adec_inst *adec_inst, uint32_t cur_time)
{
    if (adec_inst->dec_success_cnt > 1) {
        /* 第1次无须计算帧间隔，否则会导致数据异常 */
        uint32_t time_diff = audio_osal_cali_time_diff_us(adec_inst->last_dec_time, cur_time);
        adec_inst->cur_dec_interval = time_diff;
        if (time_diff > adec_inst->max_dec_interval) {
            adec_inst->max_dec_interval = time_diff;
        }
        if (time_diff < adec_inst->min_dec_interval) {
            adec_inst->min_dec_interval = time_diff;
        }
        adec_inst->total_dec_interval += time_diff;
    }

    adec_inst->last_dec_time = cur_time;
}

static void update_decode_time_consume(audio_adec_inst *adec_inst, uint32_t time_begin, uint32_t time_end)
{
    uint32_t time_diff = audio_osal_cali_time_diff_us(time_begin, time_end);
    adec_inst->cur_dec_consume = time_diff;
    if (time_diff > adec_inst->max_dec_consume) {
        adec_inst->max_dec_consume = time_diff;
    }
    if (time_diff < adec_inst->min_dec_consume) {
        adec_inst->min_dec_consume = time_diff;
    }
    adec_inst->total_dec_consume += time_diff;
}

static uint32_t adec_check_frame_info_changed(const audio_adec_inst *adec_inst)
{
    if (adec_inst->format_changed == AUDIO_TRUE) {
        return AUDIO_TRUE;
    }

    return (adec_inst->out_pkt.out_channels != adec_inst->pcm_attr.channels ||
        adec_inst->out_pkt.bit_per_sample != adec_inst->pcm_attr.bit_depth ||
        adec_inst->out_pkt.out_sample_rate != adec_inst->pcm_attr.sample_rate);
}

static void adec_report_frame_info_changed(audio_adec_inst *adec_inst)
{
    if (!adec_check_frame_info_changed(adec_inst)) {
        return;
    }

    adec_inst->info_change_cnt++;

    adec_inst->pcm_attr.channels = adec_inst->out_pkt.out_channels;
    adec_inst->pcm_attr.bit_depth = adec_inst->out_pkt.bit_per_sample;
    adec_inst->pcm_attr.sample_rate = adec_inst->out_pkt.out_sample_rate;
    adec_inst->pcm_attr.samples_per_frame = adec_inst->pcm_attr.sample_rate / AUDIO_10MS_NUM_PER_SECOND;

    adec_inst->frame_pms = calc_frame_size_per_ms(&adec_inst->pcm_attr);
    adec_inst->format_changed = AUDIO_FALSE;
}

int32_t adec_decode_frame(uint32_t pkg_loss)
{
    int32_t ret;
    audio_adec_inst *adec_inst = adec_get_inst();
#ifdef AUDIO_CODEC_TIME_INFO
    uint32_t time_begin;
    uint32_t time_end;
    time_begin = audio_osal_gettime_us();
#endif
    ret = adec_decode_frame_proc(adec_inst, pkg_loss);
    if (ret != AUDIO_SUCCESS) {
        audio_dec_set_product_data_size(0);
        printf("adec decode_frame warning %x\n", ret);
        if (adec_inst->not_enough_data_cnt > 3 || adec_inst->acquire_fail_cnt > 3) { /* 3: Minimum data. */
            return ret;
        }
        return ERR_HACODEC_READ_FIFO_NOT_ENOUGH_DATA;
    }
#ifdef AUDIO_CODEC_TIME_INFO
    time_end = audio_osal_gettime_us();
    update_decode_time_interval(adec_inst, time_begin);
    update_decode_time_consume(adec_inst, time_begin, time_end);

    adec_report_frame_info_changed(adec_inst);
#endif
    unused(adec_report_frame_info_changed);
    unused(update_decode_time_interval);
    unused(update_decode_time_consume);
    return adec_output_frame(adec_inst);
}

void audio_decoder_fifo_info_display()
{
    float in_fifo_use;
    float out_fifo_use;
    ring_buffer_t *dec_in_fifo = audio_get_in_fifo(HA_CODEC_DECODER);
    ring_buffer_t *dec_out_fifo = audio_get_out_fifo(HA_CODEC_DECODER);

    if (dec_in_fifo->read > dec_in_fifo->write) {
        in_fifo_use = (float)(dec_in_fifo->write + dec_in_fifo->total_size - dec_in_fifo->read) /
        dec_in_fifo->total_size;
    } else {
        in_fifo_use = (float)(dec_in_fifo->write - dec_in_fifo->read) / dec_in_fifo->total_size;
    }

    if (dec_out_fifo->read > dec_out_fifo->write) {
        out_fifo_use = (float)(dec_out_fifo->write + dec_out_fifo->total_size - dec_out_fifo->read) /
        dec_out_fifo->total_size;
    } else {
        out_fifo_use = (float)(dec_out_fifo->write - dec_out_fifo->read) / dec_out_fifo->total_size;
    }

    printf("=======================================audio decoder fifo info========================================\n");
    printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "fifo", "read", "write", "usage",
        "tail_flag", "valid_size", "total_size");
    printf("%-8s\t%-8d\t%-8d\t%-8.4f\t%-8d\t%-8d\t%-8d\t\n", "in_fifo", dec_in_fifo->read, dec_in_fifo->write,
        in_fifo_use, dec_in_fifo->tail_flag, dec_in_fifo->valid_size, dec_in_fifo->total_size);
    printf("%-8s\t%-8d\t%-8d\t%-8.4f\t%-8d\t%-8d\t%-8d\t\n", "out_fifo", dec_out_fifo->read, dec_out_fifo->write,
        out_fifo_use, dec_out_fifo->tail_flag, dec_out_fifo->valid_size, dec_out_fifo->total_size);
    printf("====================================audio decoder fifo info end=======================================\n");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
