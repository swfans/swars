/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gspr_map.c
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
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

#include "insprite.h"
#include "bfscreen.h"
#include "privbflog.h"

/** @internal
 *  Draw part of sprite line.
 *
 * @param buf_out
 * @param buf_inp
 * @param buf_len
 * @param mirror
 */
static inline void LbDrawBufferTrRemap(unsigned char **buf_out, const char *buf_inp,
        const int buf_len, const unsigned char *cmap, const TbBool mirror)
{
  int i;
  unsigned int val;
  if ( mirror )
  {
    if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
    {
        for (i=0; i<buf_len; i++ )
        {
            val = cmap[*(const unsigned char *)buf_inp];
            **buf_out = lbDisplay.GlassMap[(val<<8) + **buf_out];
            buf_inp++;
            (*buf_out)--;
        }
    } else
    {
        for (i=0; i<buf_len; i++ )
        {
            val = cmap[*(const unsigned char *)buf_inp];
            **buf_out = lbDisplay.GlassMap[((**buf_out)<<8) + val];
            buf_inp++;
            (*buf_out)--;
        }
    }
  } else
  {
    if ( lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4 )
    {
        for (i=0; i<buf_len; i++ )
        {
            val = cmap[*(const unsigned char *)buf_inp];
            **buf_out = lbDisplay.GlassMap[(val<<8) + **buf_out];
            buf_inp++;
            (*buf_out)++;
        }
    } else
    {
        for (i=0; i<buf_len; i++ )
        {
            val = cmap[*(const unsigned char *)buf_inp];
            **buf_out = lbDisplay.GlassMap[((**buf_out)<<8) + val];
            buf_inp++;
            (*buf_out)++;
        }
    }
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
static inline void LbDrawBufferSlRemap(unsigned char **buf_out, const char *buf_inp,
        const int buf_len, const unsigned char *cmap, const TbBool mirror)
{
    int i;
    for (i=0; i < buf_len; i++)
    {
        **buf_out = cmap[*(const unsigned char *)buf_inp];
        buf_inp++;
        (*buf_out)--;
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
static inline void LbDrawBufferFCRemap(unsigned char **buf_out, const char *buf_inp,
        const int buf_len, const unsigned char *cmap)
{
    int i;
    for (i=0; i < buf_len; i++)
    {
        **buf_out = cmap[*(const unsigned char *)buf_inp];
        buf_inp++;
        (*buf_out)++;
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

/** @internal
 * Draw one line of a solid sprite with remapped colours.
 */
static inline void LbSpriteDrawLineSlRemap(const char **sp, unsigned char **r, short *x1,
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
            (*r) -= drawOut;
            (*sp)++;
        } else
        {
            // Draw the part of current block which exceeds value of 'lpos'
            drawOut = schr - lpos;
            if (drawOut > (*x1))
              drawOut = (*x1);
            LbDrawBufferSlRemap(r,(*sp)+(lpos+1),drawOut,cmap,mirror);
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
            (*r) += *(*sp);
            (*sp)++;
        } else
        //if ( schr > 0 )
        { // Drawing some pixels
            drawOut = schr;
            if (drawOut >= (*x1))
                drawOut = (*x1);
            LbDrawBufferSlRemap(r,(*sp)+1,drawOut,cmap,mirror);
            (*x1) -= schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 * Draw whole solid sprite with remapped colours.
 */
static inline TbResult LbSpriteDrawSlRemap(const char *sp,short sprWd,short sprHt,
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
        LbSpriteDrawLineSlRemap(&sp,&r,&x1,cmap,lpos,mirror);
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

/** @internal
 * Draw one line of a sprite with remapped colours using fast copy.
 */
static inline void LbSpriteDrawLineFCRemap(const char **sp, unsigned char **r, short *x1,
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
            LbDrawBufferFCRemap(r,(*sp)+(lpos+1),drawOut,cmap);
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
            LbDrawBufferFCRemap(r,(*sp)+1,drawOut,cmap);
            (*x1) -= schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 * Fast copy sprite drawing routine with colour remap. Does not support transparency nor mirroring.
 *
 * @param sp
 * @param sprWd
 * @param sprHt
 * @param r
 * @param nextRowDelta
 * @param left
 * @param mirror
 * @return
 */
static inline TbResult LbSpriteDrawFCRemap(const char *sp,short sprWd,short sprHt,unsigned char *r,
    const unsigned char *cmap,int nextRowDelta,short left,const TbBool mirror)
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
        LbSpriteDrawLineFCRemap(&sp,&r,&x1,cmap,lpos,mirror);
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

TbResult LbSpriteDrawRemap(long x, long y, const TbSprite *spr, const ubyte *cmap)
{
    TbSpriteDrawData spd;
    TbResult ret;

    LOGDBG("at (%ld,%ld)",x,y);
    ret = LbSpriteDrawPrepare(&spd, x, y, spr);
    if (ret != Lb_SUCCESS)
        return ret;
    if ((lbDisplay.DrawFlags & (Lb_SPRITE_TRANSPAR4|Lb_SPRITE_TRANSPAR8)) != 0) {
        return LbSpriteDrawTrRemap(spd.sp,spd.Wd,spd.Ht,spd.r,cmap,spd.nextRowDelta,spd.startShift,spd.mirror);
    } else
    if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0) {
        return LbSpriteDrawSlRemap(spd.sp,spd.Wd,spd.Ht,spd.r,cmap,spd.nextRowDelta,spd.startShift,spd.mirror);
    } else
    {
        return LbSpriteDrawFCRemap(spd.sp,spd.Wd,spd.Ht,spd.r,cmap,spd.nextRowDelta,spd.startShift,spd.mirror);
    }
}

/******************************************************************************/
