/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvdraw3d.c
 *     Routines for level and map drawing using 3D rendering.
 * @par Purpose:
 *     Implement functions for creating drawlist for rendering the 3D view.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     24 Dec 2023 - 10 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "lvdraw3d.h"

#include <assert.h>
#include "bfendian.h"
#include "bfmath.h"
#include "bfplanar.h"

#include "bigmap.h"
#include "display.h"
#include "enginbckt.h"
#include "engindrwlstm.h"
#include "engindrwlstx.h"
#include "enginfloor.h"
#include "enginlights.h"
#include "enginsngtxtr.h"
#include "engintrns.h"
#include "enginzoom.h"
#include "game.h"
#include "game_speed.h"
#include "player.h"
#include "scanner.h"
#include "swlog.h"
#include "thing.h"
#include "tngcolisn.h"
#include "tngobjdrw.h"
/******************************************************************************/

struct Range {
    int beg;
    int fin;
};

/******************************************************************************/

extern short super_quick_light[(RENDER_AREA_MAX+1)*(RENDER_AREA_MAX+1)];
extern short word_152F00;

extern long dword_152E50;
extern long dword_152E54;
extern long dword_152E58;

extern long dword_176CC0;

extern short word_19CC64;
extern short word_19CC66;
extern long dword_19F4F8;


int shpoint_compute_coord_y(struct ShEnginePoint *p_sp, struct MyMapElement *p_mapel, int elcr_x, int elcr_z)
{
    int elcr_y;

    if (game_perspective == 1)
    {
        elcr_y = 0;
        p_sp->field_9 = 0;
    }
    else if ((p_mapel->Flags & 0x10) == 0)
    {
        elcr_y = 8 * p_mapel->Alt;
        if ((p_mapel->Flags & 0x40) != 0)
            elcr_y += waft_table[gameturn & 0x1F];
        p_sp->field_9 = 0;
    }
    else
    {
        int wobble, dvfactor;

        elcr_y = 8 * p_mapel->Alt;
        dvfactor = 140 + ((bw_rotl32(0x5D3BA6C3, elcr_z >> 8) ^ bw_rotr32(0xA7B4D8AC, elcr_x >> 8)) & 0x7F);
        wobble = (waft_table2[(gameturn + (elcr_x >> 7)) & 0x1F]
             + waft_table2[(gameturn + (elcr_z >> 7)) & 0x1F]
             + waft_table2[(32 * gameturn / dvfactor) & 0x1F]) >> 3;
        elcr_y += 4 * wobble;
        p_sp->field_9 = (wobble + 32) << 9;
    }
    return elcr_y;
}

short shpoint_compute_shade(struct ShEnginePoint *p_sp, struct MyMapElement *p_mapel, short *p_sqlight)
{
    int shd;
    ushort qlght, n;

    shd = (p_mapel->Ambient << 7) + (p_sp->field_9) + 256 + (*p_sqlight << 8);
    qlght = p_mapel->Shade;
    n = 0;
    while (qlght != 0)
    {
        struct QuickLight *p_qlght;
        n++;
        if (n >= 100)
            break;
        p_qlght = &game_quick_lights[qlght];
        shd += p_qlght->Ratio * game_full_lights[p_qlght->Light].Intensity;
        qlght = p_qlght->NextQuick;
    }
    if (shd > 32256)
        shd = 32512;
    return shd;
}

ubyte lvdraw_fill_bound_points(struct TbPoint *bound_pts)
{
    int fctr_x1, fctr_z1, fctr_x2, fctr_z2;
    int mag_x, mag_z;
    ubyte slt, slt_zmin;
    ushort angXZ;
    int sin_angl, cos_angl;

    angXZ = (engn_anglexz >> 5) & 0x7FF;
    sin_angl = lbSinTable[angXZ];
    cos_angl = lbSinTable[angXZ + 512];

    fctr_x1 = (dword_152E58 * render_area_a * cos_angl) >> 16;
    fctr_z2 = (dword_152E58 * render_area_a * -sin_angl) >> 16;
    fctr_z1 = (render_area_b * cos_angl) >> 8;
    fctr_x2 = (render_area_b * sin_angl) >> 8;
    mag_z = engn_zc & 0xFF00;
    mag_x = engn_xc & 0xFF00;

    bound_pts[0].y = (mag_z + ((dword_152E50 * (fctr_z1 + fctr_z2)) >> 8)) >> 8;
    bound_pts[1].y = (mag_z + ((dword_152E54 * (fctr_z2 - fctr_z1)) >> 8)) >> 8;
    bound_pts[1].x = (mag_x + ((dword_152E54 * (fctr_x1 - fctr_x2)) >> 8)) >> 8;
    bound_pts[2].x = (mag_x + ((dword_152E54 * (-fctr_x1 - fctr_x2)) >> 8)) >> 8;
    bound_pts[0].x = (mag_x + ((dword_152E50 * (fctr_x2 + fctr_x1)) >> 8)) >> 8;
    bound_pts[2].y = (mag_z + ((dword_152E54 * (-fctr_z2 - fctr_z1)) >> 8)) >> 8;
    bound_pts[3].y = (mag_z + ((dword_152E50 * (fctr_z1 - fctr_z2)) >> 8)) >> 8;
    bound_pts[3].x = (mag_x + ((dword_152E50 * (fctr_x2 - fctr_x1)) >> 8)) >> 8;

    slt_zmin = 0;
    for (slt = 1; slt < 4; slt++) {
        if (bound_pts[slt].y < bound_pts[slt_zmin].y)
            slt_zmin = slt;
    }

    return slt_zmin;
}

