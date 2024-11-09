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

#include "bfmath.h"

#include "display.h"
#include "enginbckt.h"
#include "engindrwlstx.h"
#include "engintrns.h"
#include "game.h"
#include "game_data.h"
#include "game_speed.h"
#include "swlog.h"
#include "thing.h"
/******************************************************************************/
extern ubyte byte_176D49;

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
    struct SortLine *p_sline;
    int dt_x, dt_y, dt_z;
    int dist;
    int end_x, end_y, end_z;
    int scr_dx, scr_dy;
    short scr_x1, scr_y1, scr_x2, scr_y2;
    int fctr_y, fctr_xz, pers5_range;
    int abs_scr_dy, abs_scr_dx;
    int bckt_shift;
    ubyte flags_A, flags_B;

    dt_y = y2 - y1;
    dt_z = z2 - z1;
    dt_x = x2 - x1;

    dist = LbSqrL(dt_z * dt_z + dt_x * dt_x + dt_y * dt_y);
    if (dist == 0)
        dist = 1;
    end_y = y1 + dt_y * len / dist;
    end_z = z1 + dt_z * len / dist;
    end_x = x1 + dt_x * len / dist;

    fctr_y = 8 * y1 - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x1 + dword_176D14 * z1) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x1 - dword_176D10 * z1) >> 16;

    flags_A = 0;
    flags_B = 0;
    bckt_shift = pers5_range;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    scr_x1 = dword_176D3C + scr_dx;
    if (scr_x1 < 0) {
        if (scr_x1 < -2000)
            scr_x1 = -2000;
        flags_A |= 0x01;
    } else if (scr_x1 >= vec_window_width) {
        if (scr_x1 > 2000)
            scr_x1 = 2000;
        flags_A |= 0x02;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y1 = dword_176D40 - scr_dy;
    if (scr_y1 < 0) {
        if (scr_y1 < -2000)
            scr_y1 = -2000;
        flags_A |= 0x04;
    } else if (scr_y1 >= vec_window_height) {
        if (scr_y1 > 2000)
            scr_y1 = 2000;
        flags_A |= 0x08;
    }

    flags_A |= 0x40;

    fctr_y = 8 * end_y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * end_x + dword_176D14 * end_z) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * end_x - dword_176D10 * end_z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    scr_x2 = dword_176D3C + scr_dx;
    if (scr_x2 < 0) {
        if (scr_x2 < -2000)
            scr_x2 = -2000;
        flags_B |= 0x01;
    } else if (scr_x2 >= vec_window_width) {
        if (scr_x2 > 2000)
            scr_x2 = 2000;
        flags_B |= 0x02;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = ((0x4000 - pers5_range) * scr_dy) >> 14;

    scr_y2 = dword_176D40 - scr_dy;
    if (scr_y2 < 0) {
        if (scr_y2 < -2000)
            scr_y2 = -2000;
        flags_B |= 0x04;
    } else if (scr_y2 >= vec_window_height) {
        if (scr_y2 > 2000)
            scr_y2 = 2000;
        flags_B |= 0x08;
    }

    flags_B |= 0x40;

    if ((flags_B & flags_A & 0xF) != 0)
        return;

    p_sline = draw_item_add_line(DrIT_Unkn11, bckt_shift + 5000);
    if (p_sline == NULL)
        return;

    p_sline->Shade = 32;
    p_sline->Flags = 0;
    p_sline->X1 = scr_x1;
    p_sline->Y1 = scr_y1;
    p_sline->X2 = scr_x2;
    p_sline->Y2 = scr_y2;
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
    struct SortSprite *p_sspr;
    int scr_dx, scr_dy;
    short scr_x, scr_y, scr_z;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;
    int pers5_range;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    scr_x = dword_176D3C + scr_dx;
    if (scr_x < 0) {
        if (scr_x < -2000)
            scr_x = -2000;
    } else if (scr_x >= vec_window_width) {
        if (scr_x > 2000)
            scr_x = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y = dword_176D40 - scr_dy;
    if (scr_y < 0) {
        if (scr_y < -2000)
            scr_y = -2000;
    } else if (scr_y >= vec_window_height) {
        if (scr_y > 2000)
            scr_y = 2000;
    }

    scr_z = pers5_range - radius;
    if ((ingame.DisplayMode != 50) && ((p_thing->Flag2 & 0x20000000) != 0))
        scr_z = -10000;

    p_sspr = draw_item_add_sprite(DrIT_Unkn3, scr_z + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
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
    struct SortSprite *p_sspr;
    int scr_dx, scr_dy;
    short scr_x, scr_y, scr_z;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;
    int pers5_range;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - fctr_xz * dword_176D18) >> 16;
    abs_scr_dx = (x * dword_176D14 - z * dword_176D10) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = (scr_dx * (0x4000 - pers5_range)) >> 14;

    scr_x = scr_dx + dword_176D3C;
    if (scr_x < 0) {
        if (scr_x < -2000)
            scr_x = -2000;
    } else if (scr_x >= vec_window_width) {
        if (scr_x > 2000)
            scr_x = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y = dword_176D40 - scr_dy;
    if (scr_y < 0) {
        if (scr_y < -2000)
            scr_y = -2000;
    } else if (scr_y >= vec_window_height) {
        if (scr_y > 2000)
            scr_y = 2000;
    }
    scr_z = pers5_range - (unsigned __int16)radius - 100;

    p_sspr = draw_item_add_sprite(DrIT_Unkn15, scr_z + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
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
    struct SortSprite *p_sspr;
    int scr_dx, scr_dy;
    short scr_x, scr_y;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;
    int pers5_range;
    int bckt_shift;
    ubyte bri;

    bri = byte_152EF0[p_thing->SubType] + intensity;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    pers5_range = (dword_176D18 * fctr_y + dword_176D1C * fctr_xz) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = (scr_dx * (0x4000 - pers5_range)) >> 14;

    scr_x = dword_176D3C + scr_dx;
    if (scr_x < 0) {
        if (scr_x < -2000)
            scr_x = -2000;
    } else if (scr_x >= vec_window_width) {
        if (scr_x > 2000)
            scr_x = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y = dword_176D40 - scr_dy;
    if (scr_y < 0) {
        if (scr_y < -2000)
            scr_y = -2000;
    } else if (scr_y >= vec_window_height) {
        if (scr_y > 2000)
            scr_y = 2000;
    }

    bckt_shift = pers5_range - radius;
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

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
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

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
    p_sspr->PThing = p_thing;
}

void FIRE_draw_fire(struct SimpleThing *p_sthing)
{
    asm volatile ("call ASM_FIRE_draw_fire\n"
        : : "a" (p_sthing));
}

void draw_bang(struct SimpleThing *p_pow)
{
    asm volatile ("call ASM_draw_bang\n"
        : : "a" (p_pow));
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

void func_2e440(void)
{
    asm volatile ("call ASM_func_2e440\n"
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
