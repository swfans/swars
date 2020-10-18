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

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#define PALETTE_COLORS 256
#define PALETTE_SIZE (3*PALETTE_COLORS)

#define MAX_SUPPORTED_SCREEN_WIDTH  3840
#define MAX_SUPPORTED_SCREEN_HEIGHT 2160

#pragma pack(1)

/** Pixel definition - represents value of one point on the graphics screen. */
typedef unsigned char TbPixel;

/** Standard video mode indexes, registered by LbScreenInitialize(). */
// TODO (related number 01) do we need all these wired reolution? What about chaning them to
// useful value like 1920 1080?
enum ScreenMode {
    Lb_SCREEN_MODE_INVALID      = 0x00,
    Lb_SCREEN_MODE_320_200_8    = 0x01,
    Lb_SCREEN_MODE_320_200_16   = 0x02,
    Lb_SCREEN_MODE_320_200_24   = 0x03,
    Lb_SCREEN_MODE_512_384_16   = 0x08,
    Lb_SCREEN_MODE_512_384_24   = 0x09,
    Lb_SCREEN_MODE_640_400_8    = 0x0A,
    Lb_SCREEN_MODE_640_400_16   = 0x0B,
    Lb_SCREEN_MODE_320_240_8    = 0x04,
    Lb_SCREEN_MODE_320_240_16   = 0x05,
    Lb_SCREEN_MODE_320_240_24   = 0x06,
    Lb_SCREEN_MODE_512_384_8    = 0x07,
    Lb_SCREEN_MODE_640_400_24   = 0x0C,
    Lb_SCREEN_MODE_640_480_8    = 0x0D,
    Lb_SCREEN_MODE_640_480_16   = 0x0E,
    Lb_SCREEN_MODE_640_480_24   = 0x0F,
    Lb_SCREEN_MODE_800_600_8    = 0x10,
    Lb_SCREEN_MODE_800_600_16   = 0x11,
    Lb_SCREEN_MODE_800_600_24   = 0x12,
    Lb_SCREEN_MODE_1024_768_8   = 0x13,
    Lb_SCREEN_MODE_1024_768_16  = 0x14,
    Lb_SCREEN_MODE_1024_768_24  = 0x15,
    Lb_SCREEN_MODE_1200_1024_8  = 0x16,
    Lb_SCREEN_MODE_1200_1024_16 = 0x17,
    Lb_SCREEN_MODE_1200_1024_24 = 0x18,
    Lb_SCREEN_MODE_1600_1200_8  = 0x19,
    Lb_SCREEN_MODE_1600_1200_16 = 0x1A,
    Lb_SCREEN_MODE_1600_1200_24 = 0x1B,
/* new way - maybe switch later
    Lb_SCREEN_MODE_INVALID      = 0x00,
    Lb_SCREEN_MODE_320_200      = 0x01, // Used to play movies
    Lb_SCREEN_MODE_320_240      = 0x02,
    Lb_SCREEN_MODE_512_384      = 0x03,
    Lb_SCREEN_MODE_640_400      = 0x04,
    Lb_SCREEN_MODE_640_480      = 0x05,
    Lb_SCREEN_MODE_800_600      = 0x06,
    Lb_SCREEN_MODE_1024_768     = 0x07,
    Lb_SCREEN_MODE_1200_1024    = 0x08,
    Lb_SCREEN_MODE_1600_1200    = 0x09,*/
};

typedef unsigned short TbScreenMode;
typedef short TbScreenCoord;

enum TbPaletteFadeFlag {
    Lb_PALETTE_FADE_OPEN   = 0,
    Lb_PALETTE_FADE_CLOSED = 1,
};

enum TbDrawFlags {
    Lb_SPRITE_FLIP_HORIZ   = 0x0001,
    Lb_SPRITE_FLIP_VERTIC  = 0x0002,
    Lb_SPRITE_TRANSPAR4    = 0x0004,
    Lb_SPRITE_TRANSPAR8    = 0x0008,
    Lb_SPRITE_OUTLINE      = 0x0010,
    Lb_TEXT_HALIGN_LEFT    = 0x0020,
    Lb_TEXT_ONE_COLOR      = 0x0040,
    Lb_TEXT_HALIGN_RIGHT   = 0x0080,
    Lb_TEXT_HALIGN_CENTER  = 0x0100,
    Lb_TEXT_HALIGN_JUSTIFY = 0x0200,
    Lb_TEXT_UNDERLINE      = 0x0400,
    Lb_TEXT_UNDERLNSHADOW  = 0x0800,
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

struct ScreenModeInfo {
    /** Hardware driver screen width. */
    TbScreenCoord Width;

