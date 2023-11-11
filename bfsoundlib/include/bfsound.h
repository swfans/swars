/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfsound.h
 *     Header file for ???.
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
#ifndef BFSOUNDLIB_BFSOUND_H_
#define BFSOUNDLIB_BFSOUND_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct SNDSAMPLE;

struct SampleInfo {
  struct SNDSAMPLE *SampleHandle;
  long SampleVolume;
  ushort SamplePitch;
  ushort SamplePan;
  ushort FadeToVolume;
  ulong SourceID;
  short SampleNumber;
  ubyte FadeState;
  ubyte FadeStopFlag;
  ubyte FadeStep;
  ubyte UserFlag;
  ubyte SampleType;
};

#pragma pack()
/******************************************************************************/

void InitSound(void);
void FreeSound(void);

int LoadSounds(ubyte bank_no);
int AllocateSoundBankMemory(ushort snd_type);
//void DeInitSoundBank();

ubyte GetSoundTpNo(ushort snd_type);
//int SwitchOffSound();
//int SwitchOnSound();

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_BFSOUND_H_
