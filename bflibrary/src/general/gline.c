/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gline.c
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
#include <stdlib.h>
#include "bfline.h"
#include "bfscreen.h"

static void draw_b_line(long x1, long y1, long x2, long y2, TbPixel colour)
{
  long apx = 2 * abs(x2 - x1);
  long spx;
  if (x2 - x1 <= 0)
    spx = -1;
  else
    spx = 1;
  long apy = 2 * abs(y2 - y1);
  long spy;
  if (y2 - y1 <= 0 )
    spy = -1;
  else
    spy = 1;
  long doffy = spy * lbDisplay.GraphicsScreenWidth;
  long offset = lbDisplay.GraphicsScreenWidth * y1 + x1;
  long x = x1;
  long y = y1;
  if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4)
  {
    if (apx <= apy)
    {
      long d = apx - (apy>>1);
      while ( true )
      {
        unsigned short glass_idx = lbDisplay.GraphicsWindowPtr[offset]
                + ((unsigned char)colour<<8);
        lbDisplay.GraphicsWindowPtr[offset] = lbDisplay.GlassMap[glass_idx];
        if (y==y2) break;
        if (d>=0)
        {
          offset += spx;
          d -= apy;
        }
        y += spy;
        offset += doffy;
        d += apx;
      }
    } else
    {
      long d = apy - (apx >> 1);
      while ( true )
      {
        unsigned short glass_idx = lbDisplay.GraphicsWindowPtr[offset]
                + ((unsigned char)colour<<8);
        lbDisplay.GraphicsWindowPtr[offset] = lbDisplay.GlassMap[glass_idx];
        if (x==x2) break;
        if (d>=0)
        {
          offset += doffy;
          d -= apx;
        }
        x += spx;
        offset += spx;
        d += apy;
      }
    }
  } else
  if (lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8)
  {
      if ( apx <= apy )
      {
        long d = apx - (apy >> 1);
        while ( 1 )
        {
          unsigned short glass_idx = (lbDisplay.GraphicsWindowPtr[offset]<<8)
                + ((unsigned char)colour);
          lbDisplay.GraphicsWindowPtr[offset] = lbDisplay.GlassMap[glass_idx];
          if (y == y2) break;
          if (d >= 0)
          {
            offset += spx;
            d -= apy;
          }
          y += spy;
          offset += doffy;
          d += apx;
        }
      } else
      {
        long d = apy - (apx >> 1);
        while ( 1 )
        {
          unsigned short glass_idx = (lbDisplay.GraphicsWindowPtr[offset]<<8)
                + ((unsigned char)colour);
          lbDisplay.GraphicsWindowPtr[offset] = lbDisplay.GlassMap[glass_idx];
          if (x == x2) break;
          if (d >= 0)
          {
            offset += doffy;
            d -= apx;
          }
          x += spx;
          offset += spx;
          d += apy;
        }
      }
  } else
  {
      if ( apx <= apy )
      {
        long d = apx - (apy >> 1);
        while ( true )
        {
          lbDisplay.GraphicsWindowPtr[offset] = ((unsigned char)colour);
          if (y==y2) break;
          if ( d >= 0 )
          {
            offset += spx;
            d -= apy;
          }
          y += spy;
          offset += doffy;
          d += apx;
        }
      }
      else
      {
        long d = apy - (apx >> 1);
        while ( 1 )
        {
          lbDisplay.GraphicsWindowPtr[offset] = ((unsigned char)colour);
          if ( x == x2 )
            break;
          if ( d >= 0 )
          {
            offset += doffy;
            d -= apx;
          }
          x += spx;
          offset += spx;
          d += apy;
        }
      }
  }
}

TbResult LbDrawLine(long X1, long Y1, long X2, long Y2, TbPixel colour)
{
    TbResult result = Lb_OK;
    // Adjusting X-dimension coordinates
    long width_max = lbDisplay.GraphicsWindowWidth - 1;
    if ( X1 >= 0 )
    {
        if ( X1 <= width_max )
        {
          if ( X2 >= 0 )
          {
            if ( X2 > width_max )
            {
              Y2 -= (X2-width_max) * (Y2-Y1) / (X2-X1);
              X2 = width_max;
              result = Lb_SUCCESS;
            }
          } else
          {
            Y2 -= X2 * (Y2-Y1) / (X2-X1);
            X2 = 0;
            result = Lb_SUCCESS;
          }
        } else
        {
          if ( X2 > width_max ) return 1;
          Y1 -= (X1-width_max) * (Y1-Y2) / (X1-X2);
          X1 = width_max;
          result = Lb_SUCCESS;
          if ( X2 < 0 )
          {
            Y2 -= X2 * (Y2-Y1) / (X2-X1);
            X2 = 0;
          }
        }
    }
    else
    {
        if ( X2 < 0 ) return 1;
        Y1 -= X1 * (Y1 - Y2) / (X1 - X2);
        X1 = 0;
        result = Lb_SUCCESS;
        if ( X2 > width_max )
        {
          Y2 -= (X2 - width_max) * (Y2 - Y1) / (X2 - X1);
          X2 = lbDisplay.GraphicsWindowWidth - 1;
        }
    }
    // Adjusting Y-dimension coordinates
    long height_max = lbDisplay.GraphicsWindowHeight - 1;
    if ( Y1 < 0 )
    {
        if ( Y2 < 0 ) return 1;
        X1 -= Y1 * (X1 - X2) / (Y1 - Y2);
        Y1 = 0;
        result = Lb_SUCCESS;
        if ( Y2 > height_max )
        {
          X2 -= (Y2 - height_max) * (X2 - X1) / (Y2 - Y1);
          Y2 = height_max;
        }
    } else
    if ( Y1 <= height_max )
    {
        if ( Y2 >= 0 )
        {
          if ( Y2 > height_max )
          {
            X2 -= (Y2 - height_max) * (X2 - X1) / (Y2 - Y1);
            Y2 = height_max;
            result = Lb_SUCCESS;
          }
        } else
        {
          X2 -= Y2 * (X2 - X1) / (Y2 - Y1);
          Y2 = 0;
          result = Lb_SUCCESS;
        }
    } else
    {
        if ( Y2 > height_max )
          return 1;
        X1 -= (Y1 - height_max) * (X1 - X2) / (Y1 - Y2);
        Y1 = height_max;
        result = Lb_SUCCESS;
        if ( Y2 < 0 )
        {
          X2 -= Y2 * (X2 - X1) / (Y2 - Y1);
          Y2 = 0;
        }
    }
    draw_b_line(X1, Y1, X2, Y2, colour);
    return result;
}

/******************************************************************************/
