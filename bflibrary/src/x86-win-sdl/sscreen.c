/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sscreen.cpp
 *     Implementation of related functions.
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
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include "bfconfig.h"
#include "bfscreen.h"

#include "bfscrsurf.h"
#include "bfpalette.h"
#include "bfmouse.h"
#include "bftext.h"
#include "bfutility.h"
#include "bfexe_key.h"
#include "bflog.h"

#define to_SDLSurf(h) ((SDL_Surface  *)h)

extern char lbDrawAreaTitle[128];
extern short lbIconIndex;
extern ResourceMappingFunc userResourceMapping;

/** @internal
 *  Minimal value of dimensions in physical resolution.
 *
 *  On a try to setup lower resolution, the library will use pixel doubling
 *  to reach the minimal size for both dimensions.
 *
 *  If the values is 1, phyical screen doubling is always disabled.
 */
long lbMinPhysicalScreenResolutionDim = 1;

/** @internal
 * Physical resolution multiplier.
 * Pixel doubling factor used to achieve minimal resolution.
 * If set to 1, no doubling is applied.
 */
long lbPhysicalResolutionMul = 1;

/** @internal
 * Informs if separate screen and draw surfaces were created
 * during LbScreenSetup().
 */
TbBool lbHasSecondSurface = false;

//int lbScreenDirectAccessActive;

/** Bytes per pixel expected by the engine.
 * On any try of entering different video BPP, this mode will be emulated. */
ushort lbEngineBPP = 8;

static inline void *LbI_XMemCopy(void *dest, void *source, ulong len)
{
    ulong remain;
    ubyte *s;
    ubyte *d;
    s = (ubyte *)source;
    d = (ubyte *)dest;
    for (remain = len >> 2; remain != 0; remain--)
    {
        *(ulong *)d = *(ulong *)s;
        d += 4;
        s += 4;
    }
    return dest;
}

static inline void *LbI_XMemCopyAndSet(void *dest, void *source, ulong val, ulong len)
{
    ulong remain;
    ubyte *s;
    ubyte *d;
    s = (ubyte *)source;
    d = (ubyte *)dest;
    for (remain = len >> 2; remain != 0; remain--)
    {
        *(ulong *)d = *(ulong *)s;
        *(ulong *)s = val;
        d += 4;
        s += 4;
    }
    return dest;
}

#if defined(WIN32)

TbResult LbScreenUpdateIcon(void)
{
    HICON hIcon;
    HINSTANCE lbhInstance;
    SDL_SysWMinfo wmInfo;
    const char * rname;

    SDL_VERSION(&wmInfo.version);
    if (SDL_GetWMInfo(&wmInfo) < 0) {
        LIBLOG("Cannot set icon: Get SDL window info failed: %s", SDL_GetError());
        return Lb_FAIL;
    }

    rname = userResourceMapping(lbIconIndex);
    if (rname != NULL) {
        lbhInstance = GetModuleHandle(NULL);
        hIcon = LoadIcon(lbhInstance, rname);
    } else {
        LIBLOG("Cannot set icon: Resource mapped to NULL");
        return Lb_FAIL;
    }
    SendMessage(wmInfo.window, WM_SETICON, ICON_BIG,  (LPARAM)hIcon);
    SendMessage(wmInfo.window, WM_SETICON, ICON_SMALL,(LPARAM)hIcon);

    return Lb_SUCCESS;
}

#else

TbResult LbScreenUpdateIcon(void)
{
    Uint32          colorkey;
    SDL_Surface     *image;
    const char * rname;

    rname = userResourceMapping(lbIconIndex);
    if (rname != NULL) {
        image = SDL_LoadBMP(rname);
    } else {
        image = NULL;
    }
    if (image == NULL) {
        LIBLOG("Cannot set icon: Image load failed: %s", SDL_GetError());
        return Lb_FAIL;
    }
    colorkey = SDL_MapRGB(image->format, 255, 0, 255);
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey);
    SDL_WM_SetIcon(image, NULL);

    return Lb_SUCCESS;
}

#endif

TbResult LbScreenSetMinPhysicalScreenResolution(long dim)
{
    lbMinPhysicalScreenResolutionDim = dim;
    return Lb_SUCCESS;
}

