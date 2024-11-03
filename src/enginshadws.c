/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginshadws.c
 *     Shadows preparation and drawing required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling shadows in 3D world.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 13 Sep 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginshadws.h"

#include <string.h>
#include "bfmath.h"

#include "display.h"
#include "engintrns.h"
#include "engindrwlst.h"
#include "thing.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

extern const ubyte byte_154F2C[32];
extern const ushort word_154F4C[14];

extern ubyte sprshadow_EE90[24];
extern ubyte sprshadow_EEA8[600];
extern ubyte sprshadow_F100[24];
extern ubyte sprshadow_F118[600];
extern ubyte sprshadow_F370[24];
extern ubyte sprshadow_F388[600];
extern ubyte sprshadow_F5E0[24];
extern ubyte sprshadow_F5F8[600];
extern sbyte sprshadow_F850[512];

void draw_person_shadow(ushort face)
{
#if 0
    asm volatile (
      "call ASM_draw_person_shadow\n"
        : : "a" (face));
    return;
#endif
    struct Thing *p_thing;
    ushort fr, anmode;
    ushort shpak;
    int frgrp;
    ubyte k;
    int ssh_y, ssh_x;
    int sh_x, sh_y;
    int sc_a, sc_b;
    short strng;

    struct SortSprite *sspr;
    struct EnginePoint point4;
    struct EnginePoint point2;
    struct EnginePoint point1;
    struct EnginePoint point3;

    p_thing = game_sort_sprites[face].PThing;
    vec_mode = 10;
    vec_map = vec_tmap[ingame.LastTmap];

    fr = p_thing->Frame - nstart_ani[p_thing->StartFrame + 1 + p_thing->U.UObject.Angle];
    anmode = p_thing->U.UPerson.AnimMode;
    if ((anmode == 2) || (anmode == 15) || (anmode == 7))
        shpak = 12;
    else if ((anmode == 1) || (anmode == 14) || (anmode == 6))
        shpak = byte_154F2C[2 * p_thing->SubType + 1];
    else
        shpak = byte_154F2C[2 * p_thing->SubType + 0];

    frgrp =  8 * shpak + (((p_thing->U.UPerson.Shadows[0] >> 5) - p_thing->U.UObject.Angle + 8) & 7);
    point3.pp.U = sprshadow_EE90[6 * frgrp + fr] << 16;
    point3.pp.V = sprshadow_F5E0[6 * frgrp + fr] << 16;
    point4.pp.U = sprshadow_F370[6 * frgrp + fr] << 16;
    point4.pp.V = point3.pp.V;
    point1.pp.U = point4.pp.U;
    point1.pp.V = sprshadow_F100[6 * frgrp + fr] << 16;
    point2.pp.U = point3.pp.U;
    point2.pp.V = point1.pp.V;

    k = p_thing->U.UPerson.Shadows[0] - (engn_anglexz >> 8);
    ssh_x = sprshadow_F850[2 * k + 1];
    ssh_y = -sprshadow_F850[2 * k + 0];
    sh_y = (6 * ssh_y + 64) >> 7;
    sh_x = (6 * ssh_x + 64) >> 7; // We will reverse the sign later
    sh_x = (overall_scale * sh_x) >> 8;
    sh_y = (overall_scale * sh_y) >> 8;
    strng = p_thing->U.UPerson.Shadows[1];
    if (strng > 128)
        strng = 128;
    vec_colour = (strng >> 3) + 16;
    sc_a = (strng * sh_y) >> 6;
    sc_b = (strng * sh_x) >> 6;
    sh_x = -sh_x;

    sspr = &game_sort_sprites[face];
    point3.pp.X = sspr->X - sh_x;
    point3.pp.Y = sspr->Y - sh_y;
    point4.pp.X = sspr->X + sh_x;
    point4.pp.Y = sspr->Y + sh_y;

    if (strng > 64) {
        sh_x = (sh_x * strng) >> 6;
        sh_y = (sh_y * strng) >> 6;
    }

    sspr = &game_sort_sprites[face];
    point1.pp.X = 4 * sc_a + sspr->X + sh_x;
    point1.pp.Y = 4 * sc_b + sspr->Y + sh_y;
    point2.pp.X = 4 * sc_a + sspr->X - sh_x;
    point2.pp.Y = 4 * sc_b + sspr->Y - sh_y;

    dword_176D4C++;
    if (vec_mode == 2)
        vec_mode = 27;
    draw_trigpoly(&point1.pp, &point4.pp, &point3.pp);
    dword_176D4C++;
    if (vec_mode == 2)
        vec_mode = 27;
    draw_trigpoly(&point2.pp, &point1.pp, &point3.pp);
}

void copy_from_screen_ani(ubyte *buf)
{
    int y;
    ubyte *o;
    const ubyte *inp;

    o = buf;
    inp = lbDisplay.WScreen;
    for (y = 0; y < 256; y++)
    {
        memcpy(o, inp, 256);
        o += 256;
        inp += lbDisplay.GraphicsScreenWidth;
    }
}

