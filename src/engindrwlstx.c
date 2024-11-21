/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engindrwlstx.c
 *     Drawlists execution for the 3D engine.
 * @par Purpose:
 *     Implements functions for executing previously made drawlists,
 *     meaning the actual drawing based on primitives in the list.
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
#include "engindrwlstx.h"

#include "bfbox.h"
#include "bfkeybd.h"
#include "bfgentab.h"
#include "bfline.h"
#include "bfsprite.h"
#include "insspr.h"
#include <assert.h>

#include "bflib_render_drspr.h"

#include "bigmap.h"
#include "display.h"
#include "drawtext.h"
#include "enginbckt.h"
#include "engindrwlstm.h"
#include "enginfexpl.h"
#include "enginfloor.h"
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
#define MAX_LIGHTS_AFFECTING_FACE 100

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

extern ubyte byte_176D49;

extern long dword_19F4FC;
extern long dword_19F500;
extern long dword_19F504;
extern long dword_19F508;

extern ushort shield_frm[4];

extern short word_1A5834;
extern short word_1A5836;

extern long sprite_over_16x16;

extern ubyte byte_1C844E;

extern ubyte byte_1DB2E9;

sbyte byte_153014[] = {
  1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

ubyte byte_15399C[] = {
  0, 1, 2, 0, 0,
  0, 2, 1, 0, 0,
  0, 1, 1, 0, 0,
  1, 2, 0, 0, 0,
  1, 1, 0, 0, 0,
  2, 0, 1, 0, 0,
  2, 0, 2, 0, 0,
  2, 0, 2, 0, 0,
};

void draw_unkn1_scaled_alpha_sprite(ushort frm, int scr_x, int scr_y, ushort scale, ushort alpha)
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

    p_frm = &frame[frm];
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

void draw_unkn2_scaled_alpha_sprite(ubyte *frv, ushort frm, short x, short y,
  ubyte bri)
{
#if 0
    asm volatile (
      "push %4\n"
      "call ASM_draw_unkn2_scaled_alpha_sprite\n"
        : : "a" (frv), "d" (frm), "b" (x), "c" (y), "g" ((uint)bri));
    return;
#endif
    struct Frame *p_frm;
    struct Element *p_elem;
    int max_x, max_y;
    int min_x, min_y;
    int range_x, range_y;
    TbBool really_draw;

    really_draw = 0;
    max_x = -99999;
    max_y = -99999;
    min_x = 99999;
    min_y = 99999;
    p_frm = &frame[frm];

    for (p_elem = &melement_ani[p_frm->FirstElement]; p_elem > melement_ani; p_elem = &melement_ani[p_elem->Next])
    {
        struct TbSprite *p_spr;

        if (frv[(p_elem->Flags >> 4) & 0x1F] == ((p_elem->Flags >> 9) & 0x07))
        {
            int tmp;

            really_draw = 1;
            p_spr = (struct TbSprite *)((ubyte *)m_sprites + p_elem->ToSprite);
            if (min_x > p_elem->X >> 1)
                min_x = p_elem->X >> 1;
            if (min_y > p_elem->Y >> 1)
                min_y = p_elem->Y >> 1;
            tmp = p_spr->SWidth + (p_elem->X >> 1);
            if (max_x < tmp)
                max_x = tmp;
            tmp = p_spr->SHeight + (p_elem->Y >> 1);
            if (max_y < tmp)
                max_y = tmp;
        }
    }

    if (!really_draw)
        return;

    lbSpriteReMapPtr = &pixmap.fade_table[256 * bri];
    word_1A5834 = min_x;
    word_1A5836 = min_y;

    range_x = max_x - min_x;
    range_y = max_y - min_y;
    if ((range_x > 0) && (range_x <= 128) && (range_y > 0) && (range_y <= 128))
    {
        if ( (overall_scale * p_frm->SWidth) >> 9 > 1 && (overall_scale * p_frm->SHeight) >> 9 > 1 )
        {
            dword_176CE0 = ((min_x * overall_scale) >> 8) + x;
            dword_176CE4 = ((min_y * overall_scale) >> 8) + y;
            dword_176CE8 = range_x;
            dword_176CEC = range_y;
            dword_176CF0 = (range_x * overall_scale) >> 8;
            dword_176CF4 = (range_y * overall_scale) >> 8;
            SetAlphaScalingData(dword_176CE0, dword_176CE4, dword_176CE8, dword_176CEC, dword_176CF0, dword_176CF4);

            for (p_elem = &melement_ani[p_frm->FirstElement]; p_elem > melement_ani; p_elem = &melement_ani[p_elem->Next])
            {
                struct TbSprite *p_spr;

                p_spr = (struct TbSprite *)((ubyte *)m_sprites + p_elem->ToSprite);
                if (p_spr <= m_sprites)
                    continue;

                lbDisplay.DrawFlags = p_elem->Flags & 7;
                if ((lbDisplay.DrawFlags & 0x0004) == 0)
                    lbDisplay.DrawFlags |= 0x0008;

                if (frv[(p_elem->Flags >> 4) & 0x1F] == ((p_elem->Flags >> 9) & 0x07))
                {
                    unkn1_pos_x = (p_elem->X >> 1) - min_x;
                    unkn1_pos_y = (p_elem->Y >> 1) - min_y;
                    unkn1_spr = p_spr;
                    DrawSpriteWthShadowUsingScalingData(unkn1_pos_x, unkn1_pos_y, unkn1_spr);
                }
            }
            lbDisplay.DrawFlags = 0;
        }
    }
}


void draw_unkn3_scaled_alpha_sprite(ushort frm, short x, short y, ubyte bri)
{
    struct Frame *p_frm;
    struct Element *p_elem;

    p_frm = &frame[frm];
    for (p_elem = &melement_ani[p_frm->FirstElement]; p_elem > melement_ani; p_elem = &melement_ani[p_elem->Next])
    {
        struct TbSprite *p_spr;

        p_spr = (struct TbSprite *)((ubyte *)m_sprites + p_elem->ToSprite);
        if (p_spr <= m_sprites)
            continue;

        lbDisplay.DrawFlags = p_elem->Flags & 0x07;
        if ((p_elem->Flags & 0xFE00) == 0) {
            LbSpriteDrawRemap((p_elem->X >> 1) + x, (p_elem->Y >> 1) + y,
              p_spr, &pixmap.fade_table[256 * bri]);
        }
        if (word_1A5834 > p_elem->X >> 1)
            word_1A5834 = p_elem->X >> 1;
        if (word_1A5836 > p_elem->Y >> 1)
            word_1A5836 = p_elem->Y >> 1;
    }
    lbDisplay.DrawFlags = 0;
}

void draw_unkn4_scaled_alpha_sprite(ubyte *frv, ushort frm, short x, short y,
  ubyte bri)
{
    struct Frame *p_frm;
    struct Element *p_elem;

    p_frm = &frame[frm];
    for (p_elem = &melement_ani[p_frm->FirstElement]; p_elem > melement_ani; p_elem = &melement_ani[p_elem->Next])
    {
        struct TbSprite *p_spr;

        p_spr = (struct TbSprite *)((ubyte *)m_sprites + p_elem->ToSprite);
        if (p_spr <= m_sprites)
            continue;

        lbDisplay.DrawFlags = p_elem->Flags & 7;
        if (frv[(p_elem->Flags >> 4) & 0x1F] == ((p_elem->Flags >> 9) & 0x07))
        {
            if (((p_elem->Flags >> 4) & 0x1F) == 4)
                LbSpriteDraw((p_elem->X >> 1) + x, (p_elem->Y >> 1) + y, p_spr);
            else
                LbSpriteDrawRemap((p_elem->X >> 1) + x, (p_elem->Y >> 1) + y,
                  p_spr, &pixmap.fade_table[256 * bri]);
            if (word_1A5834 > p_elem->X >> 1)
                word_1A5834 = p_elem->X >> 1;
            if (word_1A5836 > p_elem->Y >> 1)
                word_1A5836 = p_elem->Y >> 1;
        }
    }
    lbDisplay.DrawFlags = 0;
}

void reset_drawlist(void)
{
    tnext_screen_point = next_screen_point;
    next_screen_point = 0;

    next_sort_line = 0;
    p_current_sort_line = &game_sort_lines[next_sort_line];

    tnext_draw_item = next_draw_item;
    next_draw_item = 1;
    p_current_draw_item = &game_draw_list[next_draw_item];

    tnext_sort_sprite = next_sort_sprite;
    next_sort_sprite = 0;
    p_current_sort_sprite = &game_sort_sprites[next_sort_sprite];

    next_special_face = 1;

    tnext_special_face4 = next_special_face4;
    next_special_face4 = 1;

    ingame.NextRocket = 0;

    tnext_floor_texture = next_floor_texture;

    next_floor_tile = 1;

    dword_176CC4 = 0;
}

int calculate_enginepoint_shade_1(struct PolyPoint *p_pt1, struct SingleObjectFace3 *p_face, ushort pt2)
{
#if 0
    int ret;
    asm volatile (
      "call ASM_calculate_enginepoint_shade_1\n"
        : "=r" (ret) : "a" (p_pt1), "d" (p_face), "b" (pt2));
    return ret;
#endif
    struct SinglePoint *p_pt2;
    struct SingleObject *p_sobj;
    int dist_x, dist_y, dist_z;
    int distance;

    p_pt2 = &game_object_points[p_face->PointNo[pt2]];
    p_sobj = &game_objects[p_face->Object];

    dist_x = dword_19F500 - p_pt2->X - p_sobj->MapX;
    dist_y = dword_19F504 - p_pt2->Y - p_sobj->OffsetY;
    dist_z = dword_19F508 - p_pt2->Z - p_sobj->MapZ;
    distance = (dist_y * dist_y + dist_x * dist_x + dist_z * dist_z) >> 17;

    if (distance != 0)
        p_pt1->S += dword_19F4FC * (0x1000000 / distance);
    else
        p_pt1->S = 0x3F0000;

    if (p_pt1->S > 0x3F0000)
        p_pt1->S = 0x3F0000;

    return p_pt1->S;
}

int calculate_enginepoint_shade_2(struct PolyPoint *p_pt1, struct SingleObjectFace4 *p_face4, ushort pt2)
{
#if 0
    int ret;
    asm volatile (
      "call ASM_calculate_enginepoint_shade_2\n"
        : "=r" (ret) : "a" (p_pt1), "d" (p_face4), "b" (pt2));
    return ret;
#endif
    struct SinglePoint *p_pt2;
    struct SingleObject *p_sobj;
    int dist_x, dist_y, dist_z;
    int distance;

    p_pt2 = &game_object_points[p_face4->PointNo[pt2]];
    p_sobj = &game_objects[p_face4->Object];

    dist_x = dword_19F500 - p_pt2->X - p_sobj->MapX;
    dist_y = dword_19F504 - p_pt2->Y - p_sobj->OffsetY;
    dist_z = dword_19F508 - p_pt2->Z - p_sobj->MapZ;
    distance = (dist_y * dist_y + dist_x * dist_x + dist_z * dist_z) >> 17;

    if (distance != 0)
        p_pt1->S += dword_19F4FC * (0x1000000 / distance);
    else
        p_pt1->S = 0x3F0000;

    if (p_pt1->S > 0x3F0000)
        p_pt1->S = 0x3F0000;

    return p_pt1->S;
}

uint cummulate_shade_from_quick_lights(ushort light_first)
{
        struct QuickLight *p_qlight;
        ushort light;
        uint shade;
        short i;

        shade = 0;
        for (light = light_first, i = 0; light != 0; light = p_qlight->NextQuick, i++)
        {
            short intens;

            if (i > MAX_LIGHTS_AFFECTING_FACE)
                break;
            p_qlight = &game_quick_lights[light];
            intens = game_full_lights[p_qlight->Light].Intensity;
            shade += intens * p_qlight->Ratio;
        }
        return shade;
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
            if ((p_face->GFlags & FGFlg_Unkn02) != 0)
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
        uint shade;

        shade = p_face->Shade0 << 7;
        shade += cummulate_shade_from_quick_lights(p_face->Light0);
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
        ushort shade;

        shade = p_face->Shade2 << 7;
        shade += cummulate_shade_from_quick_lights(p_face->Light2);
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
        ushort shade;

        shade = p_face->Shade1 << 7;
        shade += cummulate_shade_from_quick_lights(p_face->Light1);
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
        if ((p_face->GFlags & FGFlg_Unkn01) != 0)
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

void debug_check_unkn_sprite_size(const char *src_fname, int src_line)
{
    if (!sprite_over_16x16 && (m_sprites[1158].SWidth > 16 || m_sprites[1158].SHeight > 16))
        sprite_over_16x16 = 1;
}

void draw_sorted_sprite1b(ubyte *frv, ushort frm, short x, short y,
  ubyte bri, ubyte angle)
{
#if 0
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_draw_sorted_sprite1b\n"
        : : "a" (frv), "d" (frm), "b" (x), "c" (y), "g" (bri), "g" (angle));
    return;
#endif
#if 0
    debug_check_unkn_sprite_size(__FILE__, __LINE__);
#endif

    if ((frv[4] != 0) && (angle > 1) && (angle < 7))
        bri += 15;
    if (bri < 10)
        bri = 10;
    if (bri > 60)
        bri = 60;

    if ((overall_scale == 256) || (overall_scale <= 0) || (overall_scale >= 4096))
    {
        draw_unkn4_scaled_alpha_sprite(frv, frm, x, y, bri);
    }
    else
    {
        draw_unkn2_scaled_alpha_sprite(frv, frm, x, y, bri);
    }
}

ubyte check_mouse_overlap(ushort sspr)
{
    ubyte ret;
    asm volatile (
      "call ASM_check_mouse_overlap\n"
        : "=r" (ret) : "a" (sspr));
    return ret;
}

ubyte check_mouse_overlap_item(ushort sspr)
{
    ubyte ret;
    asm volatile (
      "call ASM_check_mouse_overlap_item\n"
        : "=r" (ret) : "a" (sspr));
    return ret;
}

ubyte check_mouse_overlap_corpse(ushort sspr)
{
    ubyte ret;
    asm volatile (
      "call ASM_check_mouse_overlap_corpse\n"
        : "=r" (ret) : "a" (sspr));
    return ret;
}

ubyte check_mouse_over_unkn2(ushort sspr, struct Thing *p_thing)
{
    ubyte ret;
    asm volatile (
      "call ASM_check_mouse_over_unkn2\n"
        : "=r" (ret) : "a" (sspr), "d" (p_thing));
    return ret;
}

void check_mouse_over_face(struct PolyPoint *pt1, struct PolyPoint *pt2,
  struct PolyPoint *pt3, int face, int type)
{
    asm volatile (
      "push %4\n"
      "call ASM_check_mouse_over_face\n"
        : : "a" (pt1), "d" (pt2), "b" (pt3), "c" (face), "g" (type));
}

void draw_sort_sprite1a(ushort sspr)
{
#if 0
    asm volatile (
      "call ASM_draw_sort_sprite1a\n"
        : : "a" (a1));
    return;
#endif
    struct SortSprite *p_sspr;
    struct Thing *p_thing;
    PlayerInfo *p_locplayer;

    p_sspr = &game_sort_sprites[sspr];
    p_thing = p_sspr->PThing;
    p_locplayer = &players[local_player_no];

    word_1A5834 = 120;
    word_1A5836 = 120;
    draw_sorted_sprite1a(p_sspr->Frame, p_sspr->X, p_sspr->Y, p_sspr->Brightness);
    p_thing = game_sort_sprites[sspr].PThing;

    if ((p_locplayer->TargetType <= 5) && (p_thing->Type == SmTT_DROPPED_ITEM)) {
        check_mouse_overlap_item(sspr);
    }

    if ((p_locplayer->TargetType < 6) && (p_thing->Type == TT_MINE))
    {
        if ((p_thing->SubType == 7) || (p_thing->SubType == 3))
            check_mouse_overlap_item(sspr);
        else if (p_thing->SubType == 48)
            check_mouse_overlap(sspr);
    }
}

void draw_ex_face(ushort exface)
{
#if 0
    asm volatile (
      "call ASM_draw_ex_face\n"
        : : "a" (exface));
    return;
#endif
    struct ExplodeFace3 *p_exface;
    struct PolyPoint point2;
    struct PolyPoint point3;
    struct PolyPoint point1;
    struct PolyPoint point4;
    struct SingleFloorTexture *p_sftex;
    struct SingleTexture *p_stex;
    struct SpecialPoint *p_scrpoint;

    p_exface = &ex_faces[exface];
    p_scrpoint = &game_screen_point_pool[p_exface->PointOffset];
    vec_colour = p_exface->Col;
    vec_mode = p_exface->Flags;

    switch (p_exface->Type)
    {
    case 1:
    case 3:
    case 5:
      p_stex = &game_face_textures[p_exface->Texture];
      vec_map = vec_tmap[p_stex->Page];

      point1.U = p_stex->TMapX1 << 16;
      point1.V = p_stex->TMapY1 << 16;
      point2.U = p_stex->TMapX3 << 16;
      point2.V = p_stex->TMapY3 << 16;
      point3.U = p_stex->TMapX2 << 16;
      point3.V = p_stex->TMapY2 << 16;

      point1.X = p_scrpoint[0].X;
      point1.Y = p_scrpoint[0].Y;
      point1.S = 0x100000;

      point3.X = p_scrpoint[1].X;
      point3.Y = p_scrpoint[1].Y;
      point3.S = 0x100000;

      point2.X = p_scrpoint[2].X;
      point2.Y = p_scrpoint[2].Y;
      point2.S = 0x100000;

      if (vec_mode == 2)
          vec_mode = 27;
      draw_trigpoly(&point1, &point2, &point3);

      if (vec_mode == 2)
          vec_mode = 27;
      draw_trigpoly(&point3, &point2, &point1);
      dword_176D4C++;
      break;
    case 2:
    case 4:
    case 6:
      p_sftex = &game_textures[p_exface->Texture];
      vec_map = vec_tmap[p_sftex->Page];

      point1.U = p_sftex->TMapX1 << 16;
      point1.V = p_sftex->TMapY1 << 16;
      point2.U = p_sftex->TMapX2 << 16;
      point2.V = p_sftex->TMapY2 << 16;
      point3.U = p_sftex->TMapX3 << 16;
      point3.V = p_sftex->TMapY3 << 16;
      point4.U = p_sftex->TMapX4 << 16;
      point4.V = p_sftex->TMapY4 << 16;

      point1.X = p_scrpoint[0].X;
      point1.Y = p_scrpoint[0].Y;
      point1.S = 0x100000;

      point2.X = p_scrpoint[1].X;
      point2.Y = p_scrpoint[1].Y;
      point2.S = 0x100000;

      point3.X = p_scrpoint[2].X;
      point3.Y = p_scrpoint[2].Y;
      point3.S = 0x100000;

      point4.X = p_scrpoint[3].X;
      point4.Y = p_scrpoint[3].Y;
      point4.S = 0x100000;

      if (vec_mode == 2)
          vec_mode = 27;
      draw_trigpoly(&point1, &point2, &point3);

      if (vec_mode == 2)
          vec_mode = 27;
      draw_trigpoly(&point3, &point2, &point1);

      if (vec_mode == 2)
          vec_mode = 27;
      draw_trigpoly(&point2, &point3, &point4);

      if (vec_mode == 2)
          vec_mode = 27;
      draw_trigpoly(&point4, &point3, &point2);
      dword_176D4C++;
      break;
    default:
      break;
    }
}

void set_floor_tile_point_uv_map_a(struct PolyPoint *p_pt1, struct PolyPoint *p_pt2, struct PolyPoint *p_pt3, struct PolyPoint *p_pt4, ubyte page)
{
    switch (page)
    {
    case 1:
        p_pt1->U = 0xA00000;
        p_pt1->V = 0x400000;
        p_pt2->U = 0xA00000;
        p_pt3->U = 0xBF0000;
        p_pt4->U = 0xBF0000;
        p_pt2->V = 0x5F0000;
        p_pt4->V = 0x5F0000;
        p_pt3->V = 0x400000;
        break;
    case 2:
        p_pt1->V = 0x400000;
        p_pt2->V = 0x5F0000;
        p_pt3->U = 0x800000;
        p_pt3->V = 0x400000;
        p_pt4->U = 0x800000;
        p_pt1->U = 0x9F0000;
        p_pt2->U = 0x9F0000;
        p_pt4->V = 0x5F0000;
        break;
    case 3:
        p_pt1->U = 0xDF0000;
        p_pt1->V = 0x400000;
        p_pt2->U = 0xDF0000;
        p_pt2->V = 0x5F0000;
        p_pt3->U = 0xC00000;
        p_pt4->U = 0xC00000;
        p_pt4->V = 0x5F0000;
        p_pt3->V = 0x400000;
        break;
    case 4:
        p_pt3->V = 0x400000;
        p_pt1->V = 0x5F0000;
        p_pt4->U = 0xBF0000;
        p_pt4->V = 0x400000;
        p_pt2->U = 0xBF0000;
        p_pt3->U = 0xA00000;
        p_pt1->U = 0xA00000;
        p_pt2->V = 0x5F0000;
        break;
    case 5:
        p_pt1->U = 0x9F0000;
        p_pt1->V = 0x5F0000;
        p_pt2->U = 0x9F0000;
        p_pt3->U = 0x800000;
        p_pt4->U = 0x800000;
        p_pt2->V = 0x400000;
        p_pt4->V = 0x400000;
        p_pt3->V = 0x5F0000;
        break;
    case 6:
        p_pt2->V = 0x400000;
        p_pt3->U = 0xC00000;
        p_pt4->U = 0xC00000;
        p_pt1->U = 0xDF0000;
        p_pt1->V = 0x5F0000;
        p_pt2->U = 0xDF0000;
        p_pt3->V = 0x5F0000;
        p_pt4->V = 0x400000;
        break;
    case 7:
        p_pt4->U = 0xA00000;
        p_pt4->V = 0x400000;
        p_pt3->U = 0xA00000;
        p_pt2->U = 0xBF0000;
        p_pt1->U = 0xBF0000;
        p_pt3->V = 0x5F0000;
        p_pt1->V = 0x5F0000;
        p_pt2->V = 0x400000;
        break;
    case 8:
        p_pt1->V = 0x5F0000;
        p_pt2->V = 0x400000;
        p_pt3->U = 0x9F0000;
        p_pt3->V = 0x5F0000;
        p_pt4->U = 0x9F0000;
        p_pt1->U = 0x800000;
        p_pt2->U = 0x800000;
        p_pt4->V = 0x400000;
        break;
    case 9:
        p_pt1->U = 0xC00000;
        p_pt1->V = 0x5F0000;
        p_pt2->U = 0xC00000;
        p_pt2->V = 0x400000;
        p_pt3->U = 0xDF0000;
        p_pt4->U = 0xDF0000;
        p_pt4->V = 0x400000;
        p_pt3->V = 0x5F0000;
        break;
    case 10:
        p_pt2->V = 0x400000;
        p_pt4->V = 0x5F0000;
        p_pt1->U = 0xBF0000;
        p_pt1->V = 0x400000;
        p_pt3->U = 0xBF0000;
        p_pt2->U = 0xA00000;
        p_pt4->U = 0xA00000;
        p_pt3->V = 0x5F0000;
        break;
    case 11:
        p_pt1->U = 0x800000;
        p_pt1->V = 0x400000;
        p_pt2->U = 0x800000;
        p_pt3->U = 0x9F0000;
        p_pt4->U = 0x9F0000;
        p_pt2->V = 0x5F0000;
        p_pt4->V = 0x5F0000;
        p_pt3->V = 0x400000;
        break;
    case 12:
        p_pt2->V = 0x5F0000;
        p_pt3->U = 0xDF0000;
        p_pt4->U = 0xDF0000;
        p_pt1->U = 0xC00000;
        p_pt1->V = 0x400000;
        p_pt2->U = 0xC00000;
        p_pt3->V = 0x400000;
        p_pt4->V = 0x5F0000;
        break;
    }
}

void set_floor_tile_point_uv_map_b(struct PolyPoint *p_pt1, struct PolyPoint *p_pt2, struct PolyPoint *p_pt3, struct PolyPoint *p_pt4, ubyte page)
{
    //TODO check if it's the same as this:
    //set_floor_tile_point_uv_map_a(p_pt3, p_pt1, p_pt4, p_pt2, ((page+8) % 12) + 1);
    switch (page)
    {
    case 1:
        p_pt1->V = 0x400000;
        p_pt2->V = 0x5F0000;
        p_pt3->U = 0xBF0000;
        p_pt3->V = 0x400000;
        p_pt4->U = 0xBF0000;
        p_pt1->U = 0xA00000;
        p_pt2->U = 0xA00000;
        p_pt4->V = 0x5F0000;
        break;
    case 2:
        p_pt1->U = 0x9F0000;
        p_pt2->U = 0x9F0000;
        p_pt2->V = 0x5F0000;
        p_pt3->U = 0x800000;
        p_pt4->V = 0x5F0000;
        p_pt1->V = 0x400000;
        p_pt3->V = 0x400000;
        p_pt4->U = 0x800000;
        break;
    case 3:
        p_pt1->U = 0xDF0000;
        p_pt1->V = 0x400000;
        p_pt2->U = 0xDF0000;
        p_pt3->U = 0xC00000;
        p_pt4->U = 0xC00000;
        p_pt2->V = 0x5F0000;
        p_pt4->V = 0x5F0000;
        p_pt3->V = 0x400000;
        break;
    case 4:
        p_pt3->V = 0x400000;
        p_pt1->V = 0x5F0000;
        p_pt4->U = 0xBF0000;
        p_pt4->V = 0x400000;
        p_pt2->U = 0xBF0000;
        p_pt3->U = 0xA00000;
        p_pt1->U = 0xA00000;
        p_pt2->V = 0x5F0000;
        break;
    case 5:
        p_pt2->V = 0x400000;
        p_pt3->U = 0x800000;
        p_pt4->U = 0x800000;
        p_pt1->U = 0x9F0000;
        p_pt1->V = 0x5F0000;
        p_pt2->U = 0x9F0000;
        p_pt3->V = 0x5F0000;
        p_pt4->V = 0x400000;
        break;
    case 6:
        p_pt1->U = 0xDF0000;
        p_pt1->V = 0x5F0000;
        p_pt2->U = 0xDF0000;
        p_pt2->V = 0x400000;
        p_pt3->U = 0xC00000;
        p_pt4->U = 0xC00000;
        p_pt4->V = 0x400000;
        p_pt3->V = 0x5F0000;
        break;
    case 7:
        p_pt4->V = 0x400000;
        p_pt3->V = 0x5F0000;
        p_pt2->V = 0x400000;
        p_pt1->V = 0x5F0000;
        p_pt4->U = 0xA00000;
        p_pt2->U = 0xBF0000;
        p_pt1->U = 0xBF0000;
        p_pt3->U = 0xA00000;
        break;
    case 8:
        p_pt1->U = 0x800000;
        p_pt2->U = 0x800000;
        p_pt2->V = 0x400000;
        p_pt3->U = 0x9F0000;
        p_pt4->U = 0x9F0000;
        p_pt1->V = 0x5F0000;
        p_pt3->V = 0x5F0000;
        p_pt4->V = 0x400000;
        break;
    case 9:
        p_pt1->U = 0xC00000;
        p_pt2->U = 0xC00000;
        p_pt2->V = 0x400000;
        p_pt3->U = 0xDF0000;
        p_pt4->V = 0x400000;
        p_pt1->V = 0x5F0000;
        p_pt3->V = 0x5F0000;
        p_pt4->U = 0xDF0000;
        break;
    case 10:
        p_pt2->U = 0xA00000;
        p_pt2->V = 0x400000;
        p_pt4->U = 0xA00000;
        p_pt1->U = 0xBF0000;
        p_pt3->U = 0xBF0000;
        p_pt4->V = 0x5F0000;
        p_pt3->V = 0x5F0000;
        p_pt1->V = 0x400000;
        break;
    case 11:
        p_pt1->U = 0x800000;
        p_pt2->U = 0x800000;
        p_pt2->V = 0x5F0000;
        p_pt3->U = 0x9F0000;
        p_pt4->U = 0x9F0000;
        p_pt4->V = 0x5F0000;
        p_pt1->V = 0x400000;
        p_pt3->V = 0x400000;
        break;
    case 12:
        p_pt1->V = 0x400000;
        p_pt2->V = 0x5F0000;
        p_pt3->U = 0xDF0000;
        p_pt3->V = 0x400000;
        p_pt4->U = 0xDF0000;
        p_pt1->U = 0xC00000;
        p_pt2->U = 0xC00000;
        p_pt4->V = 0x5F0000;
        break;
    }
}

void draw_floor_tile1a(ushort tl)
{
#if 0
    asm volatile (
      "call ASM_draw_floor_tile1a\n"
        : : "a" (tl));
    return;
#endif
    struct FloorTile *p_floortl;
    struct PolyPoint point3;
    struct PolyPoint point1;
    struct PolyPoint point2;
    struct PolyPoint point4;
    int dist;

    p_floortl = &game_floor_tiles[tl];
    vec_colour = p_floortl->Col;
    vec_mode = p_floortl->Flags;
    if ((p_floortl->Flags == 5) || (p_floortl->Flags == 21))
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = p_floortl->Texture;
        if (byte_19EC6F) {
            if (current_map == 11) // map011 Orbital Station
                vec_mode = 6;
        } else {
            if (p_floortl->Flags == 5)
                vec_mode = 2;
            else
                vec_mode = 19;
        }
        vec_map = vec_tmap[p_sftex->Page];
        point2.U = p_sftex->TMapX1 << 16;
        point2.V = p_sftex->TMapY1 << 16;
        point3.U = p_sftex->TMapX3 << 16;
        point3.V = p_sftex->TMapY3 << 16;
        point1.U = p_sftex->TMapX4 << 16;
        point1.V = p_sftex->TMapY4 << 16;
        point4.U = p_sftex->TMapX2 << 16;
        point4.V = p_sftex->TMapY2 << 16;
    }
    point1.X = p_floortl->X[0];
    point1.Y = p_floortl->Y[0];
    point1.S = p_floortl->Shade[0] << 7;
    point2.X = p_floortl->X[3];
    point2.Y = p_floortl->Y[3];
    point2.S = p_floortl->Shade[3] << 7;
    point3.X = p_floortl->X[1];
    point3.Y = p_floortl->Y[1];
    point3.S = p_floortl->Shade[1] << 7;
    point4.X = p_floortl->X[2];
    point4.Y = p_floortl->Y[2];
    point4.S = p_floortl->Shade[2] << 7;

    if (game_perspective == 7) {
        vec_mode = 7;
        vec_colour = point3.S >> 16;
    }

    dist = (point3.Y - point2.Y) * (point2.X - point1.X)
       - (point3.X - point2.X) * (point2.Y - point1.Y);
    if (dist > 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }

    dist = (point2.X - point3.X) * (point4.Y - point2.Y)
       - (point2.Y - point3.Y) * (point4.X - point2.X);
    if (dist > 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point3, &point2, &point4);
    }
    dword_176D4C += 2;

    // damage overlays
    if ((p_floortl->Page > 0) && (p_floortl->Page <= 12))
    {
        vec_map = vec_tmap[4];
        vec_mode = 6;
        set_floor_tile_point_uv_map_b(&point1, &point2, &point3, &point4, p_floortl->Page);
        draw_trigpoly(&point1, &point2, &point3);
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point3, &point2, &point4);
        dword_176D4C += 2;
    }
}


