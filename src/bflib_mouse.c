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

TbResult LbMouseChangeSprite_UNUSED(const struct TbSprite *pointer_spr);


TbResult LbMouseChangeSprite(const struct TbSprite *pointer_spr)
{
    TbResult ret;
    LbMouseChangeSprite_UNUSED(pointer_spr);
    asm volatile ("call ASM_LbMouseChangeSprite\n"
        : "=r" (ret) : "a" (pointer_spr));
    return ret;
}

/******************************************************************************/
#ifdef __cplusplus
}
#endif
