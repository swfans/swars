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
#include "privbflog.h"

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

int poly_line_UNUSED()
{
// code at 0001:000e5adf
}

void trig(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c)
{
    struct PolyPoint *opt_a;
    struct PolyPoint *opt_b;
    struct PolyPoint *opt_c;
    ubyte start_type;
    struct TrigLocalPrep tlp;
    struct TrigLocalRend tlr;

    LOGNO("Pa(%ld,%ld,%ld)", point_a->X, point_a->Y, point_a->S);
    LOGNO("Pb(%ld,%ld,%ld)", point_b->X, point_b->Y, point_b->S);
    LOGNO("Pc(%ld,%ld,%ld)", point_c->X, point_c->Y, point_c->S);

    opt_a = point_a;
    opt_b = point_b;
    opt_c = point_c;
    start_type = trig_reorder_input_points(&opt_a, &opt_b, &opt_c);

    LOGNO("start type %d",(int)start_type);

    switch (start_type)
    {
    case RendStart_LL:
        if (!trig_ll_start(&tlp, &tlr, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_RL:
        if (!trig_rl_start(&tlp, &tlr, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_FB:
        if (!trig_fb_start(&tlp, &tlr, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_FT:
        if (!trig_ft_start(&tlp, &tlr, opt_a, opt_b, opt_c)) {
            return;
        }
        break;
    case RendStart_NO:
        return;
    }

    LOGNO("render mode %d",(int)vec_mode);

    switch (vec_mode)
    {
    case RendVec_mode00:
        trig_render_md00(&tlr);
        break;

    case RendVec_mode01:
        trig_render_md01(&tlr);
        break;

    case RendVec_mode02:
        trig_render_md02(&tlr);
        break;

    case RendVec_mode03:
        trig_render_md03(&tlr);
        break;

    case RendVec_mode04:
        trig_render_md04(&tlr);
        break;

    case RendVec_mode05:
        trig_render_md05(&tlr);
        break;

    case RendVec_mode06:
        trig_render_md06(&tlr);
        break;

    case RendVec_mode07:
    case RendVec_mode11:
        if (vec_colour == 0x20)
            trig_render_md02(&tlr);
        else
            trig_render_md07(&tlr);
        break;

    case RendVec_mode08:
        trig_render_md08(&tlr);
        break;

    case RendVec_mode09:
        trig_render_md09(&tlr);
        break;

    case RendVec_mode10:
        trig_render_md10(&tlr);
        break;

    case RendVec_mode12:
        trig_render_md12(&tlr);
        break;

    case RendVec_mode13:
        trig_render_md13(&tlr);
        break;

    case RendVec_mode14:
        trig_render_md14(&tlr);
        break;

    case RendVec_mode15:
        trig_render_md15(&tlr);
        break;

    case RendVec_mode16:
        trig_render_md16(&tlr);
        break;

    case RendVec_mode17:
        trig_render_md17(&tlr);
        break;

    case RendVec_mode18:
        trig_render_md18(&tlr);
        break;

    case RendVec_mode19:
        trig_render_md19(&tlr);
        break;

    case RendVec_mode20:
        trig_render_md20(&tlr);
        break;

    case RendVec_mode21:
        trig_render_md21(&tlr);
        break;

    case RendVec_mode22:
        trig_render_md22(&tlr);
        break;

    case RendVec_mode23:
        trig_render_md23(&tlr);
        break;

    case RendVec_mode24:
        trig_render_md24(&tlr);
        break;

    case RendVec_mode25:
        trig_render_md25(&tlr);
        break;

    case RendVec_mode26:
        trig_render_md26(&tlr);
        break;
    }

    LOGNO("end");
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

/******************************************************************************/