void draw_floor_tile1b(ushort tl)
{
#if 0
    asm volatile (
      "call ASM_draw_floor_tile1b\n"
        : : "a" (tl));
    return;
#endif
    struct FloorTile *p_floortl;
    struct PolyPoint point2;
    struct PolyPoint point1;
    struct PolyPoint point4;
    struct PolyPoint point3;
    int dist;

    p_floortl = &game_floor_tiles[tl];
    vec_colour = p_floortl->Col;
    vec_mode = p_floortl->Flags;
    if ((p_floortl->Flags == 5) || (p_floortl->Flags == 21))
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = p_floortl->Texture;
        if (byte_19EC6F) {
            if (current_map == 11) // map011 Orbital Station
                vec_mode = 6;
        }
        else {
            if (p_floortl->Flags == 5)
                vec_mode = 2;
            else
                vec_mode = 19;
        }
        vec_map = vec_tmap[p_sftex->Page];
        point2.U = p_sftex->TMapX1 << 16;
        point2.V = p_sftex->TMapY1 << 16;
        point3.U = p_sftex->TMapX3 << 16;
        point3.V = p_sftex->TMapY3 << 16;
        point1.U = p_sftex->TMapX4 << 16;
        point1.V = p_sftex->TMapY4 << 16;
        point4.U = p_sftex->TMapX2 << 16;
        point4.V = p_sftex->TMapY2 << 16;
    }
    point1.X = p_floortl->X[0];
    point1.Y = p_floortl->Y[0];
    point1.S = p_floortl->Shade[0] << 7;
    point2.X = p_floortl->X[3];
    point2.Y = p_floortl->Y[3];
    point2.S = p_floortl->Shade[3] << 7;
    point3.X = p_floortl->X[1];
    point3.Y = p_floortl->Y[1];
    point3.S = p_floortl->Shade[1] << 7;
    point4.X = p_floortl->X[2];
    point4.Y = p_floortl->Y[2];
    point4.S = p_floortl->Shade[2] << 7;

    if (game_perspective == 7) {
        vec_mode = 7;
        vec_colour = point3.S << 16;
    }

    dist = (point2.X - point1.X) * (point4.Y - point2.Y)
       - (point2.Y - point1.Y) * (point4.X - point2.X);
    if (dist > 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point4);
    }
    dist = (point3.X - point4.X) * (point1.Y - point3.Y)
       - (point3.Y - point4.Y) * (point1.X - point3.X);
    if (dist > 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point1);
    }
    dword_176D4C += 2;

    // damage overlays
    if ((p_floortl->Page > 0) && (p_floortl->Page <= 12))
    {
        vec_map = vec_tmap[4];
        vec_mode = 6;
        set_floor_tile_point_uv_map_a(&point1, &point2, &point3, &point4, p_floortl->Page);
        draw_trigpoly(&point1, &point2, &point4);
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point1);
        dword_176D4C += 2;
    }
    //TODO why the second time using the same page?
    if ((p_floortl->Page > 0) && (p_floortl->Page <= 12))
    {
        vec_map = vec_tmap[4];
        vec_mode = 6;
        set_floor_tile_point_uv_map_b(&point1, &point2, &point3, &point4, p_floortl->Page);
        draw_trigpoly(&point1, &point2, &point4);
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point1);
        dword_176D4C += 2;
    }
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
            if ((p_face->GFlags & FGFlg_Unkn02) != 0)
                vec_map = scratch_buf1;
            if ((p_face->GFlags & FGFlg_Unkn40) != 0) {
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

    if ((p_face->GFlags & FGFlg_Unkn01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
        dword_176D4C++;
    }
}

void draw_object_face4d_textrd_dk(ushort face4)
{
#if 0
    asm volatile (
      "call ASM_draw_object_face4d_textrd_dk\n"
        : : "a" (face4));
    return;
#endif
    struct SingleObjectFace4 *p_face4;
    struct PolyPoint point4;
    struct PolyPoint point1;
    struct PolyPoint point2;
    struct PolyPoint point3;

    p_face4 = &game_object_faces4[face4];
    vec_mode = 4;
    vec_colour = 64;

    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        vec_map = vec_tmap[p_sftex->Page];
        if (p_face4->GFlags != 0)
        {
            if ((p_face4->GFlags & FGFlg_Unkn02) != 0)
                vec_map = scratch_buf1;
        }
        point1.U = p_sftex->TMapX1 << 16;
        point1.V = p_sftex->TMapY1 << 16;
        point2.U = p_sftex->TMapX3 << 16;
        point2.V = p_sftex->TMapY3 << 16;
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
        point2.S = 0x200000;
    }
    else
    {
        ushort shade;

        shade = p_face4->Shade0 << 7;
        shade += cummulate_shade_from_quick_lights(p_face4->Light0);
        if (shade > 0x7E00)
            shade = 0x7F00;
        point1.S = shade << 7;
    }
    point1.S = calculate_enginepoint_shade_2(&point1, p_face4, 0);

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[2]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point2.X = p_scrpoint->X + dword_176D00;
        point2.Y = p_scrpoint->Y + dword_176D04;
    }
    if (vec_mode == 2)
    {
        point2.S = 0x200000;
    }
    else
    {
        ushort shade;

        shade = p_face4->Shade2 << 7;
        shade += cummulate_shade_from_quick_lights(p_face4->Light2);
        if (shade > 0x7E00)
            shade = 0x7F00;
        point2.S = shade << 7;
    }
    point2.S = calculate_enginepoint_shade_2(&point2, p_face4, 2);

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[1]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point3.X = p_scrpoint->X + dword_176D00;
        point3.Y = p_scrpoint->Y + dword_176D04;
    }
    if (vec_mode == 2)
    {
        point3.S = 0x200000;
    }
    else
    {
        ushort shade;

        shade = p_face4->Shade1 << 7;
        shade += cummulate_shade_from_quick_lights(p_face4->Light1);
        if (shade > 0x7E00)
            shade = 0x7F00;
        point3.S = shade << 7;
    }
    point3.S = calculate_enginepoint_shade_2(&point2, p_face4, 1); //TODO why point2? is that a coding mistake?

    {
        struct SinglePoint *p_point;
        struct SpecialPoint *p_scrpoint;

        p_point = &game_object_points[p_face4->PointNo[3]];
        p_scrpoint = &game_screen_point_pool[p_point->PointOffset];
        point4.X = p_scrpoint->X + dword_176D00;
        point4.Y = p_scrpoint->Y + dword_176D04;
    }
    if (vec_mode == 2)
    {
        point4.S = 0x200000;
    }
    else
    {
        ushort shade;

        shade = p_face4->Shade3 << 7;
        shade += cummulate_shade_from_quick_lights(p_face4->Light3);
        if (shade > 0x7E00)
            shade = 0x7F00;
        point4.S = shade << 7;
    }
    point4.S = calculate_enginepoint_shade_2(&point4, p_face4, 3);

    if (byte_19EC6F == 0)
    {
        point1.S = 0x200000;
        point2.S = 0x200000;
        point3.S = 0x200000;
        point4.S = 0x200000;
    }

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point2, &point3);
    }
    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }
    dword_176D4C++;

    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        point4.U = p_sftex->TMapX4 << 16;
        point4.V = p_sftex->TMapY4 << 16;
        point2.U = p_sftex->TMapX3 << 16;
        point2.V = p_sftex->TMapY3 << 16;
        point3.U = p_sftex->TMapX2 << 16;
        point3.V = p_sftex->TMapY2 << 16;
    }

    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point2);
    }
    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point2, &point3);
    }
    dword_176D4C++;
}

