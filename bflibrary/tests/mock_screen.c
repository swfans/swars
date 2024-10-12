/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mock_screen.c
 *     Implementation which only pretends to do stuff, for test purposes.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     05 Nov 2021 - 29 Jul 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdlib.h>
#include "mock_bfscreen.h"
#include "mock_bfmouse.h"
#include "mock_bfpalette.h"
#include "bfscreen.h"
#include "bfscrsurf.h"
#include "bfwindows.h"
#include "bfanywnd.h"
#include "bfutility.h"
#include "bftstlog.h"

extern long lbMinScreenSurfaceDimension;


TbResult MockScreenFindVideoModes(void)
{
    int i;
    for (i = 0; i < lbScreenModeInfoNum; i++)
        lbScreenModeInfo[i].Available = true;
    return Lb_SUCCESS;
}

TbBool MockScreenIsModeAvailable(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    static TbBool setup = false;
    if (!setup) {
        MockScreenFindVideoModes();
        setup = true;
    }
    mdinfo = LbScreenGetModeInfo(mode);
    return mdinfo->Available;
}

TbResult MockScreenSetupAnyMode(TbScreenMode mode, TbScreenCoord width,
    TbScreenCoord height, ubyte *palette)
{
    long hot_x, hot_y;
    long mdWidth, mdHeight;
    const struct TbSprite *msspr;
    TbScreenModeInfo *mdinfo;

    msspr = NULL;
    if (!lbLibInitialised) {
        LOGERR("set screen mode on ununitialized bflibrary");
        return Lb_FAIL;
    }

    if (lbDisplay.MouseSprite != NULL) {
        msspr = lbDisplay.MouseSprite;
        MockMouseGetSpriteOffset(&hot_x, &hot_y);
    }
    MockMouseChangeSprite(NULL);

    free((void *)lbScreenSurface);
    if (lbHasSecondSurface) {
        free((void *)lbDrawSurface);
        lbHasSecondSurface = false;
    }
    lbScreenInitialised = false;

    if (lbDisplay.OldVideoMode == 0)
        lbDisplay.OldVideoMode = 0xFF;

    mdinfo = LbScreenGetModeInfo(mode);
    if (!MockScreenIsModeAvailable(mode)) {
        LOGERR("%s resolution %dx%d (mode %d) not available",
            (mdinfo->VideoMode & Lb_VF_WINDOWED) ? "windowed" : "full screen",
            (int)mdinfo->Width, (int)mdinfo->Height, (int)mode);
        return Lb_FAIL;
    }
    mdWidth = mdinfo->Width;
    mdHeight = mdinfo->Height;
    {
        const long minD = min(mdWidth, mdHeight);
        if (minD != 0 && minD < lbMinScreenSurfaceDimension) {
            mdWidth = lbScreenSurfaceDimensions.Width =
                lbMinScreenSurfaceDimension * mdWidth / minD;
            mdHeight = lbScreenSurfaceDimensions.Height =
                lbMinScreenSurfaceDimension * mdHeight / minD;
        }
    }
    LOGDBG("screen surface dimensions set to %ldx%ld", mdWidth, mdHeight);
    lbDisplay.VesaIsSetUp = false;

    lbScreenSurface = lbDrawSurface = (OSSurfaceHandle)malloc(mdWidth *
      mdHeight * (mdinfo->BitsPerPixel+7) / 8);

    if ((mdinfo->BitsPerPixel != lbEngineBPP) ||
        (mdWidth != width) || (mdHeight != height))
    {
        lbDrawSurface = (OSSurfaceHandle)malloc(width * height * (lbEngineBPP+7) / 8);
        lbHasSecondSurface = true;
    }

    lbDisplay.DrawFlags = 0;
    lbDisplay.DrawColour = 0;
#if defined(LB_ENABLE_SHADOW_COLOUR)
    lbDisplay.ShadowColour = 0;
#endif
    lbDisplay.PhysicalScreenWidth = mdinfo->Width;
    lbDisplay.PhysicalScreenHeight = mdinfo->Height;
    lbDisplay.ScreenMode = mode;
    lbDisplay.PhysicalScreen = NULL;
    lbDisplay.GraphicsScreenWidth  = width;
    lbDisplay.GraphicsScreenHeight = height;

    lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;

    lbScreenInitialised = true;
    if (palette != NULL)
    {
        if (MockPaletteSet(palette) != Lb_SUCCESS) {
            LOGERR("palette setting failed");
            return Lb_FAIL;
        }
    }
    LbScreenSetGraphicsWindow(0, 0, mdinfo->Width, mdinfo->Height);
    LbTextSetWindow(0, 0, mdinfo->Width, mdinfo->Height);
    if ( MockMouseIsInstalled() )
    {
        MockMouseChangeSpriteOffset(hot_x, hot_y);
        if (msspr != NULL)
          MockMouseChangeSprite(msspr);
    }
    return Lb_SUCCESS;
}

TbResult MockScreenReset(void)
{
    if (!lbScreenInitialised)
      return Lb_FAIL;

    free((void *)lbScreenSurface);
    if (lbHasSecondSurface) {
        free((void *)lbDrawSurface);
        lbHasSecondSurface = false;
    }
    lbDrawSurface = NULL;
    lbScreenSurface = NULL;
    lbScreenInitialised = false;
    return Lb_SUCCESS;
}

TbBool MockScreenIsLocked(void)
{
    return (lbDisplay.WScreen != NULL) && (lbScreenInitialised)
      && (lbDrawSurface != NULL);
}

TbResult MockScreenLock(void)
{
    if (!lbScreenInitialised)
        return Lb_FAIL;

    lbDisplay.WScreen = (ubyte *) lbDrawSurface;
    lbDisplay.GraphicsWindowPtr = &lbDisplay.WScreen[lbDisplay.GraphicsWindowX +
        lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsWindowY];

    return Lb_SUCCESS;
}

TbResult MockScreenUnlock(void)
{
    if (!lbScreenInitialised)
        return Lb_FAIL;

    lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;
    return Lb_SUCCESS;
}

/******************************************************************************/
