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
#include "bfendian.h"
#include "bfgentab.h"
#include "bfkeybd.h"
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

struct NearestPos {
    ulong dist;
    short x;
    short y;
};

/** String of keycodes for special input.
 * Cannot be of type TbKeyCode as contains plain numeric value as well.
 */
const int scanner_keys[] = {
    KC_NUMPAD3, KC_DECIMAL, KC_NUMPAD1, KC_NUMPAD4,
    KC_NUMPAD1, KC_NUMPAD5, KC_NUMPAD9, KC_NUMPAD2,
    KC_NUMPAD6, KC_NUMPAD5, KC_NUMPAD3, KC_NUMPAD5,
    9999,
};

extern long scanner_next_key_no;

extern long SCANNER_dw064;
extern long SCANNER_dw068;
extern long SCANNER_dw06C;
extern long SCANNER_dw070;
extern long SCANNER_dw074;
extern long SCANNER_dw07C;
extern long SCANNER_dw080;

extern ubyte SCANNER_bt084;
extern ubyte SCANNER_bt085;
extern ubyte SCANNER_brig;
extern ubyte SCANNER_cont;

extern long dword_155340[32];
extern struct scanstr1 SCANNER_bbpoint[255];
extern long dword_1DBB64[];
extern long dword_1DBB6C[512];
extern TbPixel *SCANNER_screenptr;
extern ulong SCANNER_keep_arcs;
extern ubyte SCANNER_colour[5];
extern long scanner_blink; // = 1;

extern struct scanstr3 SCANNER_arcpoint[20];
extern TbPixel SCANNER_people_colours[15];
extern ubyte byte_1C5C30[8];

void SCANNER_process_special_input(void)
{
    ushort sckey, nxkey;

    sckey = scanner_keys[scanner_next_key_no];
    if (lbKeyOn[sckey])
    {
        lbKeyOn[sckey] = 0;
        nxkey = scanner_keys[++scanner_next_key_no];
        if (nxkey == 9999)
        {
            scanner_next_key_no = 0;
            scanner_blink = scanner_blink ^ 1;
        }
    }
}

void SCANNER_dnt_SCANNER_dw070_update(ushort flags1)
{
    s64 nm_prec;
    int nm_shft;
    int pv_val, mn_val, mn_of2;
    int n1_cand, n2_cand;

    n1_cand = 0xFFFFFFF;
    n2_cand = 0xFFFFFFF;

    if ((flags1 & 0x01) != 0)
    {
        nm_prec = (s64)SCANNER_dw06C << 16;
        nm_shft = -(nm_prec / SCANNER_dw064);
        n2_cand = nm_shft >> 16;
    }
    else if ((flags1 & 0x02) != 0)
    {
        nm_prec = (0x1000000 - (s64)SCANNER_dw06C) << 16;
        nm_shft = nm_prec / SCANNER_dw064;
        n2_cand = nm_shft >> 16;
    }

    if ( (flags1 & 0x04) != 0 )
    {
        nm_prec = (s64)SCANNER_dw070 << 16;
        nm_shft = -(nm_prec / SCANNER_dw068);
        n1_cand = nm_shft >> 16;
    }
    else if ((flags1 & 0x08) != 0)
    {
        nm_prec = (0x1000000 - (s64)SCANNER_dw070) << 16;
        nm_shft = nm_prec / SCANNER_dw068;
        n1_cand = nm_shft >> 16;
    }

    // set SCANNER_dw074 = min(SCANNER_dw074, n2_cand, n1_cand);
    pv_val = SCANNER_dw074;
    if (n1_cand >= pv_val)
        mn_of2 = pv_val;
    else
        mn_of2 = n1_cand;
    if (mn_of2 > n2_cand)
      mn_val = n2_cand;
    else if (n1_cand >= pv_val)
        mn_val = pv_val;
    else
        mn_val = n1_cand;
    SCANNER_dw074 = mn_val;
}

void SCANNER_process_bbpoints(void)
{
    asm volatile (
      "call ASM_SCANNER_process_bbpoints\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub1(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub1\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub2(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub2\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub3(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub3\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub4(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub4\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub5(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub5\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub6(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub6\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub7(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub7\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub8(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub8\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub9(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub9\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub10(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub10\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub11(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub11\n"
        :  :  : "eax" );
}

