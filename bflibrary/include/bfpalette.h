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

struct Palette { // sizeof=3
    UBYTE Red; // offset=0
    UBYTE Green; // offset=1
    UBYTE Blue; // offset=2
};

typedef struct Palette Palette;

TbResult LbPaletteFade(UBYTE *from_pal, UBYTE arg2, UBYTE fade_steps);
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
