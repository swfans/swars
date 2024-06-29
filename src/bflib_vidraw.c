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

#include <stdlib.h>
#include <string.h>
#include "bfscreen.h"
#include "bfline.h"
/******************************************************************************/

struct TbSPoint {
  short x;
  short y;
};

extern uchar byte_1E957C[0xf00];

void LbDrawTriangleOutline(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    LbDrawLine(x1, y1, x2, y2, colour);
    LbDrawLine(x2, y2, x3, y3, colour);
    LbDrawLine(x3, y3, x1, y1, colour);
}

// overflow flag of subtraction (x-y)
ubyte __OFSUBW__(short x, short y)
{
    int y2 = y;
    ubyte sx = (x < 0);
    return (sx ^ (y2 < 0)) & (sx ^ (x-y2 < 0));
}

void LbDrawTriangleFilled(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    //TODO make sure to test whether this works exactly like original, especially for large triangles
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_LbDrawTriangleFilled\n"
        :  : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (x3), "g" (y3), "g" (colour));
#endif
  struct TbSPoint *p_pt1;
  struct TbSPoint *p_pt2;
  struct TbSPoint *p_pt3;
  int v12;
  int oval0a, oval0b, oval1a, oval1b;
  int v20;
  ubyte *o;
  int v24;
  ubyte v25;
  short v26;
  int v35;
  int v38;
  short v39;
  int v55;
  uchar *v58;
  ubyte v65;
  TbBool pt2y_overflow;
  char blt2skip;
  TbBool pt3y_overflow;
  ubyte v70;
  TbBool pt1y_underflow;
  short v75;
  short v76;
  short v77;
  short v78;
  int v79;
  int v80;
  int v81;
  int wnd_h;
  int v84;
  int v85, v89;
  short blt1width, blt2width;
  int oinc1a, oinc1b;
  int oinc0a, oinc0b;
  TbPixel *p_screen;
  struct TbSPoint pt3;
  struct TbSPoint pt2;
  struct TbSPoint pt1;

  p_pt1 = &pt1;
  p_pt2 = &pt2;
  p_pt3 = &pt3;
  pt1.x = x1;
  pt2.x = x2;
  pt3.x = x3;
  pt1.y = y1;
  pt2.y = y2;
  pt3.y = y3;

  if (pt1.y == pt2.y)
  {
    if (pt1.y == pt3.y)
      return;
    if (pt1.y - pt3.y < 0)
    {
      if (pt2.x <= pt1.x)
        return;
      goto LABEL_92;
    }
    if (pt1.x <= pt2.x)
      return;
    p_pt1 = &pt3;
    p_pt2 = &pt1;
    p_pt3 = &pt2;
    goto LABEL_77;
  }
  if (pt1.y <= pt2.y)
  {
    if (pt1.y != y3 )
    {
      if (pt1.y - y3 >= 0)
      {
        p_pt1 = &pt3;
        p_pt2 = &pt1;
        p_pt3 = &pt2;
        goto LABEL_24;
      }
      if (pt2.y != pt3.y)
      {
        if (pt2.y <= pt3.y)
          goto LABEL_24;
        goto LABEL_50;
      }
      if (pt2.x <= pt3.x)
        return;
      goto LABEL_77;
    }
    if (pt1.x <= pt3.x)
      return;
    p_pt1 = &pt3;
    p_pt2 = &pt1;
    p_pt3 = &pt2;
    goto LABEL_92;
  }
  if (pt1.y == pt3.y)
  {
    if (pt3.x <= pt1.x)
      return;
    p_pt1 = &pt2;
    p_pt2 = &pt3;
    p_pt3 = &pt1;
    goto LABEL_77;
  }
  if (pt1.y - pt3.y < 0)
  {
    p_pt1 = &pt2;
    p_pt2 = &pt3;
    p_pt3 = &pt1;
    goto LABEL_50;
  }
  if (pt2.y == pt3.y)
  {
    if (pt3.x <= pt2.x)
      return;
    p_pt1 = &pt2;
    p_pt2 = &pt3;
    p_pt3 = &pt1;
    goto LABEL_92;
  }
  if (pt2.y - pt3.y < 0)
  {
    p_pt1 = &pt2;
    p_pt2 = &pt3;
    p_pt3 = &pt1;
    goto LABEL_24;
  }
  p_pt1 = &pt3;
  p_pt2 = &pt1;
  p_pt3 = &pt2;
  goto LABEL_50;


