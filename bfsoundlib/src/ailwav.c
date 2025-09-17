/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ailss.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     SS functions from MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>

#include "ailss.h"
#include "aildebug.h"
/******************************************************************************/

/** RIFF header, used, among others, in .WAV files */
typedef struct {
    char RIFF_string[4];
    uint32_t chunk_size;
    char ID_string[4];
    uint8_t data[1];
} RIFF;

/** .WAV PCM file format chunk */
typedef struct {
    char FMT_string[4];
    uint32_t  chunk_size;
    int16_t format_tag;
    int16_t channels;
    int32_t sample_rate;
    int32_t average_data_rate;
    int16_t alignment;
    int16_t bits_per_sample;
    int16_t extra;
    int16_t samples_per_block;
} WAVE_FMT;

enum WAVE_FORMAT_TAGS {
    WAVE_FORMAT_PCM = 0x0001,
    WAVE_FORMAT_IEEE_FLOAT = 0x0003,
    WAVE_FORMAT_ALAW = 0x0006,
    WAVE_FORMAT_MULAW = 0x0007,
    WAVE_FORMAT_EXTENSIBLE = 0xFFFE,
};

/** .WAV file data chunk */
typedef struct {
    char DATA_string[4];
    uint32_t chunk_size;
    uint8_t data[1];
} WAVE_DATA;

void AIL_process_WAV_image(const uint8_t *image, SNDSAMPLE *s)
{
#if 0
    asm volatile (
      "push %1\n"
      "push %0\n"
      "call ASM_AIL_process_WAV_image\n"
      "add $0x8, %%esp\n"
        :  : "g" (info), "g" (s) : "eax" );
#endif
    WAVE_FMT *f;
    WAVE_DATA *d;

    // Find mandatory <fmt-ck>
    f = (WAVE_FMT*)(((RIFF*)image)->data);

    while (strncasecmp(f->FMT_string, "fmt ", 4) != 0)
    {
        f = (WAVE_FMT*)((int8_t*)f + f->chunk_size + 8 + (f->chunk_size & 1));
    }

    assert(f->format_tag == WAVE_FORMAT_PCM);

    // Configure sample type and rate based on FMT chunk
    // 8-bit PCM is always unsigned, 16-bit is always signed
    if ((f->channels == 1) && (f->bits_per_sample == 8))
    {
        AIL_set_sample_type(s, DIG_F_MONO_8, 0);
    }
    else if ((f->channels == 2) && (f->bits_per_sample == 8))
    {
        AIL_set_sample_type(s, DIG_F_STEREO_8, 0);
    }
    else if ((f->channels == 1) && (f->bits_per_sample == 16))
    {
        AIL_set_sample_type(s, DIG_F_MONO_16, DIG_PCM_SIGN);
    }
    else if ((f->channels == 2) && (f->bits_per_sample == 16))
    {
        AIL_set_sample_type(s, DIG_F_STEREO_16, DIG_PCM_SIGN);
    }
    AIL_set_sample_playback_rate(s, f->sample_rate);

    // Find mandatory <data-ck>
    d = (WAVE_DATA*)(((RIFF*)image)->data);

    while (strncasecmp(d->DATA_string, "data", 4))
    {
        d = (WAVE_DATA*)((int8_t*)d + d->chunk_size + 8 + (d->chunk_size & 1));
    }

    AIL_set_sample_address(s, d->data, d->chunk_size);
}

/******************************************************************************/
