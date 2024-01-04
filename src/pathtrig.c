/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file pathtrig.c
 *     Pathfinding triangulation support required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling pathfinding triangulation.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "pathtrig.h"

#include "swlog.h"
/******************************************************************************/

void path_init8_unkn3(struct Path *path, int ax8, int ay8, int bx8, int by8, int a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_path_init8_unkn3\n"
        : : "a" (path), "d" (ax8), "b" (ay8), "c" (bx8), "g" (by8), "g" (a6));
}

int triangle_findSE8(int x, int y)
{
    int ret;
    asm volatile ("call ASM_triangle_findSE8\n"
        : "=r" (ret) : "a" (x), "d" (y));
    return ret;
}

void triangulation_select(int tgnNo)
{
    asm volatile ("call ASM_triangulation_select\n"
        : : "a" (tgnNo));
}

/******************************************************************************/
