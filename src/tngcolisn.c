/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file tngcolisn.c
 *     Thing collisions support.
 * @par Purpose:
 *     Defines structures and routibes for detecting and handling collisions.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     04 Sep 2024 - 11 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "tngcolisn.h"

#include <stdio.h>
#include "bfline.h"

#include "engincolour.h"
#include "enginzoom.h"
#include "huddrwlstm.h"

#include "bigmap.h"
#include "bfmath.h"
#include "drawtext_wrp.h"
#include "thing.h"
/******************************************************************************/

extern short link__MapColListEmptyHead;
extern short link__MapColVectEmptyHead;

struct ColVect *game_col_vects = NULL;
ushort next_col_vect = 1;

struct ColVectList *game_col_vects_list = NULL;
ushort next_vects_list = 1;

ubyte debug_hud_collision = 0;

/******************************************************************************/

void set_dome_col(struct Thing *p_building, ubyte flag)
{
    struct ColVectList *p_cvlist;
    ushort vl_beg, vl_end;
    ushort  vl;

    // The thing data contains properties with range of ColVectList
    // which can switch the passability
    vl_beg = p_building->U.UObject.BuildStartVect;
    vl_end = vl_beg + p_building->U.UObject.BuildNumbVect;
    if (flag)
    {
        for (vl = vl_beg; vl < vl_end; vl++)
        {
            p_cvlist = &game_col_vects_list[vl];
            p_cvlist->NextColList |= 0x8000;
        }
    }
    else
    {
        for (vl = vl_beg; vl < vl_end; vl++)
        {
            p_cvlist = &game_col_vects_list[vl];
            p_cvlist->NextColList &= ~0x8000;
        }
    }
}

void draw_mark_over_tile_with_number(short tl_x, short tl_z, sbyte ln, int num)
{
    int x0, z0, x1, z1, xc, zc;
    TbPixel lncol;

    lncol = 0x63;
    x0 = TILE_TO_MAPCOORD(tl_x, 0);
    z0 = TILE_TO_MAPCOORD(tl_z, 0);
    x1 = TILE_TO_MAPCOORD(tl_x + 1, 0);
    z1 = TILE_TO_MAPCOORD(tl_z + 1, 0);
    if (x1 >= MAP_COORD_WIDTH)
        x1 = MAP_COORD_WIDTH - 1;
    if (z1 >= MAP_COORD_HEIGHT)
        z1 = MAP_COORD_HEIGHT - 1;

    draw_mapcoord_line_at_ground(x0, z0, x1, z0, lncol);
    draw_mapcoord_line_at_ground(x1, z0, x1, z1, lncol);
    draw_mapcoord_line_at_ground(x0, z1, x1, z1, lncol);
    draw_mapcoord_line_at_ground(x0, z1, x0, z0, lncol);

    xc = (x0 + x1) / 2;
    zc = (z0 + z1) / 2;
    draw_mapcoord_simple_text_number_at_ground(xc, zc, ln, num);
}

void draw_collision_vector_line(ushort colvect)
{
    struct ColVect *p_colvect;
    TbPixel lncol, txcol;

    p_colvect = &game_col_vects[colvect];
    lncol = colour_lookup[ColLU_WHITE];
    txcol = colour_lookup[ColLU_PINK];

    enlist_hud_draw_mapcoord_line_annotated_num(
      p_colvect->X1, p_colvect->Y1, p_colvect->Z1,
      p_colvect->X2, p_colvect->Y2, p_colvect->Z2,
      colvect, 0, 1, lncol, txcol);
}

void draw_engine_unk3_last(short x, short z)
{
    int tl_beg_x, tl_end_x;
    int tl_beg_z, tl_end_z;
    int cx, cz;

    tl_beg_x = MAPCOORD_TO_TILE(x) - (render_area_a >> 1);
    tl_end_x = MAPCOORD_TO_TILE(x) + (render_area_a >> 1);
    tl_beg_z = MAPCOORD_TO_TILE(z) - (render_area_b >> 1);
    tl_end_z = MAPCOORD_TO_TILE(z) + (render_area_b >> 1);

    if (tl_beg_x < 0)
        tl_beg_x = 0;
    if (tl_beg_z < 0)
        tl_beg_z = 0;
    if (tl_end_x > MAP_TILE_WIDTH)
        tl_end_x = MAP_TILE_WIDTH;
    if (tl_end_z > MAP_TILE_HEIGHT)
        tl_end_z = MAP_TILE_HEIGHT;

    for (cx = tl_beg_x; cx < tl_end_x; cx++)
    {
        for (cz = tl_beg_z; cz < tl_end_z; cz++)
        {
            struct MyMapElement *p_mapel;
            short vl;

            p_mapel = &game_my_big_map[cz * MAP_TILE_WIDTH + cx];

            vl = p_mapel->ColHead;
            while (vl != 0)
            {
                struct ColVectList *p_cvlist;
                short tl_x, tl_z;
                uint mapel;

                mapel = (p_mapel - game_my_big_map);
                tl_z = (mapel / MAP_TILE_WIDTH);
                tl_x = (mapel % MAP_TILE_WIDTH);

                p_cvlist = &game_col_vects_list[vl];
                vl = p_cvlist->NextColList & 0x7FFF;

                draw_collision_vector_line(p_cvlist->Vect);
                draw_mark_over_tile_with_number(tl_x, tl_z, 0, p_cvlist->Object);
            }
        }
    }
}

