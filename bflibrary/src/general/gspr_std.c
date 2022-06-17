/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gspr_std.cpp
 *     Implementation of drawing sprites on screen in one color.
 * @par Purpose:
 *     Draw sprites using given color for all non-transparent pixels.
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
#include "privbflog.h"

TbResult LbSpriteDrawPrepare(TbSpriteDrawData *spd, long x, long y, const TbSprite *spr)
{
    if (spr == NULL)
    {
        LOGWARN("at (%ld,%ld): NULL sprite", x, y);
        return Lb_FAIL;
    }
    if ((spr->SWidth < 1) || (spr->SHeight < 1))
    {
        LOGDBG("at (%ld,%ld): zero size sprite (%d,%d)", x, y, spr->SWidth, spr->SHeight);
        return Lb_OK;
    }
    if ((lbDisplay.GraphicsWindowWidth == 0) || (lbDisplay.GraphicsWindowHeight == 0))
    {
        LOGERR("at (%ld,%ld): invalid graphics window dimensions", x, y);
        return Lb_FAIL;
    }
    x += lbDisplay.GraphicsWindowX;
    y += lbDisplay.GraphicsWindowY;
    short left,right,top,btm;
    short sprWd = spr->SWidth;
    short sprHt = spr->SHeight;
    //Coordinates range checking - x coords
    int delta;
    delta = lbDisplay.GraphicsWindowX - x;
    if (delta <= 0)
    {
        left = 0;
    } else
    {
        if (sprWd <= delta)
            return Lb_OK;
        left = delta;
    }
    delta = x + sprWd - (lbDisplay.GraphicsWindowWidth+lbDisplay.GraphicsWindowX);
    if ( delta <= 0 )
    {
        right = sprWd;
    } else
    {
        if (sprWd <= delta)
            return Lb_OK;
        right = sprWd - delta;
    }
    //Coordinates range checking - y coords
    delta = lbDisplay.GraphicsWindowY - y;
    if (delta <= 0)
    {
      top = 0;
    } else
    {
      if (sprHt <= delta)
        return Lb_OK;
      top = delta;
    }
    delta = y + sprHt - (lbDisplay.GraphicsWindowHeight + lbDisplay.GraphicsWindowY);
    if (y + sprHt - (lbDisplay.GraphicsWindowHeight + lbDisplay.GraphicsWindowY) <= 0)
    {
      btm = sprHt;
    } else
    {
      if (sprHt <= delta)
        return Lb_OK;
      btm = sprHt - delta;
    }
    if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_VERTIC) != 0)
    {
        spd->r = &lbDisplay.WScreen[x + (y+btm-1)*lbDisplay.GraphicsScreenWidth + left];
        spd->nextRowDelta = -lbDisplay.GraphicsScreenWidth;
        short tmp_btm = btm;
        btm = sprHt - top;
        top = sprHt - tmp_btm;
    } else
    {
        spd->r = &lbDisplay.WScreen[x + (y+top)*lbDisplay.GraphicsScreenWidth + left];
        spd->nextRowDelta = lbDisplay.GraphicsScreenWidth;
    }
    spd->Ht = btm - top;
    spd->Wd = right - left;
    spd->sp = (char *)spr->Data;
    LOGNO("sprite coords X=%d...%d Y=%d...%d data=%08x", left, right, top, btm, spd->sp);
    long htIndex;
    if ( top )
    {
        htIndex = top;
        while ( 1 )
        {
            char chr = *(spd->sp);
            while (chr > 0)
            {
                spd->sp += chr + 1;
                chr = *(spd->sp);
            }
            spd->sp++;
            if (chr == 0)
            {
              htIndex--;
              if (htIndex <= 0) break;
            }
        }
    }
    LOGNO("at (%ld,%ld): drawing sprite of size (%d,%d)", x, y, (int)spd->Ht, (int)spd->Wd);
    if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
    {
        spd->r += spd->Wd - 1;
        spd->mirror = true;
        short tmpwidth = spr->SWidth;
        short tmpright = right;
        right = tmpwidth - left;
        spd->startShift = tmpwidth - tmpright;
    } else
    {
        spd->mirror = false;
        spd->startShift = left;
    }
    return Lb_SUCCESS;
}

short LbSpriteDrawLineSkipLeft(const char **sp, short *x1, short left)
{
    char schr;
    // Cut the left side of the sprite, if needed
    if (left != 0)
    {
        short lpos = left;
        while (lpos > 0)
        {
            schr = *(*sp);
            // Value > 0 means count of filled characters, < 0 means skipped characters
            // Equal to 0 means EOL
            if (schr == 0)
            {
              (*x1) = 0;
              break;
            }
            if (schr < 0)
            {
                if (-schr <= lpos)
                {
                    lpos += schr;
                    (*sp)++;
                } else
                // If we have more empty spaces than we want to skip
                {
                    // Return remaining part to skip, so that we can do it outside
                    return lpos;
                }
            } else
            //if (schr > 0)
            {
                if (schr <= lpos)
                // If we have less than we want to skip
                {
                    lpos -= schr;
                    (*sp) += (*(*sp)) + 1;
                } else
                // If we have more characters than we want to skip
                {
                    // Return remaining part to skip, so that we can draw it
                    return lpos;
                }
            }
        }
    }
    return 0;
}

void LbSpriteDrawLineSkipToEol(const char **sp, short *x1)
{
    char schr;
    if ((*x1) <= 0)
    {
      do {
        schr = *(*sp);
        while (schr > 0)
        {
          (*sp) += schr+1;
          schr = *(*sp);
        }
        (*sp)++;
      } while (schr);
    } else
    {
        (*sp)++;
    }
}

