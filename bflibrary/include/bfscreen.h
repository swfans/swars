/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfscreen.h
 *     Header file for gscreen.cpp, sscreen.cpp.
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
#ifndef BFLIBRARY_BFSCREEN_H_
#define BFLIBRARY_BFSCREEN_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SUPPORTED_SCREEN_WIDTH  3840
#define MAX_SUPPORTED_SCREEN_HEIGHT 2160

/** Standard video modes, registered by LbScreenInitialize().
 * These are standard VESA modes, indexed this way in all Bullfrog games.
 */
enum ScreenMode { // type=int8_t
    Lb_SCREEN_MODE_INVALID = 0,
    Lb_SCREEN_MODE_320_200_8,
    Lb_SCREEN_MODE_320_200_16,
    Lb_SCREEN_MODE_320_200_24,
    Lb_SCREEN_MODE_320_240_8,
    Lb_SCREEN_MODE_320_240_16, // 5
    Lb_SCREEN_MODE_320_240_24,
    Lb_SCREEN_MODE_512_384_8,
    Lb_SCREEN_MODE_512_384_16,
    Lb_SCREEN_MODE_512_384_24,
    Lb_SCREEN_MODE_640_400_8, // 10
    Lb_SCREEN_MODE_640_400_16,
    Lb_SCREEN_MODE_640_400_24,
    Lb_SCREEN_MODE_640_480_8,
    Lb_SCREEN_MODE_640_480_16,
    Lb_SCREEN_MODE_640_480_24, // 15
    Lb_SCREEN_MODE_800_600_8,
    Lb_SCREEN_MODE_800_600_16,
    Lb_SCREEN_MODE_800_600_24,
    Lb_SCREEN_MODE_1024_768_8,
    Lb_SCREEN_MODE_1024_768_16, // 20
    Lb_SCREEN_MODE_1024_768_24,
    Lb_SCREEN_MODE_1200_1024_8,
    Lb_SCREEN_MODE_1200_1024_16,
    Lb_SCREEN_MODE_1200_1024_24,
    Lb_SCREEN_MODE_1600_1200_8, // 25
    Lb_SCREEN_MODE_1600_1200_16,
    Lb_SCREEN_MODE_1600_1200_24,
};

typedef enum ScreenMode TbScreenMode;

typedef struct TbSprite TbSprite;

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

#pragma pack(1)

typedef struct DisplayStruct TbDisplayStruct;

struct DisplayStruct { // sizeof=118
    /** Pointer to physical screen buffer, if available. */
    ubyte *PhysicalScreen; // offset=0

    /** Pointer to graphics screen buffer, if locked. */
    ubyte *WScreen; // offset=4

    /** Pointer to glass map, used for indexed color video transparency. */
    ubyte *GlassMap; // offset=8

    /** Pointer to fade table, used for indexed color video fading. */
    ubyte *FadeTable; // offset=12

    /** Pointer to graphics window buffer, if locked. */
    ubyte *GraphicsWindowPtr; // offset=16

    /** Sprite used as mouse cursor. */
    const struct TbSprite *MouseSprite;

    /** Resolution in width of the current video mode.
     *  Note that it's not always "physical" size.
     *  It is the part of screen buffer which is being drawn
     *  on physical screen (WScreen X visible pixel number). */
    long PhysicalScreenWidth; // offset=24

    /** Resolution in height of the current video mode.
     *  Note that it's not always "physical" size.
     *  It is the part of screen buffer which is being drawn
     *  on physical screen (WScreen Y visible pixel number). */
    long PhysicalScreenHeight; // offset=28

    /** Width of the screen buffer (WScreen X pitch).
     *  Note that only part of this width may be drawn on real screen. */
    long GraphicsScreenWidth; // offset=32

    /** Height of the screen buffer (WScreen Y pitch).
    *  Note that only part of this height may be drawn on real screen. */
    long GraphicsScreenHeight; // offset=36

    /** Current drawing area beginning X coordinate. */
    long GraphicsWindowX; // offset=40

    /** Current drawing area beginning Y coordinate. */
    long GraphicsWindowY; // offset=44

    /** Current drawing area width (size in X axis). */
    long GraphicsWindowWidth; // offset=48

    /** Current drawing area height (size in Y axis). */
    long GraphicsWindowHeight; // offset=52

    /** Current mouse clipping window start X coordinate. */
    long MouseWindowX; // offset=56

    /** Current mouse clipping window start Y coordinate. */
    long MouseWindowY; // offset=60

    /** Current mouse clipping window width (in pixels). */
    long MouseWindowWidth; // offset=64

    /** Current mouse clipping window height (in pixels). */
    long MouseWindowHeight; // offset=68

    /** Mouse position during button "down" event, X coordinate. */
    long MouseX; // offset=72

