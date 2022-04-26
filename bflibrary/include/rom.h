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

#ifdef __cplusplus
extern "C" {
#endif

int prop_text();
int make_fade_table();
extern int tabwidth;
extern int font;
int qaz();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_ROM_H_
/******************************************************************************/
