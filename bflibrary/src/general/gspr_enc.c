/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gspr_enc.c
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     Encodes raw pixel buffer to sprite data.
 * @author   Tomasz Lis
 * @date     12 Jan 2024 - 20 Mar 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>
#include "bfsprite.h"

#include "bfmemut.h"
#include "bfscreen.h"
#include "privbflog.h"

static int LbI_SpriteEncodeLine(const TbPixel *p_inbuf, short width, TbPixel transcol, TbPixel *p_outrow)
{
    int area;
    int outIndex;
    short i;

    outIndex = 0;
    i = 0;
    while (i < width)
    {
        // Filled
        area = 0;
        while ((i + area < width) && (p_inbuf[i + area] != transcol)) {
            area++;
        }
        LOGDBG("fill area %d", area);
        while (area > 0)
        {
            int part_area;

            if (area > 127) {
                part_area = 127;
                area -= 127;
            } else {
                part_area = area;
                area = 0;
            }
            *(sbyte *)(p_outrow + outIndex) = (sbyte)(part_area);
            outIndex += sizeof(sbyte);
            LbMemoryCopy(p_outrow + outIndex, p_inbuf + i, part_area);
            outIndex += part_area;
            i += part_area;
        }
        // Transparent
        area = 0;
        while ((i + area < width) && (p_inbuf[i + area] == transcol)) {
            area++;
        }
        LOGDBG("trans area %d", area);
        if (i+area >= width) {
            i += area;
            area = 0;
        }
        while (area > 0)
        {
            int part_area;

            if (area > 127) {
                part_area = 127;
                area -= 127;
            } else {
                part_area = area;
                area = 0;
            }
            *(sbyte *)(p_outrow + outIndex) = (sbyte)(-part_area);
            outIndex += sizeof(sbyte);
            i += part_area;
        }
    }
    { // End a line with 0
        *(sbyte *)(p_outrow + outIndex) = 0;
        outIndex += sizeof(sbyte);
    }
    return outIndex;
}

int LbSpriteEncode(const TbPixel *p_buf, short scanline, TbPixel transcol, TbSprite *p_spr)
{
    TbSpriteData p_outrow;
    short y;

    p_outrow = p_spr->Data;
    for (y = 0; y < p_spr->SHeight; y++)
    {
        int ln_len;

        ln_len = LbI_SpriteEncodeLine(p_buf, p_spr->SWidth, transcol, p_outrow);
        p_outrow += ln_len;
        p_buf += scanline;
    }
    return (p_outrow - p_spr->Data);
}

int LbScreenSpriteEncode(TbSprite *p_spr, TbSpriteData p_spr_data)
{
    TbPixel transcol;

    LOGNO("at (%ld,%ld)", x, y);
    transcol = lbDisplay.WScreen[0];
    p_spr->SWidth = lbDisplay.GraphicsWindowWidth;
    p_spr->SHeight = lbDisplay.GraphicsWindowHeight;
    assert(p_spr->SWidth == lbDisplay.GraphicsWindowWidth);
    assert(p_spr->SHeight == lbDisplay.GraphicsWindowHeight);
    p_spr->Data = p_spr_data;

    return LbSpriteEncode(lbDisplay.GraphicsWindowPtr, lbDisplay.GraphicsScreenWidth, transcol, p_spr);
}

/******************************************************************************/
