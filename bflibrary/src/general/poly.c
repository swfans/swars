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

int polyscans_UNUSED;

ubyte *poly_screen_UNUSED;

long vec_window_width_UNUSED;
long vec_window_height_UNUSED;

long vec_screen_width_UNUSED;

ubyte *vec_screen_UNUSED;

ubyte *vec_map_UNUSED;

ubyte *vec_pal_UNUSED;

ubyte *dither_map_UNUSED;
ubyte *dither_end_UNUSED;

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

int vec_mode_UNUSED;

int vec_colour_UNUSED;

int trig_UNUSED()
{
// code at 0001:000dead0
}

int mgt_jt_UNUSED()
{
// code at 0001:000e156a
}

int add_to_edi_UNUSED()
{
// code at 0001:000e1123
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
