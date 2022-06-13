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

TbResult LbDrawBox(long X1, long Y1, long X2, long Y2, TbPixel colour)
{
    if (lbDisplay.DrawFlags & Lb_SPRITE_OUTLINE)
    {
        if (X2 < 1 || Y2 < 1)
            return Lb_FAIL;
        LbDrawHVLine(X1, Y1, X2 + X1 - 1, Y1, colour);
        LbDrawHVLine(X1, Y2 + Y1 - 1, X2 + X1 - 1, Y2 + Y1 - 1, colour);
        if (Y2 > 2)
        {
            LbDrawHVLine(X1, Y1 + 1, X1, Y2 + Y1 - 2, colour);
            LbDrawHVLine(X2 + X1 - 1, Y1 + 1, X2 + X1 - 1, Y2 + Y1 - 2, colour);
        }
    } else
    {
        LbDrawBoxClip(X1, Y1, X2, Y2, colour);
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