TbResult LbScreenSetupAnyMode(TbScreenMode mode, TbScreenCoord width,
    TbScreenCoord height, ubyte *palette)
{
    SDL_Surface * prevScreenSurf;
    long hot_x, hot_y;
    long mdWidth, mdHeight;
    const struct TbSprite *msspr;
    TbScreenModeInfo *mdinfo;
    unsigned long sdlFlags;

    msspr = NULL;
    LbExeReferenceNumber();
    if (lbDisplay.MouseSprite != NULL) {
        msspr = lbDisplay.MouseSprite;
        LbMouseGetSpriteOffset(&hot_x, &hot_y);
    }
    prevScreenSurf = to_SDLSurf(lbScreenSurface);
    LbMouseChangeSprite(NULL);
    if (lbHasSecondSurface) {
        SDL_FreeSurface(to_SDLSurf(lbDrawSurface));
        lbHasSecondSurface = false;
    }
    lbDrawSurface = NULL;
    lbScreenInitialised = false;

    if (prevScreenSurf != NULL) {
    }

    // No need for old video mode in window-based environments
    if (lbDisplay.OldVideoMode == 0)
        lbDisplay.OldVideoMode = 0xFF;

    mdinfo = LbScreenGetModeInfo(mode);
    if (!LbScreenIsModeAvailable(mode)) {
        LIBLOG("%s resolution %dx%d (mode %d) not available",
            (mdinfo->VideoMode & Lb_VF_WINDOWED) ? "Windowed" : "Full screen",
            (int)mdinfo->Width, (int)mdinfo->Height, (int)mode);
        return Lb_FAIL;
    }

    {
        long minDim = min(mdinfo->Width,mdinfo->Height);
        if ((minDim != 0) && (minDim < lbMinPhysicalScreenResolutionDim)) {
            lbPhysicalResolutionMul = (lbMinPhysicalScreenResolutionDim + minDim - 1) / minDim;
        } else {
            lbPhysicalResolutionMul = 1;
        }
        mdWidth = mdinfo->Width * lbPhysicalResolutionMul;
        mdHeight = mdinfo->Height * lbPhysicalResolutionMul;
    }


    // No need for video buffer paging when using SDL
    lbDisplay.VesaIsSetUp = false;

    // SDL video mode flags
    sdlFlags = 0;
    sdlFlags |= SDL_SWSURFACE;
    if (mdinfo->BitsPerPixel == lbEngineBPP) {
        sdlFlags |= SDL_HWPALETTE;
    }
#if 0
    if (lbDoubleBufferingRequested) {
        sdlFlags |= SDL_DOUBLEBUF;
    }
#endif
    if ((mdinfo->VideoMode & Lb_VF_WINDOWED) == 0) {
        sdlFlags |= SDL_FULLSCREEN;
    }

    // Set SDL video mode (also creates window).
    lbScreenSurface = lbDrawSurface = SDL_SetVideoMode(mdWidth, mdHeight,
      mdinfo->BitsPerPixel, sdlFlags);

    if (lbScreenSurface == NULL) {
        LIBLOG("Failed to initialize mode %d: %s", (int)mode, SDL_GetError());
        return Lb_FAIL;
    }

    SDL_WM_SetCaption(lbDrawAreaTitle, lbDrawAreaTitle);
    LbScreenUpdateIcon();

    // Create secondary surface if necessary,
    // that is if BPP or dimensions do not match.
    if ((mdinfo->BitsPerPixel != lbEngineBPP) ||
        (mdWidth != width) || (mdHeight != height))
    {
        lbDrawSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
          width, height, lbEngineBPP, 0, 0, 0, 0);
        if (lbDrawSurface == NULL) {
            LIBLOG("Secondary surface creation error: %s", SDL_GetError());
            LbScreenReset();
            return Lb_FAIL;
        }
        lbHasSecondSurface = true;
    }

    lbDisplay.DrawFlags = 0;
    lbDisplay.DrawColour = 0;
#if defined(ENABLE_SHADOW_COLOUR)
    lbDisplay.ShadowColour = 0;
#endif
    lbDisplay.PhysicalScreenWidth = mdinfo->Width;
    lbDisplay.PhysicalScreenHeight = mdinfo->Height;
    lbDisplay.ScreenMode = mode;
    lbDisplay.PhysicalScreen = NULL;
    // The graphics screen size should be really taken after screen is locked
    lbDisplay.GraphicsScreenWidth  = width;
    lbDisplay.GraphicsScreenHeight = height;

#if defined(BFLIB_WSCREEN_CONTROL)
    lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;