struct Element *element_unkn_func_05(ushort a1, short *a2, short *a3, short *a4, short *a5)
{
    struct Element *ret;
    asm volatile (
      "push %5\n"
      "call ASM_element_unkn_func_05\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5));
    return ret;
}

/** Prepare buffer with sprite shadows.
 * Clear the wscreen buffer before this call. Also make sure m_sprites are loaded.
 */
void generate_shadows_for_multicolor_sprites(void)
{
#if 0
    asm volatile ("call ASM_generate_shadows_for_multicolor_sprites\n"
        :  :  : "eax" );
    return;
#endif
    struct ScreenBufBkp bkp;
    int shpak;
    short v23mw;
    short v23hw;
    int v25a;
    int i;

    // TODO would be better to use some back buffer instead of normal screen buf
    screen_switch_to_custom_buffer(&bkp, lbDisplay.WScreen,
      lbDisplay.GraphicsScreenWidth, 256);
    LbScreenClear(0);

    overall_scale = 256;

    v23hw = 0;
    v25a = 0;
    v23mw = 0;
    for (shpak = 12; shpak > -1; shpak--)
    {
        int base_idx;
        ushort spr;

        base_idx = shpak * 8 * 6;
        for (spr = 0; spr < 4; spr++)
        {
            ushort fr;
            ushort kk;

            fr = nstart_ani[spr + word_154F4C[shpak]];
            for (kk = 0; kk < 6; kk += 2)
            {
                int idx;
                short a2b, a3a, v25b, a5a;
                ubyte val3a, val5a, val5b, val6a, val8a;
                short v21;

                element_unkn_func_05(fr, &a2b, &a3a, &v25b, &a5a);
                if (v23hw + a3a - a2b + 1 > 255)
                {
                    v23hw = 0;
                    v25a += v23mw;
                    v23mw = 0;
                }
                if (a5a - v25b + 1 > v23mw)
                    v23mw = a5a - v25b + 1;
                v23hw -= a2b;
                v21 = v25a - v25b;
                draw_sorted_sprite1a(fr, v23hw, v21, 10);

                val3a = v23hw + a2b;
                val5a = v21 + v25b;
                val5b = v23hw + a2b;
                val6a = v23hw + a3a;
                val8a = v21 + a5a;

                idx = base_idx + 6 * spr;
                sprshadow_EE90[kk + idx + 0] = val5b;
                sprshadow_F100[kk + idx + 0] = val5a;
                sprshadow_F370[kk + idx + 0] = val6a;
                sprshadow_F100[kk + idx + 1] = val5a;
                sprshadow_F370[kk + idx + 1] = val6a;
                sprshadow_F5E0[kk + idx + 0] = val8a;
                sprshadow_EE90[kk + idx + 1] = val3a;
                sprshadow_F5E0[kk + idx + 1] = val8a;
                if (spr != 0)
                {
                  idx = base_idx + 6 * (8 - spr);
                  sprshadow_F370[kk + idx + 0] = val3a;
                  sprshadow_F100[kk + idx + 0] = val5a;
                  sprshadow_EE90[kk + idx + 0] = val6a;
                  sprshadow_F5E0[kk + idx + 0] = val8a;
                  sprshadow_F370[kk + idx + 1] = val3a;
                  sprshadow_F100[kk + idx + 1] = val5a;
                  sprshadow_EE90[kk + idx + 1] = val6a;
                  sprshadow_F5E0[kk + idx + 1] = val8a;
                }
                else
                {
                  idx = base_idx;
                  sprshadow_F388[kk + idx + 0] = val3a;
                  sprshadow_F118[kk + idx + 0] = val5a;
                  sprshadow_EEA8[kk + idx + 0] = val6a;
                  sprshadow_F5F8[kk + idx + 0] = val8a;
                  sprshadow_F388[kk + idx + 1] = val3a;
                  sprshadow_F118[kk + idx + 1] = val5a;
                  sprshadow_EEA8[kk + idx + 1] = val6a;
                  sprshadow_F5F8[kk + idx + 1] = val8a;
                }
                fr = frame[frame[fr].Next].Next;
                v23hw += a3a + 1;
            }
        }
    }

    copy_from_screen_ani(vec_tmap[ingame.LastTmap]);

    for (i = 0; i < 256; i++)
    {
        int x, y, angle;

        angle = 8 * i;
        x = lbSinTable[angle] >> 9;
        y = lbSinTable[angle + 512] >> 9;
        if (x > 127)
            x = 127;
        if (x < -128)
            x = -128;
        if (y > 127)
            y = 127;
        if (y < -128)
            y = -128;

        sprshadow_F850[2 * i + 0] = x;
        sprshadow_F850[2 * i + 1] = y;
    }

    screen_load_backup_buffer(&bkp);
}

/******************************************************************************/
