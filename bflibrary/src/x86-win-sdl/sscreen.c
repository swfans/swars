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
#include "bfwindows.h"
#include "bfutility.h"
#include "bfexe_key.h"
#include "privbflog.h"

#define to_SDLSurf(h) ((SDL_Surface  *)h)

#define CLIP_START_END_COORDS(cor1, cor2, corlimit) \
    if (cor1 > cor2) \
    { \
        if (cor1 < 0) \
            return Lb_FAIL; \
        if (cor2 > corlimit) \
            return Lb_FAIL; \
        if (cor2 < 0) \
            cor2 = 0; \
        if (cor1 > corlimit) \
            cor1 = corlimit; \
    } else \
    { \
        if (cor2 < 1) \
            return Lb_FAIL; \
        if (corlimit - 1 < cor1) \
            return Lb_FAIL; \
        if (cor1 < 0) \
            cor1 = 0; \
        if (cor2 > corlimit) \
            cor2 = corlimit; \
    }


extern char lbDrawAreaTitle[128];
extern short lbIconIndex;
extern ResourceMappingFunc userResourceMapping;
extern volatile TbBool lbPointerAdvancedDraw;

volatile TbBool lbScreenDirectAccessActive = false;

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

/** @internal
 *  True if we request the double buffering to be on in next mode switch.
 */
TbBool lbDoubleBufferingRequested = false;

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

