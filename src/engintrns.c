/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engintrns.c
 *     Math transformations required by the 3D engine.
 * @par Purpose:
 *     Implement functions for transforming positions in 3D world.
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
#include "engintrns.h"

#include "display.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

void transform_point(struct EnginePoint *p_ep)
{
#if 0
    asm volatile ("call ASM_transform_point\n"
        :  : "a" (p_ep));
    return;
#endif
    int fctr_a, fctr_b, fctr_c;
    int scr_shx, scr_shy;

    p_ep->Y3d -= 8 * engn_yc;
    fctr_a = (dword_176D14 * p_ep->X3d - dword_176D10 * p_ep->Z3d) >> 16;
    fctr_b = (dword_176D14 * p_ep->Z3d + dword_176D10 * p_ep->X3d) >> 16;
    fctr_c = (dword_176D1C * p_ep->Y3d - dword_176D18 * fctr_b) >> 16;

    p_ep->Z3d = (dword_176D1C * fctr_b + dword_176D18 * p_ep->Y3d) >> 16;
    p_ep->X3d = overall_scale * fctr_a;
    p_ep->Y3d = overall_scale * fctr_c;

    if (game_perspective == 5)
        scr_shx = (0x4000 - p_ep->Z3d) * (p_ep->X3d >> 11) >> 14;
    else
        scr_shx = p_ep->X3d >> 11;

    p_ep->pp.X = dword_176D3C + scr_shx;
    if (p_ep->pp.X < 0)
    {
        p_ep->Flags |= 0x01;
        if (p_ep->pp.X < -2000)
            p_ep->pp.X = -2000;
    }
    else if (p_ep->pp.X >= vec_window_width)
    {
        p_ep->Flags |= 0x02;
        if (p_ep->pp.X > 2000)
            p_ep->pp.X = 2000;
    }

    if (game_perspective == 5)
        scr_shy = (0x4000 - p_ep->Z3d) * (p_ep->Y3d >> 11) >> 14;
    else
        scr_shy = p_ep->Y3d >> 11;

    p_ep->pp.Y = dword_176D40 - scr_shy;
    if (p_ep->pp.Y < 0)
    {
        p_ep->Flags |= 0x04;
        if (p_ep->pp.Y < -2000)
            p_ep->pp.Y = -2000;
    }
    else if (p_ep->pp.Y >= vec_window_height)
    {
        p_ep->Flags |= 0x08;
        if (p_ep->pp.Y > 2000)
            p_ep->pp.Y = 2000;
    }
    p_ep->Flags |= 0x40;
}

void transform_shpoint(struct ShEnginePoint *p_sp, int dxc, int dyc, int dzc)
{
    int fctr_a, fctr_b, fctr_c, fctr_d;
    int scr_shx, scr_shy, sca_x, sca_y;
    int scr_x, scr_y;
    ubyte flg;

    fctr_a = (dword_176D14 * dxc - dword_176D10 * dzc) >> 16;
    fctr_b = (dword_176D10 * dxc + dword_176D14 * dzc) >> 16;
    fctr_c = (dword_176D1C * dyc - dword_176D18 * fctr_b) >> 16;
    fctr_d = (dword_176D18 * dyc + dword_176D1C * fctr_b) >> 16;
    sca_x = overall_scale * fctr_a;
    sca_y = overall_scale * fctr_c;
    flg = 0;

    if (game_perspective == 5)
        scr_shx = (sca_x >> 11) * (0x4000 - fctr_d) >> 14;
    else
        scr_shx = sca_x >> 11;

    scr_x = dword_176D3C + scr_shx;
    if (scr_x < 0)
    {
        flg |= 0x01;
        if (scr_x < -2000)
            scr_x = -2000;
    }
    else if (scr_x >= vec_window_width)
    {
        flg |= 0x02;
        if (scr_x > 2000)
            scr_x = 2000;
    }

    if (game_perspective == 5)
        scr_shy = (sca_y >> 11) * (0x4000 - fctr_d) >> 14;
    else
        scr_shy = sca_y >> 11;

    scr_y = dword_176D40 - scr_shy;
    if (scr_y < 0)
    {
        flg |= 0x04;
        if (scr_y < -2000)
            scr_y = -2000;
    }
    else if (scr_y >= vec_window_height)
    {
        flg |= 0x08;
        if (scr_y > 2000)
            scr_y = 2000;
    }

    flg |= 0x40;
    p_sp->Flags = flg;
    p_sp->X = scr_x;
    p_sp->Y = scr_y;
    p_sp->field_4 = fctr_d;
}

void process_engine_unk1(void)
{
#if 0
    asm volatile ("call ASM_process_engine_unk1\n"
        :  :  : "eax" );
    return;
#endif
    int angle;

    dword_176D4C = 0;
    dword_176D64 = -70;
    dword_176D3C = vec_window_width / 2;
    dword_176D40 = vec_window_height / 2;
    engn_anglexz += dword_176D54;
    dword_176D44 = 4 * (vec_window_width / 2) / 3;
    angle = (engn_anglexz >> 5) & LbFPMath_AngleMask;
    dword_176D0C = angle;
    dword_176D14 = lbSinTable[angle + LbFPMath_PI/2];
    dword_176D10 = lbSinTable[angle];
    angle = dword_152EEC & LbFPMath_AngleMask;
    dword_176D18 = lbSinTable[angle];
    dword_176D1C = lbSinTable[angle + LbFPMath_PI/2];
}

void setup_engine_nullsub4(void)
{
}

void calc_mouse_pos(void)
{
    asm volatile ("call ASM_calc_mouse_pos\n"
        :  :  : "eax" );
}

void process_engine_unk2(void)
{
#if 0
    asm volatile ("call ASM_process_engine_unk2\n"
        :  :  : "eax" );
    return;
#endif
    short msx, msy;
    int offs_y;
    int point_x, point_y;
    int shift_x, shift_y;
    int map_xc, map_yc;

    if (ingame.DisplayMode == DpM_UNKN_32)
      offs_y = overall_scale * engn_yc >> 8;
    else
      offs_y = 0;
    msx = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
    msy = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;

    if (lbDisplay.GraphicsScreenHeight < 400)
    {
        point_y = (msy >> 1) - offs_y;
        point_x = msx >> 1;
    }
    else
    {
        point_y = msy - offs_y;
        point_x = msx;
    }
    if (dword_176D18 != 0)
    {
        int shift_a;

        shift_x = ((point_x - dword_176D3C) << 11) / overall_scale;
        shift_a = ((point_y - dword_176D40) << 11) / overall_scale;
        shift_y = -((shift_a << 16) / dword_176D18);
    }
    else
    {
        shift_x = 0;
        shift_y = 0;
    }
    map_xc =  ((dword_176D14 * shift_x - dword_176D10 * shift_y) >> 16);
    map_yc = -((dword_176D10 * shift_x + dword_176D14 * shift_y) >> 16);
    mouse_map_x = engn_xc + map_xc;
    mouse_map_z = engn_zc + map_yc;
    if (ingame.DisplayMode == DpM_UNKN_32)
        calc_mouse_pos();
    setup_engine_nullsub4();
}

/******************************************************************************/
