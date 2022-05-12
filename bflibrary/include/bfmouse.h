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

#pragma pack(1)

struct TbSprite;

struct mouse_buffer { // sizeof=4128
    DwBool Valid; // offset=0
    long Width; // offset=4
    long Height; // offset=8
    ulong Offset; // offset=12
    ubyte Buffer[4096]; // offset=16
    long X; // offset=4112
    long Y; // offset=4116
    long XOffset; // offset=4120
    long YOffset; // offset=4124
};

typedef struct mouse_buffer mouse_buffer;

struct mouse_info { // sizeof=4112
    long XMoveRatio; // offset=0
    long YMoveRatio; // offset=4
    long XSpriteOffset; // offset=8
    long YSpriteOffset; // offset=12
    ubyte Sprite[64*64]; // offset=16
};

typedef struct mouse_info mouse_info;

#pragma pack()

int LbMousePlace();
int LbMouseRemove();
int LbMouseChangeSpriteOffset(unsigned long hsX, unsigned long hsY);
TbResult LbMouseChangeSprite(const struct TbSprite *pointer_spr);
int LbMouseChangeMoveRatio();
TbResult LbMouseSetup(const struct TbSprite *pointer_spr, int ratio_x, int ratio_y);
int LbMouseReset();
int LbMouseSuspend();
int LbMouseSetWindow();
int LbMouseSetPosition();
int LbMouseUpdatePosition();

extern volatile TbBool lbMouseInstalled;
extern volatile TbBool lbMouseOffline;

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFMOUSE_H_
/******************************************************************************/
