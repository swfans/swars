/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bffont.h
 *     Header file for bffont.c.
 * @par Purpose:
 *     Handling of sprite fonts on single character level.
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
#ifndef BFLIBRARY_BFFONT_H_
#define BFLIBRARY_BFFONT_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TbSprite;

/** Computes standard height of a line of text, in currently set font.
 */
i32 LbTextLineHeight(void);

/** Computes height of a line of text, in currently set font.
 */
i32 LbTextHeight(const char *text);

/**
 * Computes width of a single character in current font.
 * For characters that do not have a sprite (like tab), returns 0.
 * The currently set font is used for the computations.
 */
i32 LbTextCharWidth(const u32 chr);

/**
 * Computes height of a single character in current font.
 * For characters that do not have a sprite (like tab), returns 0.
 * The currently set font is used for the computations.
 */
i32 LbTextCharHeight(const u32 chr);

/**
 * Computes width of a single character in given font.
 * For characters that do not have a sprite (like tab), returns 0.
 */
i32 LbSprFontCharWidth(const struct TbSprite *font, const u32 chr);

/**
 * Computes height of a single character in given font.
 * For characters that do not have a sprite (like tab), returns 0.
 */
i32 LbSprFontCharHeight(const struct TbSprite *font, const u32 chr);

/**
 * Returns sprite of a single character in given font.
 * For characters that do not have a sprite, returns NULL.
 */
const struct TbSprite *LbFontCharSprite(const struct TbSprite *font,
  const u32 chr);

/**
 * Draws an underline below the character.
 * @param pos_x
 * @param pos_y
 * @param width
 * @param height
 * @param draw_colr
 * @param shadow_colr
 */
void LbDrawCharUnderline(i32 pos_x, i32 pos_y, i32 width, i32 height,
  TbPixel draw_colr, TbPixel shadow_colr);

/**
 * Returns X coordinate for a text character on screen.
 * Takes into account the current text window and justification settings.
 */
i32 LbGetJustifiedCharPosX(i32 startx, i32 all_chars_width,
  i32 spr_width, i32 mul_width, ushort fdflags);

/**
 * Returns Y coordinate for a text character on screen.
 * Takes into account the current text window and justification settings.
 */
i32 LbGetJustifiedCharPosY(i32 starty, i32 all_lines_height,
  i32 spr_height, ushort fdflags);

/**
 * Returns width for an empty space between words in text on screen.
 * Takes into account the current text window and justification settings.
 */
i32 LbGetJustifiedCharWidth(i32 all_chars_width,
  i32 spr_width, i32 words_count, int units_per_px, ushort fdflags);

/**
 * Returns height for an empty space between lines in text on screen.
 * Takes into account the current text window and justification settings.
 */
i32 LbGetJustifiedCharHeight(i32 all_lines_height,
  i32 spr_height, i32 lines_count, ushort fdflags);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFFONT_H_
/******************************************************************************/