    /** Hardware driver screen height. */
    TbScreenCoord Height;

    /** Hardware driver color depth. Not in use since we fixed this to 32, remove when possible.*/
    unsigned short BitsPerPixel;

    /** Is the mode currently available for use. */
    int Available;

    /** Video mode flags. Can be Lb_VF_DEFAULT, Lb_VF_PALETTE, Lb_VF_TRUCOLOR, Lb_VF_RGBCOLOR.
    Not in use anymore since we fixed video mode to 32bit RGB color, remove when possible.;*/
    unsigned long VideoFlags;

    /** Text description of the mode. */
    char Desc[24];
};
typedef struct ScreenModeInfo TbScreenModeInfo;

// Do NOT modify imported structures
struct DisplayStruct {
        /** Pointer to physical screen buffer, not used. */
        unsigned char *PhysicalScreen;

        /** Pointer to graphics screen buffer, if locked. */
        unsigned char *WScreen;

        /** Pointer to glass map, used for 8-bit video transparency. */
        unsigned char *GlassMap;

        /** Pointer to fade table, used for 8-bit video fading. */
        unsigned char *FadeTable;

        /** Pointer to graphics window buffer, if locked. */
        unsigned char *GraphicsWindowPtr;

        /** Sprite used as mouse cursor. */
        struct TbSprite *MouseSprite;

        /** Resolution in width of the current video mode.
         *  Note that it's not always "physical" size.
         *  It is the part of screen buffer which is being drawn
         *  on physical screen (WScreen X pixel number). */
        long PhysicalScreenWidth;

        /** Resolution in height of the current video mode.
         *  Note that it's not always "physical" size.
         *  It is the part of screen buffer which is being drawn
         *  on physical screen (WScreen Y pixel number). */
        long PhysicalScreenHeight;

        /** Width of the screen buffer (WScreen X pitch).
         *  Note that only part of this width may be drawn on real screen. */
        long GraphicsScreenWidth;

        /** Height of the screen buffer (WScreen Y pitch).
        *  Note that only part of this height may be drawn on real screen. */
        long GraphicsScreenHeight;

        /** Current drawing area beginning X coordinate. */
        long GraphicsWindowX;

        /** Current drawing area beginning Y coordinate. */
        long GraphicsWindowY;

        /** Current drawing area width (size in X axis). */
        long GraphicsWindowWidth;

        /** Current drawing area height (size in Y axis). */
        long GraphicsWindowHeight;

        /** Current mouse clipping window start X coordinate. */
        long MouseWindowX;

        /** Current mouse clipping window start Y coordinate. */
        long MouseWindowY;

        /** Current mouse clipping window width (in pixels). */
        long MouseWindowWidth;

        /** Current mouse clipping window height (in pixels). */
        long MouseWindowHeight;

        /** Mouse position during button "down" event, X coordinate. */
        long MouseX;

        /** Mouse position during button "down" event, Y coordinate. */
        long MouseY;

        /** Mouse position during move, X coordinate. */
        long MMouseX;

        /** Mouse position during move, Y coordinate. */
        long MMouseY;

        /** Mouse position during button release, X coordinate. */
        long RMouseX;

        /** Mouse position during button release, Y coordinate. */
        long RMouseY;

        unsigned short DrawFlags;
        short OldVideoMode;

        // Actual Screen Mode of the lbDrawTexture, can be same as mode in setting, or 320*200 for playing movie.
        unsigned short ScreenMode;

        /** VESA set-up flag, used only with VBE video modes. */
        unsigned char VesaIsSetUp;
        unsigned char LeftButton;
        unsigned char RightButton;
        unsigned char MiddleButton;
        unsigned char MLeftButton;
        unsigned char MRightButton;
        unsigned char MMiddleButton;
        unsigned char RLeftButton;
        unsigned char RMiddleButton;
        unsigned char RRightButton;
        unsigned char FadeStep;
        /** Selected drawing colour index. */
        unsigned char DrawColour;
        /** Currently active colour palette.
         *  LbPaletteGet() should be used to retrieve a copy of the palette. */
        unsigned char *Palette;
};
typedef struct DisplayStruct TbDisplayStruct;

#pragma pack()

extern TbDisplayStruct lbDisplay;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
