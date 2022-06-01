/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gpalette.cpp
 *     Implementation of related functions.
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
#include "bfpalette.h"

#include <string.h>

TbResult LbPaletteFade(ubyte *from_pal, ubyte arg2, ubyte fade_steps)
{
// code at 0001:00098ac0
}

int LbPaletteStopOpenFade_UNUSED()
{
// code at 0001:00098cb8
}

TbPixel LbPaletteFindColour_UNUSED(ubyte *pal, ubyte rval, ubyte gval, ubyte bval)
{
// code at 0001:00098ccc
}

int LbPaletteDraw_UNUSED()
{
// code at 0001:00099098
}

TbResult LbPaletteDataFillBlack(ubyte *palette)
{
    memset(palette, 0, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

TbResult LbPaletteDataFillWhite(ubyte *palette)
{
    memset(palette, 0x3F, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

/******************************************************************************/
