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
typedef uc8 * TbSpriteData;

typedef struct TbSprite TbSprite;

struct TbSprite {
    TbSpriteData Data;
#ifdef SPRITE_FORMAT_V2
    ushort SWidth;
    ushort SHeight;
#else
    u8 SWidth;
    u8 SHeight;
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
    i32 * Lines;  //**< Index of line starts in the sprite data.
    u32 SWidth;
    u32 SHeight;
};

#define TILED_SPRITE_DIM 10

struct TiledSprite {
    u8 x_num;
    u8 y_num;
    u16 spr_idx[TILED_SPRITE_DIM][TILED_SPRITE_DIM];
};

#pragma pack()

/** Set up data pointers in an array of sprites within given bounds.
 */
TbResult LbSpriteSetup(TbSprite *start, const TbSprite *end, const u8 * data);

/** Set up data pointers in all arrays of sprites from given setup list.
 */
TbResult LbSpriteSetupAll(TbSetupSprite t_setup[]);

/** Clear data pointers in all arrays of sprites from given setup list.
 */
TbResult LbSpriteClearAll(TbSetupSprite t_setup[]);

/** Revert data pointers in an array of sprites to initial values.
 *
 * This basically reverts LbSpriteSetup() operation, making data fields store offsets
 * instead of addresses.
 */
TbResult LbSpriteReset(TbSprite *start, const TbSprite *end, const u8 *data);

TbResult LbSpriteResetAll();

extern u8 * lbSpriteReMapPtr;

TbResult LbSpriteDrawOneColour(i32 x, i32 y, const TbSprite *spr, const TbPixel colour);
TbResult LbSpriteDraw(i32 x, i32 y, const TbSprite *spr);
TbResult LbSpriteDrawRemap(i32 x, i32 y, const TbSprite *spr, const u8 *cmap);

/**
 * Draws a scaled sprite on current graphics window at given position.
 * Requires LbSpriteSetScalingData() to be called before.
 *
 * @param posx The X coord within current graphics window.
 * @param posy The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 * @see LbSpriteSetScalingData()
 */
TbResult LbSpriteDrawUsingScalingData(i32 posx, i32 posy, const TbSprite *sprite);

/**
 * Sets scaling data for drawing scaled sprites.
 * @param x Position of the sprite in output buffer, X coord.
 * @param y Position of the sprite in output buffer, Y coord.
 * @param swidth Source sprite original width.
 * @param sheight Source sprite original height.
 * @param dwidth Width which the sprite should have on destination buffer.
 * @param dheight Height which the sprite should have on destination buffer.
 */
void LbSpriteSetScalingData(i32 x, i32 y, i32 swidth, i32 sheight,
    i32 dwidth, i32 dheight);

/**
 * Draws an alpha-blended scaled sprite on current graphics window at given position.
 * Requires SetAlphaScalingData() to be called before.
 * Unlike LbSpriteDrawUsingScalingData(), this one ignores some of the DrawFlags, and
 * assumes transparency even if related flag is not set. So it is basically kind of
 * hacked version of that function. No idea why, but it's used in Dungeon Keeper.
 *
 * @param posx The X coord within current graphics window.
 * @param posy The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 * @see SetAlphaScalingData()
 * @see LbSpriteDrawUsingScalingData()
 */
TbResult DrawAlphaSpriteUsingScalingData(i32 posx, i32 posy, const TbSprite *sprite);

/**
 * Draws a scaled sprite on current graphics window at given position, ignoring shadows.
 * Requires SetAlphaScalingData() to be called before.
 * This is a modification of LbSpriteDrawUsingScalingData() which changes meaning
 * of Lb_TEXT_UNDERLNSHADOW and Lb_SPRITE_TRANSPAR8. No idea why, but it's used in Syndicate Wars.
 *
 * @param posx The X coord within current graphics window.
 * @param posy The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @return Gives 0 on success.
 * @see LbSpriteSetScalingData()
 */
TbResult DrawSpriteWthShadowUsingScalingData(i32 posx, i32 posy, const TbSprite *sprite);

void SetAlphaScalingData(i32 x, i32 y, i32 swidth, i32 sheight,
    i32 dwidth, i32 dheight);

TbResult LbSpriteDrawScaled(i32 xpos, i32 ypos, const TbSprite *sprite,
    i32 dest_width, i32 dest_height);

#define LbSpriteDrawResized(xpos, ypos, un_per_px, sprite) \
  LbSpriteDrawScaled(xpos, ypos, sprite, \
    ((sprite)->SWidth * un_per_px + 8) / 16, \
    ((sprite)->SHeight * un_per_px + 8) / 16)

/**
 * Draws a scaled sprite with one colour on current graphics window at given position.
 * Sets scaling data, and then uses it to draw the sprite.
 *
 * @param xpos The X coord within current graphics window.
 * @param ypos The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @param dest_width Width which the sprite should have on destination buffer.
 * @param dest_height Height which the sprite should have on destination buffer.
 * @param colour The colour to be used for drawing.
 * @return 
 */
TbResult LbSpriteDrawScaledOneColour(i32 xpos, i32 ypos, const TbSprite *sprite,
  i32 dest_width, i32 dest_height, const TbPixel colour);

#define LbSpriteDrawResizedOneColour(xpos, ypos, un_per_px, sprite, colour) \
  LbSpriteDrawScaledOneColour(xpos, ypos, sprite, \
    ((sprite)->SWidth * un_per_px + 8) / 16, \
    ((sprite)->SHeight * un_per_px + 8) / 16, colour)

/**
 * Draws a scaled sprite with remapped colours on current graphics window at given position.
 * Sets scaling data, and then uses it to draw the sprite.
 *
 * @param xpos The X coord within current graphics window.
 * @param ypos The Y coord within current graphics window.
 * @param sprite The source sprite.
 * @param dest_width Width which the sprite should have on destination buffer.
 * @param dest_height Height which the sprite should have on destination buffer.
 * @param cmap Colour mapping array.
 * @return 
 */
TbResult LbSpriteDrawScaledRemap(i32 xpos, i32 ypos, const TbSprite *sprite,
    i32 dest_width, i32 dest_height, const u8 *cmap);

#define LbSpriteDrawResizedRemap(xpos, ypos, un_per_px, sprite, cmap) \
  LbSpriteDrawScaledRemap(xpos, ypos, sprite, \
    ((sprite)->SWidth * un_per_px + 8) / 16, \
    ((sprite)->SHeight * un_per_px + 8) / 16, cmap)

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFSPRITE_H_
/******************************************************************************/
