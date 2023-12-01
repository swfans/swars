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

#define MAX_SUPPORTED_SCREEN_WIDTH  2560
#define MAX_SUPPORTED_SCREEN_HEIGHT 1440

#define LB_MAX_SCREEN_MODES_COUNT 40

/** Standard video modes, registered by LbBaseInitialise().
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

enum TbVideoModeFlags {
    Lb_VF_DEFAULT     = 0x0000, /**< dummy flag, marks no flag toggled */
    Lb_VF_MODE_MASK   = 0x00ff, /**< mask for hardware-specific video mode number */
    Lb_VF_VESA        = 0x0100, /**< marks the mode as VESA, with paged access */
    Lb_VF_RGBCOLOUR   = 0x0200, /**< colours have separate RGB channels (True Colour) */
    Lb_VF_HICOLOUR    = 0x0400, /**< colours have RGB channels not aligned to bytes */
    Lb_VF_PALETTE     = 0x0800, /**< pixel value is just an index in a palette */
    Lb_VF_WINDOWED    = 0x1000, /**< the mode runs in a window rather than full screen */
};

#pragma pack(1)

typedef struct DisplayStruct TbDisplayStruct;

struct DisplayStruct { // sizeof=118
    /** Pointer to physical screen buffer, if available.
     * Not all platforms allow access to physical screen, so this pointer
     * could be always NULL or have access restrictions. It is also not
     * guaranteed to be the real physical screen buffer; it can be just
     * a standard application buffer, the one handed to OS for further
     * compositing and display.
     */
    ubyte *PhysicalScreen; // offset=0

    /** Pointer to graphics screen working buffer.
     * As a working buffer, this can be read and modified to draw the next frame.
     * This pointer can be accessed only when the screen is locked. It is not
     * guaranteed that the pointer will remain unchanged after each locking.
     */
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
#if defined(ENABLE_MOUSE_WHEEL)
    short WheelPosition;
    ushort WheelMoveUp;
    ushort WheelMoveDown;
#endif
#if defined(ENABLE_MOUSE_MOVE_RATIO)
    short MouseMoveRatioX;
    short MouseMoveRatioY;
#endif
#if defined(ENABLE_SHADOW_COLOUR)
    /** Colour index used for drawing shadow. */
    uchar ShadowColour;
#endif
};

typedef struct ScreenModeInfo TbScreenModeInfo;

struct ScreenModeInfo { // sizeof=38
    /** Hardware driver screen width. */
    ushort Width; // offset=0

    /** Hardware driver screen height. */
    ushort Height; // offset=2

    /** OS driver color depth. */
    ushort BitsPerPixel; // offset=4

    /** Is the mode currently available for use. */
    DwBool Available; // offset=6

    /** Video mode flags. Can be combination of Lb_VF_* flags.
     * Lowest byte can be used for OS-specific data.
     */
    ulong VideoMode; // offset=10

    /** Text description of the mode. */
    char Desc[24]; // offset=14
};

typedef struct SurfaceDimensions TbSurfaceDimensions;

struct SurfaceDimensions {
    long Width, Height;
};

typedef const char *(*ResourceMappingFunc)(short);

#pragma pack()

/**
 * List of registered video modes.
 * Any direct use on application side should be replaced with
 * LbScreenGetModeInfo().
 */
extern TbScreenModeInfo lbScreenModeInfo[LB_MAX_SCREEN_MODES_COUNT];

/** Count of used entries in registered video modes list. */
extern long lbScreenModeInfoNum;

extern TbDisplayStruct lbDisplay;
extern DwBool lbScreenInitialised;

/* Used to transform mouse position when screen surface scaling is applied */
extern TbSurfaceDimensions lbScreenSurfaceDimensions;

/** True if we have two surfaces. */
extern TbBool lbHasSecondSurface;

/** GUI scaling factor, to be used for drawing sprites smaller or larger.
 * For VGA resolution (width=640), the value is 16. It is smaller for lower resolutinons,
 * and larger for higher resolutions.
 */
extern ushort lbUnitsPerPixel;