void SCANNER_dnt_sub1_sub12(void)
{
    asm volatile (
      "call ASM_SCANNER_dnt_sub1_sub12\n"
        :  :  : "eax" );
}

void SCANNER_draw_new_transparent_map_line(int cu_x2, int cu_y2)
{
    ushort flags1, flags2;

    flags1 = (0x01 * (cu_y2 < 0))
           | (0x02 * (cu_y2 >= 0x1000000))
           | (0x04 * (cu_x2 < 0))
           | (0x08 * (cu_x2 >= 0x1000000));

    while ( 2 )
    {
        int dt_val, pv_val, cu_val;

        flags2 = (0x01 * (SCANNER_dw06C < 0))
               | (0x02 * (SCANNER_dw06C >= 0x1000000))
               | (0x04 * (SCANNER_dw070 < 0))
               | (0x08 * (SCANNER_dw070 >= 0x1000000));

        if ((flags1 | flags2) == 0)
        {
            SCANNER_dnt_sub1_sub12();
            break;
        }
        if ((flags1 & flags2) != 0)
        {
            SCANNER_dnt_sub1_sub1();
            break;
        }

        switch (flags2)
        {
        case 0x00:
            pv_val = SCANNER_dw074;
            SCANNER_dnt_SCANNER_dw070_update(flags1);
            cu_val = SCANNER_dw074;
            dt_val = pv_val - cu_val;
            if ((cu_val > 0) && (cu_val <= 400)) {
                SCANNER_dnt_sub1_sub2();
            }
            if ((dt_val > 0) && (dt_val <= 400)) {
                SCANNER_dw074 = dt_val;
                SCANNER_dnt_sub1_sub3();
            }
            break;
        case 0x01:
            SCANNER_dnt_sub1_sub4();
            if (SCANNER_dw074)
                continue;
            break;
        case 0x02:
            SCANNER_dnt_sub1_sub5();
            if (SCANNER_dw074)
                continue;
            break;
        case 0x04:
            SCANNER_dnt_sub1_sub6();
            if (SCANNER_dw074)
                continue;
            break;
        case 0x05:
            SCANNER_dnt_sub1_sub8();
            if (SCANNER_dw074)
                continue;
            break;
        case 0x06:
            SCANNER_dnt_sub1_sub9();
            if (SCANNER_dw074)
                continue;
            break;
        case 0x08:
            SCANNER_dnt_sub1_sub7();
            if (SCANNER_dw074)
                continue;
            break;
        case 0x09:
            SCANNER_dnt_sub1_sub10();
            if (SCANNER_dw074)
                continue;
            break;
        case 0x0A:
            SCANNER_dnt_sub1_sub11();
            if (SCANNER_dw074)
                continue;
            break;
        default:
            break;
        }
        break;
    }
}

void SCANNER_draw_new_transparent_map(void)
{
#if 0
    asm volatile ("call ASM_SCANNER_draw_new_transparent_map\n"
        :  :  : "eax" );
    return;
#endif
    int dt_x, dt_y;
    int sh_x, sh_y;
    int cu_x1, cu_y1, cu_x2, cu_y2;
    long *p_width;
    TbPixel *p_out;
    int cu_y;

    dt_x = (ingame.Scanner.X2 - ingame.Scanner.X1) >> 1;
    dt_y = (ingame.Scanner.Y2 - ingame.Scanner.Y1) >> 1;
    sh_y = (ingame.Scanner.Zoom * lbSinTable[ingame.Scanner.Angle]) >> 8;
    sh_x = (ingame.Scanner.Zoom * lbSinTable[ingame.Scanner.Angle + 512]) >> 8;

    SCANNER_dw07C = sh_y << 16;
    SCANNER_dw080 = sh_x << 16;
    SCANNER_bt084 = (sh_y >> 16);
    SCANNER_bt085 = (sh_x >> 16);
    SCANNER_brig = ingame.Scanner.Brightness;
    SCANNER_cont = ingame.Scanner.Contrast;
    SCANNER_dw064 = sh_y;
    SCANNER_dw068 = sh_x;

    cu_y2 = (ingame.Scanner.MZ << 16) + sh_y * dt_x + sh_x * dt_y;
    cu_x2 = (ingame.Scanner.MX << 16) + sh_x * dt_x - sh_y * dt_y;
    cu_x1 = (ingame.Scanner.MZ << 16) + sh_x * dt_y - sh_y * dt_x;
    cu_y1 = (ingame.Scanner.MX << 16) - sh_x * dt_x - sh_y * dt_y;
    p_width = SCANNER_width;
    p_out = &lbDisplay.WScreen[ingame.Scanner.X1 + lbDisplay.PhysicalScreenWidth * ingame.Scanner.Y1];

    for (cu_y = ingame.Scanner.Y1; cu_y <= ingame.Scanner.Y2; cu_y++)
    {
        SCANNER_dw06C = cu_x1;
        SCANNER_dw070 = cu_y1;
        SCANNER_dw074 = *p_width + 1;
        SCANNER_screenptr = p_out;

        SCANNER_draw_new_transparent_map_line(cu_x2, cu_y2);

        p_width++;
        p_out += lbDisplay.PhysicalScreenWidth;
        cu_y1 += sh_y;
        cu_x1 -= sh_x;
        cu_y2 -= sh_x;
        cu_x2 += sh_y;
    }
}

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