int lvdraw_fill_ranges_x(int slt_zmin, struct Range *ranges_x, struct TbPoint *bound_pts)
{
    int rn, ranges_used;
    int slt1_fcx, slt2_fcx;
    int slt1_incx, slt2_incx;
    int z_cur, z_max;
    ubyte slt, slt1, slt2;

    z_max = bound_pts[0].y;
    for (slt = 1; slt < 4; slt++) {
        if (z_max < bound_pts[slt].y)
            z_max = bound_pts[slt].y;
    }

    slt2 = slt_zmin;
    slt1 = slt_zmin;
    slt1_fcx = slt2_fcx = 0;
    slt1_incx = slt2_incx = 0;
    z_cur = bound_pts[slt_zmin].y;
    rn = 0;
    while (z_cur < z_max)
    {
        if (z_cur == bound_pts[slt1].y)
        {
            int dt_z;
            int base_fcx;
            int x_cur;

            while (z_cur == bound_pts[slt1].y)
            {
              int x_tmp;
              x_tmp = bound_pts[slt1].x;
              slt1 = (slt1 + 1) & 3;
              base_fcx = x_tmp << 16;
            }
            x_cur = bound_pts[slt1].x;
            dt_z = bound_pts[slt1].y - z_cur;
            slt1_incx = ((x_cur << 16) - base_fcx) / dt_z;
            slt1_fcx = slt1_fcx + 0x10000;
        }

        if (z_cur == bound_pts[slt2].y)
        {
            int dt_z;
            int base_fcx;
            int x_cur;

            while (z_cur == bound_pts[slt2].y)
            {
              int x_tmp;
              x_tmp = bound_pts[slt2].x;
              slt2 = (slt2 - 1) & 3;
              base_fcx = x_tmp << 16;
            }
            x_cur = bound_pts[slt2].x;
            dt_z = bound_pts[slt2].y - z_cur;
            slt2_incx = ((x_cur << 16) - base_fcx) / dt_z;
            slt2_fcx = base_fcx + 0x10000;
        }

        ranges_x[rn + 1].beg = slt1_fcx >> 16 << 8;
        ranges_x[rn + 1].fin = slt2_fcx >> 16 << 8;

        z_cur++;
        slt2_fcx += slt2_incx;
        slt1_fcx += slt1_incx;
        rn++;
    }

    ranges_used = rn;
    for (rn = 0; rn < ranges_used; rn++)
    {
        if (ranges_x[rn + 1].beg < 0)
            ranges_x[rn + 1].beg = 0;
        if (ranges_x[rn + 1].fin > 0x7F00)
            ranges_x[rn + 1].fin = 0x7F00;
    }

    return ranges_used;
}

void filter_ranges_max_of_two(int ranges_len, struct Range *ranges_flt, struct Range *ranges_inp)
{
    int rn;

    ranges_flt[0].beg = ranges_inp[1].beg;
    ranges_flt[0].fin = ranges_inp[1].fin + 256;

    for (rn = 1; rn < ranges_len; rn++)
    {
        int x1, x2;

        x1 = ranges_inp[rn].beg;
        if (x1 < ranges_inp[rn + 1].beg)
            x1 = ranges_inp[rn + 1].beg;
        ranges_flt[rn].beg = x1;

        x2 = ranges_inp[rn].fin + 256;
        if (x2 < ranges_inp[rn + 1].fin + 256)
            x2 = ranges_inp[rn + 1].fin + 256;
        ranges_flt[rn].fin = x2;
    }

    ranges_flt[rn].beg = ranges_flt[rn - 1].beg;
    ranges_flt[rn].fin = ranges_flt[rn - 1].fin;
}

void lvdraw_do_objects(int prc_z_beg, uint ranges_x_len, struct Range *ranges_x)
{
    int prc_z;
    int prc_x, prc_x_end;
    uint rn;

    prc_z = prc_z_beg;
    for (rn = 0; rn < ranges_x_len; rn++)
    {
        prc_x = ranges_x[rn + 1].beg;
        prc_x_end = ranges_x[rn + 1].fin;
        for (; prc_x <= prc_x_end; prc_x += (1 << 8))
        {
            struct Thing *p_objtng;
            struct MyMapElement *p_mapel;
            ThingIdx objtng;
            short tile_x, tile_z;

            if ((prc_x <= 0) || (prc_x >= 0x8000) || (prc_z <= 0) || (prc_z >= 0x8000))
                continue;

            tile_z = prc_z >> 8;
            tile_x = prc_x >> 8;
            p_mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];
            objtng = game_col_vects_list[p_mapel->ColHead].Object;
            if (objtng > 0)
            {
                p_objtng = &things[objtng];
                if (p_objtng->U.UObject.DrawTurn != gameturn)
                    draw_thing_object(p_objtng);
            }
        }
        prc_z += (1 << 8);
    }
}

