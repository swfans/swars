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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#include "ailss.h"
#include "aildebug.h"
#include "ail.h"
#include "mssdig.h"
#include "drv_oal.h"
/******************************************************************************/
/** Recognized file types
 */
enum SampleFileTypes {
    SMP_FTYP_VOC = 0,
    SMP_FTYP_WAV = 1,
    SMP_FTYP_ASI = 2,
};

void SS_build_amplitude_tables(SNDSAMPLE *s)
{
    asm volatile (
      "push %0\n"
      "call ASM_SS_build_amplitude_tables\n"
      "add $0x4, %%esp\n"
        :  : "g" (s) : "eax" );
}

void SS_flush(DIG_DRIVER *digdrv)
{
    // Initialize the build buffer by flushing with 0
    memset(digdrv->build_buffer, 0, digdrv->build_size);
}

void SS_copy(DIG_DRIVER *digdrv, void *pWaveAddr)
{
#if defined(DOS)||defined(GO32)
    AILSSA_DMA_copy((pWaveAddr == dig->DMA[0] ) ? 0 : 1,
          &dig->DMA[0], dig->hw_mode_flags, dig->hw_format,
          dig->n_active_samples, dig->build_buffer,
          dig->samples_per_buffer, dig->half_buffer_size);
#else
    // TODO not sure what is needed here
#endif
}

