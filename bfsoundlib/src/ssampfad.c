/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssamplst.c
 *     Sound Sample Fade support routines.
 * @par Purpose:
 *     Functions for handling sound samples fade.
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

#include "ssampfad.h"

#include "bfsound.h"
#include "aildebug.h"
/******************************************************************************/

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

extern long sample_fade_handle;

extern ubyte volatile samples_currently_fading;
extern ubyte volatile switch_off_sample_fade_timer;

/******************************************************************************/

void StopAllSampleFadeTimers(void)
{
    struct SampleInfo *smpinfo;

    if (samples_currently_fading == 0)
        return;
    switch_off_sample_fade_timer = 1;
    AIL_release_timer_handle(sample_fade_handle);
    samples_currently_fading = 0;
    for (smpinfo = sample_id; smpinfo <= end_sample_id; smpinfo++)
    {
        smpinfo->FadeState = 0;
        smpinfo->FadeStopFlag = 0;
    }
}

/******************************************************************************/
