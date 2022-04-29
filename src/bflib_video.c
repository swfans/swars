/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_video.c
 *     Video support library for 8-bit graphics.
 * @par Purpose:
 *     Allows displaying on graphics device - graphic canvas setup and locking
 *     functions.
 * @par Comment:
 *     This is SDL-based implementation of the video routines.
 * @author   Tomasz Lis
 * @date     11 Feb 2008 - 10 Oct 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_video.h"

#include <string.h>

#include "bflib_basics.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

/******************************************************************************/
TbScreenCoord LbGraphicsScreenWidth(void)
{
    return lbDisplay.GraphicsScreenWidth;
}

TbScreenCoord LbGraphicsScreenHeight(void)
{
    return lbDisplay.GraphicsScreenHeight;
}

/** Resolution in width of the current video mode.
 *  Note that it's not always "physical" size,
 *  and it definitely can't be used as pitch/scanline
 *  (size of data for one line) in the graphics buffer.
 *
 *  But it is the width that will be visible on screen.
 *
 * @return
 */
TbScreenCoord LbScreenWidth(void)
{
    return lbDisplay.PhysicalScreenWidth;
}

TbScreenCoord LbScreenHeight(void)
{
    return lbDisplay.PhysicalScreenHeight;
}


/** Clears the 8-bit video palette with black colour.
 * Only writes values to given palette bufer - does no screen operations.
 *
 * @param palette Pointer to the palette colors data.
 * @return Lb_SUCCESS, or error code.
 */
TbResult LbPaletteDataFillBlack(unsigned char *palette)
{
    memset(palette, 0, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

/** Clears the 8-bit video palette with white colour.
 *
 * @param palette Pointer to the palette colors data.
 * @return Lb_SUCCESS, or error code.
 */
TbResult LbPaletteDataFillWhite(unsigned char *palette)
{
    memset(palette, 0x3F, PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}

/** Sets the 8-bit video palette.
 *
 * @param palette Pointer to the palette colors data.
 * @return Lb_SUCCESS, or error code.
 */
/* version from display.c is currently in use
TbResult LbPaletteSet(unsigned char *palette)
{
    unsigned char * bufColors;
    SDL_Color * destColors;
    const unsigned char * srcColors;
    unsigned long i;
    TbResult ret;
    SYNCDBG(12,"Starting");
    if ((!lbScreenInitialised) || (lbDrawSurface == NULL))
      return Lb_FAIL;
    destColors = lbPaletteColors;
    srcColors = palette;
    bufColors = lbPalette;
    if ((destColors == NULL) || (srcColors == NULL))
      return Lb_FAIL;
    ret = Lb_SUCCESS;
    for (i = 0; i < PALETTE_COLORS; i++) {
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
    //if (SDL_SetPalette(lbDrawSurface, SDL_LOGPAL | SDL_PHYSPAL,
    if (SDL_SetColors(lbDrawSurface,
        lbPaletteColors, 0, PALETTE_COLORS) != 1) {
        SYNCDBG(8,"SDL SetPalette failed.");
        ret = Lb_FAIL;
    }
    //free(destColors);
    lbDisplay.Palette = lbPalette;
    return ret;
}*/

/** Retrieves the 8-bit video palette.
 *
 * @param palette Pointer to target palette colors buffer.
 * @return Lb_SUCCESS, or error code.
 */
/* To be enabled when needed
TbResult LbPaletteGet(unsigned char *palette)
{
    SYNCDBG(12,"Starting");
    if ((!lbScreenInitialised) || (lbDrawSurface == NULL))
      return Lb_FAIL;
    if (lbDisplay.Palette == NULL)
        return Lb_FAIL;
    memcpy(palette,lbDisplay.Palette,PALETTE_8b_SIZE);
    return Lb_SUCCESS;
}
*/
/******************************************************************************/
#ifdef __cplusplus
}
#endif
