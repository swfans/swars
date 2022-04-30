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

extern long xsteps_array[2*SPRITE_SCALING_XSTEPS];
extern long ysteps_array[2*SPRITE_SCALING_YSTEPS];

extern long alpha_xsteps_array[2*SPRITE_SCALING_XSTEPS];
extern long alpha_ysteps_array[2*SPRITE_SCALING_YSTEPS];

extern long scale_up;
extern long alpha_scale_up;

extern ubyte *render_ghost;
extern ubyte *render_alpha;


void LbPixelBlockCopyForward(TbPixel * dst, const TbPixel * src, long len);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_INSPRITE_H_
/******************************************************************************/
