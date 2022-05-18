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
#include "bfscreen.h"

#include "bfscrsurf.h"
#include "bfmouse.h"
#include "bflog.h"

#define to_SDLSurf(h) ((SDL_Surface  *)h)

//TODO delete
#define lbScreenInitialised 1
#define lbHasSecondSurface 0

//int lbScreenDirectAccessActive;

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
}

int LbScreenSetupAnyMode_TODO(unsigned short mode, unsigned long width,
    unsigned long height, TbPixel *palette)
{
// code at 0001:000954e0
}

int LbScreenClearGraphicsWindow_TODO()
{
// code at 0001:000956dc
}

int LbScreenClear_TODO()
{
// code at 0001:00095728
}

int LbScreenReset_TODO()
{
// code at 0001:00095754
}

TbResult LbScreenLock_TODO(void)
{
    LIBLOG("Starting");
    if (!lbScreenInitialised)
        return Lb_FAIL;

    if (SDL_MUSTLOCK(to_SDLSurf(lbDrawSurface))) {
        if (SDL_LockSurface(to_SDLSurf(lbDrawSurface)) < 0) {
            LIBLOG("error: SDL Lock Surface: %s", SDL_GetError());
            lbDisplay.GraphicsWindowPtr = NULL;
            lbDisplay.WScreen = NULL;
            return Lb_FAIL;
        }
    }

    lbDisplay.WScreen = (unsigned char *) to_SDLSurf(lbDrawSurface)->pixels;
    lbDisplay.GraphicsScreenWidth = to_SDLSurf(lbDrawSurface)->pitch;
    lbDisplay.GraphicsWindowPtr = &lbDisplay.WScreen[lbDisplay.GraphicsWindowX +
        lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsWindowY];

    return Lb_SUCCESS;
}

TbResult LbScreenUnlock_TODO(void)
{
    LIBLOG("Starting");
    if (!lbScreenInitialised)
        return Lb_FAIL;

    lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;

    if (SDL_MUSTLOCK(to_SDLSurf(lbDrawSurface)))
        SDL_UnlockSurface(to_SDLSurf(lbDrawSurface));

    return Lb_SUCCESS;
}

int LbScreenSetDoubleBuffering_TODO()
{
// code at 0001:000957b8
}

int LbScreenSetWScreenInVideo_TODO()
{
// code at 0001:000957d8
}

int LbScreenFindVideoModes_TODO()
{
// code at 0001:000957f8
}

TbResult LbScreenSwap(void)
{
    TbResult ret;

    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

    LbMousePlace();
    {
        ulong blremain;
        blremain = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        LbI_XMemCopy(lbDisplay.PhysicalScreen, lbDisplay.WScreen, blremain);
        ret = Lb_SUCCESS;
    }
    LbMouseRemove();
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

int LbScreenSwapBoxClear_TODO()
{
// code at 0001:00095964
}

TbResult LbScreenSwapClear(TbPixel colour)
{
    assert(!lbDisplay.VesaIsSetUp); // video mem paging not supported with SDL

    LbMousePlace();
    {
        int blsize;
        ubyte *blsrcbuf;
        blsrcbuf = lbDisplay.WScreen;
        blsize = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        LbI_XMemCopyAndSet(lbDisplay.PhysicalScreen, blsrcbuf, 0x01010101 * colour, blsize);
    }
    LbMouseRemove();
    return Lb_SUCCESS;
}

int LbScreenSwapBox_TODO()
{
// code at 0001:00095c38
}

int LbScreenDrawHVLineDirect_TODO()
{
// code at 0001:00095dc4
}

int LbScreenWaitVbi_TODO()
{
// code at 0001:000961b0
}


/******************************************************************************/
