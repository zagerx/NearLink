/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Application core os initialize interface header for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
 */
#include <sys/time.h>
#include "audio_enc_codec.h"
/* wzm add */
#include "test_suite_log.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MAX_TRANS_UNIT_SIZE 4096
#define AUDIO_CODEC_PARA_SIZE 32
extern void *overlay_enc_entry(void);
static audio_aenc_inst g_aenc_inst;
static uint8_t g_enc_private_data[AUDIO_CODEC_PARA_SIZE];
static uint32_t g_consume_data_size = 960;
static uint32_t g_product_data_size = 0;
static inline audio_aenc_inst *aenc_get_inst(void)
{
    return &g_aenc_inst;
}

uint32_t audio_get_encoder_consume_data_size(void)
{
    return g_consume_data_size;
}

uint32_t audio_get_encoder_product_data_size(void)
{
    return g_product_data_size;
}

void audio_set_encoder_consume_data_size(uint32_t data_size)
{
#ifdef ERROR_DETECT
    printf("enc consume data is %d\n", data_size);
#endif
    g_consume_data_size = data_size;
}

void audio_set_product_data_size(uint32_t data_size)
{
#ifdef ERROR_DETECT
    printf("enc product data is %d\n", data_size);
#endif
    g_product_data_size = data_size;
}

static ha_codec_encode *aenc_get_lib_inst(void)
{
    ha_codec_encode *dev = overlay_enc_entry();
    return dev;
}

void aenc_attr_set(int32_t codecid, aenc_attr *attr, ha_codec_enc_param *open_param)
{
    attr->codec_id = codecid;
    attr->open_param.channels = open_param->channels;
    attr->open_param.bit_depth = open_param->bit_depth;
    attr->open_param.sample_rate = open_param->sample_rate;
    attr->open_param.private_data = open_param->private_data;
    attr->open_param.private_data_size = open_param->private_data_size;
}

static void aenc_get_default_private_attr(ha_codec_encode *codec, acodec_get_private_param *arg, aenc_attr *attr)
{
    int32_t ret;

    if (codec->send_cmd == NULL) {
        return;
    }

    arg->codec_id = attr->codec_id;
    arg->private_data = attr->open_param.private_data;
    arg->private_data_size = attr->open_param.private_data_size;

    arg->private_attr = (void *)&g_enc_private_data[0];
    arg->private_attr_size = AUDIO_CODEC_PARA_SIZE;

    ret = codec->send_cmd(NULL, ACODEC_PRIVATE_PARAM_GET_CMD, (void *)arg);
    if (ret != AUDIO_SUCCESS) {
        printf("aenc no need to get private para, return %x\n", ret);
        return;
    }

    attr->open_param.private_data = arg;
    attr->open_param.private_data_size = sizeof(*arg);
}

static void aenc_inst_save_attr(audio_aenc_inst *aenc_inst, const aenc_attr *attr)
{
    aenc_inst->codec_id = (ha_codec_id)attr->codec_id;
    aenc_inst->pcm_attr.channels = attr->open_param.channels;
    aenc_inst->pcm_attr.bit_depth = attr->open_param.bit_depth;
    aenc_inst->pcm_attr.sample_rate = attr->open_param.sample_rate;
    aenc_inst->pcm_attr.samples_per_frame = attr->open_param.samples_per_frame;
    aenc_inst->frame_pms = calc_frame_size_per_ms(&aenc_inst->pcm_attr);
    aenc_inst->frame_size = 0;
}

int32_t aenc_open_codec(int32_t codecid, ha_codec_enc_param *open_param)
{
    int32_t ret;
    acodec_get_private_param arg;
    aenc_attr attr;
    audio_aenc_inst *aenc_inst = aenc_get_inst();
    (void)memset_s(aenc_inst, sizeof(audio_aenc_inst), 0, sizeof(audio_aenc_inst));
    aenc_inst->codec = aenc_get_lib_inst();
    if (aenc_inst->codec == NULL) {
        return ERR_HACODEC_NOT_SUPPORT;
    }
    aenc_attr_set(codecid, &attr, open_param);
    aenc_get_default_private_attr(aenc_inst->codec, &arg, &attr);
    aenc_inst_save_attr(aenc_inst, &attr);
    ret = aenc_inst->codec->init(&aenc_inst->h_enc, &attr.open_param);
    if (ret != AUDIO_SUCCESS) {
        printf("aenc init err %x\n", ret);
        return ret;
    }

    ret = aenc_inst->codec->get_max_bits_out_size(aenc_inst->h_enc, &aenc_inst->max_bit_out_size);
    if (ret != AUDIO_SUCCESS) {
        aenc_inst->codec->deinit(aenc_inst->h_enc);
        return ret;
    }

    if (aenc_inst->max_bit_out_size == 0 || aenc_inst->max_bit_out_size > AUDIO_FIFO_SIZE) {
        ret = ERR_HACODEC_NOT_ENOUGH_OUT_BUFFER_SIZE;
        aenc_inst->codec->deinit(aenc_inst->h_enc);
        return ret;
    }

    return ring_buffer_init(HA_CODEC_ENDODER);
}

void aenc_close_codec()
{
    audio_aenc_inst *aenc_inst = aenc_get_inst();
    if (aenc_inst->codec != NULL) {
        if (aenc_inst->h_enc != NULL) {
            aenc_inst->codec->deinit(aenc_inst->h_enc);
            aenc_inst->h_enc = NULL;
        }
        aenc_inst->max_bit_out_size = 0;
        aenc_inst->codec = NULL;
    }
}

