/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file box_clip.asm
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
#include <string.h>
#include "bfbox.h"
#include "bfscreen.h"

TbResult LbDrawBoxNoClip(long destX, long destY, ulong width, ulong height,
  TbPixel colour)
{
    ubyte *d;
    ubyte *m;
    long w, h, shift;

    d = &lbDisplay.GraphicsWindowPtr[destX + lbDisplay.GraphicsScreenWidth * destY];
    shift = lbDisplay.GraphicsScreenWidth - width;
    if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4)
    {
        m = lbDisplay.GlassMap;
        for (h = height; h >= 0; h--)
        {
            for (w = width; w >= 0; w--)
            {
                *d = m[(colour << 8) + (*d)];
                d++;
            }
            d += shift;
        }
    }
    else if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8)
    {
        m = lbDisplay.GlassMap;
        for (h = height; h >= 0; h--)
        {
            for (w = width; w >= 0; w--)
            {
                *d = m[((*d) << 8) + colour];
                d++;
            }
            d += shift;
        }
    }
    else
    {
        for (h = height; h >= 0; h--)
        {
            memset(d, colour, width);
            d += width;
            d += shift;
        }
    }
    return Lb_SUCCESS;
}

TbResult LbDrawBoxClip(long destX, long destY, ulong width, ulong height,
  TbPixel colour)
{
    long clpY;
    long clpwidth, clpheight;

    clpwidth = width;
    clpheight = height;
    clpY = destY;

    if (destY >= lbDisplay.GraphicsWindowHeight) {
        return Lb_FAIL;
    }
    if (destY < 0) {
        clpheight = destY + height;
        clpY = 0;
    }
    if (clpheight + clpY > lbDisplay.GraphicsWindowHeight) {
        clpheight = lbDisplay.GraphicsWindowHeight - clpY;
    }
    if (clpheight <= 0) {
        return Lb_FAIL;
    }
    if (destX >= lbDisplay.GraphicsWindowWidth) {
        return Lb_FAIL;
    }
    if (destX < 0) {
        clpwidth = destX + width;
        destX = 0;
    }
    if (clpwidth + destX > lbDisplay.GraphicsWindowWidth) {
        clpwidth = lbDisplay.GraphicsWindowWidth - destX;
    }
    if (clpwidth <= 0) {
        return Lb_FAIL;
    }

    long deltaX, deltaY;
    ubyte *d;
    ubyte *m;
    long w, h, shift;

    // After original, this function uses WScreen instead of precomputed
    // GraphicsWindowPtr. There doesn't seem to be any logical reason for that.
    deltaX = lbDisplay.GraphicsWindowX + destX;
    deltaY = lbDisplay.GraphicsScreenWidth * (lbDisplay.GraphicsWindowY + clpY);
    d = &lbDisplay.WScreen[deltaX + deltaY];
    shift = lbDisplay.GraphicsScreenWidth - clpwidth;
    if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4)
    {
        m = lbDisplay.GlassMap;
        for (h = clpheight; h >= 0; h--)
        {
            for (w = clpwidth; w >= 0; w--)
            {
                *d = m[(colour << 8) + (*d)];
                d++;
            }
            d += shift;
        }
    }
    else if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8)
    {
        m = lbDisplay.GlassMap;
        for (h = clpheight; h >= 0; h--)
        {
            for (w = clpwidth; w >= 0; w--)
            {
                *d = m[((*d) << 8) + colour];
                d++;
            }
            d += shift;
        }
    }
    else
    {
        for (h = clpheight; h >= 0; h--)
        {
            memset(d, colour, clpwidth);
            d += width;
            d += shift;
        }
    }
    return Lb_SUCCESS;
}

/******************************************************************************/
