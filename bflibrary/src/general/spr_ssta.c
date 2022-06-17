/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file spr_ssta.c
 *     Sprites drawing with scaling, using their standard colors, but forcing alpha.
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

#include <stdlib.h>
#include <assert.h>
#include "insspr.h"
#include "bfscreen.h"
#include "privbflog.h"

// The functions below are from standard version of the routine - reused here
TbResult LbSpriteDrawUsingScalingUpDataTrans1RL(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);
TbResult LbSpriteDrawUsingScalingUpDataTrans1LR(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);
TbResult LbSpriteDrawUsingScalingDownDataTrans1RL(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);
TbResult LbSpriteDrawUsingScalingDownDataTrans1LR(ubyte *outbuf, int scanline, int outheight,
  long *xstep, long *ystep, const TbSprite *sprite, const unsigned char *transmap);


TbResult DrawAlphaSpriteUsingScalingData(long posx, long posy, const TbSprite *sprite)
{
    long *xstep;
    long *ystep;
    int scanline;

    LOGNO("at (%ld,%ld): drawing", posx, posy);
    //TODO set this in higher level function instead, when possible
    render_alpha = lbSpriteReMapPtr;
    assert(render_alpha != NULL);
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
        xstep = &alpha_xsteps_array[2 * sposx];
        ystep = &alpha_ysteps_array[2 * sposy];
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
    if ( alpha_scale_up )
    {
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
        {
            return LbSpriteDrawUsingScalingUpDataTrans1RL(outbuf, scanline, outheight,
              xstep, ystep, sprite, render_alpha);
        }
        else
        {
            return LbSpriteDrawUsingScalingUpDataTrans1LR(outbuf, scanline, outheight,
              xstep, ystep, sprite, render_alpha);
        }
    }
    else
    {
        if ((lbDisplay.DrawFlags & Lb_SPRITE_FLIP_HORIZ) != 0)
        {
            return LbSpriteDrawUsingScalingDownDataTrans1RL(outbuf, scanline, outheight,
              xstep, ystep, sprite, render_alpha);
        }
        else
        {
            return LbSpriteDrawUsingScalingDownDataTrans1LR(outbuf, scanline, outheight,
              xstep, ystep, sprite, render_alpha);
        }
    }
}
/******************************************************************************/
