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
#include "enginfloor.h"
#include "enginlights.h"
#include "enginsngtxtr.h"
#include "engintrns.h"
#include "enginzoom.h"
#include "game_speed.h"
#include "scanner.h"
#include "swlog.h"
#include "player.h"
#include "thing.h"
/******************************************************************************/
extern short super_quick_light[(RENDER_AREA_MAX+1)*(RENDER_AREA_MAX+1)];
extern short word_152F00;

extern long dword_152E50;
extern long dword_152E54;
extern long dword_152E58;

extern long dword_176CC0;

extern short word_19CC64;
extern short word_19CC66;


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

void func_2e440(void)
{
#if 1
    asm volatile ("call ASM_func_2e440\n"
        :  :  : "eax" );
    return;
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

/******************************************************************************/
