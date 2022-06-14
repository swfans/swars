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
#include "bfline.h"
#include "bfscreen.h"

TbResult LbDrawHVLine(long X1, long Y1, long X2, long Y2, TbPixel colour)
{
    long clipX1, clipX2, clipY1, clipY2;
    long maxX, maxY, x, y;
    ubyte *ptr;
    ubyte *m;

    maxX = lbDisplay.GraphicsWindowWidth - 1;
    clipX1 = X1;
    clipX2 = X2;
    if (clipX1 > clipX2)
    {
        if (clipX1 < 0)
            return Lb_FAIL;
        if (clipX2 > maxX)
            return Lb_FAIL;
        if (clipX2 < 0)
            clipX1 = 0;
        if (clipX1 > maxX)
            clipX2 = maxX;
    }
    else
    {
        if (clipX2 < 0)
            return Lb_FAIL;
        if (clipX1 > maxX)
            return Lb_FAIL;
        if (clipX1 < 0)
            clipX1 = 0;
        if (clipX2 > maxX)
            clipX2 = maxX;
    }

    maxY = lbDisplay.GraphicsWindowHeight - 1;
    clipY1 = Y1;
    clipY2 = Y2;
    if (clipY1 > clipY2)
    {
        if (Y1 < 0)
            return Lb_FAIL;
        if (Y2 > maxY)
            return Lb_FAIL;
        if (Y2 < 0)
            clipY2 = 0;
        if (Y1 > maxY)
            clipY1 = maxY;
    }
    else
    {
        if (Y2 < 0)
            return Lb_FAIL;
        if (Y1 > maxY)
            return Lb_FAIL;
        if (Y1 < 0)
            clipY2 = 0;
        if (Y2 > maxY)
            clipY1 = maxY;
    }

    ptr = &lbDisplay.GraphicsWindowPtr[clipX1 + lbDisplay.GraphicsScreenWidth * clipY2];
    if (clipX2 == clipX1)
    {
        y = clipY1 - clipY2 + 1;
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
    {
        x = clipX2 - clipX1 + 1;
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
