/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfline.h
 *     Header file for gline.cpp, hvline.asm.
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
#ifndef BFLIBRARY_BFLINE_H_
#define BFLIBRARY_BFLINE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Draws a line on current graphics window.
 *
 * Truncates the coordinates if they go off the window. Requires the screen
 * to be locked.
 */
TbResult LbDrawLine(long X1, long Y1, long X2, long Y2, TbPixel colour);

TbResult LbDrawHVLine(long X1, long Y1, long X2, long Y2, TbPixel colour);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFLINE_H_
/******************************************************************************/
