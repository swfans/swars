/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ap_gspr_map.c
 *     Sprites drawing with remap, application modification for panel.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     Part of 8-bit graphics canvas drawing library.
 *     Note: Lines modified from original (other than function names) are
 *     marked by comments. The rest of the code is a copy from `gspr_map.c`.
 *     If you want to introduce changes, consider doing that to the original.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "app_sprite.h"

#include "insprite.h"
#include "bfgentab.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "privbflog.h"

TbResult LbSpriteDrawSlRemap(const char *sp,short sprWd,short sprHt,
        unsigned char *r,const unsigned char *cmap,int nextRowDelta,short left,const TbBool mirror);
TbResult LbSpriteDrawFCRemap(const char *sp,short sprWd,short sprHt,unsigned char *r,
    const unsigned char *cmap,int nextRowDelta,short left,const TbBool mirror);

/** @internal
 *  Draw part of sprite line.
 *
 * @param buf_out
 * @param buf_inp
 * @param buf_len
 * @param mirror
 */
static inline void LbDrawBufferTrRemap(unsigned char **buf_out, const char *buf_inp,
        const int buf_len, const unsigned char *transmap, const TbBool mirror)
{
  int i;
  if ( mirror )
  {
    if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
    {
        for (i=0; i<buf_len; i++ )
        {
            // App-specific code starts
            unsigned int pxmap;
            int pxbase;
            pxbase = dword_1DC36C + (SCANNER_pal_bright[*(const ubyte *)buf_inp] >> 1);
            pxmap = pxbase + (SCANNER_pal_bright[**buf_out] >> 1);
            pxmap = (SCANNER_bright_limit[pxmap] << 8) | ((*(const ubyte *)buf_inp));
            **buf_out = transmap[pxmap];
            // App-specific code ends
            buf_inp++;
            (*buf_out)--;
        }
    } else
    {
        for (i=0; i<buf_len; i++ )
        {
            // App-specific code starts
            unsigned int pxmap;
            int pxbase;
            pxbase = dword_1DC36C + (SCANNER_pal_bright[*(const ubyte *)buf_inp] >> 1);
            pxmap = pxbase + (SCANNER_pal_bright[**buf_out] >> 1);
            pxmap = (SCANNER_bright_limit[pxmap]) | ((*(const ubyte *)buf_inp) << 8);
            **buf_out = transmap[pxmap];
            // App-specific code ends
            buf_inp++;
            (*buf_out)--;
        }
    }
  } else
  {
    if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
    {
        for (i=0; i<buf_len; i++ )
        {
            // App-specific code starts
            unsigned int pxmap;
            int pxbase;
            pxbase = dword_1DC36C + (SCANNER_pal_bright[*(const ubyte *)buf_inp] >> 1);
            pxmap = pxbase + (SCANNER_pal_bright[**buf_out] >> 1);
            pxmap = (SCANNER_bright_limit[pxmap] << 8) | ((*(const ubyte *)buf_inp));
            **buf_out = transmap[pxmap];
            // App-specific code ends
            buf_inp++;
            (*buf_out)++;
        }
    } else
    {
        for (i=0; i<buf_len; i++ )
        {
            // App-specific code starts
            unsigned int pxmap;
            int pxbase;
            pxbase = dword_1DC36C + (SCANNER_pal_bright[*(const ubyte *)buf_inp] >> 1);
            pxmap = pxbase + (SCANNER_pal_bright[**buf_out] >> 1);
            pxmap = (SCANNER_bright_limit[pxmap]) | ((*(const ubyte *)buf_inp) << 8);
            **buf_out = transmap[pxmap];
            // App-specific code ends
            buf_inp++;
            (*buf_out)++;
        }
    }
  }
}

/** @internal
 * Draw one line of a transparent sprite with remapped colours.
 *
 * @param sp
 * @param r
 * @param x1
 * @param lpos
 * @param mirror
 */