void draw_sort_line(struct SortLine *p_sline)
{
#if 0
    asm volatile (
      "call ASM_draw_sort_line\n"
        : : "a" (p_sline));
    return;
#endif
    ushort ftcor;
    if ((p_sline->Flags & (0x01|0x02)) != 0)
    {
        int dist_x, dist_y;
        int dtX, dtY;

        dist_x = p_sline->X1 - p_sline->X2;
        dist_y = p_sline->Y1 - p_sline->Y2;
        if (dist_x < 0)
            dist_x = -dist_x;
        if (dist_y < 0)
            dist_y = -dist_y;
        if (dist_x <= dist_y) {
          dtX = 1;
          dtY = 0;
        } else {
          dtX = 0;
          dtY = 1;
        }

        if ((p_sline->Flags & 0x02) != 0)
            lbDisplay.DrawFlags = 0x0004;
        ftcor = 256 * p_sline->Shade + p_sline->Col;
        LbDrawLine(p_sline->X1, p_sline->Y1,
          p_sline->X2, p_sline->Y2,
          pixmap.fade_table[ftcor]);

        lbDisplay.DrawFlags = 0x0004;
        ftcor = 256 * 20 + p_sline->Col;
        LbDrawLine(p_sline->X1 + dtX, p_sline->Y1 + dtY,
          p_sline->X2 + dtX, p_sline->Y2 + dtY,
          pixmap.fade_table[ftcor]);

        ftcor = 256 * 20 + p_sline->Col;
        LbDrawLine(p_sline->X1 - dtX, p_sline->Y1 - dtY,
          p_sline->X2 - dtX, p_sline->Y2 - dtY,
          pixmap.fade_table[ftcor]);
        lbDisplay.DrawFlags = 0;
    }
    else
    {
        ftcor = 256 * p_sline->Shade + p_sline->Col;
        LbDrawLine(p_sline->X1, p_sline->Y1,
          p_sline->X2, p_sline->Y2,
          pixmap.fade_table[ftcor]);
    }
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
    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
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
    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point3, &point1);
    }
    dword_176D4C++;
}

