/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file specblit.c
 *     Special graphics data copy (blit) routines.
 * @par Purpose:
 *     Implement functions for copying memory buffers with quirks used
 *     to perform some special blits of graphics data.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "specblit.h"

#include "bfmemut.h"
#include "bfscreen.h"

#include "swlog.h"
/******************************************************************************/

void ApScreenCopyRectColorKey(TbPixel *sourceBuf, TbPixel *destBuf,
  ushort sourceWidth, ushort destWidth, ushort height, TbPixel ckey)
{
    ubyte *s;
    ubyte *d;
    short shift;
    short w, h;

    s = sourceBuf;
    d = destBuf;
    shift = destWidth - sourceWidth;
    // Note that source and destination buffers have different line lengths
    for (h = height; h > 0; h--)
    {
        for (w = 0; w < sourceWidth; w++)
        {
            if (*s != ckey)
                *d = *s;
            s++;
            d++;
        }
        d += shift;
    }
}

void ApScreenCopyColorKey(TbPixel *sourceBuf, TbPixel *destBuf, ushort height, TbPixel ckey)
{
    ubyte *s;
    ubyte *d;
    short shift;
    short w, h;

    s = sourceBuf;
    d = destBuf;
    shift = lbDisplay.GraphicsScreenWidth - lbDisplay.GraphicsWindowWidth;
    // Note that source and destination buffers have different line lengths
    for (h = height; h > 0; h--)
    {
        for (w = 0; w < lbDisplay.GraphicsWindowWidth; w++)
        {
            if (*s != ckey)
                *d = *s;
            s++;
            d++;
        }
        d += shift;
    }
}

/******************************************************************************/
