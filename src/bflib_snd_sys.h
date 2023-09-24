/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_snd_sys.h
 *     Header file for bflib_snd_sys.c.
 * @par Purpose:
 *     Sound playback support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_SNDSYS_H
#define BFLIB_SNDSYS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct MusicBankHead {
  long info_offset;
  long data_offset;
  long info_size;
  long data_size;
};

struct BfMusicInfo {
  char FileName[12];
  long mifld_12;
  short mifld_16;
  ubyte *DataBeg;
  long mifld_22;
  ubyte *DataEnd;
  short mifld_30;
};

#pragma pack()
/******************************************************************************/
void StopAllSamples(void);
void FreeAudio(void);

void SetMusicVolume(int msec, ubyte volume);

int LoadMusic(ushort bankNo);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