void SCANNER_draw_mark_point(int x, int y, ushort sz, TbPixel col, TbBool filled)
{
    int h;

    switch (sz)
    {
    default:
        break;
    case 7:
    case 8:
        if (x > ingame.Scanner.X1 + 2)
        {
            if (y > ingame.Scanner.Y1)
                LbDrawPixel(x - 3, y - 2, col);
            LbDrawPixel(x - 3, y, col);
            if (y < ingame.Scanner.Y2)
                LbDrawPixel(x - 3, y + 2, col);
        }
        h = y - ingame.Scanner.Y1;
        if (y < ingame.Scanner.Y2 - 2)
        {
            if (x > ingame.Scanner.X1)
                LbDrawPixel(x - 2, y + 3, col);
            LbDrawPixel(x, y + 3, col);
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h+3] + 0)
                LbDrawPixel(x + 2, y + 3, col);
        }
        if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h] - 1)
        {
            if ((y > ingame.Scanner.Y1 + 1) && (x < ingame.Scanner.X1 + ingame.Scanner.Width[h-2] - 2))
                LbDrawPixel(x + 3, y - 2, col);
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h] - 1)
                LbDrawPixel(x + 3, y,     col);
        }
        if ((y < ingame.Scanner.Y2) && x < (ingame.Scanner.X1 + ingame.Scanner.Width[h+2] - 1))
        {
            LbDrawPixel(x + 3, y + 2, col);
        }
        if (y > ingame.Scanner.Y1 + 2)
        {
            if (x > ingame.Scanner.X1)
                LbDrawPixel(x - 2, y - 3, col);
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h-3] + 1)
                LbDrawPixel(x,     y - 3, col);
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h-3] + 0)
                LbDrawPixel(x + 2, y - 3, col);
        }
        if (!filled)
            break;
        //  fall through
    case 6:
    case 5:
        if (x > ingame.Scanner.X1 + 1)
        {
            LbDrawPixel(x - 2, y - 1, col);
            LbDrawPixel(x - 2, y,     col);
            LbDrawPixel(x - 2, y + 1, col);
        }
        if (y < ingame.Scanner.Y2 - 2)
        {
            LbDrawPixel(x - 1, y + 2, col);
            LbDrawPixel(x,     y + 2, col);
            LbDrawPixel(x + 1, y + 2, col);
        }
        h = y - ingame.Scanner.Y1;
        if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h])
        {
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h-1])
                LbDrawPixel(x + 2, y - 1, col);
            LbDrawPixel(x + 2, y, col);
            LbDrawPixel(x + 2, y + 1, col);
        }
        if (y > ingame.Scanner.Y1 + 1)
        {
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h-2] + 2)
                LbDrawPixel(x - 1, y - 2, col);
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h-2] + 1)
                LbDrawPixel(x,     y - 2, col);
            if (x < ingame.Scanner.X1 + ingame.Scanner.Width[h-2] + 0)
                LbDrawPixel(x + 1, y - 2, col);
        }
        if (!filled)
            break;
        //  fall through
    case 4:
    case 3:
        LbDrawPixel(x, y - 1, col);
        LbDrawPixel(x, y + 1, col);
        LbDrawPixel(x - 1, y, col);
        LbDrawPixel(x + 1, y, col);
        if (!filled)
            break;
        //  fall through
    case 2:
    case 1:
        LbDrawPixel(x, y, col);
        break;
    case 0:
        break;
    }
}

