/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfaudio.h
 *     Header file for bfaudio.c.
 * @par Purpose:
 *     Set of general audio-related calls.
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
#ifndef BFSOUNDLIB_BFAUDIO_H_
#define BFSOUNDLIB_BFAUDIO_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

typedef struct AudioInitOptions AudioInitOptions;

struct AudioInitOptions {
  const char *IniPath;
  const char *SoundDriverPath;
  const char *SoundDataPath;
  u16 SoundType;
  u16 AbleFlags;
  i16 SelectedWin95MidiDevice;
  i16 SelectedWin95WaveDevice;
  u8 MaxSamples;
  u8 StereoOption;
  u8 AutoScan;
  u8 DisableDangerMusic;
  u8 DisableLoadSounds;
  u8 DisableLoadMusic;
  u8 UseCurrentAwe32Soundfont;
  u8 UseMultiMediaExtensions;
  u8 InitStreamedSound;
  u8 InitRedbookAudio;
};

#pragma pack()
/******************************************************************************/

void EnsureAILStartup(void);
void InitAudio(AudioInitOptions *audOpts);
void FreeAudio(void);

void SetSoundMasterVolume(long vol);
void SetMusicMasterVolume(long vol);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_BFAUDIO_H_