#endif

    lbScreenInitialised = true;
    LIBLOG("Mode %dx%dx%d setup succeeded", (int)to_SDLSurf(lbScreenSurface)->w,
      (int)to_SDLSurf(lbScreenSurface)->h,
      (int)to_SDLSurf(lbScreenSurface)->format->BitsPerPixel);
    if (palette != NULL)
    {
        TbResult ret;
        ret = LbPaletteSet(palette);
        if (ret != Lb_SUCCESS) {
            LIBLOG("Palette setting failed");
            LbScreenReset();
            return Lb_FAIL;
        }
    }
    LbScreenSetGraphicsWindow(0, 0, mdinfo->Width, mdinfo->Height);
    LbTextSetWindow(0, 0, mdinfo->Width, mdinfo->Height);
    LIBLOG("Done filling display properties struct");
    if ( LbMouseIsInstalled() )
    {
        LbMouseSetWindow(0, 0, lbDisplay.PhysicalScreenWidth, lbDisplay.PhysicalScreenHeight);
        LbMouseSetPosition(lbDisplay.PhysicalScreenWidth / 2, lbDisplay.PhysicalScreenHeight / 2);
        LbMouseChangeSpriteOffset(hot_x, hot_y);
        if (msspr != NULL)
          LbMouseChangeSprite(msspr);
    }
#if 0
    LbInputRestate();
    LbScreenActivationUpdate();
#endif
    LIBLOG("Finished");
    return Lb_SUCCESS;
}

int LbScreenClearGraphicsWindow_TODO()
{
// code at 0001:000956dc
}

int LbScreenClear_TODO()
{
// code at 0001:00095728
}

TbResult LbScreenReset(void)
{
    if (!lbScreenInitialised)
      return Lb_FAIL;

    LbMouseSuspend();
    if (lbDisplay.WScreen != NULL) {
        LIBLOG("Screen got reset while locked");
        LbScreenUnlock();
    }
    if (lbHasSecondSurface) {
        SDL_FreeSurface(to_SDLSurf(lbDrawSurface));
    }
    // do not free screen surface, it is freed automatically
    // on SDL_Quit or next call to set video mode
    lbHasSecondSurface = false;
    lbDrawSurface = NULL;
    lbScreenSurface = NULL;
    // Mark as not initialized
    lbScreenInitialised = false;

    return Lb_SUCCESS;
}

TbBool LbScreenIsLocked(void)
{
#if defined(BFLIB_WSCREEN_CONTROL)
    return (lbDisplay.WScreen != NULL);
#else
    return (lbDisplay.WScreen != NULL) && (to_SDLSurf(lbDrawSurface)->pixels != NULL);
#endif
}

TbResult LbScreenLock(void)
{
    LIBLOG("Starting");
    if (!lbScreenInitialised)
        return Lb_FAIL;

    if (SDL_MUSTLOCK(to_SDLSurf(lbDrawSurface))) {
        if (SDL_LockSurface(to_SDLSurf(lbDrawSurface)) < 0) {
            LIBLOG("error: SDL Lock Draw Surface: %s", SDL_GetError());
#if defined(BFLIB_WSCREEN_CONTROL)
            lbDisplay.GraphicsWindowPtr = NULL;
            lbDisplay.WScreen = NULL;
#endif
            return Lb_FAIL;
        }
    }

#if defined(BFLIB_WSCREEN_CONTROL)
    lbDisplay.WScreen = (ubyte *) to_SDLSurf(lbDrawSurface)->pixels;
    lbDisplay.GraphicsScreenWidth = to_SDLSurf(lbDrawSurface)->pitch;
    lbDisplay.GraphicsWindowPtr = &lbDisplay.WScreen[lbDisplay.GraphicsWindowX +
        lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsWindowY];
#endif

    return Lb_SUCCESS;
}

TbResult LbScreenUnlock(void)
{
    LIBLOG("Starting");
    if (!lbScreenInitialised)
        return Lb_FAIL;

#if defined(BFLIB_WSCREEN_CONTROL)
    lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;
#endif

    if (SDL_MUSTLOCK(to_SDLSurf(lbDrawSurface)))
        SDL_UnlockSurface(to_SDLSurf(lbDrawSurface));

    return Lb_SUCCESS;
}

/** Locks physical screen surface and allows access to its pixels.
 */
