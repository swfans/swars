/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file tringops.c
 *     Pathfinding related operations concerning multiple triangles.
 * @par Purpose:
 *     Implement functions for performing operations on triangles.
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
#include "tringops.h"

#include "triangls.h"
#include "trstate.h"
#include "trlog.h"
/******************************************************************************/

static TrTriangId tri_split2(TrTriangId tri, TrTipId cor,
  TrCoord pt_x, TrCoord pt_y, TrPointId pt)
{
    //TODO implement
    return 0;
}

TrPointId edge_split(TrTriangId tri, TrTipId cor, TrCoord pt_x, TrCoord pt_y)
{
    struct TrTriangle *p_tri;
    struct TrTriangle *p_tri_sec;
    TrPointId pt;
    TrTriangId tri_sec;
    TrTipId cor_sec;
    TrTriangId tri_sp1, tri_sp2;

    // Create and fill new point
    pt = point_set_new_or_reuse(pt_x, pt_y);
    if (pt < 0) {
        return -1;
    }
    p_tri = &triangulation[0].Triangles[tri];
    // Find second tri and cor which links it back to the first
    tri_sec = p_tri->tri[cor];
    cor_sec = link_find(tri_sec, tri);
    if (cor_sec < 0) {
        LOGERR("no two-way link between triangles %d and %d", tri, tri_sec);
        point_dispose(pt);
        return -1;
    }
    // Do the splitting
    tri_sp1 = tri_split2(tri, cor, pt_x, pt_y, pt);
    tri_sp2 = tri_split2(tri_sec, cor_sec, pt_x, pt_y, pt);
    p_tri_sec = &triangulation[0].Triangles[tri_sec];
    p_tri_sec->tri[cor_sec] = tri_sp1;
    p_tri->tri[cor] = tri_sp2;
    return pt;
}

/******************************************************************************/
