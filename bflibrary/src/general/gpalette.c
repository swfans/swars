/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gpalette.cpp
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bfpalette.h"

#include "bfscreen.h"
#include "bfbox.h"
#include "bftext.h"

TbResult LbPaletteFade_UNUSED(ubyte *from_pal, ubyte arg2, ubyte fade_steps)
{
// code at 0001:00098ac0
}

int LbPaletteStopOpenFade_UNUSED()
{
// code at 0001:00098cb8
}

TbPixel LbPaletteFindColour(const ubyte *pal, ubyte r, ubyte g, ubyte b)
{
    int min_delta;
    const unsigned char *c;
    int i;
    // Compute minimal square difference in color; return exact match if found
    min_delta = 999999;
    c = pal;
    for (i = 0; i < 256; i++)
    {
        int dr,dg,db;
        dr = (r - c[0]) * (r - c[0]);
        dg = (g - c[1]) * (g - c[1]);
        db = (b - c[2]) * (b - c[2]);
        if (min_delta > dr+dg+db)
        {
            min_delta = dr+dg+db;
            if (min_delta == 0) {
                return i;
            }
        }
        c += 3;
    }
    // Gather all the colors with minimal square difference
    unsigned char tmcol[256];
    unsigned char *o;
    int n;
    n = 0;
    o = tmcol;
    c = pal;
    for (i = 0; i < 256; i++)
    {
        int dr,dg,db;
        dr = (r - c[0]) * (r - c[0]);
        dg = (g - c[1]) * (g - c[1]);
        db = (b - c[2]) * (b - c[2]);
        if (min_delta == dr+dg+db)
        {
            n += 1;
            *o = i;
            o++;
        }
        c += 3;
    }
    // If there's only one left on list - return it
    if (n == 1) {
        return tmcol[0];
    }
    // Get minimal linear difference out of remaining colors
    min_delta = 999999;
    for (i = 0; i < n; i++)
    {
        int dr,dg,db;
        c = &pal[3 * tmcol[i]];
        dr = abs(r - c[0]);
        dg = abs(g - c[1]);
        db = abs(b - c[2]);
        if (min_delta > dr+dg+db) {
            min_delta = dr+dg+db;
        }
    }
    // Gather all the colors with minimal linear difference
    // Note that we may re-use tmcol array, because (i <= m)
    int m;
    m = 0;
    o = tmcol;
    for (i = 0; i < n; i++)
    {
        int dr,dg,db;
        c = &pal[3 * tmcol[i]];
        dr = abs(r - c[0]);
        dg = abs(g - c[1]);
        db = abs(b - c[2]);
        if (min_delta == dr+dg+db)
        {
            m += 1;
            *o = tmcol[i];
            o++;
        }
    }
    // If there's only one left on list - return it
    if (m == 1) {
        return tmcol[0];
    }
    // It's hard to select best color out of the left ones - use darker one with wages
    min_delta = 999999;
    o = &tmcol[0];
    for (i = 0; i < m; i++)
    {
        int dr,dg,db;
        c = &pal[3 * tmcol[i]];
        dr = (c[0] * c[0]);
        dg = (c[1] * c[1]);
        db = (c[2] * c[2]);
        if (min_delta > db+2*(dg+dr))
        {
          min_delta = db+2*(dg+dr);
          o = &tmcol[i];
        }
    }
    return *o;
}

TbResult LbPaletteDraw(long X, long Y,
  ulong Width, ulong Height, ushort Flags)
{
    char buf[16];
    long nrow, ncol;
    long curX, curY;
    ulong bkp_DrawFlags;
    TbPixel bkp_DrawColour;
    TbPixel colour;

    bkp_DrawFlags = lbDisplay.DrawFlags;
    bkp_DrawColour = lbDisplay.DrawColour;
    lbDisplay.DrawFlags = 0x0040;

    colour = 0;
    curY = Y;
    for (nrow = 0; nrow < 16; nrow++)
    {
        curX = X;
        for (ncol = 0; ncol < 16; ncol++)
        {
          LbDrawBox(curX, curY, Width, Height, colour);
          if (Flags & 0x01) {
              sprintf(buf, "%d", colour);
              lbDisplay.DrawColour = 0xFF - colour;
              LbTextDraw(curX, curY, buf);
          }
          ++colour;
          curX += Width;
        }
        curY += Height;
    }
    lbDisplay.DrawFlags = bkp_DrawFlags;
    lbDisplay.DrawColour = bkp_DrawColour;
    return Lb_SUCCESS;
}

TbResult LbPaletteDataFillBlack(ubyte *palette)
{
    memset(palette, 0, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

TbResult LbPaletteDataFillWhite(ubyte *palette)
{
    memset(palette, 0x3F, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

/******************************************************************************/
