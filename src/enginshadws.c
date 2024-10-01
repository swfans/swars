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

#include "engintrns.h"
#include "display.h"
#include "thing.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

extern const ushort word_154F4C[14];

extern ubyte sprshadow_EE90[24];
extern ubyte sprshadow_EEA8[600];
extern ubyte sprshadow_F100[24];
extern ubyte sprshadow_F118[600];
extern ubyte sprshadow_F370[24];
extern ubyte sprshadow_F388[600];
extern ubyte sprshadow_F5E0[24];
extern ubyte sprshadow_F5F8[600];
extern ubyte sprshadow_F850[512];

void draw_effect_object_face(ushort face)
{
    asm volatile (
      "call ASM_draw_effect_object_face\n"
        : : "a" (face));
}

void draw_sorted_sprite1a(ushort frm, short x, short y, ubyte csel)
{
    asm volatile (
      "call ASM_draw_sorted_sprite1a\n"
        : : "a" (frm), "d" (x), "b" (y), "c" (csel));
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
    long bkpPhysScrHeight;
    long bkpGrphScrHeight;
    long bkpGrphWndwHeight;
    int v20;
    short v23mw;
    short v23hw;
    int v25a;
    int i;

    overall_scale = 256;
    bkpPhysScrHeight = lbDisplay.PhysicalScreenHeight;
    bkpGrphScrHeight = lbDisplay.GraphicsScreenHeight;
    bkpGrphWndwHeight = lbDisplay.GraphicsWindowHeight;
    lbDisplay.PhysicalScreenHeight = 256;
    lbDisplay.GraphicsScreenHeight = 256;
    lbDisplay.GraphicsWindowHeight = 256;

    memset(lbDisplay.WScreen, 0, lbDisplay.PhysicalScreenWidth << 8);

    v23hw = 0;
    v25a = 0;
    v23mw = 0;
    for (v20 = 12; v20 > -1; v20--)
    {
        int base_idx;
        ushort spr;

        base_idx = v20 * 8 * 6;
        for (spr = 0; spr < 4; spr++)
        {
            ushort fr;
            ushort kk;

            fr = nstart_ani[spr + word_154F4C[v20]];
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
            x = 128;
        if (y > 127)
            y = 127;
        if (y < -128)
            y = 128;

        sprshadow_F850[2 * i + 0] = x;
        sprshadow_F850[2 * i + 1] = y;
    }

    lbDisplay.PhysicalScreenHeight = bkpPhysScrHeight;
    lbDisplay.GraphicsScreenHeight = bkpGrphScrHeight;
    lbDisplay.GraphicsWindowHeight = bkpGrphWndwHeight;
}

/******************************************************************************/
