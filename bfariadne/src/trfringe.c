/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file trfringe.c
 *     Triangulation Fringe support and usage.
 * @par Purpose:
 *     Implement functions for handling fringe map, required for pathfinding.
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
#include "trfringe.h"

#include <string.h>
#include "trpoints.h"
/******************************************************************************/
extern TrCoord fringe_y[256];
extern TrCoord fringe_x1, fringe_y1, fringe_x2, fringe_y2;
extern ubyte *fringe_map;


void fringe_init(ubyte *p_map, TrCoord x1, TrCoord y1, TrCoord x2, TrCoord y2)
{
    int x;

    fringe_map = p_map;
    fringe_x1 = x1;
    fringe_y1 = y1;
    fringe_x2 = x2;
    fringe_y2 = y2;
    for (x = x1; x < x2; x++) {
        fringe_y[x] = y1;
    }
}

int fringe_scan(TrCoord *p_x1, TrCoord *p_y1, TrCoord *p_x2, TrCoord *p_y2)
{
    TrCoord x1, y1, x2;
    TrCoord x, cx;

    x = fringe_x1;
    y1 = fringe_y2;
    while (x < fringe_x2)
    {
        cx = x + 1;
        if (y1 <= fringe_y[x]) {
            x++;
            continue;
        }
        y1 = fringe_y[x];
        x1 = x++;
        while (cx < fringe_x2)
        {
            if (y1 != fringe_y[cx])
                break;
            cx++;
            x++;
        }
        x2 = x - x1;
    }

    if (y1 == fringe_y2)
        return 0;

    *p_x1 = x1;
    *p_y1 = y1;
    *p_x2 = x2;
    *p_y2 = fringe_y2 - y1;
    return 1;
}

int fringe_get_rectangle(TrCoord *p_x1, TrCoord *p_y1,
  TrCoord *p_x2, TrCoord *p_y2, ubyte *p_solid)
{
    TrCoord frx1, fry1, frx2, fry2;
    int dx, dy;
    ubyte solid;
    ubyte *m_start;
    ubyte *m;
    int k;

    if (!fringe_scan(&frx1, &fry1, &frx2, &fry2))
        return 0;

    m_start = &fringe_map[256 * fry1 + frx1];
    solid = *m_start;

    m = m_start + 1;
    for (dx = 1; dx < frx2; dx++)
    {
        if (solid != *m)
            break;
        m++;
    }

    m = m_start + 256;
    for (dy = 1; dy < fry2; dy++)
    {
        if (memcmp(m, m_start, dx) != 0)
            break;
        m += 256;
    }

    for (k = frx1; k < frx1 + dx; k++)
      fringe_y[k] = fry1 + dy;

    *p_solid = solid;
    *p_x1 = frx1;
    *p_y1 = fry1;
    *p_x2 = frx1 + dx;
    *p_y2 = fry1 + dy;
    return 1;
}

/******************************************************************************/
