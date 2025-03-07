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
#include "bfscrcopy.h"
#include "bfutility.h"

#include "app_sprite.h"
#include "swlog.h"
/******************************************************************************/

void ApScreenSetRect(TbPixel *destBuf, TbPixel px,
  ushort setWidth, ushort destWidth, ushort height)
{
    ubyte *d;
    short h;

    d = destBuf;
    for (h = height; h > 0; h--)
    {
        LbMemorySet(d, px, min(destWidth,setWidth));
        d += destWidth;
    }
}

void ApScreenCopyRect(TbPixel *sourceBuf, TbPixel *destBuf,
  ushort sourceWidth, ushort destWidth, ushort height)
{
    ubyte *s;
    ubyte *d;
    short h;

    s = sourceBuf;
    d = destBuf;
    // Note that source and destination buffers have different line lengths
    for (h = height; h > 0; h--)
    {
        LbMemoryCopy(d, s, min(destWidth,sourceWidth));
        s += sourceWidth;
        d += destWidth;
    }
}

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

void ApScreenCopyColorKey(TbPixel *sourceBuf, TbPixel *destBuf,
  ushort height, TbPixel ckey)
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

void ApScreenCopyTrans1LR(TbPixel *sourceBuf, TbPixel *destBuf,
  ushort height, const ubyte *transmap)
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
            // App-specific code starts
            unsigned int pxmap;
            pxmap = ((*d) << 8) | ((*(const ubyte *)s));
            *d = transmap[pxmap];
            // App-specific code ends
            s++;
            d++;
        }
        d += shift;
    }
}

void ApScreenCopyTrans2LR(TbPixel *sourceBuf, TbPixel *destBuf,
  ushort height, const ubyte *transmap)
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
            // App-specific code starts
            unsigned int pxmap;
            pxmap = ((*d)) | ((*(const ubyte *)s) << 8);
            *d = transmap[pxmap];
            // App-specific code ends
            s++;
            d++;
        }
        d += shift;
    }
}

void ApScreenCopyRemap(TbPixel *sourceBuf, TbPixel *destBuf,
  ushort height, const ubyte *transmap)
{
    if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
    {
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
        {
            //not implemented ApScreenCopyTrans1RL(sourceBuf, destBuf, height, transmap);
        }
        else
        {
            ApScreenCopyTrans1LR(sourceBuf, destBuf, height, transmap);
        }
    }
    else
    if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
    {
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
        {
            //not implemented ApScreenCopyTrans2RL(sourceBuf, destBuf, height, transmap);
        }
        else
        {
            ApScreenCopyTrans2LR(sourceBuf, destBuf, height, transmap);
        }
    }
    else
    {
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
        {
            //not implemented ApScreenCopySolidRL(sourceBuf, destBuf, height);
        }
        else
        {
            LbScreenCopy(sourceBuf, destBuf, height);
        }
    }
}
/******************************************************************************/
