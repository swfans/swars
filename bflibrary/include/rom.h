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
extern s32 tabwidth;

/** Draw text on screen using simple built-in 5x6 font.
 */
void prop_text(const char *text, TbPixel *out, s32 scanline, TbPixel colour);

/** Get width of given simple text if drawn using `prop_text()`.
 */
short prop_text_width(const char *text);

/** Generate table for fading to and from given colour.
 */
void make_fade_table(const ubyte *pal, ubyte *out, ubyte cr, ubyte cg, ubyte cb,
  ubyte ir, ubyte ig, ubyte ib);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_ROM_H_
/******************************************************************************/
