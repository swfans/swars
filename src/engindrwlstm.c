/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engindrwlstm.c
 *     Making drawlists for the 3D engine.
 * @par Purpose:
 *     Implements functions for filling drawlists.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 12 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "engindrwlstm.h"

#include "bfendian.h"
#include "bfmath.h"

#include "display.h"
#include "enginbckt.h"
#include "engindrwlstx.h"
#include "enginshrapn.h"
#include "engintrns.h"
#include "game.h"
#include "game_data.h"
#include "game_speed.h"
#include "swlog.h"
#include "thing.h"
/******************************************************************************/
extern ubyte byte_176D49;

extern long dword_176CAC;
extern long dword_152E4C;

ubyte byte_152EF0[] = {
   0, 10,  5, 10,  7,  7,  8, 10,
  10, 10,  5,  7,  5,  7,  7,  0,
};

/** Add a new draw item and return linked SortLine instance.
 *
 * @param ditype Draw item type, should be one of SortLine related types.
 * @param bckt Destination bucket for this draw item.
 * @return SortLine instance to fill, or NULL if arrays exceeded.
 */
struct SortLine *draw_item_add_line(ubyte ditype, ushort bckt)
{
    struct SortLine *p_sline;

    if (next_sort_line >= mem_game[33].N)
        return NULL;

    p_sline = p_current_sort_line;
    if (!draw_item_add(ditype, next_sort_line, bckt))
        return NULL;

    p_current_sort_line++;
    next_sort_line++;

    return p_sline;
}

/** Add a new draw item and return linked SortSprite instance.
 *
 * @param ditype Draw item type, should be one of SortSprite related types.
 * @param bckt Destination bucket for this draw item.
 * @return SortSprite instance to fill, or NULL if arrays exceeded.
 */
struct SortSprite *draw_item_add_sprite(ubyte ditype, ushort bckt)
{
    struct SortSprite *p_sspr;

    if (next_sort_sprite >= mem_game[32].N)
        return NULL;

    p_sspr = p_current_sort_sprite;
    if (!draw_item_add(ditype, next_sort_sprite, bckt))
        return NULL;

    p_current_sort_sprite++;
    next_sort_sprite++;

    return p_sspr;
}

/** Add a new draw item and return first of linked SpecialPoint instances.
 *
 * @param ditype Draw item type, should be one of SpecialPoint related types.
 * @param bckt Destination bucket for this draw item.
 * @param npoints Amount of consecutive points to reserve.
 * @return SpecialPoint instance to fill, or NULL if arrays exceeded.
 */
struct SpecialPoint *draw_item_add_points(ubyte ditype, ushort offset, ushort bckt, ushort npoints)
{
    struct SpecialPoint *p_scrpoint;

    if (next_screen_point + npoints > mem_game[30].N)
        return NULL;

    p_scrpoint = &game_screen_point_pool[next_screen_point];
    if (!draw_item_add(ditype, offset, bckt))
        return NULL;

    next_screen_point += npoints;

    return p_scrpoint;
}

void draw_mapwho_vect_len(int x1, int y1, int z1, int x2, int y2, int z2, int len, int col)
{
#if 0
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_mapwho_vect_len\n"
        : : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "g" (y2), "g" (z2), "g" (len), "g" (col));
    return;
#endif
    struct ShEnginePoint sp1, sp2;
    struct SortLine *p_sline;
    int dt_x, dt_y, dt_z;
    int dist;
    int end_x, end_y, end_z;
    int scr_dx, scr_dy;
    int fctr_y, fctr_xz, pers5_range;
    int abs_scr_dy, abs_scr_dx;

    dt_y = y2 - y1;
    dt_z = z2 - z1;
    dt_x = x2 - x1;

    dist = LbSqrL(dt_z * dt_z + dt_x * dt_x + dt_y * dt_y);
    if (dist == 0)
        dist = 1;
    end_y = y1 + dt_y * len / dist;
    end_z = z1 + dt_z * len / dist;
    end_x = x1 + dt_x * len / dist;