static int32_t aenc_output_stream(audio_aenc_inst *aenc_inst)
{
    ring_buffer_t *out_fifo = audio_get_out_fifo(HA_CODEC_ENDODER);
    audio_set_product_data_size(aenc_inst->out_pkt.bits_out_bytes);
    return write_data_into_fifo((uint8_t *)aenc_inst->out_pkt.bits_out_buf, out_fifo,
        aenc_inst->out_pkt.bits_out_bytes);
}

static uint32_t calc_samples_per_frame(uint32_t frame_size, uint32_t channels, uint32_t bit_depth)
{
    uint32_t sample_size = calc_sample_size(channels, bit_depth);
    if (sample_size == 0) {
        return 0;
    }

    return (frame_size / sample_size);
}

static int32_t aenc_encode_frame_proc(void)
{
    int32_t ret;
    uint32_t sample_num;
    uint32_t consume_data_size = audio_get_encoder_consume_data_size();
    audio_aenc_inst *aenc_inst = aenc_get_inst();
    ring_buffer_t *in_fifo = audio_get_in_fifo(HA_CODEC_ENDODER);
    uint8_t *input_data = NULL;
    uint8_t out_buf[AUDIO_OUT_FIFO_READ_DATA_LEN];
    ret = read_data_from_fifo(&input_data, in_fifo, consume_data_size);
    if (ret != AUDIO_SUCCESS) {
        printf("aenc_encode_frame_proc read %d\n", ret);
        return ret;
    }

    aenc_inst->in_pkt.data = input_data;
    aenc_inst->in_pkt.size = AUDIO_FIFO_SIZE;
    aenc_inst->out_pkt.bits_out_buf = (int32_t *)&out_buf[0];
    aenc_inst->out_pkt.bits_out_buf_size = AUDIO_OUT_FIFO_READ_DATA_LEN;
    aenc_inst->out_pkt.bits_out_bytes = 0;
    cps_profile_start(&aenc_inst->profile);
    ret = aenc_inst->codec->encode_frame(aenc_inst->h_enc, &aenc_inst->in_pkt, &aenc_inst->out_pkt);
    consume_data_size = AUDIO_FIFO_SIZE - aenc_inst->in_pkt.size;
    if (ret == AUDIO_SUCCESS) {
        sample_num = calc_samples_per_frame(consume_data_size,
                                            aenc_inst->pcm_attr.channels, aenc_inst->pcm_attr.bit_depth);
        cps_profile_stop(&aenc_inst->profile, aenc_inst->pcm_attr.sample_rate, sample_num);
    }

    audio_set_encoder_consume_data_size(consume_data_size);
    read_data_from_fifo_finish(in_fifo, consume_data_size);

    if (ret != AUDIO_SUCCESS) {
        if (ret == (int32_t)ERR_HACODEC_NOT_ENOUGH_DATA) {
            aenc_inst->not_enough_data_cnt++;
        }
        printf("aenc encoder warning %x\n", ret);
        return ret;
    }

    return aenc_output_stream(aenc_inst);
}

int32_t aenc_encode_frame(void)
{
    int32_t ret;

    ret = aenc_encode_frame_proc();
    if (ret != AUDIO_SUCCESS) {
        audio_set_product_data_size(0);
    }
    return ret;
}

void audio_encoder_fifo_info_display()
{
    float in_fifo_use;
    float out_fifo_use;
    ring_buffer_t *enc_in_fifo = audio_get_in_fifo(HA_CODEC_ENDODER);
    ring_buffer_t *enc_out_fifo = audio_get_out_fifo(HA_CODEC_ENDODER);

    if (enc_in_fifo->read > enc_in_fifo->write) {
        in_fifo_use = (float)(enc_in_fifo->write + enc_in_fifo->total_size - enc_in_fifo->read) /
        enc_in_fifo->total_size;
    } else {
        in_fifo_use = (float)(enc_in_fifo->write - enc_in_fifo->read) / enc_in_fifo->total_size;
    }

    if (enc_out_fifo->read > enc_out_fifo->write) {
        out_fifo_use = (float)(enc_out_fifo->write + enc_out_fifo->total_size - enc_out_fifo->read) /
        enc_out_fifo->total_size;
    } else {
        out_fifo_use = (float)(enc_out_fifo->write - enc_out_fifo->read) / enc_out_fifo->total_size;
    }

    printf("=======================================audio encoder fifo info========================================\n");
    printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "fifo", "read", "write", "usage",
        "tail_flag", "valid_size", "total_size");
    printf("%-8s\t%-8d\t%-8d\t%-8.4f\t%-8d\t%-8d\t%-8d\t\n", "in_fifo", enc_in_fifo->read, enc_in_fifo->write,
        in_fifo_use, enc_in_fifo->tail_flag, enc_in_fifo->valid_size, enc_in_fifo->total_size);
    printf("%-8s\t%-8d\t%-8d\t%-8.4f\t%-8d\t%-8d\t%-8d\t\n", "out_fifo", enc_out_fifo->read, enc_out_fifo->write,
        out_fifo_use, enc_out_fifo->tail_flag, enc_out_fifo->valid_size, enc_out_fifo->total_size);
    printf("====================================audio encoder fifo info end=======================================\n");
}
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
