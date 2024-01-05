/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file triangls.c
 *     Pathfinding Triangle declaration, life cycle and usage.
 * @par Purpose:
 *     Implement functions for handling triangles used for triangulation.
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
#include "triangls.h"

#include <limits.h>
#include "trstate.h"
#include "trlog.h"
/******************************************************************************/

/** Value stored in point Y coord to mark it as unused.
 * Needs to fit TrCoord type.
 */
#define TRIANGLE_UNALLOCATED_MARK UCHAR_MAX


TrTriangId tri_new(void)
{
    struct TrTriangle *p_tri;
    TrTriangId tri;

    if (triangulation[0].free_Triangles == -1)
    {
        tri = triangulation[0].ix_Triangles;
        if (tri < 0) {
            LOGERR("ix_Triangles depeled, got %d", (int)tri);
            return -1;
        }
        p_tri = &triangulation[0].Triangles[tri];
        triangulation[0].ix_Triangles++;
    } else
    {
        tri = triangulation[0].free_Triangles;
        if (tri < 0) {
            LOGERR("free_Triangles depeled, got %d", (int)tri);
            return -1;
        }
        p_tri = &triangulation[0].Triangles[tri];
        triangulation[0].free_Triangles = p_tri->tri[0];
    }
    p_tri->solid = 0;
    triangulation[0].count_Triangles++;
    return tri;
}

void tri_dispose(TrTriangId tri)
{
    struct TrTriangle *p_tri;
    TrTriangId tri_pvfree;

    tri_pvfree = triangulation[0].free_Triangles;
    triangulation[0].free_Triangles = tri;
    p_tri = &triangulation[0].Triangles[tri];
    p_tri->tri[0] = tri_pvfree;
    p_tri->solid = TRIANGLE_UNALLOCATED_MARK;
    triangulation[0].count_Triangles--;
}

TrTipId link_find(TrTriangId tri, TrTriangId tri_nx)
{
    struct TrTriangle *p_tri;
    TrTipId cor;

    p_tri = &triangulation[0].Triangles[tri];
    if (tri < 0) {
        return -1;
    }
    for (cor = 0; cor < 3; cor++)
    {
        if (p_tri->tri[cor] == tri_nx) {
            return cor;
        }
    }
    return -1;
}

TbBool triangle_tip_equals(TrTriangId tri, TrTipId cor, TrCoord pt_x, TrCoord pt_y)
{
    TrPointId pt;
    if (tri < 0)
        return false;
    if ((cor < 0) || (cor >= 3))
        return false;
    pt = triangulation[0].Triangles[tri].point[cor];
    return point_equals(pt, pt_x, pt_y);
}

TbBool triangle_has_point_coord(TrTriangId tri, TrCoord pt_x, TrCoord pt_y)
{
    if (triangle_tip_equals(tri, 0, pt_x, pt_y))
        return true;

    if (triangle_tip_equals(tri, 1, pt_x, pt_y))
        return true;

    if (triangle_tip_equals(tri, 2, pt_x, pt_y))
        return true;

    return false;
}


/******************************************************************************/
