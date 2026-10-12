/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file drawtext.c
 *     Drawing simple text within the 3D world.
 * @par Purpose:
 *     Implement functions for drawing text, with its position transformed fro
 *     in-mission map coordinated. Draws simple text, to be used for debug
 *     or in-game editors; for proper playtime text, use bitmap fonts instead.
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

#include "engincam.h"
#include "engincolour.h"
#include "engintrns.h"
#include "privrdlog.h"
/******************************************************************************/

short simple_text_width(const char *text)
{
    return prop_text_width(text);
}

short simple_text_char_height(char c)
{
    return 5;
}

void draw_simple_text(short x, short y, const char *text, ubyte colour)
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

void draw_mapcoord_simple_text(int cor_x, int cor_y_m8, int cor_z, const char *text, TbPixel col)
{
    struct EnginePoint ep;
    short w, h;

    w = lbDisplay.GraphicsScreenWidth;
    h = lbDisplay.GraphicsScreenHeight;
    ep.X3d = cor_x - engn_xc;
    ep.Z3d = cor_z - engn_zc;
    ep.Y3d = cor_y_m8; // subtracted within transform_point(): - 8 * engn_yc;
    ep.Flags = 0;
    transform_point(&ep);

    if ((ep.pp.X > 0) && (ep.pp.Y > 0) && (ep.pp.X < w) && (ep.pp.Y < h))
    {
        draw_simple_text(ep.pp.X, ep.pp.Y, text, col);
    }
}

void draw_text_transformed(int cor_x, int cor_y_m8, int cor_z, const char *text)
{
    draw_mapcoord_simple_text(cor_x, cor_y_m8, cor_z, text, colour_lookup[ColLU_GREEN]);
}

void draw_number_transformed(int cor_x, int cor_y_m8, int cor_z, int num)
{
    char locstr[52];

    sprintf(locstr, "%d", num);
    draw_mapcoord_simple_text(cor_x, cor_y_m8, cor_z, locstr, colour_lookup[ColLU_GREEN]);
}

/******************************************************************************/
