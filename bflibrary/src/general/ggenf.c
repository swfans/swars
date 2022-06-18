/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ggenf.c
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
#include "privbflog.h"

/** @internal
 * Generate table for fading between given palette and given RGB colour.
 * The resulting array contains multiple palettes, which gradually
 * fade from given RGB values to colours within the palette.
 *
 * @param pal The palette used.
 * @param r Faded colour red component value.
 * @param g Faded colour green component value.
 * @param b Faded colour blue component value.
 * @param table The output table pointer. Needs to have size
 *        PALETTE_FADE_LEVELS * PALETTE_8b_COLORS.
 */
static void LbFadeTableToRGBGenerate(const ubyte *pal,
  ubyte r, ubyte g, ubyte b, TbPixel *table)
{
    TbPixel *t;
    long i, k;

    t = table;
    for (i = 0; i < PALETTE_FADE_LEVELS; i++)
    {
        const ubyte *p;
        ubyte rd, gd, bd;

        rd = ((64 - i) * r) >> 5;
        gd = ((64 - i) * g) >> 5;
        bd = ((64 - i) * b) >> 5;

        p = pal;
        for (k = PALETTE_8b_COLORS; k > 0; k--)
        {
            int rk, gk, bk;

            rk = (i * (int)p[0]) >> 5;
            gk = (i * (int)p[1]) >> 5;
            bk = (i * (int)p[2]) >> 5;
            *t = LbPaletteFindColour(pal, rk + rd, gk + gd, bk + bd);
            t++;
            p += 3;
        }
    }
}

TbResult LbFadeTableGenerate(const ubyte *palette, const char *fname)
{
    TbBool generate = false;

    if (fname == NULL) {
        LOGSYNC("Generating colour fading requested");
        generate = true;
    }
    else if (LbFileLoadAt(fname, pixmap.fade_table) !=
      PALETTE_FADE_LEVELS * PALETTE_8b_COLORS) {
        LOGSYNC("Generating colour fading, as saved file is invalid");
        generate = true;
    }

    if (generate) {
        LbFadeTableToRGBGenerate(palette, 0, 0, 0, pixmap.fade_table);
    }

    if (generate && (fname != NULL)) {
        if (LbFileSaveAt(fname, pixmap.fade_table,
          PALETTE_FADE_LEVELS*PALETTE_8b_COLORS) == Lb_FAIL) {
            LOGERR("%s: Re-save colour fading file failed", fname);
            return Lb_FAIL;
        }
    }
    lbDisplay.FadeTable = pixmap.fade_table;
    return Lb_SUCCESS;
}

TbResult LbFadeTableLoad(const ubyte *palette, const char *fname)
{
    long len;

    len = LbFileLoadAt(fname, pixmap.fade_table);
    lbDisplay.FadeTable = pixmap.fade_table;

    if (len != PALETTE_FADE_LEVELS * PALETTE_8b_COLORS) {
        if (len == Lb_FAIL) {
            LOGERR("%s: Fade table loading failed", fname);
        } else {
            LOGWARN("%s: Loaded fade table has unexpected size (%ld)", fname, len);
        }
        return Lb_FAIL;
    }
    return Lb_SUCCESS;
}

TbResult LbColourTablesGenerate(const ubyte *palette, const char *fname)
{
    TbResult retf, retg;

    retf = LbFadeTableGenerate(palette, NULL);
    retg = LbGhostTableGenerate(palette, 50, NULL);

    if ((retf == Lb_SUCCESS) && (retg == Lb_SUCCESS)) {
        if (LbFileSaveAt(fname, &pixmap, sizeof(pixmap)) == Lb_FAIL) {
            LOGERR("%s: Re-save colour fading file failed", fname);
            return Lb_FAIL;
        }
    }
    return Lb_SUCCESS;
}

TbResult LbColourTablesLoad(const ubyte *palette, const char *fname)
{
    long len;

    len = LbFileLoadAt(fname, &pixmap);
    lbDisplay.FadeTable = pixmap.fade_table;
    lbDisplay.GlassMap = pixmap.ghost_table;

    if (len != sizeof(pixmap)) {
        if (len == Lb_FAIL) {
            LOGERR("%s: Colour tables loading failed", fname);
        } else {
            LOGWARN("%s: Loaded colour tables have unexpected size (%ld)", fname, len);
        }
        return Lb_FAIL;
    }
    return Lb_SUCCESS;
}

/******************************************************************************/
