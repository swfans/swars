/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file cd.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Functions for CD Audio playback control.
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
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "bfscd.h"
#include "bffile.h"
#include "bfsvaribl.h"
#include "aildebug.h"
#include "oggvorbis.h"
#include "snderr.h"
/******************************************************************************/

const char *cd_errors[] = {
    "Write-protect violation",
    "Unknown unit",
    "Drive not ready",
    "Unknown command",
    "CRC error",
    "Bad drive request structure length",
    "Seek error",
    "Unknown media",
    "Sector not found",
    "Printer out of paper",
    "Write fault",
    "Read fault",
    "General failure",
    "Reserved",
    "Reserved",
    "Invalid disk change",
};

ushort cd_total;
ushort cd_first;
TbBool CDAble = true;
TbBool CDTimerActive;
long CDCount_handle;
ushort CurrentCDTrack;
ulong TrackLength;
volatile ulong CDCountdown;
TbBool is_da_track[99];
ulong track_start_sector[99];
ulong track_lengths[99];
sbyte InitialCDVolume = -1;

OggVorbisStream sound_music_stream;

/** CD device type.
 */
ubyte CDType = CDTYP_NONE;

/** Directory with music files.
 */
char music_dir[FILENAME_MAX];

/******************************************************************************/

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

ushort cd_getaudiodiscinfo(ushort a1)
{
    assert(!"not implemented");
    return 0;
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
    assert(!"not implemented");
    return 0;
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
              "%s" FS_SEP_STR "track_%i.ogg",
              music_dir, trkno - 1);
        is_da_track[trkno] = (access(file_name, F_OK) == 0);
        if (!is_da_track[trkno] && trkno > 4) {
            sprintf(SoundProgressMessage, "BF106 - No tracks beyond %d\n", (int)(trkno-1));
            SoundProgressLog(SoundProgressMessage);
            break;
        }
    }
    for (trkno++; trkno < 99; trkno++) {
        is_da_track[trkno] = false;
    }
}

void PlayCDTrack(ushort trkno)
{
    char file_name[FILENAME_MAX];
    ulong start_sect, len_sect;
    ushort i;

    if (!CDAble)
        return;
    if (!is_daudio_track(trkno)) {
        sprintf(SoundProgressMessage, "BF103 - No audio track %d\n", (int)trkno);
        SoundProgressLog(SoundProgressMessage);
        return;
    }
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
              "%s" FS_SEP_STR "track_%i.ogg",
              music_dir, trkno - 1);

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

void InitMusicOGG(const char *nmusic_dir)
{
    if (!GetSoundAble() && !GetMusicAble())
        AIL_startup();

    strncpy(music_dir, nmusic_dir, sizeof(music_dir));

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
