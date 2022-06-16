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
#include "bftext.h"
#include "bfscreen.h"
#include "bffont.h"
#include "bfanywnd.h"

int lbSpacesPerTab_UNUSED;
struct TbSprite *lbFontPtr_UNUSED;

int LbTextStringWidth_UNUSED()
{
// code at 0001:000bb1a4
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

int LbTextStringHeight_UNUSED()
{
// code at 0001:000bb300
}

/**
 * Draws a string in the current text window in given scale.
 * @param posx Position of the text, X coord.
 * @param posy Position of the text, Y coord.
 * @param units_per_px Scale in pixels; 16 is 100%.
 * @param text The text to be drawn.
 * @return
 */
TbBool LbTextDrawResized(int posx, int posy, int units_per_px, const char *text)
{
#if 0
    struct TbAnyWindow grwnd;
    // Counter for amount of blank characters in a line
    long count;
    long justifyx,justifyy;
    long startx,starty;
    const char *sbuf;
    const char *ebuf;
    const char *prev_ebuf;
    long chr;
    long x,y,len;
    long w,h;
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
        chr = (unsigned char)*ebuf;
        if (is_wide_charcode(chr))
        {
            ebuf++;
            if (*ebuf == '\0') break;
            chr = (chr<<8) + (unsigned char)*ebuf;
        }

        if (chr > 32)
        {
            w = LbTextCharWidth(chr) * units_per_px / 16;
            if ((posx+w-justifyx <= lbTextJustifyWindow.width) || (count > 0) ||
              !LbAlignMethodSet(lbDisplay.DrawFlags))
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
            if (LbAlignMethodSet(lbDisplay.DrawFlags))
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
            if (LbAlignMethodSet(lbDisplay.DrawFlags))
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
#endif
}

TbResult LbTextDraw_UNUSED(long X, long Y, const char *Text)
{
}

/******************************************************************************/
