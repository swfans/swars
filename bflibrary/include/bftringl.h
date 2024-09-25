/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bftringl.h
 *     Header file for gtringl.c.
 * @par Purpose:
 *     Drawing triangles.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 22 Sep 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFTRINGL_H_
#define BFLIBRARY_BFTRINGL_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Draw clipped triangle on graphics window at given 3 points, with colour.
 *
 * Graphics window needs to be set and locked. Coordinates are clipped if they
 * exceed the current graphics window. For the triangle to be visible, points
 * need to be provided in clockwise order, which means the inequality need to
 * be met: `(y3 - y2) * (x2 - x1) > (y2 - y1) * (x3 - x2)`.
 * This function honors DrawFlags.
 */
void LbDrawTriangle(short x1, short y1, short x2, short y2, short x3, short y3,
  TbPixel colour);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFTRINGL_H_
/******************************************************************************/