/** Amount of bits per pixel for which the engine is compiled.
 * Basically, it is always 8.
 */
extern ushort lbEngineBPP;

/** True when in a code block where PhysicalScreen is being accessed.
 */
extern volatile TbBool lbScreenDirectAccessActive;

/** Returns info struct for requested screen mode.
 *
 * @return The info struct for given screen mode, or empty
 *     struct for invalid mode if no such mode exists.
 */
TbScreenModeInfo *LbScreenGetModeInfo(TbScreenMode mode);

/** Register a new video mode with given properties.
 *
 * @return The index under which the new Screen mode was registered.
 */
TbScreenMode LbRegisterVideoMode(const char *desc,
    TbScreenCoord width, TbScreenCoord height,
    ushort bpp, ulong flags);

/**
 * Parses video mode description string and registers new mode.
 *
 * @param desc Mode description string, in form of WIDTHxHEIGHTxBPP or WIDTHxHEIGHTwBPP.
 * @return Mode number assigned to the newly created video mode.
 */
TbScreenMode LbRegisterVideoModeString(const char *desc);

/** Set minimal value of screen surface width and height
 *
 *  If an application requests a lower resolution, the library will use
 *  nearest-neighbour scaling to reach the minimum width and height. This
 *  mechanism should be used to rescale games which do not have resolution
 *  change implemented within them - when games are remade well enough to allow
 *  resolution control from config files, it is better to leave this feature
 *  inactive.
 *
 *  The scaling is invisible to the lbDisplay struct and mouse position; that
 *  is, the related width/height/position values behave like there was no
 *  scaling. This means that as the library performs scaling, the application
 *  side does not need to change any behaviour as a result of this call.
 *
 *  If the values is 1, screen surface scaling is always disabled.
 */
TbResult LbScreenSetMinScreenSurfaceDimension(long dim);

/** Set title of the application to be used in target OS.
 */
TbResult LbSetTitle(const char *title);

/** Set icon index of the program.
 *
 * This is the resource index which will be requested in userResourceMapping() call.
 */
TbResult LbSetIcon(short nicon);

TbResult LbSetUserResourceMapping(ResourceMappingFunc func);

/** Updates icon of the application.
 *  This function maps previously set icon index into OS-specific
 *  resource, and applies it to current process.
 *
 * @return If icon was updated, Lb_SUCCESS is returned.
 */
TbResult LbScreenUpdateIcon(void);

TbResult LbScreenSetup(TbScreenMode mode, TbScreenCoord width, TbScreenCoord height,
    ubyte *palette);

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

TbBool LbScreenIsModeAvailable(TbScreenMode mode);

/** Get number of a new video mode modes.
 *
 * Try to find and return mode number for mode with given properties;
 * if not found, register the new mode and return its number.
 */
TbScreenMode LbRegisterVideoMode(const char *desc, TbScreenCoord width,
  TbScreenCoord height, ushort bpp, ulong flags);

/** Get number of a video mode with given string name.
 */
TbScreenMode LbRecogniseVideoModeString(const char *desc);

TbResult LbScreenSetGraphicsWindow(ulong x, ulong y, ulong width, ulong height);

TbResult LbScreenSetupAnyMode(TbScreenMode mode, TbScreenCoord width,
    TbScreenCoord height, ubyte *palette);

/** Fill current graphics window with given colour.
 *
 * This function fills the area pointed by GraphicsWindowPtr and bounded
 * by GraphicsWindow sizes.
 * It requires the pointer to graphics window to be accessible.
 */
TbResult LbScreenClearGraphicsWindow(TbPixel colour);

/** Clear work screen, filling it with given colour.
 *
 *  Fills whole WScreen with given colour.
 *  Requires WScreen pointer to be accessible (screen needs to be locked
 *  during this call).
 */
TbResult LbScreenClear(TbPixel colour);

/** Finalize the work with graphical screen.
 *
 *  This reverses the screen initialization from LbScreenSetupAnyMode().
 *  It frees screen-related buffers, and reverts graphics to previous
 *  screen mode, if neccessary.
 */
