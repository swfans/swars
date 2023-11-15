/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mock_bfpalette.h
 *     Header file for mock_palette.c.
 * @par Purpose:
 *     Implementation which only pretends to do stuff, for test purposes.
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
#ifndef MOCK_BFPALETTE_H_
#define MOCK_BFPALETTE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

TbResult MockPaletteSet(const u8 *palette);

/** Make simple and fairly universal palette.
 */
void make_general_palette(u8 *pal);

#ifdef __cplusplus
};
#endif

#endif // MOCK_BFPALETTE_H_
/******************************************************************************/
