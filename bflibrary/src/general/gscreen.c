/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gscreen.cpp
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
#include <string.h>
#include "bfscreen.h"

#include "privbflog.h"

TbScreenModeInfo lbScreenModeInfo[LB_MAX_SCREEN_MODES_COUNT];

i32 lbScreenModeInfoNum = 0;

/** Application title for target OS. */
char lbDrawAreaTitle[128] = "Bullfrog Shell";

DwBool lbScreenInitialised = false;

short lbIconIndex = 0;

const char * EmptyResourceMapping(short index);

/** Callback for mapping resource index into string icon identifier. */
ResourceMappingFunc userResourceMapping = EmptyResourceMapping;

TbDisplayStruct lbDisplay;

ushort lbUnitsPerPixel = 16;

TbBool LbHwCheckIsModeAvailable(TbScreenMode mode);

TbScreenModeInfo *LbScreenGetModeInfo(TbScreenMode mode)
{
    if (mode < lbScreenModeInfoNum)
        return &lbScreenModeInfo[mode];
    return &lbScreenModeInfo[0];
}

TbResult LbScreenFindVideoModes(void)
{
    int i, avail_num;
    avail_num = 0;
    lbScreenModeInfo[0].Available = false;
    for (i = 1; i < lbScreenModeInfoNum; i++)
    {
        if (LbHwCheckIsModeAvailable(i)) {
            lbScreenModeInfo[i].Available = true;
            avail_num++;
        } else {
            lbScreenModeInfo[i].Available = false;
        }
    }
    if (avail_num > 0)
        return Lb_SUCCESS;
    return Lb_FAIL;
}

TbBool LbScreenIsModeAvailable(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    static TbBool setup = false;
    if (!setup)
    {
        if (LbScreenFindVideoModes() != Lb_SUCCESS)
          return false;
        setup = true;
    }
    mdinfo = LbScreenGetModeInfo(mode);
    return mdinfo->Available;
}

TbScreenMode LbRecogniseVideoModeString(const char *desc)
{
    int mode;
    for (mode = 0; mode < lbScreenModeInfoNum; mode++)
    {
      if (strcasecmp(lbScreenModeInfo[mode].Desc, desc) == 0)
        return (TbScreenMode)mode;
    }
    return Lb_SCREEN_MODE_INVALID;
}

TbScreenMode LbRegisterVideoMode(const char *desc, TbScreenCoord width,
  TbScreenCoord height, ushort bpp, u32 flags)
{
    TbScreenModeInfo *mdinfo;
    TbScreenMode mode;

    mode = LbRecogniseVideoModeString(desc);
    if (mode != Lb_SCREEN_MODE_INVALID)
    {
        mdinfo = &lbScreenModeInfo[mode];
        if ((mdinfo->Width == width) && (mdinfo->Height == height) &&
          (mdinfo->BitsPerPixel == bpp))
        {
            // Mode is already registered
            return mode;
        }
        // Mode with same name but different params is registered
        LOGERR("Mode with same name but different params is registered, cannot register %dx%dx%d",
          (int)width, (int)height, (int)bpp);
        return Lb_SCREEN_MODE_INVALID;
    }
    if (lbScreenModeInfoNum >= sizeof(lbScreenModeInfo)/sizeof(lbScreenModeInfo[0]))
    {
        // No free mode slots
        return Lb_SCREEN_MODE_INVALID;
    }
    // Insert new mode to array
    mode = lbScreenModeInfoNum;
    lbScreenModeInfoNum++;
    mdinfo = &lbScreenModeInfo[mode];
    // Fill the mode content
    memset(mdinfo, 0, sizeof(TbScreenModeInfo));
    mdinfo->Width = width;
    mdinfo->Height = height;
    mdinfo->BitsPerPixel = bpp;
    mdinfo->Available = false;
    mdinfo->VideoMode = flags;
    strncpy(mdinfo->Desc, desc, sizeof(mdinfo->Desc));
    return mode;
}

TbResult LbSetTitle(const char *title)
{
    strncpy(lbDrawAreaTitle, title, sizeof(lbDrawAreaTitle)-1);
    return Lb_SUCCESS;
}

const char * EmptyResourceMapping(short index)
{
    return NULL;
}

TbResult LbSetUserResourceMapping(ResourceMappingFunc func)
{
    userResourceMapping = func;
    return Lb_SUCCESS;
}

TbResult LbSetIcon(short nicon)
{
    lbIconIndex = nicon;
    return Lb_SUCCESS;
}

TbScreenCoord LbGraphicsScreenWidth(void)
{
    return lbDisplay.GraphicsScreenWidth;
}

TbScreenCoord LbGraphicsScreenHeight(void)
{
    return lbDisplay.GraphicsScreenHeight;
}

TbScreenCoord LbScreenWidth(void)
{
    return lbDisplay.PhysicalScreenWidth;
}

TbScreenCoord LbScreenHeight(void)
{
    return lbDisplay.PhysicalScreenHeight;
}

TbResult LbScreenSetup(TbScreenMode mode, TbScreenCoord width, TbScreenCoord height,
    u8 *palette)
{
    return LbScreenSetupAnyMode(mode, width, height, palette);
}

TbResult LbScreenSetGraphicsWindow(u32 x, u32 y, u32 width, u32 height)
{
    i32 x2,y2;
    i32 i;
    x2 = x + width;
    y2 = y + height;
    if (x2 < x)
    {
        i = (x^x2);
        x = x^i;
        x2 = x^i^i;
    }
    if (y2 < y)
    {
        i = (y^y2);
        y = y^i;
        y2 = y^i^i;
    }
    if (x < 0)
        x = 0;
    if (x2 < 0)
        x2 = 0;
    if (y < 0)
        y = 0;
    if (y2 < 0)
        y2 = 0;
    if (x > lbDisplay.GraphicsScreenWidth)
        x = lbDisplay.GraphicsScreenWidth;
    if (x2 > lbDisplay.GraphicsScreenWidth)
        x2 = lbDisplay.GraphicsScreenWidth;
    if (y > lbDisplay.GraphicsScreenHeight)
        y = lbDisplay.GraphicsScreenHeight;
    if (y2 > lbDisplay.GraphicsScreenHeight)
        y2 = lbDisplay.GraphicsScreenHeight;
    lbDisplay.GraphicsWindowX = x;
    lbDisplay.GraphicsWindowY = y;
    lbDisplay.GraphicsWindowWidth = x2 - x;
    lbDisplay.GraphicsWindowHeight = y2 - y;
    if (lbDisplay.WScreen != NULL)
    {
        lbDisplay.GraphicsWindowPtr = lbDisplay.WScreen + lbDisplay.GraphicsScreenWidth*y + x;
    } else
    {
        lbDisplay.GraphicsWindowPtr = NULL;
    }
    return Lb_SUCCESS;
}

/******************************************************************************/
