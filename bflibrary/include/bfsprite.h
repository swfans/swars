/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfsprite.h
 *     Header file for gsprite.cpp, gspr_std.cpp, gspr_col.cpp, spr_scl.asm.
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
#ifndef BFLIBRARY_GSPRITE_H_
#define BFLIBRARY_GSPRITE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SUPPORTED_SPRITE_DIM 256

struct TbSprite { // sizeof=6
    SBYTE *Data; // offset=0
    UBYTE SWidth; // offset=4
    UBYTE SHeight; // offset=5
};

typedef struct TbSprite TbSprite;

struct TbSetupSprite { // sizeof=12
    TbSprite **Start; // offset=0
    TbSprite **End; // offset=4
    SBYTE **Data; // offset=8
};

typedef struct TbSetupSprite TbSetupSprite;

int LbSpriteReset();
int LbSpriteResetAll();
int LbSpriteSetup();
int LbSpriteSetupAll();
int LbSpriteDrawScaled();
extern int lbSpriteReMapPtr;

int LbSpriteDrawOneColour();
int LbSpriteDraw();

int LbSpriteDrawUsingScalingData();
int LbSpriteSetScalingData();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GSPRITE_H_
/******************************************************************************/
