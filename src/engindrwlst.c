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

#include "bfbox.h"
#include "bfkeybd.h"
#include "bfgentab.h"
#include "bfsprite.h"
#include "insspr.h"
#include <assert.h>

#include "display.h"
#include "drawtext.h"
#include "enginbckt.h"
#include "enginlights.h"
#include "enginpeff.h"
#include "enginsngobjs.h"
#include "enginsngtxtr.h"
#include "enginshadws.h"
#include "enginshrapn.h"
#include "engintrns.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "player.h"
#include "thing.h"
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
extern long unkn1_pos_x;
extern long unkn1_pos_y;
extern struct TbSprite *unkn1_spr;
extern long dword_176CE0;
extern long dword_176CE4;
extern long dword_176CE8;
extern long dword_176CEC;
extern long dword_176CF0;
extern long dword_176CF4;
extern long dword_176D00;
extern long dword_176D04;

extern short word_1A5834;
extern short word_1A5836;

extern ubyte byte_1C844E;

sbyte byte_153014[] = {
  1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

void LbSpriteDraw_1(int x, int y, struct TbSprite *spr)
{
    asm volatile (
      "call ASM_LbSpriteDraw_1\n"
        : : "a" (x), "d" (y), "b" (spr));
}

void LbSpriteDraw_2(int x, int y, struct TbSprite *spr)
{
    asm volatile (
      "call ASM_LbSpriteDraw_2\n"
        : : "a" (x), "d" (y), "b" (spr));
}

void draw_unkn1_scaled_alpha_sprite(ushort fr, int scr_x, int scr_y, ushort scale, ushort alpha)
{
#if 0
    asm volatile (
      "push %4\n"
      "call ASM_draw_unkn1_scaled_alpha_sprite\n"
        : : "a" (fr), "d" (scr_x), "b" (scr_y), "c" (scale), "g" (alpha));
    return;
#endif
    struct Frame *p_frm;
    struct Element *p_el;
    int pos_x, pos_y;
    int swidth, sheight;

    p_frm = &frame[fr];
    lbSpriteReMapPtr = &pixmap.fade_table[256 * alpha];
    //TODO would probably make more sense to set the ghost ptr somewhere during game setup
    render_ghost = &pixmap.ghost_table[0*PALETTE_8b_COLORS];

    pos_x = 99999;
    pos_y = 99999;
    for (p_el = &melement_ani[p_frm->FirstElement]; p_el > melement_ani; p_el = &melement_ani[p_el->Next])
    {
        if (pos_x > p_el->X >> 1)
            pos_x = p_el->X >> 1;
        if (pos_y > p_el->Y >> 1)
            pos_y = p_el->Y >> 1;
    }

    swidth = p_frm->SWidth;
    sheight = p_frm->SHeight;
    word_1A5834 = pos_x;
    word_1A5836 = pos_y;
    if ((scale * swidth) >> 9 <= 1)
        return;
    if ((scale * sheight) >> 9 <= 1)
        return;

    dword_176CF0 = (scale * swidth) >> 9;
    dword_176CF4 = (scale * sheight) >> 9;
    dword_176CE0 = scr_x + ((scale * pos_x) >> 8);
    dword_176CE4 = scr_y + ((scale * pos_y) >> 8);
    dword_176CE8 = swidth >> 1;
    dword_176CEC = sheight >> 1;
    SetAlphaScalingData(dword_176CE0, dword_176CE4, dword_176CE8, dword_176CEC, dword_176CF0, dword_176CF4);

    for (p_el = &melement_ani[p_frm->FirstElement]; p_el > melement_ani; p_el = &melement_ani[p_el->Next])
    {
        struct TbSprite *p_spr;

        if ((p_el->Flags & 0xFE00) != 0)
            continue;
        p_spr = (struct TbSprite *)((ubyte *)m_sprites + p_el->ToSprite);
        if (p_spr <= m_sprites)
            continue;

        lbDisplay.DrawFlags = p_el->Flags & 0x0F;
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) == 0)
            lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR8;

        unkn1_pos_x = (p_el->X >> 1) - pos_x;
        unkn1_pos_y = (p_el->Y >> 1) - pos_y;
        unkn1_spr = p_spr;
        DrawSpriteWthShadowUsingScalingData(unkn1_pos_x, unkn1_pos_y, p_spr);
    }
    lbDisplay.DrawFlags = 0;
}

void draw_sorted_sprite1a(ushort frm, short x, short y, ubyte csel)
{
    asm volatile (
      "call ASM_draw_sorted_sprite1a\n"
        : : "a" (frm), "d" (x), "b" (y), "c" (csel));
}

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

/**
 * Draw triangular face with normally textured surface, but dark.
 *
 * The dark view is used to contrast with extremely intense light, like nuclear explosions.
 *
 * @param face Index of SingleObjectFace3 instance.
 */
void draw_object_face3_textrd_dk(ushort face)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face3_textrd_dk\n"
        : : "a" (face));
    return;
