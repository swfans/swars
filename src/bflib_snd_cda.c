/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_snd_cda.c
 *     CD Audio playback support.
 * @par Purpose:
 *     Implement CD Audio playback support for games.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_snd_cda.h"

#include "bflib_snd_mss.h"
/******************************************************************************/
extern ushort cd_first;
extern TbBool CDAble;
extern TbBool CDTimerActive;
extern long CDCount_handle;
extern ushort CurrentCDTrack;

ushort GetCDFirst(void)
{
  return cd_first;
}

ushort cd_stop(ushort a1)
{
    ushort ret;
    asm volatile ("call ASM_cd_stop\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

void StopCD(void)
{
    ushort i;

    if (!CDAble)
        return;
    if ( CDTimerActive )
    {
        CDTimerActive = 0;
        AIL_release_timer_handle(CDCount_handle);
    }
    CurrentCDTrack = 0;
    i = GetCDFirst();
    cd_stop(i);
}

/******************************************************************************/
