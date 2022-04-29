/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_video.h
 *     Header file for bflib_video.c.
 * @par Purpose:
 *     Video support library for 8-bit graphics.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     11 Feb 2008 - 22 Jul 2015
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_VIDEO_H
#define BFLIB_VIDEO_H

#include "bflib_basics.h"
#include "bfscreen.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)

/** Pixel definition - represents value of one point on the graphics screen. */
typedef unsigned char TbPixel;

enum TbPaletteFadeFlag {
    Lb_PALETTE_FADE_OPEN   = 0,
    Lb_PALETTE_FADE_CLOSED = 1,
};

enum TbVideoModeFlags {
    Lb_VF_DEFAULT     = 0x0000, // dummy flag
    Lb_VF_RGBCOLOR    = 0x0001,
    Lb_VF_WINDOWED    = 0x0010,
};

struct GraphicsWindow {
    long x;
    long y;
    long width;
    long height;
    TbPixel *ptr;
};
typedef struct GraphicsWindow TbGraphicsWindow;


#pragma pack()

/******************************************************************************/
int LbScreenSetupAnyMode(unsigned short mode, unsigned long width,
    unsigned long height, TbPixel *palette);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
