/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sndtimer.c
 *     Bullfrog sound timers.
 * @par Purpose:
 *     Implementation of Bullfrog sound timers API.
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
#include <assert.h>

#include "sndtimer.h"
#include "bftypes.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "aildebug.h"
/******************************************************************************/

struct sound_timer_inf sound_timer[5];

extern TbBool MusicInstalled;
extern TbBool SoundInstalled;

/******************************************************************************/

void InitAllBullfrogSoundTimers(void)
{
    if (!MusicInstalled && !SoundInstalled)
        return;
    LbMemorySet(&sound_timer, 0, 5 * sizeof(struct sound_timer_inf));
}

ushort SetupBullfrogSoundTimer(ushort freq, void (*cb)(void*))
{
    struct sound_timer_inf *stinf;
    ushort st;

    if (!MusicInstalled && !SoundInstalled)
        return 0;

    for (st = 0; st < 5; st++)
    {
        stinf = &sound_timer[st];
        if (stinf->used)
            break;
    }
    if (st >= 5)
        return 0;

    stinf = &sound_timer[st];
    stinf->handle = AIL_register_timer(cb);
    AIL_set_timer_frequency(stinf->handle, freq);
    AIL_start_timer(stinf->handle);
    stinf->used = 1;
    return st+1;
}

void ReleaseBullfrogSoundTimer(ushort st)
{
    struct sound_timer_inf *stinf;

    if (!MusicInstalled && !SoundInstalled)
        return;

    stinf = &sound_timer[st-1];
    if (stinf->used == 1)
    {
        AIL_release_timer_handle(stinf->handle);
        stinf->used = 0;
    }
}

/******************************************************************************/
