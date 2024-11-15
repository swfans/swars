/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_render_drspr.c
 *     Rendering function draw_gpoly() for drawing 3D view elements.
 * @par Purpose:
 *     Function for rendering 3D elements.
 * @par Comment:
 *     Go away from here, you bad optimizer! Do not compile this with optimizations.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 14 Feb 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "poly.h"

#include "globals.h"
#include "swlog.h"

/******************************************************************************/


/******************************************************************************/

void LbSpriteDraw_1(int x, int y, struct TbSprite *spr)
{
    asm volatile (
      "call ASM_LbSpriteDraw_1\n"
        : : "a" (x), "d" (y), "b" (spr));
}

void LbSpriteDraw_2(int x, int y, struct TbSprite *spr)
{
    asm volatile (
      "call ASM_LbSpriteDraw_2\n"
        : : "a" (x), "d" (y), "b" (spr));
}

/******************************************************************************/