#if 0
    transform_shpoint(&sp1, x1, 8 * y1 - 8 * engn_yc, z1);
    transform_shpoint(&sp2, end_x, 8 * end_y - 8 * engn_yc, end_z);
#else
    fctr_y = 8 * y1 - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x1 + dword_176D14 * z1) >> 16;
    abs_scr_dx = (dword_176D14 * x1 - dword_176D10 * z1) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    pers5_range = (dword_176D18 * fctr_y + dword_176D1C * fctr_xz) >> 16;

    sp1.Flags = 0;
    sp2.Flags = 0;
    sp1.Depth = pers5_range;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    sp1.X = dword_176D3C + scr_dx;
    if (sp1.X < 0) {
        if (sp1.X < -2000)
            sp1.X = -2000;
        sp1.Flags |= 0x01;
    } else if (sp1.X >= vec_window_width) {
        if (sp1.X > 2000)
            sp1.X = 2000;
        sp1.Flags |= 0x02;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    sp1.Y = dword_176D40 - scr_dy;
    if (sp1.Y < 0) {
        if (sp1.Y < -2000)
            sp1.Y = -2000;
        sp1.Flags |= 0x04;
    } else if (sp1.Y >= vec_window_height) {
        if (sp1.Y > 2000)
            sp1.Y = 2000;
        sp1.Flags |= 0x08;
    }

    sp1.Flags |= 0x40;

    fctr_y = 8 * end_y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * end_x + dword_176D14 * end_z) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * end_x - dword_176D10 * end_z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    sp2.X = dword_176D3C + scr_dx;
    if (sp2.X < 0) {
        if (sp2.X < -2000)
            sp2.X = -2000;
        sp2.Flags |= 0x01;
    } else if (sp2.X >= vec_window_width) {
        if (sp2.X > 2000)
            sp2.X = 2000;
        sp2.Flags |= 0x02;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = ((0x4000 - pers5_range) * scr_dy) >> 14;

    sp2.Y = dword_176D40 - scr_dy;
    if (sp2.Y < 0) {
        if (sp2.Y < -2000)
            sp2.Y = -2000;
        sp2.Flags |= 0x04;
    } else if (sp2.Y >= vec_window_height) {
        if (sp2.Y > 2000)
            sp2.Y = 2000;
        sp2.Flags |= 0x08;
    }

    sp2.Flags |= 0x40;
#endif

    if ((sp2.Flags & sp1.Flags & 0xF) != 0)
        return;

    p_sline = draw_item_add_line(DrIT_Unkn11, sp1.Depth + 5000);
    if (p_sline == NULL)
        return;

    p_sline->Shade = 32;
    p_sline->Flags = 0;
    p_sline->X1 = sp1.X;
    p_sline->Y1 = sp1.Y;
    p_sline->X2 = sp2.X;
    p_sline->Y2 = sp2.Y;
    p_sline->Col = col;
}

void draw_e_graphic(int x, int y, int z, ushort frame, int radius, int intensity, struct Thing *p_thing)
{
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_e_graphic\n"
        : : "a" (x), "d" (y), "b" (z), "c" (frame), "g" (radius), "g" (intensity), "g" (p_thing));
    return;
#endif
    struct ShEnginePoint sp;
    struct SortSprite *p_sspr;
    int scr_z;
    int scr_dx, scr_dy;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    sp.Depth = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - sp.Depth) * scr_dx) >> 14;

    sp.X = dword_176D3C + scr_dx;
    if (sp.X < 0) {
        if (sp.X < -2000)
            sp.X = -2000;
    } else if (sp.X >= vec_window_width) {
        if (sp.X > 2000)
            sp.X = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - sp.Depth)) >> 14;

    sp.Y = dword_176D40 - scr_dy;
    if (sp.Y < 0) {
        if (sp.Y < -2000)
            sp.Y = -2000;
    } else if (sp.Y >= vec_window_height) {
        if (sp.Y > 2000)
            sp.Y = 2000;
    }

    scr_z = sp.Depth - radius;
    if ((ingame.DisplayMode != 50) && ((p_thing->Flag2 & 0x20000000) != 0))
        scr_z = -10000;

    p_sspr = draw_item_add_sprite(DrIT_Unkn3, scr_z + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = sp.X;
    p_sspr->Y = sp.Y;
    p_sspr->Z = scr_z;
    p_sspr->Frame = frame;
    p_sspr->Brightness = intensity;
    p_sspr->Scale = 256;
    p_sspr->PThing = p_thing;
}