/** @internal
 *  Draw part of sprite line with transparency.
 *
 * @param buf_out
 * @param buf_inp
 * @param buf_len
 * @param mirror
 */
static inline void LbDrawBufferTranspr(unsigned char **buf_out, const char *buf_inp,
        const int buf_len, const TbBool mirror)
{
  int i;
  unsigned int val;
  if ( mirror )
  {
    if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
    {
        for (i=0; i<buf_len; i++ )
        {
            val = *(const unsigned char *)buf_inp;
            **buf_out = lbDisplay.GlassMap[(val<<8) + **buf_out];
            buf_inp++;
            (*buf_out)--;
        }
    } else
    {
        for (i=0; i<buf_len; i++ )
        {
            val = *(const unsigned char *)buf_inp;
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
            val = *(const unsigned char *)buf_inp;
            **buf_out = lbDisplay.GlassMap[(val<<8) + **buf_out];
            buf_inp++;
            (*buf_out)++;
        }
    } else
    {
        for (i=0; i<buf_len; i++ )
        {
            val = *(const unsigned char *)buf_inp;
            **buf_out = lbDisplay.GlassMap[((**buf_out)<<8) + val];
            buf_inp++;
            (*buf_out)++;
        }
    }
  }
}

/** @internal
 *  Draw part of sprite line as solid pixels.
 *  Draws by copying pixels from input buffer into output buffer, without any kind of blending
 *  or altering the color values. Palette for both buffers must be identical.
 *
 * @param buf_out
 * @param buf_inp
 * @param buf_len
 * @param mirror
 */
static inline void LbDrawBufferSolid(unsigned char **buf_out, const char *buf_inp,
        const int buf_len, const TbBool mirror)
{
    int i;
    if ( mirror )
    {
        for (i=0; i < buf_len; i++)
        {
            **buf_out = *(const unsigned char *)buf_inp;
            buf_inp++;
            (*buf_out)--;
        }
    } else
    {
        for (i=0; i < buf_len; i++)
        {
            **buf_out = *(const unsigned char *)buf_inp;
            buf_inp++;
            (*buf_out)++;
        }
    }
}

/** @internal
 *  Draw one line of a transparent sprite.
 *
 * @param sp
 * @param r
 * @param x1
 * @param lpos
 * @param mirror
 */
static inline void LbSpriteDrawLineTranspr(const char **sp, unsigned char **r, short *x1,
    short lpos, const TbBool mirror)
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
            LbDrawBufferTranspr(r,(*sp)+(lpos+1),drawOut,mirror);
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
            LbDrawBufferTranspr(r,(*sp)+1,drawOut,mirror);
            (*x1) -= schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 * Draw sprite data with transparency.
 */
static inline TbResult LbSpriteDrawTranspr(const char *sp, short sprWd, short sprHt, unsigned char *r,
    int nextRowDelta, short left, const TbBool mirror)
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
        LbSpriteDrawLineTranspr(&sp,&r,&x1,lpos,mirror);
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
 *  Draw one line of a solid sprite.
 *  Supports only mirrored sprites.
 *
 * @param sp
 * @param r
 * @param x1
 * @param lpos
 * @param mirror
 */
static inline void LbSpriteDrawLineSolid(const char **sp, unsigned char **r, short *x1, short lpos, const TbBool mirror)
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
            LbDrawBufferSolid(r,(*sp)+(lpos+1),drawOut,mirror);
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
            LbDrawBufferSolid(r,(*sp)+1,drawOut,mirror);
            (*x1) -= schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 *  Solid sprite drawing routine. Optimized for mirrored ones, without transparency.
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
static inline TbResult LbSpriteDrawSolid(const char *sp,short sprWd,short sprHt,unsigned char *r,
    int nextRowDelta,short left,const TbBool mirror)
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
        LbSpriteDrawLineSolid(&sp,&r,&x1,lpos,mirror);
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
 * Draw solid sprite line using optimized copy.
 */
static inline void LbSpriteDrawLineFastCpy(const char **sp, unsigned char **r, short *x1, short lpos)
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
            LbDrawBufferSolid(r, (*sp)+(lpos+1), drawOut, false);
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
            memcpy((*r), (*sp)+1, drawOut);
            (*x1) -= schr;
            (*r) += schr;
            (*sp) += (*(*sp)) + 1;
        }
    } //end while
}

/** @internal
 *  Fast copy sprite drawing routine. Does not support transparency nor mirroring.
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
static inline TbResult LbSpriteDrawFastCpy(const char *sp,short sprWd,short sprHt,unsigned char *r,
    int nextRowDelta,short left,const TbBool mirror)
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
        LbSpriteDrawLineFastCpy(&sp,&r,&x1,lpos);
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

TbResult LbSpriteDraw(long x, long y, const TbSprite *spr)
{
    TbSpriteDrawData spd;
    TbResult ret;

    LOGNO("at (%ld,%ld)", x, y);
    ret = LbSpriteDrawPrepare(&spd, x, y, spr);
    if (ret != Lb_SUCCESS)
        return ret;
    if ((lbDisplay.DrawFlags & (Lb_SPRITE_TRANSPAR4|Lb_SPRITE_TRANSPAR8)) != 0)
        return LbSpriteDrawTranspr(spd.sp,spd.Wd,spd.Ht,spd.r,spd.nextRowDelta,spd.startShift,spd.mirror);
    else
    if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
        return LbSpriteDrawSolid(spd.sp,spd.Wd,spd.Ht,spd.r,spd.nextRowDelta,spd.startShift,spd.mirror);
    else
        return LbSpriteDrawFastCpy(spd.sp,spd.Wd,spd.Ht,spd.r,spd.nextRowDelta,spd.startShift,spd.mirror);
}

/******************************************************************************/
