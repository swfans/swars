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

#include "ail.h"
#include "aildebug.h"
/******************************************************************************/
extern ushort cd_first;
extern TbBool CDAble;
extern TbBool CDTimerActive;
extern long CDCount_handle;
extern ushort CurrentCDTrack;
extern ulong TrackLength;
extern volatile ulong CDCountdown;
extern TbBool is_da_track[99];
extern ulong track_start_sector[99];
extern ulong track_lengths[99];

void ASM_cbCDCountdown(void *data);

ushort GetCDFirst(void)
{
  return cd_first;
}

TbBool GetCDAble(void)
{
  return CDAble;
}

TbBool is_daudio_track(ushort trkno)
{
    return is_da_track[trkno];
}

ulong GetCDTrackStartSector(ushort trkno)
{
    return track_start_sector[trkno];
}

ulong GetCDTrackLength(ushort trkno)
{
    return track_lengths[trkno];
}

ushort cd_stop(ushort a1)
{
    ushort ret;
    asm volatile ("call ASM_cd_stop\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

ushort cd_play(ushort cd, ulong start, ulong len)
{
    int ret;
    asm volatile ("call ASM_cd_play\n"
        : "=r" (ret) : "a" (cd), "d" (start), "b" (len));
    return ret;
}

void PlayCDTrack(ushort trkno)
{
#if 0
    int ret;
    asm volatile ("call ASM_PlayCDTrack\n"
        : "=r" (ret) : "a" (trkno));
#endif
    ulong start_sect, len_sect;
    ushort cd;

    if (!CDAble)
        return;
    if (!is_daudio_track(trkno))
        return;
    start_sect = GetCDTrackStartSector(trkno);
    len_sect = GetCDTrackLength(trkno);
    if (!CurrentCDTrack || CurrentCDTrack != trkno)
    {
        if (CDTimerActive == 1)
        {
            AIL_release_timer_handle(CDCount_handle);
            CDTimerActive = 0;
        }
        StopCD();
        TrackLength = len_sect / 75;
        CDCountdown = 5 * (len_sect / 75 / 5) + 5;
        CDCount_handle = AIL_register_timer(ASM_cbCDCountdown);
        AIL_set_timer_period(CDCount_handle, 5000000);
        cd = GetCDFirst();
        cd_play(cd, start_sect, len_sect);
        AIL_start_timer(CDCount_handle);
        CDTimerActive = 1;
        CurrentCDTrack = trkno;
    }
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

void InitRedbook(void)
{
    asm volatile ("call ASM_InitRedbook\n"
        :  :  : "eax" );
}

/******************************************************************************/
