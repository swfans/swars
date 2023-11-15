/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file drawtext.c
 *     Math transformations required by the 3D engine.
 * @par Purpose:
 *     Implement functions for transforming positions in 3D world.
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
#include "drawtext.h"

#include "bfscreen.h"
#include "rom.h"
#include "swlog.h"
/******************************************************************************/

void draw_text(short x, short y, const char *text, u8 colour)
{
    long scrn_w, scrn_h;
    u8 *scr;

    if (x < 0 || y < 0)
        return;
    scrn_w = lbDisplay.GraphicsScreenWidth;
    scrn_h = lbDisplay.GraphicsScreenHeight;
    if (x > scrn_w - 1)
        return;
    if ( y > scrn_h - 1)
        return;

    scr = &lbDisplay.WScreen[x + scrn_w * y];
    prop_text(text, scr, scrn_w, colour);
}

/******************************************************************************/
