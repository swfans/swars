/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssampply.c
 *     Sound Sample playback support routines.
 * @par Purpose:
 *     Functions for handling sound samples playback.
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
#include <assert.h>

#include "ssampply.h"
#include "bfsound.h"
#include "aildebug.h"
#include "ssampfad.h"
#include "ssamplst.h"
/******************************************************************************/

extern TbBool SoundInstalled;
extern TbBool SoundAble;
extern TbBool SoundActive;
extern TbBool StereoSound;
extern ulong SampleRate;

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

/******************************************************************************/

static struct SampleInfo *FindSampleInfoSrcSmp(long source_id, short smp_id)
{
    struct SampleInfo *p_smpinf;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if (((ulong)source_id == p_smpinf->SourceID)
          && (smp_id == p_smpinf->SampleNumber)) {
            return p_smpinf;
        }
    }
    return NULL;
}

static struct SampleInfo *FindSampleInfoSrcPlaying(long source_id)
{
    struct SampleInfo *p_smpinf;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if (((ulong)source_id == p_smpinf->SourceID)
          && AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING) {
            return p_smpinf;
        }
    }
    return NULL;
}

static struct SampleInfo *FindSampleInfoSrcSmpPlaying(long source_id, short smp_id)
{
    struct SampleInfo *p_smpinf;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if (((ulong)source_id == p_smpinf->SourceID)
          && (smp_id == p_smpinf->SampleNumber)
          && AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING) {
            return p_smpinf;
        }
    }
    return NULL;
}

static struct SampleInfo *FindSampleInfoAnyDone(void)
{
    struct SampleInfo *p_smpinf;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if (AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_DONE) {
            return p_smpinf;
        }
    }
    return NULL;
}

struct SampleInfo *FindSampleInfoSrcSmpNotDone(long source_id, short smp_id)
{
    struct SampleInfo *p_smpinf;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if (((ulong)source_id == p_smpinf->SourceID)
          && (smp_id == p_smpinf->SampleNumber)
          && AIL_sample_status(p_smpinf->SampleHandle) != SNDSMP_DONE) {
            return p_smpinf;
        }
    }
    return NULL;
}

TbBool IsSamplePlaying(long source_id, short smp_id, TbSampleHandle handle)
{
#if 0
    TbBool ret;
    asm volatile ("call ASM_IsSamplePlaying\n"
        : "=r" (ret) : "a" (source_id), "d" (smp_id), "b" (handle));
    return ret;
#endif
    if (!SoundInstalled || !SoundAble || !SoundActive)
        return false;

    if ((smp_id == 0) && (source_id == 0))
    {
        SNDSAMPLE *p_sample;

        p_sample = (SNDSAMPLE *)handle;
        return AIL_sample_status(p_sample) == SNDSMP_PLAYING;
    }
    else if (smp_id == 0)
    {
        return FindSampleInfoSrcPlaying(source_id) != NULL;
    }
    else
    {
        return FindSampleInfoSrcSmpPlaying(source_id, smp_id) != NULL;
    }
}