static inline void *LbI_XMemRectCopy(void *dest, void *source, ulong lineLen,
  ulong width, ulong height)
{
    ulong remainH, remainW;
    ubyte *s;
    ubyte *d;
    s = (ubyte *)source;
    d = (ubyte *)dest;

    for (remainH = height; remainH != 0; remainH--)
    {
        for (remainW = width >> 2; remainW != 0; remainW--)
        {
            *(ulong *)d = *(ulong *)s;
            d += 4;
            s += 4;
        }
        for (remainW = width & 0x3; remainW != 0; remainW--)
        {
            *d = *s;
            d ++;
            s ++;
        }
        s += lineLen - width;
        d += lineLen - width;
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
        LOGWARN("cannot set icon: get SDL window info failed: %s", SDL_GetError());
        return Lb_FAIL;
    }

    rname = userResourceMapping(lbIconIndex);
    if (rname != NULL) {
        lbhInstance = GetModuleHandle(NULL);
        hIcon = LoadIcon(lbhInstance, rname);
    } else {
        LOGWARN("cannot set icon: resource mapped to NULL");
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
        LOGWARN("cannot set icon: image load failed: %s", SDL_GetError());
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

/** @internal
 * Registers standard VESA video modes.
 *
 * The modes have to registered to an empty list of modes, to make index of each mode
 * match the mode index in VESA standard.
 * Non-standard modes may be register later, getting mode numbers higher than standard modes.
 */
void LbRegisterStandardVideoModes(void)
{
    lbScreenModeInfoNum = 0;
    LbRegisterVideoMode("INVALID",       0,    0,  0, Lb_VF_DEFAULT);
    LbRegisterVideoMode("320x200x8",   320,  200,  8, Lb_VF_PALETTE | 19);
    LbRegisterVideoMode("320x200x16",  320,  200, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("320x200x24",  320,  200, 24, Lb_VF_RGBCOLOUR);
    LbRegisterVideoMode("320x240x8",   320,  240,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("320x240x16",  320,  240, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("320x240x24",  320,  240, 24, Lb_VF_RGBCOLOUR);
    LbRegisterVideoMode("512x384x8",   512,  384,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("512x384x16",  512,  384, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("512x384x24",  512,  384, 24, Lb_VF_RGBCOLOUR | Lb_VF_VESA | 0);
    LbRegisterVideoMode("640x400x8",   640,  400,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("640x400x16",  640,  400, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("640x400x24",  640,  400, 24, Lb_VF_RGBCOLOUR | Lb_VF_VESA | 1);
    LbRegisterVideoMode("640x480x8",   640,  480,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("640x480x16",  640,  480, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("640x480x24",  640,  480, 24, Lb_VF_RGBCOLOUR | Lb_VF_VESA | 3);
    LbRegisterVideoMode("800x600x8",   800,  600,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("800x600x16",  800,  600, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("800x600x24",  800,  600, 24, Lb_VF_RGBCOLOUR | Lb_VF_VESA | 5);
    LbRegisterVideoMode("1024x768x8", 1024,  768,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("1024x768x16",1024,  768, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("1024x768x24",1024,  768, 24, Lb_VF_RGBCOLOUR | Lb_VF_VESA | 7);
    LbRegisterVideoMode("1280x1024x8", 1280,1024,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("1280x1024x16",1280,1024, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("1280x1024x24",1280,1024, 24, Lb_VF_RGBCOLOUR);
    LbRegisterVideoMode("1600x1200x8", 1600,1200,  8, Lb_VF_PALETTE);
    LbRegisterVideoMode("1600x1200x16",1600,1200, 16, Lb_VF_HICOLOUR);
    LbRegisterVideoMode("1600x1200x24",1600,1200, 24, Lb_VF_RGBCOLOUR);
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
    if (!lbLibInitialised) {
        TbResult result;
        result = LbBaseInitialise();
        if (result != Lb_SUCCESS)
            return result;
    }

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
        LOGERR("%s resolution %dx%d (mode %d) not available",
            (mdinfo->VideoMode & Lb_VF_WINDOWED) ? "windowed" : "full screen",
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
        LOGDBG("physical resolution multiplier %ld", lbPhysicalResolutionMul);
    }


    // No need for video buffer paging when using SDL
    lbDisplay.VesaIsSetUp = false;

    // SDL video mode flags
    sdlFlags = 0;
    if (mdinfo->BitsPerPixel == lbEngineBPP) {
        sdlFlags |= SDL_HWPALETTE;
    }
    if (lbDoubleBufferingRequested) {
        // Only hardware surfaces can use double buffering
        sdlFlags |= SDL_HWSURFACE;
        sdlFlags |= SDL_DOUBLEBUF;
    } else {
        sdlFlags |= SDL_SWSURFACE;
    }
    if ((mdinfo->VideoMode & Lb_VF_WINDOWED) == 0) {
        sdlFlags |= SDL_FULLSCREEN;
    }

    // Set SDL video mode (also creates window).
    lbScreenSurface = lbDrawSurface = SDL_SetVideoMode(mdWidth, mdHeight,
      mdinfo->BitsPerPixel, sdlFlags);

    if (lbScreenSurface == NULL) {
        LOGERR("failed to initialize mode %d: %s", (int)mode, SDL_GetError());
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
            LOGERR("secondary surface creation error: %s", SDL_GetError());
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
    LOGSYNC("mode %dx%dx%d setup succeeded", (int)to_SDLSurf(lbScreenSurface)->w,
      (int)to_SDLSurf(lbScreenSurface)->h,
      (int)to_SDLSurf(lbScreenSurface)->format->BitsPerPixel);
    if (palette != NULL)
    {
        TbResult ret;
        ret = LbPaletteSet(palette);
        if (ret != Lb_SUCCESS) {
            LOGERR("palette setting failed");
            LbScreenReset();
            return Lb_FAIL;
        }
    }
    LbScreenSetGraphicsWindow(0, 0, mdinfo->Width, mdinfo->Height);
    LbTextSetWindow(0, 0, mdinfo->Width, mdinfo->Height);
    LOGDBG("done filling display properties struct");
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
    LOGDBG("finished");
    return Lb_SUCCESS;
}

int LbScreenClearGraphicsWindow_UNUSED()
{
// code at 0001:000956dc
}

int LbScreenClear_UNUSED()
{
// code at 0001:00095728
}

TbResult LbScreenReset(void)
{
    if (!lbScreenInitialised)
      return Lb_FAIL;

    LbMouseSuspend();
    if (LbScreenIsLocked()) {
        LOGWARN("screen got reset while locked");
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
    LOGSYNC("screen reset finished");

    return Lb_SUCCESS;
}

TbBool LbScreenIsLocked(void)
{
#if defined(BFLIB_WSCREEN_CONTROL)
    return (lbDisplay.WScreen != NULL) && (lbScreenInitialised)
      && (lbDrawSurface != NULL);
#else
    return (lbDisplay.WScreen != NULL) && (lbScreenInitialised)
      && (lbDrawSurface != NULL)
      && (to_SDLSurf(lbDrawSurface)->pixels != NULL);
#endif
}

TbResult LbScreenLock(void)
{
    LOGDBG("starting");
    if (!lbScreenInitialised)
        return Lb_FAIL;

    if (SDL_MUSTLOCK(to_SDLSurf(lbDrawSurface))) {
        if (SDL_LockSurface(to_SDLSurf(lbDrawSurface)) < 0) {
            LOGERR("cannot lock draw Surface: %s", SDL_GetError());
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
    LOGDBG("starting");
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
            LOGERR("cannot lock screen surface: %s", SDL_GetError());
            return Lb_FAIL;
        }
    }
    // set vga buffer address
    lbDisplay.PhysicalScreen = (ubyte *)to_SDLSurf(lbScreenSurface)->pixels;
    lbScreenDirectAccessActive = true;

    return Lb_SUCCESS;
}

/** Unlocks physical screen surface ending access to its pixels.
 */
static TbResult LbIPhysicalScreenUnlock(void)
{
    lbScreenDirectAccessActive = false;
    lbDisplay.PhysicalScreen = NULL;
    if (lbHasSecondSurface && SDL_MUSTLOCK(to_SDLSurf(lbScreenSurface))) {
        SDL_UnlockSurface(to_SDLSurf(lbScreenSurface));
    }
    return Lb_SUCCESS;
}

TbResult LbScreenSetDoubleBuffering(TbBool state)
{
    lbDoubleBufferingRequested = state;
    return Lb_SUCCESS;
}

TbBool LbScreenIsDoubleBufferred(void)
{
    return ((to_SDLSurf(lbScreenSurface)->flags & SDL_DOUBLEBUF) != 0);
}

int LbScreenSetWScreenInVideo_UNUSED()
{
// code at 0001:000957d8
}

/** @internal
 * Implements checking whether a specific video mode is available.
 */
TbBool LbHwCheckIsModeAvailable(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    ulong sdlFlags;
    long minDim;
    int closestBPP;

    mdinfo = LbScreenGetModeInfo(mode);
    sdlFlags = 0;
    if (mdinfo->BitsPerPixel == lbEngineBPP) {
        sdlFlags |= SDL_HWPALETTE;
    }
    if (lbDoubleBufferingRequested) {
        sdlFlags |= SDL_HWSURFACE;
        sdlFlags |= SDL_DOUBLEBUF;
    } else {
        sdlFlags |= SDL_SWSURFACE;
    }
    if ((mdinfo->VideoMode & Lb_VF_WINDOWED) == 0) {
        sdlFlags |= SDL_FULLSCREEN;
    }

    minDim = min(mdinfo->Width,mdinfo->Height);
    if ((minDim != 0) && (minDim < lbMinPhysicalScreenResolutionDim)) {
        lbPhysicalResolutionMul =
          (lbMinPhysicalScreenResolutionDim + minDim - 1) / minDim;
        //TODO search for a matching mode instead
        return true;
    } else {
        lbPhysicalResolutionMul = 1;
    }

    closestBPP = SDL_VideoModeOK(mdinfo->Width, mdinfo->Height,
      mdinfo->BitsPerPixel, sdlFlags);

    // Even if different colour depth is returned, as long as the value is
    // non-zero, SDL can simulate any bpp with additional internal surface
    return (closestBPP != 0);
}

/** @internal
 * Provides simplified SDL_BlitScaled() functionality for SDL1.
 */
int LbI_SDL_BlitScaled(SDL_Surface *src, SDL_Surface *dst, int resolutionMul)
{
    long i, j;
    long mdWidth, mdHeight;

    LbScreenLock();
    LbIPhysicalScreenLock();

    ubyte *poutput = (ubyte *)dst->pixels;
    ubyte *pinput  = (ubyte *)src->pixels;

    mdWidth = lbDisplay.PhysicalScreenWidth;
    mdHeight = lbDisplay.PhysicalScreenHeight;

    for (j = 0; j < mdHeight; j++)
    {
        for (i = 0; i < mdWidth; i++)
        {
            long di, dj;
            int input_xy = j * mdWidth + i;

            for (dj = 0; dj < resolutionMul; dj++)
            {
                int output_xy = (j*resolutionMul+dj) *
                  mdWidth*resolutionMul +
                  i*resolutionMul;

                for (di = 0; di < resolutionMul; di++) {
                    poutput[output_xy++] = pinput[input_xy];
                }
            }
        }
    }

    LbIPhysicalScreenUnlock();
    LbScreenUnlock();
    return 0;
}

TbResult LbScreenSwap(void)
{
    TbResult ret;
    int blresult;

    LOGDBG("starting");
    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

#if defined(BFLIB_WSCREEN_CONTROL)
    // Blit the cursor on Draw Surface; simple if we have WScreen control
    LbMouseOnBeginSwap();
    ret = Lb_SUCCESS;
#else
    // Non-advanced cursor is drawn on WScreen pixels, so should be drawn here
    if (!lbPointerAdvancedDraw)
        LbMouseOnBeginSwap();

    ret = LbScreenLock();
    // If WScreen is application-controlled buffer, copy it to SDL surface
    if (ret == Lb_SUCCESS) {
        ulong blsize;
        blsize = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        LbI_XMemCopy(to_SDLSurf(lbDrawSurface)->pixels, lbDisplay.WScreen, blsize);
        ret = Lb_SUCCESS;
    }
    LbScreenUnlock();

    // Advanced cursor is blitted on lbDrawSurface, so should be drawn here
    if (lbPointerAdvancedDraw)
        LbMouseOnBeginSwap();
#endif

    // Put the data from Draw Surface onto Screen Surface
    if ((ret == Lb_SUCCESS) && (lbHasSecondSurface))
    {
        if (lbPhysicalResolutionMul > 1) {
            blresult = LbI_SDL_BlitScaled(to_SDLSurf(lbDrawSurface),
              to_SDLSurf(lbScreenSurface), lbPhysicalResolutionMul);
        } else {
            blresult = SDL_BlitSurface(to_SDLSurf(lbDrawSurface), NULL,
              to_SDLSurf(lbScreenSurface), NULL);
        }
        if (blresult < 0) {
            LOGERR("blit failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    // Flip the image displayed on Screen Surface
    if (ret == Lb_SUCCESS) {
        // calls SDL_UpdateRect for entire screen if not double buffered
        blresult = SDL_Flip(to_SDLSurf(lbScreenSurface));
        if (blresult < 0) {
            // In some cases this situation seems to be quite common
            LOGERR("flip failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    LbMouseOnEndSwap();
    return ret;
}

TbResult LbScreenSwapClear(TbPixel colour)
{
    TbResult ret;
    int blresult;

    LOGDBG("starting");
    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

#if defined(BFLIB_WSCREEN_CONTROL)
    // Blit the cursor on Draw Surface; simple if we have WScreen control
    LbMouseOnBeginSwap();
    ret = Lb_SUCCESS;
#else
    // Non-advanced cursor is drawn on WScreen pixels, so should be drawn here
    if (!lbPointerAdvancedDraw)
        LbMouseOnBeginSwap();

    ret = LbScreenLock();
    // If WScreen is application-controlled buffer, copy it to SDL surface
    if (ret == Lb_SUCCESS) {
        ulong blsize;
        ubyte *blsrcbuf;
        blsrcbuf = lbDisplay.WScreen;
        blsize = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        LbI_XMemCopyAndSet(to_SDLSurf(lbDrawSurface)->pixels,
          blsrcbuf, 0x01010101 * colour, blsize);
        ret = Lb_SUCCESS;
    }
    LbScreenUnlock();

    // Advanced cursor is blitted on lbDrawSurface, so should be drawn here
    if (lbPointerAdvancedDraw)
        LbMouseOnBeginSwap();
#endif

    // Put the data from Draw Surface onto Screen Surface
    if ((ret == Lb_SUCCESS) && (lbHasSecondSurface))
    {
        if (lbPhysicalResolutionMul > 1) {
            blresult = LbI_SDL_BlitScaled(to_SDLSurf(lbDrawSurface),
              to_SDLSurf(lbScreenSurface), lbPhysicalResolutionMul);
        } else {
            blresult = SDL_BlitSurface(to_SDLSurf(lbDrawSurface), NULL,
              to_SDLSurf(lbScreenSurface), NULL);
        }
        if (blresult < 0) {
            LOGERR("blit failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    // Flip the image displayed on Screen Surface
    if (ret == Lb_SUCCESS) {
        // calls SDL_UpdateRect for entire screen if not double buffered
        blresult = SDL_Flip(to_SDLSurf(lbScreenSurface));
        if (blresult < 0) {
            // In some cases this situation seems to be quite common
            LOGERR("flip failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    LbMouseOnEndSwap();
#if defined(BFLIB_WSCREEN_CONTROL)
    SDL_FillRect(lbDrawSurface, NULL, colour);
#endif
    return ret;
}

TbResult LbScreenSwapBox(ubyte *sourceBuf, long sourceX, long sourceY,
  long destX, long destY, ulong width, ulong height)
{
    TbResult ret;
    int blresult;

    LOGDBG("starting");
    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

    // First, copy the input buffer rect to our WScreen
    ret = LbScreenLock();
    // If WScreen is application-controlled buffer, copy it to SDL surface
    if (ret == Lb_SUCCESS) {
        ulong sourcePos = sourceY * lbDisplay.GraphicsScreenWidth + sourceX;
        ulong destPos = destY * lbDisplay.GraphicsScreenWidth + destX;

        LbI_XMemRectCopy(lbDisplay.WScreen + sourcePos, sourceBuf + destPos,
          lbDisplay.GraphicsScreenWidth, width, height);
        ret = Lb_SUCCESS;
    }
    LbScreenUnlock();

#if defined(BFLIB_WSCREEN_CONTROL)
    // Blit the cursor on Draw Surface; simple if we have WScreen control
    LbMouseOnBeginSwap();
    ret = Lb_SUCCESS;
#else
    // Non-advanced cursor is drawn on WScreen pixels, so should be drawn here
    if (!lbPointerAdvancedDraw)
        LbMouseOnBeginSwap();

    ret = LbScreenLock();
    // If WScreen is application-controlled buffer, copy it to SDL surface
    if (ret == Lb_SUCCESS) {
        ulong blsize;
        blsize = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        LbI_XMemCopy(to_SDLSurf(lbDrawSurface)->pixels, lbDisplay.WScreen, blsize);
        ret = Lb_SUCCESS;
    }
    LbScreenUnlock();

    // Advanced cursor is blitted on lbDrawSurface, so should be drawn here
    if (lbPointerAdvancedDraw)
        LbMouseOnBeginSwap();
#endif

    // Put the data from Draw Surface onto Screen Surface
    if ((ret == Lb_SUCCESS) && (lbHasSecondSurface))
    {
        SDL_Rect clipRect = {destX, destY, width, height};
        SDL_SetClipRect(to_SDLSurf(lbScreenSurface), &clipRect);
        if (lbPhysicalResolutionMul > 1) {
            blresult = LbI_SDL_BlitScaled(to_SDLSurf(lbDrawSurface),
              to_SDLSurf(lbScreenSurface), lbPhysicalResolutionMul);
        } else {
            blresult = SDL_BlitSurface(to_SDLSurf(lbDrawSurface), NULL,
              to_SDLSurf(lbScreenSurface), NULL);
        }
        SDL_SetClipRect(to_SDLSurf(lbScreenSurface), NULL);
        if (blresult < 0) {
            LOGERR("blit failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    // Flip the image displayed on Screen Surface
    if (ret == Lb_SUCCESS) {
        // calls SDL_UpdateRect for entire screen if not double buffered
        blresult = SDL_Flip(to_SDLSurf(lbScreenSurface));
        if (blresult < 0) {
            // In some cases this situation seems to be quite common
            LOGERR("flip failed: %s", SDL_GetError());
            ret = Lb_FAIL;
        }
    }
    LbMouseOnEndSwap();
    return ret;
}

TbResult LbScreenSwapBoxClear(ubyte *sourceBuf, long sourceX, long sourceY,
  long destX, long destY, ulong width, ulong height, ubyte colour)
{
    assert(!"not implemented, as is never used");
    return Lb_FAIL;
}

static TbResult LbI_ScreenDrawHLineDirect(long X1, long Y1, long X2, long Y2)
{
    ubyte *ptr;
    ubyte *ptrEnd;
    long xBeg, xEnd;
    long width, shiftX, shiftY;

    LOGDBG("starting");
    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

    if (X1 <= X2) {
        width = X2 - X1;
        shiftY = lbDisplay.GraphicsWindowWidth * Y1;
        shiftX = X1;
    } else {
        width = X1 - X2;
        shiftY = lbDisplay.GraphicsWindowWidth * Y1;
        shiftX = X2;
    }
    xBeg = shiftY + shiftX;
    xEnd = width + shiftY + shiftX;

    {
        ptrEnd = &lbDisplay.PhysicalScreen[xEnd];
        ptr = &lbDisplay.PhysicalScreen[xBeg];
        do {
            *ptr += 128;
            ptr++;
        } while (ptr < ptrEnd);
    }
}

static TbResult LbI_ScreenDrawVLineDirect(long X1, long Y1, long X2, long Y2)
{
    ubyte *ptr;
    ubyte *ptrEnd;
    long yBeg, yEnd;
    long height, shiftX, shiftY;
    long delta;

    LOGDBG("starting");
    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

    if (Y1 <= Y2) {
        height = Y2 - Y1;
        shiftY = lbDisplay.GraphicsWindowWidth * Y1;
    } else {
        height = Y1 - Y2;
        shiftY = lbDisplay.GraphicsWindowWidth * Y2;
    }
    yBeg = shiftY + X1;
    yEnd = height * lbDisplay.GraphicsScreenWidth + yBeg;

    {
        ptrEnd = &lbDisplay.PhysicalScreen[yEnd];
        ptr = &lbDisplay.PhysicalScreen[yBeg];
        delta = lbDisplay.GraphicsScreenWidth;
        do {
            *ptr += 128;
            ptr += delta;
        } while (ptr < ptrEnd);
    }
}

TbResult LbScreenDrawHVLineDirect(long X1, long Y1, long X2, long Y2)
{
    CLIP_START_END_COORDS(X1, X2, lbDisplay.GraphicsWindowWidth);
    CLIP_START_END_COORDS(Y1, Y2, lbDisplay.GraphicsWindowHeight);
    LbIPhysicalScreenLock();
    if (X1 != X2)
    {
        LbI_ScreenDrawHLineDirect(X1, Y1, X2, Y2);
    } else
    {
        LbI_ScreenDrawVLineDirect(X1, Y1, X2, Y2);
    }
    LbIPhysicalScreenUnlock();
    return Lb_SUCCESS;
}

TbResult LbScreenWaitVbi_UNUSED(void)
{
    // No need to wait for vblank when using SDL
    // If we really, really wanted to achieve synchronization, we could use
    // SDL_UpdateWindowSurface() - that prevents tearing.
    return Lb_SUCCESS;
}

/******************************************************************************/