void SCANNER_draw_mark_point5_blink3(int x, int y, TbPixel col)
{
#if 0
    asm volatile (
      "call ASM_SCANNER_draw_mark_point5_blink3\n"
        : : "a" (x), "d" (y), "b" (col));
#endif
    ushort frame;

    frame = gameturn & 3;
    SCANNER_draw_mark_point(x, y, 2 * frame + 1, col, false);
}

void SCANNER_draw_mark_point7(int x, int y, TbPixel col)
{
#if 0
    asm volatile (
      "call ASM_SCANNER_draw_mark_point7\n"
        : : "a" (x), "d" (y), "b" (col));
#endif
    SCANNER_draw_mark_point(x, y, 7, col, false);
}

void SCANNER_process_arcpoints(void)
{
    asm volatile (
      "call ASM_SCANNER_process_arcpoints\n"
        :  :  : "eax" );
}

void SCANNER_draw_orientation_arrow(int pos_x1, int pos_y1, int range, int angle)
{
        int x1, y1, x2, y2, len_x, len_y;

        x1 = ((range * lbSinTable[angle]) >> 16) + pos_x1;
        len_y = (range * lbSinTable[angle]) >> 19;
        y1 = ((range * -lbSinTable[angle + 512]) >> 16) + pos_y1;
        len_x = (range * -lbSinTable[angle + 512]) >> 19;
        x2 = x1 - len_y;
        y2 = y1 - len_x;
        LbDrawLine(x1, y1, x2 - len_x, y2 + len_y, colour_lookup[1]);
        LbDrawLine(x1, y1, x2 + len_x, y2 - len_y, colour_lookup[1]);
}

TbBool thing_visible_on_scanner(struct Thing *p_thing)
{
    TbBool ret;

    if ((p_thing->Flag2 & 0x21000002) != 0)
        return false;

    switch (p_thing->Type)
    {
    case TT_VEHICLE:
        ret = true;
        break;
    case TT_PERSON:
        ret = true;
        break;
    case TT_MINE:
        ret = (p_thing->SubType == 48);
        break;
    default:
        ret = false;
        break;
    }
    return ret;
}

TbBool thing_visible_on_scanner_this_turn(struct Thing *p_thing)
{
    TbBool ret;
    if ((gameturn & 8) != 0)
    {
        switch (p_thing->Type)
        {
        case TT_PERSON:
            ret = (p_thing->Flag & 0x10000000) == 0;
            break;
        default:
            ret = true;
            break;
        }
    } else
    {
        switch (p_thing->Type)
        {
        case TT_VEHICLE:
            ret = p_thing->U.UVehicle.PassengerHead == 0;
            break;
        default:
            ret = true;
            break;
        }
    }
    return ret;
}

static void scanner_coords_line_clip(int *x1, int *y1, int *x2, int *y2, int sc_width)
{
    int dist_x, dist_z;
    s64 magn;
    int factr;
    u32 tmp;

    if (*x2 - sc_width > *y2)
    {
        dist_x = *x2 - *x1;
        dist_z = *y2 - *y1;

        magn = (*y1 - *x1 + (s64)sc_width) << 32;
        factr = magn / ((dist_x - dist_z) << 16);

        tmp = (factr * dist_x) & 0xFFFF0000;
        tmp |= ((factr * (s64)dist_x) >> 32) & 0xFFFF;
        *x2 = bw_rotl32(tmp, 16) + *x1;

        tmp = (factr * dist_z) & 0xFFFF0000;
        tmp |= ((factr * (s64)dist_z) >> 32) & 0xFFFF;
        *y2 = bw_rotl32(tmp, 16) + *y1;
    }
    else if (*x1 - sc_width > *y1)
    {
        dist_x = *x1 - *x2;
        dist_z = *y1 - *y2;

        magn = (*y2 - *x2 + (s64)sc_width) << 32;
        factr = magn / ((dist_x - dist_z) << 16);

        tmp = (factr * dist_x) & 0xFFFF0000;
        tmp |= ((factr * (s64)dist_x) >> 32) & 0xFFFF;
        *x1 = bw_rotl32(tmp, 16) + *x2;

        tmp = (factr * dist_z) & 0xFFFF0000;
        tmp |= ((factr * (s64)dist_z) >> 32) & 0xFFFF;
        *y1 = bw_rotl32(tmp, 16) + *y2;
    }
}

