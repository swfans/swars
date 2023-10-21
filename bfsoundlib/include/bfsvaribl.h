/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfsvaribl.h
 *     Header file for variable.c.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFSOUNDLIB_BFSVARIBL_H_
#define BFSOUNDLIB_BFSVARIBL_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)

struct DIG_DRIVER;
struct MDI_DRIVER;
struct SNDSAMPLE;
struct SNDSEQUENCE;
struct SampleInfo;

#pragma pack()

/******************************************************************************/

ushort GetNumberOfSamples(void);

TbBool GetSoundAble(void);
TbBool GetMusicAble(void);

TbBool GetSoundActive(void);
TbBool GetMusicActive(void);

TbBool GetSoundInstalled(void);
TbBool GetMusicInstalled(void);

TbBool GetStereoSound(void);
ushort GetNumberOfSongs(void);

int GetCurrentSoundMasterVolume(void);
int GetCurrentMusicMasterVolume(void);

struct DIG_DRIVER *GetSoundDriver(void);
struct MDI_DRIVER *GetMusicDriver(void);

int GetSelected95MidiDevice(void);
int GetSelected95WaveDevice(void);

struct SampleInfo *GetFirstSampleInfoStructure(void);
struct SampleInfo *GetLastSampleInfoStructure(void);

struct SNDSEQUENCE *GetSongHandle(void);
sbyte GetCDSpeed(ubyte n);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_BFSVARIBL_H_
