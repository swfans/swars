/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file spr_sstd.c
 *     Sprites drawing with scaling, using their standard colors.
 * @par Purpose:
 *     Drawing scaled sprites, with original colors and optional transparency.
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

#include "bfscreen.h"
#include "bfutility.h"
#include "bflog.h"

#define SPRITE_SCALING_XSTEPS max(MAX_SUPPORTED_SPRITE_DIM,MAX_SUPPORTED_SCREEN_WIDTH)
#define SPRITE_SCALING_YSTEPS max(MAX_SUPPORTED_SPRITE_DIM,MAX_SUPPORTED_SCREEN_HEIGHT)

/******************************************************************************/
extern long xsteps_array[2*SPRITE_SCALING_XSTEPS];
extern long ysteps_array[2*SPRITE_SCALING_YSTEPS];
extern long alpha_xsteps_array[2*SPRITE_SCALING_XSTEPS];
extern long alpha_ysteps_array[2*SPRITE_SCALING_YSTEPS];
extern UBYTE *render_ghost;
extern long scale_up;
/******************************************************************************/
void LbPixelBlockCopyForward(TbPixel * dst, const TbPixel * src, long len);
// The functions below are from colour remap version of the routine - they're used for shadows of sprites
TbResult LbSpriteDrawRemapUsingScalingUpDataSolidRL(UBYTE *outbuf, int scanline, int outheight, long *xstep, long *ystep, const struct TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingUpDataSolidLR(UBYTE *outbuf, int scanline, int outheight, long *xstep, long *ystep, const struct TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingDownDataSolidRL(UBYTE *outbuf, int scanline, int outheight, long *xstep, long *ystep, const struct TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingDownDataSolidLR(UBYTE *outbuf, int scanline, int outheight, long *xstep, long *ystep, const struct TbSprite *sprite, const TbPixel *cmap);
/******************************************************************************/

/**
 * Draws a scaled up sprite on given buffer, with transparency mapping, from right to left.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingUpDataTrans1RL(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            unsigned char *prevdata;
            int xdup, ydup;
            long *xcurstep;
            ydup = ycurstep[1];
            if (ycurstep[0]+ydup > outheight)
                ydup = outheight-ycurstep[0];
            prevdata = sprdata;
            while (ydup > 0)
            {
                sprdata = prevdata;
                xcurstep = xstep;
                TbPixel *out_end;
                out_end = outbuf;
                while ( 1 )
                {
                    long pxlen;
                    pxlen = (signed char)*sprdata;
                    sprdata++;
                    if (pxlen == 0)
                        break;
                    if (pxlen < 0)
                    {
                        pxlen = -pxlen;
                        out_end -= xcurstep[0] + xcurstep[1];
                        xcurstep -= 2 * pxlen;
                        out_end += xcurstep[0] + xcurstep[1];
                    }
                    else
                    {
                        for (;pxlen > 0; pxlen--)
                        {
                            xdup = xcurstep[1];
                            if (xcurstep[0]+xdup > abs(scanline))
                                xdup = abs(scanline)-xcurstep[0];
                            if (xdup > 0)
                            {
                                unsigned int pxmap;
                                pxmap = ((*sprdata) << 8);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = (pxmap & ~0x00ff) | ((*out_end));
                                    *out_end = transmap[pxmap];
                                    out_end--;
                                }
                            }
                            sprdata++;
                            xcurstep -= 2;
                        }
                    }
                }
                outbuf += scanline;
                ydup--;
            }
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled up sprite on given buffer, with transparency mapping, from left to right.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used. Should have
 *     a size of 256x256 to avoid invalid memory reads.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingUpDataTrans1LR(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            unsigned char *prevdata;
            int xdup, ydup;
            long *xcurstep;
            ydup = ycurstep[1];
            if (ycurstep[0]+ydup > outheight)
                ydup = outheight-ycurstep[0];
            prevdata = sprdata;
            while (ydup > 0)
            {
                sprdata = prevdata;
                xcurstep = xstep;
                TbPixel *out_end;
                out_end = outbuf;
                while ( 1 )
                {
                    long pxlen;
                    pxlen = (signed char)*sprdata;
                    sprdata++;
                    if (pxlen == 0)
                        break;
                    if (pxlen < 0)
                    {
                        pxlen = -pxlen;
                        out_end -= xcurstep[0];
                        xcurstep += 2 * pxlen;
                        out_end += xcurstep[0];
                    }
                    else
                    {
                        for (;pxlen > 0; pxlen--)
                        {
                            xdup = xcurstep[1];
                            if (xcurstep[0]+xdup > abs(scanline))
                                xdup = abs(scanline)-xcurstep[0];
                            if (xdup > 0)
                            {
                                unsigned int pxmap;
                                pxmap = ((*sprdata) << 8);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = (pxmap & ~0x00ff) | ((*out_end));
                                    *out_end = transmap[pxmap];
                                    out_end++;
                                }
                            }
                            sprdata++;
                            xcurstep += 2;
                        }
                    }
                }
                outbuf += scanline;
                ydup--;
            }
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled up sprite on given buffer, with reversed transparency mapping, from right to left.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingUpDataTrans2RL(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            unsigned char *prevdata;
            int xdup, ydup;
            long *xcurstep;
            ydup = ycurstep[1];
            if (ycurstep[0]+ydup > outheight)
                ydup = outheight-ycurstep[0];
            prevdata = sprdata;
            while (ydup > 0)
            {
                sprdata = prevdata;
                xcurstep = xstep;
                TbPixel *out_end;
                out_end = outbuf;
                while ( 1 )
                {
                    long pxlen;
                    pxlen = (signed char)*sprdata;
                    sprdata++;
                    if (pxlen == 0)
                        break;
                    if (pxlen < 0)
                    {
                        pxlen = -pxlen;
                        out_end -= xcurstep[0] + xcurstep[1];
                        xcurstep -= 2 * pxlen;
                        out_end += xcurstep[0] + xcurstep[1];
                    }
                    else
                    {
                        for (;pxlen > 0; pxlen--)
                        {
                            xdup = xcurstep[1];
                            if (xcurstep[0]+xdup > abs(scanline))
                                xdup = abs(scanline)-xcurstep[0];
                            if (xdup > 0)
                            {
                                unsigned int pxmap;
                                pxmap = (*sprdata);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = (pxmap & ~0xff00) | ((*out_end) << 8);
                                    *out_end = transmap[pxmap];
                                    out_end--;
                                }
                            }
                            sprdata++;
                            xcurstep -= 2;
                        }
                    }
                }
                outbuf += scanline;
                ydup--;
            }
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled up sprite on given buffer, with reversed transparency mapping, from left to right.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingUpDataTrans2LR(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            unsigned char *prevdata;
            int xdup, ydup;
            long *xcurstep;
            ydup = ycurstep[1];
            if (ycurstep[0]+ydup > outheight)
                ydup = outheight-ycurstep[0];
            prevdata = sprdata;
            while (ydup > 0)
            {
                sprdata = prevdata;
                xcurstep = xstep;
                TbPixel *out_end;
                out_end = outbuf;
                while ( 1 )
                {
                    long pxlen;
                    pxlen = (signed char)*sprdata;
                    sprdata++;
                    if (pxlen == 0)
                        break;
                    if (pxlen < 0)
                    {
                        pxlen = -pxlen;
                        out_end -= xcurstep[0];
                        xcurstep += 2 * pxlen;
                        out_end += xcurstep[0];
                    }
                    else
                    {
                        for (;pxlen > 0; pxlen--)
                        {
                            xdup = xcurstep[1];
                            if (xcurstep[0]+xdup > abs(scanline))
                                xdup = abs(scanline)-xcurstep[0];
                            if (xdup > 0)
                            {
                                unsigned int pxmap;
                                pxmap = (*sprdata);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = (pxmap & ~0xff00) | ((*out_end) << 8);
                                    *out_end = transmap[pxmap];
                                    out_end++;
                                }
                            }
                            sprdata++;
                            xcurstep += 2;
                        }
                    }
                }
                outbuf += scanline;
                ydup--;
            }
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled up sprite on given buffer, with original colours, from right to left.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingUpDataSolidRL(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            int ycur;
            int solid_len;
            TbPixel * out_line;
            int xdup, ydup;
            long *xcurstep;
            ydup = ycurstep[1];
            if (ycurstep[0]+ydup > outheight)
                ydup = outheight-ycurstep[0];
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0] + xcurstep[1];
                    xcurstep -= 2 * pxlen;
                    out_end += xcurstep[0] + xcurstep[1];
                }
                else
                {
                    TbPixel *out_start;
                    out_start = out_end;
                    for(;pxlen > 0; pxlen--)
                    {
                        xdup = xcurstep[1];
                        if (xcurstep[0]+xdup > abs(scanline))
                            xdup = abs(scanline)-xcurstep[0];
                        if (xdup > 0)
                        {
                            unsigned char pxval;
                            pxval = *sprdata;
                            for (;xdup > 0; xdup--)
                            {
                                *out_end = pxval;
                                out_end--;
                            }
                        }
                        sprdata++;
                        xcurstep -= 2;
                    }
                    ycur = ydup - 1;
                    if (ycur > 0)
                    {
                        solid_len = out_start - out_end;
                        out_start = out_end;
                        solid_len++;
                        out_line = out_start + scanline;
                        for (;ycur > 0; ycur--)
                        {
                            if (solid_len > 0) {
                                LbPixelBlockCopyForward(out_line, out_start, solid_len);
                            }
                            out_line += scanline;
                        }
                    }
                }
            }
            outbuf += scanline;
            ycur = ydup - 1;
            for (;ycur > 0; ycur--)
            {
                outbuf += scanline;
            }
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled up sprite on given buffer, with original colours, from left to right.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param outheight
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingUpDataSolidLR(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            int ycur;
            int solid_len;
            TbPixel * out_line;
            int xdup, ydup;
            long *xcurstep;
            ydup = ycurstep[1];
            if (ycurstep[0]+ydup > outheight)
                ydup = outheight-ycurstep[0];
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0];
                    xcurstep += 2 * pxlen;
                    out_end += xcurstep[0];
                }
                else
                {
                    TbPixel *out_start;
                    out_start = out_end;
                    for(;pxlen > 0; pxlen--)
                    {
                        xdup = xcurstep[1];
                        if (xcurstep[0]+xdup > abs(scanline))
                            xdup = abs(scanline)-xcurstep[0];
                        if (xdup > 0)
                        {
                            unsigned char pxval;
                            pxval = *sprdata;
                            for (;xdup > 0; xdup--)
                            {
                                *out_end = pxval;
                                out_end++;
                            }
                        }
                        sprdata++;
                        xcurstep += 2;
                    }
                    ycur = ydup - 1;
                    if (ycur > 0)
                    {
                        solid_len = out_end - out_start;
                        out_line = out_start + scanline;
                        for (;ycur > 0; ycur--)
                        {
                            if (solid_len > 0) {
                                LbPixelBlockCopyForward(out_line, out_start, solid_len);
                            }
                            out_line += scanline;
                        }
                    }
                }
            }
            outbuf += scanline;
            ycur = ydup - 1;
            for (;ycur > 0; ycur--)
            {
                outbuf += scanline;
            }
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled down sprite on given buffer, with transparency mapping, from right to left.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingDownDataTrans1RL(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            long *xcurstep;
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0] + xcurstep[1];
                    xcurstep -= 2 * pxlen;
                    out_end += xcurstep[0] + xcurstep[1];
                }
                else
                {
                    for (;pxlen > 0; pxlen--)
                    {
                        if (xcurstep[1] > 0)
                        {
                            unsigned int pxmap;
                            pxmap = ((*sprdata) << 8);
                            {
                                pxmap = (pxmap & ~0x00ff) | ((*out_end));
                                *out_end = transmap[pxmap];
                                out_end--;
                            }
                        }
                        sprdata++;
                        xcurstep -= 2;
                    }
                }
            }
            outbuf += scanline;
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled down sprite on given buffer, with transparency mapping, from left to right.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingDownDataTrans1LR(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            long *xcurstep;
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0];
                    xcurstep += 2 * pxlen;
                    out_end += xcurstep[0];
                }
                else
                {
                    for (;pxlen > 0; pxlen--)
                    {
                        if (xcurstep[1] > 0)
                        {
                            unsigned int pxmap;
                            pxmap = ((*sprdata) << 8);
                            {
                                pxmap = (pxmap & ~0x00ff) | ((*out_end));
                                *out_end = transmap[pxmap];
                                out_end++;
                            }
                        }
                        sprdata++;
                        xcurstep += 2;
                    }
                }
            }
            outbuf += scanline;
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled down sprite on given buffer, with reverse transparency mapping, from right to left.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingDownDataTrans2RL(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            long *xcurstep;
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0] + xcurstep[1];
                    xcurstep -= 2 * pxlen;
                    out_end += xcurstep[0] + xcurstep[1];
                }
                else
                {
                    for (;pxlen > 0; pxlen--)
                    {
                        if (xcurstep[1] > 0)
                        {
                            unsigned int pxmap;
                            pxmap = ((*sprdata));
                            {
                                pxmap = (pxmap & ~0xff00) | ((*out_end) << 8);
                                *out_end = transmap[pxmap];
                                out_end--;
                            }
                        }
                        sprdata++;
                        xcurstep -= 2;
                    }
                }
            }
            outbuf += scanline;
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled down sprite on given buffer, with reverse transparency mapping, from left to right.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingDownDataTrans2LR(UBYTE *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const struct TbSprite *sprite, const unsigned char *transmap)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            long *xcurstep;
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0];
                    xcurstep += 2 * pxlen;
                    out_end += xcurstep[0];
                }
                else
                {
                    for (;pxlen > 0; pxlen--)
                    {
                        if (xcurstep[1] > 0)
                        {
                            unsigned int pxmap;
                            pxmap = ((*sprdata));
                            {
                                pxmap = (pxmap & ~0xff00) | ((*out_end) << 8);
                                *out_end = transmap[pxmap];
                                out_end++;
                            }
                        }
                        sprdata++;
                        xcurstep += 2;
                    }
                }
            }
            outbuf += scanline;
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled down sprite on given buffer, with original colours, from right to left.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingDownDataSolidRL(UBYTE *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const struct TbSprite *sprite)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            long *xcurstep;
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0] + xcurstep[1];
                    xcurstep -= 2 * pxlen;
                    out_end += xcurstep[0] + xcurstep[1];
                }
                else
                {
                    for (;pxlen > 0; pxlen--)
                    {
                        if (xcurstep[1] > 0)
                        {
                            unsigned char pxval;
                            pxval = *sprdata;
                            {
                                *out_end = pxval;
                                out_end--;
                            }
                        }
                        sprdata++;
                        xcurstep -= 2;
                    }
                }
            }
            outbuf += scanline;
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled down sprite on given buffer, with original colours, from left to right.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 */
TbResult LbSpriteDrawUsingScalingDownDataSolidLR(UBYTE *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const struct TbSprite *sprite)
{
    LIBLOG("Drawing");
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    ystep_delta = 2;
    if (scanline < 0) {
        ystep_delta = -2;
    }
    sprdata = sprite->Data;
    ycurstep = ystep;

    int h;
    for (h=sprite->SHeight; h > 0; h--)
    {
        if (ycurstep[1] != 0)
        {
            long *xcurstep;
            xcurstep = xstep;
            TbPixel *out_end;
            out_end = outbuf;
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                    break;
                if (pxlen < 0)
                {
                    pxlen = -pxlen;
                    out_end -= xcurstep[0];
                    xcurstep += 2 * pxlen;
                    out_end += xcurstep[0];
                }
                else
                {
                    for (;pxlen > 0; pxlen--)
                    {
                        if (xcurstep[1] > 0)
                        {
                            unsigned char pxval;
                            pxval = *sprdata;
                            {
                                *out_end = pxval;
                                out_end++;
                            }
                        }
                        sprdata++;
                        xcurstep += 2;
                    }
                }
            }
            outbuf += scanline;
        }
        else
        {
            while ( 1 )
            {
                long pxlen;
                pxlen = (signed char)*sprdata;
                sprdata++;
                if (pxlen == 0)
                  break;
                if (pxlen > 0)
                {
                    sprdata += pxlen;
                }
            }
        }
        ycurstep += ystep_delta;
    }
    return 0;
}

/**
 * Draws a scaled sprite on current graphics window at given position.
 * Requires LbSpriteSetScalingData() to be called before.
 *
 * @param posx The X coord within current graphics window.
 * @param posy The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 * @see LbSpriteSetScalingData()
 */
TbResult LbSpriteDrawUsingScalingData(long posx, long posy, const struct TbSprite *sprite)
{
    LIBLOG("Drawing at (%ld,%ld)",posx,posy);
    long *xstep;
    long *ystep;
    int scanline;
    {
        long sposx, sposy;
        sposx = posx;
        sposy = posy;
        scanline = lbDisplay.GraphicsScreenWidth;
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0) {
            sposx = sprite->SWidth + posx - 1;
        }
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_VERTIC) != 0) {
            sposy = sprite->SHeight + posy - 1;
            scanline = -lbDisplay.GraphicsScreenWidth;
        }
        xstep = &xsteps_array[2 * sposx];
        ystep = &ysteps_array[2 * sposy];
    }
    UBYTE *outbuf;
    int outheight;
    {
        int gspos_x,gspos_y;
        gspos_y = ystep[0];
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_VERTIC) != 0)
            gspos_y += ystep[1] - 1;
        gspos_x = xstep[0];
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
            gspos_x += xstep[1] - 1;
        outbuf = &lbDisplay.GraphicsWindowPtr[gspos_x + lbDisplay.GraphicsScreenWidth * gspos_y];
        outheight = lbDisplay.GraphicsScreenHeight;
    }
    if ( scale_up )
    {
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLNSHADOW) != 0)
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawRemapUsingScalingUpDataSolidRL(outbuf, scanline, outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
          else
          {
              return LbSpriteDrawRemapUsingScalingUpDataSolidLR(outbuf, scanline, outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingUpDataTrans1RL(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
          else
          {
              return LbSpriteDrawUsingScalingUpDataTrans1LR(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingUpDataTrans2RL(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
          else
          {
              return LbSpriteDrawUsingScalingUpDataTrans2LR(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
        }
        else
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingUpDataSolidRL(outbuf, scanline, outheight, xstep, ystep, sprite);
          }
          else
          {
              return LbSpriteDrawUsingScalingUpDataSolidLR(outbuf, scanline, outheight, xstep, ystep, sprite);
          }
        }
    }
    else
    {
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLNSHADOW) != 0)
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawRemapUsingScalingDownDataSolidRL(outbuf, scanline, outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
          else
          {
              return LbSpriteDrawRemapUsingScalingDownDataSolidLR(outbuf, scanline, outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingDownDataTrans1RL(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
          else
          {
              return LbSpriteDrawUsingScalingDownDataTrans1LR(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingDownDataTrans2RL(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
          else
          {
              return LbSpriteDrawUsingScalingDownDataTrans2LR(outbuf, scanline, outheight, xstep, ystep, sprite, render_ghost);
          }
        }
        else
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingDownDataSolidRL(outbuf, scanline, outheight, xstep, ystep, sprite);
          }
          else
          {
              return LbSpriteDrawUsingScalingDownDataSolidLR(outbuf, scanline, outheight, xstep, ystep, sprite);
          }
        }
    }
}

TbResult LbSpriteDrawScaled(long xpos, long ypos, const struct TbSprite *sprite, long dest_width, long dest_height)
{
    LIBLOG("At (%ld,%ld) size (%ld,%ld)",xpos,ypos,dest_width,dest_height);
    if ((dest_width <= 0) || (dest_height <= 0))
      return 1;
    if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLNSHADOW) != 0)
        lbSpriteReMapPtr = lbDisplay.FadeTable + ((lbDisplay.FadeStep & 0x3F) << 8);
    LbSpriteSetScalingData(xpos, ypos, sprite->SWidth, sprite->SHeight, dest_width, dest_height);
    return LbSpriteDrawUsingScalingData(0, 0, sprite);
}

/******************************************************************************/
