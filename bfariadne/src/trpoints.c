/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file trpoints.c
 *     Triangle points declaration, life cycle and usage.
 * @par Purpose:
 *     Implement functions for handling triangle points used for triangulation.
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
#include "trpoints.h"

#include <assert.h>
#include "trstate.h"
/******************************************************************************/

TbBool point_set(TrPointId pt, TrCoord pt_x, TrCoord pt_y)
{
    struct TrPoint *p_point;

    if (pt < 0)
        return false;
    p_point = &triangulation[0].Points[pt];

    p_point->x = pt_x;
    p_point->y = pt_y;
    return true;
}

TrPointId point_new(void)
{
    struct TrPoint *p_point;
    TrPointId pt;

    if (triangulation[0].free_Points == -1)
    {
        pt = triangulation[0].ix_Points++;
        p_point = &triangulation[0].Points[pt];
    }
    else
    {
        pt = triangulation[0].free_Points;
        p_point = &triangulation[0].Points[pt];
        triangulation[0].free_Points = p_point->x;
    }
    // Clear the value which marked the point as unused
    p_point->y = 0;
    triangulation[0].count_Points++;
    return pt;
}

void point_dispose(TrPointId pt)
{
    struct TrPoint *p_point;
    TrPointId last_pt;

    last_pt = triangulation[0].free_Points;
    p_point = &triangulation[0].Points[pt];
    // Reuse y coord to mark the point as unused
    p_point->y = POINT_UNALLOCATED_MARK;
    assert(p_point->y == POINT_UNALLOCATED_MARK);
    triangulation[0].free_Points = pt;
    // Reuse x coord to link unused points into a chain
    assert(sizeof(TrCoord) >= sizeof(TrPointId));
    p_point->x = last_pt;
    triangulation[0].count_Points--;
}


/******************************************************************************/