    /** Mouse position during button "down" event, Y coordinate. */
    long MouseY; // offset=76

    /** Mouse position during move, X coordinate. */
    long MMouseX; // offset=80

    /** Mouse position during move, Y coordinate. */
    long MMouseY; // offset=84

    /** Mouse position during button release, X coordinate. */
    long RMouseX; // offset=88

    /** Mouse position during button release, Y coordinate. */
    long RMouseY; // offset=92
    ushort DrawFlags; // offset=96

    /** Graphic mode at the start of the app.
     * At exit, the app goes back to this mode. */
    ushort OldVideoMode; // offset=98

    /** Actual Screen Mode of the lbDrawTexture, can be same
     * as mode in setting, or lower res when playing a movie. */
    ushort ScreenMode; // offset=100

    /** VESA set-up flag, used only with VBE video modes. */
    ubyte VesaIsSetUp; // offset=102
    ubyte LeftButton; // offset=103
    ubyte RightButton; // offset=104
    ubyte MiddleButton; // offset=105
    ubyte MLeftButton; // offset=106
    ubyte MRightButton; // offset=107
    ubyte MMiddleButton; // offset=108
    ubyte RLeftButton; // offset=109
    ubyte RMiddleButton; // offset=110
    ubyte RRightButton; // offset=111
    ubyte FadeStep; // offset=112

    /** Selected drawing colour index. */
    ubyte DrawColour; // offset=113

    /** Currently active colour palette.
     *  LbPaletteGet() should be used to retrieve a copy of the palette. */
    ubyte *Palette; // offset=114
};

typedef struct ScreenModeInfo TbScreenModeInfo;

struct ScreenModeInfo { // sizeof=38
    /** Hardware driver screen width. */
    ushort Width; // offset=0

    /** Hardware driver screen height. */
    ushort Height; // offset=2

    /** Hardware driver color depth. Not in use since we fixed this to 32, remove when possible.*/
    ushort BitsPerPixel; // offset=4

    /** Is the mode currently available for use. */
    DwBool Available; // offset=6

    /** Video mode flags. Can be Lb_VF_DEFAULT, Lb_VF_PALETTE, Lb_VF_TRUCOLOR, Lb_VF_RGBCOLOR.*/
    long VideoMode; // offset=10

    /** Text description of the mode. */
    char Desc[24]; // offset=14
};

#pragma pack()

extern TbScreenModeInfo lbScreenModeInfo[];
extern TbDisplayStruct lbDisplay;

/** GUI scaling factor, to be used for drawing sprites smaller or larger.
 * For VGA resolution (width=640), the value is 16. It is smaller for lower resolutinons,
 * and larger for higher resolutions.
 */
extern ushort lbUnitsPerPixel;

TbResult LbScreenSetup(TbScreenMode mode, TbScreenCoord width, TbScreenCoord height,
    unsigned char *palette);

/** Graphics screen buffer (WScreen) width.
 * Gives whole buffer dimension, not only the visible part.
 * Acts as pitch (buffer line width) of both graphics screen
 * and graphics window buffer.
 */
TbScreenCoord LbGraphicsScreenWidth(void);

/** Graphics screen buffer (WScreen) height.
 * Gives whole buffer dimension, not only the visible part.
 */
TbScreenCoord LbGraphicsScreenHeight(void);

/** Horizontal resolution (width) of the current video mode.
 *  Note that it's not always "physical" size,
 *  and it definitely can't be used as pitch/scanline
 *  (size of data for one line) in the graphics buffer.
 *
 *  But it is the width that will be visible on screen.
 *
 * @return
 */
TbScreenCoord LbScreenWidth(void);

/** Vertical resolution (height) of the current video mode.
 *  It is the height that will be visible on screen.
 *
 * @return
 */
TbScreenCoord LbScreenHeight(void);

int LbScreenIsModeAvailable();
TbResult LbScreenSetGraphicsWindow(ulong x, ulong y, ulong width, ulong height);

int LbScreenSetupAnyMode(unsigned short mode, unsigned long width,
    unsigned long height, TbPixel *palette);

int LbScreenClearGraphicsWindow();
int LbScreenClear();
int LbScreenReset();
int LbScreenLock();
int LbScreenUnlock();
int LbScreenSetDoubleBuffering();
int LbScreenSetWScreenInVideo();
extern int lbScreenDirectAccessActive;
int LbScreenFindVideoModes();
int LbScreenSwap();
int LbScreenSwapBoxClear();
TbResult LbScreenSwapClear(TbPixel colour);
int LbScreenSwapBox();
int LbScreenDrawHVLineDirect();
int LbScreenWaitVbi();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFSCREEN_H_
/******************************************************************************/