void draw_e_graphic_scale(int x, int y, int z, ushort frame, int radius, int intensity, int scale)
{
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_e_graphic_scale\n"
        : : "a" (x), "d" (y), "b" (z), "c" (frame), "g" (radius), "g" (intensity), "g" (scale));
    return;
#endif
    struct ShEnginePoint sp;
    struct SortSprite *p_sspr;
    int scr_z;
    int scr_dx, scr_dy;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    sp.Depth = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - fctr_xz * dword_176D18) >> 16;
    abs_scr_dx = (x * dword_176D14 - z * dword_176D10) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = (scr_dx * (0x4000 - sp.Depth)) >> 14;

    sp.X = scr_dx + dword_176D3C;
    if (sp.X < 0) {
        if (sp.X < -2000)
            sp.X = -2000;
    } else if (sp.X >= vec_window_width) {
        if (sp.X > 2000)
            sp.X = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - sp.Depth)) >> 14;

    sp.Y = dword_176D40 - scr_dy;
    if (sp.Y < 0) {
        if (sp.Y < -2000)
            sp.Y = -2000;
    } else if (sp.Y >= vec_window_height) {
        if (sp.Y > 2000)
            sp.Y = 2000;
    }
    scr_z = sp.Depth - radius - 100;

    p_sspr = draw_item_add_sprite(DrIT_Unkn15, scr_z + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = sp.X;
    p_sspr->Y = sp.Y;
    p_sspr->Z = scr_z;
    p_sspr->Frame = frame;
    p_sspr->Brightness = intensity;
    p_sspr->Scale = scale;
    p_sspr->PThing = NULL;
}

void draw_pers_e_graphic(struct Thing *p_thing, int x, int y, int z, int frame, int radius, int intensity)
{
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_pers_e_graphic\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z), "g" (frame), "g" (radius), "g" (intensity));
    return;
#endif
    struct ShEnginePoint sp;
    struct SortSprite *p_sspr;
    int scr_dx, scr_dy;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;
    int bckt_shift;
    ubyte bri;

    bri = byte_152EF0[p_thing->SubType] + intensity;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    sp.Depth = (dword_176D18 * fctr_y + dword_176D1C * fctr_xz) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = (scr_dx * (0x4000 - sp.Depth)) >> 14;

    sp.X = dword_176D3C + scr_dx;
    if (sp.X < 0) {
        if (sp.X < -2000)
            sp.X = -2000;
    } else if (sp.X >= vec_window_width) {
        if (sp.X > 2000)
            sp.X = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - sp.Depth)) >> 14;

    sp.Y = dword_176D40 - scr_dy;
    if (sp.Y < 0) {
        if (sp.Y < -2000)
            sp.Y = -2000;
    } else if (sp.Y >= vec_window_height) {
        if (sp.Y > 2000)
            sp.Y = 2000;
    }

    bckt_shift = sp.Depth - radius;
    if (ingame.DisplayMode == 50)
    {
        if ((p_thing->Flag2 & 0x20000000) != 0) {
            if ((p_thing->Flag & 0x02) != 0)
                return;
            bckt_shift -= 1000000;
        }
    }
    else
    {
        if ((p_thing->Flag2 & 0x20000000) != 0)
            bckt_shift = -10000;
    }

    p_sspr = draw_item_add_sprite(DrIT_Unkn13, bckt_shift + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = sp.X;
    p_sspr->Y = sp.Y;
    p_sspr->Z = 0;
    p_sspr->Frame = frame;
    p_sspr->PThing = p_thing;
    p_sspr->Brightness = bri;
    p_sspr->Angle = (p_thing->U.UObject.Angle + 8 - byte_176D49) & 7;

    if ((p_thing->Flag2 & 0x20000000) != 0 || p_thing->U.UPerson.OnFace || (p_thing->SubType == SubTT_PERS_MECH_SPIDER))
        return;

    p_sspr = draw_item_add_sprite(DrIT_Unkn19, bckt_shift + 5000 - 200);
    if (p_sspr == NULL)
        return;

    p_sspr->X = sp.X;
    p_sspr->Y = sp.Y;
    p_sspr->PThing = p_thing;
}

