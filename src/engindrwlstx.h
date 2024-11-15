/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engindrwlstx.h
 *     Header file for engindrwlstx.c.
 * @par Purpose:
 *     Drawlists execution for the 3D engine.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 12 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ENGINDRWLSTX_H
#define ENGINDRWLSTX_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)

struct DrawItem {
    ubyte Type;
    ushort Offset;
    ushort Child;
};

struct SpecialPoint {
    short X;
    short Y;
    short Z;
    short PadTo8;
};

struct SortSprite {
    short X;
    short Y;
    short Z;
    ushort Frame;
    struct Thing *PThing;
    ubyte Brightness;
    ubyte Angle;
    short Scale;
};

struct SortLine {
    short X1;
    short Y1;
    short X2;
    short Y2;
    ubyte Col;
    ubyte Shade;
    ubyte Flags;
};

struct TbSprite;

#pragma pack()
/******************************************************************************/
extern struct DrawItem *game_draw_list;
extern struct DrawItem *p_current_draw_item;
extern ushort next_draw_item;

extern struct SpecialPoint *game_screen_point_pool;
extern ushort next_screen_point;

extern struct SortSprite *game_sort_sprites;
extern struct SortSprite *p_current_sort_sprite;
extern ushort next_sort_sprite;

extern struct SortLine *game_sort_lines;
extern struct SortLine *p_current_sort_line;
extern ushort next_sort_line;

extern ubyte deep_radar_surface_col;
extern ubyte deep_radar_line_col;

extern ushort next_special_face;
extern ushort next_special_face4;

void draw_unkn1_scaled_alpha_sprite(ushort fr, int scr_x, int scr_y,
  ushort scale, ushort alpha);
void draw_sorted_sprite1a(ushort frm, short x, short y, ubyte csel);

uint cummulate_shade_from_quick_lights(ushort light_first);

void draw_drawitem_1(ushort dihead);
void draw_drawitem_2(ushort dihead);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