static void map_coords_to_scanner(int *sc_x, int *sc_y, int sh_x, int sh_y, int bsh_x, int bsh_y)
{
    u32 tmp;
    int rval_xy, rval_yy, rval_yx, rval_xx, rval_div;
    long prec_x, prec_y;

    tmp = (sh_x * bsh_y) & 0xFFFF0000;
    tmp |= ((sh_x * (s64)bsh_y) >> 32) & 0xFFFF;
    rval_xy = bw_rotl32(tmp, 16);

    tmp = (sh_y * bsh_y) & 0xFFFF0000;
    tmp |= ((sh_y * (s64)bsh_y) >> 32) & 0xFFFF;
    rval_yy = bw_rotl32(tmp, 16);

    tmp = (sh_y * bsh_x) & 0xFFFF0000;
    tmp |= ((sh_y * (s64)bsh_x) >> 32) & 0xFFFF;
    rval_yx = bw_rotl32(tmp, 16);

    tmp = (sh_x * bsh_x) & 0xFFFF0000;
    tmp |= ((sh_x * (s64)bsh_x) >> 32) & 0xFFFF;
    rval_xx = bw_rotl32(tmp, 16);

    tmp = (sh_y * sh_y) & 0xFFFF0000;
    tmp |= ((sh_y * (s64)sh_y) >> 32) & 0xFFFF;
    rval_div = bw_rotl32(tmp, 16);
    tmp = (sh_x * sh_x) & 0xFFFF0000;
    tmp |= ((sh_x * (s64)sh_x) >> 32) & 0xFFFF;
    rval_div += bw_rotl32(tmp, 16);

    prec_y = ((rval_yx - (s64)rval_xy) << 16) / rval_div;
    prec_x = ((rval_xx + (s64)rval_yy) << 16) / rval_div;

    *sc_y = prec_y >> 16;
    *sc_x = prec_x >> 16;
}

void SCANNER_draw_blips(int pos_mx, int pos_mz, int sh_x, int sh_y)
{
    short bn;
    int sc_width;

    sc_width = (ingame.Scanner.X2 - ingame.Scanner.X1) - 24;

    for (bn = 0; bn < SCANNER_BIG_BLIP_COUNT; bn++)
    {
        int base_x, base_y;
        int px_x, px_y;
        int base_i, i;
        short gtr;

        if (ingame.Scanner.BigBlip[bn].Period == 0)
            continue;

        {
            int bsh_x, bsh_y;

            bsh_y = 2 * ingame.Scanner.BigBlip[bn].Z - pos_mz;
            bsh_x = 2 * ingame.Scanner.BigBlip[bn].X - pos_mx;
            map_coords_to_scanner(&base_x, &base_y, sh_x, sh_y, bsh_x, bsh_y);
        }

        gtr = (gameturn - 0) & 0xF;
        px_x = base_x + dword_155340[2 * gtr + 0];
        px_y = base_y + dword_155340[2 * gtr + 1];
        if ((px_y >= 0) && (px_y + ingame.Scanner.Y1 <= ingame.Scanner.Y2)
          && (px_x >= 0) && (px_x <= SCANNER_width[px_y])) {
            LbDrawPixel(px_x, px_y, pixmap.fade_table[0x2000 + ingame.Scanner.BigBlip[bn].Colour]);
        }
        gtr = (gameturn - 1) & 0xF;
        px_x = base_x + dword_155340[2 * gtr + 0];
        px_y = base_y + dword_155340[2 * gtr + 1];
        if ( px_y >= 0 && px_y + ingame.Scanner.Y1 <= ingame.Scanner.Y2
          && px_x >= 0 && px_x <= SCANNER_width[px_y]) {
            LbDrawPixel(px_x, px_y, pixmap.fade_table[0x1800 + ingame.Scanner.BigBlip[bn].Colour]);
        }
        gtr = (gameturn - 2) & 0xF;
        px_x = base_x + dword_155340[2 * gtr + 0];
        px_y = base_y + dword_155340[2 * gtr + 1];
        if ( px_y >= 0 && px_y + ingame.Scanner.Y1 <= ingame.Scanner.Y2
          && px_x >= 0 && px_x <= SCANNER_width[px_y]) {
            LbDrawPixel(px_x, px_y, pixmap.fade_table[0x1000 + ingame.Scanner.BigBlip[bn].Colour]);
        }

        base_i = bn * 16;

        for (i = 0; i < 16; i++)
        {
            int bsh_x, bsh_y;
            int sc_x, sc_y;

            bsh_y = 2 * SCANNER_bbpoint[i].u - pos_mz;
            bsh_x = 2 * SCANNER_bbpoint[i].v - pos_mx;
            map_coords_to_scanner(&sc_x, &sc_y, sh_x, sh_y, bsh_x, bsh_y);

            dword_1DBB6C[2 * (base_i + i) + 0] = sc_x;
            dword_1DBB6C[2 * (base_i + i) + 1] = sc_y;
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
                ushort ft_idx;

                scanner_coords_line_clip(&x1, &y1, &x2, &y2, sc_width);
                ft_idx = 512 * (31 - ingame.Scanner.BigBlip[bn].Counter)
                           + ingame.Scanner.BigBlip[bn].Colour;
                LbDrawLine(x1, y1, x2, y2, pixmap.fade_table[ft_idx]);
            }
        }
    }
}

