/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfcircle.h
 *     Header file for gcircle.c.
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
#ifndef BFLIBRARY_BFCIRCLE_H_
#define BFLIBRARY_BFCIRCLE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

TbResult LbDrawCircle(i32 x, i32 y, u32 radius, TbPixel colour);

TbResult LbDrawCircleOutline(i32 x, i32 y, u32 radius, TbPixel colour);
TbResult LbDrawCircleFilled(i32 x, i32 y, u32 radius, TbPixel colour);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFCIRCLE_H_
/******************************************************************************/
