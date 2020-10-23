/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_bufrw.c
 *     Header file for bflib_mouse.c.
 * @par Purpose:
 *     Mouse related routines.
 * @par Comment:
 *   Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     12 Feb 2008 - 10 Oct 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_MOUSE_H
#define BFLIB_MOUSE_H

#include "bflib_basics.h"
#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct TbSprite;
struct TbPoint;

enum TbMouseAction {
    MActn_NONE = 0,
    MActn_MOUSEMOVE,
    MActn_LBUTTONDOWN,
    MActn_LBUTTONUP,
    MActn_RBUTTONDOWN,
    MActn_RBUTTONUP,
    MActn_MBUTTONDOWN,
    MActn_MBUTTONUP,
    MActn_WHEELMOVEUP,
    MActn_WHEELMOVEDOWN,
};

struct mouse_buffer {
        long Valid;//bool
        long Width;
        long Height;
        unsigned long Offset;
        unsigned char Buffer[0x1000];
        long X;
        long Y;
        long XOffset;
        long YOffset;
};

struct mouse_info {
        long XMoveRatio;
        long YMoveRatio;
        long XSpriteOffset;
        long YSpriteOffset;
        //Note: debug info says it has 0x100 items, but this is suspicious..
        unsigned char Sprite[0x1000];
};

#pragma pack()
/******************************************************************************/
/*TbResult LbMouseChangeSpriteAndHotspot(struct TbSprite *pointer_spr, long hot_x, long hot_y);*/
TbResult LbMouseSetup(struct TbSprite *pointer_spr, int ratio_x, int ratio_y);
/*TbResult LbMouseSetPointerHotspot(long hot_x, long hot_y);
TbResult LbMouseSetPosition(long x, long y);*/
TbResult LbMouseChangeSprite(struct TbSprite *pointer_spr);
/*TbResult LbMouseSuspend(void);
void GetPointerHotspot(long *hot_x, long *hot_y);
TbResult LbMouseIsInstalled(void);
TbResult LbMouseSetWindow(long x, long y, long width, long height);
TbResult LbMouseChangeMoveRatio(long ratio_x, long ratio_y);

void mouseControl(unsigned int action, struct TbPoint *pos);
TbResult LbMouseOnBeginSwap(void);
TbResult LbMouseOnEndSwap(void);*/
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
