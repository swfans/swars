/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfmouse.h
 *     Header file for smouse.cpp.
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
#ifndef BFLIBRARY_BFMOUSE_H_
#define BFLIBRARY_BFMOUSE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TbSprite;

TbResult LbMousePlace(void);
TbResult LbMouseRemove(void);
TbResult LbMouseChangeSpriteOffset(unsigned long hsX, unsigned long hsY);
TbResult LbMouseChangeSprite(const struct TbSprite *pointer_spr);
TbResult LbMouseChangeMoveRatio(long ratio_x, long ratio_y);
TbResult LbMouseSetup(const struct TbSprite *pointer_spr, int ratio_x, int ratio_y);
TbResult LbMouseReset(void);
TbResult LbMouseSuspend(void);
TbResult LbMouseSetWindow(long x, long y, long width, long height);
TbResult LbMouseSetPosition(long x, long y);
int LbMouseUpdatePosition();

extern volatile TbBool lbMouseInstalled;
extern volatile TbBool lbMouseOffline;
extern volatile TbBool lbInteruptMouse;

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFMOUSE_H_
/******************************************************************************/
