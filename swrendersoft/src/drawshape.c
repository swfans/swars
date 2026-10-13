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

#include "drawtext.h"
#include "engincam.h"
#include "engincolour.h"
#include "engintrns.h"
#include "privrdlog.h"
/******************************************************************************/

void draw_mapcoord_line_col(int cor1_x, int cor1_y_m8, int cor1_z,
  int cor2_x, int cor2_y_m8, int cor2_z, TbPixel colour)
{
    struct EnginePoint ep1, ep2;

    ep1.X3d = cor1_x - engn_xc;
    ep1.Y3d = cor1_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
    ep1.Z3d = cor1_z - engn_zc;
    ep1.Flags = 0;
    transform_point(&ep1);

    ep2.X3d = cor2_x - engn_xc;
    ep2.Y3d = cor2_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
    ep2.Z3d = cor2_z - engn_zc;
    ep2.Flags = 0;
    transform_point(&ep2);

    if ((ep1.pp.X > 0) && (ep1.pp.X < lbDisplay.GraphicsScreenWidth)
      && (ep1.pp.Y > 0) && (ep1.pp.Y < lbDisplay.GraphicsScreenHeight)
      && (ep2.pp.X > 0) && (ep2.pp.X < lbDisplay.GraphicsScreenWidth)
      && (ep2.pp.Y > 0) && (ep2.pp.Y < lbDisplay.GraphicsScreenHeight)) {
        LbDrawLine(ep1.pp.X, ep1.pp.Y, ep2.pp.X, ep2.pp.Y, colour);
    }
}

void draw_mapcoord_line_annotated(int cor1_x, int cor1_y_m8, int cor1_z,
  int cor2_x, int cor2_y_m8, int cor2_z, const char *annot_text, TbPixel lncol, TbPixel txcol)
{
    struct EnginePoint ep1, ep2;
    short scr_x, scr_y;

    ep1.X3d = cor1_x - engn_xc;
    ep1.Y3d = cor1_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
    ep1.Z3d = cor1_z - engn_zc;
    ep1.Flags = 0;
    transform_point(&ep1);

    ep2.X3d = cor2_x - engn_xc;
    ep2.Y3d = cor2_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
    ep2.Z3d = cor2_z - engn_zc;
    ep2.Flags = 0;
    transform_point(&ep2);

    LbDrawLine(ep1.pp.X, ep1.pp.Y, ep2.pp.X, ep2.pp.Y, lncol);
    scr_x = (ep2.pp.X + ep1.pp.X) / 2;
    scr_y = (ep2.pp.Y + ep1.pp.Y) / 2;
    draw_simple_text(scr_x, scr_y, annot_text, txcol);
}

void draw_mapcoord_line_annotated_num(int cor1_x, int cor1_y_m8, int cor1_z,
  int cor2_x, int cor2_y_m8, int cor2_z, int annot_num, TbPixel lncol, TbPixel txcol)
{
    char locstr[16];

    sprintf(locstr, "%d", annot_num);
    draw_mapcoord_line_annotated(cor1_x, cor1_y_m8, cor1_z,
      cor2_x, cor2_y_m8, cor2_z, locstr, lncol, txcol);
}

void draw_mapcoord_to_scrcoord_line(short scr_x, short scr_y,
  int cor_x, int cor_y_m8, int cor_z, TbPixel colour)
{
    struct EnginePoint ep;

    ep.X3d = cor_x - engn_xc;
    ep.Y3d = cor_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
    ep.Z3d = cor_z - engn_zc;
    ep.Flags = 0;
    transform_point(&ep);

    LbDrawLine(scr_x, scr_y, ep.pp.X, ep.pp.Y, colour);
}

void draw_bevel_rect(int scr_x, int scr_y,
  int width, int height, TbPixel colour, TbPixel bvcolour)
{
    lbDisplay.DrawFlags &= ~Lb_SPRITE_OUTLINE;
    LbDrawBox(scr_x, scr_y, width, height, colour);
    LbDrawLine(scr_x, scr_y, scr_x + width - 2, scr_y, bvcolour);
    LbDrawLine(scr_x, scr_y, scr_x, scr_y + height - 2, bvcolour);
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

void draw_mapcoord_flat_circle_outline(short cor_x, short cor_y_m8, short cor_z,
  short radius, TbPixel colour)
{
    struct EnginePoint ep1;
    struct EnginePoint ep2;
    int cir_cor_x, cir_cor_z;
    int cir_nxt_x, cir_nxt_z;
    ushort slice;

    cir_cor_x = cor_x;
    cir_cor_z = cor_z + radius;
    for (slice = 1; slice < 0x40; slice++)
    {
        short cir_dt_x, cir_dt_z;
        cir_dt_x = (radius * lbSinTable[slice * 2 * LbFPMath_PI / 0x40]) >> 16;
        cir_dt_z = (radius * lbSinTable[LbFPMath_PI/2 + slice * 2*LbFPMath_PI / 0x40]) >> 16;
        cir_nxt_x = cor_x + cir_dt_x;
        cir_nxt_z = cor_z + cir_dt_z;

        ep1.X3d = cir_cor_x - engn_xc;
        ep1.Y3d = cor_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
        ep1.Z3d = cir_cor_z - engn_zc;
        ep1.Flags = 0;
        transform_point(&ep1);
        ep2.X3d = cir_nxt_x - engn_xc;
        ep2.Y3d = cor_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
        ep2.Z3d = cir_nxt_z - engn_zc;
        ep2.Flags = 0;
        transform_point(&ep2);
        LbDrawLine(ep1.pp.X, ep1.pp.Y, ep2.pp.X, ep2.pp.Y, colour);

        cir_cor_x = cir_nxt_x;
        cir_cor_z = cir_nxt_z;
    }
    {
        cir_nxt_x = cor_x;
        cir_nxt_z = cor_z + radius;

        ep1.X3d = cir_cor_x - engn_xc;
        ep1.Y3d = cor_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
        ep1.Z3d = cir_cor_z - engn_zc;
        ep1.Flags = 0;
        transform_point(&ep1);
        ep2.X3d = cir_nxt_x - engn_xc;
        ep2.Y3d = cor_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
        ep2.Z3d = cir_nxt_z - engn_zc;
        ep2.Flags = 0;
        transform_point(&ep2);
        LbDrawLine(ep1.pp.X, ep1.pp.Y, ep2.pp.X, ep2.pp.Y, colour);
    }
}

/******************************************************************************/