void FIRE_draw_fire(struct SimpleThing *p_sthing)
{
#if 0
    asm volatile ("call ASM_FIRE_draw_fire\n"
        : : "a" (p_sthing));
    return;
#endif
    struct FireFlame *p_flame;
    ushort flm;

    for (flm = p_sthing->U.UFire.flame; flm; flm = p_flame->next)
    {
        struct ShEnginePoint sp;
        struct SpecialPoint *p_scrpoint;
        int x, y, z;
        int scr_dx, scr_dy;
        int abs_scr_dy, abs_scr_dx;
        int fctr_xz, fctr_y;

        p_flame = &FIRE_flame[flm];
        x = p_flame->x - engn_xc;
        y = p_flame->y - engn_yc;
        z = p_flame->z - engn_zc;

        if (current_map == 9) // map009 Singapore on-water map
            y += waft_table[gameturn & 0x1F];

        fctr_y = y - 8 * engn_yc;
        fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
        sp.Depth = (dword_176D18 * fctr_y + dword_176D1C * fctr_xz) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
        abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;
        sp.Flags = 0;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (scr_dx * (0x4000 - sp.Depth)) >> 14;

        sp.X = dword_176D3C + scr_dx;
        if (sp.X < 0) {
            if (sp.X < -2000)
                sp.X = -2000;
            sp.Flags |= 0x01;
        } else if (sp.X >= vec_window_width) {
            if (sp.X > 2000)
                sp.X = 2000;
            sp.Flags |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - sp.Depth)) >> 14;

        sp.Y = dword_176D40 - scr_dy;
        if (sp.Y < 0) {
            if (sp.Y < -2000)
                sp.Y = -2000;
            sp.Flags |= 0x04;
        } else if (sp.Y >= vec_window_height) {
            if (sp.Y > 2000)
                sp.Y = 2000;
            sp.Flags |= 0x08;
        }

        sp.Flags |= 0x40;

        p_flame->PointOffset = next_screen_point;
        p_scrpoint = draw_item_add_points(DrIT_Unkn25, flm, sp.Depth + 5000 - 50, 1);
        if (p_scrpoint == NULL)
            break;

        p_scrpoint->X = sp.X;
        p_scrpoint->Y = sp.Y;
        p_scrpoint->Z = sp.Depth;
    }
}

void draw_bang_phwoar(struct SimpleThing *p_pow)
{
    struct Phwoar *p_phwoar;
    ushort phw;

    for (phw = p_pow->U.UBang.phwoar; phw != 0; phw = p_phwoar->child)
    {
        struct ShEnginePoint sp;
        struct SpecialPoint *p_scrpoint;
        int x, y, z;
        int scr_dx, scr_dy;
        int abs_scr_dy, abs_scr_dx;
        int fctr_xz;

        p_phwoar = &phwoar[phw];
        x = (p_phwoar->x >> 8) - engn_xc;
        z = (p_phwoar->z >> 8) - engn_zc;
        y = (p_phwoar->y >> 5) - engn_yc - 8 * engn_yc;

        fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
        sp.Depth = (dword_176D18 * y + dword_176D1C * fctr_xz) >> 16;
        abs_scr_dy = (dword_176D1C * y - dword_176D18 * fctr_xz) >> 16;
        abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;
        sp.Flags = 0;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = ((0x4000 - sp.Depth) * scr_dx) >> 14;

        sp.X = dword_176D3C + scr_dx;
        if (sp.X < 0) {
            if (sp.X < -2000)
                sp.X = -2000;
            sp.Flags |= 0x01;
        } else if (sp.X >= vec_window_width) {
            if (sp.X > 2000)
                sp.X = 2000;
            sp.Flags |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - sp.Depth)) >> 14;

        sp.Y = dword_176D40 - scr_dy;
        if (sp.Y < 0) {
            if (sp.Y < -2000)
                sp.Y = -2000;
            sp.Flags |= 0x04;
        } else if (sp.Y >= vec_window_height) {
            if (sp.Y > 2000)
                sp.Y = 2000;
            sp.Flags |= 0x08;
        }

        sp.Flags |= 0x40;

        p_phwoar->PointOffset = next_screen_point;
        p_scrpoint = draw_item_add_points(DrIT_Unkn21, phw, sp.Depth + 5000 - 100, 1);
        if (p_scrpoint == NULL)
            break;

        p_scrpoint->X = sp.X;
        p_scrpoint->Y = sp.Y;
        p_scrpoint->Z = sp.Depth;
    }
}