void func_218D3(void)
{
#if 0
    asm volatile ("call ASM_func_218D3\n"
        :  :  : "eax" );
    return;
#endif
    struct ShEnginePoint loc_unknarrD[(RENDER_AREA_MAX+1)*4];
    int shift_a, shift_b;
    int elcr_z, elpv_z;
    struct FloorTile *p_floortl;
    short *p_sqlight;

    word_19CC64 = (engn_xc & 0xFF00) - (render_area_a << 7);
    word_19CC66 = (engn_zc & 0xFF00) - (render_area_b << 7);
    if (word_19CC66 < 0)
        word_19CC66 = 0;
    p_floortl = &game_floor_tiles[1];
    p_sqlight = super_quick_light;
    elcr_z = word_19CC66;
    shift_b = 0;
    { // Separate first row from the rest as it has no previous
        struct MyMapElement *p_mapel;
        struct ShEnginePoint *p_spcr;
        int elcr_x;

        p_spcr = &loc_unknarrD[shift_b & 1];
        shift_a = 0;
        elcr_x = word_19CC64;
        p_mapel = &game_my_big_map[(elcr_x >> 8) + (elcr_z >> 8 << 7)];

        while (shift_a < render_area_a + 1)
        {
            int dxc, dyc, dzc;
            int elcr_y;

            elcr_y = shpoint_compute_coord_y(p_spcr, p_mapel, elcr_x, elcr_z);
            dxc = elcr_x - engn_xc;
            dzc = elcr_z - engn_zc;
            dyc = elcr_y - 8 * engn_yc;

            transform_shpoint(p_spcr, dxc, dyc, dzc);
            p_spcr->Shade = shpoint_compute_shade(p_spcr, p_mapel, p_sqlight);

            p_spcr += 2;
            p_mapel++;
            shift_a++;
            elcr_x += 256;
        }
    }

    elpv_z = elcr_z;
    elcr_z += 256;
    shift_b++;
    while (shift_b < render_area_b && elcr_z < 0x8000)
    {
        struct MyMapElement *p_mapel;
        struct ShEnginePoint *p_spcr;
        int elcr_x;

        p_spcr = &loc_unknarrD[shift_b & 1];
        shift_a = 0;
        elcr_x = word_19CC64;
        p_mapel = &game_my_big_map[(elcr_x >> 8) + (elcr_z >> 8 << 7)];

        while (shift_a < render_area_a + 1)
        {
            int dxc, dyc, dzc;
            int elcr_y;

            elcr_y = shpoint_compute_coord_y(p_spcr, p_mapel, elcr_x, elcr_z);
            dxc = elcr_x - engn_xc;
            dzc = elcr_z - engn_zc;
            dyc = elcr_y - 8 * engn_yc;

            transform_shpoint(p_spcr, dxc, dyc, dzc);
            p_spcr->Shade = -1;

            p_spcr += 2;
            p_mapel++;
            shift_a++;
            elcr_x += 256;
        }

        struct ShEnginePoint *p_spnx;

        p_spnx = &loc_unknarrD[(shift_b + 1) & 1];
        p_spcr = &loc_unknarrD[shift_b & 1];
        shift_a = 0;
        elcr_x = word_19CC64;
        while (shift_a < render_area_a)
        {
          int bktalt;

          bktalt = 0;
          if (word_152F00 > 17998) {
            break;
          }
          p_mapel = &game_my_big_map[(elpv_z >> 8 << 7) + (elcr_x >> 8)];
          if (((p_spcr[2].Flags | p_spnx[2].Flags | p_spcr->Flags | p_spnx->Flags) & 0x20) != 0
            || ((p_spnx[2].Flags & p_spcr->Flags & p_spnx->Flags & p_spcr[2].Flags) & 0x0F) != 0
            || elcr_x <= 0 || elcr_x >= 0x8000
            || elcr_z <= 0 || elcr_z >= 0x8000
            || ((game_perspective != 2) && ((p_mapel->Flags & 0x80) != 0)))
          {
              p_sqlight++;
              p_spcr += 2;
              p_spnx += 2;
          }
          else
          {
              struct MyMapElement *p_mapelXnx;
              struct MyMapElement *p_mapelXZnx;
              struct MyMapElement *p_mapelZnx;
              int bckt;
              ubyte ditype;

              p_floortl->X[0] = p_spnx->X;
              p_floortl->Y[0] = p_spnx->Y;
              bckt = p_spnx->field_4;
              if (p_spnx->Shade < 0) {
                  p_spnx->Shade = shpoint_compute_shade(p_spnx, p_mapel, p_sqlight);
              }
              p_floortl->Shade[0] = p_spnx->Shade;
              p_mapel->ShadeR = p_spnx->Shade >> 9;

              p_mapelXnx = p_mapel + 1;
              p_spnx += 2;
              p_sqlight += 1;
              p_floortl->X[1] = p_spnx->X;
              p_floortl->Y[1] = p_spnx->Y;
              if (bckt < p_spnx->field_4)
                  bckt = p_spnx->field_4;
              if (p_spnx->Shade < 0) {
                  p_spnx->Shade = shpoint_compute_shade(p_spnx, p_mapelXnx, p_sqlight);
              }
              p_floortl->Shade[1] = p_spnx->Shade;
              p_mapelXnx->ShadeR = p_spnx->Shade >> 9;

              p_spcr += 2;
              p_mapelXZnx = p_mapel + 128 + 1;
              p_sqlight += render_area_a;
              p_floortl->X[2] = p_spcr->X;
              p_floortl->Y[2] = p_spcr->Y;
              if (bckt < p_spcr->field_4)
                  bckt = p_spcr->field_4;
              if (p_spcr->Shade < 0) {
                  p_spcr->Shade = shpoint_compute_shade(p_spnx, p_mapelXZnx, p_sqlight);
              }
              p_floortl->Shade[2] = p_spcr->Shade;
              p_mapelXZnx->ShadeR = p_spcr->Shade >> 9;

              p_sqlight--;
              p_spcr -= 2;
              p_mapelZnx = p_mapel + 128;
              p_floortl->X[3] = p_spcr->X;
              p_floortl->Y[3] = p_spcr->Y;
              if (bckt < p_spcr->field_4)
                  bckt = p_spcr->field_4;
              if (p_spcr->Shade < 0) {
                  p_spcr->Shade = shpoint_compute_shade(p_spnx, p_mapelZnx, p_sqlight);
              }
              p_floortl->Shade[3] = p_spcr->Shade;
              p_mapelZnx->ShadeR = p_spcr->Shade >> 9;

              p_mapel = &game_my_big_map[(elpv_z >> 8 << 7) + (elcr_x >> 8)];
              if (p_mapel->Texture)
              {
                  struct SingleFloorTexture *p_fltextr;
                  p_floortl->Flags2 = 0;
                  p_fltextr = &game_textures[p_mapel->Texture & 0x3FFF];
                  if ((p_mapel->Texture & 0x8000) != 0)
                  {
                      p_floortl->Flags2 = 1;
                      if (byte_1C8444)
                      {
                          int alt;
                          if (p_mapel->Alt <= 0)
                            alt = 15000 * overall_scale;
                          else
                            alt = 500 * overall_scale;
                          bktalt = alt >> 8;
                      }
                      else
                      {
                          if (p_mapel->Alt <= 0)
                            bktalt = 3500;
                          else
                            bktalt = 2500;
                      }
                  }
                  p_floortl->Texture = p_fltextr;
                  if ((p_mapel->Flags & 0x20) != 0)
                      p_floortl->Flags = 0x10|0x04|0x01;
                  else
                      p_floortl->Flags = 0x04|0x01;
              }
              else
              {
                  p_floortl->Flags = 0x04;
                  p_floortl->Col = colour_grey2;
              }
              if ((p_mapel->Flags & 0x01) != 0)
              {
                  p_floortl->Shade[0] = 16128;
                  p_floortl->Shade[1] = 16128;
                  p_floortl->Shade[2] = 16128;
                  p_floortl->Shade[3] = 16128;
              }
              if ((p_mapel->Flags & 0x08) != 0)
                  p_floortl->Flags2 |= 0x02;
              bktalt += 200;
              p_floortl->Flags2 = p_mapel->Flags;
              p_floortl->Offset = p_mapel - game_my_big_map;
              p_floortl->Flags2b = p_mapel->Flags2;
              p_floortl->Page = p_mapel->ColumnHead >> 12;
              p_floortl++;
              bckt += 5000 + bktalt;
               if ((p_mapel->Texture & 0x4000) != 0)
                  ditype = DrIT_Unkn6;
              else
                  ditype = DrIT_Unkn4;
              draw_item_add(ditype, word_152F00, bckt);
              p_sqlight = &p_sqlight[-render_area_a + 1];
              p_spcr += 2;
              ++word_152F00;
            }
            shift_a++;
            elcr_x += 256;
        }
        shift_b++;
        elpv_z += 256;
        elcr_z += 256;
    }
}

