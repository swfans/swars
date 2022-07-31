/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mock_palette.c
 *     Implementation which only pretends to do stuff, for test purposes.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     05 Nov 2021 - 29 Jul 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <string.h>
#include "mock_bfpalette.h"
#include "bfpalette.h"
#include "bfscreen.h"
#include "bftstlog.h"

extern ubyte lbPalette[PALETTE_8b_SIZE];

TbResult MockPaletteSet(const ubyte *palette)
{
    if (!lbScreenInitialised)
      return Lb_FAIL;

    memcpy(lbPalette, palette, PALETTE_8b_SIZE);
    lbDisplay.Palette = lbPalette;
    return Lb_SUCCESS;
}

void make_general_palette(ubyte *pal)
{
    short i, k;
    ubyte *p;

    p = pal;
    k = 0;
    // Grey
    for (i = 0; i < 64; i++) {
        p[0] = i;
        p[1] = i;
        p[2] = i;
        p += 3;
        k++;
    }
    // Red
    for (i = 0; i < 24; i++) {
        p[0] = 55*i/23 + 8;
        p[1] = 0;
        p[2] = 0;
        p += 3;
        k++;
    }
    // Light Red
    for (i = 0; i < 8; i++) {
        p[0] = 55*i/7 + 8;
        p[1] = p[0] / 2;
        p[2] = p[0] / 2;
        p += 3;
        k++;
    }
    // Green
    for (i = 0; i < 24; i++) {
        p[0] = 0;
        p[1] = 55*i/23 + 8;
        p[2] = 0;
        p += 3;
        k++;
    }
    // Light Green
    for (i = 0; i < 8; i++) {
        p[1] = 55*i/7 + 8;
        p[0] = p[1] / 2;
        p[2] = p[1] / 2;
        p += 3;
        k++;
    }
    // Blue
    for (i = 0; i < 24; i++) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 55*i/23 + 8;
        p += 3;
        k++;
    }
    // Light Blue
    for (i = 0; i < 8; i++) {
        p[2] = 55*i/7 + 8;
        p[0] = p[2] / 2;
        p[1] = p[2] / 2;
        p += 3;
        k++;
    }
    // Yellow
    for (i = 0; i < 24; i++) {
        p[0] = 55*i/23 + 8;
        p[1] = 55*i/23 + 8;
        p[2] = 0;
        p += 3;
        k++;
    }
    // Light Yellow
    for (i = 0; i < 8; i++) {
        p[0] = 55*i/7 + 8;
        p[1] = 55*i/7 + 8;
        p[2] = p[0] / 2;
        p += 3;
        k++;
    }
    // Magenta
    for (i = 0; i < 24; i++) {
        p[0] = 55*i/23 + 8;
        p[1] = 0;
        p[2] = 55*i/23 + 8;
        p += 3;
        k++;
    }
    // Light Magenta
    for (i = 0; i < 8; i++) {
        p[0] = 55*i/7 + 8;
        p[1] = p[0] / 2;
        p[2] = 55*i/7 + 8;
        p += 3;
        k++;
    }
    // Cyan
    for (i = 0; i < 24; i++) {
        p[0] = 0;
        p[1] = 55*i/23 + 8;
        p[2] = 55*i/23 + 8;
        p += 3;
        k++;
    }
    // Light Cyan
    for (i = 0; i < 8; i++) {
        p[1] = 55*i/7 + 8;
        p[0] = p[1] / 2;
        p[2] = 55*i/7 + 8;
        p += 3;
        k++;
    }
}

/******************************************************************************/
