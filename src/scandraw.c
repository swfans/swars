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
#include "swlog.h"
/******************************************************************************/

void unkn_draw_transformed_point(short x, short y, long ptX, long ptY, long ptZ, ubyte colour)
{
    struct EnginePoint ep;

    ep.X3d = ptX - engn_xc;
    ep.Z3d = ptZ - engn_zc;
    ep.Y3d = ptY - engn_yc;
    transform_point(&ep);
    LbDrawLine(x, y, ep.pp.X, ep.pp.Y, colour);
}

/******************************************************************************/
