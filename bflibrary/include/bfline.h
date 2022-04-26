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
#ifndef BFLIBRARY_GLINE_H_
#define BFLIBRARY_GLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

int LbDrawLine();
void draw_b_line( long, long, long, long, char unsigned );
int LbDrawHVLine();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GLINE_H_
/******************************************************************************/
