/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file drawtext.c
 *     Math transformations required by the 3D engine.
 * @par Purpose:
 *     Implement functions for transforming positions in 3D world.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "drawtext.h"

#include "bfscreen.h"
#include "rom.h"

#include "bigmap.h"
#include "display.h"
#include "engintrns.h"
#include "swlog.h"
/******************************************************************************/

void draw_text(short x, short y, const char *text, ubyte colour)
{
    long scrn_w, scrn_h;
    ubyte *scr;

    if (x < 0 || y < 0)
        return;
    scrn_w = lbDisplay.GraphicsScreenWidth;
    scrn_h = lbDisplay.GraphicsScreenHeight;
    if (x > scrn_w - 1)
        return;
    if ( y > scrn_h - 1)
        return;

    scr = &lbDisplay.WScreen[x + scrn_w * y];
    prop_text(text, scr, scrn_w, colour);
}

void draw_text_transformed(int coord_x, int coord_y, int coord_z, const char *text)
{
#if 0
    asm volatile (
      "call ASM_draw_text_transformed\n"
        : : "a" (coord_x), "d" (coord_y), "b" (coord_z), "c" (text));
    return;
#endif
    struct EnginePoint ep;
    short w, h;

    w = lbDisplay.GraphicsScreenWidth;
    h = lbDisplay.GraphicsScreenHeight;
    ep.X3d = coord_x - engn_xc;
    ep.Z3d = coord_z - engn_zc;
    ep.Y3d = coord_y - engn_yc;
    ep.Flags = 0;
    transform_point(&ep);
    if ((ep.pp.X > 0) && (ep.pp.Y > 0) && (ep.pp.X < w) && (ep.pp.Y < h))
    {
        draw_text(ep.pp.X, ep.pp.Y, text, colour_lookup[ColLU_GREEN]);
    }
}

void draw_number_transformed(int coord_x, int coord_y, int coord_z, int num)
{
    char locstr[52];
    struct EnginePoint ep;
    short w, h;

    w = lbDisplay.GraphicsScreenWidth;
    h = lbDisplay.GraphicsScreenHeight;
    ep.X3d = coord_x - engn_xc;
    ep.Z3d = coord_z - engn_zc;
    ep.Y3d = coord_y - engn_yc;
    ep.Flags = 0;
    transform_point(&ep);
    if ((ep.pp.X > 0) && (ep.pp.Y > 0) && (ep.pp.X < w) && (ep.pp.Y < h))
    {
        sprintf(locstr, "%d", num);
        draw_text(ep.pp.X, ep.pp.Y, locstr, colour_lookup[ColLU_GREEN]);
    }
}

void draw_line_transformed_at_ground(int x1, int y1, int x2, int y2, TbPixel colour)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_line_transformed_at_ground\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (colour));
}

void draw_text_transformed_at_ground(int coord_x, int coord_z, const char *text)
{
#if 0
    asm volatile (
      "call ASM_draw_text_transformed_at_ground\n"
        : : "a" (coord_x), "d" (coord_z), "b" (text));
    return;
#endif
    struct EnginePoint ep;
    short w, h;

    w = lbDisplay.GraphicsScreenWidth;
    h = lbDisplay.GraphicsScreenHeight;
    ep.X3d = coord_x - engn_xc;
    ep.Z3d = coord_z - engn_zc;
    ep.Y3d = PRCCOORD_TO_YCOORD(alt_at_point(coord_x, coord_z)) - engn_yc;
    ep.Flags = 0;
    transform_point(&ep);
    if ((ep.pp.X > 0) && (ep.pp.Y > 0) && (ep.pp.X < w) && (ep.pp.Y < h))
    {
        draw_text(ep.pp.X, ep.pp.Y, text, colour_lookup[ColLU_RED]);
    }
}

void draw_number_transformed_at_ground(int coord_x, int coord_z, int num)
{
    char locstr[52];
    struct EnginePoint ep;
    short w, h;

    w = lbDisplay.GraphicsScreenWidth;
    h = lbDisplay.GraphicsScreenHeight;
    ep.X3d = coord_x - engn_xc;
    ep.Z3d = coord_z - engn_zc;
    ep.Y3d = PRCCOORD_TO_YCOORD(alt_at_point(coord_x, coord_z)) - engn_yc;
    ep.Flags = 0;
    transform_point(&ep);
    if ((ep.pp.X > 0) && (ep.pp.Y > 0) && (ep.pp.X < w) && (ep.pp.Y < h))
    {
        sprintf(locstr, "%d", num);
        draw_text(ep.pp.X, ep.pp.Y, locstr, colour_lookup[ColLU_RED]);
    }
}

/******************************************************************************/
