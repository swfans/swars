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
#include "bfscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

void get_example_sprites_file_name(int sprfile_no, char *fname);
TbScreenMode get_example_sprites_screen_mode(int sprfile_no);
int get_example_sprites_total_count(int sprfile_no);
void palette_remap_to_screen(TbPixel *p_remap, const TbPixel *p_altpal);

int generate_example_sprites_from_screen(int sprfile_no, const ubyte *pal,
  ubyte *p_dat, TbSprite *p_tab);

#ifdef __cplusplus
};
#endif

#endif // HELPERS_SPRITE_H_
/******************************************************************************/