#endif
    struct PolyPoint point2;
    struct PolyPoint point1;
    struct PolyPoint point3;
    struct SingleObjectFace3 *p_face;

    p_face = &game_object_faces[face];
    vec_colour = 64;
    vec_mode = 4;

    if (p_face->Texture != 0)
    {
        struct SingleTexture *p_stex;

        p_stex = &game_face_textures[p_face->Texture];
        vec_map = vec_tmap[p_stex->Page];
        if (p_face->GFlags != 0)
        {
            if ((p_face->GFlags & 0x02) != 0)
                vec_map = scratch_buf1;
        }
        point1.U = p_stex->TMapX1 << 16;
        point1.V = p_stex->TMapY1 << 16;
        point2.U = p_stex->TMapX3 << 16;
        point2.V = p_stex->TMapY3 << 16;
        point3.U = p_stex->TMapX2 << 16;
        point3.V = p_stex->TMapY2 << 16;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[0]];
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
        point1.S = shade << 7;
    }
    point1.S = calculate_enginepoint_shade_1(&point1, p_face, 0);

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    if ((vec_mode == 2) || (vec_mode == 0))
    {
        point2.S = 0x200000;
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
        point2.S = shade << 7;
    }
    point2.S = calculate_enginepoint_shade_1(&point2, p_face, 2);

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
    point3.S = calculate_enginepoint_shade_1(&point3, p_face, 1);

    if (!byte_19EC6F)
    {
      point1.S = 0x200000;
      point2.S = 0x200000;
      point3.S = 0x200000;
    }
    dword_176D4C++;

    if (game_perspective == 3)
    {
        vec_colour = colour_lookup[3];
        if (!lbKeyOn[KC_RALT])
        {
            poly_line(&point1, &point3);
            poly_line(&point2, &point3);
            poly_line(&point1, &point2);
        }
    }
    else
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
        if ((p_face->GFlags & 0x01) != 0)
        {
            if (vec_mode == 2)
                vec_mode = 27;
            draw_trigpoly(&point1, &point3, &point2);
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

/**
 * Draw triangular face with textured surface, version G.
 * TODO: figure out how this version is unique.
 *
 * @param face Index of SingleObjectFace3 instance.
 */
void draw_object_face3g_textrd(ushort face)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face3g_textrd\n"
        : : "a" (face));
    return;
#endif
    struct SingleObjectFace3 *p_face;
    struct PolyPoint point1;
    struct PolyPoint point3;
    struct PolyPoint point2;

    p_face = &game_object_faces[face];
    vec_colour = p_face->ExCol;
    vec_mode = p_face->Flags;

    if (p_face->Texture != 0)
    {
        struct SingleTexture *p_stex;

        p_stex = &game_face_textures[p_face->Texture];
        vec_map = vec_tmap[p_stex->Page];
        if (p_face->GFlags != 0)
        {
            if ((p_face->GFlags & 0x02) != 0)
                vec_map = scratch_buf1;
            if ((p_face->GFlags & 0x40) != 0) {
                uint frame;
                frame = gameturn + p_face->Object;
                if ((frame & 0x1F) > 0x10)
                    vec_mode = 5;
            }
        }
        point1.U = p_stex->TMapX1 << 16;
        point1.V = p_stex->TMapY1 << 16;
        point2.U = p_stex->TMapX3 << 16;
        point2.V = p_stex->TMapY3 << 16;
        point3.U = p_stex->TMapX2 << 16;
        point3.V = p_stex->TMapY2 << 16;
    }
    else
    {
        vec_mode = 4;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct Normal *p_nrml;
        int shade;

        p_nrml = &game_normals[p_face->Shade0];
        shade = p_nrml->LightRatio >> 16;
        if (shade < 0)
            shade = 0;
        point1.S = shade << 14;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct Normal *p_nrml;
        int shade;

        p_nrml = &game_normals[p_face->Shade2];
        shade = p_nrml->LightRatio >> 16;
        if (shade < 0)
            shade = 0;
        point2.S = shade << 14;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct Normal *p_nrml;
        int shade;

        p_nrml = &game_normals[p_face->Shade1];
        shade = p_nrml->LightRatio >> 16;
        if (shade < 0)
            shade = 0;
        point3.S = shade << 14;
    }

    {
        ushort mag;
        mag = (p_face->WalkHeader + (p_face->GFlags >> 2)) & 7;
        if (mag != 0)
        {
            point1.S = (mag * point1.S) >> 3;
            point2.S = (mag * point2.S) >> 3;
            point3.S = (mag * point3.S) >> 3;
        }
    }

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
        dword_176D4C++;
    }

    if ((p_face->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
        dword_176D4C++;
    }
}

void draw_object_face4d_textrd_dk(ushort a1)
{
    asm volatile (
      "call ASM_draw_object_face4d_textrd_dk\n"
        : : "a" (a1));
}

void draw_sort_line(struct SortLine *sline)
{
    asm volatile (
      "call ASM_draw_sort_line\n"
        : : "a" (sline));
}

/**
 * Draw a textured beam stored as special object face, usually from energy weapon.
 * What is special about these is that each SingleObjectFace4 instance stores
 * SpecialPoint indexes rather than SinglePoint indexes.
 *
 * @param face4 Index of SingleObjectFace4 instance.
 */
