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

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

/******************************************************************************/

TbBool IsSamplePlaying(long tng_offs, ushort smp_id, TbSampleHandle handle)
{
    TbBool ret;
    asm volatile ("call ASM_IsSamplePlaying\n"
        : "=r" (ret) : "a" (tng_offs), "d" (smp_id), "b" (handle));
    return ret;
}

void ReleaseLoopedSample(ushort sourceId, ushort fx)
{
#if 0
    asm volatile ("call ASM_ReleaseLoopedSample\n"
        : : "a" (sourceId),  "d" (fx));
#endif
    struct SampleInfo *smpinfo;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    for (smpinfo = sample_id; smpinfo <= end_sample_id; smpinfo++)
    {
        if (sourceId == smpinfo->SourceID && fx == smpinfo->SampleNumber) {
            if (AIL_sample_status(smpinfo->SampleHandle) == SNDSMP_PLAYING)
                AIL_set_sample_loop_count(smpinfo->SampleHandle, 1);
        }
    }
}

void StopAllSamples(void)
{
#if 0
    asm volatile ("call ASM_StopAllSamples\n"
        :  :  : "eax" );
#endif
    struct SampleInfo *smpinfo;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;
    StopAllSampleFadeTimers();
    for (smpinfo = sample_id; smpinfo <= end_sample_id; smpinfo++)
    {
        AIL_end_sample(smpinfo->SampleHandle);
        smpinfo->SampleNumber = 0;
        smpinfo->SourceID = 0;
        smpinfo->FadeState = 0;
        smpinfo->FadeStopFlag = 0;
    }
    StopSampleQueueList();
}

/******************************************************************************/
