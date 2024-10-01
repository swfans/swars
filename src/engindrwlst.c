/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engindrwlst.c
 *     Drawlists handling for the 3D engine.
 * @par Purpose:
 *     Implements functions for filling and using drawlists.
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
#include "engindrwlst.h"

#include "display.h"
#include "drawtext.h"
#include "enginbckt.h"
#include "enginpeff.h"
#include "enginsngtxtr.h"
#include "enginshadws.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/
extern ushort next_screen_point;
extern ushort next_sort_sprite;
extern ushort next_sort_line;

extern ushort next_special_face;
extern ushort next_special_face4;

extern ushort tnext_screen_point;
extern ushort tnext_draw_item;
extern ushort tnext_sort_sprite;
//extern ushort tnext_sort_line; -- no such var?
//extern ushort tnext_special_face;
extern ushort tnext_special_face4;

extern long dword_176CC4;

void reset_drawlist(void)
{
    tnext_screen_point = next_screen_point;
    next_screen_point = 0;
    next_sort_line = 0;
    tnext_draw_item = next_draw_item;
    next_draw_item = 1;
    next_special_face = 1;
    tnext_sort_sprite = next_sort_sprite;
    next_sort_sprite = 0;
    tnext_special_face4 = next_special_face4;
    next_special_face4 = 1;
    ingame.NextRocket = 0;
    tnext_floor_texture = next_floor_texture;
    dword_176CC4 = 0;
    p_current_sort_line = game_sort_lines;
    p_current_sort_sprite = game_sort_sprites;
    p_current_draw_item = &game_draw_list[1];
}

void draw_object_face1a(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face1a\n"
        : : "a" (a1));
}

void draw_sort_sprite1a(ushort a1)
{
    asm volatile (
      "call ASM_draw_sort_sprite1a\n"
        : : "a" (a1));
}

void draw_floor_tile1a(ushort a1)
{
    asm volatile (
      "call ASM_draw_floor_tile1a\n"
        : : "a" (a1));
}

void draw_ex_face(ushort a1)
{
    asm volatile (
      "call ASM_draw_ex_face\n"
        : : "a" (a1));
}

void draw_floor_tile1b(ushort a1)
{
    asm volatile (
      "call ASM_draw_floor_tile1b\n"
        : : "a" (a1));
}

void draw_object_face1b(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face1b\n"
        : : "a" (a1));
}

void draw_object_face4a(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4a\n"
        : : "a" (a1));
}

void draw_sort_line(struct SortLine *sline)
{
    asm volatile (
      "call ASM_draw_sort_line\n"
        : : "a" (sline));
}

void draw_object_face4b(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4b\n"
        : : "a" (a1));
}

void draw_sort_sprite1b(int a1)
{
    asm volatile (
      "call ASM_draw_sort_sprite1b\n"
        : : "a" (a1));
}

void draw_object_face4c(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4c\n"
        : : "a" (a1));
}

void draw_sort_sprite1c_sub(ushort a1, short a2, short a3, ubyte a4, ushort a5)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_sort_sprite1c_sub\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5));
}

void draw_sort_sprite1c(ushort a1)
{
    struct SortSprite *sspr;
    sspr = &game_sort_sprites[a1];
    draw_sort_sprite1c_sub(sspr->Frame, sspr->X, sspr->Y, sspr->Brightness, sspr->Scale);
}

void draw_object_face1c(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face1c\n"
        : : "a" (a1));
}

void draw_object_face4d(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4d\n"
        : : "a" (a1));
}

void draw_object_face4g(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4g\n"
        : : "a" (a1));
}

void draw_object_face1e(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face1e\n"
        : : "a" (a1));
}

void draw_object_face4f(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4f\n"
        : : "a" (a1));
}

void draw_shrapnel(ushort shrap)
{
    asm volatile (
      "call ASM_draw_shrapnel\n"
        : : "a" (shrap));
}

void draw_phwoar(ushort ph)
{
    asm volatile (
      "call ASM_draw_phwoar\n"
        : : "a" (ph));
}

void draw_sort_sprite_tng(short a1)
{
    asm volatile (
      "call ASM_draw_sort_sprite_tng\n"
        : : "a" (a1));
}