void lvdraw_compute_lights(int prc_z_beg, int ranges_len, struct Range *smrang_x, struct ShEnginePoint *p_unknarrD)
{
    int prc_z;
    int v144;
    int v159;

    word_19CC64 = (engn_xc & 0xFF00) - (render_area_a << 7);
    word_19CC66 = (engn_zc & 0xFF00) - (render_area_b << 7);

    prc_z = prc_z_beg;

    v159 = prc_z >> 8;
    v144 = prc_z >> 7;
    // TODO why this isn't going through all ranges? What does it do?
    {
        int prc_x, prc_x_end;

        prc_x = smrang_x[0].beg;
        prc_x_end = smrang_x[0].fin;

        for (; prc_x <= prc_x_end; prc_x += (1 << 8))
        {
            struct MyMapElement *p_mapel;
            struct ShEnginePoint *p_unkitmD;
            int scr_dx, scr_dy, abs_scr_dx, abs_scr_dy;
            int scr_x, scr_y;
            uint shade;
            ubyte flags;
            int v121;
            int prc_dx;
            int v38;
            int v46;
            int v122;

            p_mapel = &game_my_big_map[MAP_TILE_WIDTH * (prc_z >> 8) + (prc_x >> 8)];
            p_unkitmD = &p_unknarrD[2 * (prc_x >> 8)];
            prc_dx = prc_x - engn_xc;

            if (game_perspective == 1)
            {
                v121 = 0;
                p_unkitmD->field_9 = 0;
            }
            else if ((p_mapel->Flags & 0x10) == 0)
            {
                v121 = 8 * p_mapel->Alt;
                if ((p_mapel->Flags & 0x40) != 0)
                    v121 += waft_table[gameturn & 0x1F];
                p_unkitmD->field_9 = 0;
            }
            else
            {
                v121 = 8 * p_mapel->Alt;
                v46 = ((*(int *)(&deep_radar_surface_col + 2 * (((ubyte)gameturn + (ubyte)(prc_x >> 7)) & 0x1F)) >> 16)
                 + (*(int *)(&deep_radar_surface_col + 2 * (((ubyte)gameturn + (ubyte)v144) & 0x1F)) >> 16)
                 + (*(int *)(&deep_radar_surface_col + 2 * ((32 * gameturn
                             / ((((ubyte)bw_rotl32(0x5D3BA6C3, v159) ^ (ubyte)bw_rotr32(0xA7B4D8AC, (sbyte)(prc_x >> 8))) & 0x7Fu)
                              + 140)) & 0x1F)) >> 16)) >> 3;
                v121 += 8 * (short)v46;
                p_unkitmD->field_9 = ((ushort)v46 + 32) << 9;
            }
            v121 -= 8 * engn_yc;

            abs_scr_dx = (dword_176D14 * prc_dx - dword_176D10 * (prc_z - engn_zc)) >> 16;
            v38 = (dword_176D10 * prc_dx + dword_176D14 * (prc_z - engn_zc)) >> 16;
            v122 = (dword_176D18 * v121 + dword_176D1C * v38) >> 16;
            abs_scr_dy = (dword_176D1C * v121 - v38 * dword_176D18) >> 16;
            scr_dx = (overall_scale * abs_scr_dx) >> 8;
            scr_dy = (overall_scale * abs_scr_dy) >> 8;

            flags = 0;
            if (v122 >= -500)
            {
                scr_dx = (1500 * scr_dx / ((v122 >> 2) + 500)) >> 1;
                scr_x = dword_176D3C + scr_dx;
                if (scr_x < 0) {
                    if (scr_x < -2000)
                        scr_x = -2000;
                    flags = 0x01;
                } else if (scr_x >= vec_window_width) {
                    if (scr_x > 2000)
                        scr_x = 2000;
                    flags |= 0x02;
                }

                scr_dy = -(1500 * scr_dy / ((v122 >> 2) + 500)) >> 1;
                scr_y = dword_176D40 + scr_dy;
                if (scr_y < 0) {
                    if (scr_y < -2000)
                        scr_y = -2000;
                    flags |= 0x04;
                } else if (scr_y >= vec_window_height) {
                    if (scr_y > 2000)
                        scr_y = 2000;
                    flags |= 0x08;
                }
                flags |= 0x40;
                v122 /= (ushort)render_area_a / 20 + 1;
            }
            else
            {
                scr_y = dword_176D40 + scr_dy;
                scr_x = dword_176D3C + scr_dx;
                flags = 0x20;
            }

            shade = ((ushort)p_mapel->Ambient << 7) + p_unkitmD->field_9 + 256;
            shade += cummulate_shade_from_quick_lights(p_mapel->Shade);
            if (v122 > 3000) {
              if (3512 - v122 > 0)
                  shade = ((3512 - v122) * shade) >> 9;
              else
                  shade = 0;
            }
            if (shade > 0x7E00)
                shade = 0x7F00;

            p_unkitmD->Flags = flags;
            p_unkitmD->X = scr_x;
            p_unkitmD->Y = scr_y;
            p_unkitmD->field_4 = v122;
            p_unkitmD->Shade = shade;
        }
    }
}

