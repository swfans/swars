/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gpalcrss.c
 *     Palette changing functions.
 * @par Purpose:
 *     Allows to generate cross palette and apply it to an image.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2021 - 05 Jun 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfpalcrss.h"
#include "bfpalette.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfmath.h"

TbResult LbCrossPaletteLutGen(ubyte *cross_lut, const ubyte *src_palette,
  const ubyte *dst_palette)
{
    long i;
    ubyte *o;
    const ubyte *s;

    s = src_palette;
    o = cross_lut;
    for (i = 0; i < PALETTE_8b_COLORS; i++)
    {
        *o = LbPaletteFindColour(dst_palette, s[0], s[1], s[2]);
        o++;
        s += 3;
    }
    return Lb_SUCCESS;
}

TbResult LbBufferHistogram(ulong *hist, const TbPixel *buf, ulong buf_len)
{
    ulong i;
    const ubyte *b;

    LbMemorySet(hist, 0, sizeof(ulong) * 256);
    b = buf;
    for (i = 0; i < buf_len; i++)
    {
        hist[*b]++;
        b++;
    }
    return Lb_SUCCESS;
}

short LbImageBuffersMaxDifference(const TbPixel *buf1, const ubyte *pal1,
  const TbPixel *buf2, const ubyte *pal2, ulong buf_len, ulong *pos)
{
    ulong i;
    TbBool *already_map;
    long maxdiff = 0;
    ulong maxpos = 0;

    already_map = LbMemoryAlloc(256 * 256 * sizeof(TbBool));
    LbMemorySet(already_map, 0, 256 * 256 * sizeof(TbBool));

    for (i = 0; i < buf_len; i++)
    {
        ulong k;
        long curdiff;
        const ubyte *p1;
        const ubyte *p2;

        k = buf1[i] * 256 + buf2[i];
        if (already_map[k]) continue;
        already_map[k] = true;
        p1 = &pal1[3 * buf1[i]];
        p2 = &pal2[3 * buf2[i]];
        curdiff = LbColourDistanceLinearSq(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2]);
        if (curdiff > maxdiff) {
            maxdiff = curdiff;
            maxpos = i;
        }
    }

    LbMemoryFree(already_map);
    if (pos != NULL)
        *pos = maxpos;
    return LbSqrL(maxdiff);
}

/******************************************************************************/
