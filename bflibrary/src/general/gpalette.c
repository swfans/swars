/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gpalette.c
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
#include "bfwindows.h"
#include "bfbox.h"
#include "bftext.h"

typedef i32 (*ColourDistanceFunc)(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2);

i32 fade_count;
u8 fade_from_pal[PALETTE_8b_SIZE];
u8 fade_to_pal[PALETTE_8b_SIZE];
TbBool fade_started = false;

TbResult LbPaletteFade(u8 *from_pal, u8 fade_to, u8 fade_steps)
{
    u8 palette[PALETTE_8b_SIZE];
    u8 *ptr;
    short i;

    ptr = from_pal;

    if (fade_steps == 1)
    {
        LbPaletteGet(fade_from_pal);
        if (ptr == NULL)
        {
            ptr = fade_to_pal;
            memset(fade_to_pal, 0, PALETTE_8b_SIZE);
        }

        for (fade_count = 0; fade_count <= fade_to; fade_count++)
        {
            for (i = 0; i < PALETTE_8b_SIZE; i++)
            {
                short diff;
                diff = ptr[i] - (short)fade_from_pal[i];
                palette[i] = fade_from_pal[i] + (fade_count * diff / fade_to);
            }
            LbScreenWaitVbi();
            LbPaletteSet(palette);
            if (fade_count < fade_to)
                LbDoMultitasking();
        }
        fade_started = false;
    }
    else
    {
        if (fade_started)
        {
            if (fade_to == ++fade_count)
                fade_started = 0;
        }
        else
        {
            fade_count = 0;
            fade_started = true;
            LbPaletteGet(fade_from_pal);
            if (ptr == NULL)
                memset(fade_to_pal, 0, PALETTE_8b_SIZE);
        }
        if (ptr == NULL)
            ptr = fade_to_pal;

        for (i = 0; i < PALETTE_8b_SIZE; i++)
        {
            short diff;
            diff = ptr[i] - (short)fade_from_pal[i];
            palette[i] = fade_from_pal[i] + (fade_count * diff / fade_to);
        }
        LbScreenWaitVbi();
        LbPaletteSet(palette);
    }
    return fade_count;
}

TbResult LbPaletteStopOpenFade(void)
{
    fade_started = false;
    return Lb_SUCCESS;
}

/** @internal
 *  Get colour intensity by adding the factors with wages.
 * This is often used to convert RGB colour to perceptually similar greyscale value.
 * The RGB wages are 0.30 0.59 0.11.
 */
static u8 LbColourIntensity(u8 r, u8 g, u8 b)
{
    return (r*77 + g*151 + b*28) >> 8;
}

/** @internal
 *  Get colour distance using the simplest box method.
 */
i32 LbColourDistanceBox(u8 r1, u8 g1, u8 b1,
  u8 r2, u8 g2, u8 b2)
{
    i32 dr, dg, db;
    dr = abs(r2 - (i32)r1);
    dg = abs(g2 - (i32)g1);
    db = abs(b2 - (i32)b1);
    return dr + dg + db;
}

i32 LbColourDistanceLinearSq(u8 r1, u8 g1, u8 b1,
  u8 r2, u8 g2, u8 b2)
{
    i32 dr, dg, db;
    dr = (r2 - (i32)r1) * (r2 - (i32)r1);
    dg = (g2 - (i32)g1) * (g2 - (i32)g1);
    db = (b2 - (i32)b1) * (b2 - (i32)b1);
    return dr + dg + db;
}

/** @internal
 * Get colour distance by computing waged squares of coordinates.
 * While way less accurate than CIELAB deltaE, this provides more of
 * perception-based difference.
 */
i32 LbColourDistanceWagedSq(u8 r1, u8 g1, u8 b1,
  u8 r2, u8 g2, u8 b2)
{
    i32 dr, dg, db;
    // RGB factors are 0.30 0.59 0.11;
    // here we convert them to squared 8-bit multiplier
    dr = (r2 - (i32)r1) * (r2 - (i32)r1) * 23;
    dg = (g2 - (i32)g1) * (g2 - (i32)g1) * 89;
    db = (b2 - (i32)b1) * (b2 - (i32)b1) * 3;
    return (dr + dg + db) >> 2;
}

/** @internal
 * Get colour distance by computing waged squares of coordinates,
 * with non-squared wages.
 */
