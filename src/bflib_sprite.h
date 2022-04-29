/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_sprite.h
 *     Header file for bflib_sprite.c.
 * @par Purpose:
 *     Graphics sprites support library.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     12 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_SPRITE_H
#define BFLIB_SPRITE_H

#include "globals.h"
#include "bfsprite.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

/******************************************************************************/
/*
extern struct TbSetupSprite setup_sprites[];
extern char mouse_pointer_sprite;
extern char lang_selection;
*/
/******************************************************************************/
int LbSpriteClearAll(struct TbSetupSprite t_setup[]);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
