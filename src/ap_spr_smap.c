/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ap_spr_smap.c
 *     Sprites drawing with scaling and remapping, app mod for panel.
 * @par Purpose:
 *     Allows drawing scaled sprites, using given array to remap sprite colors.
 * @par Comment:
 *     Part of 8-bit graphics canvas drawing library.
 *     Note: Lines modified from original (other than function names) are
 *     marked by comments. The rest of the code is a copy from `spr_smap.c`.
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

#include <stdlib.h>
#include <assert.h>
#include "bfgentab.h"
#include "bfsprite.h"
#include "insspr.h"
#include "bfscreen.h"
#include "privbflog.h"

TbResult LbSpriteDrawRemapUsingScalingUpDataSolidRL(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingUpDataSolidLR(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingDownDataSolidRL(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingDownDataSolidLR(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);

/**
 * Draws a scaled up sprite on given buffer, with transparency mapping and
 * source colours remapped, from right to left.
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
TbResult ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans1RL(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                                // App-specific code starts
                                unsigned int pxmap;
                                int pxbase;
                                pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                    pxmap = (SCANNER_bright_limit[pxmap] << 8) | ((*sprdata));
                                    *out_end = transmap[pxmap];
                                    out_end--;
                                }
                                // App-specific code ends
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
 * Draws a scaled up sprite on given buffer, with transparency mapping and source colours remapped, from left to right.
 * Requires step arrays for scaling.
 *
 * @param outbuf The output buffer.
 * @param scanline Length of the output buffer scanline.
 * @param xstep Scaling steps array, x dimension.
 * @param ystep Scaling steps array, y dimension.
 * @param sprite The source sprite.
 * @param transmap The transparency mapping table to be used. Should have a size of 256x256 to avoid invalid memory reads.
 * @return Gives 0 on success.
 */
TbResult ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans1LR(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                                // App-specific code starts
                                unsigned int pxmap;
                                int pxbase;
                                pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                    pxmap = (SCANNER_bright_limit[pxmap] << 8) | ((*sprdata));
                                    *out_end = transmap[pxmap];
                                    out_end++;
                                }
                                // App-specific code ends
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
 * Draws a scaled up sprite on given buffer, with reversed transparency mapping and source colours remapped, from right to left.
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
TbResult ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans2RL(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                                // App-specific code starts
                                unsigned int pxmap;
                                int pxbase;
                                pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                    pxmap = (SCANNER_bright_limit[pxmap]) | ((*sprdata) << 8);
                                    *out_end = transmap[pxmap];
                                    out_end--;
                                }
                                // App-specific code ends
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
 * Draws a scaled up sprite on given buffer, with reversed transparency mapping and source colours remapped, from left to right.
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
TbResult ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans2LR(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                                // App-specific code starts
                                unsigned int pxmap;
                                int pxbase;
                                pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                                for (;xdup > 0; xdup--)
                                {
                                    pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                    pxmap = (SCANNER_bright_limit[pxmap]) | ((*sprdata) << 8);
                                    *out_end = transmap[pxmap];
                                    out_end++;
                                }
                                // App-specific code ends
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
 * Draws a scaled down sprite on given buffer, with transparency mapping and source colours remapped, from right to left.
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
TbResult ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans1RL(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                            // App-specific code starts
                            unsigned int pxmap;
                            int pxbase;
                            pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                            {
                                pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                pxmap = (SCANNER_bright_limit[pxmap] << 8) | ((*sprdata));
                                *out_end = transmap[pxmap];
                                out_end--;
                            }
                            // App-specific code ends
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
 * Draws a scaled down sprite on given buffer, with transparency mapping and source colours remapped, from left to right.
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
TbResult ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans1LR(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                            // App-specific code starts
                            unsigned int pxmap;
                            int pxbase;
                            pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                            {
                                pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                pxmap = (SCANNER_bright_limit[pxmap] << 8) | ((*sprdata));
                                *out_end = transmap[pxmap];
                                out_end++;
                            }
                            // App-specific code ends
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
 * Draws a scaled down sprite on given buffer, with reverse transparency mapping and source colours remapped, from right to left.
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
TbResult ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans2RL(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                            // App-specific code starts
                            unsigned int pxmap;
                            int pxbase;
                            pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                            {
                                pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                pxmap = (SCANNER_bright_limit[pxmap]) | ((*sprdata) << 8);
                                *out_end = transmap[pxmap];
                                out_end--;
                            }
                            // App-specific code ends
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
 * Draws a scaled down sprite on given buffer, with reverse transparency mapping and source colours remapped, from left to right.
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
TbResult ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans2LR(uchar *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite,
  const TbPixel *transmap)
{
    int ystep_delta;
    unsigned char *sprdata;
    long *ycurstep;

    LOGDBG("drawing");
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
                            // App-specific code starts
                            unsigned int pxmap;
                            int pxbase;
                            pxbase = dword_1DC36C + (SCANNER_pal_bright[*sprdata] >> 1);
                            {
                                pxmap = pxbase + (SCANNER_pal_bright[*out_end] >> 1);
                                pxmap = (SCANNER_bright_limit[pxmap]) | ((*sprdata) << 8);
                                *out_end = transmap[pxmap];
                                out_end++;
                            }
                            // App-specific code ends
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
 * Draws a scaled sprite with remapped colours on current graphics window at given position.
 * Requires LbSpriteSetScalingData() to be called before.
 *
 * @param posx The X coord within current graphics window.
 * @param posy The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @param transmap Colour mixint/transparency array.
 * @return Gives 0 on success.
 * @see LbSpriteSetScalingData()
 */
TbResult ApSpriteDrawLowTransGreyRemapUsingScalingData(long posx, long posy, const TbSprite *sprite,
  const TbPixel *transmap)
{
    long *xstep;
    long *ystep;
    int scanline;

    LOGDBG("at (%ld,%ld): drawing", posx, posy);
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
    uchar *outbuf;
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
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
        {
          assert(render_ghost != NULL);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans1RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
          else
          {
              return ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans1LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
        {
          assert(transmap != NULL);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans2RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
          else
          {
              return ApSpriteDrawLTGreyRemapUsingScalingUpDataTrans2LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
        }
        else
        {
          ubyte bri;
          bri = (dword_1DC36C <= 8) ? (24 + dword_1DC36C) : (32 + (dword_1DC36C - 8) / 2);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawRemapUsingScalingUpDataSolidRL(outbuf, scanline,
               outheight, xstep, ystep, sprite, &transmap[bri * PALETTE_8b_COLORS]);
          }
          else
          {
              return LbSpriteDrawRemapUsingScalingUpDataSolidLR(outbuf, scanline,
               outheight, xstep, ystep, sprite, &transmap[bri * PALETTE_8b_COLORS]);
          }
        }
    }
    else
    {
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
        {
          assert(transmap != NULL);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans1RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
          else
          {
              return ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans1LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
        {
          assert(transmap != NULL);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans2RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
          else
          {
              return ApSpriteDrawLTGreyRemapUsingScalingDownDataTrans2LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, transmap);
          }
        }
        else
        {
          ubyte bri;
          bri = (dword_1DC36C <= 8) ? (24 + dword_1DC36C) : (32 + (dword_1DC36C - 8) / 2);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawRemapUsingScalingDownDataSolidRL(outbuf, scanline,
               outheight, xstep, ystep, sprite, &transmap[bri * PALETTE_8b_COLORS]);
          }
          else
          {
              return LbSpriteDrawRemapUsingScalingDownDataSolidLR(outbuf, scanline,
               outheight, xstep, ystep, sprite, &transmap[bri * PALETTE_8b_COLORS]);
          }
        }
    }
}

TbResult ApSpriteDrawScaledLowTransGreyRemap(long xpos, long ypos, const TbSprite *sprite,
    long dest_width, long dest_height, const ubyte *transmap)
{
    LOGNO("at (%ld,%ld) size (%ld,%ld)", xpos, ypos, dest_width, dest_height);
    if ((dest_width <= 0) || (dest_height <= 0))
      return 1;
    if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLNSHADOW) != 0)
        lbSpriteReMapPtr = lbDisplay.FadeTable + ((lbDisplay.FadeStep & 0x3F) << 8);
    LbSpriteSetScalingData(xpos, ypos, sprite->SWidth, sprite->SHeight, dest_width, dest_height);
    return ApSpriteDrawLowTransGreyRemapUsingScalingData(0, 0, sprite, transmap);
}

/******************************************************************************/
