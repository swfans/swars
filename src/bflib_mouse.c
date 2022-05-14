/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_mouse.cpp
 *     Mouse related routines.
 * @par Purpose:
 *     Pointer position, movement and cursor support.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Feb 2008 - 26 Oct 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfmouse.h"

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "bflib_basics.h"
#include "globals.h"
#include "bfsprite.h"
#include "bfscreen.h"

extern ulong redraw_active_lock;

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

TbResult LbMouseSuspend(void)
{
    TbResult ret;
    if ( !lbMouseInstalled )
        return -1;
#if 1
    asm volatile ("call ASM_LbMouseSuspend\n"
        : "=r" (ret) : );
    return ret;
#endif
    redraw_active_lock = 1;
#if 0
    if ( !lbScreenDirectAccessActive )
        screen_remove(1);
    memset(&mbuffer, 0, 0x1020u);
#endif
    lbDisplay.MouseSprite = 0;
    lbMouseInstalled = 0;
    redraw_active_lock = 0;
    return Lb_SUCCESS;
}

TbResult LbMouseChangeMoveRatio(long ratio_x, long ratio_y)
{
    TbResult ret;
    asm volatile ("call ASM_LbMouseChangeMoveRatio\n"
        : "=r" (ret) : "a" (ratio_x), "d" (ratio_y));
    return ret;
}

TbResult LbMouseChangeSpriteOffset(unsigned long hsX, unsigned long hsY)
{
    TbResult ret;
    asm volatile ("call ASM_LbMouseChangeSpriteOffset\n"
        : "=r" (ret) : "a" (hsX), "d" (hsY));
    return ret;
}

TbResult LbMousePlace(void)
{
    TbResult ret;
    asm volatile ("call ASM_LbMousePlace\n"
        : "=r" (ret) : );
    return ret;
}

TbResult LbMouseRemove(void)
{
    TbResult ret;
    asm volatile ("call ASM_LbMouseRemove\n"
        : "=r" (ret) : );
    return ret;
}

TbResult LbMouseReset(void)
{
    TbResult ret;
    asm volatile ("call ASM_LbMouseReset\n"
        : "=r" (ret) : );
    return ret;
}

TbResult LbMouseSetWindow(long x, long y, long width, long height)
{
    TbResult ret;
    asm volatile ("call ASM_LbMouseSetWindow\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (width), "c" (height));
    return ret;
}

TbResult LbMouseSetPosition(long x, long y)
{
    TbResult ret;
    asm volatile ("call ASM_LbMouseSetPosition\n"
        : "=r" (ret) : "a" (x), "d" (y));
    return ret;
}

TbResult LbMouseSetup(const struct TbSprite *pointer_spr, int ratio_x, int ratio_y)
{
    long x,y;

    if (lbMouseInstalled)
        LbMouseSuspend();

#if 1
    TbResult ret;
    asm volatile ("call ASM_LbMouseSetup\n"
        : "=r" (ret) : "a" (pointer_spr), "d" (ratio_x), "b" (ratio_y));
    return ret;
#endif

#if 0
    pointerHandler.Install();
#endif

#if 0
    minfo.XSpriteOffset = 0;
    minfo.YSpriteOffset = 0;
    minfo.XMoveRatio = 1;
    minfo.YMoveRatio = 1;
    memset(minfo.Sprite, 254, 0x1000u);
    lbDisplay.MouseSprite = 0;
    redraw_active_lock = 0;
    memset(&mbuffer, 0, 0x1020u);
#endif

    lbMouseOffline = true;
    lbMouseInstalled = true;

    if ( LbMouseSetWindow(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight) != Lb_SUCCESS )
    {
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    y = lbDisplay.MouseWindowY + lbDisplay.MouseWindowHeight / 2;
    x = lbDisplay.MouseWindowX + lbDisplay.MouseWindowWidth / 2;
    if ( LbMouseChangeMoveRatio(ratio_x, ratio_y) != Lb_SUCCESS )
    {
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    if ( LbMouseSetPosition(x, y) != Lb_SUCCESS )
    {
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    if ( LbMouseChangeSprite(pointer_spr) != Lb_SUCCESS )
    {
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    lbMouseOffline = false;
    return Lb_SUCCESS;
}

/*TbResult LbMouseSetPointerHotspot(long hot_x, long hot_y)
{
  if (!lbMouseInstalled)
    return Lb_FAIL;
  if (!pointerHandler.SetPointerOffset(hot_x, hot_y))
    return Lb_FAIL;
  return Lb_SUCCESS;
}

TbResult LbMouseSetPosition(long x, long y)
{
  if (!lbMouseInstalled)
    return Lb_FAIL;
  if (!pointerHandler.SetMousePosition(x, y))
    return Lb_FAIL;
  return Lb_SUCCESS;
}*/

TbResult LbMouseChangeSprite(const struct TbSprite *pointer_spr)
{
    if (pointer_spr == NULL)
        BFLIB_DEBUGLOG(0,"Setting to %s","NONE");
    else
        BFLIB_DEBUGLOG(0,"Setting to %dx%d, data at %p",(int)pointer_spr->SWidth,(int)pointer_spr->SHeight,pointer_spr->Data);
    TbResult ret;
    asm volatile ("call ASM_LbMouseChangeSprite\n"
        : "=r" (ret) : "a" (pointer_spr));
    return ret;
    /*
  if (!lbMouseInstalled)
    return Lb_FAIL;
  if (!pointerHandler.SetMousePointer(pointerSprite))
    return Lb_FAIL;
  return Lb_SUCCESS;*/
}

/******************************************************************************/
#ifdef __cplusplus
}
#endif
