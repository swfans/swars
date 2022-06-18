/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gtext.c
 *     Functions for drawing text on graphical screen.
 * @par Purpose:
 *     Allows drawing text using various positioning and font sprites.
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
#include <limits.h>
#include "bftext.h"
#include "bfsprite.h"
#include "bfscreen.h"
#include "bffont.h"
#include "bfmemory.h"
#include "bfanywnd.h"
#include "privbflog.h"

#if defined(ENABLE_SHADOW_COLOUR)
#  define SHADOW_COLOUR lbDisplay.ShadowColour
#else
#  define SHADOW_COLOUR 0x00
#endif

ubyte lbSpacesPerTab;
struct TbSprite *lbFontPtr;

/** @internal
 * Returns if the given char starts a wide charcode.
 * @param chr the 8-bit char to check
 */
TbBool is_wide_charcode(ulong chr)
{
    return false;
}

TbBool LbIApplyControlCharToDrawSettings(const char **c)
{
    ubyte chr;

    chr = (ubyte)(**c);
    switch (chr)
    {
      case 1:
        lbDisplay.DrawFlags ^= Lb_SPRITE_TRANSPAR4;
        break;
      case 2:
        lbDisplay.DrawFlags ^= Lb_SPRITE_TRANSPAR8;
        break;
      case 3:
        lbDisplay.DrawFlags ^= Lb_SPRITE_OUTLINE;
        break;
      case 4:
        lbDisplay.DrawFlags ^= Lb_SPRITE_FLIP_HORIZ;
        break;
      case 5:
        lbDisplay.DrawFlags ^= Lb_SPRITE_FLIP_VERTIC;
        break;
      case 11:
        lbDisplay.DrawFlags ^= Lb_TEXT_UNDERLINE;
        break;
      case 12:
        lbDisplay.DrawFlags ^= Lb_TEXT_ONE_COLOR;
        break;
      case 14:
        (*c)++;
        lbDisplay.DrawColour = (ubyte)(**c);
        break;
      default:
        return false;
    }
    return true;
}

/** @internal
 * Puts simple text sprites on screen.
 * @param sbuf
 * @param ebuf
 * @param x
 * @param y
 * @param len
 */
void put_down_simpletext_sprites(const char *sbuf, const char *ebuf, long x, long y, long len)
{
  const char *c;
  const struct TbSprite *spr;
  ubyte chr;
  long w,h;
  for (c=sbuf; c < ebuf; c++)
  {
    chr = (ubyte)(*c);
    if (chr > 32)
    {
      spr = LbFontCharSprite(lbFontPtr,chr);
      if (spr != NULL)
      {
        if ((lbDisplay.DrawFlags & Lb_TEXT_ONE_COLOR) != 0)
          LbSpriteDrawOneColour(x, y, spr, lbDisplay.DrawColour);
        else
          LbSpriteDraw(x, y, spr);
        w = spr->SWidth;
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLINE) != 0)
        {
            h = LbTextLineHeight();
            LbDrawCharUnderline(x, y, w, h, lbDisplay.DrawColour, SHADOW_COLOUR);
        }
        x += w;
      }
    } else
    if (chr == ' ')
    {
        w = len;
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLINE) != 0)
        {
            h = LbTextLineHeight();
            LbDrawCharUnderline(x, y, w, h, lbDisplay.DrawColour, SHADOW_COLOUR);
        }
        x += w;
    } else
    if (chr == '\t')
    {
        w = len*(long)lbSpacesPerTab;
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLINE) != 0)
        {
            h = LbTextLineHeight();
            LbDrawCharUnderline(x, y, w, h, lbDisplay.DrawColour, SHADOW_COLOUR);
        }
        x += w;
    } else
    {
        LbIApplyControlCharToDrawSettings(&c);
    }
  }
}

/** @internal
 * Puts scaled simple text sprites on screen.
 * @param sbuf
 * @param ebuf
 * @param x
 * @param y
 * @param len
 */
