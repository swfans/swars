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
    ubyte *ptr;

    if ((x < 0) || (x >= lbDisplay.GraphicsWindowWidth))
        return Lb_FAIL;
    if ((y < 0) || (y >= lbDisplay.GraphicsWindowHeight))
        return Lb_FAIL;

    ptr = &lbDisplay.GraphicsWindowPtr[lbDisplay.GraphicsScreenWidth * y + x];
    if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4)
    {
        *ptr = lbDisplay.GlassMap[(colour << 8) + *ptr];
    }
    else if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8)
    {
        *ptr = lbDisplay.GlassMap[(*ptr << 8) + colour];
    }
    else
    {
        *ptr = colour;
    }
    return Lb_SUCCESS;
}

TbResult LbDrawPixel(long x, long y, TbPixel colour)
{
    ubyte *ptr;
    ptr = &lbDisplay.GraphicsWindowPtr[x + lbDisplay.GraphicsScreenWidth * y];
    *ptr = colour;
    return Lb_SUCCESS;
}


/******************************************************************************/
