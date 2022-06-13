/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gcircle.c
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
#include "bfscreen.h"
#include "bfcircle.h"
#include "bfline.h"
#include "bfpixel.h"

TbResult LbDrawCircleOutline(long x, long y, ulong radius, TbPixel colour)
{
#if 1
    assert(!"not implemented");
#else
// code at 0001:000a182c
#endif
    return Lb_SUCCESS;
}

TbResult LbDrawCircleFilled(long x, long y, ulong radius, TbPixel colour)
{
#if 1
    assert(!"not implemented");
#else
    long where1, r1, r2;

    if (radius < 1)
    {
        LbDrawPixelClip(x, y, colour);
        return Lb_SUCCESS;
    }
    if (radius == 1)
    {
        LbDrawPixelClip(x - 1, y, colour);
        LbDrawPixelClip(x, y - 1, colour);
        LbDrawPixelClip(x + 1, y, colour);
        LbDrawPixelClip(x, y + 1, colour);
        LbDrawPixelClip(x, y, colour);
        return Lb_SUCCESS;
    }
    r1 = 0;
    r2 = radius;
    where1 = 3 - 2 * radius;
    LbDrawHVLine(x - radius, y, radius + x, y, colour);
    while ( 1 )
    {
        if (where1 >= 0)
        {
            LbDrawHVLine(x - r1, y - r2, r1 + x, y - r2, colour);
            LbDrawHVLine(x - r1, r2 + y, r1 + x, r2 + y, colour);
            where1 += 4 * (r1 - r2) + 10;
            r2--;
        }
        else
        {
            where1 += 4 * r1 + 6;
        }
        r1++;
        if (r1 >= r2)
          break;
        LbDrawHVLine(x - r2, y - r1, r2 + x, y - r1, colour);
        LbDrawHVLine(x - r2, r1 + y, r2 + x, r1 + y, colour);
    }
    if (r1 == r2)
    {
        LbDrawHVLine(x - r2, y - r1, r2 + x, y - r1, colour);
        LbDrawHVLine(x - r2, r1 + y, r2 + x, r1 + y, colour);
    }
#endif
    return Lb_SUCCESS;
}

TbResult LbDrawCircle(long x, long y, ulong radius, TbPixel colour)
{
    TbResult ret;

    if (lbDisplay.DrawFlags & Lb_SPRITE_OUTLINE)
        ret = LbDrawCircleOutline(x, y, radius, colour);
    else
        ret = LbDrawCircleFilled(x, y, radius, colour);
    return ret;
}

/******************************************************************************/
