/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfmusic.h
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
#ifndef BFSOUNDLIB_BFMUSIC_H_
#define BFSOUNDLIB_BFMUSIC_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct MusicBankSizes {
    i32 mbs0;
    i32 mbs1;
    i32 mbs2;
    i32 mbs3;
    i32 mbs4;
    i32 mbs5;
    i32 mbs6;
    i32 mbs7;
};

struct BfMusicInfo {
  char FileName[12];
  i32 mifld_12;
  i16 mifld_16;
  u8 *DataBeg;
  i32 mifld_22;
  u8 *DataEnd;
  i16 mifld_30;
};

#pragma pack()
/******************************************************************************/

void InitMusic(void);
void FreeMusic(void);

int LoadMusic(u16 bankNo);

void StopMusicIfActive(void);
void StopMusic(void);

void SetMusicVolume(int msec, u8 volume);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_BFMUSIC_H_