void draw_special_object_face4(ushort face4)
{
#if 0
    asm volatile (
      "call ASM_draw_special_object_face4\n"
        : : "a" (face4));
    return;
#endif

    struct SingleObjectFace4 *p_face4;
    struct PolyPoint point4;
    struct PolyPoint point2;
    struct PolyPoint point1;
    struct PolyPoint point3;

    p_face4 = &game_special_object_faces4[face4];
    vec_colour = p_face4->ExCol;
    vec_mode = p_face4->Flags;

    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_face4->PointNo[0]];
        point2.X = p_scrpoint->X;
        point2.Y = p_scrpoint->Y;
        point2.S = p_face4->Shade0 << 15;
    }
    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_face4->PointNo[1]];
        point1.X = p_scrpoint->X;
        point1.Y = p_scrpoint->Y;
        point1.S = p_face4->Shade1 << 15;
    }
    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_face4->PointNo[2]];
        point3.X = p_scrpoint->X;
        point3.Y = p_scrpoint->Y;
        point3.S = p_face4->Shade2 << 15;
    }
    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_face4->PointNo[3]];
        point4.X = p_scrpoint->X;
        point4.Y = p_scrpoint->Y;
        point4.S = p_face4->Shade3 << 15;
    }

    if ((p_face4->Flags == 10) || (p_face4->Flags == 9))
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        vec_map = vec_tmap[p_sftex->Page];
        point2.U = p_sftex->TMapX1 << 16;
        point2.V = p_sftex->TMapY1 << 16;
        point1.U = p_sftex->TMapX2 << 16;
        point1.V = p_sftex->TMapY2 << 16;
        point3.U = p_sftex->TMapX3 << 16;
        point3.V = p_sftex->TMapY3 << 16;
        point4.U = p_sftex->TMapX4 << 16;
        point4.V = p_sftex->TMapY4 << 16;
    }
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }
    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point3, &point2, &point1);
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point4);
    }
    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point1);
    }
    dword_176D4C++;
}

void draw_sort_sprite1b(int a1)
{
    asm volatile (
      "call ASM_draw_sort_sprite1b\n"
        : : "a" (a1));
}

/**
 * Draw a textured pole between two points, using remaining two point indexes as diameters at each end.
 *
 * @param face4 Index of SingleObjectFace4 instance.
 */
void draw_object_face4_pole(ushort face4)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face4_pole\n"
        : : "a" (face4));
    return;
#endif
    struct SingleObjectFace4 *p_face4;
    struct PolyPoint point2;
    struct PolyPoint point3;
    struct PolyPoint point4;
    struct PolyPoint point1;

    p_face4 = &game_object_faces4[face4];
    vec_colour = p_face4->ExCol;
    vec_mode = p_face4->Flags;

    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        vec_map = vec_tmap[p_sftex->Page];
        point3.U = p_sftex->TMapX1 << 16;
        point3.V = p_sftex->TMapY1 << 16;
        point2.U = p_sftex->TMapX2 << 16;
        point2.V = p_sftex->TMapY2 << 16;
        point1.U = p_sftex->TMapX3 << 16;
        point1.V = p_sftex->TMapY3 << 16;
        point4.U = p_sftex->TMapX4 << 16;
        point4.V = p_sftex->TMapY4 << 16;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;
        int shift_x;

        p_point = &game_object_points[p_face4->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];

        shift_x = (overall_scale * p_face4->PointNo[2]) >> 8;

        point3.X = p_scrpoint->X - shift_x;
        point3.Y = p_scrpoint->Y;

        point2.X = p_scrpoint->X + shift_x;
        point2.Y = p_scrpoint->Y;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;
        int shift_x;

        p_point = &game_object_points[p_face4->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];

        shift_x = (overall_scale * p_face4->PointNo[3]) >> 8;

        point1.X = p_scrpoint->X + shift_x;
        point1.Y = p_scrpoint->Y;

        point4.X = p_scrpoint->X - shift_x;
        point4.Y = p_scrpoint->Y;
    }

    {
        point3.S = 0x200000;
        point2.S = 0x200000;
        point1.S = 0x200000;
        point4.S = 0x200000;
    }

    dword_176D4C++;
    if (vec_mode == 2)
        vec_mode = 27;
    draw_trigpoly(&point1, &point2, &point3);

    dword_176D4C++;
    if (vec_mode == 2)
        vec_mode = 27;
    draw_trigpoly(&point4, &point1, &point3);
}

void draw_sort_sprite1c_sub(ushort a1, short a2, short a3, ubyte a4, ushort a5)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_sort_sprite1c_sub\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5));
}

void draw_sort_sprite1c(ushort sspr)
{
    struct SortSprite *p_sspr;
    p_sspr = &game_sort_sprites[sspr];
    draw_sort_sprite1c_sub(p_sspr->Frame, p_sspr->X, p_sspr->Y, p_sspr->Brightness, p_sspr->Scale);
}

void check_mouse_over_face(struct PolyPoint *pt1, struct PolyPoint *pt2,
  struct PolyPoint *pt3, int face, int type)
{
    asm volatile (
      "push %4\n"
      "call ASM_check_mouse_over_face\n"
        : : "a" (pt1), "d" (pt2), "b" (pt3), "c" (face), "g" (type));
}

/**
 * Draw triangular face with textured surface.
 *
 * @param face Index of SingleObjectFace3 instance.
 */
void draw_object_face3_textrd(ushort face)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face3_textrd\n"
        : : "a" (face));
    return;
