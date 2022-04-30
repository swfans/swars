/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file insprite.h
 *     Internal header for shares between various sprite drawing functions.
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
#ifndef BFLIBRARY_INSPRITE_H_
#define BFLIBRARY_INSPRITE_H_

#include "bftypes.h"
#include "bfsprite.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef struct TbSpriteDrawData TbSpriteDrawData;

/** @internal
 *  Storage for temp sprite data while drawing it.
 */
struct TbSpriteDrawData {
    char *sp;
    short Wd;
    short Ht;
    unsigned char *r;
    int nextRowDelta;
    short startShift;
    TbBool mirror;
};

#pragma pack()

/** @internal
 * Prepare sprite drawing.
 * Fills TbSpriteDrawData struct with values accepted by drawing routines.
 * Used is several sprite drawing functions, not only the standard one.
 *
 * @param spd The TbSpriteDrawData struct to be filled.
 * @param x Drawing position x coordinate.
 * @param y Drawing position y coordinate.
 * @param spr Sprite to be drawn.
 * @return Gives Lb_SUCCESS if the data was prepared.
 */
TbResult LbSpriteDrawPrepare(TbSpriteDrawData *spd, long x, long y, const TbSprite *spr);

/** @internal
 *  Skip some of sprite data before drawing is started.
 *
 * @param sp Sprite data buffer pointer.
 * @param r Output buffer pointer.
 * @param x1 Width to be drawn.
 * @param left Width of the area to skip.
 */
short LbSpriteDrawLineSkipLeft(const char **sp, short *x1, short left);

/** @internal
 *  Skip to next line after drawing a requested area.
 *
 * @param sp Sprite data buffer pointer.
 * @param x1 Width difference after draw.
 */
void LbSpriteDrawLineSkipToEol(const char **sp, short *x1);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_INSPRITE_H_
/******************************************************************************/
