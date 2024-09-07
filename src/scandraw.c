/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file scandraw.c
 *     Ingame scanner (minimap/radar) drawing.
 * @par Purpose:
 *     Implement functions drawing the scanner map elements.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "scandraw.h"

#include "bfline.h"
#include "engintrns.h"
#include "campaign.h"
#include "display.h"
#include "thing.h"
#include "swlog.h"
/******************************************************************************/

void unkn_draw_transformed_point(short x, short y, long ptX, long ptY, long ptZ, ubyte colour)
{
    struct EnginePoint ep;

    ep.X3d = ptX - engn_xc;
    ep.Z3d = ptZ - engn_zc;
    ep.Y3d = ptY - engn_yc;
    ep.Flags = 0;
    transform_point(&ep);
    LbDrawLine(x, y, ep.pp.X, ep.pp.Y, colour);
}

void draw_objective_point(long x, long y, ThingIdx thing, short a4, ubyte colour)
{
    long ptX, ptY, ptZ;

    if (a4)
        return;

    if (thing <= 0) {
        struct SimpleThing *p_sthing;
        p_sthing = &sthings[thing];
        ptX = p_sthing->X;
        ptY = p_sthing->Y;
        ptZ = p_sthing->Z;
    } else {
        struct Thing *p_thing;
        p_thing = &things[thing];
        ptX = p_thing->X;
        ptY = p_thing->Y;
        ptZ = p_thing->Z;
    }
    unkn_draw_transformed_point(x, y, ptX, ptY, ptZ, colour);
}

void func_711F4(short a1, short a2, short a3, short a4, ubyte colour)
{
    asm volatile (
      "push %4\n"
      "call ASM_func_711F4\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (colour));
}

void SCANNER_draw_new_transparent(void)
{
    asm volatile ("call ASM_SCANNER_draw_new_transparent\n"
        :  :  : "eax" );
}

/******************************************************************************/