#endif
    struct PolyPoint point1;
    struct PolyPoint point2;
    struct PolyPoint point3;
    struct SingleObjectFace3 *p_face;

    p_face = &game_object_faces[face];
    vec_colour = p_face->ExCol;
    vec_mode = p_face->Flags;

    if (p_face->Texture != 0)
    {
        struct SingleTexture *p_stex;

        p_stex = &game_face_textures[p_face->Texture];
        vec_map = vec_tmap[p_stex->Page];
        if (p_face->GFlags != 0)
        {
            if ((p_face->GFlags & 0x02) != 0)
                vec_map = scratch_buf1;
            if ((p_face->GFlags & 0x40) != 0) {
                uint frame;
                frame = gameturn + p_face->Object;
                if ((frame & 0x1FF) > 0x100 && !byte_153014[frame & 0x3F])
                    vec_mode = 5;
            }
        }
        point1.U = p_stex->TMapX1 << 16;
        point1.V = p_stex->TMapY1 << 16;
        point2.U = p_stex->TMapX3 << 16;
        point2.V = p_stex->TMapY3 << 16;
        point3.U = p_stex->TMapX2 << 16;
        point3.V = p_stex->TMapY2 << 16;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[0]];
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
        point1.S = shade << 7;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    if (game_perspective == 7)
    {
        vec_mode = 7;
        vec_colour = point1.S >> 16;
    }
    else
    {
        if ((vec_mode == 2) || (vec_mode == 0))
        {
            point2.S = 0x200000;
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
                intens = game_full_lights[p_qlight->Light].Intensity;
                light = p_qlight->NextQuick;
                shade += intens * p_qlight->Ratio;
            }
            if (shade > 0x7E00)
                shade = 0x7F00;
            point2.S = shade << 7;
        }

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
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }

    if (!byte_19EC6F)
    {
        point1.S = 0x200000;
        point2.S = 0x200000;
        point3.S = 0x200000;
    }
    dword_176D4C++;

    if (game_perspective == 3)
    {
        vec_mode = 0;
        vec_colour = pixmap.fade_table[256 * (point3.S >> 16) + colour_lookup[2]];
        draw_trigpoly(&point1, &point2, &point3);
        vec_colour = colour_lookup[3];
        if (!lbKeyOn[KC_RALT])
        {
            poly_line(&point1, &point3);
            poly_line(&point2, &point3);
            poly_line(&point1, &point2);
        }
    }
    else
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
        if ((p_face->GFlags & 0x01) != 0)
        {
            if (vec_mode == 2)
                vec_mode = 27;
            draw_trigpoly(&point1, &point3, &point2);
            dword_176D4C++;
        }
    }

    if ((p_face->GFlags & 0x04) != 0)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if (p_locplayer->TargetType < 3) {
            check_mouse_over_face(&point1, &point2, &point3, face, 1);
        }
    }

    if (byte_1C844E) {
        swap_wscreen();
    }
}

/**
 * Draw rectangular face with textured surface, version D.
 * TODO: figure out how this version is unique.
 *
 * @param face4 Index of SingleObjectFace4 instance.
 */
void draw_object_face4d_textrd(ushort face4)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face4d_textrd\n"
        : : "a" (face4));
    return;
#endif
    struct SingleObjectFace4 *p_face4;
    struct PolyPoint point3;
    struct PolyPoint point4;
    struct PolyPoint point1;
    struct PolyPoint point2;

    p_face4 = &game_object_faces4[face4];
    vec_colour = p_face4->ExCol;
    vec_mode = p_face4->Flags;
    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        vec_map = vec_tmap[p_sftex->Page];
        if (p_face4->GFlags != 0)
        {
            if ((p_face4->GFlags & 0x02) != 0)
                vec_map = scratch_buf1;
            if ((p_face4->GFlags & 0x40) != 0) {
                uint frame;
                frame = gameturn + p_face4->Object;
                if ((frame & 0x1FF) > 0x100 && !byte_153014[frame & 0x3F])
                    vec_mode = 5;
            }
        }
        if ((p_face4->GFlags & 0x20) != 0) {
            point2.U = p_sftex->TMapX4 << 16;
            point2.V = p_sftex->TMapY4 << 16;
        } else {
            point2.U = p_sftex->TMapX3 << 16;
            point2.V = p_sftex->TMapY3 << 16;
        }
        point1.U = p_sftex->TMapX1 << 16;
        point1.V = p_sftex->TMapY1 << 16;
        point3.U = p_sftex->TMapX2 << 16;
        point3.V = p_sftex->TMapY2 << 16;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    if (vec_mode == 2)
    {
        point1.S = 0x200000;
    }
    else
    {
        ushort light, shade;
        short i;

        light = p_face4->Light0;
        shade = p_face4->Shade0 << 7;
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
        point1.S = shade << 7;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    if (game_perspective == 7)
    {
        vec_mode = 7;
        vec_colour = point1.S >> 16;
    }
    else
    {
        if (vec_mode == 2)
        {
            point2.S = 0x200000;
        }
        else
        {
            ushort light, shade;
            short i;

            light = p_face4->Light2;
            shade = p_face4->Shade2 << 7;
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

        if (vec_mode == 2)
        {
            point3.S = 0x200000;
        }
        else
        {
            ushort light, shade;
            short i;

            light = p_face4->Light1;
            shade = p_face4->Shade1 << 7;
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

        if (vec_mode == 2)
        {
            point4.S = 0x200000;
        }
        else
        {
            ushort light, shade;
            short i;

            light = p_face4->Light3;
            shade = p_face4->Shade3 << 7;
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
            point4.S = shade << 7;
        }
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[3]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point4.X = p_scrpoint->X + dword_176D00;
        point4.Y = p_scrpoint->Y + dword_176D04;
    }

    if (!byte_19EC6F)
    {
        point1.S = 0x200000;
        point2.S = 0x200000;
        point3.S = 0x200000;
        point4.S = 0x200000;
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }

    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }

    if ((p_face4->GFlags & 0x04) != 0)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if (p_locplayer->TargetType < 3) {
            check_mouse_over_face(&point1, &point2, &point3, face4, 2);
        }
    }

    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        if ((p_face4->GFlags & 0x20) != 0) {
            point4.U = p_sftex->TMapX3 << 16;
            point4.V = p_sftex->TMapY3 << 16;
            point2.U = p_sftex->TMapX4 << 16;
            point2.V = p_sftex->TMapY4 << 16;
        } else {
            point4.U = p_sftex->TMapX4 << 16;
            point4.V = p_sftex->TMapY4 << 16;
            point2.U = p_sftex->TMapX3 << 16;
            point2.V = p_sftex->TMapY3 << 16;
        }
        point3.U = p_sftex->TMapX2 << 16;
        point3.V = p_sftex->TMapY2 << 16;
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point2);
    }

    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point2, &point3);
    }

    if ((p_face4->GFlags & 0x04) != 0)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if (p_locplayer->TargetType < 3) {
            check_mouse_over_face(&point4, &point3, &point2, face4, 3);
        }
    }
}

