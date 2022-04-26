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
#ifndef BFLIBRARY_PIXEL_H_
#define BFLIBRARY_PIXEL_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

int LbDrawPixelClip();
int LbDrawPixel();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_PIXEL_H_
/******************************************************************************/
