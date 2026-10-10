/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file drawshape.c
 *     Drawing simple 2D shapes within the 3D world.
 * @par Purpose:
 *     Implement functions for drawing lines and simple shapes, with their
 *     position transformed from in-mission map coordinated.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 20 Jan 2026
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "drawshape.h"

#include "bfbox.h"
#include "bfline.h"
#include "bfscreen.h"

#include "engincam.h"
#include "engincolour.h"
#include "engintrns.h"
#include "privrdlog.h"
/******************************************************************************/

void draw_line_transformed_col(int x1, int y1, int z1, int x2, int y2, int z2, TbPixel colour)
{
    struct EnginePoint ep1, ep2;

    ep1.X3d = x1 - engn_xc;
    ep1.Y3d = y1 - engn_yc;
    ep1.Z3d = z1 - engn_zc;
    ep1.Flags = 0;
    transform_point(&ep1);

    ep2.X3d = x2 - engn_xc;
    ep2.Y3d = y2 - engn_yc;
    ep2.Z3d = z2 - engn_zc;
    ep2.Flags = 0;
    transform_point(&ep2);

    if ((ep1.pp.X > 0) && (ep1.pp.X < lbDisplay.GraphicsScreenWidth)
      && (ep1.pp.Y > 0) && (ep1.pp.Y < lbDisplay.GraphicsScreenHeight)
      && (ep2.pp.X > 0) && (ep2.pp.X < lbDisplay.GraphicsScreenWidth)
      && (ep2.pp.Y > 0) && (ep2.pp.Y < lbDisplay.GraphicsScreenHeight)) {
        LbDrawLine(ep1.pp.X, ep1.pp.Y, ep2.pp.X, ep2.pp.Y, colour);
    }
}

void draw_purple_rect(int x, int y, int w, int h, ubyte active)
{
    TbPixel col1, col2;

    lbDisplay.DrawFlags &= ~Lb_SPRITE_OUTLINE;
    if (active) {
        col1 = 0x0E;
        col2 = 0x0C;
    } else {
        col1 = 0x10;
        col2 = 0x0E;
    }
    LbDrawBox(x, y, w, h, col1);
    LbDrawLine(x, y, x + w - 2, y, col2);
    LbDrawLine(x, y, x, y + h - 2, col2);
}

void draw_corner_rect(int scr_x, int scr_y,
  int width, int height, int lnlen, int lnthick, TbPixel colour)
{
    short i;

    for (i = 0; i < lnthick; i++)
    {
        short s;

        s = i - (lnthick >> 1);
        LbDrawLine(scr_x - width, scr_y - height + s,
          scr_x - width + lnlen, scr_y - height + s, colour);
        LbDrawLine(scr_x + width - lnlen, scr_y - height + s,
          scr_x + width, scr_y - height + s, colour);
        LbDrawLine(scr_x + width + s, scr_y - height,
          scr_x + width + s, scr_y - height + lnlen, colour);
        LbDrawLine(scr_x + width + s, scr_y + height - lnlen,
          scr_x + width + s, scr_y + height, colour);
        LbDrawLine(scr_x + width, scr_y + height + s,
          scr_x + width - lnlen, scr_y + height + s, colour);
        LbDrawLine(scr_x - width, scr_y + height + s,
          scr_x - width + lnlen, scr_y + height + s, colour);
        LbDrawLine(scr_x - width + s, scr_y + height,
          scr_x - width + s, scr_y + height - lnlen, colour);
        LbDrawLine(scr_x - width + s, scr_y - height,
          scr_x - width + s, scr_y - height + lnlen, colour);
    }
}

/******************************************************************************/