/**
 * Draw rectangular face with textured surface, version G.
 * TODO: figure out how this version is unique.
 *
 * @param face4 Index of SingleObjectFace4 instance.
 */
void draw_object_face4g_textrd(ushort face4)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face4g_textrd\n"
        : : "a" (face4));
    return;
#endif
    struct SingleObjectFace4 *p_face4;
    struct PolyPoint point4;
    struct PolyPoint point3;
    struct PolyPoint point2;
    struct PolyPoint point1;

    p_face4 = &game_object_faces4[face4];
    vec_colour = p_face4->ExCol;
    vec_mode = p_face4->Flags;
    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        vec_map = vec_tmap[p_sftex->Page];
        {
            if ((p_face4->GFlags & 0x02) != 0)
                vec_map = scratch_buf1;
            if ((p_face4->GFlags & (0x40|0x02)) != 0) {
                uint frame;
                frame = gameturn + 4 * p_face4->Object;
                if ((frame & 0x0F) <= 7) {
                    vec_mode = 2;
                } else {
                    vec_colour = 0;
                    vec_mode = 0;
                }
            }
        }
        if ((p_face4->GFlags & 0x20) != 0) {
            point2.U = p_sftex->TMapX4 << 16;
            point2.V = p_sftex->TMapY4 << 16;
        } else {
            point2.U = p_sftex->TMapX3 << 16;
            point2.V = p_sftex->TMapY3 << 16;
        }
        point1.U = p_sftex->TMapX1 << 16;
        point1.V = p_sftex->TMapY1 << 16;
        point3.U = p_sftex->TMapX2 << 16;
        point3.V = p_sftex->TMapY2 << 16;
    }

    if (ingame.draw_unknprop_01 == 0)
        ingame.draw_unknprop_01 = face4;

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct Normal *p_nrml;
        int shade;

        p_nrml = &game_normals[p_face4->Shade0];
        shade = p_nrml->LightRatio >> 16;
        if (shade < 0)
            shade = 0;
        point1.S = shade << 14;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct Normal *p_nrml;
        int shade;

        p_nrml = &game_normals[p_face4->Shade2];
        shade = p_nrml->LightRatio >> 16;
        if (shade < 0)
            shade = 0;
        point2.S = shade << 14;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct Normal *p_nrml;
        int shade;

        p_nrml = &game_normals[p_face4->Shade1];
        shade = p_nrml->LightRatio >> 16;
        if (shade < 0)
            shade = 0;
        point3.S = shade << 14;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[3]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point4.X = p_scrpoint->X + dword_176D00;
        point4.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct Normal *p_nrml;
        int shade;

        p_nrml = &game_normals[p_face4->Shade3];
        shade = p_nrml->LightRatio >> 16;
        if (shade < 0)
            shade = 0;
        point4.S = shade << 14;
    }

    {
        ushort mag;
        mag = (p_face4->WalkHeader + (p_face4->GFlags >> 2)) & 7;
        if (mag != 0)
        {
            point1.S = (mag * point1.S) >> 3;
            point2.S = (mag * point2.S) >> 3;
            point3.S = (mag * point3.S) >> 3;
            point4.S = (mag * point4.S) >> 3;
        }
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }

    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }

    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        if ((p_face4->GFlags & 0x20) != 0) {
            point4.U = p_sftex->TMapX3 << 16;
            point4.V = p_sftex->TMapY3 << 16;
            point2.U = p_sftex->TMapX4 << 16;
            point2.V = p_sftex->TMapY4 << 16;
        } else {
            point4.U = p_sftex->TMapX4 << 16;
            point4.V = p_sftex->TMapY4 << 16;
            point2.U = p_sftex->TMapX3 << 16;
            point2.V = p_sftex->TMapY3 << 16;
        }
        point3.U = p_sftex->TMapX2 << 16;
        point3.V = p_sftex->TMapY2 << 16;
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point2);
    }

    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point2, &point3);
    }
}