static TbResult LbIPhysicalScreenLock(void)
{
    if (lbHasSecondSurface && SDL_MUSTLOCK(to_SDLSurf(lbScreenSurface))) {
        if (SDL_LockSurface (to_SDLSurf(lbScreenSurface)) != 0) {
            LIBLOG("error: SDL Lock Screen Surface: %s", SDL_GetError());
            return Lb_FAIL;
        }
    }
    // set vga buffer address
    lbDisplay.PhysicalScreen = (ubyte *)to_SDLSurf(lbScreenSurface)->pixels;

}

/** Unlocks physical screen surface ending access to its pixels.
 */
static TbResult LbIPhysicalScreenUnlock(void)
{
    lbDisplay.PhysicalScreen = NULL;
    if (lbHasSecondSurface && SDL_MUSTLOCK(to_SDLSurf(lbScreenSurface))) {
        SDL_UnlockSurface(to_SDLSurf(lbScreenSurface));
    }
}

int LbScreenSetDoubleBuffering_TODO()
{
// code at 0001:000957b8
}

int LbScreenSetWScreenInVideo_TODO()
{
// code at 0001:000957d8
}

TbResult LbScreenFindVideoModes(void)
{
    TbScreenModeInfo *mdinfo;
    int i;

    for (i = 1; lbScreenModeInfo[i].Width; i++)
    {
        mdinfo = &lbScreenModeInfo[i];
        /* No restrictions on modes at this time */
        mdinfo->Available = 1;
    }
    return Lb_SUCCESS;
}

TbResult LbScreenSwap(void)
{
    TbResult ret;

    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

    LbIPhysicalScreenLock();
    LbMousePlace();
#if defined(BFLIB_WSCREEN_CONTROL)
    LbScreenLock();
    // TODO use blit if we have wscreen control
    LbScreenUnlock();
#else
    {
        ulong blremain;
        blremain = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        //TODO copy to lbDisplay.PhysicalScreen when possible
        // currently lbDrawSurface is copied to lbScreenSurface during display_update()
        LbI_XMemCopy(to_SDLSurf(lbDrawSurface)->pixels, lbDisplay.WScreen, blremain);
        ret = Lb_SUCCESS;
    }
#endif
    LbMouseRemove();
    LbIPhysicalScreenUnlock();
/*
    int blresult;
    LIBLOG("Starting");
    ret = LbMouseOnBeginSwap();
    // Put the data from Draw Surface onto Screen Surface
    if ((ret == Lb_SUCCESS) && (lbHasSecondSurface)) {
        blresult = SDL_BlitSurface(to_SDLSurf(lbDrawSurface), NULL,
          to_SDLSurf(lbScreenSurface), NULL);
        if (blresult < 0) {
            LIBLOG("Blit failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    // Flip the image displayed on Screen Surface
    if (ret == Lb_SUCCESS) {
        // calls SDL_UpdateRect for entire screen if not double buffered
        blresult = SDL_Flip(to_SDLSurf(lbScreenSurface));
        if (blresult < 0) {
            // In some cases this situation seems to be quite common
            LIBLOG("Flip failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    LbMouseOnEndSwap();
*/
    return ret;
}

TbResult LbScreenSwapClear(TbPixel colour)
{
    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

    LbIPhysicalScreenLock();
    LbMousePlace();
#if defined(BFLIB_WSCREEN_CONTROL)
    LbScreenLock();
    // TODO use blit if we have wscreen control
    LbScreenUnlock();
#else
    {
        int blsize;
        ubyte *blsrcbuf;
        blsrcbuf = lbDisplay.WScreen;
        blsize = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        //TODO copy to lbDisplay.PhysicalScreen when possible
        // currently lbDrawSurface is copied to lbScreenSurface during display_update()
        LbI_XMemCopyAndSet(to_SDLSurf(lbDrawSurface)->pixels, blsrcbuf, 0x01010101 * colour, blsize);
    }
#endif
    LbMouseRemove();
    LbIPhysicalScreenUnlock();
    return Lb_SUCCESS;
}

int LbScreenSwapBox_TODO()
{
// code at 0001:00095c38
}

int LbScreenSwapBoxClear_TODO()
{
// code at 0001:00095964
}

int LbScreenDrawHVLineDirect_TODO()
{
// code at 0001:00095dc4
}

TbResult LbScreenWaitVbi_TODO(void)
{
    // No need to wait for vblank when using SDL
    return Lb_SUCCESS;
}

/******************************************************************************/
