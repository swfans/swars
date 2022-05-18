/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file spalette.cpp
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
#include <stdio.h>
#include <assert.h>
#include <SDL/SDL.h>
#include "bfpalette.h"

#include "bfscreen.h"
#include "bfscrsurf.h"
#include "bflog.h"

#define to_SDLSurf(h) ((SDL_Surface  *)h)

/** Colour palette buffer, to be used inside lbDisplay. */
ubyte lbPalette[PALETTE_8b_SIZE];
/** Driver-specific colour palette buffer. */
SDL_Color lbPaletteColors[PALETTE_8b_COLORS];

TbResult LbPaletteGet(ubyte *palette)
{
    LIBLOG("Starting");
    if ((!lbScreenInitialised) || (lbDrawSurface == NULL))
      return Lb_FAIL;

    if (lbDisplay.Palette == NULL)
        return Lb_FAIL;

    memcpy(palette, lbDisplay.Palette, PALETTE_8b_SIZE);

    return Lb_SUCCESS;
}

TbResult LbPaletteSet(const ubyte *palette)
{
    unsigned char * bufColors;
    SDL_Color * destColors;
    const unsigned char * srcColors;
    unsigned long i;
    TbResult ret;
    LIBLOG("Starting");
    if ((!lbScreenInitialised) || (lbDrawSurface == NULL))
      return Lb_FAIL;

    destColors = lbPaletteColors;
    srcColors = palette;
    bufColors = lbPalette;
    if ((destColors == NULL) || (srcColors == NULL))
      return Lb_FAIL;
    ret = Lb_SUCCESS;
    for (i = 0; i < PALETTE_8b_COLORS; i++) {
        // note that bufColors and srcColors could be the same pointer
        bufColors[0] = srcColors[0] & 0x3F;
        bufColors[1] = srcColors[1] & 0x3F;
        bufColors[2] = srcColors[2] & 0x3F;
        destColors[i].r = (bufColors[0] << 2);
        destColors[i].g = (bufColors[1] << 2);
        destColors[i].b = (bufColors[2] << 2);
        srcColors += 3;
        bufColors += 3;
    }
    //if (SDL_SetPalette(to_SDLSurf(lbDrawSurface), SDL_LOGPAL | SDL_PHYSPAL,
    if (SDL_SetColors(to_SDLSurf(lbDrawSurface),
        lbPaletteColors, 0, PALETTE_8b_COLORS) != 1) {
        LIBLOG("SDL SetPalette failed: %s", SDL_GetError());
        ret = Lb_FAIL;
    }
    lbDisplay.Palette = lbPalette;

    return ret;
}

/******************************************************************************/
