/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfsprite.h
 *     Header file for gsprite.c, gspr_std.cpp, gspr_col.cpp, spr_scl.asm.
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
#ifndef BFLIBRARY_BFSPRITE_H_
#define BFLIBRARY_BFSPRITE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SPRITE_FORMAT_V2
#define MAX_SUPPORTED_SPRITE_DIM 4096
#else
#define MAX_SUPPORTED_SPRITE_DIM 256
#endif

#pragma pack(1)

/**
 * Type which contains buffer of a sprite, with RLE-encoded alpha channel.
 */
typedef unsigned char * TbSpriteData;

typedef struct TbSprite TbSprite;

struct TbSprite {
    TbSpriteData Data;
#ifdef SPRITE_FORMAT_V2
    ushort SWidth;
    ushort SHeight;
#else
    ubyte SWidth;
    ubyte SHeight;
#endif
};

typedef struct TbSetupSprite TbSetupSprite;

struct TbSetupSprite {
    TbSprite **Start;
    TbSprite **End;
    TbSpriteData *Data;
};

struct TbHugeSprite {
    TbSpriteData Data;  //**< Raw sprite data, with RLE coded transparency.
    long * Lines;  //**< Index of line starts in the sprite data.
    unsigned long SWidth;
    unsigned long SHeight;
};

#define TILED_SPRITE_DIM 10

struct TiledSprite {
    unsigned char x_num;
    unsigned char y_num;
    unsigned short spr_idx[TILED_SPRITE_DIM][TILED_SPRITE_DIM];
};

#pragma pack()

short LbSpriteSetup(struct TbSprite *start, const struct TbSprite *end, const unsigned char * data);
int LbSpriteSetupAll(struct TbSetupSprite t_setup[]);

int LbSpriteReset();
int LbSpriteResetAll();
int LbSpriteDrawScaled();
extern ubyte * lbSpriteReMapPtr;

int LbSpriteDrawOneColour();
int LbSpriteDraw();

int LbSpriteDrawUsingScalingData();
int LbSpriteSetScalingData();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFSPRITE_H_
/******************************************************************************/
