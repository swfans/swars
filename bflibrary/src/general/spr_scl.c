/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file spr_scl.c
 *     Sprite scaling arrays preparations.
 * @par Purpose:
 *     Fills arrays used for drawing scaled sprites.
 * @par Comment:
 *     Part of 8-bit graphics canvas drawing library.
 *     Used for drawing sprites on screen.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfsprite.h"

#include "insspr.h"
#include "bfscreen.h"
#include "privbflog.h"

i32 xsteps_array[2*SPRITE_SCALING_XSTEPS];
i32 ysteps_array[2*SPRITE_SCALING_YSTEPS];

i32 alpha_xsteps_array[2*SPRITE_SCALING_XSTEPS];
i32 alpha_ysteps_array[2*SPRITE_SCALING_YSTEPS];

i32 scale_up;
i32 alpha_scale_up;

// TODO move these to rendering/trig data module
TbPixel *render_ghost = NULL;
TbPixel *render_alpha = NULL;
//TODO currently, render_ghost is unused; but to make it ready to use, we need to set:
//render_ghost = lbSpriteReMapPtr;


void LbSpriteSetScalingWidthClippedArray(i32 * xsteps_arr, i32 x, i32 swidth, i32 dwidth, i32 gwidth)
{
    i32 *pwidth;
    i32 pxpos;
    pwidth = xsteps_arr;
    i32 factor = (dwidth<<16)/swidth;
    i32 tmp = (factor >> 1) + (x << 16);
    pxpos = tmp >> 16;
    i32 w = swidth;
    do {
        tmp += factor;
        i32 pxstart,pxend;
        pxstart = pxpos;
        pxend = tmp>>16;
        // Remember unclipped difference
        i32 wdiff = pxend - pxstart;
        // Now clip to graphics line bounds
        if (pxstart < 0) {
            pxstart = 0;
            pxend = pxstart;
        } else
        if (pxstart >= gwidth) {
            pxstart = gwidth-1;
            pxend = pxstart;
        } else
        if (pxend < 0) {
            pxend = 0;
        } else
        if (pxend > gwidth) {
            pxend = gwidth;
        }
        // Set clipped difference to be drawn
        pwidth[0] = pxstart;
        pwidth[1] = pxend - pxstart;
        // But store position for the unclipped difference
        pxpos += wdiff;
        w--;
        pwidth += 2;
    } while (w > 0);
}

void LbSpriteSetScalingWidthClipped(i32 x, i32 swidth, i32 dwidth, i32 gwidth)
{
    LOGDBG("starting %d -> %d at %d",(int)swidth,(int)dwidth,(int)x);
    if (swidth > SPRITE_SCALING_XSTEPS)
        swidth = SPRITE_SCALING_XSTEPS;
    LbSpriteSetScalingWidthClippedArray(xsteps_array, x, swidth, dwidth, gwidth);
}

void LbSpriteSetAlphaScalingWidthClipped(i32 x, i32 swidth, i32 dwidth, i32 gwidth)
{
    LOGDBG("starting %d -> %d at %d",(int)swidth,(int)dwidth,(int)x);
    if (swidth > SPRITE_SCALING_XSTEPS)
        swidth = SPRITE_SCALING_XSTEPS;
    LbSpriteSetScalingWidthClippedArray(alpha_xsteps_array, x, swidth, dwidth, gwidth);
}

void LbSpriteSetScalingWidthSimpleArray(i32 * xsteps_arr, i32 x, i32 swidth, i32 dwidth)
{
    i32 *pwidth;
    i32 cwidth;
    pwidth = xsteps_arr;
    i32 factor = (dwidth<<16)/swidth;
    i32 tmp = (factor >> 1) + (x << 16);
    cwidth = tmp >> 16;
    i32 w = swidth;
    do {
      int i;
      for (i=0; i < 16; i+=2)
      {
          pwidth[i] = cwidth;
          tmp += factor;
          pwidth[i+1] = (tmp>>16) - cwidth;
          cwidth = (tmp>>16);
          w--;
          if (w <= 0)
              break;
      }
      pwidth += 16;
    } while (w > 0);
}

void LbSpriteSetScalingWidthSimple(i32 x, i32 swidth, i32 dwidth)
{
    LOGDBG("starting %d -> %d at %d",(int)swidth,(int)dwidth,(int)x);
    if (swidth > SPRITE_SCALING_XSTEPS)
        swidth = SPRITE_SCALING_XSTEPS;
    LbSpriteSetScalingWidthSimpleArray(xsteps_array, x, swidth, dwidth);
}

void LbSpriteSetAlphaScalingWidthSimple(i32 x, i32 swidth, i32 dwidth)
{
    LOGDBG("starting %d -> %d at %d",(int)swidth,(int)dwidth,(int)x);
    if (swidth > SPRITE_SCALING_XSTEPS)
        swidth = SPRITE_SCALING_XSTEPS;
    LbSpriteSetScalingWidthSimpleArray(alpha_xsteps_array, x, swidth, dwidth);
}