int32_t SS_configure_buffers(DIG_DRIVER *digdrv)
{
    int32_t i, bsmin, bsmax, hb_samples;
    int32_t n, match, delta;
    int32_t pref[4];
    int32_t DMA_status;

    DMA_status = digdrv->playing;

    if (DMA_status)
    {
        // Pause all playing samples
        for (i = 0; i < digdrv->n_samples; i++)
        {
            // use one of system_data[] slots for temporary storage,
            // the storage place becomes free again when this function ends
            digdrv->samples[i].system_data[7] =
                digdrv->samples[i].status;

            if (digdrv->samples[i].status == SNDSMP_PLAYING)
                digdrv->samples[i].status = SNDSMP_STOPPED;
        }

        // Halt DMA playback and flush DMA buffers with silence
        SS_stop_DIG_driver_playback(digdrv);
        SS_flush(digdrv);
        SS_copy(digdrv, digdrv->DMA[0]);
        SS_copy(digdrv, digdrv->DMA[1]);
    }

    // Select hardware output format based on application's preference
    // If desired mode is unavailable, downgrade/upgrade to other modes
    switch ((AIL_preference[DIG_USE_STEREO] << 1)
        | (AIL_preference[DIG_USE_16_BITS]))
    {
    case DIG_F_MONO_8:
        pref[0] = DIG_F_MONO_8;
        pref[1] = DIG_F_MONO_16;
        pref[2] = DIG_F_STEREO_8;
        pref[3] = DIG_F_STEREO_16;
        break;

    case DIG_F_MONO_16:
        pref[0] = DIG_F_MONO_16;
        pref[1] = DIG_F_MONO_8;
        pref[2] = DIG_F_STEREO_16;
        pref[3] = DIG_F_STEREO_8;
        break;

    case DIG_F_STEREO_8:
        pref[0] = DIG_F_STEREO_8;
        pref[1] = DIG_F_STEREO_16;
        pref[2] = DIG_F_MONO_8;
        pref[3] = DIG_F_MONO_16;
        break;

    case DIG_F_STEREO_16:
        pref[0] = DIG_F_STEREO_16;
        pref[1] = DIG_F_STEREO_8;
        pref[2] = DIG_F_MONO_16;
        pref[3] = DIG_F_MONO_8;
        break;
    }
    // Check what the driver marked as supported
    for (i = 0; i < 4; i++)
    {
        if (digdrv->DDT->format_supported[pref[i]])
        {
            digdrv->hw_format = pref[i];
            break;
        }
    }

    digdrv->hw_mode_flags = digdrv->DDT->format_data[digdrv->hw_format].flags;

    // Select physical output rate based on application's preference
    switch (AIL_preference[DIG_HARDWARE_SAMPLE_RATE])
    {
    case AILPREF_MIN_VAL:
        digdrv->DMA_rate =
            digdrv->DDT->format_data[digdrv->hw_format].minimum_physical_sample_rate;
        break;

    case AILPREF_NOM_VAL:
        digdrv->DMA_rate =
            digdrv->DDT->format_data[digdrv->hw_format].nominal_physical_sample_rate;
        break;

    case AILPREF_MAX_VAL:
        digdrv->DMA_rate =
            digdrv->DDT->format_data[digdrv->hw_format].maximum_physical_sample_rate;
        break;

    default:
        // If preference set to explicit frequency, use best match with
        // current driver
        n = AIL_preference[DIG_HARDWARE_SAMPLE_RATE];
        pref[0] =
            digdrv->DDT->format_data[digdrv->hw_format].minimum_physical_sample_rate;
        pref[1] =
            digdrv->DDT->format_data[digdrv->hw_format].nominal_physical_sample_rate;
        pref[2] =
            digdrv->DDT->format_data[digdrv->hw_format].maximum_physical_sample_rate;
        delta = LONG_MAX;
        for (i = 0; i < 3; i++)
        {
            if (abs(n - pref[i]) <= delta) {
                delta = abs(n - pref[i]);
                match = i;
            }
        }
        digdrv->DMA_rate = pref[match];
        break;
    }

    // Set sample size values
    switch (digdrv->hw_format)
    {
    case DIG_F_MONO_8:
        digdrv->channels_per_sample = 1;
        digdrv->bytes_per_channel = 1;
        break;

    case DIG_F_STEREO_8:
        digdrv->channels_per_sample = 2;
        digdrv->bytes_per_channel = 1;
        break;

    case DIG_F_MONO_16:
        digdrv->channels_per_sample = 1;
        digdrv->bytes_per_channel = 2;
        break;

    case DIG_F_STEREO_16:
        digdrv->channels_per_sample = 2;
        digdrv->bytes_per_channel = 2;
        break;
    }

    // Get # of samples per half-buffer, assuming desired latency
    hb_samples = (digdrv->DMA_rate * AIL_preference[DIG_LATENCY]) / 1000;

    // Calculate half-buffer size in bytes
    digdrv->half_buffer_size = hb_samples * digdrv->bytes_per_channel
        * digdrv->channels_per_sample;

    // Make sure half-buffer size is legal with this driver
    bsmin = digdrv->DDT->format_data[digdrv->hw_format].minimum_DMA_half_buffer_size;
    bsmax = digdrv->DDT->format_data[digdrv->hw_format].maximum_DMA_half_buffer_size;

    // Round half-buffer size to nearest binary power between 8 and
    // DIG_DMA_RESERVE / 2; ensure result within driver limits
    delta = LONG_MAX;

    for (n = 8; n <= (AIL_preference[DIG_DMA_RESERVE] / 2); n <<= 1)
    {
        if (abs(n - digdrv->half_buffer_size) <= delta)
        {
            delta = abs(n - digdrv->half_buffer_size);
            match = n;
        }
    }

    digdrv->half_buffer_size = match;
    if (digdrv->half_buffer_size < bsmin)
        digdrv->half_buffer_size = bsmin;
    if (digdrv->half_buffer_size > bsmax)
        digdrv->half_buffer_size = bsmax;

    // Set DMA half-buffer segments in driver DST, and store equivalent
    // protected-mode pointers in driver descriptor structure
    digdrv->DST->DMA_buffer_A = digdrv->DMA_buf;
    digdrv->DST->DMA_buffer_B = (digdrv->DMA_buf + digdrv->half_buffer_size);

    digdrv->DMA[0] = digdrv->DST->DMA_buffer_A;
    digdrv->DMA[1] = digdrv->DST->DMA_buffer_B;

    if (DMA_status)
    {
        // Resume any interrupted samples
        for (i = 0; i < digdrv->n_samples; i++)
            digdrv->samples[i].status =
                digdrv->samples[i].system_data[7];

        // Flush new DMA buffers with silence and resume DMA playback
        SS_flush(digdrv);
        SS_copy(digdrv, digdrv->DMA[0]);
        SS_copy(digdrv, digdrv->DMA[1]);
        SS_start_DIG_driver_playback(digdrv);
    }

    return 1;
}

void SS_stream_to_buffer(SNDSAMPLE *s)
{
  //TODO though this is not used with OpenAL
}

