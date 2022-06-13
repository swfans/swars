/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfbox.h
 *     Header file for gbox.cpp, box_clip.asm.
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
#ifndef BFLIBRARY_BFBOX_H_
#define BFLIBRARY_BFBOX_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

int LbDrawBox();
int LbDrawBoxCoords();

/** Draw box on graphics window with given coords, dimensions and colour.
 *
 * Graphics window needs to be set and locked. Coordinaates have to be correct,
 * because they are not verified to bounds.
 * This function honors DrawFlags.
 */
TbResult LbDrawBoxNoClip(long destX, long destY, ulong width, ulong height,
  TbPixel colour);

/** Draw clipped box on graphics window with given coords, dimensions and colour.
 *
 * Graphics window needs to be set and locked. Coordinates are clipped if they
 * exceed the current graphics window.
 * This function honors DrawFlags.
 */
TbResult LbDrawBoxClip(long destX, long destY, ulong width, ulong height,
  TbPixel colour);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFBOX_H_
/******************************************************************************/
