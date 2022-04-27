/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfpalette.h
 *     Header file for gpalette.cpp, spalette.cpp.
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
#ifndef BFLIBRARY_GPALETTE_H_
#define BFLIBRARY_GPALETTE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct Palette { // sizeof=3
    ubyte Red; // offset=0
    ubyte Green; // offset=1
    ubyte Blue; // offset=2
};

typedef struct Palette Palette;

#pragma pack()

TbResult LbPaletteFade(ubyte *from_pal, ubyte arg2, ubyte fade_steps);
int LbPaletteStopOpenFade();
int LbPaletteFindColour();
int LbPaletteDraw();

int LbPaletteGet();
int LbPaletteSet();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GPALETTE_H_
/******************************************************************************/