void func_2e440_fill_drawlist(int prc_z_beg, int ranges_x_len, int *smrang_x, struct Range *ranges_x, struct ShEnginePoint *p_unknarrD)
{
    struct FloorTile *v34;
    int v147;
    int v148;
    int v171;

    v34 = game_floor_tiles + 1;
    v171 = prc_z_beg;
    v147 = prc_z_beg - 256;
    for (v148 = 1; v148 < ranges_x_len; v148++)
    {
        struct ShEnginePoint *p_unknarrD1;
        struct MyMapElement *p_mapel1;
        int v47;
        int v49;
        int v51;
        int v161;
        int v162;

        int v53;
        int v54;
        int v55;
        int v56;
        unsigned int v57;
        int v58;
        int v60;
        int v61;
        struct ShEnginePoint *v62;
        int v63;
        int v64;
        struct ShEnginePoint *v65;
        struct MyMapElement *p_mapel3;
        int v67;
        ushort n;
        struct QuickLight *p_qlght4;
        int v70;
        int v71;
        struct ShEnginePoint *v72;
        int v73;
        struct MyMapElement *p_mapel4;
        int v75;
        struct ShEnginePoint *v76;
        int v77;
        ushort ii;
        struct QuickLight *p_qlght3;
        int v80;
        ubyte k;
        int v81;
        int v82;
        struct MyMapElement *p_mapel5;
        int v84;
        struct ShEnginePoint *v85;
        int v86;
        ushort jj;
        struct QuickLight *p_qlght2;
        int v89;
        int v90;
        struct MyMapElement *p_mapel6;
        struct MyMapElement *p_mapel8;
        short v102;
        int v104;
        ubyte v105;
        ubyte v106;
        struct MyMapElement *p_mapel7;
        ushort m;
        struct QuickLight *p_qlght;
        int v110;
        int v111;

        int v118;
        int v119;
        int v120;
        int v121;
        int v122;
        ubyte flags_A;
        struct ShEnginePoint *v149;
        int v163;
        int v164;
        int v165;
        int v167;
        int v170;
        struct MyMapElement *p_mapel2;
        int v174;
        ushort v175;

        v47 = smrang_x[2 * v148 + 0] >> 8;
        p_unknarrD1 = &p_unknarrD[2 * v47 + (v148 & 1)];
        v161 = v171 >> 8;
        v49 = smrang_x[2 * v148 + 0];
        v162 = v171 >> 7;
        p_mapel1 = &game_my_big_map[128 * (v171 >> 8) + v47];
        v51 = smrang_x[2 * v148 + 1];
        p_mapel2 = p_mapel1;
        for (k = v49 <= v51; k; k = v49 <= v58)
        {
          v120 = v49 - engn_xc;
          if (game_perspective == 1)
          {
            v121 = 0;
            p_unknarrD1->field_9 = 0;
          }
          else if ((p_mapel2->Flags & 0x10) == 0)
          {
            v121 = 8 * p_mapel2->Alt;
            if ((p_mapel2->Flags & 0x40) != 0)
                v121 += waft_table[gameturn & 0x1F];
            p_unknarrD1->field_9 = 0;
          }
          else
          {
            v121 = 8 * p_mapel2->Alt;
            v60 = ((*(int *)(&deep_radar_surface_col + 2 * ((32 * gameturn
                           / ((((ubyte)bw_rotl32(0x5D3BA6C3, v161) ^ (ubyte)bw_rotr32(0xA7B4D8AC, (sbyte)(v49 >> 8))) & 0x7Fu)
                            + 140)) & 0x1F)) >> 16)
               + (*(int *)(&deep_radar_surface_col + 2 * (((ubyte)gameturn + (ubyte)(v49 >> 7)) & 0x1F)) >> 16)
               + (*(int *)(&deep_radar_surface_col + 2 * (((ubyte)gameturn + (ubyte)v162) & 0x1F)) >> 16)) >> 3;
            v121 += 8 * (short)v60;
            p_unknarrD1->field_9 = ((ushort)v60 + 32) << 9;
          }
          flags_A = 0;
          v121 -= 8 * engn_yc;
          v163 = (dword_176D14 * v120 - dword_176D10 * (v171 - engn_zc)) >> 16;
          v53 = (dword_176D10 * v120 + dword_176D14 * (v171 - engn_zc)) >> 16;
          v165 = dword_176D1C * v121;
          v164 = (dword_176D1C * v121 - v53 * dword_176D18) >> 16;
          v122 = (dword_176D1C * v53 + dword_176D18 * v121) >> 16;
          v120 = (overall_scale * v163) >> 8;
          v121 = (overall_scale * v164) >> 8;
          if (v122 >= -500)
          {
            v165 = (v122 >> 2) + 500;
            v54 = ((1500 * v120 / v165) >> 1) + dword_176D3C;
            v118 = v54;
            if (v54 < 0)
            {
                if (v54 < -2000)
                    v118 = -2000;
                flags_A |= 0x01;
            }
            else if (v54 >= vec_window_width)
            {
                if (v54 > 2000)
                    v118 = 2000;
                flags_A |= 0x02;
            }

            v165 = (v122 >> 2) + 500;
            v55 = dword_176D40 + (-(1500 * v121 / v165) >> 1);
            v119 = v55;
            if (v55 < 0) {
                if (v119 < -2000)
                    v119 = -2000;
                flags_A |= 0x04;
            } else if (v55 >= vec_window_height) {
                if (v119 > 2000)
                    v119 = 2000;
                flags_A |= 0x08;
            }
            flags_A |= 0x40;
            v122 /= render_area_a / 20 + 1;
          }
          else
          {
            flags_A = 0x20;
          }

          p_unknarrD1->Flags = flags_A;
          p_unknarrD1->X = v118;
          p_unknarrD1->Y = v119;
          p_unknarrD1->field_4 = v122;
          p_unknarrD1->Shade = -1;
          p_unknarrD1 += 2;
          v56 = (int)p_mapel2;
          v49 += 256;
          v57 = 2 * v148;
          v58 = smrang_x[v57 + 1];
          p_mapel2 = (struct MyMapElement *)(v56 + 18);
        }
        v61 = 2 * (ranges_x[v148].beg >> 8);
        v149 = &p_unknarrD[v61 + ((v148 + 1) & 1)];
        v62 = &p_unknarrD[v61 + ((v148) & 1)];
        v170 = ranges_x[v148].beg;
        v167 = v147 >> 8 << 7;
        v63 = ranges_x[v148].fin;
        if ( v63 >= v170 )
        {
          do
          {
            v175 = 0;
            if ( (ushort)word_152F00 > 0x464Eu )
              break;
            v105 = v62->Flags;
            v106 = v149[2].Flags;
            if ( ((v62[2].Flags | (ubyte)(v106 | v105 | v149->Flags)) & 0x20) != 0
              || ((ubyte)(v106 & v105 & v149->Flags) & v62[2].Flags & 0xF) != 0
              || v170 <= 0 || v170 >= 0x8000
              || v171 <= 0 || v171 >= 0x8000 )
            {
              v62 += 2;
              v149 += 2;
            }
            else
            {
              p_mapel7 = &game_my_big_map[v167 + (v170 >> 8)];
              v34->X[0] = v149->X;
              v34->Y[0] = v149->Y;
              v174 = v149->field_4;
              if (v149->Shade >= 0)
              {
                v34->Shade[0] = v149->Shade;
              }
              else
              {
                v64 = ((ushort)p_mapel7->Ambient << 7) + v149->field_9 + 256;
                for ( m = p_mapel7->Shade; m; v64 += v110 )
                {
                  p_qlght = &game_quick_lights[m];
                  v110 = p_qlght->Ratio * game_full_lights[p_qlght->Light].Intensity;
                  m = p_qlght->NextQuick;
                }
                if (v149->field_4 > 3000)
                {
                  v111 = v149->field_4;
                  if ( 3512 - v111 > 0 )
                    v64 = ((3512 - v111) * v64) >> 9;
                  else
                    v64 = 0;
                }
                if (v64 > 0x7E00)
                  v64 = 0x7F00;
                v65 = v149;
                v34->Shade[0] = v64;
                v65->Shade = v64;
              }
              p_mapel7->ShadeR = v149->Shade >> 9;
              v149 += 2;
              v34->X[1] = v149->X;
              v34->Y[1] = v149->Y;
              p_mapel3 = p_mapel7 + 1;
              if (v149->field_4 > v174)
                  v174 = v149->field_4;
              if (v149->Shade >= 0)
              {
                v34->Shade[1] = v149->Shade;
              }
              else
              {
                v67 = ((ushort)p_mapel3->Ambient << 7) + v149->field_9 + 256;
                for ( n = p_mapel3->Shade; n; v67 += v70 )
                {
                  p_qlght4 = &game_quick_lights[n];
                  v70 = p_qlght4->Ratio * game_full_lights[p_qlght4->Light].Intensity;
                  n = p_qlght4->NextQuick;
                }
                if (v149->field_4 > 3000)
                {
                  v71 = v149->field_4;
                  if ( 3512 - v71 > 0 )
                    v67 = ((3512 - v71) * v67) >> 9;
                  else
                    v67 = 0;
                }
                if (v67 > 0x7E00)
                  v67 = 0x7F00;
                v72 = v149;
                v34->Shade[1] = v67;
                v72->Shade = v67;
              }
              p_mapel3->ShadeR = v149->Shade >> 9;
              v34->X[2] = v62[2].X;
              v73 = v174;
              v34->Y[2] = v62[2].Y;
              p_mapel4 = p_mapel3 + 128;
              v75 = v62[2].field_4;
              v76 = v62 + 2;
              if (v75 > v73)
                  v174 = v75;
              if (v76->Shade >= 0)
              {
                v34->Shade[2] = v76->Shade;
              }
              else
              {
                v77 = ((ushort)p_mapel4->Ambient << 7) + v149->field_9 + 256;
                for ( ii = p_mapel4->Shade; ii; v77 += v80 )
                {
                  p_qlght3 = &game_quick_lights[ii];
                  v80 = p_qlght3->Ratio * game_full_lights[p_qlght3->Light].Intensity;
                  ii = p_qlght3->NextQuick;
                }
                v81 = v76->field_4;
                if ( v81 > 3000 )
                {
                  if ( 3512 - v81 > 0 )
                    v77 = ((3512 - v81) * v77) >> 9;
                  else
                    v77 = 0;
                }
                if (v77 > 0x7E00)
                    v77 = 0x7F00;
                v34->Shade[2] = v77;
                v76->Shade = v77;
              }
              p_mapel4->ShadeR = v76->Shade >> 9;
              v34->X[3] = v76[-2].X;
              v82 = v174;
              v34->Y[3] = v76[-2].Y;
              p_mapel5 = p_mapel4 - 1;
              v84 = v76[-2].field_4;
              v85 = v76 - 2;
              if ( v84 > v82 )
                  v174 = v84;
              if (v85->Shade >= 0)
              {
                v34->Shade[3] = v85->Shade;
              }
              else
              {
                v86 = ((ushort)p_mapel5->Ambient << 7) + v149->field_9 + 256;
                for ( jj = p_mapel5->Shade; jj; v86 += v89 )
                {
                  p_qlght2 = &game_quick_lights[jj];
                  v89 = p_qlght2->Ratio * game_full_lights[p_qlght2->Light].Intensity;
                  jj = p_qlght2->NextQuick;
                }
                v90 = v85->field_4;
                if (v90 > 3000)
                {
                  if ( 3512 - v90 > 0 )
                    v86 = ((3512 - v90) * v86) >> 9;
                  else
                    v86 = 0;
                }
                if (v86 > 0x7E00)
                  v86 = 0x7F00;
                v34->Shade[3] = v86;
                v85->Shade = v86;
              }
              p_mapel5->ShadeR = v85->Shade >> 9;
              p_mapel6 = &game_my_big_map[v167 + (v170 >> 8)];
              if (p_mapel6->Texture != 0)
              {
                struct SingleFloorTexture *p_fltextr;
                short fltextr;

                fltextr = p_mapel6->Texture & 0x3FFF;
                v34->Flags2 = 0;
                p_fltextr = &game_textures[fltextr];
                if ((p_mapel6->Texture & 0x8000) != 0)
                {
                  v34->Flags2 = 1;
                  if (byte_1C8444)
                  {
                      uint tmp;
                      if (p_mapel6->Alt <= 0)
                        tmp = 15000 * overall_scale;
                      else
                        tmp = 500 * overall_scale;
                      v175 = tmp >> 8;
                  }
                  else
                  {
                      if (p_mapel6->Alt <= 0)
                        v175 = 2000;
                      else
                        v175 = 1000;
                  }
                }
                v34->Texture = p_fltextr;
                v34->Flags = (p_mapel6->Flags & 0x20) != 0 ? 21 : 5;
                v34->Page = (int)(ushort)p_mapel6->ColumnHead >> 12;
              }
              else
              {
                v34->Flags = 4;
                v34->Col = colour_grey2;
              }
              if ((p_mapel6->Flags & 0x01) != 0)
              {
                  v34->Shade[0] = 0x3F00;
                  v34->Shade[1] = 0x3F00;
                  v34->Shade[2] = 0x3F00;
                  v34->Shade[3] = 0x3F00;
              }
              if ((p_mapel6->Flags & 0x08) != 0)
                  v34->Flags2 |= 0x02;
              p_mapel8 = game_my_big_map;
              v174 += v175 + 5000;
              v34->Flags2 = p_mapel6->Flags;
              v34->Offset = p_mapel6 - p_mapel8;
              v34->Flags2b = p_mapel6->Flags2;
              ubyte ditype;
              ditype = (p_mapel6->Texture & 0x4000) != 0 ? DrIT_Unkn6 : DrIT_Unkn4;
              v102 = word_152F00;

              if (!draw_item_add(ditype, v102, v174))
                  break;

              ++v34;
              v62 = v85 + 2;
              word_152F00 = v102 + 1;
            }
            v104 = ranges_x[v148].fin;
            v170 += 256;
          }
          while ( v170 <= v104 );
        }
        v147 += 256;
        v171 += 256;
    }
}

