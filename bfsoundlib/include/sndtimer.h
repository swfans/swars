/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sndtimer.h
 *     Header file for sndtimer.c.
 * @par Purpose:
 *     Bullfrog sound timers.
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
#ifndef BFSOUNDLIB_SNDTIMER_H_
#define BFSOUNDLIB_SNDTIMER_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct sound_timer_inf {
    ulong handle;
    ubyte used;
};

#pragma pack()
/******************************************************************************/

void InitAllBullfrogSoundTimers(void);
ushort SetupBullfrogSoundTimer(ushort freq, void (*cb)());
void ReleaseBullfrogSoundTimer(ushort st);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_SNDTIMER_H_