void LbSpriteClearScalingWidthArray(i32 * xsteps_arr, i32 swidth)
{
    int i;
    i32 *pwidth;
    pwidth = xsteps_arr;
    for (i=0; i < swidth; i++)
    {
        pwidth[0] = 0;
        pwidth[1] = 0;
        pwidth += 2;
    }
}

void LbSpriteClearScalingWidth(void)
{
    LbSpriteClearScalingWidthArray(xsteps_array, SPRITE_SCALING_XSTEPS);
}

void LbSpriteClearAlphaScalingWidth(void)
{
    LbSpriteClearScalingWidthArray(alpha_xsteps_array, SPRITE_SCALING_XSTEPS);
}

void LbSpriteSetScalingHeightClippedArray(i32 * ysteps_arr, i32 y, i32 sheight, i32 dheight, i32 gheight)
{
    i32 *pheight;
    i32 lnpos;
    pheight = ysteps_arr;
    i32 factor = (dheight<<16)/sheight;
    i32 tmp = (factor >> 1) + (y << 16);
    lnpos = tmp >> 16;
    if (lnpos < 0)
        lnpos = 0;
    if (lnpos >= gheight)
        lnpos = gheight;
    i32 h = sheight;
    do {
        tmp += factor;
        i32 lnstart,lnend;
        lnstart = lnpos;
        lnend = tmp>>16;
        // Remember unclipped difference
        i32 hdiff = lnend - lnstart;
        // Now clip to graphics line bounds
        if (lnstart < 0) {
            lnstart = 0;
            lnend = lnstart;
        } else
        if (lnstart >= gheight) {
            lnstart = gheight-1;
            lnend = lnstart;
        } else
        if (lnend < 0) {
            lnend = 0;
        } else
        if (lnend > gheight) {
            lnend = gheight;
        }
        // Set clipped difference to be drawn
        pheight[0] = lnstart;
        pheight[1] = lnend - lnstart;
        // But store position for the unclipped difference
        lnpos += hdiff;
        h--;
        pheight += 2;
    } while (h > 0);
}

void LbSpriteSetScalingHeightClipped(i32 y, i32 sheight, i32 dheight, i32 gheight)
{
    LOGSYNC("starting %d -> %d at %d",(int)sheight,(int)dheight,(int)y);
    if (sheight > SPRITE_SCALING_YSTEPS)
        sheight = SPRITE_SCALING_YSTEPS;
    LbSpriteSetScalingHeightClippedArray(ysteps_array, y, sheight, dheight, gheight);
}

void LbSpriteSetAlphaScalingHeightClipped(i32 y, i32 sheight, i32 dheight, i32 gheight)
{
    LOGDBG("starting %d -> %d at %d",(int)sheight,(int)dheight,(int)y);
    if (sheight > SPRITE_SCALING_YSTEPS)
        sheight = SPRITE_SCALING_YSTEPS;
    LbSpriteSetScalingHeightClippedArray(alpha_ysteps_array, y, sheight, dheight, gheight);
}

void LbSpriteSetScalingHeightSimpleArray(i32 * ysteps_arr, i32 y, i32 sheight, i32 dheight)
{
    i32 *pheight;
    i32 cheight;
    pheight = ysteps_arr;
    i32 factor = (dheight<<16)/sheight;
    i32 tmp = (factor >> 1) + (y << 16);
    cheight = tmp >> 16;
    i32 h = sheight;
    do {
      int i=0;
      for (i=0; i < 16; i+=2)
      {
        pheight[i] = cheight;
        tmp += factor;
        pheight[i+1] = (tmp>>16) - cheight;
        cheight = (tmp>>16);
        h--;
        if (h <= 0)
          break;
      }
      pheight += 16;
    } while (h > 0);
}

void LbSpriteSetScalingHeightSimple(i32 y, i32 sheight, i32 dheight)
{
    LOGDBG("starting %d -> %d at %d",(int)sheight,(int)dheight,(int)y);
    if (sheight > SPRITE_SCALING_YSTEPS)
        sheight = SPRITE_SCALING_YSTEPS;
    LbSpriteSetScalingHeightSimpleArray(ysteps_array, y, sheight, dheight);
}

void LbSpriteSetAlphaScalingHeightSimple(i32 y, i32 sheight, i32 dheight)
{
    LOGDBG("starting %d -> %d at %d",(int)sheight,(int)dheight,(int)y);
    if (sheight > SPRITE_SCALING_YSTEPS)
        sheight = SPRITE_SCALING_YSTEPS;
    LbSpriteSetScalingHeightSimpleArray(alpha_ysteps_array, y, sheight, dheight);
}