void draw_bang_shrapnel(struct SimpleThing *p_pow)
{
    struct Shrapnel *p_shrapnel;
    ushort shrap;

    for (shrap = p_pow->U.UBang.shrapnel; shrap != 0; shrap = p_shrapnel->child)
    {
        struct ShEnginePoint sp1, sp2, sp3;
        struct SpecialPoint *p_scrpoint;
        int x, y, z;
        int scr_dx, scr_dy;
        int abs_scr_dy, abs_scr_dx;
        int fctr_xz, fctr_y;
        int x_pcc, x_pcs, y_msc, y_mss, x_mmc, y_pps;
        int z_ps, z_ms;
        ushort bckt_shift;

        p_shrapnel = &shrapnel[shrap];
        if ((p_shrapnel->type < 1) || (p_shrapnel->type > 3))
            continue;

        {
            int cos_yaw, cos_pitch, sin_yaw, sin_pitch, tmp;
            int sh_cc, sh_cs, sh_sc, sh_ss, sh_z;
            short shrap_yaw, shrap_pitch;

            shrap_yaw = 8 * p_shrapnel->yaw;
            shrap_pitch = 8 * p_shrapnel->pitch;
            cos_yaw = lbSinTable[512 + shrap_yaw];
            cos_pitch = lbSinTable[512 + shrap_pitch];
            sin_pitch = lbSinTable[shrap_pitch];
            sin_yaw = lbSinTable[shrap_yaw];

            tmp = (cos_pitch * cos_yaw) & 0xFFFF0000;
            tmp |= ((u64)(cos_pitch * (s64)cos_yaw) >> 32) & 0xFFFF;
            sh_cc = (int)bw_rotl32(tmp, 16) >> 10;

            tmp = (cos_pitch * sin_yaw) & 0xFFFF0000;
            tmp |= ((u64)(cos_pitch * (s64)sin_yaw) >> 32) & 0xFFFF;
            sh_cs = (int)bw_rotl32(tmp, 16) >> 10;

            tmp = (sin_pitch * cos_yaw) & 0xFFFF0000;
            tmp |= ((u64)(sin_pitch * (s64)cos_yaw) >> 32) & 0xFFFF;
            sh_sc = (int)bw_rotl32(tmp, 16) >> 10;

            tmp = (sin_pitch * sin_yaw) & 0xFFFF0000;
            tmp |= ((u64)(sin_pitch * (s64)sin_yaw) >> 32) & 0xFFFF;
            sh_ss = (int)bw_rotl32(tmp, 16) >> 10;

            sh_z = sin_yaw >> 10;

            x = (p_shrapnel->x >> 8) - engn_xc;
            y = (p_shrapnel->y >> 5) - engn_yc;
            z = (p_shrapnel->z >> 8) - engn_zc;

            x_pcc = x + sh_cc;
            y_msc = y - sh_sc;
            x_pcs = x + sh_cs;
            y_mss = y - sh_ss;
            y_pps = y + sh_sc + sh_ss;
            x_mmc = x - sh_cc - sh_cs;
            z_ps = z + sh_z;
            z_ms = z - sh_z;
        }

        sp3.Flags = 0;
        sp2.Flags = 0;
        sp1.Flags = 0;

        fctr_y = y_msc - 8 * engn_yc;
        fctr_xz = (dword_176D10 * x_pcc + dword_176D14 * z_ms) >> 16;
        sp1.Depth = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
        abs_scr_dx = (dword_176D14 * x_pcc - dword_176D10 * z_ms) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (scr_dx * (0x4000 - sp1.Depth)) >> 14;

        sp1.X = dword_176D3C + scr_dx;
        if (sp1.X < 0) {
            if (sp1.X < -2000)
                sp1.X = -2000;
            sp1.Flags |= 0x01;
        } else if (sp1.X >= vec_window_width) {
            if (sp1.X > 2000)
                sp1.X = 2000;
            sp1.Flags |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - sp1.Depth)) >> 14;

        sp1.Y = dword_176D40 - scr_dy;
        if (sp1.Y < 0) {
            if (sp1.Y < -2000)
                sp1.Y = -2000;
            sp1.Flags |= 0x04;
        } else if (sp1.Y >= vec_window_height) {
            if (sp1.Y > 2000)
                sp1.Y = 2000;
            sp1.Flags |= 0x08;
        }

        sp1.Flags |= 0x40;

        fctr_y = y_mss - 8 * engn_yc;
        fctr_xz = (dword_176D14 * z + dword_176D10 * x_pcs) >> 16;
        sp2.Depth = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
        abs_scr_dx = (dword_176D14 * x_pcs - dword_176D10 * z) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (scr_dx * (0x4000 - sp2.Depth)) >> 14;

        sp2.X = dword_176D3C + scr_dx;
        if (sp2.X < 0) {
            if (sp2.X < -2000)
                sp2.X = -2000;
            sp2.Flags = 0x01;
        } else if (sp2.X >= vec_window_width) {
            if (sp2.X > 2000)
                sp2.X = 2000;
            sp2.Flags = 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - sp2.Depth)) >> 14;

        sp2.Y = dword_176D40 - scr_dy;
        if (sp2.Y < 0) {
            if (sp2.Y < -2000)
                sp2.Y = -2000;
            sp2.Flags |= 0x04;
        } else if (sp2.Y >= vec_window_height) {
            if (sp2.Y > 2000)
                sp2.Y = 2000;
            sp2.Flags |= 0x08;
        }

        sp2.Flags |= 0x40;

        fctr_y = y_pps - 8 * engn_yc;
        fctr_xz = (dword_176D14 * z_ps + dword_176D10 * x_mmc) >> 16;
        sp3.Depth = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
        abs_scr_dx = (dword_176D14 * x_mmc - dword_176D10 * z_ps) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = ((0x4000 - sp3.Depth) * scr_dx) >> 14;

        sp3.X = dword_176D3C + scr_dx;
        if (sp3.X < 0) {
            if (sp3.X < -2000)
                sp3.X = -2000;
            sp3.Flags |= 0x01;
        } else if (sp3.X >= vec_window_width) {
            if (sp3.X > 2000)
                sp3.X = 2000;
            sp3.Flags |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - sp3.Depth)) >> 14;

        sp3.Y = dword_176D40 - scr_dy;
        if (sp3.Y < 0) {
            if (sp3.Y < -2000)
                sp3.Y = -2000;
            sp3.Flags |= 0x04;
        } else if (sp3.Y >= vec_window_height) {
            if (sp3.Y > 2000)
                sp3.Y = 2000;
            sp3.Flags |= 0x08;
        }

        sp3.Flags |= 0x40;

        if (((sp2.Flags & sp1.Flags & sp3.Flags) & 0xF) != 0)
            continue;

        bckt_shift = sp2.Depth;
        if (bckt_shift >= sp3.Depth)
            bckt_shift = sp3.Depth;
        if (bckt_shift > sp1.Depth)
            bckt_shift = sp1.Depth;

        p_shrapnel->PointOffset = next_screen_point;
        p_scrpoint = draw_item_add_points(DrIT_Unkn20, shrap, bckt_shift + 5000, 3);
        if (p_scrpoint == NULL)
            break;

        p_scrpoint->X = sp1.X;
        p_scrpoint->Y = sp1.Y;
        p_scrpoint->Z = sp1.Depth;
        p_scrpoint++;

        p_scrpoint->X = sp2.X;
        p_scrpoint->Y = sp2.Y;
        p_scrpoint->Z = sp2.Depth;
        p_scrpoint++;

        p_scrpoint->X = sp3.X;
        p_scrpoint->Y = sp3.Y;
        p_scrpoint->Z = sp3.Depth;
    }
}

