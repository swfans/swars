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

struct TbAnyWindow lbTextJustifyWindow_UNUSED;
struct TbAnyWindow lbTextClipWindow_UNUSED;
int lbSpacesPerTab_UNUSED;
struct TbSprite *lbFontPtr_UNUSED;

TbResult LbTextSetJustifyWindow(ulong x, ulong y, ulong width)
{
    lbTextJustifyWindow.x = x;
    lbTextJustifyWindow.y = y;
    lbTextJustifyWindow.width = width;
    lbTextJustifyWindow.ptr = &lbDisplay.WScreen[x + y * lbDisplay.GraphicsScreenWidth];
    return Lb_SUCCESS;
}

TbResult LbTextSetClipWindow(ulong x, ulong y, ulong width, ulong height)
{
    long start_x,start_y;
    long end_x,end_y;
    long i;
    start_x = x;
    start_y = y;
    end_x = x + width;
    end_y = y + height;
    if (x > end_x)
    {
      i = x ^ end_x;
      start_x = i ^ x;
      end_x = i ^ x ^ i;
    }
    if ( y > end_y )
    {
      i = y ^ end_y;
      start_y = i ^ y;
      end_y = i ^ y ^ i;
    }
    if ( start_x < 0 )
        start_x = 0;
    if ( end_x < 0 )
      end_x = 0;
    if ( start_y < 0 )
        start_y = 0;
    if ( end_y < 0 )
      end_y = 0;
    if (start_x > lbDisplay.GraphicsScreenWidth)
        start_x = lbDisplay.GraphicsScreenWidth;
    if (end_x > lbDisplay.GraphicsScreenWidth)
      end_x = lbDisplay.GraphicsScreenWidth;
    if (start_y > lbDisplay.GraphicsScreenHeight)
        start_y = lbDisplay.GraphicsScreenHeight;
    if (end_y > lbDisplay.GraphicsScreenHeight)
      end_y = lbDisplay.GraphicsScreenHeight;
    lbTextClipWindow.x = start_x;
    lbTextClipWindow.y = start_y;
    lbTextClipWindow.width = end_x - start_x;
    lbTextClipWindow.height = end_y - start_y;
    lbTextClipWindow.ptr = &lbDisplay.WScreen[x + lbDisplay.GraphicsScreenWidth * y];
    return Lb_SUCCESS;
}

TbResult LbTextSetWindow(ulong x, ulong y, ulong width, ulong height)
{
    LbTextSetJustifyWindow(x, y, width);
    LbTextSetClipWindow(x, y, width, height);
    return Lb_SUCCESS;
}

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

TbResult LbTextDraw_UNUSED(long X, long Y, const char *Text)
{
}

/******************************************************************************/
