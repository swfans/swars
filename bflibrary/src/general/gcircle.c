/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gcircle.c
 *     Implementation of related functions.
 * @par Purpose:
 *     Drawing circles.
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
  ubyte *p;
  long radM, radP, where1;

  if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4)
  {
    if (radius <= 1)
    {
        if ((x >= 0) && (x < lbDisplay.GraphicsWindowWidth) &&
          (y >= 0) && (y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y) * lbDisplay.GraphicsScreenWidth + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        return Lb_SUCCESS;
    }
    radP = 0;
    radM = radius;
    where1 = 3 - 2 * radius;
    while (radP < radM)
    {
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if (where1 >= 0) {
            where1 += 4 * (radP - radM) + 10;
            radM--;
        } else {
            where1 += 4 * radP + 6;
        }
        radP++;
    }
    if (radP == radM)
    {
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(colour << 8) + *p];
        }
    }
  }
  else if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8)
  {
    if (radius <= 1)
    {
        if ((x >= 0) && (x < lbDisplay.GraphicsWindowWidth) &&
          (y >= 0) && (y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y) * lbDisplay.GraphicsScreenWidth + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        return Lb_SUCCESS;
    }
    radP = 0;
    radM = radius;
    where1 = 3 - 2 * radius;
    while (radP < radM)
    {
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight))
        {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && y - (radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight))
        {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight))
        {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ( where1 >= 0 )
            where1 += 4 * (radP - radM--) + 10;
        else
            where1 += 4 * radP + 6;
        ++radP;
    }
    if (radP == radM)
    {
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radM + x >= 0)  && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight))
        {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight))
        {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight))
        {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = lbDisplay.GlassMap[(*p << 8) + colour];
        }
    }
  }
  else
  {
    if (radius <= 1)
    {
        if ((x >= 0) && (x < lbDisplay.GraphicsWindowWidth) &&
          (y >= 0) && (y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y) * lbDisplay.GraphicsScreenWidth + x];
            *p = colour;
        }
        return Lb_SUCCESS;
    }
    radP = 0;
    radM = radius;
    where1 = 3 - 2 * radius;
    while (radP < radM)
    {
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = colour;
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = colour;
        }
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && radM + y < lbDisplay.GraphicsWindowHeight ) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = colour;
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = colour;
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = colour;
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = colour;
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = colour;
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = colour;
        }
        if (where1 >= 0)
            where1 += 4 * (radP - radM--) + 10;
        else
            where1 += 4 * radP + 6;
        ++radP;
    }
    if (radP == radM)
    {
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = colour;
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radM >= 0) && (y - radM < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radM) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = colour;
        }
        if ((x - radP >= 0) && (x - radP < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + x - radP];
            *p = colour;
        }
        if ((radP + x >= 0) && (radP + x < lbDisplay.GraphicsWindowWidth) &&
          (radM + y >= 0) && (radM + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radM + y) * lbDisplay.GraphicsScreenWidth + radP + x];
            *p = colour;
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = colour;
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (y - radP >= 0) && (y - radP < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(y - radP) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = colour;
        }
        if ((x - radM >= 0) && (x - radM < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + x - radM];
            *p = colour;
        }
        if ((radM + x >= 0) && (radM + x < lbDisplay.GraphicsWindowWidth) &&
          (radP + y >= 0) && (radP + y < lbDisplay.GraphicsWindowHeight)) {
            p = &lbDisplay.GraphicsWindowPtr[(radP + y) * lbDisplay.GraphicsScreenWidth + radM + x];
            *p = colour;
        }
    }
  }
  return Lb_SUCCESS;
}

TbResult LbDrawCircleFilled(long x, long y, ulong radius, TbPixel colour)
{
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