void draw_sort_sprite1b(int sspr)
{
#if 0
    asm volatile (
      "call ASM_draw_sort_sprite1b\n"
        : : "a" (sspr));
    return;
#endif
    struct SortSprite *p_sspr;
    struct Thing *p_thing;
    short br_inc;
    ubyte bright;

    p_sspr = &game_sort_sprites[sspr];
    p_thing = p_sspr->PThing;
    if (p_sspr->Frame > 10000)
        return;

    br_inc = 0;
    bright = p_sspr->Brightness;
    if ((p_thing->Flag & TngF_Destroyed) == 0)
    {
        if ((p_thing->Flag & TngF_Unkn00200000) != 0)
        {
            br_inc += 16;
            if (p_thing->U.UPerson.ShieldGlowTimer) {
                br_inc += 16;
            }
        }
    }
    bright += br_inc;
    if ((p_thing->U.UPerson.AnimMode == 12) || ((ingame.Flags & GamF_ThermalView) != 0))
        bright = 32;

    word_1A5834 = 120;
    word_1A5836 = 120;

    if (((p_thing->Flag2 & TgF2_Unkn2000) != 0) && (ingame.DisplayMode == 50))
    {
        if ((ingame.Flags & GamF_ThermalView) != 0) {
            ushort fr;
            fr = nstart_ani[1066];
            draw_sorted_sprite1a(fr, p_sspr->X, p_sspr->Y, 32);
        }
    }
    else
    {
        ubyte *frv;
        if (((p_thing->Flag2 & TgF2_Unkn00080000) != 0) && (p_thing->SubType == 2))
            bright = 32;
        frv = p_thing->U.UPerson.FrameId.Version;
        draw_sorted_sprite1b(frv, p_sspr->Frame, p_sspr->X, p_sspr->Y, bright, p_sspr->Angle);
    }

    if (p_thing->U.UPerson.EffectiveGroup != ingame.MyGroup)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if ((p_thing->Flag & TngF_Destroyed) != 0)
        {
            if (p_locplayer->TargetType < 1)
                check_mouse_overlap_corpse(sspr);
        }
        else
        {
            if (p_locplayer->TargetType < 7)
                check_mouse_overlap(sspr);
        }
    }

    if (in_network_game)
    {
        struct Thing *p_owntng;

        p_owntng = NULL;
        if (((p_thing->Flag & TngF_PlayerAgent) != 0) && (p_thing->U.UPerson.ComCur >> 2 != local_player_no))
        {
            p_owntng = p_thing;
        }
        else if ((p_thing->Flag & TngF_Persuaded) != 0)
        {
            p_owntng = &things[p_thing->Owner];
            if (((p_owntng->Flag & TngF_PlayerAgent) == 0) || (p_owntng->U.UPerson.ComCur >> 2 == local_player_no))
                p_owntng = NULL;
        }
        if ((p_owntng != NULL) && (p_owntng->U.UPerson.CurrentWeapon != 30)) {
            check_mouse_over_unkn2(sspr, p_owntng);
        }
    }

    if (br_inc != 0)
    {
        ubyte *frv;
        ushort fr, k;
        fr = shield_frm[p_thing->ThingOffset & 3];
        k = ((gameturn + 16 * p_thing->ThingOffset) >> 2) & 7;
        frv = byte_15399C + 5 * k;
        draw_sorted_sprite1b(frv, fr, p_sspr->X, p_sspr->Y, br_inc, 0);
    }

    if (debug_hud_collision) {
        char locstr[32];
        short dy;
        sprintf(locstr, "%d ", p_thing->U.UPerson.RecoilTimer);
        dy = (37 * overall_scale) >> 8;
        draw_text(p_sspr->X, p_sspr->Y - dy, locstr, colour_lookup[2]);
    }

    if ((p_thing->Flag2 & TgF2_ExistsOnMap) != 0) {
        short dx, dy;
        dx = (2 * overall_scale) >> 8;
        dy = (37 * overall_scale) >> 8;
        draw_text(p_sspr->X - dx, p_sspr->Y - dy, "E", colour_lookup[2]);
    }
    if ((ingame.DisplayMode != 50) && ((p_thing->Flag2 & TgF2_InsideBuilding) != 0)) {
        short dx, dy;
        dx = (2 * overall_scale) >> 8;
        dy = (37 * overall_scale) >> 8;
        draw_text(p_sspr->X + dx, p_sspr->Y - dy, "B", colour_lookup[2]);
    }
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