/**
 * Draw triangular face with reflective (mirror) surface.
 *
 * @param face Index of SingleObjectFace3 instance.
 */
void draw_object_face3_reflect(ushort face)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face3_reflect\n"
        : : "a" (face));
    return;
#endif
    struct SingleObjectFace3 *p_face;
    struct PolyPoint point3;
    struct PolyPoint point1;
    struct PolyPoint point2;

    p_face = &game_object_faces[face];
    vec_colour = p_face->ExCol;
    vec_mode = 27;
    vec_map = vec_tmap[4];
    {
        struct Normal *p_nrml;
        sbyte texU, texV;

        p_nrml = &game_normals[p_face->Shade0];
        texU = p_nrml->LightRatio;
        texV = p_nrml->LightRatio >> 8;
        point1.U = (texU + 32) << 16;
        point1.V = (texV + 128) << 16;
    }
    {
        struct Normal *p_nrml;
        sbyte texU, texV;

        p_nrml = &game_normals[p_face->Shade2];
        texU = p_nrml->LightRatio;
        texV = p_nrml->LightRatio >> 8;
        point2.U = (texU + 32) << 16;
        point2.V = (texV + 128) << 16;
    }
    {
        struct Normal *p_nrml;
        sbyte texU, texV;

        p_nrml = &game_normals[p_face->Shade1];
        texU = p_nrml->LightRatio;
        texV = p_nrml->LightRatio >> 8;
        point3.U = (texU + 32) << 16;
        point3.V = (texV + 128) << 16;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }

    if ((p_face->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }
}

/**
 * Draw rectangular face with reflective (mirror) surface.
 *
 * @param face4 Index of SingleObjectFace4 instance.
 */
void draw_object_face4_reflect(ushort face4)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face4_reflect\n"
        : : "a" (face4));
    return;
#endif
    struct SingleObjectFace4 *p_face4;
    struct PolyPoint point4;
    struct PolyPoint point1;
    struct PolyPoint point3;
    struct PolyPoint point2;

    p_face4 = &game_object_faces4[face4];
    vec_colour = p_face4->ExCol;
    vec_mode = 27;
    vec_map = vec_tmap[4];
    {
        struct Normal *p_nrml;
        sbyte texU, texV;

        p_nrml = &game_normals[p_face4->Shade0];
        texU = p_nrml->LightRatio;
        texV = p_nrml->LightRatio >> 8;
        point1.U = (texU + 32) << 16;
        point1.V = (texV + 128) << 16;
    }
    {
        struct Normal *p_nrml;
        sbyte texU, texV;

        p_nrml = &game_normals[p_face4->Shade2];
        texU = p_nrml->LightRatio;
        texV = p_nrml->LightRatio >> 8;
        point2.U = (texU + 32) << 16;
        point2.V = (texV + 128) << 16;
    }
    {
        struct Normal *p_nrml;
        sbyte texU, texV;

        p_nrml = &game_normals[p_face4->Shade1];
        texU = p_nrml->LightRatio;
        texV = p_nrml->LightRatio >> 8;
        point3.U = (texU + 32) << 16;
        point3.V = (texV + 128) << 16;
    }
    {
        struct Normal *p_nrml;
        sbyte texU, texV;

        p_nrml = &game_normals[p_face4->Shade3];
        texU = p_nrml->LightRatio;
        texV = p_nrml->LightRatio >> 8;
        point4.U = (texU + 32) << 16;
        point4.V = (texV + 128) << 16;
    }

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[3]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point4.X = p_scrpoint->X + dword_176D00;
        point4.Y = p_scrpoint->Y + dword_176D04;
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }

    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }
    dword_176D4C++;

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point2);
    }

    if ((p_face4->GFlags & 0x01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point2, &point3);
    }
}

/**
 * Draw shrapnel polygon.
 *
 * @param shrap Index of Shrapnel instance.
 */
void draw_shrapnel(ushort shrap)
{
#if 0
    asm volatile (
      "call ASM_draw_shrapnel\n"
        : : "a" (shrap));
    return;
#endif
    struct Shrapnel *p_shrap;
    struct PolyPoint point3;
    struct PolyPoint point2;
    struct PolyPoint point1;

    p_shrap = &shrapnel[shrap];

    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_shrap->PointOffset + 0];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_shrap->PointOffset + 1];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_shrap->PointOffset + 2];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }

    vec_mode = 0;
    vec_colour = colour_lookup[8];
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }
    vec_colour = colour_lookup[9];
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }
}

/**
 * Draw smoke cloud sprite.
 *
 * @param ph Index of Phwoar instance.
 */
