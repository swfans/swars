/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gbox.cpp
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
#include "bfbox.h"
#include "bfscreen.h"
#include "bfline.h"

TbResult LbDrawBox(long X, long Y, long Width, long Height, TbPixel colour)
{
    if (lbDisplay.DrawFlags & Lb_SPRITE_OUTLINE)
    {
        if (Width < 1 || Height < 1)
            return Lb_FAIL;
        LbDrawHVLine(X, Y, X + Width - 1, Y, colour);
        LbDrawHVLine(X, Y + Height - 1, X + Width - 1, Y + Height - 1, colour);
        if (Height > 2)
        {
            LbDrawHVLine(X, Y + 1, X, Y + Height - 2, colour);
            LbDrawHVLine(X + Width - 1, Y + 1, X + Width - 1, Y + Height - 2, colour);
        }
    } else
    {
        LbDrawBoxClip(X, Y, Width, Height, colour);
    }
    return Lb_SUCCESS;
}

TbResult LbDrawBoxCoords(long X1, long Y1, long X2, long Y2, TbPixel colour)
{
    long XX, YY;

    XX = X2;
    YY = Y2;
    if (X1 > X2)
    {
        XX = X2 ^ X1 ^ X2;
        X1 = XX ^ X2 ^ X1;
    }
    if (Y1 > Y2)
    {
        YY = Y2 ^ Y1 ^ Y2;
        Y1 = YY ^ Y2 ^ Y1;
    }
    LbDrawBox(X1, Y1, XX - X1 + 1, YY - Y1 + 1, colour);
    return Lb_SUCCESS;
}

/******************************************************************************/