void AIL2OAL_API_init_sample(SNDSAMPLE *s)
{
#if 0
    asm volatile (
      "push %0\n"
      "call ASM_AIL_API_init_sample\n"
      "add $0x4, %%esp\n"
        :  : "g" (s) : "eax" );
#endif
    if (s == NULL)
        return;

    // Set status to FREE while manipulating vars, to keep callback thread
    // from reading invalid data
    s->status = SNDSMP_FREE;

    // Shut down any previously-active pipeline providers
    //   and init pipeline stages
    AIL_set_sample_processor(s, SNDSMST_SAMPLE_ALL_STAGES, 0);

    // Initialize sample vars
    s->start[0] = NULL;
    s->len[0] = 0;
    s->pos[0] = 0;
    s->done[0] = 0;

    s->start[1] = NULL;
    s->len[1] = 0;
    s->pos[1] = 0;
    s->done[1] = 1;

    s->current_buffer = 0;
    s->last_buffer = -2;

    s->loop_count = 1;
    s->format = 0;
    s->flags = 0;
    s->volume = AIL_preference[5];
    s->playback_rate = 11025;
    if ((s->driver->hw_format != 0) && (s->driver->hw_format != 1))
        s->pan = 64;
    else
        s->pan = 0;
    s->vol_scale[1][255] = 0;

    s->SOB = NULL;
    s->EOB = NULL;
    s->EOS = NULL;

    // Mark sample initialized
    s->status = SNDSMP_DONE;

    SS_build_amplitude_tables(s);
}

SNDSAMPLE *AIL2OAL_API_allocate_sample_handle(DIG_DRIVER *digdrv)
{
    int32_t i;
    SNDSAMPLE *s;

    if (digdrv == NULL)
        return NULL;

    AIL_lock();

    // Look for an unallocated sample structure
    for (i = 0; i < digdrv->n_samples; i++)
    {
        if (digdrv->samples[i].status == SNDSMP_FREE)
            break;
    }

    if (i == digdrv->n_samples)
    {
        AIL_set_error("Out of sample handles.");
        AIL_unlock();
        return NULL;
    }

    s = &digdrv->samples[i];

    // Initialize sample to status SNDSMP_DONE with nominal
    // sample attributes
    AIL_init_sample(s);

    AIL_unlock();
    return s;
}

/** Instead of setting hardware volume, scales volume for all samples.
 */
void set_master_hardware_volume(DIG_DRIVER *digdrv)
{
    SNDSAMPLE *s;
    int i;

    for (i = 0; i < digdrv->n_samples; i++)
    {
        s = &digdrv->samples[i];
        if (s->status != SNDSMP_FREE)
            SS_build_amplitude_tables(s);
    }
}

void AIL2OAL_API_set_digital_master_volume(DIG_DRIVER *digdrv, int32_t master_volume)
{
    if (digdrv == NULL)
        return;

    digdrv->master_volume = master_volume;

    set_master_hardware_volume(digdrv);
}

void AIL2OAL_API_start_sample(SNDSAMPLE *s)
{
    if (s == NULL)
        return;

    if (s->status == SNDSMP_FREE)
        return;

    // Make sure valid sample data exists
    if ((s->len[s->current_buffer] == 0) ||
     (s->start[s->current_buffer] == NULL))
        return;

    // Rewind sample to beginning
    s->pos[s->current_buffer] = 0;

    s->status = SNDSMP_PLAYING;

    // If sample's driver is not already transmitting data, start it
    SS_start_DIG_driver_playback(s->driver);
}

void AIL2OAL_API_end_sample(SNDSAMPLE *s)
{
    if (s == NULL)
        return;

    // Make sure sample has been allocated
    if (s->status == SNDSMP_FREE)
        return;

    // If sample is not already done, halt it and invoke its end-of-buffer
    // and end-of-sample callback functions
    if (s->status != SNDSMP_DONE)
    {
        s->status = SNDSMP_DONE;

        OPENAL_stop_sample(s);

        if (s->EOB != NULL)
            s->EOB(s);
        if (s->EOS != NULL)
            s->EOS(s);
    }
}

uint32_t AIL2OAL_API_sample_status(SNDSAMPLE *s)
{
    assert(s != NULL);
    return s->status;
}

/** Get double-buffer playback status for sample.
 *
 * @return Gives one of:
 *   0: Buffer 0 is ready to be filled (buffer 1 or neither buffer playing),
 *   1: Buffer 1 is ready to be filled (buffer 0 playing),
 *  -1: Both buffers are already full.
 */
int32_t AIL2OAL_API_sample_buffer_ready(SNDSAMPLE *s)
{
    if (s == NULL)
        return -1;

    // For first two calls after sample initialization, return 0 and 1,
    // respectively
    //
    // This allows the application to "prime" the buffers for continued
    // playback
    switch (s->last_buffer)
    {
    case -2:
        // First call after AIL_init_sample() must clear second buffer's
        // "done" flag to permit buffer-switching
        s->done[1] = 0;

        // Set up to load buffer 0 this call, and "bootstrap" buffer 1 at
        // next call
        s->last_buffer = -1;
        return 0;

    case -1:
        // Return 1 to force load of second buffer immediately
        // Subsequent calls should operate on alternating buffers
        s->last_buffer = s->current_buffer;
        return 1;
    }

    // If buffer has not switched since last call
    if (s->last_buffer == s->current_buffer)
        return -1;

    // New current_buffer exists -- set last_buffer equal to
    // current_buffer and return exhausted buffer
    s->last_buffer = s->current_buffer;

    return (s->current_buffer ^ 1);
}

