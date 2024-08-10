/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Application core os initialize interface header for standard \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-27, Create file. \n
*/
#include <stdio.h>
#include "common_def.h"
#include "audio_profile_calc.h"

#define AUDIO_US_NUM_PER_SECOND 1000000
void cps_profile_start(cps_profile *profile)
{
#ifdef CODEC_MCPS_STATISTIC
    profile->start_cycles = rdcycle();
#else
    unused(profile);
#endif /* SAP_MCPS_STATISTIC */
}

void cps_profile_init(cps_profile *profile)
{
    profile->frame = 0;
    profile->peak = 0;
    profile->sum = 0;
}

void cps_profile_stop(cps_profile *profile, uint32_t sample_rate, uint32_t samples)
{
#ifdef CODEC_MCPS_STATISTIC
    if (sample_rate == 0) {
        return;
    }
    uint32_t duration_us = (samples * AUDIO_US_NUM_PER_SECOND) / sample_rate;

    if (duration_us == 0) {
        return;
    }

    profile->stop_cycles = rdcycle();
    profile->frame++;
    profile->cycles = profile->stop_cycles - profile->start_cycles;

#ifdef CYCLE_FLOAT
    profile->curr = ((float)profile->cycles / duration_us);
    profile->sum += profile->curr;
    profile->ave = profile->sum / profile->frame;

    if (profile->peak < profile->curr) {
        profile->peak = profile->curr;
        profile->peak_frame = profile->frame;
    }
    printf("frame now is %d, mcps ave is %f, peak is %f, peak frame is %d\n",
        profile->frame, profile->ave, profile->peak, profile->peak_frame);
#else
    profile->curr = profile->cycles;
    profile->sum += profile->curr;
    profile->ave = profile->sum / profile->frame;
    if (profile->peak < profile->curr) {
        profile->peak = profile->curr;
        profile->peak_frame = profile->frame;
    }
    printf("frame now is %d, cycle is %lu, peak is %lu, peak frame is %d, time_us %d ave %d\n",
        profile->frame, profile->cycles, profile->peak, profile->peak_frame, duration_us, profile->ave);
#endif
#else
    unused(profile);
    unused(sample_rate);
    unused(samples);
#endif
}
