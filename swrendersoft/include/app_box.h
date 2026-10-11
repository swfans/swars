/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file app_box.h
 *     Header file for app_box.c, app_box_lt.c.
 * @par Purpose:
 *     App-specific box drawing, for slanted box and low transparency box.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 02 Oct 2026
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef APP_BOX_H_
#define APP_BOX_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Draw clipped slant box on graphics window at given point, with dimensions and colour.
 *
 * Draws slant box / kicked box / parallellogram skewed 45 deg.
 * Graphics window needs to be set and locked. Coordinates are clipped if they
 * exceed the current graphics window.
 *
 * This function is like a extension to bflibrary API, but it's defined on application side.
 * This function honors DrawFlags.
 */
TbResult AppDrawSlantBox(s32 X, s32 Y, s32 Width, s32 Height, TbPixel colour);

TbResult AppDrawVSlantBox(s32 X, s32 Y, s32 Width, s32 Height, TbPixel colour);

TbResult AppDrawBoxLowTransGrey(s32 X, s32 Y, s32 Width, s32 Height, TbPixel colour);

TbResult AppDrawSlantBoxLowTransGrey(s32 X, s32 Y, s32 Width, s32 Height, TbPixel colour);

TbResult AppDrawVSlantBoxLowTransGrey(s32 X, s32 Y, s32 Width, s32 Height, TbPixel colour);

TbResult AppDrawTexturedFlowSlantBox(s32 X, s32 Y, s32 Width, s32 Height,
  ubyte vecMode, ubyte brig, ubyte tmapNo, ushort nUa, ushort nVb);

#ifdef __cplusplus
};
#endif

#endif // APP_BOX_H_
/******************************************************************************/
