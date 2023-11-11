/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfpixel.h
 *     Header file for pixel.asm.
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
#ifndef BFLIBRARY_BFPIXEL_H_
#define BFLIBRARY_BFPIXEL_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Draw pixel on graphics window, with clipping.
 *
 */
TbResult LbDrawPixelClip(i32 x, i32 y, TbPixel colour);

/** Draw pixel on graphics window, without checking coords.
 *
 * This function just sets the pixel to given colour, ignoring DrawFlags.
 */
TbResult LbDrawPixel(i32 x, i32 y, TbPixel colour);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFPIXEL_H_
/******************************************************************************/
