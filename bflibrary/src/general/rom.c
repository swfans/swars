/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file rom.asm
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
#include "rom.h"
#include "bftypes.h"
#include "bfpalette.h"

int prop_text_UNUSED()
{
// code at 0001:000e5efc
}

void make_fade_table(const ubyte *pal, ubyte *out, ubyte cr, ubyte cg, ubyte cb, ubyte ir, ubyte ig, ubyte ib)
{
    const ubyte *p;
    ubyte *t;
    short k;

    p = pal;
    t = out;
    for (k = 256; k > 0; k--)
    {
        ubyte r, g, b;

        r = ((ushort)(ir * (short)(cr - p[0])) >> 8) + p[0];
        g = ((ushort)(ig * (short)(cg - p[1])) >> 8) + p[1];
        b = ((ushort)(ib * (short)(cb - p[2])) >> 8) + p[2];
        *t = LbPaletteFindColourHalfWaged(pal, r, g, b);
        t++;
        p += 3;
    }
}

int tabwidth_UNUSED;

int font_UNUSED;

int qaz_UNUSED()
{
// code at 0001:000e5f73
}


/******************************************************************************/