void draw_phwoar(ushort ph)
{
#if 0
    asm volatile (
      "call ASM_draw_phwoar\n"
        : : "a" (ph));
    return;
#endif
    struct Phwoar *p_phwoar;
    struct Element *p_elem;
    ushort el;
    int point_x, point_y;

    p_phwoar = &phwoar[ph];
    {
        struct SpecialPoint *p_scrpoint;

        p_scrpoint = &game_screen_point_pool[p_phwoar->PointOffset];
        point_x = p_scrpoint->X + dword_176D00;
        point_y = p_scrpoint->Y + dword_176D04;
    }

    el = frame[p_phwoar->f].FirstElement;
    for (p_elem = &melement_ani[el]; p_elem > melement_ani; p_elem = &melement_ani[el])
    {
        struct TbSprite *p_spr;

        el = p_elem->Next;
        p_spr = (struct TbSprite *)((ubyte *)m_sprites + p_elem->ToSprite);
        if ((p_spr <= m_sprites) || (p_spr >= m_sprites_end))
            continue;

        lbDisplay.DrawFlags = p_elem->Flags & 0x07;
        if ((p_elem->Flags & 0xFE00) == 0)
        {
            if (lbDisplay.ScreenMode == 1)
                LbSpriteDraw_1(point_x + (p_elem->X >> 1), point_y + (p_elem->Y >> 1), p_spr);
            else
                LbSpriteDraw_2(point_x + p_elem->X, point_y + p_elem->Y, p_spr);
        }
        if (word_1A5834 > p_elem->X >> 1)
            word_1A5834 = p_elem->X >> 1;
        if (word_1A5836 > p_elem->Y >> 1)
            word_1A5836 = p_elem->Y >> 1;
    }
    lbDisplay.DrawFlags = 0;
}

/**
 * Draw health bar of a vehicle.
 *
 * @param sspr Index of SortSprite instance which stores reference to the vehicle thing.
 */
void draw_sort_sprite_veh_health_bar(short sspr)
{
#if 0
    asm volatile (
      "call ASM_draw_sort_sprite_veh_health_bar\n"
        : : "a" (sspr));
    return;
#endif
    struct SortSprite *p_sspr;
    struct Thing *p_thing;
    ushort max_health;
    short health;
    ushort range_x;
    short level_x;
    ushort h;

    p_sspr = &game_sort_sprites[sspr];
    p_thing = p_sspr->PThing;

    max_health = p_thing->U.UVehicle.MaxHealth;
    health = p_thing->Health;
    if (max_health == 0)
        max_health = 1;
    if (health < 0)
        health = 0;
    else if (health > max_health)
        health = max_health + 1;

    range_x = (40 * overall_scale) >> 8;
    if (ingame.PanelPermutation == -3)
        h = 42;
    else
        h = 19;
    LbDrawBox(p_sspr->X - (range_x >> 1), p_sspr->Y, range_x + 4, 6, h);

    level_x = range_x * health / max_health;
    if (ingame.PanelPermutation == -3)
        h = 33;
    else
        h = 15;
    LbDrawBox(p_sspr->X - (range_x >> 1) + 2, p_sspr->Y + 1, level_x, 4, h);
}

void draw_object_face4_deep_rdr(ushort face4)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face4_deep_rdr\n"
        : : "a" (face4));
    return;
#endif
    struct SingleObjectFace4 *p_face4;
    struct PolyPoint point4;
    struct PolyPoint point1;
    struct PolyPoint point2;
    struct PolyPoint point3;

    p_face4 = &game_object_faces4[face4];
    vec_colour = deep_radar_surface_col;
    vec_mode = 15;

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[3]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point4.X = p_scrpoint->X + dword_176D00;
        point4.Y = p_scrpoint->Y + dword_176D04;
    }

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point2);
    }

    vec_colour = deep_radar_line_col;

    poly_line(&point1, &point2);
    poly_line(&point3, &point1);

    if ((p_face4->GFlags & 0x04) != 0)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if (p_locplayer->TargetType < 3) {
            check_mouse_over_face(&point1, &point2, &point3, face4, 2);
        }
    }

    poly_line(&point4, &point3);
    poly_line(&point4, &point2);

    if ((p_face4->GFlags & 0x04) != 0)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if (p_locplayer->TargetType < 3) {
            check_mouse_over_face(&point4, &point3, &point2, face4, 3);
        }
    }
}

void draw_object_face3_deep_rdr(ushort face)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face3_deep_rdr\n"
        : : "a" (face));
    return;
#endif
    struct SingleObjectFace3 *p_face;
    struct PolyPoint point2;
    struct PolyPoint point1;
    struct PolyPoint point3;

    p_face = &game_object_faces[face];

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[0]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point1.X = p_scrpoint->X + dword_176D00;
        point1.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }

    vec_colour = deep_radar_line_col;
    poly_line(&point1, &point3);
    poly_line(&point2, &point3);
    poly_line(&point1, &point2);

    vec_colour = deep_radar_surface_col;
    vec_mode = 15;
    draw_trigpoly(&point1, &point2, &point3);

    if ((p_face->GFlags & 0x04) != 0)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if (p_locplayer->TargetType < 3) {
            check_mouse_over_face(&point1, &point2, &point3, face, 1);
        }
    }
}

