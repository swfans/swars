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
#include <assert.h>
#include "poly.h"

#if defined(HAVE_CONFIG_H)
#  include "bfconfig.h"
#endif

#include "poly_gp.h"
#include "poly_trigp.h"
#include "poly_trigr.h"
#include "bfgentab.h"
#include "bfmemut.h"
#include "bfpalette.h"
#include "privbflog.h"

#if defined(LB_POLY_RENDER_TIME_CHECK)
#  include "bftime.h"
#endif

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

// Static variables used only inside draw_gpoly().
long gpoly_mode;

const long add_to_edi[] = {
  0,-15,-14,-13,-12,-11,-10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
};

void trig(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c)
{
    struct PolyPoint *opt_a;
    struct PolyPoint *opt_b;
    struct PolyPoint *opt_c;
    ubyte start_type;
    struct TrigLocalPrep tlp;
    struct TrigLocalRend tlr;
#if defined(LB_POLY_RENDER_TIME_CHECK)
    TbClockMSec exec_tm;
    TbClockMSec start_tm = LbTimerClock();
#endif

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
#if defined(LB_POLY_RENDER_TIME_CHECK)
    exec_tm = LbTimerClock() - start_tm;
    if (exec_tm > 10) {
       LOGWARN("Pa(%ld,%ld,%ld) Pb(%ld,%ld,%ld) Pc(%ld,%ld,%ld) mode %d exec time %d ms",
        point_a->X, point_a->Y, point_a->S, point_b->X, point_b->Y, point_b->S,
        point_c->X, point_c->Y, point_c->S,(int)vec_mode, (int)exec_tm);
    }
#endif
}

void draw_gpoly(struct PolyPoint *point_a, struct PolyPoint *point_b, struct PolyPoint *point_c)
{
    struct gpoly_state st;
    ubyte start_type;
    TbBool exceeds_window;

    gpoly_mode = vec_mode;

    exceeds_window = (point_c->X | point_b->X | point_a->X) < 0
             || point_a->X > vec_window_width
             || point_b->X > vec_window_width
             || point_c->X > vec_window_width;

    LOGNO("Pa(%ld,%ld,%ld)", point_a->X, point_a->Y, point_a->S);
    LOGNO("Pb(%ld,%ld,%ld)", point_b->X, point_b->Y, point_b->S);
    LOGNO("Pc(%ld,%ld,%ld)", point_c->X, point_c->Y, point_c->S);

    start_type = gpoly_reorder_input_points(&point_a, &point_b, &point_c);
    if (start_type == RendStart_NO)
        return;

    gpoly_init_state(&st, point_a, point_b, point_c);

    switch (gpoly_mode)
    {
    case RendVec_mode02:
    case RendVec_mode03:
    case RendVec_mode12:
    case RendVec_mode13:
    case RendVec_mode18:
    case RendVec_mode19:
    case RendVec_mode22:
    case RendVec_mode23:
        gpoly_sta_md03(&st);
        break;
    case RendVec_mode04:
    case RendVec_mode16:
    case RendVec_mode17:
        gpoly_sta_md04(&st);
        break;
    case RendVec_mode05:
    case RendVec_mode06:
    case RendVec_mode20:
    case RendVec_mode21:
    case RendVec_mode24:
    case RendVec_mode25:
        gpoly_sta_md05(&st);
        break;
    case 27:
    case 29:
    case 31:
    case 32:
    case 33:
    case 34:
    case 37:
    case 38:
        gpoly_sta_md27(&st);
        break;
    case 28:
    case 30:
    case 35:
    case 36:
    case 39:
    case 40:
        gpoly_sta_md28(&st);
        break;
    }

    switch (gpoly_mode)
    {
    case RendVec_mode05:
        if (exceeds_window)
            gpoly_rasterize_shaded_bound(&st);
        else
            gpoly_rasterize_shaded_nobound(&st);
        break;
    case 27:
        if (exceeds_window)
            gpoly_rasterize_noshade_bound(&st);
        else
            gpoly_rasterize_noshade_nobound(&st);
        break;
    }
}

