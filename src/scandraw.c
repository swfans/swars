/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file scandraw.c
 *     Ingame scanner (minimap/radar) drawing.
 * @par Purpose:
 *     Implement functions drawing the scanner map elements.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "scandraw.h"

#include "bfanywnd.h"
#include "bfgentab.h"
#include "bfline.h"
#include "bfmath.h"
#include "bfpixel.h"

#include "engintrns.h"
#include "campaign.h"
#include "display.h"
#include "thing.h"
#include "game_speed.h"
#include "game.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/

struct scanstr1 {
    long u;
    long v;
};

struct scanstr2 {
    long du;
    long dv;
};

struct scanstr3 {
    long u1;
    long v1;
    long u2;
    long v2;
};

extern long dword_155340[32];
extern struct scanstr1 SCANNER_bbpoint[255];
extern long dword_1DBB64[];
extern long dword_1DBB6C[512];
extern ulong SCANNER_keep_arcs;
extern ubyte SCANNER_colour[5];
extern long scanner_blink; // = 1;

extern struct scanstr3 SCANNER_arcpoint[20];
extern TbPixel SCANNER_people_colours[15];
extern ubyte byte_1C5C30[8];

void unkn_draw_transformed_point(short x, short y, long ptX, long ptY, long ptZ, ubyte colour)
{
    struct EnginePoint ep;

    ep.X3d = ptX - engn_xc;
    ep.Z3d = ptZ - engn_zc;
    ep.Y3d = ptY - engn_yc;
    ep.Flags = 0;
    transform_point(&ep);
    LbDrawLine(x, y, ep.pp.X, ep.pp.Y, colour);
}

void draw_objective_point(long x, long y, ThingIdx thing, short a4, ubyte colour)
{
    long ptX, ptY, ptZ;

    if (a4)
        return;

    if (thing <= 0) {
        struct SimpleThing *p_sthing;
        p_sthing = &sthings[thing];
        ptX = p_sthing->X;
        ptY = p_sthing->Y;
        ptZ = p_sthing->Z;
    } else {
        struct Thing *p_thing;
        p_thing = &things[thing];
        ptX = p_thing->X;
        ptY = p_thing->Y;
        ptZ = p_thing->Z;
    }
    unkn_draw_transformed_point(x, y, ptX, ptY, ptZ, colour);
}

