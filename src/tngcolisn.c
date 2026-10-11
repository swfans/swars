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
#include "drawtext_wrp.h"
#include "thing.h"
/******************************************************************************/

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
#if 0
    asm volatile (
      "call ASM_draw_engine_unk3_last\n"
        : : "a" (x), "d" (z));
#endif
    int x_beg, x_end;
    int z_beg, z_end;
    int cx, cz;

    x_beg = (x >> 8) - (render_area_a >> 1);
    x_end = (x >> 8) + (render_area_a >> 1);
    z_beg = (z >> 8) - (render_area_b >> 1);
    z_end = (z >> 8) + (render_area_b >> 1);
    for (cx = x_beg; cx < x_end; cx++)
    {
        for (cz = z_beg; cz < z_end; cz++)
        {
          struct MyMapElement *p_mapel;
          short vl;

          if (cx < 0 || cx > 127 || cz < 0 || cz > 127)
              continue;
          p_mapel = &game_my_big_map[cx + (cz << 7)];

          vl = p_mapel->ColHead;
          while ( vl )
          {
              struct ColVectList *p_cvlist;
              short cor_x, cor_y;
              uint mapel;
              TbPixel col;

              mapel = (p_mapel - game_my_big_map);
              cor_y = (mapel / 128) << 8;
              cor_x = (mapel % 128) << 8;
              col = 0x63u;
              draw_line_transformed_at_ground(cor_x, cor_y, cor_x + 256, cor_y, col);
              draw_line_transformed_at_ground(cor_x + 256, cor_y, cor_x + 256, cor_y + 256, col);
              draw_line_transformed_at_ground(cor_x, cor_y + 256, cor_x + 256, cor_y + 256, col);
              draw_line_transformed_at_ground(cor_x, cor_y + 256, cor_x, cor_y, col);

              p_cvlist = &game_col_vects_list[vl];
              draw_collision_vector_line(p_cvlist->Vect);
              vl = p_cvlist->NextColList & 0x7FFF;
              cor_x += 128;
              cor_y += 128;

              p_cvlist = &game_col_vects_list[vl];
              draw_number_transformed_at_ground(cor_x, cor_y, p_cvlist->Object);
            }
        }
    }
}

ushort dynamic_insert_vect(s32 x1, s32 y1, s32 z1, s32 x2,
  s32 y2, s32 z2, int owner, ubyte vtype)
{
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
}

/******************************************************************************/
