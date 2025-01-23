/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file app_sprite.h
 *     Header file for app_spr_init.c, ap_gspr_map.c, ap_spr_smap.c.
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
#ifndef APP_SPRITE_H_
#define APP_SPRITE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct TbSprite;

#pragma pack()

extern long dword_1DC36C;
extern ubyte SCANNER_pal_bright[];
extern ubyte SCANNER_bright_limit[];

/** Init a table storing brightness of each colour in the palette.
 */
void SCANNER_init_palette_bright(void);

/** Init a table used to limit colour value to 6-bit area.
 */
void SCANNER_init_bright_limit_table(void);

/** Sprite drawing routine with modified transparency support.
 * Instead of standard transparency, it converts input and background to
 * grayscale and recolors using given color map.
 */
TbResult ApSpriteDrawLowTransGreyRemap(long x, long y,
  const struct TbSprite *spr, const ubyte *transmap);

/** Scaled sprite drawing routine with modified transparency support.
 * Instead of standard transparency, it converts input and background to
 * grayscale and recolors using given color map.
 */
TbResult ApSpriteDrawScaledLowTransGreyRemap(long xpos, long ypos,
  const struct TbSprite *sprite, long dest_width, long dest_height,
  const ubyte *transmap);

#ifdef __cplusplus
};
#endif

#endif // APP_SPRITE_H_
/******************************************************************************/