struct SampleInfo *PlaySampleFromAddress(long source_id, short smp_id,
  short volume, ushort pan, short pitch, sbyte loop_count,
  ubyte a7, void *address)
{
    struct SampleInfo *p_smpinf;
    TbBool skip_request;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return NULL;

    p_smpinf = NULL;
    skip_request = false;
    switch (a7)
    {
    case 1: // Just play, do not care for repeats
        p_smpinf = FindSampleInfoAnyDone();
        break;
    case 2: // Play only if not already playing given sample with given source
        skip_request = FindSampleInfoSrcSmpNotDone(source_id, smp_id) != NULL;
        if (!skip_request)
            p_smpinf = FindSampleInfoAnyDone();
        break;
    case 3: // Play replacing any current playback of given sample with given source
        p_smpinf = FindSampleInfoSrcSmp(source_id, smp_id);
        if (p_smpinf == NULL)
          p_smpinf = FindSampleInfoAnyDone();
        break;
    }

    if (p_smpinf == NULL || skip_request)
        return NULL;

    AIL_init_sample(p_smpinf->SampleHandle);

    if (address == NULL)
        return NULL;

    AIL_set_sample_file(p_smpinf->SampleHandle, address, 1);
    AIL_set_sample_volume(p_smpinf->SampleHandle, volume);
    if (StereoSound)
        AIL_set_sample_pan(p_smpinf->SampleHandle, pan);
    AIL_set_sample_loop_count(p_smpinf->SampleHandle, loop_count + 1);
    AIL_start_sample(p_smpinf->SampleHandle);
    p_smpinf->SourceID = source_id;
    p_smpinf->SampleNumber = smp_id;
    p_smpinf->SampleVolume = volume;
    p_smpinf->SamplePan = pan;
    p_smpinf->SamplePitch = pitch;
    p_smpinf->FadeState = 0;
    p_smpinf->FadeStopFlag = 0;
    p_smpinf->UserFlag = 0;
    return p_smpinf;
}

void ReleaseLoopedSample(ushort source_id, short smp_id)
{
    // TODO the source_id should be of long type
#if 0
    asm volatile ("call ASM_ReleaseLoopedSample\n"
        : : "a" (source_id),  "d" (smp_id));
#endif
    struct SampleInfo *p_smpinf;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if ((ulong)source_id == p_smpinf->SourceID && smp_id == p_smpinf->SampleNumber) {
            if (AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING)
                AIL_set_sample_loop_count(p_smpinf->SampleHandle, 1);
        }
    }
}

void SetSamplePitch(long source_id, short smp_id, short pitch)
{
    struct SampleInfo *p_smpinf;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if ((ulong)source_id == p_smpinf->SourceID && smp_id == p_smpinf->SampleNumber
          && AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING)
        {
            if (pitch > 0 && p_smpinf->SamplePitch != pitch) {
                AIL_set_sample_playback_rate(p_smpinf->SampleHandle, pitch * SampleRate / 100);
                p_smpinf->SamplePitch = pitch;
            }
        }
    }
}

void SetSampleVolume(long source_id, short smp_id, short volume)
{
    struct SampleInfo *p_smpinf;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if ((ulong)source_id == p_smpinf->SourceID && smp_id == p_smpinf->SampleNumber
          && AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING)
        {
            if (volume <= 127 && volume != p_smpinf->SampleVolume)
            {
                AIL_set_sample_volume(p_smpinf->SampleHandle, volume);
                p_smpinf->SampleVolume = volume;
            }
        }
    }
}

void SetSamplePan(long source_id, short smp_id, ushort pan)
{
    struct SampleInfo *p_smpinf;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if ((ulong)source_id == p_smpinf->SourceID && smp_id == p_smpinf->SampleNumber
          && AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING)
        {
            if (pan != p_smpinf->SamplePan)
            {
                AIL_set_sample_pan(p_smpinf->SampleHandle, pan);
                p_smpinf->SamplePan = pan;
            }
        }
    }
}

void StopSample(long source_id, short smp_id)
{
    struct SampleInfo *p_smpinf;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        if ((ulong)source_id == p_smpinf->SourceID && smp_id == p_smpinf->SampleNumber
             && AIL_sample_status(p_smpinf->SampleHandle) != SNDSMP_DONE)
        {
            AIL_end_sample(p_smpinf->SampleHandle);
        }
    }
}

void StopAllSamples(void)
{
    struct SampleInfo *p_smpinf;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    StopAllSampleFadeTimers();
    for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
    {
        AIL_end_sample(p_smpinf->SampleHandle);
        p_smpinf->SampleNumber = 0;
        p_smpinf->SourceID = 0;
        p_smpinf->FadeState = 0;
        p_smpinf->FadeStopFlag = 0;
    }
    StopSampleQueueList();
}

/******************************************************************************/
