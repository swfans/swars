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

//TODO remove from ASM, then rename
struct ScreenModeInfo lbScreenModeInfo_UNUSED[] = {
    {   0,   0, 0,0,   0x0,"MODE_INVALID"},
    { 320, 200, 8,0,  0x13,"MODE_320_200_8"},
    { 320, 200,16,0,   0x0,"MODE_320_200_16"},
    { 320, 200,24,0,   0x0,"MODE_320_200_24"},
    { 320, 240, 8,0,   0x0,"MODE_320_240_8"},
    { 320, 240,16,0,   0x0,"MODE_320_240_16"},
    { 320, 240,24,0,   0x0,"MODE_320_240_24"},
    { 512, 384, 8,0,   0x0,"MODE_512_384_8"},
    { 512, 384,16,0,   0x0,"MODE_512_384_16"},
    { 512, 384,24,0,0x0100,"MODE_512_384_24"},
    { 640, 400, 8,0,   0x0,"MODE_640_400_8"},
    { 640, 400,16,0,   0x0,"MODE_640_400_16"},
    { 640, 400,24,0,0x0101,"MODE_640_400_24"},
    { 640, 480, 8,0,   0x0,"MODE_640_480_8"},
    { 640, 480,16,0,   0x0,"MODE_640_480_16"},
    { 640, 480,24,0,0x0103,"MODE_640_480_24"},
    { 800, 600, 8,0,   0x0,"MODE_800_600_8"},
    { 800, 600,16,0,   0x0,"MODE_800_600_16"},
    { 800, 600,24,0,0x0105,"MODE_800_600_24"},
    {1024, 768, 8,0,   0x0,"MODE_1024_768_8"},
    {1024, 768,16,0,   0x0,"MODE_1024_768_16"},
    {1024, 768,24,0,0x0107,"MODE_1024_768_24"},
    {1280,1024, 8,0,   0x0,"MODE_1280_1024_8"},
    {1280,1024,16,0,   0x0,"MODE_1280_1024_16"},
    {1280,1024,24,0,   0x0,"MODE_1280_1024_24"},
    {1600,1200, 8,0,   0x0,"MODE_1600_1200_8"},
    {1600,1200,16,0,   0x0,"MODE_1600_1200_16"},
    {1600,1200,24,0,   0x0,"MODE_1600_1200_24"},
    {   0,   0, 0,0,   0x0,"MODE_INVALID"},
};

/** Count of used entries in registered video modes list. */
long lbScreenModeInfoNum = 28;

/** Application title for target OS. */
char lbDrawAreaTitle[128] = "Bullfrog Shell";

short lbIconIndex = 0;

const char * EmptyResourceMapping(short index);

/** Callback for mapping resource index into string icon identifier. */
ResourceMappingFunc userResourceMapping = EmptyResourceMapping;

TbDisplayStruct lbDisplay;

ushort lbUnitsPerPixel = 16;

TbScreenModeInfo *LbScreenGetModeInfo(TbScreenMode mode)
{
    if (mode < lbScreenModeInfoNum)
        return &lbScreenModeInfo[mode];
    return &lbScreenModeInfo[0];
}

TbBool LbScreenIsModeAvailable(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
#if 0
    static TbBool setup = false;
    if (!setup)
    {
        if (LbScreenFindVideoModes() != Lb_SUCCESS)
          return false;
        setup = true;
    }
#endif
    mdinfo = LbScreenGetModeInfo(mode);
    return mdinfo->Available;
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
    unsigned char *palette)
{
    return LbScreenSetupAnyMode(mode, width, height, palette);
}

TbResult LbScreenSetGraphicsWindow(ulong x, ulong y, ulong width, ulong height)
{
    long x2,y2;
    long i;
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