/** Process .VOC file block.
 *
 * Called by .VOC initialization code and as end-of-sample callback
 * function (interrupt-based).
 *
 * If play_flag clear, search for first block after desired marker (if
 * any) and return without playing it.
 */
void AIL_process_VOC_block(SNDSAMPLE *s, int32_t play_flag)
{
    asm volatile (
      "push %1\n"
      "push %0\n"
      "call ASM_AIL_process_VOC_block\n"
      "add $0x8, %%esp\n"
        :  : "g" (s), "g" (play_flag) : "eax" );
}

/** Create sample instance by parsing .WAV file.
 */
void AIL_process_WAV_image(const AILSOUNDINFO *info, SNDSAMPLE *s)
{
    asm volatile (
      "push %1\n"
      "push %0\n"
      "call ASM_AIL_process_WAV_image\n"
      "add $0x8, %%esp\n"
        :  : "g" (info), "g" (s) : "eax" );
}

int32_t AIL2OAL_API_set_sample_file(SNDSAMPLE *s, const void *file_image, int32_t block)
{
    int32_t ftype;

    if ((s == NULL) || (file_image == NULL))
        return 0;

    AIL_init_sample(s);

    ftype = -1;
    if (ftype == -1)
    {
        if (strncasecmp(file_image, "Creative", 8) == 0)
            ftype = SMP_FTYP_VOC;
    }
    if (ftype == -1)
    {
        if (strncasecmp(file_image + 8, "WAVE", 4) == 0)
            ftype = SMP_FTYP_WAV;
    }
    if (ftype == -1)
    {
        AIL_set_error("Unknown digital audio file type.");
        return 0;
    }

    // Copy file attributes to sample
    switch (ftype)
    {
    case SMP_FTYP_WAV:
        s->system_data[6] = 0;
        AIL_process_WAV_image(file_image, s);
        break;
    case SMP_FTYP_VOC:
        // Store pointer to sample data
        s->system_data[1] = (uintptr_t)file_image + *(uint16_t *)(file_image + 20);
        //s->system_data[4] = block; // used by OpenAL
        s->system_data[6] = 0;
        //s->system_data[5] = (block == -1); // used by OpenAL
        AIL_process_VOC_block(s, 0);
        break;
    default: // s->system_data[6] == -1
        AIL_set_error("Invalid or missing data block.");
        return 0;
    }
    return 1;
}

void AIL2OAL_API_release_sample_handle(SNDSAMPLE *s)
{
    if (s == NULL)
        return;

    s->status = SNDSMP_FREE;

    // Shut down any installed pipeline providers
    AIL_set_sample_processor(s, SNDSMST_SAMPLE_ALL_STAGES, 0);
}

AILSAMPLECB AIL2OAL_API_register_EOS_callback(SNDSAMPLE *s, AILSAMPLECB EOS)
{
    AILSAMPLECB old;

    if (s == NULL)
        return NULL;

    old = s->EOS;
    s->EOS = EOS;

    return old;
}

HAILPROVIDER AIL2OAL_API_set_sample_processor(SNDSAMPLE *s,
    uint32_t pipeline_stage, HAILPROVIDER provider)
{
    // No need to implement - filters are rarely used
    return 0;
}

void AIL2OAL_API_set_sample_address(SNDSAMPLE *s, const void *start, uint32_t len)
{
    if (s == NULL)
        return;

    s->start[0] = (void *)start;
    s->len[0]   = len;

    s->start[1] = NULL;
    s->len[1]   = 0;
}

void AIL2OAL_API_set_sample_type(SNDSAMPLE *s, int32_t format, uint32_t flags)
{
    if (s == NULL)
        return;

    if ((format == s->format) && (flags == s->flags))
        return;

    s->format = format;
    s->flags  = flags;

    SS_build_amplitude_tables(s);
}

void AIL2OAL_API_set_sample_user_data(SNDSAMPLE *s, uint32_t index, intptr_t value)
{
    if (s == NULL)
        return;
    s->user_data[index] = value;
}

void AIL2OAL_API_set_sample_loop_count(SNDSAMPLE *s, int32_t loop_count)
{
    if (s == NULL)
        return;

    s->loop_count = loop_count;
}
/******************************************************************************/