void put_down_simpletext_sprites_resized(const char *sbuf, const char *ebuf,
  long x, long y, long space_len, int units_per_px)
{
  const char *c;
  const struct TbSprite *spr;
  ubyte chr;
  long w,h;
  for (c=sbuf; c < ebuf; c++)
  {
    chr = (ubyte)(*c);
    if (chr > 32)
    {
      spr = LbFontCharSprite(lbFontPtr,chr);
      if (spr != NULL)
      {
        if ((lbDisplay.DrawFlags & Lb_TEXT_ONE_COLOR) != 0) {
            LbSpriteDrawResizedOneColour(x, y, units_per_px, spr, lbDisplay.DrawColour);
        } else {
            LbSpriteDrawResized(x, y, units_per_px, spr);
        }
        w = spr->SWidth * units_per_px / 16;
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLINE) != 0)
        {
            h = LbTextLineHeight() * units_per_px / 16;
            LbDrawCharUnderline(x, y, w, h, lbDisplay.DrawColour, SHADOW_COLOUR);
        }
        x += w;
      }
    } else
    if (chr == ' ')
    {
        w = space_len;
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLINE) != 0)
        {
            h = LbTextLineHeight() * units_per_px / 16;
            LbDrawCharUnderline(x, y, w, h, lbDisplay.DrawColour, SHADOW_COLOUR);
        }
        x += w;
    } else
    if (chr == '\t')
    {
        w = space_len*(long)lbSpacesPerTab;
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLINE) != 0)
        {
            h = LbTextLineHeight() * units_per_px / 16;
            LbDrawCharUnderline(x, y, w, h, lbDisplay.DrawColour, SHADOW_COLOUR);
        }
        x += w;
    } else
    {
        LbIApplyControlCharToDrawSettings(&c);
    }
  }
}

void put_down_sprites(const char *sbuf, const char *ebuf,
  long x, long y, long len, int units_per_px)
{
    if (units_per_px == 16)
    {
        put_down_simpletext_sprites(sbuf, ebuf, x, y, len);
    } else
    {
        put_down_simpletext_sprites_resized(sbuf, ebuf, x, y, len, units_per_px);
    }
}

/** @internal
 * Returns if given draw flags have text alignment method set.
 */
TbBool LbIAlignMethodSet(ushort fdflags)
{
    const unsigned short align_flags =
      Lb_TEXT_HALIGN_LEFT | Lb_TEXT_HALIGN_RIGHT |
      Lb_TEXT_HALIGN_CENTER | Lb_TEXT_HALIGN_JUSTIFY;
    if ((fdflags & align_flags) != 0)
        return true;
    return false;
}

long LbTextStringPartWidth(const char *text, long part)
{
    const char *ebuf;
    long chr;
    int len;
    int max_len;

    if (lbFontPtr == NULL)
        return 0;
    max_len = 0;
    len = 0;
    for (ebuf = text; *ebuf != '\0'; ebuf++)
    {
        if (part <= 0) break;
        part--;
        chr = (ubyte)*ebuf;
        if (is_wide_charcode(chr))
        {
            ebuf++;
            if (*ebuf == '\0') break;
            chr = (chr << 8) + (ubyte)*ebuf;
        }
        if (chr > 31)
        {
            len += LbTextCharWidth(chr);
        } else
        if (chr == '\r')
        {
            if (len > max_len)
                max_len = len;
            len = 0;
        } else
        if (chr == '\t')
        {
            len += lbSpacesPerTab * LbTextCharWidth(' ');
        } else
        if ((chr == 6) || (chr == 7) || (chr == 8) || (chr == 9) || (chr == 14))
        {
            ebuf++;
            if (*ebuf == '\0')
                break;
        }
    }
    if (len > max_len)
        max_len = len;
    return max_len;
}

long LbTextStringWidth(const char *text)
{
    return LbTextStringPartWidth(text, LONG_MAX);
}


long LbSprFontWordWidth(const struct TbSprite *font, const char *text)
{
    long len;
    const char *c;
    if ((font == NULL) || (text == NULL))
        return 0;
    c = text;
    len = 0;
    while ((*c != ' ') && (*c != '\t') && (*c != '\0') && (*c != '\r') && (*c != '\n'))
    {
      if ((ubyte)(*c) > 32)
        len += LbSprFontCharWidth(font, (ubyte)*c);
      c++;
    }
    return len;
}

long LbTextWordWidth(const char *text)
{
    return LbSprFontWordWidth(lbFontPtr, text);
}

