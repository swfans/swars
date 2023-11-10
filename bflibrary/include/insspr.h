/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file insspr.h
 *     Internal header for shares between various sprite drawing functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_INSPRITE_H_
#define BFLIBRARY_INSPRITE_H_

#include "bftypes.h"
#include "bfsprite.h"
#include "bfscreen.h"
#include "bfutility.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPRITE_SCALING_XSTEPS max(MAX_SUPPORTED_SPRITE_DIM,MAX_SUPPORTED_SCREEN_WIDTH)
#define SPRITE_SCALING_YSTEPS max(MAX_SUPPORTED_SPRITE_DIM,MAX_SUPPORTED_SCREEN_HEIGHT)

extern i32 xsteps_array[];
extern i32 ysteps_array[];

extern i32 alpha_xsteps_array[];
extern i32 alpha_ysteps_array[];

extern i32 scale_up;
extern i32 alpha_scale_up;

extern TbPixel *render_ghost;
extern TbPixel *render_alpha;


/**
 * Sets X scaling array for drawing scaled sprites.
 * The X scaling array contains position and length of each pixel of the sprite on destination buffer.
 *
 * @param xsteps_arr The destination X scaling array.
 * @param x Position of the sprite in output buffer, X coord.
 * @param swidth Source sprite original width.
 * @param dwidth Width which the sprite should have on destination buffer.
 * @param gwidth Graphics buffer visible window line width.
 */
void LbSpriteSetScalingWidthClippedArray(i32 * xsteps_arr, i32 x, i32 swidth, i32 dwidth, i32 gwidth);
void LbSpriteSetScalingWidthSimpleArray(i32 * xsteps_arr, i32 x, i32 swidth, i32 dwidth);
void LbSpriteClearScalingWidthArray(i32 * xsteps_arr, i32 swidth);

/**
 * Sets Y scaling array for drawing scaled sprites.
 * The Y scaling array contains position and length of each line of pixels of the sprite on destination buffer.
 *
 * @param ysteps_arr The destination X scaling array.
 * @param y Position of the sprite in output buffer, Y coord.
 * @param sheight Source sprite original height.
 * @param dheight Height which the sprite should have on destination buffer.
 * @param gheight Graphics buffer visible window lines count.
 */
void LbSpriteSetScalingHeightClippedArray(i32 * ysteps_arr, i32 y, i32 sheight, i32 dheight, i32 gheight);
void LbSpriteSetScalingHeightSimpleArray(i32 * ysteps_arr, i32 y, i32 sheight, i32 dheight);
void LbSpriteClearScalingHeightArray(i32 * ysteps_arr, i32 sheight);

void LbPixelBlockCopyForward(TbPixel * dst, const TbPixel * src, i32 len);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_INSPRITE_H_
/******************************************************************************/
