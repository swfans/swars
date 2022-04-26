/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file spr_scol.c
 *     Sprites drawing with scaling, as one color areas.
 * @par Purpose:
 *     Allows drawing rescaled sprites, using selected color rather than their
 *     original color.
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
#include "bflog.h"

TbResult LbSpriteDrawScaledOneColour(long xpos, long ypos, const struct TbSprite *sprite, long dest_width, long dest_height, const TbPixel colour)
{
    LIBLOG("At (%ld,%ld) size (%ld,%ld)",xpos,ypos,dest_width,dest_height);
    if ((dest_width <= 0) || (dest_height <= 0))
      return 1;
    if ((lbDisplay.DrawFlags & Lb_TEXT_UNDERLNSHADOW) != 0)
        lbSpriteReMapPtr = lbDisplay.FadeTable + ((lbDisplay.FadeStep & 0x3F) << 8);
    LbSpriteSetScalingData(xpos, ypos, sprite->SWidth, sprite->SHeight, dest_width, dest_height);
    return LbSpriteDrawOneColourUsingScalingData(0, 0, sprite, colour);
}

/******************************************************************************/