void draw_object_face4e(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4e\n"
        : : "a" (a1));
}

void draw_object_face1d(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face1d\n"
        : : "a" (a1));
}

void draw_ssample_screen_point(ushort a1)
{
    asm volatile (
      "call ASM_draw_ssample_screen_point\n"
        : : "a" (a1));
}

void draw_screen_number(ushort a1)
{
    char locstr[50];
    struct SortSprite *sspr;
    sspr = &game_sort_sprites[a1];
    sprintf(locstr, "%d", (int)sspr->PThing);
    draw_text(2 * sspr->X,2 * sspr->Y, locstr, colour_lookup[2]);
}

void draw_drawitem_1(ushort dihead)
{
    struct DrawItem *itm;
    ushort iidx;

    for (iidx = dihead; iidx != 0; iidx = itm->Child)
    {
      itm = &game_draw_list[iidx];
      switch (itm->Type)
      {
      case 1:
      case 10:
          draw_object_face1a(itm->Offset);
          break;
      case 2:
      case 8:
          break;
      case 3:
          draw_sort_sprite1a(itm->Offset);
          break;
      case 4:
          draw_floor_tile1a(itm->Offset);
          break;
      case 5:
          draw_ex_face(itm->Offset);
          break;
      case 6:
          draw_floor_tile1b(itm->Offset);
          break;
      case 7:
          draw_object_face1b(itm->Offset);
          break;
      case 9:
          draw_object_face4a(itm->Offset);
          break;
      case 11:
          draw_sort_line(&game_sort_lines[itm->Offset]);
          break;
      case 12:
          draw_object_face4b(itm->Offset);
          break;
      case 13:
          draw_sort_sprite1b(itm->Offset);
          break;
      case 14:
          draw_object_face4c(itm->Offset);
          break;
      case 15:
          draw_sort_sprite1c(itm->Offset);
          break;
      }
    }
}

void draw_drawitem_2(ushort dihead)
{
    struct DrawItem *itm;
    ushort iidx;
    ushort i;

    i = 0;
    for (iidx = dihead; iidx != 0; iidx = itm->Child)
    {
      i++;
      if (i > BUCKET_ITEMS_MAX)
          break;
      itm = &game_draw_list[iidx];
      switch (itm->Type)
      {
      case 1:
      case 10:
          draw_object_face1c(itm->Offset);
          break;
      case 3:
          draw_sort_sprite1a(itm->Offset);
          break;
      case 4:
          draw_floor_tile1a(itm->Offset);
          break;
      case 5:
          draw_ex_face(itm->Offset);
          break;
      case 6:
          draw_floor_tile1b(itm->Offset);
          break;
      case 7:
          draw_object_face1b(itm->Offset);
          break;
      case 9:
          draw_object_face4d(itm->Offset);
          break;
      case 11:
          draw_sort_line(&game_sort_lines[itm->Offset]);
          break;
      case 12:
          draw_object_face4b(itm->Offset);
          break;
      case 13:
          draw_sort_sprite1b(itm->Offset);
          break;
      case 14:
          draw_object_face4c(itm->Offset);
          break;
      case 15:
          draw_sort_sprite1c(itm->Offset);
          break;
      case 16:
          draw_object_face4g(itm->Offset);
          break;
      case 17:
          draw_object_face1e(itm->Offset);
          break;
      case 18:
          draw_object_face4f(itm->Offset);
          break;
      case 19:
          draw_effect_object_face(itm->Offset);
          break;
      case 20:
          draw_shrapnel(itm->Offset);
          break;
      case 21:
          draw_phwoar(itm->Offset);
          break;
      case 22:
          draw_sort_sprite_tng(itm->Offset);
          break;
      case 23:
          draw_object_face4e(itm->Offset);
          break;
      case 24:
          draw_object_face1d(itm->Offset);
          break;
      case 25:
          draw_ssample_screen_point(itm->Offset);
          break;
      case 26:
          draw_screen_number(itm->Offset);
          break;
      default:
          break;
      }
    }
}

/******************************************************************************/
