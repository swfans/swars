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

#include <assert.h>
#include "bfendian.h"
#include "bfmath.h"
#include "bfutility.h"

#include "bigmap.h"
#include "display.h"
#include "enginbckt.h"
#include "engindrwlstx.h"
#include "enginfloor.h"
#include "enginsngobjs.h"
#include "enginshrapn.h"
#include "engintrns.h"
#include "game.h"
#include "game_data.h"
#include "game_speed.h"
#include "matrix.h"
#include "swlog.h"
#include "thing.h"
#include "vehicle.h"
/******************************************************************************/
extern ubyte byte_176D49;

extern long dword_176CAC;
extern long dword_176CB0;
extern long dword_176D68;
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

/** Add a new draw item and return linked FloorTile instance.
 *
 * @param ditype Draw item type, should be one of FloorTile related types.
 * @param bckt Destination bucket for this draw item.
 * @return FloorTile instance to fill, or NULL if arrays exceeded.
 */
struct FloorTile *draw_item_add_floor_tile(ubyte ditype, ushort bckt)
{
    struct FloorTile *p_floortl;

    if (next_floor_tile >= mem_game[26].N)
        return NULL;

    p_floortl = &game_floor_tiles[next_floor_tile];
    if (!draw_item_add(ditype, next_floor_tile, bckt))
        return NULL;

    next_floor_tile++;

    return p_floortl;
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
    struct ShEnginePoint sp1, sp3;
    struct SortLine *p_sline;
    int dt_x, dt_y, dt_z;
    int dist;
    int x3, y3, z3;

    dt_y = y2 - y1;
    dt_z = z2 - z1;
    dt_x = x2 - x1;

    dist = LbSqrL(dt_z * dt_z + dt_x * dt_x + dt_y * dt_y);
    if (dist == 0)
        dist = 1;
    y3 = y1 + dt_y * len / dist;
    z3 = z1 + dt_z * len / dist;
    x3 = x1 + dt_x * len / dist;

    transform_shpoint(&sp1, x1, 8 * y1 - 8 * engn_yc, z1);
    transform_shpoint(&sp3, x3, 8 * y3 - 8 * engn_yc, z3);

    if ((sp3.Flags & sp1.Flags & 0xF) != 0)
        return;

    p_sline = draw_item_add_line(DrIT_Unkn11, sp1.Depth + 5000);
    if (p_sline == NULL)
        return;

    p_sline->Shade = 32;
    p_sline->Flags = 0;
    p_sline->X1 = sp1.X;
    p_sline->Y1 = sp1.Y;
    p_sline->X2 = sp3.X;
    p_sline->Y2 = sp3.Y;
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

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    transform_shpoint(&sp, x, 8 * y - 8 * engn_yc, z);

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

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    transform_shpoint(&sp, x, 8 * y - 8 * engn_yc, z);

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
    int bckt_shift;
    ubyte bri;

    bri = byte_152EF0[p_thing->SubType] + intensity;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    transform_shpoint(&sp, x, 8 * y - 8 * engn_yc, z);

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

        p_flame = &FIRE_flame[flm];
        x = p_flame->x - engn_xc;
        y = p_flame->y - engn_yc;
        z = p_flame->z - engn_zc;

        if (current_map == 9) // map009 Singapore on-water map
            y += waft_table[gameturn & 0x1F];

        transform_shpoint(&sp, x, y - 8 * engn_yc, z);

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

        p_phwoar = &phwoar[phw];
        x = (p_phwoar->x >> 8) - engn_xc;
        z = (p_phwoar->z >> 8) - engn_zc;
        y = (p_phwoar->y >> 5) - engn_yc;

        transform_shpoint(&sp, x, y - 8 * engn_yc, z);

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

        transform_shpoint(&sp1, x_pcc, y_msc - 8 * engn_yc, z_ms);
        transform_shpoint(&sp2, x_pcs, y_mss - 8 * engn_yc, z);
        transform_shpoint(&sp3, x_mmc, y_pps - 8 * engn_yc, z_ps);

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
        int x, y, z;

