/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file pixel.asm
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
#include <assert.h>
#include "bfpixel.h"
#include "bfscreen.h"

TbResult LbDrawPixelClip(long x, long y, TbPixel colour)
{
    assert(!"not implemented");
// code at 0001:000a123d
}

TbResult LbDrawPixel(long x, long y, TbPixel colour)
{
    lbDisplay.GraphicsWindowPtr[x + lbDisplay.GraphicsScreenWidth * y] = colour;
}


/******************************************************************************/
