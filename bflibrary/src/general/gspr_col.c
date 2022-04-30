/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gspr_col.cpp
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

#include <string.h>
#include "insprite.h"
#include "bfscreen.h"
#include "bflog.h"


/** @internal
 *  Draw part of sprite line with single colour.
 *
 * @param buf_out
 * @param colour
 * @param buf_len
 * @param mirror
 */
static inline void LbDrawBufferOneColour(unsigned char **buf_out, const TbPixel colour,
        const int buf_len, const TbBool mirror)
{
    int i;
    if ( mirror )
    {
        if ( lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4 )
        {
            for (i=0; i<buf_len; i++ )
            {
                **buf_out = lbDisplay.GlassMap[(colour<<8) + **buf_out];
                (*buf_out)--;
            }
        } else
        {
            for (i=0; i<buf_len; i++ )
            {
                **buf_out = lbDisplay.GlassMap[((**buf_out)<<8) + colour];
                (*buf_out)--;
            }
        }
    } else
    {
        if ( lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4 )
        {
            for (i=0; i<buf_len; i++ )
            {
                **buf_out = lbDisplay.GlassMap[(colour<<8) + **buf_out];
                (*buf_out)++;
            }
        } else
        {
            for (i=0; i<buf_len; i++ )
            {
                **buf_out = lbDisplay.GlassMap[((**buf_out)<<8) + colour];
                (*buf_out)++;
            }
        }
    }
}

/** @internal
 *  Draw part of sprite line with single colour.
 *
 * @param buf_out
 * @param colour
 * @param buf_len
 * @param mirror
 */
static inline void LbDrawBufferOneColorSolid(unsigned char **buf_out, const TbPixel colour,
        const int buf_len, const TbBool mirror)
{
    int i;
    if ( mirror )
    {
        for (i=0; i < buf_len; i++)
        {
            **buf_out = colour;
            (*buf_out)--;
        }
    } else
    {
        for (i=0; i < buf_len; i++)
        {
            **buf_out = colour;
            (*buf_out)++;
        }
    }
}

/** @internal
 * Draw one line of a transparent sprite.
 *
 * @param sp
 * @param r
 * @param x1
 * @param lpos
 * @param mirror
 */
static inline void LbSpriteDrawLineTrOneColour(const char **sp, unsigned char **r, short *x1,
    TbPixel colour, short lpos,const TbBool mirror)
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
            LbDrawBufferOneColour(r,colour,drawOut,mirror);
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
            LbDrawBufferOneColour(r,colour,drawOut,mirror);
            (*x1) -= schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 * Draw whole transparent sprite in one color.
 */
static inline TbResult LbSpriteDrawTrOneColour(const char *sp,short sprWd,short sprHt,
        unsigned char *r,TbPixel colour,int nextRowDelta,short left,const TbBool mirror)
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
        LbSpriteDrawLineTrOneColour(&sp,&r,&x1,colour,lpos,mirror);
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
 * Draw one line of a solid sprite.
 */
static inline void LbSpriteDrawLineSlOneColour(const char **sp, unsigned char **r, short *x1,
    TbPixel colour, short lpos,const TbBool mirror)
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
            LbDrawBufferOneColorSolid(r,colour,drawOut,mirror);
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
            LbDrawBufferOneColorSolid(r,colour,drawOut,mirror);
            (*x1) -= schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 * Draw whole solid sprite in one color.
 */
static inline TbResult LbSpriteDrawSlOneColour(const char *sp,short sprWd,short sprHt,
        unsigned char *r,TbPixel colour,int nextRowDelta,short left,const TbBool mirror)
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
        LbSpriteDrawLineSlOneColour(&sp,&r,&x1,colour,lpos,mirror);
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
 * Draw single line of a sprite in one color using fast copy.
 */
static inline void LbSpriteDrawLineFCOneColour(const char **sp, unsigned char **r, short *x1, TbPixel colour, short lpos)
{
    char schr;
    unsigned char drawOut;
    if (lpos > 0)
    {
        // Draw the part of current block which exceeds value of 'lpos'
        schr = *(*sp);
        if (schr < 0)
        {
            drawOut = -schr - lpos;
            if (drawOut > (*x1))
              drawOut = (*x1);
            (*r) += drawOut;
            (*sp)++;
        } else
        {
            drawOut = schr - lpos;
            if (drawOut > (*x1))
              drawOut = (*x1);
            LbDrawBufferOneColorSolid(r, colour, drawOut, false);
            (*r) += drawOut;
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
            (*r) -= *(*sp);
            (*sp)++;
        } else
        //if ( schr > 0 )
        { // Drawing some pixels
            drawOut = schr;
            if (drawOut >= (*x1))
                drawOut = (*x1);
            memset((*r), colour, drawOut);
            (*x1) -= schr;
            (*r) += schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 * Fast copy one color sprite drawing routine. Does not support transparency nor mirroring.
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
static inline TbResult LbSpriteDrawFCOneColour(const char *sp,short sprWd,short sprHt,unsigned char *r,
    TbPixel colour,int nextRowDelta,short left,const TbBool mirror)
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
        LbSpriteDrawLineFCOneColour(&sp,&r,&x1,colour,lpos);
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

TbResult LbSpriteDrawOneColour(long x, long y, const TbSprite *spr, const TbPixel colour)
{
    TbSpriteDrawData spd;
    TbResult ret;

    LIBLOG("At (%ld,%ld)",x,y);
    ret = LbSpriteDrawPrepare(&spd, x, y, spr);
    if (ret != Lb_SUCCESS)
        return ret;
    if ((lbDisplay.DrawFlags & (Lb_SPRITE_TRANSPAR4|Lb_SPRITE_TRANSPAR8)) != 0) {
        return LbSpriteDrawTrOneColour(spd.sp,spd.Wd,spd.Ht,spd.r,colour,spd.nextRowDelta,spd.startShift,spd.mirror);
    } else
    if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0) {
        return LbSpriteDrawSlOneColour(spd.sp,spd.Wd,spd.Ht,spd.r,colour,spd.nextRowDelta,spd.startShift,spd.mirror);
    } else {
        return LbSpriteDrawFCOneColour(spd.sp,spd.Wd,spd.Ht,spd.r,colour,spd.nextRowDelta,spd.startShift,spd.mirror);
    }
}

/******************************************************************************/