void draw_sorted_sprite1a(ushort frm, short x, short y, ubyte bright)
{
#if 1
    asm volatile (
      "call ASM_draw_sorted_sprite1a\n"
        : : "a" (frm), "d" (x), "b" (y), "c" (bright));
    return;
#endif
    int sscale;
    ubyte bri;

    sscale = overall_scale;
    bri = bright;
    if (bri < 10)
        bri = 10;
    if (bri > 48)
        bri = 48;

    if ((sscale == 256) || (sscale <= 0) || (sscale >= 4096))
    {
        draw_unkn3_scaled_alpha_sprite(frm, x, y, bri);
    }
    else
    {
        draw_unkn1_scaled_alpha_sprite(frm, x, y, sscale, bri);
    }
}

void draw_sort_sprite1c_sub(ushort frm, short x, short y, ubyte bright, ushort scale)
{
#if 0
    asm volatile (
      "push %4\n"
      "call ASM_draw_sort_sprite1c_sub\n"
        : : "a" (frm), "d" (x), "b" (y), "c" (bright), "g" (scale));
    return;
#endif
    int sscale;
    ubyte bri;

    sscale = (scale * overall_scale) >> 8;

    bri = bright;
    if (bri < 10)
        bri = 10;
    if (bri > 48)
        bri = 48;

    if (sscale == 256 || sscale == 0 || sscale >= 0x1000)
    {
        draw_unkn3_scaled_alpha_sprite(frm, x, y, bri);
    }
    else
    {
        draw_unkn1_scaled_alpha_sprite(frm, x, y, sscale, bri);
    }
}