void func_711F4(short a1, short a2, short a3, short a4, ubyte colour)
{
    asm volatile (
      "push %4\n"
      "call ASM_func_711F4\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (colour));
}

void SCANNER_draw_mark_point5_blink3(int x, int y, TbPixel col)
{
    asm volatile (
      "call ASM_SCANNER_draw_mark_point5_blink3\n"
        : : "a" (x), "d" (y), "b" (col));
}

void SCANNER_draw_mark_point7(int x, int y, TbPixel col)
{
    asm volatile (
      "call ASM_SCANNER_draw_mark_point7\n"
        : : "a" (x), "d" (y), "b" (col));
}

void SCANNER_process_arcpoints(void)
{
    asm volatile (
      "call ASM_SCANNER_process_arcpoints\n"
        :  :  : "eax" );
}

void SCANNER_draw_new_transparent_sub2(void)
{
#if 1
    asm volatile ("call ASM_SCANNER_draw_new_transparent_sub2\n"
        :  :  : "eax" );
    return;
#endif
    int dt_x, dt_y;
    int sh_x, sh_y;
    int range;
    int pos_mx, pos_mz;
    struct TbAnyWindow window_bkp;
    int sc_width;
    short bn;

    int dist_x, dist_z;

    dt_x = (ingame.Scanner.X2 - ingame.Scanner.X1) >> 1;
    dt_y = (ingame.Scanner.Y2 - ingame.Scanner.Y1) >> 1;
    sh_y = (ingame.Scanner.Zoom * lbSinTable[ingame.Scanner.Angle]) >> 8;
    sh_x = (ingame.Scanner.Zoom * lbSinTable[ingame.Scanner.Angle + 512]) >> 8;
    if (dt_x >= dt_y)
        range = (ingame.Scanner.Y2 - ingame.Scanner.Y1) >> 1;
    else
        range = (ingame.Scanner.X2 - ingame.Scanner.X1) >> 1;
    range -= 5;
    pos_mz = (ingame.Scanner.MZ << 16) + sh_x * dt_y - sh_y * dt_x;
    pos_mx = (ingame.Scanner.MX << 16) - sh_x * dt_x - sh_y * dt_y;

    LbScreenStoreGraphicsWindow(&window_bkp);
    LbScreenSetGraphicsWindow(ingame.Scanner.X1, ingame.Scanner.Y1,
      ingame.Scanner.X2 - ingame.Scanner.X1 + 1,
      ingame.Scanner.Y2 - ingame.Scanner.Y1 + 1);

    sc_width = (ingame.Scanner.X2 - ingame.Scanner.X1) - 24;

    for (bn = 0; bn < SCANNER_BIG_BLIP_COUNT; bn++)
    {
        int base_x, base_z;
        int px_x, px_y;
        int base_i, i;
        short gtr;

        if (ingame.Scanner.BigBlip[bn].Period == 0)
            continue;

        {
            int bsh_x, bsh_z;
            u32 tmp;
            int rval_xz, rval_yz, rval_yx, rval_xx, rval_div;
            long prec_x, prec_z;

            bsh_z = 2 * ingame.Scanner.BigBlip[bn].Z - pos_mz;
            bsh_x = 2 * ingame.Scanner.BigBlip[bn].X - pos_mx;

            tmp = (sh_x * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_xz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_yz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_yx = _rotl(tmp, 16);

            tmp = (sh_x * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_xx = _rotl(tmp, 16);

            tmp = (sh_y * sh_y) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)sh_y) >> 32) & 0xFFFF;
            rval_div = _rotl(tmp, 16);
            tmp = (sh_x * sh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)sh_x) >> 32) & 0xFFFF;
            rval_div += _rotl(tmp, 16);

            prec_z = ((rval_yx - (s64)rval_xz) << 16) / rval_div;
            prec_x = ((rval_xx + (s64)rval_yz) << 16) / rval_div;

            base_z = prec_z >> 16;
            base_x = prec_x >> 16;
        }

        gtr = (gameturn - 0) & 0xF;
        px_x = base_x + dword_155340[2 * gtr + 0];
        px_y = base_z + dword_155340[2 * gtr + 1];
        if ((px_y >= 0) && (px_y + ingame.Scanner.Y1 <= ingame.Scanner.Y2)
          && (px_x >= 0) && (px_x <= SCANNER_width[px_y])) {
            LbDrawPixel(px_x, px_y, pixmap.fade_table[0x2000 + ingame.Scanner.BigBlip[bn].Colour]);
        }
        gtr = (gameturn - 1) & 0xF;
        px_x = base_x + dword_155340[2 * gtr + 0];
        px_y = base_z + dword_155340[2 * gtr + 1];
        if ( px_y >= 0 && px_y + ingame.Scanner.Y1 <= ingame.Scanner.Y2
          && px_x >= 0 && px_x <= SCANNER_width[px_y]) {
            LbDrawPixel(px_x, px_y, pixmap.fade_table[0x1800 + ingame.Scanner.BigBlip[bn].Colour]);
        }
        gtr = (gameturn - 2) & 0xF;
        px_x = base_x + dword_155340[2 * gtr + 0];
        px_y = base_z + dword_155340[2 * gtr + 1];
        if ( px_y >= 0 && px_y + ingame.Scanner.Y1 <= ingame.Scanner.Y2
          && px_x >= 0 && px_x <= SCANNER_width[px_y]) {
            LbDrawPixel(px_x, px_y, pixmap.fade_table[0x1000 + ingame.Scanner.BigBlip[bn].Colour]);
        }

        base_i = bn * 16;

        for (i = 0; i < 16; i++)
        {
            int bsh_x, bsh_z;
            u32 tmp;
            int rval_xz, rval_yz, rval_yx, rval_xx, rval_div;
            long prec_x, prec_z;

            bsh_z = 2 * SCANNER_bbpoint[i].u - pos_mz;
            bsh_x = 2 * SCANNER_bbpoint[i].v - pos_mx;

            tmp = (sh_x * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_xz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_yz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_yx = _rotl(tmp, 16);

            tmp = (sh_x * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_xx = _rotl(tmp, 16);

            tmp = (sh_y * sh_y) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)sh_y) >> 32) & 0xFFFF;
            rval_div = _rotl(tmp, 16);
            tmp = (sh_x * sh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)sh_x) >> 32) & 0xFFFF;
            rval_div += _rotl(tmp, 16);

            prec_z = ((rval_yx - (s64)rval_xz) << 16) / rval_div;
            prec_x = ((rval_xx + (s64)rval_yz) << 16) / rval_div;

            dword_1DBB6C[2 * (base_i + i) + 0] = prec_x >> 16;
            dword_1DBB6C[2 * (base_i + i) + 1] = prec_z >> 16;
        }

        for (i = 0; i < 16; i++)
        {
            int ri;
            int x1, y1, x2, y2;

            ri = base_i + i;
            x1 = dword_1DBB6C[2 * ri + 0];
            y1 = dword_1DBB6C[2 * ri + 1];
            if (i == 15)
              ri = base_i + i - 15;
            else
              ri = base_i + i + 1;
            x2 = dword_1DBB6C[2 * ri + 0];
            y2 = dword_1DBB6C[2 * ri + 1];

            if ((x1 - sc_width <= y1) || (x2 - sc_width <= y2))
            {
                s64 v131;
                int v132;
                ushort ft_idx;
                u32 tmp;

                if (x2 - sc_width > y2)
                {
                    dist_x = x2 - x1;
                    dist_z = y2 - y1;

                    v131 = (y1 - x1 + (s64)sc_width) << 32;
                    v132 = v131 / ((dist_x - dist_z) << 16);

                    tmp = (v132 * dist_x) & 0xFFFF0000;
                    tmp |= ((v132 * (s64)dist_x) >> 32) & 0xFFFF;
                    x2 = _rotl(tmp, 16) + x1;

                    tmp = (v132 * dist_z) & 0xFFFF0000;
                    tmp |= ((v132 * (s64)dist_z) >> 32) & 0xFFFF;
                    y2 = _rotl(tmp, 16) + y1;
                }
                else if (x1 - sc_width > y1)
                {
                      dist_x = x1 - x2;
                      dist_z = y1 - y2;

                      v131 = (y2 - x2 + (s64)sc_width) << 32;
                      v132 = v131 / ((dist_x - dist_z) << 16);

                      tmp = (v132 * dist_x) & 0xFFFF0000;
                      tmp |= ((v132 * (s64)dist_x) >> 32) & 0xFFFF;
                      x1 = _rotl(tmp, 16) + x2;

                      tmp = (v132 * dist_z) & 0xFFFF0000;
                      tmp |= ((v132 * (s64)dist_z) >> 32) & 0xFFFF;
                      y1 = y2 + _rotl(tmp, 16);
                }

                ft_idx = 512 * (31 - ingame.Scanner.BigBlip[bn].Counter)
                           + ingame.Scanner.BigBlip[bn].Colour;
                LbDrawLine(x1, y1, x2, y2, pixmap.fade_table[ft_idx]);
            }
        }
    }

    if (!SCANNER_keep_arcs)
    {
        for (bn = 0; bn < SCANNER_ARC_COUNT; bn++)
        {
            ingame.Scanner.Arc[bn].Period = 0;
            ingame.Scanner.Arc[bn].Counter = 0;
        }
    }
    SCANNER_keep_arcs = 0;
    SCANNER_process_arcpoints();

    for (bn = 0; bn < SCANNER_ARC_COUNT; bn++)
    {
        int base_x, base_z;
        int base_i, i;
        int x1, y1, x2, y2;

        if ((ingame.Scanner.Arc[bn].Period == 0) && (ingame.Scanner.Arc[bn].Counter == 0))
            continue;

        {
            int bsh_x, bsh_z;
            u32 tmp;
            int rval_xz, rval_yz, rval_yx, rval_xx, rval_div;
            long prec_x, prec_z;

            bsh_z = 2 * SCANNER_arcpoint[bn].u1 - pos_mz;
            bsh_x = 2 * SCANNER_arcpoint[bn].v1 - pos_mx;

            tmp = (sh_x * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_xz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_yz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_yx = _rotl(tmp, 16);

            tmp = (sh_x * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_xx = _rotl(tmp, 16);

            tmp = (sh_y * sh_y) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)sh_y) >> 32) & 0xFFFF;
            rval_div = _rotl(tmp, 16);
            tmp = (sh_x * sh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)sh_x) >> 32) & 0xFFFF;
            rval_div += _rotl(tmp, 16);

            prec_z = ((rval_yx - (s64)rval_xz) << 16) / rval_div;
            prec_x = ((rval_xx + (s64)rval_yz) << 16) / rval_div;

            base_x = prec_x >> 16;
            base_z = prec_z >> 16;
        }

        base_i = bn * 5;

        for (i = 1; i < 5; i++)
        {
            int bsh_x, bsh_z;
            int ri;
            u32 tmp;
            int rval_xz, rval_yz, rval_yx, rval_xx, rval_div;
            long prec_x, prec_z;
            int ssb_x, ssb_z;

            ri = base_i + i;

            bsh_z = 2 * SCANNER_arcpoint[ri].u1 - pos_mz;
            bsh_x = 2 * SCANNER_arcpoint[ri].v1 - pos_mx;

            tmp = (sh_x * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_xz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_z) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_z) >> 32) & 0xFFFF;
            rval_yz = _rotl(tmp, 16);

            tmp = (sh_y * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_yx = _rotl(tmp, 16);

            tmp = (sh_x * bsh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)bsh_x) >> 32) & 0xFFFF;
            rval_xx = _rotl(tmp, 16);

            tmp = (sh_y * sh_y) & 0xFFFF0000;
            tmp |= ((sh_y * (s64)sh_y) >> 32) & 0xFFFF;
            rval_div = _rotl(tmp, 16);
            tmp = (sh_x * sh_x) & 0xFFFF0000;
            tmp |= ((sh_x * (s64)sh_x) >> 32) & 0xFFFF;
            rval_div += _rotl(tmp, 16);

            prec_z = ((rval_yx - (s64)rval_xz) << 16) / rval_div;
            prec_x = ((rval_xx + (s64)rval_yz) << 16) / rval_div;

            ssb_z = prec_z >> 16;
            ssb_x = prec_x >> 16;

            x1 = base_x;
            y1 = base_z;
            x2 = ssb_x;
            y2 = ssb_z;

            if ((x1 - sc_width <= y1) || (x2 - sc_width <= y2))
            {
                s64 v160;
                int v161;
                u32 tmp;

                if (x2 - sc_width > y2)
                {
                    dist_x = x2 - x1;
                    dist_z = y2 - y1;
                    v160 = (y1 - x1 + (s64)sc_width) << 32;
                    v161 = v160 / ((dist_x - dist_z) << 16);

                    tmp = (v161 * dist_x) & 0xFFFF0000;
                    tmp |= ((v161 * (s64)dist_x) >> 32) & 0xFFFF;
                    x2 = _rotl(tmp, 16) + x1;

                    tmp = (v161 * dist_z) & 0xFFFF0000;
                    tmp |= ((v161 * (s64)dist_z) >> 32) & 0xFFFF;
                    y2 = _rotl(tmp, 16) + y1;
                }
                else if (x1 - sc_width > y1)
                {
                    dist_x = x1 - x2;
                    dist_z = y1 - y2;
                    v160 = (y2 - x2 + (s64)sc_width) << 32;
                    v161 = v160 / ((dist_x - dist_z) << 16);

                    tmp = (v161 * dist_x) & 0xFFFF0000;
                    tmp |= (((v160 / ((dist_x - dist_z) << 16)) * (s64)dist_x) >> 32) & 0xFFFF;
                    x1 = _rotl(tmp, 16) + x2;

                    tmp = (v161 * dist_z) & 0xFFFF0000;
                    tmp |= ((v161 * (s64)dist_z) >> 32) & 0xFFFF;
                    y1 = _rotl(tmp, 16) + y2;
                }
                LbDrawLine(x1, y1, x2, y2, ingame.Scanner.Arc[bn].ColourIsUnused);
            }

            base_x = ssb_x;
            base_z = ssb_z;
        }
    }

    LbScreenLoadGraphicsWindow(&window_bkp);

    //TODO add missing part

    LbDrawLine(ingame.Scanner.X1 - 1, ingame.Scanner.Y1 - 1,
      ingame.Scanner.X1 - 1, ingame.Scanner.Y2 + 1, SCANNER_colour[4]);
    LbDrawLine(ingame.Scanner.X1 - 1, ingame.Scanner.Y1 - 1,
      ingame.Scanner.X2 - 24, ingame.Scanner.Y1 - 1, SCANNER_colour[4]);
    LbDrawLine(ingame.Scanner.X1 - 1, ingame.Scanner.Y2 + 1,
      ingame.Scanner.X2, ingame.Scanner.Y2 + 1, SCANNER_colour[4]);
    LbDrawLine(ingame.Scanner.X2 - 24, ingame.Scanner.Y1 - 1,
      ingame.Scanner.X2 + 1, ingame.Scanner.Y1 + 24, SCANNER_colour[4]);
    LbDrawLine(ingame.Scanner.X2 + 1, ingame.Scanner.Y1 + 24,
      ingame.Scanner.X2 + 1, ingame.Scanner.Y2 + 1, SCANNER_colour[4]);
}

void SCANNER_draw_new_transparent(void)
{
    asm volatile ("call ASM_SCANNER_draw_new_transparent_sub1\n"
        :  :  : "eax" );
    SCANNER_draw_new_transparent_sub2();
}

/******************************************************************************/
