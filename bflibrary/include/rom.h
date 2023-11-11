/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file rom.h
 *     Header file for rom.asm.
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
#ifndef BFLIBRARY_ROM_H_
#define BFLIBRARY_ROM_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Width of tab character in pixels while drawing simple 5x6 font.
 */
extern i32 tabwidth;

/** Draw text on screen using simple built-in 5x6 font.
 */
void prop_text(const c8 *text, TbPixel *out, i32 scanline, TbPixel colour);

/** Generate table for fading to and from given colour.
 */
void make_fade_table(const u8 *pal, u8 *out, u8 cr, u8 cg, u8 cb,
  u8 ir, u8 ig, u8 ib);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_ROM_H_
/******************************************************************************/