void poly_line(struct PolyPoint *point_a, struct PolyPoint *point_b)
{
    TbPixel *obuf;
    ushort i;
    short x2, y2;
    short x1, y1;
    short chk_y;
    short dt_x;

    LOGNO("Pa(%ld,%ld,%ld)", point_a->X, point_a->Y, point_a->S);
    LOGNO("Pb(%ld,%ld,%ld)", point_b->X, point_b->Y, point_b->S);

    x1 = point_a->X;
    y1 = point_a->Y;
    x2 = point_b->X;
    y2 = point_b->Y;

    if (y1 < 0)
    {
        if (y2 < 0)
            return;
        chk_y = y2;
        x1 += -y1 * (x2 - x1) / (y2 - y1);
        y1 = 0;
    }
    else if (y2 < 0)
    {
        y2 = point_a->Y;
        y1 = point_b->Y;
        x1 = point_b->X;
        x2 = point_a->X;

        if (y2 < 0)
            return;
        chk_y = y2;
        x1 += -y1 * (x2 - x1) / (y2 - y1);
        y1 = 0;
    }
    else if (y1 == y2)
    {
        // Horizonal line optimization
        ushort w;
        short sx1, sx2;

        if (y1 >= vec_window_height)
            return;
        sx1 = x1;
        sx2 = x2;
        if (x2 < x1)
        {
          sx2 = x1;
          sx1 = x2;
        }
        if (sx1 >= 0)
        {
          if (sx1 > vec_window_width - 1)
              return;
        }
        else
        {
          if (sx2 <= 0)
              return;
          sx1 = 0;
        }
        if (sx2 > vec_window_width - 1)
            sx2 = vec_window_width - 1;
        w = sx2 - sx1;
        obuf = vec_screen + vec_screen_width * y1 + sx1;
        LbMemorySet(obuf, vec_colour, w + 1);
        return;
    }
    else if (y1 - y2 < 0)
    {
        if (y1 >= vec_window_height)
            return;
        chk_y = y2;
    }
    else
    {
        if (y2 >= vec_window_height)
            return;
        chk_y = y1;
        y2 = point_a->Y;
        y1 = point_b->Y;
        x1 = point_b->X;
        x2 = point_a->X;
    }

    if (chk_y >= vec_window_height) {
        x2 = x1 + (vec_window_height - y1) * (x2 - x1) / (y2 - y1);
        y2 = vec_window_height - 1;
    }

    if (x1 < 0)
    {
        if (x2 < 0)
            return;
        dt_x = 1;
        y1 += -x1 * (y2 - y1) / (x2 - x1);
        x1 = 0;
        if (x2 >= vec_window_width) {
            y2 = y1 + vec_window_width * (y2 - y1) / x2;
            x2 = vec_window_width - 1;
        }
    }
    else if (x1 >= vec_window_width)
    {
        if (x2 >= vec_window_width)
            return;
        dt_x = -1;
        y1 += (x1 - vec_window_width) * (y2 - y1) / (x1 - x2);
        x1 = vec_window_width - 1;
        if (x2 < 0)
        {
            y2 -= -x2 * (y2 - y1) / (x1 - x2);
            x2 = 0;
        }
    }
    else if (x2 < 0)
    {
        dt_x = -1;
        y2 = y1 + x1 * (y2 - y1) / (x1 - x2);
        x2 = 0;
    }
    else if (x2 >= vec_window_width)
    {
        dt_x = 1;
        y2 = y1 + (vec_window_width - x1) * (y2 - y1) / (x2 - x1);
        x2 = vec_window_width - 1;
    }
    else if (x1 == x2)
    {
        // Vertical line optimization
        ushort h;
        short sy1;

        if (y2 >= y1) {
            sy1 = y1;
            h = y2 - y1;
        } else {
            sy1 = y2;
            h = y1 - y2;
        }
        obuf = vec_screen + vec_screen_width * sy1 + x1;
        for (i = h + 1; i != 0; i--)
        {
            *obuf = vec_colour;
            obuf += vec_screen_width;
        }
        return;
    }
    else if (x1 - x2 < 0)
    {
        dt_x = 1;
    }
    else
    {
        dt_x = -1;
    }

    if (y2 == y1)
    {
        // Horizonal line optimization
        ushort w;
        short sx1;

        if (x2 >= x1) {
            sx1 = x1;
            w = x2 - x1;
        } else {
            sx1 = x2;
            w = x1 - x2;
        }
        obuf = vec_screen + vec_screen_width * y1 + sx1;
        LbMemorySet(obuf, vec_colour, w + 1);
        return;
    }

    {
        ushort dist_x, dist_y;
        short sh_posit, sh_negat, sh_chk;
        int dt_buf_x, dt_buf_y;

        obuf = vec_screen + vec_screen_width * y1 + x1;
        dist_x = dt_x * (x2 - x1);
        dist_y = y2 - y1;
        if (dist_y <= dist_x) {
            dt_buf_x = dt_x;
            dt_buf_y = vec_screen_width;
        } else {
            dist_x = y2 - y1;
            dist_y = dt_x * (x2 - x1);
            dt_buf_x = vec_screen_width;
            dt_buf_y = dt_x;
        }
        sh_negat = 2 * dist_y;
        sh_chk = 2 * dist_y - dist_x;
        sh_posit = 2 * (dist_y - dist_x);
        *obuf = vec_colour;
        for (i = dist_x; i != 0; i--)
        {
            obuf += dt_buf_x;
            if (sh_chk >= 0) {
                obuf += dt_buf_y;
                sh_chk += sh_posit;
            } else {
                sh_chk += sh_negat;
            }
            assert(obuf < vec_screen + vec_screen_width * vec_window_height);
            *obuf = vec_colour;
        }
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
