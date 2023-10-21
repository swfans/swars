/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file variable.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Handles for global properties.
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

#include "bfsvaribl.h"

#include "bfsound.h"
#include "bfscd.h"
/******************************************************************************/

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

extern TbBool SoundInstalled;
extern TbBool DisableLoadSounds;

extern ushort NumberOfSamples;
extern struct DIG_DRIVER *SoundDriver;

extern int CurrentSoundMasterVolume; // = 127;
extern int CurrentMusicMasterVolume; // = 127;

extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;

extern ushort NumberOfSongs;
extern struct MDI_DRIVER *MusicDriver;
extern struct SNDSEQUENCE *SongHandle;

extern TbBool SoundAble;
extern TbBool SoundActive;
extern TbBool StereoSound;

extern sbyte CDSpeed;

/******************************************************************************/

ushort GetNumberOfSamples(void)
{
  return NumberOfSamples;
}

TbBool GetSoundAble(void)
{
    return SoundAble;
}

TbBool GetMusicAble(void)
{
    return MusicAble;
}

TbBool GetSoundInstalled(void)
{
    return SoundInstalled;
}

TbBool GetSoundActive(void)
{
    return SoundActive;
}

TbBool GetMusicActive(void)
{
    return MusicActive;
}

TbBool GetMusicInstalled(void)
{
    return MusicInstalled;
}

TbBool GetStereoSound(void)
{
    return StereoSound;
}

ushort GetNumberOfSongs(void)
{
    return NumberOfSongs;
}

int GetCurrentSoundMasterVolume(void)
{
    return CurrentSoundMasterVolume;
}

int GetCurrentMusicMasterVolume(void)
{
    return CurrentMusicMasterVolume;
}

struct DIG_DRIVER *GetSoundDriver(void)
{
    return SoundDriver;
}

struct MDI_DRIVER *GetMusicDriver(void)
{
    return MusicDriver;
}

int GetSelected95MidiDevice(void)
{
    return 0;
}

int GetSelected95WaveDevice(void)
{
    return 0;
}

struct SampleInfo *GetFirstSampleInfoStructure(void)
{
    return sample_id;
}

struct SampleInfo *GetLastSampleInfoStructure(void)
{
    return end_sample_id;
}

struct SNDSEQUENCE *GetSongHandle(void)
{
  return SongHandle;
}

sbyte GetCDSpeed(ubyte n)
{
    ubyte speed;

    if (n)
        speed = CDSpeedTest(0);
    else
        speed = CDSpeed;
    return speed;
}

/******************************************************************************/
