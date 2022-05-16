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

TbResult LbMouseChangeMoveRatio_TODEL(long ratio_x, long ratio_y)
{
    TbResult ret;
    asm volatile ("call ASM_LbMouseChangeMoveRatio\n"
        : "=r" (ret) : "a" (ratio_x), "d" (ratio_y));
    return ret;
}

TbResult LbMouseChangeSpriteOffset_TODEL(unsigned long hsX, unsigned long hsY)
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
