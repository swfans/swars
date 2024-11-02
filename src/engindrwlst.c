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

#include "bfkeybd.h"

#include "display.h"
#include "drawtext.h"
#include "enginbckt.h"
#include "enginlights.h"
#include "enginpeff.h"
#include "enginsngobjs.h"
#include "enginsngtxtr.h"
#include "enginshadws.h"
#include "engintrns.h"
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
extern long dword_176D00;
extern long dword_176D04;

extern ubyte byte_1C844E;

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

int calculate_enginepoint_shade_1(struct PolyPoint *p_point, struct SingleObjectFace3 *p_face, ushort a3)
{
    int ret;
    asm volatile (
      "call ASM_calculate_enginepoint_shade_1\n"
        : "=r" (ret) : "a" (p_point), "d" (p_face), "b" (a3));
    return ret;
}

void draw_object_face1a(ushort face)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face1a\n"
        : : "a" (face));
#endif
    struct PolyPoint point1;
    struct PolyPoint point2;
    struct PolyPoint point3;
    struct SingleObjectFace3 *p_face;

    p_face = &game_object_faces[face];
    if (p_face->Texture != 0)
    {
        struct SingleTexture *p_stex;

        p_stex = &game_face_textures[p_face->Texture];
        vec_map = vec_tmap[p_stex->Page];
        if ((p_face->GFlags & 0x02) != 0)
            vec_map = scratch_buf1;
        point2.U = p_stex->TMapX1 << 16;
        point2.V = p_stex->TMapY1 << 16;
        point1.U = p_stex->TMapX3 << 16;
        point1.V = p_stex->TMapY3 << 16;
        point3.U = p_stex->TMapX2 << 16;
        point3.V = p_stex->TMapY2 << 16;
    }
    vec_colour = 64;
    vec_mode = 4;

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        ushort light, shade;
        short i;

        light = p_face->Light0;
        shade = p_face->Shade0 << 7;
        for (i = 0; (i <= 100) && (light != 0); i++)
        {
            struct QuickLight *p_qlight;
            short intens;

            p_qlight = &game_quick_lights[light];
            intens = game_full_lights[p_qlight->Light].Intensity;
            light = p_qlight->NextQuick;
            shade += intens * p_qlight->Ratio;
        }
        if (shade > 0x7E00)
            shade = 0x7F00;
        point2.S = shade << 7;
    }
    point2.S = calculate_enginepoint_shade_1(&point2, p_face, 0);

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    if ((vec_mode == 2) || (vec_mode == 0))
    {
        point1.S = 0x200000;
    }
    else
    {
        ushort light, shade;
        short i;

        light = p_face->Light2;
        shade = p_face->Shade2 << 7;
        for (i = 0; (i <= 100) && (light != 0); i++)
        {
            struct QuickLight *p_qlight;
            short intens;

            p_qlight = &game_quick_lights[light];
            intens = p_qlight->Ratio * game_full_lights[p_qlight->Light].Intensity;
            light = p_qlight->NextQuick;
            shade += intens;
        }
        if (shade > 0x7E00)
            shade = 0x7F00;
        point1.S = shade << 7;
    }
    point1.S = calculate_enginepoint_shade_1(&point1, p_face, 2);

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    if ((vec_mode == 2) || (vec_mode == 0))
    {
        point3.S = 0x200000;
    }
    else
    {
        ushort light, shade;
        short i;

        light = p_face->Light1;
        shade = p_face->Shade1 << 7;
        for (i = 0; (i <= 100) && (light != 0); i++)
        {
            struct QuickLight *p_qlight;
            short intens;

            p_qlight = &game_quick_lights[light];
            intens = game_full_lights[p_qlight->Light].Intensity;
            light = p_qlight->NextQuick;
            shade += intens * p_qlight->Ratio;
        }
        if (shade > 0x7E00)
            shade = 0x7F00;
        point3.S = shade << 7;
    }
    point3.S = calculate_enginepoint_shade_1(&point3, p_face, 1u);

    if (!byte_19EC6F)
    {
      point2.S = 0x200000;
      point1.S = 0x200000;
      point3.S = 0x200000;
    }
    dword_176D4C++;

    if (game_perspective == 3)
    {
        vec_colour = colour_lookup[3];
        if (!lbKeyOn[KC_RALT])
        {
            poly_line(&point2, &point3);
            poly_line(&point1, &point3);
            poly_line(&point2, &point1);
        }
    }
    else
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point2, &point1, &point3);
        if ((p_face->GFlags & 0x01) != 0)
        {
            if (vec_mode == 2)
                vec_mode = 27;
            draw_trigpoly(&point2, &point3, &point1);
            dword_176D4C++;
        }
    }
    if (byte_1C844E) {
        swap_wscreen();
    }
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
          draw_person_shadow(itm->Offset);
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
