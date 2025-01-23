/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file app_spr_sstd.c
 *     Sprites drawing with scaling, application modification for panel.
 * @par Purpose:
 *     Drawing scaled sprites, with original colors and optional transparency.
 * @par Comment:
 *     Part of 8-bit graphics canvas drawing library.
 *     Note: Lines modified from original (other than function names) are
 *     marked by comments.
 * @author   Tomasz Lis
 * @date     12 Feb 2008 - 21 Jan 2025
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

#include "display.h"

void SCANNER_init_palette_bright(void)
{
    ubyte *pal;
    int col;
    ubyte col_r, col_g, col_b;
    ubyte col_C;

    pal = display_palette;
    for (col = 0; col < PALETTE_8b_COLORS; col++)
    {
        col_r = pal[col * 3 + 0];
        col_g = pal[col * 3 + 1];
        col_b = pal[col * 3 + 2];

        if ((col_r > col_g) && (col_r > col_b))
            col_C = col_r;
        else if (col_g > col_b)
            col_C = col_g;
        else
            col_C = col_b;

        SCANNER_pal_bright[col] = (col_C + ((col_b + col_r + col_g) >> 1)) / 3;
    }
}

void SCANNER_init_bright_limit_table(void)
{
    int i;

    for (i = 0; i < 64; i++)
        SCANNER_bright_limit[i] = i;
    for (; i < 256; i++)
        SCANNER_bright_limit[i] = 63;
}


/******************************************************************************/
void LbPixelBlockCopyForward(TbPixel * dst, const TbPixel * src, long len);
// The functions below are from colour remap version of the routine - they're used for shadows of sprites
TbResult LbSpriteDrawRemapUsingScalingUpDataSolidRL(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingUpDataSolidLR(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingDownDataSolidRL(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);
TbResult LbSpriteDrawRemapUsingScalingDownDataSolidLR(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite, const TbPixel *cmap);

TbResult LbSpriteDrawUsingScalingUpDataTrans2RL(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);
TbResult LbSpriteDrawUsingScalingUpDataTrans2LR(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);
TbResult LbSpriteDrawUsingScalingUpDataSolidRL(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite);
TbResult LbSpriteDrawUsingScalingUpDataSolidLR(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite);
TbResult LbSpriteDrawUsingScalingDownDataTrans2RL(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);
TbResult LbSpriteDrawUsingScalingDownDataTrans2LR(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);
TbResult LbSpriteDrawUsingScalingDownDataSolidRL(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite);
TbResult LbSpriteDrawUsingScalingDownDataSolidLR(ubyte *outbuf, int scanline,
  int outheight, long *xstep, long *ystep, const TbSprite *sprite);
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
TbResult ApSpriteDrawUsingScalingUpDataTrans1RL(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap)
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
                                unsigned int pxmap, pxbase;
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
TbResult ApSpriteDrawUsingScalingUpDataTrans1LR(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap)
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
                                unsigned int pxmap, pxbase;
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
TbResult ApSpriteDrawUsingScalingDownDataTrans1RL(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap)
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
                            unsigned int pxmap, pxbase;
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
TbResult ApSpriteDrawUsingScalingDownDataTrans1LR(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap)
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
                            unsigned int pxmap, pxbase;
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

TbResult ApSpriteDrawUsingScalingDataModRecolorTrans(long posx, long posy, const TbSprite *sprite)
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
    ubyte *outbuf;
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
              return LbSpriteDrawRemapUsingScalingUpDataSolidRL(outbuf, scanline,
               outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
          else
          {
              return LbSpriteDrawRemapUsingScalingUpDataSolidLR(outbuf, scanline,
               outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
        {
          assert(render_ghost != NULL);
          // App-specific code starts
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return ApSpriteDrawUsingScalingUpDataTrans1RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, pixmap.fade_table);
          }
          else
          {
              return ApSpriteDrawUsingScalingUpDataTrans1LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, pixmap.fade_table);
          }
          // App-specific code ends
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
        {
          assert(render_ghost != NULL);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingUpDataTrans2RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, render_ghost);
          }
          else
          {
              return LbSpriteDrawUsingScalingUpDataTrans2LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, render_ghost);
          }
        }
        else
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingUpDataSolidRL(outbuf, scanline,
               outheight, xstep, ystep, sprite);
          }
          else
          {
              return LbSpriteDrawUsingScalingUpDataSolidLR(outbuf, scanline,
               outheight, xstep, ystep, sprite);
          }
        }
    }
    else
    {
        if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLNSHADOW) != 0)
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawRemapUsingScalingDownDataSolidRL(outbuf, scanline,
               outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
          else
          {
              return LbSpriteDrawRemapUsingScalingDownDataSolidLR(outbuf, scanline,
               outheight, xstep, ystep, sprite, lbSpriteReMapPtr);
          }
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
        {
          assert(render_ghost != NULL);
          // App-specific code starts
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return ApSpriteDrawUsingScalingDownDataTrans1RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, pixmap.fade_table);
          }
          else
          {
              return ApSpriteDrawUsingScalingDownDataTrans1LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, pixmap.fade_table);
          }
          // App-specific code ends
        }
        else
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
        {
          assert(render_ghost != NULL);
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingDownDataTrans2RL(outbuf, scanline,
               outheight, xstep, ystep, sprite, render_ghost);
          }
          else
          {
              return LbSpriteDrawUsingScalingDownDataTrans2LR(outbuf, scanline,
               outheight, xstep, ystep, sprite, render_ghost);
          }
        }
        else
        {
          if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
          {
              return LbSpriteDrawUsingScalingDownDataSolidRL(outbuf, scanline,
               outheight, xstep, ystep, sprite);
          }
          else
          {
              return LbSpriteDrawUsingScalingDownDataSolidLR(outbuf, scanline,
               outheight, xstep, ystep, sprite);
          }
        }
    }
}

TbResult ApSpriteDrawScaledModRecolorTrans(long xpos, long ypos, const TbSprite *sprite,
    long dest_width, long dest_height)
{
    LOGNO("at (%ld,%ld) size (%ld,%ld)",xpos,ypos,dest_width,dest_height);
    if ((dest_width <= 0) || (dest_height <= 0))
      return 1;
    if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLNSHADOW) != 0)
        lbSpriteReMapPtr = lbDisplay.FadeTable + ((lbDisplay.FadeStep & 0x3F) << 8);
    LbSpriteSetScalingData(xpos, ypos, sprite->SWidth, sprite->SHeight, dest_width, dest_height);
    return ApSpriteDrawUsingScalingDataModRecolorTrans(0, 0, sprite);
}

/******************************************************************************/
