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

#include <stdlib.h>
#include <string.h>
#include "swlog.h"
/******************************************************************************/

#define POINT_UNALLOCATED_MARK INT_MIN

extern int fringe_y[256];
extern int fringe_x1, fringe_y1, fringe_x2, fringe_y2;
extern ubyte *fringe_map;

/** Multiplies first pair of arguments, and second pair, returning which result is smaller.
 * @return Gives -1 if first pair multiplies to smaller value, 1 if it's the second; if equal, gives 0.
 */
sbyte path_compare_multiplications(long mul1a, long mul1b, long mul2a, long mul2b)
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

void fringe_init(ubyte *p_map, int x1, int y1, int x2, int y2)
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

int fringe_scan(int *p_x1, int *p_y1, int *p_x2, int *p_y2)
{
    int x1, y1, x2;
    int x, cx;

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

int fringe_get_rectangle(int *p_x1, int *p_y1, int *p_x2, int *p_y2, ubyte *p_solid)
{
    int frx1, fry1, frx2, fry2;
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

void path_init8_unkn3(struct Path *path, int ax8, int ay8, int bx8, int by8, int a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_path_init8_unkn3\n"
        : : "a" (path), "d" (ax8), "b" (ay8), "c" (bx8), "g" (by8), "g" (a6));
}

int triangle_find8(int pt_x, int pt_y)
{
    int ret;
    asm volatile ("call ASM_triangle_find8\n"
        : "=r" (ret) : "a" (pt_x), "d" (pt_y));
    return ret;
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

TbBool point_equals(int pt, int pt_x, int pt_y)
{
    struct TrPoint *p_point;

    if (pt < 0)
        return false;
    p_point = &triangulation[0].Points[pt];
    return ((p_point->x == pt_x) && (p_point->y == pt_y));
}

long triangle_area1(int tri)
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

/** Returns if given coords can divide triangle into same areas.
 *
 * @param tri Triangle index.
 * @param cor1 First tip/corner of the edge to be divided.
 * @param ncorB Second tip/corner of the edge to be divided.
 * @param pt_x Coord X of the dividing point.
 * @param pt_y Coord Y of the dividing point.
 * @return Zero if areas do not differ; -1 or 1 otherwise.
 */
sbyte triangle_divide_areas_differ(int tri, int cor1, int cor2, int pt_x, int pt_y)
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

TbBool triangle_tip_equals(int tri, int cor, int pt_x, int pt_y)
{
    int pt;
    if (tri < 0)
        return false;
    if ((cor < 0) || (cor >= 3))
        return false;
    pt = triangulation[0].Triangles[tri].point[cor];
    return point_equals(pt, pt_x, pt_y);
}

TbBool triangle_has_point_coord(int tri, int pt_x, int pt_y)
{
    if (triangle_tip_equals(tri, 0, pt_x, pt_y))
        return true;

    if (triangle_tip_equals(tri, 1, pt_x, pt_y))
        return true;

    if (triangle_tip_equals(tri, 2, pt_x, pt_y))
        return true;

    return false;
}

void make_edge(int x1, int y1, int x2, int y2)
{
    //TODO implement
}

int edge_find(int x1, int y1, int x2, int y2, int *ntri1, int *ntri2)
{
    //TODO implement
   return 0;
}

TbBool point_set(int pt, int pt_x, int pt_y)
{
    struct TrPoint *p_point;

    if (pt < 0)
        return false;
    p_point = &triangulation[0].Points[pt];

    p_point->x = pt_x;
    p_point->y = pt_y;
    return true;
}

int point_new(void)
{
    struct TrPoint *p_point;
    int pt;

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

void point_dispose(int pt)
{
    struct TrPoint *p_point;
    int last_pt;

    last_pt = triangulation[0].free_Points;
    p_point = &triangulation[0].Points[pt];
    // Reuse y coord to mark the point as unused
    p_point->y = POINT_UNALLOCATED_MARK;
    triangulation[0].free_Points = pt;
    // Reuse x coord to link unused points into a chain
    p_point->x = last_pt;
    triangulation[0].count_Points--;
}

/** Find edge index within given triangle which links it to next triangle.
 */
int link_find(int tri_cur, int tri_nxt)
{
    struct TrTriangle *p_tri;
    int cor;

    p_tri = &triangulation[0].Triangles[tri_cur];
    if (tri_cur < 0) {
        return -1;
    }
    for (cor = 0; cor < 3; cor++)
    {
        if (p_tri->tri[cor] == tri_nxt) {
            return cor;
        }
    }
    return -1;
}

int point_set_new_or_reuse(int pt_x, int pt_y)
{
    int pt;

    // Cannot reuse points as they don't have reference count
    // This means freeing the point once would damage the second use
#if 0
    pt = allocated_point_search(pt_x, pt_y);
    if (pt >= 0) {
        return pt;
    }
#endif
    pt = point_new();
    if (pt < 0) {
        return -1;
    }
    point_set(pt, pt_x, pt_y);
    return pt;
}

int tri_split3(int tri, int pt_x, int pt_y)
{
    //TODO implement
    return 0;
}

int tri_split2(int tri, int cor, int pt_x, int pt_y, int pt)
{
    //TODO implement
    return 0;
}

int edge_split(int tri, int cor, int pt_x, int pt_y)
{
    struct TrTriangle *p_tri;
    struct TrTriangle *p_tri_sec;
    int pt;
    int tri_sec, cor_sec;
    int tri_sp1, tri_sp2;

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

TbBool insert_point(int pt_x, int pt_y)
{
    int tri;

    tri = triangle_find8(pt_x << 8, pt_y << 8);
    if (tri == -1) {
        LOGERR("triangle not found");
        return false;
    }

    if (triangle_has_point_coord(tri, pt_x, pt_y))
        return true;

    if (triangle_divide_areas_differ(tri, 0, 1, pt_x, pt_y) == 0)
    {
        return edge_split(tri, 0, pt_x, pt_y) >= 0;
    }
    if (triangle_divide_areas_differ(tri, 1, 2, pt_x, pt_y) == 0)
    {
        return edge_split(tri, 1, pt_x, pt_y) >= 0;
    }
    if (triangle_divide_areas_differ(tri, 2, 0, pt_x, pt_y) == 0)
    {
        return edge_split(tri, 2, pt_x, pt_y) >= 0;
    }
    return tri_split3(tri, pt_x, pt_y) >= 0;
}

TbBool tri_point_within_rect_coords(struct TrPoint *p_point, int x1, int y1, int x2, int y2)
{
    if (p_point->x >= x1 && p_point->x <= x2)
    {
        if (p_point->y >= y1 && p_point->y <= y2)
            return true;
    }
    return false;
}

TbBool tri_contained_within_rect_coords(struct TrTriangle *p_tri, int x1, int y1, int x2, int y2)
{
    struct TrPoint *p_point;

    p_point = &triangulation[0].Points[p_tri->point[0]];
    if (!tri_point_within_rect_coords(p_point, x1, y1, x2, y2))
        return false;

    p_point = &triangulation[0].Points[p_tri->point[1]];
    if (!tri_point_within_rect_coords(p_point, x1, y1, x2, y2))
        return false;

    p_point = &triangulation[0].Points[p_tri->point[2]];
    if (!tri_point_within_rect_coords(p_point, x1, y1, x2, y2))
        return false;

    return true;
}

void brute_fill_rectangle(int x1, int y1, int x2, int y2, ubyte solid)
{
    int tri;

    for (tri = 0; tri < triangulation[0].ix_Triangles; tri++)
    {
        struct TrTriangle *p_tri;

        p_tri = &triangulation[0].Triangles[tri];
        if (p_tri->solid == 255)
            continue;

        if (!tri_contained_within_rect_coords(p_tri, x1, y1, x2, y2))
            continue;

        p_tri->solid = solid;
    }
}

void fill_rectangle(int x1, int y1, int x2, int y2, ubyte solid)
{
    int tri1, tri2, tri3, tri4, tri5;
    int area_r, area_t;

    area_r = (y2 - y1) * 2 * (x2 - x1);
    area_t = 0;

    edge_find(x1, y1, x1, y2, &tri1, &tri5);
    {
        triangulation[0].Triangles[tri1].solid = solid;
        area_t += triangle_area1(tri1);
    }
    if (area_t == area_r)
        return;

    edge_find(x2, y2, x2, y1, &tri2, &tri5);
    if (tri2 != tri1)
    {
        triangulation[0].Triangles[tri2].solid = solid;
        area_t += triangle_area1(tri2);
    }
    if (area_t == area_r)
        return;

    edge_find(x2, y1, x1, y1, &tri3, &tri5);
    if (tri3 != tri1 && tri3 != tri2)
    {
        triangulation[0].Triangles[tri3].solid = solid;
        area_t += triangle_area1(tri3);
    }
    if (area_t == area_r)
        return;

    edge_find(x1, y2, x2, y2, &tri4, &tri5);
    if (tri4 != tri1 && tri4 != tri2 && tri4 != tri3)
    {
        triangulation[0].Triangles[tri4].solid = solid;
        area_t += triangle_area1(tri4);
    }
    if (area_t == area_r)
        return;

    brute_fill_rectangle(x1, y1, x2, y2, solid);
}

void tri_set_rectangle(int x1, int y1, int x2, int y2, ubyte solid)
{
    int sx1, sy1, sx2, sy2;

    sx1 = x1;
    sy1 = y1;
    sx2 = x2;
    sy2 = y2;
    if (x1 > x2) {
        sx1 = x2;
        sx2 = x1;
    }
    if (y1 > y2) {
        sy1 = y2;
        sy2 = y1;
    }
    insert_point(sx1, sy1);
    insert_point(sx2, sy1);
    make_edge(sx1, sy1, sx2, sy1);
    insert_point(sx2, sy2);
    make_edge(sx2, sy1, sx2, sy2);
    insert_point(sx1, sy2);
    make_edge(sx2, sy2, sx1, sy2);
    make_edge(sx1, sy2, sx1, sy1);
    fill_rectangle(sx1, sy1, sx2, sy2, solid);
}

void triangulation_initxy(int x1, int y1, int x2, int y2)
{
    //TODO implement
}

void triangulation_init(void)
{
    triangulation[0].triangle_top = triangulation[0].max_Triangles;
    triangulation[0].point_top = triangulation[0].max_Points;
    triangulation_initxy(-4096, -4096, 37376, 37376);
    triangulation[0].last_tri = -1;
}

void triangulation_init_edges(void)
{
    insert_point(-3840, -3840);
    insert_point(37120, -3840);
    insert_point(-3840, 37120);
    insert_point(37120, 37120);
    make_edge(-3840, -3840, 37120, -3840);
    make_edge(37120, -3840, 37120, 37120);
    make_edge(37120, 37120, -3840, 37120);
    make_edge(-3840, 37120, -3840, -3840);
}

void triangulate_area(ubyte *p_map, int x1, int y1, int x2, int y2)
{
    ubyte solid;

    if (!triangulation[0].tri_initialised)
    {
        triangulation[0].tri_initialised = 1;
        triangulation_init();
    }
    tri_set_rectangle(x1 << 7, y1 << 7, x2 << 7, y2 << 7, 0);
    fringe_init(p_map, x1, y1, x2, y2);

    while (fringe_get_rectangle(&x1, &y1, &x2, &y2, &solid))
    {
        if (solid == 0)
            continue;
        tri_set_rectangle(x1 << 7, y1 << 7, x2 << 7, y2 << 7, solid);
    }
}

void triangulate_map(ubyte *p_map)
{
    triangulate_area(p_map, 0, 0, 256, 256);
}

void triangulation_clear(void)
{
    triangulation_init();
    triangulation_init_edges();
}

/******************************************************************************/
