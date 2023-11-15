/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file helpers_screen.h
 *     Header file for helpers_screen.c.
 * @par Purpose:
 *     Implementation of screen-related helper functions for tests.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     05 Nov 2021 - 29 Jul 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef HELPERS_SCREEN_H_
#define HELPERS_SCREEN_H_

#include "bftypes.h"
#include "bfscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

void generate_example_texture_map_xor_based(const u8 *pal, u8 *texmap);
void raw_to_wscreen(i16 X, i16 Y, u16 Width, u16 Height, u8 *Raw);

#ifdef __cplusplus
};
#endif

#endif // HELPERS_SCREEN_H_
/******************************************************************************/
