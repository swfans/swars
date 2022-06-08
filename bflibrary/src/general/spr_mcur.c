/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file spr_mcur.c
 *     Sprites drawing with scaling, for mouse cursor.
 * @par Purpose:
 *     Drawing scaled mouse cursor, independenlty of other sprites.
 * @par Comment:
 *     Part of 8-bit graphics canvas drawing library.
 *     Used for drawing sprites on screen.
 * @author   Tomasz Lis
 * @date     12 Feb 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfsprite.h"

#include "insspr.h"
#include "privbflog.h"

struct SSurface;

#define CURSOR_SCALING_XSTEPS MAX_SUPPORTED_SCREEN_WIDTH/10
#define CURSOR_SCALING_YSTEPS MAX_SUPPORTED_SCREEN_HEIGHT/10

//TODO add scaling support for non-advanced pointer
/** Cursor with advanced draw can be undrawn, restoring the image underneath.
 * But besides that, it is actually simpler, as it currently does not support scaling.
 */
volatile TbBool lbPointerAdvancedDraw = true;

long cursor_xsteps_array[2*CURSOR_SCALING_XSTEPS];
long cursor_ysteps_array[2*CURSOR_SCALING_YSTEPS];

// function used for actual drawing
TbResult LbSpriteDrawUsingScalingUpDataSolidLR(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const struct TbSprite *sprite);

/******************************************************************************/

void LbCursorSpriteSetScalingWidthClipped(long x, long swidth, long dwidth, long gwidth)
{
    LOGDBG("starting %ld -> %ld at %ld", swidth, dwidth, x);
    if (swidth > CURSOR_SCALING_XSTEPS)
        swidth = CURSOR_SCALING_XSTEPS;
    LbSpriteSetScalingWidthClippedArray(cursor_xsteps_array, x, swidth, dwidth, gwidth);
}

void LbCursorSpriteSetScalingWidthSimple(long x, long swidth, long dwidth)
{
    LOGDBG("starting %ld -> %ld at %ld", swidth, dwidth, x);
    if (swidth > CURSOR_SCALING_XSTEPS)
        swidth = CURSOR_SCALING_XSTEPS;
    LbSpriteSetScalingWidthSimpleArray(cursor_xsteps_array, x, swidth, dwidth);
}

void LbCursorSpriteSetScalingHeightClipped(long y, long sheight, long dheight, long gheight)
{
    LOGDBG("starting %ld -> %ld at %ld", sheight, dheight, y);
    if (sheight > CURSOR_SCALING_YSTEPS)
        sheight = CURSOR_SCALING_YSTEPS;
    LbSpriteSetScalingHeightClippedArray(cursor_ysteps_array, y, sheight, dheight, gheight);
}

void LbCursorSpriteSetScalingHeightSimple(long y, long sheight, long dheight)
{
    LOGDBG("starting %ld -> %ld at %ld", sheight, dheight, y);
    if (sheight > CURSOR_SCALING_YSTEPS)
        sheight = CURSOR_SCALING_YSTEPS;
    LbSpriteSetScalingHeightSimpleArray(cursor_ysteps_array, y, sheight, dheight);
}

/**
 * Draws the mouse pointer sprite on a display buffer.
 */
long PointerDraw(long x, long y, struct TbSprite *spr, TbPixel *outbuf, unsigned long scanline)
{
    unsigned int dwidth,dheight;
    // Prepare bounds
    dwidth = spr->SWidth * lbUnitsPerPixel / 16;
    dheight = spr->SHeight * lbUnitsPerPixel / 16;
    if ( (dwidth <= 0) || (dheight <= 0) )
        return 1;
    if ( (lbDisplay.MouseWindowWidth <= 0) || (lbDisplay.MouseWindowHeight <= 0) )
        return 1;
    // Normally it would be enough to check if ((dwidth+x) >= gwidth), but due to rounding we need to add swidth
    if ((x < 0) || ((dwidth + spr->SWidth + x) >= lbDisplay.MouseWindowWidth))
    {
        LbCursorSpriteSetScalingWidthClipped(x, spr->SWidth, dwidth, lbDisplay.MouseWindowWidth);
    } else {
        LbCursorSpriteSetScalingWidthSimple(x, spr->SWidth, dwidth);
    }
    // Normally it would be enough to check if ((dheight+y) >= gheight), but our simple rounding may enlarge the image
    if ((y < 0) || (dheight + spr->SHeight + y >= lbDisplay.MouseWindowHeight))
    {
        LbCursorSpriteSetScalingHeightClipped(y, spr->SHeight, dheight, lbDisplay.MouseWindowHeight);
    } else {
        LbCursorSpriteSetScalingHeightSimple(y, spr->SHeight, dheight);
    }
    long *xstep;
    long *ystep;
    {
        xstep = &cursor_xsteps_array[0];
        ystep = &cursor_ysteps_array[0];
    }
    outbuf = &outbuf[xstep[0] + scanline * ystep[0]];
    return LbSpriteDrawUsingScalingUpDataSolidLR(outbuf, scanline, lbDisplay.MouseWindowHeight, xstep, ystep, spr);
}

/******************************************************************************/
