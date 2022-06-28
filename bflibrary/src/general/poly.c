/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly.c
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
#include "poly.h"
#include "poly_trigp.h"
#include "poly_trigr.h"
#include "bfgentab.h"
#include "bfpalette.h"

ubyte *poly_screen;

ubyte *vec_screen;

ubyte *vec_map;
ubyte *vec_pal;

long vec_screen_width;

long vec_window_width;
long vec_window_height;

ubyte *dither_map;
ubyte *dither_end;

struct PolyPoint polyscans[POLY_SCANS_COUNT];

TbPixel vec_colour = 112;
ubyte vec_mode;

const long add_to_edi[] = {
  0,-15,-14,-13,-12,-11,-10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
};

int ft_jt_UNUSED()
{
// code at 0001:000e02f1
}

int fb_jt_UNUSED()
{
// code at 0001:000dfe72
}

int gt_jt_UNUSED()
{
// code at 0001:000e1260
}

int ll_jt_UNUSED()
{
// code at 0001:000dec37
}

int rl_jt_UNUSED()
{
// code at 0001:000df544
}

int poly_line_UNUSED()
{
// code at 0001:000e5adf
}

void trig_UNUSED(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c)
{
}

int mgt_jt_UNUSED()
{
// code at 0001:000e156a
}

void setup_vecs(ubyte *screenbuf, ubyte *nvec_map,
  ulong line_len, ulong width, ulong height)
{
    if (line_len > 0)
        vec_screen_width = line_len;
    if (screenbuf != NULL)
    {
        vec_screen = screenbuf;
        poly_screen = screenbuf - vec_screen_width;
    }
    if (nvec_map != NULL)
    {
        vec_map = nvec_map;
        dither_map = nvec_map;
        dither_end = nvec_map + 16;
    }
    if (height > 0)
        vec_window_height = height;
    if (width > 0)
        vec_window_width = width;
}

int t12_jt_UNUSED()
{
// code at 0001:000e54f0
}

int render_table_UNUSED()
{
// code at 0001:000e06e3
}


/******************************************************************************/
