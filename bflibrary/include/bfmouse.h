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
#ifndef BFLIBRARY_SMOUSE_H_
#define BFLIBRARY_SMOUSE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TbSprite;

struct mouse_buffer { // sizeof=4128
    BOOL Valid; // offset=0
    SLONG Width; // offset=4
    SLONG Height; // offset=8
    ULONG Offset; // offset=12
    UBYTE Buffer[4096]; // offset=16
    SLONG X; // offset=4112
    SLONG Y; // offset=4116
    SLONG XOffset; // offset=4120
    SLONG YOffset; // offset=4124
};

typedef struct mouse_buffer mouse_buffer;

struct mouse_info { // sizeof=4112
    SLONG XMoveRatio; // offset=0
    SLONG YMoveRatio; // offset=4
    SLONG XSpriteOffset; // offset=8
    SLONG YSpriteOffset; // offset=12
    UBYTE Sprite[4096]; // offset=16
};

typedef struct mouse_info mouse_info;

int LbMousePlace();
int LbMouseRemove();
int LbMouseChangeSpriteOffset(unsigned long hsX, unsigned long hsY);
int LbMouseChangeSprite(struct TbSprite *spr);
int LbMouseChangeMoveRatio();
int LbMouseSetup();
int LbMouseReset();
int LbMouseSuspend();
int LbMouseSetWindow();
int LbMouseSetPosition();
int LbMouseUpdatePosition();

extern volatile TbBool lbMouseInstalled;

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_SMOUSE_H_
/******************************************************************************/
