/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ggengh.c
 *     Functions for creating and handling colour tables.
 * @par Purpose:
 *     Allows high performance fading for indexed colour images.
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

static void ghost_table_generate(const ubyte *pal, short intens, ubyte *table)
{
    int i, k;
    const ubyte *colr_i;
    const ubyte *colr_k;
    ubyte *tbl_o;

    tbl_o = pixmap.ghost_table;
    colr_i = pal;
    for (i = 0; i < PALETTE_8b_COLORS; i++)
    {
        int ri, gi, bi;
        ri = colr_i[0];
        gi = colr_i[1];
        bi = colr_i[2];

        colr_k = pal;
        for (k = 0; k < PALETTE_8b_COLORS; k++)
        {
            int rk, gk, bk;
            rk = intens * (colr_k[0] - ri) / 100;
            gk = intens * (colr_k[1] - gi) / 100;
            bk = intens * (colr_k[2] - bi) / 100;
            *tbl_o = LbPaletteFindColour(pal, rk + ri, gk + gi, bk + bi);
            colr_k += 3;
            tbl_o++;
        }
        colr_i += 3;
    }
}

static void ghost_table_symmetrize(const ubyte *pal, short intens, ubyte *table)
{
    int i, k;

    for (i = 0; i < PALETTE_8b_COLORS; i++)
    {
      for (k = 0; k < i; k++)
      {
          ushort a, b;
          a = (i << 8) + k;
          b = (k << 8) + i;
          table[a] = table[b];
      }
    }
}

TbResult LbGhostTableGenerate(const ubyte *pal, short intens, const char *fname)
{
    TbBool generate = false;

    if (fname == NULL) {
        LOGSYNC("Generating colour ghosting requested");
        generate = true;
    }
    else if (LbFileLoadAt(fname, pixmap.ghost_table) !=
      PALETTE_8b_COLORS * PALETTE_8b_COLORS) {
        LOGSYNC("Generating colour ghosting, as saved file is invalid");
        generate = true;
    }

    if (generate) {
        ghost_table_generate(pal, intens, pixmap.ghost_table);
    }

    if (generate && (fname != NULL)) {
        if (LbFileSaveAt(fname, pixmap.ghost_table,
          PALETTE_8b_COLORS * PALETTE_8b_COLORS) == Lb_FAIL) {
            LOGERR("%s: Re-save colour ghosting file failed", fname);
            return Lb_FAIL;
        }
    }
    lbDisplay.GlassMap = pixmap.ghost_table;
    return Lb_SUCCESS;
}

TbResult LbGhostTableLoad(const ubyte *pal, short intens, const char *fname)
{
    long len;

    len = LbFileLoadAt(fname, pixmap.ghost_table);
    lbDisplay.GlassMap = pixmap.ghost_table;

    // At 50% intensity, the palette should be diagonally symmetrical.
    if (intens == 50) {
        ghost_table_symmetrize(pal, intens, pixmap.ghost_table);
    }
    if (len != PALETTE_8b_COLORS * PALETTE_8b_COLORS) {
        if (len == Lb_FAIL) {
            LOGERR("%s: Colour ghosting table loading failed", fname);
        } else {
            LOGWARN("%s: Loaded colour ghosting table has unexpected size (%ld)", fname, len);
        }
        return Lb_FAIL;
    }
    return Lb_SUCCESS;
}

/******************************************************************************/