i32 LbColourDistanceHalfWagedSq(u8 r1, u8 g1, u8 b1,
  u8 r2, u8 g2, u8 b2)
{
    i32 dr, dg, db;
    // RGB factors are 0.30 0.59 0.11;
    // here we convert them to squared 8-bit multiplier
    dr = (r2 - (i32)r1) * (r2 - (i32)r1) * 77;
    dg = (g2 - (i32)g1) * (g2 - (i32)g1) * 151;
    db = (b2 - (i32)b1) * (b2 - (i32)b1) * 28;
    return (dr + dg + db) >> 5;
}

static inline TbPixel LbPaletteFindColourUsingDistanceFunc(const u8 *pal,
  u8 r, u8 g, u8 b, ColourDistanceFunc primaryDistance,
  ColourDistanceFunc secondaryDistance)
{
    int min_delta;
    const unsigned char *c;
    int i;
    // Compute minimal square difference in color;
    // return exact match if found
    min_delta = 999999;
    c = pal;
    for (i = 0; i < PALETTE_8b_COLORS; i++)
    {
        i32 curr_delta;
        curr_delta = primaryDistance(c[0], c[1], c[2], r, g, b);
        if (min_delta > curr_delta)
        {
            min_delta = curr_delta;
            if (min_delta == 0) {
                return i;
            }
        }
        c += 3;
    }
    // Gather all the colors with minimal square difference
    unsigned char tmcol[PALETTE_8b_COLORS];
    unsigned char *o;
    int n;
    n = 0;
    o = tmcol;
    c = pal;
    for (i = 0; i < PALETTE_8b_COLORS; i++)
    {
        i32 curr_delta;
        curr_delta = primaryDistance(c[0], c[1], c[2], r, g, b);
        if (min_delta == curr_delta)
        {
            n += 1;
            *o = i;
            o++;
        }
        c += 3;
    }
    // If there is only one left on list - return it
    if (n == 1) {
        return tmcol[0];
    }
    // Get minimal linear difference out of remaining colors
    min_delta = 999999;
    for (i = 0; i < n; i++)
    {
        i32 curr_delta;
        c = &pal[3 * tmcol[i]];
        curr_delta = secondaryDistance(c[0], c[1], c[2], r, g, b);
        if (min_delta > curr_delta) {
            min_delta = curr_delta;
        }
    }
    // Gather all the colors with minimal linear difference
    // Note that we may re-use tmcol array, because (i <= m)
    int m;
    m = 0;
    o = tmcol;
    for (i = 0; i < n; i++)
    {
        i32 curr_delta;
        c = &pal[3 * tmcol[i]];
        curr_delta = secondaryDistance(c[0], c[1], c[2], r, g, b);
        if (min_delta == curr_delta)
        {
            m += 1;
            *o = tmcol[i];
            o++;
        }
    }
    // If there is only one left on list - return it
    if (m == 1) {
        return tmcol[0];
    }
    // It is hard to select best color out of the ones left - use darker one
    min_delta = 999999;
    o = &tmcol[0];
    for (i = 0; i < m; i++)
    {
        int intensity;
        c = &pal[3 * tmcol[i]];
        intensity = LbColourIntensity(c[0], c[1], c[2]);
        if (min_delta > intensity)
        {
          min_delta = intensity;
          o = &tmcol[i];
        }
    }
    return *o;
}

TbPixel LbPaletteFindColourLinear(const u8 *pal, u8 r, u8 g, u8 b)
{
    return LbPaletteFindColourUsingDistanceFunc(pal, r, g, b,
      LbColourDistanceLinearSq, LbColourDistanceBox);
}

TbPixel LbPaletteFindColourHalfWaged(const u8 *pal, u8 r, u8 g, u8 b)
{
    return LbPaletteFindColourUsingDistanceFunc(pal, r, g, b,
      LbColourDistanceHalfWagedSq, LbColourDistanceBox);
}

TbPixel LbPaletteFindColour(const u8 *pal, u8 r, u8 g, u8 b)
{
    return LbPaletteFindColourUsingDistanceFunc(pal, r, g, b,
      LbColourDistanceLinearSq, LbColourDistanceBox);
}

TbResult LbPaletteDraw(i32 X, i32 Y,
  u32 Width, u32 Height, ushort Flags)
{
    char buf[16];
    i32 nrow, ncol;
    i32 curX, curY;
    u32 bkp_DrawFlags;
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

TbResult LbPaletteDataFillBlack(u8 *palette)
{
    memset(palette, 0, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

TbResult LbPaletteDataFillWhite(u8 *palette)
{
    memset(palette, 0x3F, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

/******************************************************************************/
