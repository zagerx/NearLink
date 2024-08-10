/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Application core os initialize interface header for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
*/

#include "audio_common.h"

ring_buffer_t g_in_fifo[2];
ring_buffer_t g_out_fifo[2];
uint8_t __attribute__((aligned(8))) g_in_buffer[2][AUDIO_FIFO_SIZE + AUDIO_PADDING_BUFFER_SIZE];
uint8_t __attribute__((aligned(8))) g_out_buffer[2][AUDIO_FIFO_SIZE + AUDIO_PADDING_BUFFER_SIZE];

void shift_buffer_process(uint8_t* dest, uint8_t* src, uint32_t word_count)
{
    uint32_t count = word_count >> 2;
    uint32_t res = word_count & 0x3;
    uint32_t i;
    int32_t *d = (int32_t *)dest;
    int32_t *s = (int32_t *)src;

    for (i = 0; i < count; i++) {
        *(d++) = *(s++);
    }

    dest = (uint8_t *)d;
    src = (uint8_t *)s;
    for (i = 0; i < res; i++) {
        *(dest++) = *(src++);
    }
}

void ring_buffer_para_init(ring_buffer_t *ring_buffer)
{
    ring_buffer->read = 0;
    ring_buffer->write = 0;
    ring_buffer->valid_size = AUDIO_FIFO_SIZE;
    ring_buffer->total_size = AUDIO_FIFO_SIZE;
    ring_buffer->tail_flag = 0;
}

int32_t ring_buffer_init(ha_codec_tpye codec_type)
{
    ring_buffer_t *in_fifo = audio_get_in_fifo(codec_type);
    ring_buffer_t *out_fifo = audio_get_out_fifo(codec_type);
    in_fifo->buffer = &g_in_buffer[codec_type][0];
    out_fifo->buffer = &g_out_buffer[codec_type][0];

    ring_buffer_para_init(in_fifo);
    ring_buffer_para_init(out_fifo);

    return AUDIO_SUCCESS;
}

int32_t write_data_into_fifo(uint8_t *data, ring_buffer_t *fifo, uint32_t len)
{
    int32_t data_left;
    if (fifo->valid_size > len) {
        shift_buffer_process(fifo->buffer + fifo->write, data, len);
        fifo->valid_size -= len;
        fifo->write += len;
    } else {
        shift_buffer_process(fifo->buffer + fifo->write, data, fifo->valid_size);
        data_left = len - fifo->valid_size;
        shift_buffer_process(fifo->buffer, data + fifo->valid_size, data_left);
        fifo->write = data_left;
        fifo->valid_size = AUDIO_FIFO_SIZE - data_left;
        fifo->tail_flag = 1;
    }
#ifdef ERROR_DETECT
    printf("fifo write data %d %d %d %d\n", len, fifo->write, fifo->read, fifo->tail_flag);
#endif
    if ((fifo->tail_flag == 1) && (fifo->write > fifo->read)) {
        return ERR_HACODEC_WRITE_FIFO_OVERFLOW;
    }
    return AUDIO_SUCCESS;
}

int32_t read_data_from_fifo(uint8_t **data, ring_buffer_t *fifo, int32_t len)
{
    int32_t data_len;
    int32_t data_need;

    if (fifo->tail_flag == 0) {
        data_len = fifo->write - fifo->read;
        if (len > data_len) {
            printf("read_data_from_fifo fail %d %d\n", len, data_len);
            return ERR_HACODEC_READ_FIFO_NOT_ENOUGH_DATA;
        }
    } else {
        data_len = AUDIO_FIFO_SIZE - fifo->read;
        if (len > data_len) {
            data_need = len - data_len;
            shift_buffer_process(fifo->buffer + AUDIO_FIFO_SIZE, fifo->buffer, data_need);
        }
    }
    *data = fifo->buffer + fifo->read;
#ifdef ERROR_DETECT
    printf("fifo read ptr %d tail flag %d len %d\n", fifo->read, fifo->tail_flag, len);
#endif
    return AUDIO_SUCCESS;
}

void read_data_from_fifo_finish(ring_buffer_t *fifo, uint32_t len)
{
    int32_t read_len = len + fifo->read;

    if (read_len >= AUDIO_FIFO_SIZE) {
        fifo->tail_flag = 0;
        fifo->read = read_len - AUDIO_FIFO_SIZE;
        fifo->valid_size = AUDIO_FIFO_SIZE - fifo->write;
    } else {
        fifo->read = read_len;
    }
}

uint32_t calc_sample_size(uint32_t channels, uint32_t bit_depth)
{
    switch (bit_depth) {
        case BIT_DEPTH_8:
            return sizeof(int8_t) * channels;

        case BIT_DEPTH_16:
            return sizeof(int16_t) * channels;

        case BIT_DEPTH_24:
        case BIT_DEPTH_32:
            return sizeof(int32_t) * channels;

        case BIT_DEPTH_64:
            return sizeof(int64_t) * channels;

        default:
            return 0;
    }
}

uint32_t calc_frame_size_per_ms(const audio_pcm_format *pcm_format)
{
    uint32_t sample_size;

    if (pcm_format->sample_rate < HA_CODEC_SAMPLE_RATE_8K || pcm_format->sample_rate > HA_CODEC_SAMPLE_RATE_48K) {
        return 0;
    }

    if (pcm_format->channels < AUDIO_CH_MONO || pcm_format->channels > AUDIO_CH_16) {
        return 0;
    }

    sample_size = calc_sample_size(pcm_format->channels, pcm_format->bit_depth);
    return (sample_size * pcm_format->sample_rate) / AUDIO_MS_NUM_PER_SECOND;
}
