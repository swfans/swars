/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ggenf.cpp
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
#include "bfgentab.h"

#include "bffile.h"
#include "bfpalette.h"
#include "bfscreen.h"

TbPixel fade_table_UNUSED[256*64];

static void fade_table_generate(const ubyte *pal, ubyte *table)
{
    const ubyte *p;
    ubyte *t;
    long i, k;

    t = table;
    for (i = 0; i < 64; i++)
    {
        p = pal;
        for (k = 256; k > 0; k--)
        {
            int rk, gk, bk;

            rk = (i * p[0]) >> 5;
            gk = (i * p[1]) >> 5;
            bk = (i * p[2]) >> 5;
            *t = LbPaletteFindColour(pal, rk, gk, bk);
            t++;
            p += 3;
        }
    }
}

TbResult LbFadeTableGenerate(const ubyte *palette, const char *table_fname)
{
    if (LbFileLoadAt(table_fname, fade_table) == Lb_FAIL)
    {
        fade_table_generate(palette, fade_table);

        if (LbFileSaveAt(table_fname, fade_table, 64*256) == Lb_FAIL) {
            return Lb_FAIL;
        }
    }
    lbDisplay.FadeTable = fade_table;
    return Lb_SUCCESS;
}

TbResult LbFadeTableLoad(const ubyte *palette, const char *table_fname)
{
    TbResult ret;

    ret = LbFileLoadAt(table_fname, fade_table);
    lbDisplay.FadeTable = fade_table;
    return ret;
}

/******************************************************************************/