void SCANNER_draw_arcs(int pos_mx, int pos_mz, int sh_x, int sh_y)
{
    short bn;
    int sc_width;

    sc_width = (ingame.Scanner.X2 - ingame.Scanner.X1) - 24;

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
        int base_x, base_y;
        int base_i, i;
        int x1, y1, x2, y2;

        if ((ingame.Scanner.Arc[bn].Period == 0) && (ingame.Scanner.Arc[bn].Counter == 0))
            continue;

        {
            int bsh_x, bsh_y;

            bsh_y = 2 * SCANNER_arcpoint[bn].u1 - pos_mz;
            bsh_x = 2 * SCANNER_arcpoint[bn].v1 - pos_mx;
            map_coords_to_scanner(&base_x, &base_y, sh_x, sh_y, bsh_x, bsh_y);
        }

        base_i = bn * 5;

        for (i = 1; i < 5; i++)
        {
            int bsh_x, bsh_y;
            int ri;
            int ssb_x, ssb_y;

            ri = base_i + i;

            bsh_y = 2 * SCANNER_arcpoint[ri].u1 - pos_mz;
            bsh_x = 2 * SCANNER_arcpoint[ri].v1 - pos_mx;
            map_coords_to_scanner(&ssb_x, &ssb_y, sh_x, sh_y, bsh_x, bsh_y);

            x1 = base_x;
            y1 = base_y;
            x2 = ssb_x;
            y2 = ssb_y;

            if ((x1 - sc_width <= y1) || (x2 - sc_width <= y2))
            {
                scanner_coords_line_clip(&x1, &y1, &x2, &y2, sc_width);
                LbDrawLine(x1, y1, x2, y2, ingame.Scanner.Arc[bn].ColourIsUnused);
            }

            base_x = ssb_x;
            base_y = ssb_y;
        }
    }
}

void SCANNER_check_nearest(struct Thing *p_thing, struct NearestPos *p_nearest, int base_x, int base_y, int pos_x1, int pos_y1)
{
    sbyte group_col;

    if (scanner_blink)
        return;

    int dist_x, dist_y;
    ulong dist;
    int i;

    group_col = 0;
    for (i = 0; i < ingame.Scanner.GroupCount; ++i)
    {
        if (ingame.Scanner.Group[i] == p_thing->U.UObject.EffectiveGroup) {
            group_col = ingame.Scanner.GroupCol[i];
            break;
        }
    }
    dist_x = ingame.Scanner.X1 + base_x - pos_x1;
    dist_y = ingame.Scanner.Y1 + base_y - pos_y1;
    dist = dist_x * dist_x + dist_y * dist_y;
    if (group_col  && dist < p_nearest->dist)
    {
        p_nearest->x = dist_x;
        p_nearest->y = dist_y;
        p_nearest->dist = dist;
    }
}

TbPixel SCANNER_thing_colour(struct Thing *p_thing)
{
    TbPixel col;

    switch (p_thing->Type)
    {
    case TT_VEHICLE:
        col = 1;
        break;
    case TT_PERSON:
        if ((p_thing->Flag & 0x80000) != 0)
            col = colour_lookup[5];
        else if ( (p_thing->Flag & 0x2000) != 0 && p_thing->U.UPerson.CurrentWeapon == WEP_CLONESHLD)
            col = SCANNER_people_colours[4];
        else
            col = SCANNER_people_colours[p_thing->SubType];
        break;
    case TT_MINE:
        col = 1;
        break;
    default:
        col = 0;
        break;
    }
    return col;
}