void build_wobble_line(int x1, int y1, int z1,
 int x2, int y2, int z2, struct SimpleThing *p_sthing, int itime)
{
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_build_wobble_line\n"
        : : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "g" (y2), "g" (z2), "g" (p_sthing), "g" (itime));
}

ushort shrapnel_get_child_type_not_3(struct Shrapnel *p_shraparnt)
{
    struct Shrapnel *p_shrapnel;
    ushort shrap;

    for (shrap = p_shraparnt->child; shrap != 0; shrap = p_shrapnel->child)
    {
        p_shrapnel = &shrapnel[shrap];

        if (p_shrapnel->type != 3)
            return shrap;
    }
    return 0;
}

void draw_bang_wobble_line(struct SimpleThing *p_pow)
{
#if 0
    asm volatile ("call ASM_draw_bang_wobble_line\n"
        : : "a" (p_pow));
    return;
#endif
    struct Shrapnel *p_shrapnel1;
    struct Shrapnel *p_shrapnel2;
    int x, y, z;
    int fctr_xz, fctr_y;
    int scr_dx, scr_dy;
    int abs_scr_dy, abs_scr_dx;
    ushort shrap1, shrap2;

    if (dword_176CAC == 0)
        return;

    if ((dword_152E4C & 0xFF) <= 208)
        return;

    dword_176CAC--;

    shrap1 = p_pow->U.UBang.shrapnel;
    if (shrap1 == 0)
        return;
    p_shrapnel1 = &shrapnel[shrap1];

    shrap2 = shrapnel_get_child_type_not_3(p_shrapnel1);
    if (shrap2 == 0)
        return;
    p_shrapnel2 = &shrapnel[shrap2];

    {
        struct ShEnginePoint sp1, sp2;

        x = (p_shrapnel1->x >> 8) - engn_xc;
        z = (p_shrapnel1->z >> 8) - engn_zc;
        y = (p_shrapnel1->y >> 5) - engn_yc;

        fctr_y = y - 8 * engn_yc;
        fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
        sp1.Depth = (dword_176D18 * fctr_y + dword_176D1C * fctr_xz) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - fctr_xz * dword_176D18) >> 16;
        abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (scr_dx * (0x4000 - sp1.Depth)) >> 14;

        sp1.X = dword_176D3C + scr_dx;
        if (sp1.X < 0) {
            if (sp1.X < -2000)
                sp1.X = -2000;
        } else if (sp1.X >= vec_window_width) {
            if (sp1.X > 2000)
                sp1.X = 2000;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - sp1.Depth)) >> 14;

        sp1.Y = dword_176D40 - scr_dy;
        if (sp1.Y < 0) {
            if (sp1.Y < -2000)
                sp1.Y = -2000;
        } else if (sp1.Y >= vec_window_height) {
            if (sp1.Y > 2000)
                sp1.Y = 2000;
        }

        x = (p_shrapnel2->x >> 8) - engn_xc;
        z = (p_shrapnel2->z >> 8) - engn_zc;
        y = (p_shrapnel2->y >> 5) - engn_yc;

        fctr_y = y - 8 * engn_yc;
        fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
        sp2.Depth = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - fctr_xz * dword_176D18) >> 16;
        abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (scr_dx * (0x4000 - sp2.Depth)) >> 14;

        sp2.X = dword_176D3C + scr_dx;
        if (sp2.X < 0) {
            if (sp2.X < -2000)
                sp2.X = -2000;
        } else if (sp2.X >= vec_window_width) {
            if (sp2.X > 2000)
                sp2.X = 2000;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - sp2.Depth)) >> 14;

        sp2.Y = dword_176D40 - scr_dy;
        if (sp2.Y < 0) {
            if (sp2.Y < -2000)
                sp2.Y = -2000;
        } else if (sp2.Y >= vec_window_height) {
            if (sp2.Y > 2000)
                sp2.Y = 2000;
        }

        build_wobble_line(sp1.X, sp1.Y, sp1.Depth, sp2.X, sp2.Y, sp2.Depth, 0, 10);
    }
}