LABEL_50:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v76 = p_pt1->y;
        blt2skip = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt3->y - p_pt1->y;
        pt2y_overflow = p_pt2->y > lbDisplay.GraphicsWindowHeight;
        wnd_h = p_pt2->y - p_pt1->y;
        oinc0a = ((p_pt3->x - p_pt1->x) << 16) / blt1width;
        oinc1a = ((p_pt2->x - p_pt1->x) << 16) / wnd_h;
        if (oinc1a <= oinc0a)
            return;
        blt2width = p_pt2->y - p_pt3->y;
        oinc0b = ((p_pt2->x - p_pt3->x) << 16) / blt2width;
        v85 = p_pt3->x << 16;
        oval0a = p_pt1->x << 16;
        oval1a = oval0a;
        if (!pt1y_underflow)
        {
            if (pt2y_overflow)
            {
                v38 = lbDisplay.GraphicsWindowHeight - v76;
                wnd_h = v38;
                if (blt2skip) {
                    blt1width = lbDisplay.GraphicsWindowHeight - v76;
                } else {
                    v25 = __OFSUBW__(v38, blt1width);
                    v39 = v38 - blt1width;
                    blt2skip = ((v39 < 0) ^ v25) | (v39 == 0);
                    blt2width = v39;
                }
            }
            goto LABEL_71;
        }
        else
        {
            int v82;

            v35 = -v76;
            wnd_h += v76;
            if (wnd_h <= 0)
                return;
            v81 = -v76;
            if (v35 - blt1width >= 0)
            {
                v82 = v81 - blt1width;
                blt2width -= v82;
                oval0b = oinc0b * v82 + v85;
                oval1a += v82 * oinc1a + blt1width * oinc1a;
                if (pt2y_overflow)
                {
                    blt2width = lbDisplay.GraphicsWindowHeight;
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                }
                o = byte_1E957C;
                goto LABEL_72;
            }
            else
            {
                blt1width += v76;
                oval0a += oinc0a * v81;
                oval1a += v81 * oinc1a;
                if (pt2y_overflow)
                {
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                    if (blt2skip) {
                        blt1width = lbDisplay.GraphicsWindowHeight;
                    } else {
                        blt2skip = lbDisplay.GraphicsWindowHeight <= blt1width;
                        blt2width = lbDisplay.GraphicsWindowHeight - blt1width;
                    }
                }
                goto LABEL_71;
            }
        }


LABEL_77:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v77 = p_pt1->y;
        pt3y_overflow = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt3->y - p_pt1->y;
        wnd_h = blt1width;
        oinc0a = ((p_pt3->x - p_pt1->x) << 16) / blt1width;
        oinc1a = ((p_pt2->x - p_pt1->x) << 16) / blt1width;
        oval0a = p_pt1->x << 16;
        oval1a = oval0a;
        if (!pt1y_underflow)
        {
            if (pt3y_overflow)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight - v77;
                blt1width = lbDisplay.GraphicsWindowHeight - v77;
            }
        }
        else
        {
            int v46;

            v46 = -v77;
            blt1width += v77;
            wnd_h += v77;
            if (wnd_h <= 0)
                return;
            oval0a += oinc0a * v46;
            oval1a += v46 * oinc1a;
            if (pt3y_overflow)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight;
                blt1width = lbDisplay.GraphicsWindowHeight;
            }
        }
        o = byte_1E957C;
        for (; blt1width > 0; blt1width--)
        {
            *((ulong *)o + 0) = oval0a;
            oval0a += oinc0a;
            *((ulong *)o + 1) = oval1a;
            oval1a += oinc1a;
            o += 8;
        }
        goto LABEL_104;


LABEL_24:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v75 = p_pt1->y;
        v12 = p_pt3->y;
        v65 = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        v89 = p_pt3->y - p_pt1->y;
        wnd_h = v89;
        blt2skip = p_pt2->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt2->y - p_pt1->y;
        oinc0a = ((p_pt3->x - p_pt1->x) << 16) / v89;
        oinc1a = ((p_pt2->x - p_pt1->x) << 16) / v12;
        if (oinc1a <= oinc0a)
            return;
        oinc1b = ((p_pt3->x - p_pt2->x) << 16) / v12;
        blt2width = p_pt3->y - p_pt2->y;
        v84 = p_pt2->x << 16;
        oval0a = p_pt1->x << 16;
        oval1a = oval0a;
        if (!pt1y_underflow)
        {
            if (v65)
            {
                v24 = lbDisplay.GraphicsWindowHeight - v75;
                wnd_h = v24;
                if (blt2skip) {
                    blt1width = v24;
                } else {
                    v25 = __OFSUBW__(v24, blt1width);
                    v26 = v24 - blt1width;
                    blt2skip = ((v26 < 0) ^ v25) | (v26 == 0);
                    blt2width = v26;
                }
            }
            goto LABEL_43;
        }
        else
        {
            v20 = -v75;
            wnd_h += v75;
            if (wnd_h <= 0)
                return;
            v79 = -v75;
            if (v20 - blt1width >= 0)
            {
                blt2width -= v79 - blt1width;
                v80 = v79 - blt1width;
                oval0a += oinc0a * v80 + blt1width * oinc0a;
                oval1b = oinc1b * v80 + v84;
                if (v65)
                {
                    blt2width = lbDisplay.GraphicsWindowHeight;
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                }
                o = byte_1E957C;
                goto LABEL_46;
            }
            else
            {
                blt1width += v75;
                oval0a += oinc0a * v79;
                oval1a += v79 * oinc1a;
                if (v65)
                {
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                    if (blt2skip) {
                        blt1width = lbDisplay.GraphicsWindowHeight;
                    } else {
                        blt2skip = lbDisplay.GraphicsWindowHeight <= blt1width;
                        blt2width = lbDisplay.GraphicsWindowHeight - blt1width;
                    }
                }
                goto LABEL_43;
            }
        }


