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
#include "bflib_mouse.h"

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "bflib_basics.h"
#include "globals.h"
#include "bfsprite.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/*
TbResult LbMouseChangeSpriteAndHotspot(struct TbSprite *pointer_spr, long hot_x, long hot_y)
{
    if (pointerSprite == NULL)
        BFLIB_DEBUGLOG(0,"Setting to %s","NONE");
    else
        BFLIB_DEBUGLOG(0,"Setting to %dx%d, data at %p",(int)pointer_spr->SWidth,(int)pointer_spr->SHeight,pointer_spr->Data);
  if (!lbMouseInstalled)
    return Lb_FAIL;
  if (!pointerHandler.SetMousePointerAndOffset(pointerSprite, hot_x, hot_y))
    return Lb_FAIL;
  return Lb_SUCCESS;
}*/

TbResult LbMouseSetup(const struct TbSprite *pointer_spr, int ratio_x, int ratio_y)
{
  TbResult ret;
  asm volatile ("call ASM_LbMouseSetup\n"
      : "=r" (ret) : "a" (pointer_spr), "d" (ratio_x), "b" (ratio_y));
/*  long x,y;
  if (lbMouseInstalled)
    LbMouseSuspend();
  y = (lbDisplay.MouseWindowHeight + lbDisplay.MouseWindowY) / 2;
  x = (lbDisplay.MouseWindowWidth + lbDisplay.MouseWindowX) / 2;
  pointerHandler.Install();
  lbMouseOffline = true;
  lbMouseInstalled = true;
  LbMouseSetWindow(0,0,LbGraphicsScreenWidth(),LbGraphicsScreenHeight());
  ret = Lb_SUCCESS;
  if (LbMouseSetPosition(x,y) != Lb_SUCCESS)
    ret = Lb_FAIL;
  if (LbMouseChangeSprite(pointerSprite) != Lb_SUCCESS)
    ret = Lb_FAIL;
  lbMouseInstalled = (ret == Lb_SUCCESS);
  lbMouseOffline = false;*/
  return ret;
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