void func_2e440(void)
{
#if 0
    asm volatile ("call ASM_func_2e440\n"
        :  :  : "eax" );
    return;
#else
    int angXZ;
    ubyte slt_zmin;

    struct ShEnginePoint loc_unknarrD[272];
    struct Range smrang_x[160];
    struct Range ranges_x[160];
    struct TbPoint bound_pts[4];
    int prc_z_beg, ranges_x_len;

    word_152F00 = 1;

    slt_zmin = lvdraw_fill_bound_points(bound_pts);

    prc_z_beg = bound_pts[slt_zmin].y << 8;

    ranges_x_len = lvdraw_fill_ranges_x(slt_zmin, ranges_x, bound_pts);

    filter_ranges_max_of_two(ranges_x_len, smrang_x, ranges_x);

    player_target_clear(local_player_no);

    if ((ingame.Flags & 0x01) != 0)
    {
        dword_176CC0 += fifties_per_gameturn;
        if (dword_176CC0 > 80) {
            dword_176CC0 = 0;
            xdo_next_frame(1);
        }
    }
    angXZ = (engn_anglexz >> 5) & 0x7FF;
    byte_176D4B = ((angXZ + 64) >> 7) & 0xF;
    byte_176D48 = ((angXZ + 256) >> 9) & 0x3;
    byte_176D49 = ((angXZ + 128) >> 8) & 0x7;
    byte_19EC7A = byte_176D48;

    lvdraw_do_objects(prc_z_beg, ranges_x_len, ranges_x);

    lvdraw_compute_lights(prc_z_beg, ranges_x_len, smrang_x, loc_unknarrD);

    func_2e440_fill_drawlist(prc_z_beg, ranges_x_len, (int *)smrang_x, ranges_x, loc_unknarrD);

    vec_map = vec_tmap[1];

    draw_screen();
#endif
}