#define __CFSHL__(a, b)

void SCANNER_draw_thing(struct Thing *p_thing, struct NearestPos *p_nearest, int pos_mx, int pos_mz, int sh_x, int sh_y, int pos_x1, int pos_y1)
{
    int base_x, base_y;
    int x, y;
    TbPixel col;

    {
        int tng_z, tng_x;
        int bsh_x, bsh_y;

        if ((p_thing->Type == TT_PERSON) && ((p_thing->Flag & 0x4000) != 0)) {
            struct Thing *p_vehicle;

            p_vehicle = &things[p_thing->U.UPerson.Vehicle];
            tng_z = p_vehicle->Z;
            tng_x = p_vehicle->X;
        } else {
            tng_z = p_thing->Z;
            tng_x = p_thing->X;
        }
        tng_z = ((tng_z << 8) - (__CFSHL__(tng_z << 8 >> 31, 7) + (tng_z << 8 >> 31 << 7))) >> 7;
        tng_x = ((tng_x << 8) - (__CFSHL__(tng_x << 8 >> 31, 7) + (tng_x << 8 >> 31 << 7))) >> 7;
        bsh_y = tng_z - pos_mz;
        bsh_x = tng_x - pos_mx;
        map_coords_to_scanner(&base_x, &base_y, sh_x, sh_y, bsh_x, bsh_y);
    }

    SCANNER_check_nearest(p_thing, p_nearest, base_x, base_y, pos_x1, pos_y1);

    if ((base_y < 0) || (ingame.Scanner.Y1 + base_y > ingame.Scanner.Y2)
      || (base_x < 0) || (base_x > SCANNER_width[base_y]))
    {
        return;
    }

    col = SCANNER_thing_colour(p_thing);

    if (col == 0)
        return;

    x = ingame.Scanner.X1 + base_x;
    y = ingame.Scanner.Y1 + base_y;
    if ((p_thing->Flag & 0x02) == 0)
    {
        if (((p_thing->Flag & 0x2000) == 0) || (p_thing->U.UPerson.CurrentWeapon == WEP_CLONESHLD))
        {
            LbDrawPixel(x, y, col);
            if ((gameturn & 1) != 0)
            {
                LbDrawPixel(x - 1, y, col);
                LbDrawPixel(x + 1, y, col);
                LbDrawPixel(x, y + 1, col);
                LbDrawPixel(x, y - 1, col);
            }
        }
        else
        {
            if (in_network_game)
                SCANNER_draw_mark_point5_blink3(x, y, byte_1C5C30[p_thing->U.UPerson.ComCur >> 2]);
            else
                SCANNER_draw_mark_point5_blink3(x, y, col);
        }
    }
    if ((gameturn & 7) == 0)
    {
        int i;
        TbPixel gcol;

        gcol = 0;
        for (i = 0; i < ingame.Scanner.GroupCount; i++)
        {
            if (ingame.Scanner.Group[i] == p_thing->U.UPerson.EffectiveGroup) {
                gcol = ingame.Scanner.GroupCol[i];
                break;
            }
        }
        if (gcol != 0) {
              SCANNER_draw_mark_point7(x, y, gcol);
        }
    }
}

void SCANNER_draw_sthing(struct SimpleThing *p_sthing, int pos_mx, int pos_mz, int sh_x, int sh_y)
{
    int base_x, base_y;
    int x, y;
    TbPixel col;

    {
        int tng_z, tng_x;
        int bsh_x, bsh_y;

        tng_z = p_sthing->Z;
        tng_x = p_sthing->X;
        tng_z = (((tng_z << 8) - (__CFSHL__(tng_z << 8 >> 31, 7) + (tng_z << 8 >> 31 << 7))) >> 7);
        tng_x = (((tng_x << 8) - (__CFSHL__(tng_x << 8 >> 31, 7) + (tng_x << 8 >> 31 << 7))) >> 7);
        bsh_y = tng_z - pos_mz;
        bsh_x = tng_x - pos_mx;
        map_coords_to_scanner(&base_x, &base_y, sh_x, sh_y, bsh_x, bsh_y);
    }

    x = ingame.Scanner.X1 + base_x;
    y = ingame.Scanner.Y1 + base_y;

    col = p_sthing->U.UEffect.VX ? colour_lookup[8] : colour_lookup[5];

    if ((base_y >= 0) && (base_y <= ingame.Scanner.Y2 - ingame.Scanner.Y1)
      && (base_x >= 0) && (base_x <= SCANNER_width[base_y]))
    {
        LbDrawPixel(x, y, col);
    }
}