static inline void LbSpriteDrawLineTrRemap(const char **sp, unsigned char **r, short *x1,
    const unsigned char *cmap, short lpos,const TbBool mirror)
{
    char schr;
    unsigned char drawOut;
    // Draw any unfinished block, which should be only partially visible
    if (lpos > 0)
    {
        schr = *(*sp);
        if (schr < 0)
        {
            drawOut = -schr - lpos;
            if (drawOut > (*x1))
              drawOut = (*x1);
            if ( mirror )
                (*r) -= drawOut;
            else
                (*r) += drawOut;
            (*sp)++;

        } else
        {
            // Draw the part of current block which exceeds value of 'lpos'
            drawOut = schr - lpos;
            if (drawOut > (*x1))
              drawOut = (*x1);
            LbDrawBufferTrRemap(r,(*sp)+(lpos+1),drawOut,cmap,mirror);
            // Update positions and break the skipping loop
            (*sp) += (*(*sp)) + 1;
        }
        (*x1) -= drawOut;
    }
    // Draw the visible part of a sprite
    while ((*x1) > 0)
    {
        schr = *(*sp);
        if (schr == 0)
        { // EOL, breaking line loop
            break;
        }
        if (schr < 0)
        { // Skipping some pixels
            (*x1) += schr;
            if ( mirror )
               (*r) += *(*sp);
            else
               (*r) -= *(*sp);
            (*sp)++;
        } else
        //if ( schr > 0 )
        { // Drawing some pixels
            drawOut = schr;
            if (drawOut >= (*x1))
                drawOut = (*x1);
            LbDrawBufferTrRemap(r,(*sp)+1,drawOut,cmap,mirror);
            (*x1) -= schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 * Draw whole transparent sprite with remapped colours.
 */
static inline TbResult LbSpriteDrawTrRemap(const char *sp,short sprWd,short sprHt,
        unsigned char *r,const unsigned char *cmap,int nextRowDelta,short left,const TbBool mirror)
{
    unsigned char *nextRow;
    long htIndex;
    nextRow = &(r[nextRowDelta]);
    htIndex = sprHt;
    // For all lines of the sprite
    while (1)
    {
        short x1;
        short lpos;
        x1 = sprWd;
        // Skip the pixels left before drawing area
        lpos = LbSpriteDrawLineSkipLeft(&sp,&x1,left);
        // Do the actual drawing
        LbSpriteDrawLineTrRemap(&sp,&r,&x1,cmap,lpos,mirror);
        // Go to next line
        htIndex--;
        if (htIndex == 0)
            return Lb_SUCCESS;
        LbSpriteDrawLineSkipToEol(&sp,&x1);
        r = nextRow;
        nextRow += nextRowDelta;
    } //end while
    return Lb_SUCCESS;
}

TbResult ApSpriteDrawLowTransGreyRemap(long x, long y, const TbSprite *spr, const ubyte *transmap)
{
    TbSpriteDrawData spd;
    TbResult ret;

    LOGDBG("at (%ld,%ld)",x,y);
    ret = LbSpriteDrawPrepare(&spd, x, y, spr);
    if (ret != Lb_SUCCESS)
        return ret;
    if ((lbDisplay.DrawFlags & (Lb_SPRITE_TRANSPAR4|Lb_SPRITE_TRANSPAR8)) != 0) {
        return LbSpriteDrawTrRemap(spd.sp,spd.Wd,spd.Ht,spd.r,transmap,
          spd.nextRowDelta,spd.startShift,spd.mirror);
    } else
    if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0) {
        ubyte bri;
        bri = (dword_1DC36C <= 8) ? (24 + dword_1DC36C) : (32 + (dword_1DC36C - 8) / 2);
        return LbSpriteDrawSlRemap(spd.sp,spd.Wd,spd.Ht,spd.r,&transmap[bri * PALETTE_8b_COLORS],
          spd.nextRowDelta,spd.startShift,spd.mirror);
    } else
    {
        ubyte bri;
        bri = (dword_1DC36C <= 8) ? (24 + dword_1DC36C) : (32 + (dword_1DC36C - 8) / 2);
        return LbSpriteDrawFCRemap(spd.sp,spd.Wd,spd.Ht,spd.r,&transmap[bri * PALETTE_8b_COLORS],
          spd.nextRowDelta,spd.startShift,spd.mirror);
    }
}

/******************************************************************************/