void LbSpriteClearScalingHeightArray(i32 * ysteps_arr, i32 sheight)
{
    int i;
    i32 *pheight;
    pheight = ysteps_arr;
    for (i=0; i < sheight; i++)
    {
        pheight[0] = 0;
        pheight[1] = 0;
        pheight += 2;
    }
}

void LbSpriteClearScalingHeight(void)
{
    LbSpriteClearScalingHeightArray(ysteps_array, SPRITE_SCALING_YSTEPS);
}

void LbSpriteClearAlphaScalingHeight(void)
{
    LbSpriteClearScalingHeightArray(alpha_ysteps_array, SPRITE_SCALING_YSTEPS);
}

void LbSpriteSetScalingData(i32 x, i32 y, i32 swidth, i32 sheight,
    i32 dwidth, i32 dheight)
{
    i32 gwidth = lbDisplay.GraphicsWindowWidth;
    i32 gheight = lbDisplay.GraphicsWindowHeight;
    scale_up = true;
    if ((dwidth <= swidth) && (dheight <= sheight))
        scale_up = false;
    // Checking whether to select simple scaling creation,
    // or more comprehensive one - with clipping
    if ((swidth <= 0) || (dwidth <= 0)) {
        LOGWARN("tried scaling width %ld -> %ld", swidth, dwidth);
        LbSpriteClearScalingWidth();
    } else
    // Normally it would be enough to check if ((dwidth+y) >= gwidth),
    // but due to rounding we need to add swidth
    if ((x < 0) || ((dwidth+swidth+x) >= gwidth))
    {
        LbSpriteSetScalingWidthClipped(x, swidth, dwidth, gwidth);
    } else {
        LbSpriteSetScalingWidthSimple(x, swidth, dwidth);
    }
    if ((sheight <= 0) || (dheight <= 0)) {
        LOGWARN("tried scaling height %ld -> %ld", sheight, dheight);
        LbSpriteClearScalingHeight();
    } else
    // Normally it would be enough to check if ((dheight+y) >= gheight),
    // but our simple rounding may enlarge the image
    if ((y < 0) || ((dheight+sheight+y) >= gheight))
    {
        LbSpriteSetScalingHeightClipped(y, sheight, dheight, gheight);
    } else {
        LbSpriteSetScalingHeightSimple(y, sheight, dheight);
    }
}

void SetAlphaScalingData(i32 x, i32 y, i32 swidth, i32 sheight,
    i32 dwidth, i32 dheight)
{
    i32 gwidth = lbDisplay.GraphicsWindowWidth;
    i32 gheight = lbDisplay.GraphicsWindowHeight;
    alpha_scale_up = true;
    if ((dwidth <= swidth) && (dheight <= sheight))
        alpha_scale_up = false;
    if ((swidth <= 0) || (dwidth <= 0)) {
        LOGWARN("tried scaling width %ld -> %ld", swidth, dwidth);
        LbSpriteClearAlphaScalingWidth();
    } else
    if ((x < 0) || ((dwidth+x) >= gwidth))
    {
        LbSpriteSetAlphaScalingWidthClipped(x, swidth, dwidth, gwidth);
    } else {
        LbSpriteSetAlphaScalingWidthSimple(x, swidth, dwidth);
    }
    if ((sheight <= 0) || (dheight <= 0)) {
        LOGWARN("tried scaling height %ld -> %ld", sheight, dheight);
        LbSpriteClearAlphaScalingHeight();
    } else
    if ((y < 0) || ((dheight+y) >= gheight))
    {
        LbSpriteSetAlphaScalingHeightClipped(y, sheight, dheight, gheight);
    } else {
        LbSpriteSetAlphaScalingHeightSimple(y, sheight, dheight);
    }
}

void LbPixelBlockCopyForward(TbPixel * dst, const TbPixel * src, i32 len)
{
    TbPixel px;
    u32 pxquad;
    if ( !((int)dst & 3) || ((px = *src, ++src, *dst = px, ++dst, --len, len)
     && (!((int)dst & 3) || ((px = *src, ++src, *dst = px, ++dst, --len, len)
     && (!((int)dst & 3) ||  (px = *src, ++src, *dst = px, ++dst, --len, len))))) )
    {
        i32 l;
        for ( l = len>>2; l > 0; l--)
        {
            pxquad = *(u32 *)src;
            src += sizeof(u32);
            *(u32 *)dst = pxquad;
            dst += sizeof(u32);
        }
        if (len & 3)
        {
          *dst = *src;
          if ((len & 3) != 1)
          {
            *(dst + 1) = *(src + 1);
            if ((len & 3) != 2)
              *(dst + 2) = *(src + 2);
          }
        }
    }
}

/******************************************************************************/
