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

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "bffile.h"
#include "bfsvaribl.h"
#include "ail.h"
#include "aildebug.h"
#include "snderr.h"
#include "oggvorbis.h"
#include "game_data.h"
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
extern sbyte InitialCDVolume;

extern OggVorbisStream sound_music_stream;

/** CD device type.
 */
ubyte CDType = CDTYP_NONE;

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

ushort cd_init(void)
{
    assert(!"not implemented");
    return 0;
}

ushort cd_stop(ushort a1)
{
    assert(!"not implemented");
    return 0;
}

ushort cd_resume(ushort a1)
{
    assert(!"not implemented");
    return 0;
}

ushort cd_play(ushort cd, ulong start, ulong len)
{
    int ret;
    asm volatile ("call ASM_cd_play\n"
        : "=r" (ret) : "a" (cd), "d" (start), "b" (len));
    return ret;
}

sbyte GetCDAudioVolume(void)
{
    assert(!"not implemented");
    return 0;
}

void SetCDAudioVolume(short vol)
{
    assert(!"not implemented");
}

void FreeCDAudio(void)
{
    assert(!"not implemented");
}

sbyte GetCDVolume(void)
{
    sbyte vol;
    float fvol;

    if (!CDAble)
        return -1;
    switch (CDType)
    {
    case CDTYP_REAL:
        vol = GetCDAudioVolume();
        break;
    case CDTYP_OGG:
        fvol = ogg_vorbis_stream_get_gain(&sound_music_stream);
        vol = fvol * (127.f / 1.f);
        break;
    }
    return vol;
}

void SetCDVolume(short vol)
{
    if (!CDAble)
        return;
    if (InitialCDVolume == -1)
        return;
    if (vol > 127)
        vol = 127;
    switch (CDType)
    {
    case CDTYP_REAL:
        SetCDAudioVolume(2 * vol);
        break;
    case CDTYP_OGG:
        ogg_vorbis_stream_set_gain(&sound_music_stream, vol * (1.f / 127.f));
        break;
    }
}

void ogg_list_music_tracks(void)
{
    ushort trkno;

    trkno = 0;
    is_da_track[trkno] = false;
    for (trkno++; trkno < 99; trkno++) {
        char file_name[FILENAME_MAX];

        snprintf(file_name, sizeof(file_name),
              "%s" FS_SEP_STR "music" FS_SEP_STR "track_%i.ogg",
              GetDirectoryHdd(), trkno - 1);
        is_da_track[trkno] = (access(file_name, F_OK) == 0);
        if (!is_da_track[trkno] && trkno > 4)
            break;
    }
    for (trkno++; trkno < 99; trkno++) {
        is_da_track[trkno] = false;
    }
}

void PlayCDTrack(ushort trkno)
{
#if 0
    int ret;
    asm volatile ("call ASM_PlayCDTrack\n"
        : "=r" (ret) : "a" (trkno));
#endif
    char file_name[FILENAME_MAX];
    ulong start_sect, len_sect;
    ushort i;

    if (!CDAble)
        return;
    if (!is_daudio_track(trkno))
        return;
    if (CurrentCDTrack != 0 && CurrentCDTrack == trkno)
        return;
    StopCD();

    switch (CDType)
    {
    case CDTYP_REAL:
        start_sect = GetCDTrackStartSector(trkno);
        len_sect = GetCDTrackLength(trkno);
        TrackLength = len_sect / 75;
        CDCountdown = 5 * (len_sect / 75 / 5) + 5;
        i = GetCDFirst();
        cd_play(i, start_sect, len_sect);
        sprintf(SoundProgressMessage, "BF103 - CDA play track %d sect %lu len %lu\n", (int)trkno, start_sect, len_sect);
        SoundProgressLog(SoundProgressMessage);
        break;
    case CDTYP_OGG:
        snprintf(file_name, sizeof(file_name),
              "%s" FS_SEP_STR "music" FS_SEP_STR "track_%i.ogg",
              GetDirectoryHdd(), trkno - 1);

        ogg_vorbis_stream_open (&sound_music_stream, file_name);
        ogg_vorbis_stream_play(&sound_music_stream);
        sprintf(SoundProgressMessage, "BF103 - CDA play \"%s\"\n", file_name);
        SoundProgressLog(SoundProgressMessage);
        break;
    }
    CDCount_handle = AIL_register_timer(ASM_cbCDCountdown);
    AIL_set_timer_period(CDCount_handle, 5000000);
    AIL_start_timer(CDCount_handle);
    CDTimerActive = true;
    CurrentCDTrack = trkno;
}

void PauseCD(void)
{
    ushort i;

    if (!CDAble)
        return;
    if (CurrentCDTrack == 0)
        return;

    switch (CDType)
    {
    case CDTYP_REAL:
        i = GetCDFirst();
        cd_stop(i);
        break;
    case CDTYP_OGG:
        ogg_vorbis_stream_pause(&sound_music_stream);
        break;
    }

    if (CDTimerActive)
        AIL_stop_timer(CDCount_handle);
}

void ResumeCD(void)
{
    ushort i;

    if (!CDAble)
        return;
    if (CurrentCDTrack == 0)
        return;

    switch (CDType)
    {
    case CDTYP_REAL:
        i = GetCDFirst();
        cd_resume(i);
        break;
    case CDTYP_OGG:
        ogg_vorbis_stream_play(&sound_music_stream);
        break;
    }

    if (CDTimerActive)
        AIL_start_timer(CDCount_handle);
}

void StopCD(void)
{
    ushort i;

    if (!CDAble)
        return;
    if (CDTimerActive)
    {
        CDTimerActive = false;
        AIL_release_timer_handle(CDCount_handle);
    }
    switch (CDType)
    {
    case CDTYP_REAL:
        i = GetCDFirst();
        cd_stop(i);
        break;
    case CDTYP_OGG:
        ogg_vorbis_stream_stop(&sound_music_stream);
        break;
    }
    CurrentCDTrack = 0;
}

void InitRedbook(void)
{
#if 0
    asm volatile ("call ASM_InitRedbook\n"
        :  :  : "eax" );
#endif
    if (!GetSoundAble() && !GetMusicAble())
        AIL_startup();

    if (cd_init()) {
        InitialCDVolume = GetCDVolume();
        CDType = CDTYP_REAL;
    } else {
        sprintf(SoundProgressMessage, "BF101 - real cd init - failed - CDA disabled\n");
        SoundProgressLog(SoundProgressMessage);
        CDAble = false;
        CDType = CDTYP_NONE;
    }
}

void InitMusicOGG(void)
{
    if (!GetSoundAble() && !GetMusicAble())
        AIL_startup();

    if (ogg_vorbis_stream_init(&sound_music_stream)) {
        InitialCDVolume = GetCDVolume();
        CDType = CDTYP_OGG;
        ogg_list_music_tracks();
    } else {
        sprintf(SoundProgressMessage, "BF101 - ogg vorbis stream init - failed - CDA disabled\n");
        SoundProgressLog(SoundProgressMessage);
        CDAble = false;
        CDType = CDTYP_NONE;
    }
}

void FreeCD(void)
{
    if (InitialCDVolume != -1) {
        InitialCDVolume = 0;
        SetCDVolume(0);
    }
    switch (CDType)
    {
    case CDTYP_REAL:
        FreeCDAudio();
        break;
    case CDTYP_OGG:
        ogg_vorbis_stream_free(&sound_music_stream);
        break;
    }
}

/******************************************************************************/