LABEL_92:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v78 = p_pt1->y;
        v70 = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt3->y - p_pt1->y;
        wnd_h = blt1width;
        oinc0a = ((p_pt3->x - p_pt1->x) << 16) / blt1width;
        oinc1a = ((p_pt3->x - p_pt2->x) << 16) / blt1width;
        oval0a = p_pt1->x << 16;
        oval1a = p_pt2->x << 16;
        if (!pt1y_underflow)
        {
            if (v70)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight - v78;
                blt1width = lbDisplay.GraphicsWindowHeight - v78;
            }
        }
        else
        {
            v55 = -v78;
            blt1width += v78;
            wnd_h += v78;
            if (wnd_h <= 0)
                return;
            oval0a += oinc0a * v55;
            oval1a += v55 * oinc1a;
            if (v70)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight;
                blt1width = lbDisplay.GraphicsWindowHeight;
            }
        }
        o = byte_1E957C;
        for (; blt1width > 0; blt1width--)
        {
            *((ulong *)o + 0) = oval0a;
            oval0a += oinc0a;
            *((ulong *)o + 1) = oval1a;
            oval1a += oinc1a;
            o += 8;
        }
        goto LABEL_104;



LABEL_71:
        o = byte_1E957C;
        for (; blt1width > 0; blt1width--)
        {
            *((ulong *)o + 0) = oval0a;
            oval0a += oinc0a;
            *((ulong *)o + 1) = oval1a;
            oval1a += oinc1a;
            o += 8;
        }
        oval0b = v85;
LABEL_72:
        if (!blt2skip)
        {
            for (; blt2width > 0; blt2width--)
            {
                *((ulong *)o + 0) = oval0b;
                oval0b += oinc0b;
                *((ulong *)o + 1) = oval1a;
                oval1a += oinc1a;
                o += 8;
            }
        }
        goto LABEL_104;


LABEL_43:
        o = byte_1E957C;
        for (; blt1width > 0; blt1width--)
        {
            *((ulong *)o + 0) = oval0a;
            oval0a += oinc0a;
            *((ulong *)o + 1) = oval1a;
            oval1a += oinc1a;
            o += 8;
        }
        oval1b = v84;
LABEL_46:
        if (!blt2skip)
        {
            for (; blt2width > 0; blt2width--)
            {
                *((ulong *)o + 0) = oval0a;
                oval0a += oinc0a;
                *((ulong *)o + 1) = oval1b;
                oval1b += oinc1b;
                o += 8;
            }
        }
        goto LABEL_104;


LABEL_104:
    v58 = byte_1E957C;
    p_screen += lbDisplay.GraphicsWindowX + lbDisplay.GraphicsScreenWidth * (lbDisplay.GraphicsWindowY - 1);
    for (; wnd_h > 0; wnd_h--, v58 += 8)
    {
        ushort m;
        int w, shift;
        ubyte *p_map;

        shift = *((ushort *)v58 + 1);
        w = *((ushort *)v58 + 3);
        p_screen += lbDisplay.GraphicsScreenWidth;
        if ((shift & 0x8000) == 0)
        {
            if (w > lbDisplay.GraphicsWindowWidth)
                w = lbDisplay.GraphicsWindowWidth;
            w -= shift;
            if (w <= 0)
                continue;
            o = &p_screen[shift];
        }
        else
        {
            if (w <= 0)
                continue;
            if (w > lbDisplay.GraphicsWindowWidth)
                w = lbDisplay.GraphicsWindowWidth;
            o = p_screen;
        }
        p_map = lbDisplay.GlassMap;
        if ((lbDisplay.DrawFlags & 0x0004) != 0)
        {
            m = (colour << 8);
            for (; w > 0; w--)
            {
                m = (m & 0xFF00) | (*o & 0xFF);
                *o++ = p_map[m];
            }
        }
        else if ((lbDisplay.DrawFlags & 0x0008) != 0)
        {
            m = colour;
            for (; w > 0; w--)
            {
                m = (m & 0x00FF) | ((*o << 8) & 0xFF00);
                *o++ = p_map[m];
            }
        }
        else
        {
            m = colour;
            memset(o, m, w);
        }
    }
}

void LbDrawTriangle(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    if (lbDisplay.DrawFlags & Lb_SPRITE_OUTLINE)
    {
        LbDrawTriangleOutline(x1, y1, x2, y2, x3, y3, colour);
    }
    else
    {
        LbDrawTriangleFilled(x1, y1, x2, y2, x3, y3, colour);
    }
}

void LbDrawSlantBox(short X, short Y, ushort a3, ushort a4)
{
    asm volatile (
      "call ASM_LbDrawSlantBox\n"
        :  : "a" (X), "d" (Y), "b" (a3), "c" (a4));
}

/******************************************************************************/
