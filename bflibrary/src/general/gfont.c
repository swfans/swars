/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gfont.c
 *     Handling of sprite fonts on single character level.
 * @par Purpose:
 *     Provides basics for drawing text on screen.
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
#include "bffont.h"
#include "bfsprite.h"

long LbSprFontCharWidth(const struct TbSprite *font, const ulong chr)
{
    const struct TbSprite *spr;

    spr = LbFontCharSprite(font,chr);
    if (spr == NULL)
        return 0;
    return spr->SWidth;
}

long LbSprFontCharHeight(const struct TbSprite *font, const ulong chr)
{
  const struct TbSprite *spr;
  spr = LbFontCharSprite(font,chr);
  if (spr == NULL)
    return 0;
  return spr->SHeight;
}

const struct TbSprite *LbFontCharSprite(const struct TbSprite *font, const ulong chr)
{
    if (font == NULL)
        return NULL;
    if ((chr >= 31) && (chr < 256))
        return &font[(chr-31)];
    return NULL;
}

/******************************************************************************/
