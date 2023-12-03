/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file hvline.c
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
#include <assert.h>
#include "bfline.h"
#include "bfscreen.h"

TbResult LbDrawHVLine(long X1, long Y1, long X2, long Y2, TbPixel colour)
{
    long clipX1, clipX2, clipY1, clipY2;
    long maxX, maxY, x, y;
    ubyte *ptr;
    ubyte *m;

    // Sort the points on X axis
    if (X1 > X2)
    {
        clipX1 = X2;
        clipX2 = X1;
    }
    else
    {
        clipX1 = X1;
        clipX2 = X2;
    }
    // Clip them
    maxX = lbDisplay.GraphicsWindowWidth - 1;
    if (clipX2 < 0)
        return Lb_FAIL;
    if (clipX1 > maxX)
        return Lb_FAIL;
    if (clipX1 < 0)
        clipX1 = 0;
    if (clipX2 > maxX)
        clipX2 = maxX;

    // Sort the points on Y axis
    if (Y1 > Y2)
    {
        clipY1 = Y2;
        clipY2 = Y1;
    }
    else
    {
        clipY1 = Y1;
        clipY2 = Y2;
    }
    // Clip them
    maxY = lbDisplay.GraphicsWindowHeight - 1;
    if (clipY2 < 0)
        return Lb_FAIL;
    if (clipY1 > maxY)
        return Lb_FAIL;
    if (clipY1 < 0)
        clipY1 = 0;
    if (clipY2 > maxY)
        clipY2 = maxY;

    ptr = &lbDisplay.GraphicsWindowPtr[clipX1 + lbDisplay.GraphicsScreenWidth * clipY1];
    if (clipX2 == clipX1)
    { // Vertical line
        y = clipY2 - clipY1 + 1;
        assert(y > 0);
        if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4)
        {
            m = lbDisplay.GlassMap;
            for (; y > 0; y--)
            {
                *ptr = m[(colour << 8) + *ptr];
                ptr += lbDisplay.GraphicsScreenWidth;
            }
        }
        else if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8)
        {
            m = lbDisplay.GlassMap;
            for (; y > 0; y--)
            {
                *ptr = m[(*ptr << 8) + colour];
                ptr += lbDisplay.GraphicsScreenWidth;
            }
        }
        else
        {
            for (; y > 0; y--)
            {
                *ptr = colour;
                ptr += lbDisplay.GraphicsScreenWidth;
            }
        }
    }
    else
    { // Horizonal line
        x = clipX2 - clipX1 + 1;
        assert(x > 0);
        if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4)
        {
            m = lbDisplay.GlassMap;
            for (; x > 0; x--)
            {
                *ptr = m[(colour << 8) + *ptr];
                ptr++;
            }
        }
        else if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8)
        {
            m = lbDisplay.GlassMap;
            for (; x > 0; x--)
            {
                *ptr = m[(*ptr << 8) + colour];
                ptr++;
            }
        }
        else
        {
            memset(ptr, colour, x);
        }
    }
    return Lb_SUCCESS;
}

/******************************************************************************/
