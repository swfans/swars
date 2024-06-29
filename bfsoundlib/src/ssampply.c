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
extern ulong SampleRate;

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

/******************************************************************************/

TbBool IsSamplePlaying(long source_id, short smp_id, TbSampleHandle handle)
{
#if 0
    TbBool ret;
    asm volatile ("call ASM_IsSamplePlaying\n"
        : "=r" (ret) : "a" (source_id), "d" (smp_id), "b" (handle));
    return ret;
#endif
    SNDSAMPLE *p_sample;
    struct SampleInfo *p_smpinf;

    p_sample = (SNDSAMPLE *)handle;
    if (!SoundInstalled || !SoundAble || !SoundActive)
        return false;

    if ((smp_id == 0) && (source_id == 0))
    {
        return AIL_sample_status(p_sample) == SNDSMP_PLAYING;
    }
    else if (smp_id == 0)
    {
        for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
        {
            if (((ulong)source_id == p_smpinf->SourceID)
              && AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING)
                return true;
        }
        return false;
    }
    else
    {
        for (p_smpinf = sample_id; p_smpinf <= end_sample_id; p_smpinf++)
        {
            if (((ulong)source_id == p_smpinf->SourceID)
              && (smp_id == p_smpinf->SampleNumber)
              && AIL_sample_status(p_smpinf->SampleHandle) == SNDSMP_PLAYING)
                return true;
        }
        return false;
    }
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
