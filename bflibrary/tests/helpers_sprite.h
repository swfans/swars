/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file helpers_sprite.h
 *     Header file for helpers_sprite.c.
 * @par Purpose:
 *     Implementation of sample sprite data creation for tests.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 Nov 2024 - 21 Mar 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef HELPERS_SPRITE_H_
#define HELPERS_SPRITE_H_

#include "bftypes.h"
#include "bfsprite.h"

#ifdef __cplusplus
extern "C" {
#endif

void generate_example_sprites_from_png(int sprfile_no, const ubyte *pal,
  ubyte *p_dat, TbSprite *p_tab);
void raw_to_sprite(short X, short Y, ushort Width, ushort Height, ubyte *Raw);

#ifdef __cplusplus
};
#endif

#endif // HELPERS_SPRITE_H_
/******************************************************************************/
