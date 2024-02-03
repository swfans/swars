/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bflib_vidraw.h
 *     Header file for bflib_vidraw.c.
 * @par Purpose:
 *     Routines to be moved to bflibrary.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_VIDRAW_H
#define BFLIB_VIDRAW_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

/** Draw clipped triangle on graphics window at given 3 points, with colour.
 *
 * Graphics window needs to be set and locked. Coordinates are clipped if they
 * exceed the current graphics window.
 * This function honors DrawFlags.
 */
void LbDrawTriangle(short x1, short y1, short x2, short y2, short x3, short y3,
  TbPixel colour);

void LbDrawSlantBox(short X, short Y, ushort a3, ushort a4);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