void draw_sort_sprite1c(ushort sspr)
{
    struct SortSprite *p_sspr;
    p_sspr = &game_sort_sprites[sspr];
    draw_sort_sprite1c_sub(p_sspr->Frame, p_sspr->X, p_sspr->Y, p_sspr->Brightness, p_sspr->Scale);
}

void draw_sort_line1a(ushort sln)
{
    struct SortLine *p_sline;
    p_sline = &game_sort_lines[sln];
    draw_sort_line(p_sline);
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
            if ((p_face->GFlags & FGFlg_Unkn02) != 0)
                vec_map = scratch_buf1;
            if ((p_face->GFlags & FGFlg_Unkn40) != 0) {
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
        ushort shade;

        shade = p_face->Shade0 << 7;
        shade += cummulate_shade_from_quick_lights(p_face->Light0);
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
            ushort shade;

            shade = p_face->Shade2 << 7;
            shade += cummulate_shade_from_quick_lights(p_face->Light2);
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
            ushort shade;

            shade = p_face->Shade1 << 7;
            shade += cummulate_shade_from_quick_lights(p_face->Light1);
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
        if ((p_face->GFlags & FGFlg_Unkn01) != 0)
        {
            if (vec_mode == 2)
                vec_mode = 27;
            draw_trigpoly(&point1, &point3, &point2);
            dword_176D4C++;
        }
    }

    if ((p_face->GFlags & FGFlg_Unkn04) != 0)
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
            if ((p_face4->GFlags & FGFlg_Unkn02) != 0)
                vec_map = scratch_buf1;
            if ((p_face4->GFlags & FGFlg_Unkn40) != 0) {
                uint frame;
                frame = gameturn + p_face4->Object;
                if ((frame & 0x1FF) > 0x100 && !byte_153014[frame & 0x3F])
                    vec_mode = 5;
            }
        }
        if ((p_face4->GFlags & FGFlg_Unkn20) != 0) {
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
        ushort shade;

        shade = p_face4->Shade0 << 7;
        shade += cummulate_shade_from_quick_lights(p_face4->Light0);
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
            ushort shade;

            shade = p_face4->Shade2 << 7;
            shade += cummulate_shade_from_quick_lights(p_face4->Light2);
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
            ushort shade;

            shade = p_face4->Shade1 << 7;
            shade += cummulate_shade_from_quick_lights(p_face4->Light1);
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
            ushort shade;

            shade = p_face4->Shade3 << 7;
            shade += cummulate_shade_from_quick_lights(p_face4->Light3);
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

    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }

    if ((p_face4->GFlags & FGFlg_Unkn04) != 0)
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
        if ((p_face4->GFlags & FGFlg_Unkn20) != 0) {
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

    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point4, &point2, &point3);
    }

    if ((p_face4->GFlags & FGFlg_Unkn04) != 0)
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
            if ((p_face4->GFlags & FGFlg_Unkn02) != 0)
                vec_map = scratch_buf1;
            if ((p_face4->GFlags & (FGFlg_Unkn40|FGFlg_Unkn02)) != 0) {
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
        if ((p_face4->GFlags & FGFlg_Unkn20) != 0) {
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

    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
    {
        if (vec_mode == 2)
            vec_mode = 27;
        draw_trigpoly(&point1, &point3, &point2);
    }

    if (p_face4->Texture != 0)
    {
        struct SingleFloorTexture *p_sftex;

        p_sftex = &game_textures[p_face4->Texture];
        if ((p_face4->GFlags & FGFlg_Unkn20) != 0) {
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

    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
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

    if ((p_face->GFlags & FGFlg_Unkn01) != 0)
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

    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
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

    if ((p_face4->GFlags & FGFlg_Unkn01) != 0)
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
            if (lbDisplay.GraphicsScreenHeight < 400)
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

    if ((p_face4->GFlags & FGFlg_Unkn04) != 0)
    {
        PlayerInfo *p_locplayer;

        p_locplayer = &players[local_player_no];
        if (p_locplayer->TargetType < 3) {
            check_mouse_over_face(&point1, &point2, &point3, face4, 2);
        }
    }

    poly_line(&point4, &point3);
    poly_line(&point4, &point2);

    if ((p_face4->GFlags & FGFlg_Unkn04) != 0)
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

    if ((p_face->GFlags & FGFlg_Unkn04) != 0)
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

void draw_sort_sprite_number(ushort sspr)
{
    char locstr[50];
    struct SortSprite *p_sspr;

    p_sspr = &game_sort_sprites[sspr];
    sprintf(locstr, "%d", (int)p_sspr->PThing);
    draw_text(p_sspr->X, p_sspr->Y, locstr, colour_lookup[2]);
}

void number_player(struct Thing *p_person, ubyte n)
{
#if 0
    asm volatile ("call ASM_number_player\n"
        : : "a" (p_person), "d" (n));
    return;
#endif
    struct ShEnginePoint sp;
    struct Frame *p_frm;
    struct Element *p_el;
    int cor_x, cor_y, cor_z;
    int shift_x, shift_y;
    ushort ani_mdsh, ani_base;
    ushort frm;

    if (lbDisplay.GraphicsScreenHeight < 400)
        ani_mdsh = 0;
    else
        ani_mdsh = 4;
    if (byte_1DB2E9 == 1)
        ani_base = 1528;
    else
        ani_base = 1520;

    frm = nstart_ani[ani_base + ani_mdsh + n];
    {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];
        if (!p_locplayer->DoubleMode)
        {
            if ((p_person->ThingOffset != (ThingIdx)p_locplayer->DirectControl[0]) || ((gameturn & 4) != 0))
            {
                frm = frame[frm].Next;
            }
            else
            {
                ushort i;
                for (i = 0; i <= (gameturn & 3); i++)
                    frm = frame[frm].Next;
            }
        }
    }

    if ((p_person->Flag2 & TgF2_Unkn0002) != 0)
        return;

    {
        int tng_cor_x, tng_cor_y, tng_cor_z;

        if (((p_person->Flag & TngF_InVehicle) != 0) && things[p_person->U.UPerson.Vehicle].SubType == 29)
        {
            tng_cor_x = p_person->X;
            tng_cor_y = p_person->Y;
            tng_cor_z = p_person->Z;
        }
        else if ((p_person->Flag & TngF_Unkn4000) != 0)
        {
            struct Thing *p_vehicle;
            p_vehicle = &things[p_person->U.UPerson.Vehicle];
            tng_cor_x = p_vehicle->X;
            tng_cor_y = p_vehicle->Y;
            tng_cor_z = p_vehicle->Z;
        }
        else
        {
            tng_cor_x = p_person->X;
            tng_cor_y = p_person->Y;
            tng_cor_z = p_person->Z;
        }
        cor_x = PRCCOORD_TO_MAPCOORD(tng_cor_x) - engn_xc;
        cor_y = (tng_cor_y >> 5) - (engn_yc >> 3);
        cor_z = PRCCOORD_TO_MAPCOORD(tng_cor_z) - engn_zc;
    }
    {
        int cor_lr, cor_sm;
        if (abs(cor_x) <= abs(cor_z)) {
            cor_sm = abs(cor_x);
            cor_lr = abs(cor_z);
        } else {
            cor_sm = abs(cor_z);
            cor_lr = abs(cor_x);
        }
        if (cor_lr + (cor_sm >> 1) > TILE_TO_MAPCOORD(18,0))
            return;
    }

    transform_shpoint(&sp, cor_x, cor_y - 8 * engn_yc, cor_z);

    if ((p_person->Flag & TngF_InVehicle) != 0)
    {
        shift_x = 0;
        sp.X += 7 * n - 14;
    }
    else
    {
        shift_x = -lbSinTable[256 * ((p_person->U.UObject.Angle + 2 - byte_176D49 + 8) & 7) + 512] >> 14;
        if ((p_person->Flag2 & TgF2_Unkn00080000) == 0)
            shift_x = -lbSinTable[256 * ((p_person->U.UObject.Angle + 2 - byte_176D49 + 8) & 7) + 512] >> 15;
    }
    shift_y = 0;

    p_frm = &frame[frm];
    for (p_el = &melement_ani[p_frm->FirstElement]; p_el > melement_ani; p_el = &melement_ani[p_el->Next])
    {
        struct TbSprite *p_spr;
        short x, y;

        p_spr = (struct TbSprite *)((ubyte *)m_sprites + p_el->ToSprite);
        if (p_spr <= m_sprites)
           continue;

        lbDisplay.DrawFlags = p_el->Flags & 7;
        if ((p_el->Flags & 0xFE00) != 0)
            continue;

        x = sp.X + ((overall_scale * (p_el->X + shift_x)) >> 9);
        y = sp.Y + ((overall_scale * (p_el->Y + shift_y)) >> 9);
        LbSpriteDraw(x, y, p_spr);
    }
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
          draw_sort_line1a(itm->Offset);
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
          draw_sort_line1a(itm->Offset);
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
