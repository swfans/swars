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
#include "swlog.h"
/******************************************************************************/

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

    scr_x1 = scr_dx + dword_176D3C;
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

    if ((flags_B & flags_A & 0xF) == 0
      && (next_sort_line < mem_game[33].N))
    {
        struct SortLine *p_sline;

        p_sline = p_current_sort_line;
        p_current_sort_line++;
        p_sline->Shade = 32;
        p_sline->Flags = 0;
        p_sline->X1 = scr_x1;
        p_sline->Y1 = scr_y1;
        p_sline->X2 = scr_x2;
        p_sline->Y2 = scr_y2;
        p_sline->Col = col;

        draw_item_add(DrIT_Unkn11, next_sort_line, bckt_shift + 5000);
        next_sort_line++;
    }
}

void draw_e_graphic(int x, int y, int z, ushort frame, int radius, int intensity, struct Thing *p_thing)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_e_graphic\n"
        : : "a" (x), "d" (y), "b" (z), "c" (frame), "g" (radius), "g" (intensity), "g" (p_thing));
}

void draw_e_graphic_scale(int x, int y, int z, ushort frame, int radius, int intensity, int scale)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_e_graphic_scale\n"
        : : "a" (x), "d" (y), "b" (z), "c" (frame), "g" (radius), "g" (intensity), "g" (scale));
}

void draw_pers_e_graphic(struct Thing *p_thing, int x, int y, int z, int frame, int radius, int intensity)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_pers_e_graphic\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z), "g" (frame), "g" (radius), "g" (intensity));
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