TbResult LbScreenReset(void);

/** Locks the graphics screen.
 *
 *  This function gives access to the WScreen pointer, which contains buffer
 *  of size GraphicsScreenWidth x GraphicsScreenHeight.
 *  It also allows accessing GraphicsWindowPtr buffer, of size
 *  GraphicsWindowWidth x GraphicsWindowHeight, but with pitch (scanline length)
 *   same as graphics screen (which is GraphicsScreenWidth).
 *
 * @return Lb_SUCCESS if the lock was successful.
 * @see LbScreenUnlock()
 */
TbResult LbScreenLock(void);
TbResult LbScreenUnlock(void);
TbBool LbScreenIsLocked(void);

/** Allows to change recommended state of double buffering functionality.
 *
 *  Double buffering is a technique where two graphics surfaces are used,
 *  and every screen redraw (flip) switches the primary and secondary surface.
 *  This may produce smoother motion on some platforms, but it forces
 *  the screen to be redrawn completely after each switch - if only
 *  changed areas were to be updated, they would have to be updated on both
 *  surfaces.
 *
 * @param state Recommended state of Double Buffering in next video mode switch.
 * @return Lb_SUCCESS if the request has been noted and stored.
 */
TbResult LbScreenSetDoubleBuffering(TbBool state);

/** Retrieves actual state of the Double Buffering function.
 *  Note that if the function was requested, it still doesn't necessarily
 *  mean it was activated.
 *
 * @return True if the function is currently active, false otherwise.
 */
TbBool LbScreenIsDoubleBufferred(void);


/** Sets new value of a flag field, and returns previous one.
 *  Doesn't make much sense, as the value is not used for anything.
 *  But still, this is a part of bflibrary API. So here we are.
 *
 * @return Value of the flag which was given to previous call.
 */
ulong LbScreenSetWScreenInVideo(ulong flag);

/** Updates video modes info, setting availability of each mode.
 * @return Lb_SUCCESS if there is at least one mode available.
 */
TbResult LbScreenFindVideoModes(void);

/** Places working screen buffer to the physical screen.
 *
 * This call causes working screen buffer (WScreen) content to be placed
 * on physical screen. The exact mechanism to achieve that depends on a platform,
 * the WScreen can be just copied to PhysicalScreen, or blitted using graphics
 * hardware, or buffer pointers can be swapped making different one visible.
 *
 * Regardless of the method used, the current content of WScreen lands on the
 * display, and WScreen can be further used to draw the next frame.
 * Screen needs to be unlocked before performing this operation, and re-locked
 * before WScreen pointer is accessed again.
 */
TbResult LbScreenSwap(void);

TbResult LbScreenSwapClear(TbPixel colour);

/** Updates a rectangle of working screen and places it to the physical screen.
 *
 * Allows to blit a rectangle from application-controlled buffer to WScreen,
 * and then display the updated image on physical screen.
 * A bit wierd functionality, rarely (never?) used in games. Originally it
 * skipped the WScreen and drawn directly on physical; but that would mean
 * mouse cursor was not part of the update, so this implementation does
 * update WScreen first.
 */
TbResult LbScreenSwapBox(ubyte *sourceBuf, long sourceX, long sourceY,
  long destX, long destY, ulong width, ulong height);

TbResult LbScreenSwapBoxClear(ubyte *sourceBuf, long sourceX, long sourceY,
  long destX, long destY, ulong width, ulong height, ubyte colour);

/** Draws either horizonal or vertical line directly on physical screen.
 * The line colour is previous colour of each pixel shifted by 128.
 *
 * Allows draw a line without changing WScreen.
 * A bit wierd functionality, rarely (never?) used in games.
 */
TbResult LbScreenDrawHVLineDirect(long X1, long Y1, long X2, long Y2);

/** Wait for vertical blank interrupt.
 *
 * Used to achieve vblank sychronization. May do nothing on some platforms.
 */
TbResult LbScreenWaitVbi(void);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFSCREEN_H_
/******************************************************************************/