/** Links given collision vector into the ColVectList chain of the map
 * element located at given precise coordinates.
 *
 * Does nothing if the map element already has that vector linked, or if
 * there is no free ColVectList entry to use.
 */
static void dynamic_add_collision_to_single_mapwho(short x, short z,
  short owner, ushort vect_idx)
{
    struct MyMapElement *p_mapel;
    struct ColVectList *p_cvlist;
    ushort new_idx;

    p_mapel = &game_my_big_map[MAP_TILE_WIDTH * MAPCOORD_TO_TILE(z)
      + MAPCOORD_TO_TILE(x)];

    if (game_col_vects_list[p_mapel->ColHead].Vect == vect_idx)
        return;

    new_idx = (ushort)link__MapColListEmptyHead;
    if (new_idx == 0)
        return;

    p_cvlist = &game_col_vects_list[new_idx];
    link__MapColListEmptyHead = p_cvlist->NextColList;
    p_cvlist->Vect = vect_idx;
    p_cvlist->NextColList = p_mapel->ColHead;
    p_cvlist->Object = owner;
    p_mapel->ColHead = new_idx;
}

ushort dynamic_insert_vect(s32 x1, s32 y1, s32 z1, s32 x2,
  s32 y2, s32 z2, short owner, ubyte vtype)
{
#if 0
    // Pushed through a register holding them: a "g" operand may be placed
    // relative to the stack pointer, which each push moves.
    int stkargs[4];
    short ret;

    stkargs[0] = (int)(intptr_t)y2;
    stkargs[1] = (int)(intptr_t)z2;
    stkargs[2] = (int)(intptr_t)owner;
    stkargs[3] = (int)(intptr_t)vtype;

    asm volatile (
      "push 12(%5)\n"
      "push 8(%5)\n"
      "push 4(%5)\n"
      "push 0(%5)\n"
      "call ASM_dynamic_insert_vect\n"
        : "=a" (ret) : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "S" (stkargs)
        : "cc", "memory");
    return ret;
#endif
    struct ColVect *p_colvect;
    ushort vect_idx;
    int dx, dz;
    s32 length;
    s32 step_x, step_z;
    s32 swp_x, swp_z;
    int step, sweep_steps;

    vect_idx = link__MapColVectEmptyHead;
    if (vect_idx == 0)
        return 0;
    p_colvect = &game_col_vects[vect_idx];
    // The freelist link is kept in the Face field while a ColVect is free;
    // it gets overwritten with the owner once the vector becomes used.
    link__MapColVectEmptyHead = p_colvect->Face;

    if ((x1 < 0) || (x1 > MAP_COORD_WIDTH))
        return 0;
    if ((x2 < 0) || (x2 > MAP_COORD_WIDTH))
        return 0;
    if ((z1 < 0) || (z1 > MAP_COORD_HEIGHT))
        return 0;
    if ((z2 < 0) || (z2 > MAP_COORD_HEIGHT))
        return 0;

    p_colvect->X1 = x1;
    p_colvect->Y1 = y1;
    p_colvect->Z1 = z1;
    p_colvect->X2 = x2;
    p_colvect->Y2 = y2;
    p_colvect->Z2 = z2;
    p_colvect->Face = owner;

    dx = x2 - x1;
    dz = z2 - z1;
    length = LbSqrL(dx * dx + dz * dz);

    sweep_steps = length >> 5;
    if (sweep_steps == 0)
        sweep_steps = 1;

    step_x = (dx << 10) / sweep_steps;
    step_z = (dz << 10) / sweep_steps;

    swp_x = x1 << 10;
    swp_z = z1 << 10;

    // The segment is sampled at `sweep_steps`+1 points (both endpoints included).
    for (step = sweep_steps; step >= 0; step--)
    {
        dynamic_add_collision_to_single_mapwho(swp_x >> 10, swp_z >> 10,
          owner, vect_idx);

        swp_x += step_x;
        swp_z += step_z;
    }

    return vect_idx;
}

/******************************************************************************/