long LbTextStringHeight(const char *text)
{
    long i, h, lines;
    lines = 1;
    if ((lbFontPtr == NULL) || (text == NULL))
        return 0;
    for (i = 0; i < TEXT_DRAW_MAX_LEN; i++)
    {
        if (text[i] == '\0') break;
        if (text[i] == '\r') lines++;
    }
    if (i >= TEXT_DRAW_MAX_LEN) {
        LOGWARN("Reached character limit while counting width");
    }
    h = LbTextLineHeight();
    return h * lines;
}

TbBool LbTextDrawResized(int posx, int posy, int units_per_px, const char *text)
{
    struct TbAnyWindow grwnd;
    // Counter for amount of blank characters in a line
    long count;
    long justifyx,justifyy;
    long startx,starty;
    const char *sbuf;
    const char *ebuf;
    const char *prev_ebuf;
    long chr;
    long x, y, len;
    long w, h;

    if ((lbFontPtr == NULL) || (text == NULL))
        return true;
    LbScreenStoreGraphicsWindow(&grwnd);
    LbScreenLoadGraphicsWindow(&lbTextClipWindow);
    count = 0;
    justifyx = lbTextJustifyWindow.x - lbTextClipWindow.x;
    justifyy = lbTextJustifyWindow.y - lbTextClipWindow.y;
    posx += justifyx;
    startx = posx;
    starty = posy + justifyy;

    h = LbTextLineHeight() * units_per_px / 16;
    sbuf = text;
    for (ebuf=text; *ebuf != '\0'; ebuf++)
    {
        prev_ebuf=ebuf-1;
        chr = (ubyte)*ebuf;
        if (is_wide_charcode(chr))
        {
            ebuf++;
            if (*ebuf == '\0') break;
            chr = (chr<<8) + (ubyte)*ebuf;
        }

        if (chr > 32)
        {
            w = LbTextCharWidth(chr) * units_per_px / 16;
            if ((posx+w-justifyx <= lbTextJustifyWindow.width) || (count > 0) ||
              !LbIAlignMethodSet(lbDisplay.DrawFlags))
            {
                posx += w;
                continue;
            }
            // If the char exceeds screen, and there were no spaces in that line,
            // and alignment is set - divide the line here
            w = LbTextCharWidth(' ') * units_per_px / 16;
            posx += w;
            x = LbGetJustifiedCharPosX(startx, posx, w, 1, lbDisplay.DrawFlags);
            y = LbGetJustifiedCharPosY(starty, h, h, lbDisplay.DrawFlags);
            len = LbGetJustifiedCharWidth(posx, w, count, units_per_px, lbDisplay.DrawFlags);
            ebuf = prev_ebuf;
            put_down_sprites(sbuf, ebuf, x, y, len, units_per_px);
            // We already know that alignment is set - don't re-check
            {
                posx = startx;
                sbuf = ebuf + 1; // sbuf points at start of char, while ebuf points at end of char
                starty += h;
            }
            count = 0;
        } else

        if (chr == ' ')
        {
            w = LbTextCharWidth(' ') * units_per_px / 16;
            len = LbSprFontWordWidth(lbFontPtr,ebuf+1) * units_per_px / 16;
            if (posx+w+len-justifyx <= lbTextJustifyWindow.width)
            {
                count++;
                posx += w;
                continue;
            }
            posx += w;
            x = LbGetJustifiedCharPosX(startx, posx, w, 1, lbDisplay.DrawFlags);
            y = LbGetJustifiedCharPosY(starty, h, h, lbDisplay.DrawFlags);
            len = LbGetJustifiedCharWidth(posx, w, count, units_per_px, lbDisplay.DrawFlags);
            put_down_sprites(sbuf, ebuf, x, y, len, units_per_px);
            // End the line only if align method is set
            if (LbIAlignMethodSet(lbDisplay.DrawFlags))
            {
              posx = startx;
              sbuf = ebuf + 1; // sbuf points at start of char, while ebuf points at end of char
              starty += h;
            }
            count = 0;
        } else

        if (chr == '\n')
        {
            w = 0;
            x = LbGetJustifiedCharPosX(startx, posx, w, 1, lbDisplay.DrawFlags);
            y = LbGetJustifiedCharPosY(starty, h, h, lbDisplay.DrawFlags);
            len = LbTextCharWidth(' ') * units_per_px / 16;
            y = starty;
            put_down_sprites(sbuf, ebuf, x, y, len, units_per_px);
            // We've got EOL sign - end the line
            sbuf = ebuf;
            posx = startx;
            starty += h;
            count = 0;
        } else

        if (chr == '\t')
        {
            w = LbTextCharWidth(' ') * units_per_px / 16;
            posx += lbSpacesPerTab*w;
            len = LbSprFontWordWidth(lbFontPtr,ebuf+1) * units_per_px / 16;
            if (posx+len-justifyx <= lbTextJustifyWindow.width)
            {
              count += lbSpacesPerTab;
              continue;
            }
            x = LbGetJustifiedCharPosX(startx, posx, w, lbSpacesPerTab, lbDisplay.DrawFlags);
            y = LbGetJustifiedCharPosY(starty, h, h, lbDisplay.DrawFlags);
            len = LbGetJustifiedCharWidth(posx, w, count, units_per_px, lbDisplay.DrawFlags);
            put_down_sprites(sbuf, ebuf, x, y, len, units_per_px);
            if (LbIAlignMethodSet(lbDisplay.DrawFlags))
            {
              posx = startx;
              sbuf = ebuf + 1;
              starty += h;
            }
            count = 0;
            continue;

        } else

        if ((chr == 6) || (chr == 7) || (chr == 8) || (chr == 9))
        {
            if (posx-justifyx > lbTextJustifyWindow.width)
            {
              x = startx;
              y = starty;
              len = LbTextCharWidth(' ') * units_per_px / 16;
              put_down_sprites(sbuf, ebuf, x, y, len, units_per_px);
              posx = startx;
              sbuf = ebuf;
              count = 0;
              starty += h;
            }
            switch (*ebuf)
            {
            case 6:
              lbDisplay.DrawFlags ^= Lb_TEXT_HALIGN_LEFT;
              break;
            case 7:
              lbDisplay.DrawFlags ^= Lb_TEXT_HALIGN_RIGHT;
              break;
            case 8:
              lbDisplay.DrawFlags ^= Lb_TEXT_HALIGN_CENTER;
              break;
            case 9:
              lbDisplay.DrawFlags ^= Lb_TEXT_HALIGN_JUSTIFY;
              break;
            }
        } else

        if (chr == 14)
        {
            ebuf++;
            if (*ebuf == '\0')
              break;
        }
    }
    x = LbGetJustifiedCharPosX(startx, posx, 0, 1, lbDisplay.DrawFlags);
    y = LbGetJustifiedCharPosY(starty, h, h, lbDisplay.DrawFlags);
    len = LbTextCharWidth(' ') * units_per_px / 16;
    put_down_sprites(sbuf, ebuf, x, y, len, units_per_px);
    LbScreenLoadGraphicsWindow(&grwnd);
    return true;
}

TbBool LbTextDraw(int posx, int posy, const char *text)
{
    // Using resized version - it will end up with version optimized for no resize anyway
    return LbTextDrawResized(posx, posy, 16, text);
}

TbBool LbTextDrawResizedVA(int posx, int posy, int units_per_px, const char *fmt, va_list arg)
{
    char * text = (char *)LbMemoryAlloc(TEXT_DRAW_MAX_LEN);
    if (text == NULL) return false;
    vsnprintf(text, TEXT_DRAW_MAX_LEN, fmt, arg);
    TbBool result = LbTextDrawResized(posx, posy, units_per_px, text);
    LbMemoryFree(text);
    return result;
}

TbBool LbTextDrawFmt(int posx, int posy, const char *fmt, ...)
{
    va_list val;
    va_start(val, fmt);
    TbBool result = LbTextDrawResizedVA(posx, posy, 16, fmt, val);
    va_end(val);
    return result;
}

TbBool LbTextDrawResizedFmt(int posx, int posy, int units_per_px, const char *fmt, ...)
{
    va_list val;
    va_start(val, fmt);
    TbBool result = LbTextDrawResizedVA(posx, posy, units_per_px, fmt, val);
    va_end(val);
    return result;
}


/******************************************************************************/
