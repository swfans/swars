/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file scr_copy.c
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
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
#include "bfscrcopy.h"
#include "bfscreen.h"

void *LbI_XMemCopy(void *dest, void *source, ulong len);

void *LbI_XMemRectCopy(void *dest, void *source, ulong lineLen,
  ulong width, ulong height);

void LbScreenCopyBox(TbPixel *sourceBuf, TbPixel *destBuf,
  long sourceX, long sourceY, long destX, long destY,
  ulong width, ulong height)
{
    ubyte *s;
    ubyte *d;

    s = &sourceBuf[sourceX + sourceY * lbDisplay.GraphicsScreenWidth];
    d = &destBuf[destX + destY * lbDisplay.GraphicsScreenWidth];

    // The function is able to copy non-aligned width without issues;
    // we are aligning it for original API compatibility
    LbI_XMemRectCopy(d, s, lbDisplay.GraphicsScreenWidth,
      width & ~0x3, height);
}

void LbScreenCopy(TbPixel *sourceBuf, TbPixel *destBuf, ushort height)
{
    ubyte *s;
    ubyte *d;
    short shift;
    short h;

    s = sourceBuf;
    d = destBuf;
    shift = lbDisplay.GraphicsScreenWidth - lbDisplay.GraphicsWindowWidth;
    // Note that source and destination buffers have different line lengths
    for (h = height; h > 0; h--)
    {
        LbI_XMemCopy(d, s, lbDisplay.GraphicsWindowWidth);
        s += lbDisplay.GraphicsWindowWidth;
        d += lbDisplay.GraphicsWindowWidth + shift;
    }
}

/** @internal
 *  Draw part of sprite line.
 *
 * @param buf_out
 * @param buf_inp
 * @param buf_len
 * @param mirror
 */
static inline void LbDrawBufferFCRemap(TbPixel **buf_out, const TbPixel *buf_inp,
        const int buf_len, const TbPixel *cmap)
{
    int i;

    for (i = 0; i < buf_len; i++)
    {
        **buf_out = cmap[*(const ubyte *)buf_inp];
        buf_inp++;
        (*buf_out)++;
    }
}

void LbScreenCopyRemap(TbPixel *sourceBuf, TbPixel *destBuf, ushort height,
  const TbPixel *cmap)
{
    ubyte *s;
    ubyte *d;
    short shift;
    short h;

    s = sourceBuf;
    d = destBuf;
    shift = lbDisplay.GraphicsScreenWidth - lbDisplay.GraphicsWindowWidth;
    // Note that source and destination buffers have different line lengths
    for (h = height; h > 0; h--)
    {
        LbDrawBufferFCRemap(&d, s, lbDisplay.GraphicsWindowWidth, cmap);
        s += lbDisplay.GraphicsWindowWidth;
        d += shift;
    }
}

void LbScreenSave(TbPixel *sourceBuf, TbPixel *destBuf, ushort height)
{
    ubyte *s;
    ubyte *d;
    short shift;
    short h;

    s = sourceBuf;
    d = destBuf;
    shift = lbDisplay.GraphicsScreenWidth - lbDisplay.GraphicsWindowWidth;
    // Note that source and destination buffers have different line lengths
    for (h = height; h > 0; h--)
    {
        LbI_XMemCopy(d, s, lbDisplay.GraphicsWindowWidth);
        s += lbDisplay.GraphicsWindowWidth + shift;
        d += lbDisplay.GraphicsWindowWidth;
    }
}

/******************************************************************************/
