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
    ulong mbs0;
    ulong mbs1;
    ulong mbs2;
    ulong mbs3;
    ulong mbs4;
    ulong mbs5;
    ulong mbs6;
    ulong mbs7;
};

#pragma pack()
/******************************************************************************/

void InitMusic(void);
void FreeMusic(void);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_BFMUSIC_H_
