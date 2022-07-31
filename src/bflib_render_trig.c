/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_render_trig.c
 *     Rendering function trig() for drawing 3D view elements.
 * @par Purpose:
 *     Function for rendering 3D elements.
 * @par Comment:
 *     Go away from here, you bad optimizer! Do not compile this with optimizations.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 30 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_render.h"
#include "bfgentab.h"
#include "poly.h"
#include "poly_trigp.h"
#include "poly_trigr.h"
#include "swlog.h"

#include "globals.h"

/******************************************************************************/
extern GameTurn gameturn;
/******************************************************************************/

/* trig() wrapper for debug (comparing remade trig() with original assembly */
void trig_dbg(struct PolyPoint *point_a, struct PolyPoint *point_b,
  struct PolyPoint *point_c)
{
    if (gameturn & 0x04) {
        asm volatile ("call ASM_trig\n"
            :  : "a" (point_a), "d" (point_b), "b" (point_c));
    } else {
        trig(point_a, point_b, point_c);
    }
}
/******************************************************************************/
