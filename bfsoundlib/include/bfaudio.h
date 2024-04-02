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
  /** Custom path to where INI files are. If NULL, "$PWD/sound" is used. */
  const char *IniPath;
  /** Custom path to where sound and MIDI driver files are. If NULL, "$PWD/sound" is used. */
  const char *SoundDriverPath;
  /** Custom path to where sound bank files are. If NULL, "$PWD/sound" is used. */
  const char *SoundDataPath;
  ushort SoundType;
  ushort AbleFlags;
  short SelectedWin95MidiDevice;
  short SelectedWin95WaveDevice;
  ubyte MaxSamples;
  ubyte StereoOption;
  ubyte AutoScan;
  ubyte DisableDangerMusic;
  ubyte DisableLoadSounds;
  ubyte DisableLoadMusic;
  ubyte UseCurrentAwe32Soundfont;
  ubyte UseMultiMediaExtensions;
  ubyte InitStreamedSound;
  ubyte InitRedbookAudio;
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