void SCANNER_draw_things_dots(int pos_mx, int pos_mz, int sh_x, int sh_y, int pos_x1, int pos_y1, int range)
{
    struct Thing *p_thing;
    struct SimpleThing *p_sthing;
    struct NearestPos nearest;
    long limit;
    short thing;

    nearest.dist = 0x7FFFFFFF;
    limit = things_used;
    for (thing = things_used_head; thing > 0; thing = p_thing->LinkChild)
    {
        if (--limit == -1)
            break;
        p_thing = &things[thing];

        if (!thing_visible_on_scanner(p_thing))
            continue;

        if (thing_visible_on_scanner_this_turn(p_thing)) {
            SCANNER_draw_thing(p_thing, &nearest, pos_mx, pos_mz, sh_x, sh_y, pos_x1, pos_y1);
        }
    }

    limit = sthings_used;
    for (thing = sthings_used_head; thing < 0; thing = p_sthing->LinkChild)
    {
        if (--limit == -1)
            break;
        p_sthing = &sthings[thing];

        if (p_sthing->Type != SmTT_DROPPED_ITEM)
            continue;

        SCANNER_draw_sthing(p_sthing, pos_mx, pos_mz, sh_x, sh_y);
    }

    if (scanner_blink)
    {
        SCANNER_draw_orientation_arrow(pos_x1, pos_y1, range, ingame.Scanner.Angle);
    }
    else if ((nearest.dist > range * range) && (nearest.dist != 0x7FFFFFFF))
    {
        long angle;
        angle = arctan(nearest.x, nearest.y);
        SCANNER_draw_orientation_arrow(pos_x1, pos_y1, range, angle);
    }
}

void SCANNER_draw_area_frame(void)
{
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

void SCANNER_draw_signals(void)
{
#if 0
    asm volatile ("call ASM_SCANNER_draw_signals\n"
        :  :  : "eax" );
    return;
#endif
    int sh_x, sh_y;
    int range;
    int pos_mx, pos_mz;
    int pos_x1, pos_y1;
    struct TbAnyWindow window_bkp;

    {
        int dt_x, dt_y;

        dt_x = (ingame.Scanner.X2 - ingame.Scanner.X1) >> 1;
        dt_y = (ingame.Scanner.Y2 - ingame.Scanner.Y1) >> 1;
        sh_y = (ingame.Scanner.Zoom * lbSinTable[ingame.Scanner.Angle]) >> 8;
        sh_x = (ingame.Scanner.Zoom * lbSinTable[ingame.Scanner.Angle + 512]) >> 8;
        pos_x1 = ingame.Scanner.X1 + dt_x;
        pos_y1 = ingame.Scanner.Y1 + dt_y;
        if (dt_x >= dt_y)
            range = (ingame.Scanner.Y2 - ingame.Scanner.Y1) >> 1;
        else
            range = (ingame.Scanner.X2 - ingame.Scanner.X1) >> 1;
        range -= 5;
        pos_mz = (ingame.Scanner.MZ << 16) + sh_x * dt_y - sh_y * dt_x;
        pos_mx = (ingame.Scanner.MX << 16) - sh_x * dt_x - sh_y * dt_y;
    }

    LbScreenStoreGraphicsWindow(&window_bkp);
    LbScreenSetGraphicsWindow(ingame.Scanner.X1, ingame.Scanner.Y1,
      ingame.Scanner.X2 - ingame.Scanner.X1 + 1,
      ingame.Scanner.Y2 - ingame.Scanner.Y1 + 1);

    SCANNER_draw_blips(pos_mx, pos_mz, sh_x, sh_y);

    SCANNER_draw_arcs(pos_mx, pos_mz, sh_x, sh_y);

    LbScreenLoadGraphicsWindow(&window_bkp);

    SCANNER_draw_things_dots(pos_mx, pos_mz, sh_x, sh_y, pos_x1, pos_y1, range);
}

void SCANNER_draw_new_transparent(void)
{
    SCANNER_process_special_input();
    SCANNER_process_bbpoints();
    SCANNER_draw_new_transparent_map();
    SCANNER_draw_signals();
    SCANNER_draw_area_frame();
}

/******************************************************************************/
