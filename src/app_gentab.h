/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file app_gentab.h
 *     Header file for ap_gengh.c.
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
#ifndef APP_GENTAB_H_
#define APP_GENTAB_H_

#include "bftypes.h"
#include "bfpalette.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

/** Colour tables, used for fading and merging colours in palette mode.
 */
struct ApColorTables {
  /** Table used for adding two colors without brightness schange of the first. */
  TbPixel ghost_add_table[PALETTE_8b_COLORS*PALETTE_8b_COLORS];
};

#pragma pack()

extern struct ApColorTables appixmap;

TbResult LbScreenTableGenerate(const ubyte *pal, short intens, const char *fname);
TbResult LbScreenTableLoad(const ubyte *pal, short intens, const char *fname);

#ifdef __cplusplus
};
#endif

#endif // APP_GENTAB_H_
/******************************************************************************/
