/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bflib_vidraw.c
 *     Routines to be moved to bflibrary.
 * @par Purpose:
 *     Implement functions from bflibrary but not yet integrated there.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_vidraw.h"
#include "bftringl.h"

#include <stdlib.h>
#include <string.h>
#include "bfscreen.h"
#include "bfline.h"
#include "bfutility.h"

#include "engintrns.h"
/******************************************************************************/

extern ubyte byte_1DDC44[640];
extern ubyte byte_1DDEC4;
extern ubyte byte_1C4B7F;// = 0;
extern ubyte byte_1C6DED;// = 0;
extern ubyte byte_155189;// = 0xF7;

void draw_noise_box(short x, short y, ushort w, ushort h)
{
#if 0
    asm volatile (
      "call ASM_draw_noise_box\n"
        :  : "a" (X), "d" (Y), "b" (a3), "c" (a4));
    return;
#endif
    uint dx, dy;
    ushort rnd;

    if (!byte_1DDEC4 || byte_1C6DED)
    {
        for (dx = 0; dx < sizeof(byte_1DDC44); dx++)
        {
            byte_1DDC44[dx] = 0;
            rnd = LbRandomAnyShort();
            if ((rnd % 9) > 4) {
                byte_1DDC44[dx] = byte_155189;
            }
        }
        byte_1C6DED = 0;
        byte_1DDEC4 = 1;
    }

    if (!byte_1C4B7F)
    {
        ubyte *out;
        ubyte *inp;

        for (dy = 1; dy < (uint)h - 1; dy++)
        {
            rnd = LbRandomAnyShort();
            out = &lbDisplay.WScreen[x + 1 + (y + dy) * lbDisplay.GraphicsScreenWidth];
            inp = &byte_1DDC44[rnd % (sizeof(byte_1DDC44) - w)];
            memcpy(out, inp, w - 2);
        }
    }
}

//TODO add tests for this function
void poly_line(struct PolyPoint *point_a, struct PolyPoint *point_b)
{
#if 0
    asm volatile (
      "call ASM_poly_line\n"
        :  : "a" (point_a), "d" (point_b));
    return;
#endif
    TbPixel *obuf;
    ushort i;
    short x2, y2;
    short x1, y1;
    short dt_x;

    x1 = point_a->X;
    y1 = point_a->Y;
    x2 = point_b->X;
    y2 = point_b->Y;

    if (y1 < 0)
    {
        if (y2 < 0)
            return;
        x1 += -y1 * (x2 - x1) / (y2 - y1);
        y1 = 0;
    }
    else if (y2 < 0)
    {
        y2 = point_a->Y;
        y1 = point_b->Y;
        x1 = point_b->X;
        x2 = point_a->X;

        if (y2 < 0)
            return;
        x1 += -y1 * (x2 - x1) / (y2 - y1);
        y1 = 0;
    }
    else if (y1 == y2)
    {
        // Horizonal line optimization
        ushort w;
        short sx1, sx2;

        if (y1 >= vec_window_height)
            return;
        sx1 = x1;
        sx2 = x2;
        if (x2 < x1)
        {
          sx2 = x1;
          sx1 = x2;
        }
        if (sx1 >= 0)
        {
          if (sx1 > vec_window_width - 1)
              return;
        }
        else
        {
          if (sx2 <= 0)
              return;
          sx1 = 0;
        }
        if (sx2 > vec_window_width - 1)
            sx2 = vec_window_width - 1;
        w = sx2 - sx1;
        obuf = vec_screen + vec_screen_width * y1 + sx1;
        memset(obuf, vec_colour, w + 1);
        return;
    }
    else if (y1 - y2 < 0)
    {
        if (y1 >= vec_window_height)
            return;
    }
    else
    {
        if (y2 >= vec_window_height)
            return;
        y2 = point_a->Y;
        y1 = point_b->Y;
        x1 = point_b->X;
        x2 = point_a->X;
    }

    if (y1 >= vec_window_height) {
        x2 = x1 + (vec_window_height - y1) * (x2 - x1) / (y2 - y1);
        y2 = vec_window_height - 1;
    }

    if (x1 < 0)
    {
        if (x2 < 0)
            return;
        dt_x = 1;
        y1 += -x1 * (y2 - y1) / (x2 - x1);
        x1 = 0;
        if (x2 >= vec_window_width) {
            y2 = y1 + vec_window_width * (y2 - y1) / x2;
            x2 = vec_window_width - 1;
        }
    }
    else if (x1 >= vec_window_width)
    {
        if (x2 >= vec_window_width)
            return;
        dt_x = -1;
        y1 += (x1 - vec_window_width) * (y2 - y1) / (x1 - x2);
        x1 = vec_window_width - 1;
        if (x2 < 0)
        {
            y2 -= -x2 * (y2 - y1) / (x1 - x2);
            x2 = 0;
        }
    }
    else if (x2 < 0)
    {
        dt_x = -1;
        y2 = y1 + x1 * (y2 - y1) / (x1 - x2);
        x2 = 0;
    }
    else if (x2 >= vec_window_width)
    {
        dt_x = 1;
        y2 = y1 + (vec_window_width - x1) * (y2 - y1) / (x2 - x1);
        x2 = vec_window_width - 1;
    }
    else if (x1 == x2)
    {
        // Vertical line optimization
        ushort h;
        short sy1;

        if (y2 >= y1) {
            sy1 = y1;
            h = y2 - y1;
        } else {
            sy1 = y2;
            h = y1 - y2;
        }
        obuf = vec_screen + vec_screen_width * sy1 + x1;
        for (i = h + 1; i != 0; i--)
        {
            *obuf = vec_colour;
            obuf += vec_screen_width;
        }
        return;
    }
    else if (x1 - x2 < 0)
    {
        dt_x = 1;
    }
    else
    {
        dt_x = -1;
    }

    if (y2 == y1)
    {
        // Horizonal line optimization
        ushort w;
        short sx1;

        if (x2 >= x1) {
            sx1 = x1;
            w = x2 - x1;
        } else {
            sx1 = x2;
            w = x1 - x2;
        }
        obuf = vec_screen + vec_screen_width * y1 + sx1;
        memset(obuf, vec_colour, w + 1);
        return;
    }

    {
        ushort v6;
        ushort v7;
        short v9;
        short v10;
        short v11;
        int dt_buf;

        obuf = vec_screen + vec_screen_width * y1 + x1;
        v6 = dt_x * (x2 - x1);
        v7 = y2 - y1;
        if (v7 <= v6) {
            dt_buf = vec_screen_width;
        } else {
            v6 = y2 - y1;
            v7 = dt_x * (x2 - x1);
            dt_buf = dt_x;
            dt_x = vec_screen_width;
        }
        v9 = 2 * v7;
        v10 = 2 * v7 - v6;
        v11 = 2 * (v7 - v6);
        *obuf = vec_colour;
        for (i = v6; i != 0; i--)
        {
            obuf += dt_x;
            if (v10 >= 0) {
                obuf += dt_buf;
                v10 += v11;
            } else {
                v10 += v9;
            }
            *obuf = vec_colour;
        }
    }
}
/******************************************************************************/
