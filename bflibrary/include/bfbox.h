/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfbox.h
 *     Header file for gbox.c, box_clip.c.
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

/** Draw clipped box on graphics window at given point, with dimensions and colour.
 *
 * Graphics window needs to be set and locked. Coordinates are clipped if they
 * exceed the current graphics window.
 * This function honors DrawFlags.
 */
TbResult LbDrawBox(long X, long Y, long Width, long Height, TbPixel colour);

/** Draw clipped box on graphics window with given coords and colour.
 *
 * High level function, after some coords manipulation internally calls
 * LbDrawBox() to perform the drawing.
 */
TbResult LbDrawBoxCoords(long X1, long Y1, long X2, long Y2, TbPixel colour);

/** Draw box on graphics window with given coords, dimensions and colour.
 *
 * Graphics window needs to be set and locked. Coordinates have to be correct,
 * because they are not verified to bounds.
 * This function honors some DrawFlags.
 */
TbResult LbDrawBoxNoClip(long destX, long destY, ulong width, ulong height,
  TbPixel colour);

/** Draw clipped box on graphics window with given coords, dimensions and colour.
 *
 * Graphics window needs to be set and locked. Coordinates are clipped if they
 * exceed the current graphics window.
 * This function honors some DrawFlags.
 */
TbResult LbDrawBoxClip(long destX, long destY, ulong width, ulong height,
  TbPixel colour);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFBOX_H_
/******************************************************************************/
