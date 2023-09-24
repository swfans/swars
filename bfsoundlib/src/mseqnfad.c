/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssamplst.c
 *     Music Sequence List support routines.
 * @par Purpose:
 *     Functions for handling a list/queue of music sequences.
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

#include "bfmusic.h"
#include "aildebug.h"
#include "mssal.h"
/******************************************************************************/

extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;

extern ushort SongCurrentlyPlaying;
extern SNDSEQUENCE *SongHandle;

/******************************************************************************/

void SetMusicVolume(int msec, ubyte volume)
{
    if (!MusicAble || !MusicInstalled)
        return;

    if (!MusicActive || !SongCurrentlyPlaying)
        return;

    if (volume > 127)
        return;

    if (AIL_sequence_status(SongHandle) == 2)
        return;

    AIL_set_sequence_volume(SongHandle, volume, msec);
}

/******************************************************************************/