        x = (p_shrapnel1->x >> 8) - engn_xc;
        z = (p_shrapnel1->z >> 8) - engn_zc;
        y = (p_shrapnel1->y >> 5) - engn_yc;

        transform_shpoint(&sp1, x, y - 8 * engn_yc, z);

        x = (p_shrapnel2->x >> 8) - engn_xc;
        z = (p_shrapnel2->z >> 8) - engn_zc;
        y = (p_shrapnel2->y >> 5) - engn_yc;

        transform_shpoint(&sp2, x, y - 8 * engn_yc, z);

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

static void transform_rot_object_shpoint(struct ShEnginePoint *p_sp, int offset_x, int offset_y, int offset_z, ushort mat, ushort pt)
{
    struct SinglePoint *p_snpoint;
    struct SpecialPoint *p_specpt;

    p_snpoint = &game_object_points[pt];
    if ((p_snpoint->Flags & 0x40) != 0) // has sub-item alocated in `PointOffset`
    {
        p_specpt = &game_screen_point_pool[p_snpoint->PointOffset];
        p_sp->X = p_specpt->X;
        p_sp->Y = p_specpt->Y;
        p_sp->Depth = p_specpt->Z;
        p_sp->Flags = p_snpoint->Flags;
    }
    else
    {
        struct M31 vec_inp, vec_rot;
        int dxc, dyc, dzc;

        vec_inp.R[0] = 2 * p_snpoint->X;
        vec_inp.R[1] = 2 * p_snpoint->Y;
        vec_inp.R[2] = 2 * p_snpoint->Z;
        matrix_transform(&vec_rot, &local_mats[mat], &vec_inp);

        dxc = offset_x + (vec_rot.R[0] >> 15);
        dyc = offset_y + (vec_rot.R[1] >> 15);
        dzc = offset_z + (vec_rot.R[2] >> 15);
        transform_shpoint(p_sp, dxc, dyc - 8 * engn_yc, dzc);

        p_specpt = &game_screen_point_pool[next_screen_point];
        p_specpt->X = p_sp->X;
        p_specpt->Y = p_sp->Y;
        p_specpt->Z = p_sp->Depth;

        p_snpoint->PointOffset = next_screen_point;
        p_snpoint->Flags = p_sp->Flags;
        next_screen_point++;
    }
}

ushort draw_rot_object(int offset_x, int offset_y, int offset_z, struct SingleObject *point_object, struct Thing *p_thing)
{
#if 0
    ushort ret;
    asm volatile (
      "push %5\n"
      "call ASM_draw_rot_object\n"
        : "=r" (ret) : "a" (offset_x), "d" (offset_y), "b" (offset_z), "c" (point_object), "g" (p_thing));
    return ret;
#endif
    int i, bckt_max;
    int face_beg, face;
    short faces_num;
    ushort faceWH, faceGF;

    bckt_max = 0;

    faceGF = 0;
    if ((p_thing->Type != TT_UNKN35) && (p_thing->SubType != SubTT_VEH_TRAIN))
    {
        short pos_x, pos_z;
        ushort darken;

        // Cannot get absolute map position from p_thing as it might be relative; so get it from the offset
        pos_x = engn_xc + offset_x;
        pos_z = engn_zc + offset_z;

        darken = 9;
        if (pos_x < TILE_TO_MAPCOORD(8, 0))
            darken = min(darken, pos_x >> 7);
        else if (pos_x > TILE_TO_MAPCOORD(MAP_TILE_WIDTH - 8, 0))
            darken = min(darken, (MAP_COORD_WIDTH - pos_x) >> 7);
        else if (pos_z < TILE_TO_MAPCOORD(8, 0))
            darken = min(darken, pos_z >> 7);
        else if (pos_z > TILE_TO_MAPCOORD(MAP_TILE_HEIGHT - 8, 0))
            darken = min(darken, (MAP_COORD_HEIGHT - pos_z) >> 7);

        if (darken < 9)
        {
            if (darken > 7)
                darken = 7;
            if (darken <= 0)
                darken = 1;
            faceGF |= (darken << 2);
        }
    }

    if ((p_thing->Flag & 0x01000000) != 0)
    {
        p_thing->Flag &= ~0x01000000;
        faceWH = 11 - (gameturn & 3);
    }
    else
    {
      faceWH = 0;
    }

    if (current_map == 9) // map009 Singapore on-water map
        offset_y += waft_table[gameturn & 0x1F];

    for (i = point_object->StartPoint; i < point_object->EndPoint; i++)
    {
        struct SinglePoint *p_snpoint;

        p_snpoint = &game_object_points[i];
        p_snpoint->Flags = 0;
    }

    for (i = point_object->OffsetX; i < point_object->OffsetY; i++)
    {
        struct M31 vec_nx, vec_rot;
        struct Normal *p_nrml;
        int fctr_p, fctr_s, fctr_o, fctr_r;

        p_nrml = &game_normals[i];
        vec_nx = *(struct M31 *)&p_nrml->NX;
        matrix_transform(&vec_rot, &local_mats[p_thing->U.UVehicle.MatrixIndex], &vec_nx);

        fctr_o = dword_176D14 * (vec_rot.R[0] >> 14) - dword_176D10 * (vec_rot.R[2] >> 14);
        fctr_p = (dword_176D14 * (vec_rot.R[2] >> 14) + dword_176D10 * (vec_rot.R[0] >> 14)) >> 16;
        fctr_r = dword_176D1C * (vec_rot.R[1] >> 14) - dword_176D18 * fctr_p;
        fctr_s = (dword_176D18 * (vec_rot.R[1] >> 14) + dword_176D1C * fctr_p) >> 16;

        p_nrml->LightRatio = 0;
        p_nrml->LightRatio |= ((fctr_o >> 19) & 0xFF);
        p_nrml->LightRatio |= (((fctr_r >> 19) & 0xFF) << 8);
        p_nrml->LightRatio |= ((fctr_s & 0xFFFF) << 16);
    }

    faces_num = point_object->NumbFaces;
    face_beg = point_object->StartFace;

    face = face_beg;
    for (i = 0; i < faces_num; i++, face++)
    {
        struct ShEnginePoint sp1, sp2, sp3;
        struct SingleObjectFace3 *p_face;
        int depth_max, bckt;

        if (next_screen_point > mem_game[30].N - 5)
            break;

        p_face = &game_object_faces[face];
        p_face->GFlags &= ~0x1C;
        p_face->GFlags |= faceGF;
        p_face->WalkHeader = faceWH;

        transform_rot_object_shpoint(&sp1, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face->PointNo[0]);

        transform_rot_object_shpoint(&sp2, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face->PointNo[2]);

        transform_rot_object_shpoint(&sp3, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face->PointNo[1]);

        depth_max = SHRT_MIN;
        if (depth_max < sp1.Depth)
            depth_max = sp1.Depth;
        if (depth_max < sp2.Depth)
            depth_max = sp2.Depth;
        if (depth_max < sp3.Depth)
            depth_max = sp3.Depth;

        if ((sp1.Flags & sp2.Flags & sp3.Flags & 0xF) != 0)
            continue;
        if ((p_face->GFlags & 0x01) == 0) {
            if  ((sp3.Y - sp2.Y) * (sp2.X - sp1.X) -
              (sp3.X - sp2.X) * (sp2.Y - sp1.Y) <= 0)
                continue;
        }

        ubyte ditype;
        dword_176D68++;
        if ((p_face->GFlags & 0x80u) == 0)
            ditype = 7;
        else
            ditype = 17;
        bckt = depth_max + 5000 - 250;
        if (bckt_max < bckt)
           bckt_max = bckt;
        draw_item_add(ditype, face, bckt);
    }

    faces_num = point_object->NumbFaces4;
    face_beg = point_object->StartFace4;

    face = face_beg;
    for (i = 0; i < faces_num; i++, face++)
    {
        struct ShEnginePoint sp1, sp2, sp3, sp4;
        struct SingleObjectFace4 *p_face4;
        int depth_max, bckt;

        if (next_screen_point > mem_game[30].N - 5)
            break;

        p_face4 = &game_object_faces4[face];
        p_face4->GFlags &= ~0x1C;
        p_face4->GFlags |= faceGF;
        p_face4->WalkHeader = faceWH;

        transform_rot_object_shpoint(&sp1, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[0]);

        transform_rot_object_shpoint(&sp2, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[2]);

        transform_rot_object_shpoint(&sp3, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[1]);

        transform_rot_object_shpoint(&sp4, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[3]);

        depth_max = SHRT_MIN;
        if (depth_max < sp1.Depth)
            depth_max = sp1.Depth;
        if (depth_max < sp2.Depth)
            depth_max = sp2.Depth;
        if (depth_max < sp3.Depth)
            depth_max = sp3.Depth;
        if (depth_max < sp4.Depth)
            depth_max = sp4.Depth;

        if ((sp1.Flags & sp2.Flags & sp3.Flags & sp4.Flags & 0xF) != 0)
            continue;

        if ((p_face4->GFlags & 0x01) == 0) {
            if  ((sp2.X - sp1.X) * (sp3.Y - sp2.Y) -
              (sp2.Y - sp1.Y) * (sp3.X - sp2.X) <= 0)
                continue;
        }

        ubyte ditype;
        if ((p_face4->GFlags & 0x80u) == 0)
            ditype = DrIT_Unkn16;
        else
            ditype = DrIT_Unkn18;
        bckt = depth_max + 5000 - 250;
        if (bckt_max < bckt)
            bckt_max = bckt;
        dword_176D68++;

        draw_item_add(ditype, face, bckt);
    }

    // Plasma jumps when a vehicle got influenced by explosion or is crashing
    if ((p_thing->Type == TT_VEHICLE) && (p_thing->State == VehSt_UNKN_45 ||
      (p_thing->U.UVehicle.WorkPlace & VWPFlg_Unkn0080) != 0))
    {
        if ((LbRandomPosShort() & 0xFF) > 0xE0)
            dword_176CB0 = (LbRandomPosShort() & 0xFF) - 0xD0;

        if (dword_176CB0 && (LbRandomPosShort() & 0xFF) > 0x90)
        {
            int points_num, rnd_range;

            points_num = point_object->EndPoint - point_object->StartPoint;
            dword_176CB0--;
            rnd_range = points_num - 4;
            assert(rnd_range < next_screen_point);

            for (i = 0; i < 10; i++)
            {
              struct SpecialPoint *p_specpt2;
              struct SpecialPoint *p_specpt1;
              int pt1, pt2;

              pt1 = next_screen_point - (((ushort)LbRandomPosShort() % rnd_range) + 1);
              pt2 = next_screen_point - (((ushort)LbRandomPosShort() % rnd_range) + 1);
              p_specpt2 = &game_screen_point_pool[pt2];
              p_specpt1 = &game_screen_point_pool[pt1];

              build_wobble_line(p_specpt1->X, p_specpt1->Y, p_specpt1->Z - 1024,
                p_specpt2->X, p_specpt2->Y, p_specpt2->Z - 1024, 0, 10);
            }
        }
    }
    return bckt_max;
}

ushort draw_rot_object2(int offset_x, int offset_y, int offset_z, struct SingleObject *point_object, struct Thing *p_thing)
{
#if 1
    ushort ret;
    asm volatile (
      "push %5\n"
      "call ASM_draw_rot_object2\n"
        : "=r" (ret) : "a" (offset_x), "d" (offset_y), "b" (offset_z), "c" (point_object), "g" (p_thing));
    return ret;
#endif
    int i, bckt_max;
    int face_beg, face;
    short faces_num;

    bckt_max = 0;

    faces_num = point_object->NumbFaces;
    face_beg = point_object->StartFace;

    face = face_beg;
    for (i = 0; i < faces_num; i++, face++)
    {
        struct SingleObjectFace3 *p_face;
        struct SinglePoint *p_snpoint;

        p_face = &game_object_faces[face];

        p_snpoint = &game_object_points[p_face->PointNo[0]];
        p_snpoint->Flags = 0;
        p_snpoint = &game_object_points[p_face->PointNo[1]];
        p_snpoint->Flags = 0;
        p_snpoint = &game_object_points[p_face->PointNo[2]];
        p_snpoint->Flags = 0;
    }

    face = face_beg;
    for (i = 0; i < faces_num; i++, face++)
    {
        struct ShEnginePoint sp1, sp2, sp3;
        struct SingleObjectFace3 *p_face;
        int depth_max, bckt;

        if (next_screen_point > mem_game[30].N - 5)
            break;

        p_face = &game_object_faces[face];

        transform_rot_object_shpoint(&sp1, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face->PointNo[0]);

        transform_rot_object_shpoint(&sp2, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face->PointNo[2]);

        transform_rot_object_shpoint(&sp3, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face->PointNo[1]);

        depth_max = SHRT_MIN;
        if (depth_max < sp1.Depth)
            depth_max = sp1.Depth;
        if (depth_max < sp2.Depth)
            depth_max = sp2.Depth;
        if (depth_max < sp3.Depth)
            depth_max = sp3.Depth;

        if ((sp1.Flags & sp2.Flags & sp3.Flags & 0xF) != 0)
            continue;
        if ((p_face->GFlags & 0x01) == 0) {
            if  ((sp3.Y - sp2.Y) * (sp2.X - sp1.X) -
              (sp3.X - sp2.X) * (sp2.Y - sp1.Y) <= 0)
                continue;
        }

        ubyte ditype;
        dword_176D68++;
        ditype = 1;
        bckt = depth_max + 5000 - 150;
        if (bckt_max < bckt)
           bckt_max = bckt;
        draw_item_add(ditype, face, bckt);
    }

    faces_num = point_object->NumbFaces4;
    face_beg = point_object->StartFace4;

    face = face_beg;
    for (i = 0; i < faces_num; i++, face++)
    {
        struct SingleObjectFace4 *p_face4;
        struct SinglePoint *p_snpoint;

        p_face4 = &game_object_faces4[face];

        p_snpoint = &game_object_points[p_face4->PointNo[0]];
        p_snpoint->Flags = 0;
        p_snpoint = &game_object_points[p_face4->PointNo[1]];
        p_snpoint->Flags = 0;
        p_snpoint = &game_object_points[p_face4->PointNo[2]];
        p_snpoint->Flags = 0;
        p_snpoint = &game_object_points[p_face4->PointNo[3]];
        p_snpoint->Flags = 0;
    }

    face = face_beg;
    for (i = 0; i < faces_num; i++, face++)
    {
        struct ShEnginePoint sp1, sp2, sp3, sp4;
        struct SingleObjectFace4 *p_face4;
        int depth_max, bckt;

        if (next_screen_point > mem_game[30].N - 5)
            break;

        p_face4 = &game_object_faces4[face];

        transform_rot_object_shpoint(&sp1, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[0]);

        transform_rot_object_shpoint(&sp2, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[2]);

        transform_rot_object_shpoint(&sp3, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[1]);

        transform_rot_object_shpoint(&sp4, offset_x, offset_y, offset_z,
          p_thing->U.UVehicle.MatrixIndex, p_face4->PointNo[3]);

        depth_max = SHRT_MIN;
        if (depth_max < sp1.Depth)
            depth_max = sp1.Depth;
        if (depth_max < sp2.Depth)
            depth_max = sp2.Depth;
        if (depth_max < sp3.Depth)
            depth_max = sp3.Depth;
        if (depth_max < sp4.Depth)
            depth_max = sp4.Depth;

        if ((sp1.Flags & sp2.Flags & sp3.Flags & sp4.Flags & 0xF) != 0)
            continue;

        if ((p_face4->GFlags & 0x01) == 0) {
            if  ((sp2.X - sp1.X) * (sp3.Y - sp2.Y) -
              (sp2.Y - sp1.Y) * (sp3.X - sp2.X) <= 0)
                continue;
        }

        ubyte ditype;
        ditype = DrIT_Unkn9;
        bckt = depth_max + 5000 - 250;
        if (bckt_max < bckt)
            bckt_max = bckt;
        dword_176D68++;

        draw_item_add(ditype, face, bckt);
    }

    return bckt_max;
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
