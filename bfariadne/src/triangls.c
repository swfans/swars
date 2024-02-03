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
#include <stdlib.h>
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

void tri_clear(TrTriangId tri)
{
    struct TrTriangle *p_tri;
    TrTipId cor;

    p_tri = &triangulation[0].Triangles[tri];

    p_tri->solid = TRIANGLE_UNALLOCATED_MARK;
    for (cor = 0; cor < 3; cor++) {
        p_tri->point[cor] = 0;
    }
    for (cor = 0; cor < 3; cor++) {
        p_tri->tri[cor] = -1;
    }
    p_tri->jump = -1;
    p_tri->enter = 0;
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

TbBool tri_is_allocated(TrTriangId tri)
{
    struct TrTriangle *p_tri;

    if (tri < 0)
        return false;
    p_tri = &triangulation[0].Triangles[tri];
    return (p_tri->solid != TRIANGLE_UNALLOCATED_MARK);
}

TrTipId link_find(TrTriangId tri, TrTriangId tri_nx)
{
    struct TrTriangle *p_tri;
    TrTipId cor;

    if (tri < 0) {
        return -1;
    }
    p_tri = &triangulation[0].Triangles[tri];
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

    if (tri < 0) {
        return false;
    }
    if ((cor < 0) || (cor >= 3)) {
        return false;
    }
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

TbBool triangle_contained_within_rect_coords(TrTriangId tri,
  TrCoord x1, TrCoord y1, TrCoord x2, TrCoord y2)
{
    struct TrTriangle *p_tri;

    p_tri = &triangulation[0].Triangles[tri];
    if (!point_within_rect_coords(p_tri->point[0], x1, y1, x2, y2))
        return false;

    if (!point_within_rect_coords(p_tri->point[1], x1, y1, x2, y2))
        return false;

    if (!point_within_rect_coords(p_tri->point[2], x1, y1, x2, y2))
        return false;

    return true;
}

/** Return if given triangle is at border of triangulated area - has one of its siblings not set.
 */
static TbBool triangleptr_is_border(struct TrTriangle *p_tri)
{
    TrTipId cor;

    for (cor = 0; cor < 3; cor++)
    {
        if (p_tri->tri[cor] == -1)
              return true;
    }
    return false;
}

/** Multiplies first pair of arguments, and second pair, returning which result is smaller.
 * @return Gives -1 if first pair multiplies to smaller value, 1 if it's the second; if equal, gives 0.
 */
static sbyte path_compare_multiplications(long mul1a, long mul1b, long mul2a, long mul2b)
{
    long long mul1,mul2;
    mul1 = (long long)mul1a * (long long)mul1b;
    mul2 = (long long)mul2a * (long long)mul2b;
    if (mul1 > mul2)
        return 1;
    if (mul1 < mul2)
        return -1;
    return 0;
}

long triangle_area1(TrTriangId tri)
{
    struct TrTriangle *p_tri;
    struct TrPoint *p_point1;
    struct TrPoint *p_point2;
    struct TrPoint *p_point3;
    long long area;

    p_tri = &triangulation[0].Triangles[tri];
    p_point1 = &triangulation[0].Points[p_tri->point[0]];
    p_point2 = &triangulation[0].Points[p_tri->point[1]];
    p_point3 = &triangulation[0].Points[p_tri->point[2]];
    area = (p_point3->y - p_point1->y) * (p_point2->x - p_point1->x)
       - (p_point3->x - p_point1->x) * (p_point2->y - p_point1->y);
    return llabs(area);
}

sbyte triangle_divide_areas_differ(TrTriangId tri,
  TrTipId cor1, TrTipId cor2, TrCoord pt_x, TrCoord pt_y)
{
    struct TrTriangle *p_tri;
    struct TrPoint *p_point1;
    struct TrPoint *p_point2;

    p_tri = &triangulation[0].Triangles[tri];

    p_point1 = &triangulation[0].Points[p_tri->point[cor1]];
    p_point2 = &triangulation[0].Points[p_tri->point[cor2]];

    return path_compare_multiplications(
      pt_y - p_point2->y, p_point2->x - p_point1->x,
      pt_x - p_point2->x, p_point2->y - p_point1->y);
}

sbyte compare_point_cross_distances(TrPointId pt1, TrPointId pt2, TrPointId pt3)
{
    struct TrPoint *p_point1;
    struct TrPoint *p_point2;
    struct TrPoint *p_point3;

    p_point1 = &triangulation[0].Points[pt1];
    p_point2 = &triangulation[0].Points[pt2];
    p_point3 = &triangulation[0].Points[pt3];

    TrCoord delta_ax = p_point3->x - p_point2->x;
    TrCoord delta_ay = p_point3->y - p_point2->y;
    TrCoord delta_bx = p_point1->x - p_point3->x;
    TrCoord delta_by = p_point1->y - p_point3->y;

    return path_compare_multiplications(delta_bx, delta_ay, delta_ax, delta_by);
}

void make_triangle_solid(TrTriangId tri)
{
    struct TrTriangle *p_tri;

    if (selected_triangulation_no != 1)
        return;
    if (tri >= triangulation[0].max_Triangles || tri < triangulation[0].triangle_top)
        return;
    p_tri = &triangulation[0].Triangles[tri];
    p_tri->solid |= (0x04 | 0x02);
}

static void triangle_clear_enter_into_solid_gnd(TrTriangId tri)
{
    struct TrTriangle *p_tri;
    TbBool is_border;
    TrTipId cor;

    p_tri = &triangulation[0].Triangles[tri];
    is_border = triangleptr_is_border(p_tri);

    if ((p_tri->solid & (0x04 | 0x02)) || is_border)
    {
        p_tri->enter &= ~0x07;
        for (cor = 0; cor < 3; cor++)
        {
            struct TrTriangle *p_ctri;
            TrTipId ccor;

            if (p_tri->tri[cor] == -1)
                continue;

            p_ctri = &triangulation[0].Triangles[p_tri->tri[cor]];

            for (ccor = 0; ccor < 3; ccor++)
            {
                if (p_ctri->tri[ccor] == tri) {
                    p_ctri->enter &= ~(1 << ccor);
                    break;
                }
            }
        }
    }
    for (cor = 0; cor < 3; cor++)
    {
        if (p_tri->tri[cor] == -1)
            p_tri->enter &= ~(1 << cor);
    }
}

static void triangle_clear_enter_into_solid_air(TrTriangId tri)
{
    struct TrTriangle *p_tri;
    TbBool is_border;

    p_tri = &triangulation[0].Triangles[tri];
    is_border = triangleptr_is_border(p_tri);

    if (is_border)
    {
        p_tri->solid |= 0x09;
    }
}

void triangulation_clear_enter_into_solid_gnd(void)
{
    TrTriangId tri;

    if (!triangulation[0].tri_initialised)
    {
        LOGERR("triangulation %d not initialized", (int)selected_triangulation_no);
        return;
    }

    for (tri = 0; tri < triangulation[0].ix_Triangles; tri++)
    {
        triangle_clear_enter_into_solid_gnd(tri);
    }
}

void triangulation_clear_enter_into_solid_air(void)
{
    TrTriangId tri;

    if (!triangulation[0].tri_initialised)
    {
        LOGERR("triangulation %d not initialized", (int)selected_triangulation_no);
        return;
    }

    for (tri = 0; tri < triangulation[0].ix_Triangles; tri++)
    {
        triangle_clear_enter_into_solid_air(tri);
    }
}
/******************************************************************************/
