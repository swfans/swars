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
#include "bfscreen.h"
#include "bftext.h"

long LbGetJustifiedCharPosX(long startx, long all_chars_width,
  long spr_width, long mul_width, ushort fdflags)
{
    long justifyx;
    if ((fdflags & Lb_TEXT_HALIGN_LEFT) != 0)
    {
        return startx;
    } else
    if ((fdflags & Lb_TEXT_HALIGN_RIGHT) != 0)
    {
        justifyx = lbTextJustifyWindow.x - lbTextClipWindow.x;
        return startx + (lbTextJustifyWindow.width + justifyx +
          mul_width*spr_width - all_chars_width);
    } else
    if ((fdflags & Lb_TEXT_HALIGN_CENTER) != 0)
    {
        justifyx = lbTextJustifyWindow.x - lbTextClipWindow.x;
        return startx + (lbTextJustifyWindow.width + justifyx +
          mul_width*spr_width - all_chars_width) / 2;
    } else
    if ((fdflags & Lb_TEXT_HALIGN_JUSTIFY) != 0)
    {
        return startx;
    }
    return startx;
}

long LbGetJustifiedCharPosY(long starty, long all_lines_height,
  long spr_height, ushort fdflags)
{
    // No vertical justification supported - so the decision is simple
    return starty;
}

long LbGetJustifiedCharWidth(long all_chars_width, long spr_width,
  long words_count, int units_per_px, ushort fdflags)
{
    long justifyx;
    long space_width;
    if ((fdflags & Lb_TEXT_HALIGN_JUSTIFY) != 0)
    {
        space_width = LbTextCharWidth(' ') * units_per_px / 16;
        justifyx = lbTextJustifyWindow.x - lbTextClipWindow.x;
        if (words_count > 0)
            return spr_width + (lbTextJustifyWindow.width + justifyx +
              space_width - all_chars_width) / words_count;
        return spr_width;
    }
    return spr_width;
}

long LbGetJustifiedCharHeight(long all_lines_height, long spr_height,
  long lines_count, ushort fdflags)
{
    // No vertical justification supported - so the decision is simple
    return spr_height;
}

long LbTextCharWidth(const ulong chr)
{
    return LbSprFontCharWidth(lbFontPtr,(ubyte)chr);
}

long LbTextCharHeight(const ulong chr)
{
    return LbSprFontCharHeight(lbFontPtr,(ubyte)chr);
}

long LbSprFontCharWidth(const struct TbSprite *font, const ulong chr)
{
    const struct TbSprite *spr;

    spr = LbFontCharSprite(font, chr);
    if (spr == NULL)
        return 0;
    return spr->SWidth;
}

long LbSprFontCharHeight(const struct TbSprite *font, const ulong chr)
{
  const struct TbSprite *spr;
  spr = LbFontCharSprite(font, chr);
  if (spr == NULL)
    return 0;
  return spr->SHeight;
}

const struct TbSprite *LbFontCharSprite(const struct TbSprite *font,
  const ulong chr)
{
    if (font == NULL)
        return NULL;
    if ((chr >= 31) && (chr < 256))
        return &font[(chr-31)];
    return NULL;
}

/******************************************************************************/