#define SUPER_QUICK_RADIUS 5
void apply_super_quick_light(short lx, short lz, ushort b, ubyte *mapwho_lights)
{
    short tile_x_beg, tile_x_end;
    short tile_z_beg, tile_z_end;
    short tile_x, tile_z;
    int mapcor_x, mapcor_z;
    short ratile_x_beg, ratile_z_beg;
    short ratile_x, ratile_z;

    tile_z_beg = MAPCOORD_TO_TILE(lz) - SUPER_QUICK_RADIUS/2;
    tile_x_beg = MAPCOORD_TO_TILE(lx) - SUPER_QUICK_RADIUS/2;
    if (tile_z_beg <= -SUPER_QUICK_RADIUS || tile_z_beg >= MAP_TILE_HEIGHT)
        return;
    if (tile_x_beg <= -SUPER_QUICK_RADIUS || tile_x_beg >= MAP_TILE_WIDTH)
        return;

    mapcor_z = TILE_TO_MAPCOORD(render_area_b,0) / 2;
    mapcor_x = TILE_TO_MAPCOORD(render_area_a,0) / 2;
    if ((lz <= engn_zc - mapcor_z) || (lz >= engn_zc + mapcor_z))
        return;
    if ((lx <= engn_xc - mapcor_x) || (lx >= engn_xc + mapcor_x))
        return;

    ratile_z_beg = (render_area_b >> 1) - MAPCOORD_TO_TILE(engn_zc);
    if (ratile_z_beg > 0) // required to avoid shifting light to terrain near map border
        ratile_z_beg = 0;
    ratile_z_beg += tile_z_beg;
    ratile_x_beg = (render_area_a >> 1) - MAPCOORD_TO_TILE(engn_xc);
    ratile_x_beg += tile_x_beg;
    tile_x_end = tile_x_beg + SUPER_QUICK_RADIUS;
    tile_z_end = tile_z_beg + SUPER_QUICK_RADIUS;

    for (tile_z = tile_z_beg, ratile_z = ratile_z_beg; tile_z < tile_z_end; tile_z++, ratile_z++)
    {
        mapcor_z = TILE_TO_MAPCOORD(tile_z,0);
        for (tile_x = tile_x_beg, ratile_x = ratile_x_beg; tile_x < tile_x_end; tile_x++, ratile_x++)
        {
            short *p_sqlight;
            int f, dist;
            short intensity;

            mapcor_x = TILE_TO_MAPCOORD(tile_x,0);

            if (ratile_x < 0 || ratile_x >= render_area_a)
                continue;
            if (ratile_z < 0 || ratile_z >= render_area_b)
                continue;
            if (tile_x < 0 || tile_x >= MAP_TILE_WIDTH)
                continue;
            if (tile_z < 0 || tile_z >= MAP_TILE_HEIGHT)
                continue;

            p_sqlight = &super_quick_light[ratile_x + render_area_a * ratile_z];

            dist = (mapcor_x - lx) * (mapcor_x - lx)
                + (mapcor_z - lz) * (mapcor_z - lz);
            if (dist > 0)
              f = 1088608 / dist;
            else
              f = 32;
            intensity = b * f >> 5;
            if (intensity > 32)
                intensity = 32;
            *p_sqlight += intensity;
        }
    }
}

void clear_super_quick_lights(void)
{
    int i;

    for (i = 0; i < render_area_a * (render_area_b + 1); i++)
    {
        short *p_sqlight;
        p_sqlight = &super_quick_light[i];
        *p_sqlight = 0;
    }
}

void reset_super_quick_lights(void)
{
    word_152F00 = 1;
}

void draw_screen(void)
{
    if (dword_19F4F8)
    {
        draw_drawlist_1();
    }
    else
    {
        draw_drawlist_2();
    }
#if 0
    //TODO Setting first palette colour was often used as debug helper; to be removed
    __outbyte(0x3C8u, 0);
    __outbyte(0x3C9u, byte_1C83E0);
    __outbyte(0x3C9u, 0);
    __outbyte(0x3C9u, 0);
#endif
    reset_drawlist();
}

/******************************************************************************/