void draw_fire_flame(ushort flm)
{
#if 0
    asm volatile (
      "call ASM_draw_fire_flame\n"
        : : "a" (flm));
    return;
#endif
    struct FireFlame *p_flame;
    struct SpecialPoint *p_scrpoint;

    p_flame = &FIRE_flame[flm];
    if (p_flame->big != 0)
    {
        p_scrpoint = &game_screen_point_pool[p_flame->PointOffset];
        draw_unkn1_scaled_alpha_sprite(p_flame->frame, p_scrpoint->X + dword_176D00,
          p_scrpoint->Y + dword_176D04, (overall_scale * (p_flame->big + 128)) >> 7, 0x20);
    }
    else
    {
        p_scrpoint = &game_screen_point_pool[p_flame->PointOffset];
        draw_sorted_sprite1a(p_flame->frame, p_scrpoint->X + dword_176D00,
          p_scrpoint->Y + dword_176D04, 0x20);
    }
}

void draw_sort_sprite_number(ushort sospr)
{
    char locstr[50];
    struct SortSprite *p_sospr;

    p_sospr = &game_sort_sprites[sospr];
    sprintf(locstr, "%d", (int)p_sospr->PThing);
    draw_text(2 * p_sospr->X,2 * p_sospr->Y, locstr, colour_lookup[2]);
}

// Special non-textured draw; used during nuclear explosions?
void draw_drawitem_1(ushort dihead)
{
    struct DrawItem *itm;
    ushort iidx;

    for (iidx = dihead; iidx != 0; iidx = itm->Child)
    {
      itm = &game_draw_list[iidx];
      switch (itm->Type)
      {
      case DrIT_Unkn1:
      case DrIT_Unkn10:
          draw_object_face3_textrd_dk(itm->Offset);
          break;
      case DrIT_Unkn2:
      case DrIT_Unkn8:
          break;
      case DrIT_Unkn3:
          draw_sort_sprite1a(itm->Offset);
          break;
      case DrIT_Unkn4:
          draw_floor_tile1a(itm->Offset);
          break;
      case DrIT_Unkn5:
          draw_ex_face(itm->Offset);
          break;
      case DrIT_Unkn6:
          draw_floor_tile1b(itm->Offset);
          break;
      case DrIT_Unkn7:
          draw_object_face3g_textrd(itm->Offset);
          break;
      case DrIT_Unkn9:
          draw_object_face4d_textrd_dk(itm->Offset);
          break;
      case DrIT_Unkn11:
          draw_sort_line(&game_sort_lines[itm->Offset]);
          break;
      case DrIT_Unkn12:
          draw_special_object_face4(itm->Offset);
          break;
      case DrIT_Unkn13:
          draw_sort_sprite1b(itm->Offset);
          break;
      case DrIT_Unkn14:
          draw_object_face4_pole(itm->Offset);
          break;
      case DrIT_Unkn15:
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
      case DrIT_Unkn1:
      case DrIT_Unkn10:
          draw_object_face3_textrd(itm->Offset);
          break;
      case DrIT_Unkn3:
          draw_sort_sprite1a(itm->Offset);
          break;
      case DrIT_Unkn4:
          draw_floor_tile1a(itm->Offset);
          break;
      case DrIT_Unkn5:
          draw_ex_face(itm->Offset);
          break;
      case DrIT_Unkn6:
          draw_floor_tile1b(itm->Offset);
          break;
      case DrIT_Unkn7:
          draw_object_face3g_textrd(itm->Offset);
          break;
      case DrIT_Unkn9:
          draw_object_face4d_textrd(itm->Offset);
          break;
      case DrIT_Unkn11:
          draw_sort_line(&game_sort_lines[itm->Offset]);
          break;
      case DrIT_Unkn12:
          draw_special_object_face4(itm->Offset);
          break;
      case DrIT_Unkn13:
          draw_sort_sprite1b(itm->Offset);
          break;
      case DrIT_Unkn14:
          draw_object_face4_pole(itm->Offset);
          break;
      case DrIT_Unkn15:
          draw_sort_sprite1c(itm->Offset);
          break;
      case DrIT_Unkn16:
          draw_object_face4g_textrd(itm->Offset);
          break;
      case DrIT_Unkn17:
          draw_object_face3_reflect(itm->Offset);
          break;
      case DrIT_Unkn18:
          draw_object_face4_reflect(itm->Offset);
          break;
      case DrIT_Unkn19:
          draw_person_shadow(itm->Offset);
          break;
      case DrIT_Unkn20:
          draw_shrapnel(itm->Offset);
          break;
      case DrIT_Unkn21:
          draw_phwoar(itm->Offset);
          break;
      case DrIT_Unkn22:
          draw_sort_sprite_veh_health_bar(itm->Offset);
          break;
      case DrIT_Unkn23:
          draw_object_face4_deep_rdr(itm->Offset);
          break;
      case DrIT_Unkn24:
          draw_object_face3_deep_rdr(itm->Offset);
          break;
      case DrIT_Unkn25:
          draw_fire_flame(itm->Offset);
          break;
      case DrIT_Unkn26:
          draw_sort_sprite_number(itm->Offset);
          break;
      default:
          break;
      }
    }
}

/******************************************************************************/