void build_laser(int x1, int y1, int z1, int x2, int y2, int z2, int itime, struct Thing *p_owner, int colour)
{
    asm volatile (
      "push %8\n"
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_build_laser\n"
        : : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "g" (y2), "g" (z2), "g" (itime), "g" (p_owner), "g" (colour));
}

void draw_bang(struct SimpleThing *p_pow)
{
#if 0
    asm volatile ("call ASM_draw_bang\n"
        : : "a" (p_pow));
    return;
#endif
    if (p_pow->State != 0)
    {
        short st;
        TbPixel col;
        st = p_pow->State;
        if (st < 100) {
            col = colour_lookup[3];
        } else {
            st = -10;
            col = colour_lookup[7];
        }
        build_laser(p_pow->X >> 8, p_pow->Y >> 8, p_pow->Z >> 8,
          (p_pow->X >> 8), (p_pow->Y >> 8) + 400, p_pow->Z >> 8, st, 0, col);
    }

    dword_152E4C = bw_rotr32(dword_152E4C, 7) + 0x16365267;
    {
        short tmp;
        tmp = (dword_152E4C & 0xFF);
        if (tmp > 240)
            dword_176CAC = tmp - 240;
    }
    dword_152E4C = bw_rotr32(dword_152E4C, 7) + 0x16365267;

    draw_bang_wobble_line(p_pow);
    draw_bang_shrapnel(p_pow);
    draw_bang_phwoar(p_pow);
}

