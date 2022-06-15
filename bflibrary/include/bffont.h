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

/**
 * Computes width of a single character in given font.
 * For characters that do not have a sprite (like tab), returns 0.
 */
long LbSprFontCharWidth(const struct TbSprite *font, const ulong chr);

/**
 * Computes height of a single character in given font.
 * For characters that do not have a sprite (like tab), returns 0.
 */
long LbSprFontCharHeight(const struct TbSprite *font, const ulong chr);

/**
 * Returns sprite of a single character in given font.
 * For characters that do not have a sprite, returns NULL.
 */
const struct TbSprite *LbFontCharSprite(const struct TbSprite *font, const ulong chr);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFFONT_H_
/******************************************************************************/
