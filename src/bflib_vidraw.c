/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bflib_vidraw.c
 *     Routines to be moved to bflibrary.
 * @par Purpose:
 *     Implement functions from bflibrary but not yet integrated there.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_vidraw.h"

#include <stdlib.h>
#include "bfscreen.h"
#include "bfline.h"
/******************************************************************************/

void LbDrawTriangleFilled(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_LbDrawTriangleFilled\n"
        :  : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (x3), "g" (y3), "g" (colour));
}

void LbDrawTriangle(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    if (lbDisplay.DrawFlags & Lb_SPRITE_OUTLINE)
    {
        LbDrawLine(x1, y1, x2, y2, colour);
        LbDrawLine(x2, y2, x3, y3, colour);
        LbDrawLine(x3, y3, x1, y1, colour);
    }
    else
    {
        LbDrawTriangleFilled(x1, y1, x2, y2, x3, y3, colour);
    }
}

void LbDrawSlantBox(short X, short Y, ushort a3, ushort a4)
{
    asm volatile (
      "call ASM_LbDrawSlantBox\n"
        :  : "a" (X), "d" (Y), "b" (a3), "c" (a4));
}

/******************************************************************************/