ushort draw_rot_object(int offset_x, int offset_y, int offset_z, struct SingleObject *point_object, struct Thing *p_thing)
{
    ushort ret;
    asm volatile (
      "push %5\n"
      "call ASM_draw_rot_object\n"
        : "=r" (ret) : "a" (offset_x), "d" (offset_y), "b" (offset_z), "c" (point_object), "g" (p_thing));
    return ret;
}

ushort draw_rot_object2(int offset_x, int offset_y, int offset_z, struct SingleObject *point_object, struct Thing *p_thing)
{
    ushort ret;
    asm volatile (
      "push %5\n"
      "call ASM_draw_rot_object2\n"
        : "=r" (ret) : "a" (offset_x), "d" (offset_y), "b" (offset_z), "c" (point_object), "g" (p_thing));
    return ret;
}

ushort draw_object(int x, int y, int z, struct SingleObject *point_object)
{
    ushort ret;
    asm volatile ("call ASM_draw_object\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (z), "c" (point_object));
    return ret;
}

void draw_vehicle_health(struct Thing *p_thing)
{
    asm volatile ("call ASM_draw_vehicle_health\n"
        : : "a" (p_thing));
}

void draw_vehicle_shadow(ushort veh, ushort sort)
{
    asm volatile ("call ASM_draw_vehicle_shadow\n"
        : : "a" (veh), "d" (sort));
}

void draw_explode(void)
{
    asm volatile ("call ASM_draw_explode\n"
        :  :  : "eax" );
}

void build_polygon_circle(int x1, int y1, int z1, int r1, int r2, int flag, struct SingleFloorTexture *p_tex, int col, int bright1, int bright2)
{
    asm volatile (
      "push %9\n"
      "push %8\n"
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_build_polygon_circle\n"
        : : "a" (x1), "d" (y1), "b" (z1), "c" (r1), "g" (r2), "g" (flag), "g" (p_tex), "g" (col), "g" (bright1), "g" (bright2));
}

/******************************************************************************/
