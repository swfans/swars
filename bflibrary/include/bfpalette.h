/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfpalette.h
 *     Header file for gpalette.cpp, spalette.cpp.
 * @par Purpose:
 *     Video support library for indexed colour graphics.
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
#ifndef BFLIBRARY_BFPALETTE_H_
#define BFLIBRARY_BFPALETTE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

#define PALETTE_8b_COLORS 256
#define PALETTE_8b_SIZE (3*PALETTE_8b_COLORS)
#define PALETTE_FADE_LEVELS 64


struct Palette { // sizeof=3
    u8 Red;
    u8 Green;
    u8 Blue;
};

typedef struct Palette Palette;

#pragma pack()

/** Perform palette fading.
 * Does either one step, or several steps of the fading process.
 *
 * @param from_pal Initial palette for fading. If NULL, current screen
 *        palette is used.
 * @param fade_to Target step of the fade.
 * @param fade_steps Total amount of fading steps. If set to 1, the
 *        fading can be done in one call, and the function will return
 *        only when the fading has completed.
 */
TbResult LbPaletteFade(u8 *from_pal, u8 fade_to, u8 fade_steps);

/** Stops any fading currently in progress.
 */
TbResult LbPaletteStopOpenFade(void);

/** Finds a colour index in given palette best matching given RGB values.
 * Uses linear colour distance algorithm.
 */
TbPixel LbPaletteFindColour(const u8 *pal, u8 r, u8 g, u8 b);

/** Finds a colour index in given palette best matching given RGB values.
 * Uses linear RGB colour distance algorithm.
 */
TbPixel LbPaletteFindColourLinear(const u8 *pal, u8 r, u8 g, u8 b);

/** Finds a colour index in given palette best matching given RGB values.
 * Uses waged linear RGB colour distance algorithm, with non-squared wages.
 */
TbPixel LbPaletteFindColourHalfWaged(const u8 *pal, u8 r, u8 g, u8 b);

/** Draw the current palette on screen, in form ot 16x16 table.
 */
TbResult LbPaletteDraw(i32 X, i32 Y,
  u32 Width, u32 Height, ushort Flags);

/** Retrieves the indexed colour video palette.
 *
 * @param palette Pointer to target palette colors buffer.
 * @return Lb_SUCCESS, or error code.
 */
TbResult LbPaletteGet(u8 *palette);

/** Sets the indexed colour video palette.
 *
 * @param palette Pointer to the palette colors data.
 * @return Lb_SUCCESS, or error code.
 */
TbResult LbPaletteSet(const u8 *palette);

/** Clears the indexed colour video palette with black colour.
 * Only writes values to given palette bufer - does no screen operations.
 *
 * @param palette Pointer to the palette colors data.
 * @return Lb_SUCCESS, or error code.
 */
TbResult LbPaletteDataFillBlack(u8 *palette);

/** Clears the indexed colour video palette with white colour.
 *
 * @param palette Pointer to the palette colors data.
 * @return Lb_SUCCESS, or error code.
 */
TbResult LbPaletteDataFillWhite(u8 *palette);

/** Get colour distance by computing squares of coordinates.
 */
i32 LbColourDistanceLinearSq(u8 r1, u8 g1, u8 b1,
  u8 r2, u8 g2, u8 b2);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFPALETTE_H_
/******************************************************************************/
