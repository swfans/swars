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

#include <assert.h>
#include <string.h>
#include <limits.h>
#include "bfmath.h"
#include "bigmap.h"
#include "enginsngobjs.h"
#include "game.h"
#include "game_data.h"
#include "thing.h"
#include "triangls.h"
#include "tringops.h"
#include "trpoints.h"
#include "trstate.h"
#include "trfringe.h"
#include "swlog.h"
/******************************************************************************/
extern long ixE;
extern long thin_wall_x1, thin_wall_y1;
extern long thin_wall_x2, thin_wall_y2;

#define MAX_THINGS_ON_TILE 200

extern const short MOD3[] ;

void path_init8_unkn3(struct Path *path, int ax8, int ay8, int bx8, int by8, int a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_path_init8_unkn3\n"
        : : "a" (path), "d" (ax8), "b" (ay8), "c" (bx8), "g" (by8), "g" (a6));
}

//TODO temp copy of static func
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

TbBool triangle_contains8(int tri, int x, int y)
{
    TbBool ret;
    asm volatile ("call ASM_triangle_contains8\n"
        : "=r" (ret) : "a" (tri), "d" (x), "b" (y));
    return ret;
}

int pointed_at8(TrFineCoord pt_x, TrFineCoord pt_y, int *r_tri, int *r_cor)
{
#if 1
    int ret;
    asm volatile ("call ASM_pointed_at8\n"
        : "=r" (ret) : "a" (pt_x), "d" (pt_y), "b" (r_tri), "c" (r_cor));
    return ret;
#else
    struct TrTriangle *p_tri;
    struct TrPoint *p_pt;

    TrTipId cor;
    TrTriangId tri;
    TrFineCoord ptAx, ptAy;
    TrFineCoord ptBx, ptBy;

    tri = *r_tri;
    cor = *r_cor;

    if (!tri_is_allocated(tri)) {
        return -1;
    }
    p_tri = &triangulation[0].Triangles[tri];

    p_pt = &triangulation[0].Points[p_tri->point[cor]];
    ptAx = (p_pt->x << 8) - pt_x;
    ptAy = (p_pt->y << 8) - pt_y;
    p_pt = &triangulation[0].Points[p_tri->point[MOD3[cor+2]]];
    ptBx = (p_pt->x << 8) - pt_x;
    ptBy = (p_pt->y << 8) - pt_y;
    TbBool pt_rel;
    pt_rel = path_compare_multiplications(ptBy, ptAx, ptBx, ptAy) > 0;
    TbBool prev_rel;
    long k;
    k = 0;
    while ( 1 )
    {
        prev_rel = pt_rel;
        p_tri = &triangulation[0].Triangles[tri];
        p_pt = &triangulation[0].Points[p_tri->point[MOD3[cor+1]]];
        ptBy = (p_pt->y << 8) - pt_y;
        ptBx = (p_pt->x << 8) - pt_x;
        pt_rel = path_compare_multiplications(ptBy, ptAx, ptBx, ptAy) > 0;

        if ( prev_rel && !pt_rel )
        {
            *r_tri = tri;
            *r_cor = cor;
            return MOD3[cor+1];
        }
        TrTriangId lnk_tri;
        TrTipId lnk_cor;
        lnk_tri = p_tri->tri[cor];
        if (lnk_tri < 0) {
            break;
        }
        lnk_cor = link_find(lnk_tri, tri);
        if (lnk_cor < 0) {
            LOGERR("no tri link from %d to %d", (int)lnk_tri, (int)tri);
            break;
        }
        cor = MOD3[lnk_cor+1];
        tri = lnk_tri;
        k++;
        if (k >= triangulation[0].max_Triangles) {
            LOGERR("Infinite loop detected");
            break;
        }
    }
    return -1;
#endif
}

int triangle_find8(TrFineCoord pt_x, TrFineCoord pt_y)
{
#if 1
    int ret;
    asm volatile ("call ASM_triangle_find8\n"
        : "=r" (ret) : "a" (pt_x), "d" (pt_y));
    return ret;
#else
    int tri; //TODO switch type to TrTriangId
    int remain;

    tri = triangulation[0].last_tri;

    if (!tri_is_allocated(tri))
    {
        for (tri = 0; tri < triangulation[0].ix_Triangles; tri++)
        {
            if (tri_is_allocated(tri) && triangle_contains8(tri, pt_x, pt_y))
                break;
        }
        if (tri >= triangulation[0].ix_Triangles) {
            tri = -1;
        }
        triangulation[0].last_tri = tri;
        return tri;
    }

    remain = 2000;
    while (tri >= 0)
    {
        struct TrTriangle *p_tri;
        TrFineCoord pt0_x, pt0_y;
        TrFineCoord pt1_x, pt1_y;
        TrFineCoord pt2_x, pt2_y;
        TbBool eqA, eqB, eqC;

        remain--;
        if (remain <= 0) {
            tri = -1;
            LOGERR("Cannot find (%d,%d)", pt_x, pt_y);
            break;
        }
        p_tri = &triangulation[0].Triangles[tri];
        {
            struct TrPoint *p_pt;
            p_pt = &triangulation[0].Points[p_tri->point[0]];
            pt0_x = p_pt->x << 8;
            pt0_y = p_pt->y << 8;
            p_pt = &triangulation[0].Points[p_tri->point[1]];
            pt1_x = p_pt->x << 8;
            pt1_y = p_pt->y << 8;
            p_pt = &triangulation[0].Points[p_tri->point[2]];
            pt2_x = p_pt->x << 8;
            pt2_y = p_pt->y << 8;
        }
        eqA = path_compare_multiplications(pt1_x - pt0_x, pt_y - pt1_y, pt_x - pt1_x, pt1_y - pt0_y) > 0;
        eqB = path_compare_multiplications(pt2_x - pt1_x, pt_y - pt2_y, pt_x - pt2_x, pt2_y - pt1_y) > 0;
        eqC = path_compare_multiplications(pt0_x - pt2_x, pt_y - pt0_y, pt_x - pt0_x, pt0_y - pt2_y) > 0;
        if (!eqA && !eqB && !eqC)
            break;

        if (eqA && !eqB && !eqC && p_tri->tri[0] >= 0) {
            tri = p_tri->tri[0];
        } else if (eqB && !eqC && !eqA && p_tri->tri[1] >= 0) {
            tri = p_tri->tri[1];
        } else if (eqC && !eqA && !eqB && p_tri->tri[2] >= 0) {
            tri = p_tri->tri[2];
        } else if (eqA && eqB) {
            int cor, rcor;
            cor = 1;
            rcor = pointed_at8(pt_x, pt_y, &tri, &cor);
            if (rcor < 0) tri = -1;
        } else if (eqB && eqC) {
            int cor, rcor;
            cor = 2;
            rcor = pointed_at8(pt_x, pt_y, &tri, &cor);
            if (rcor < 0) tri = -1;
        } else if (eqC && eqA) {
            int cor, rcor;
            cor = 0;
            rcor = pointed_at8(pt_x, pt_y, &tri, &cor);
            if (rcor < 0) tri = -1;
        }
    }
    if (tri >= 0 && !triangle_contains8(tri, pt_x, pt_y)) {
        tri = -1;
    }
    triangulation[0].last_tri = tri;
    return tri;
#endif
}

int triangle_findSE8(int x, int y)
{
    int ret;
    asm volatile ("call ASM_triangle_findSE8\n"
        : "=r" (ret) : "a" (x), "d" (y));
    return ret;
}

void triangulation_select(int trglno)
{
    asm volatile ("call ASM_triangulation_select\n"
        : : "a" (trglno));
}

void make_edge(int x1, int y1, int x2, int y2)
{
    asm volatile (
      "call ASM_make_edge\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2));
}

int edge_find(int x1, int y1, int x2, int y2, int *ntri1, int *ntri2)
{
    int ret;
    asm volatile (
      "push %6\n"
      "push %5\n"
      "call ASM_edge_find\n"
        : "=r" (ret) : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (ntri1), "g" (ntri2));
    return ret;
}

/**
 * Finds point with given coords in list of triangles.
 * @param pt_x
 * @param pt_y
 * @param r_tri Output pointer for triangle index.
 * @param r_cor Output corner/tip index within the triangle.
 * @return Gives true if a point was found and output pointers set.
 */
TbBool point_find(TrCoord pt_x, TrCoord pt_y, int *r_tri, int *r_cor)
// TODO change types to TrTriangId *r_tri, TrTipId *r_cor
{
#if 1
    int ret;
    asm volatile (
      "call ASM_point_find\n"
        : "=r" (ret) : "a" (pt_x), "d" (pt_y), "b" (r_tri), "c" (r_cor));
    return ret;
#else
    TrTriangId tri;
    TrTipId cor;

    tri = triangle_find8(pt_x << 8, pt_y << 8);
    if (tri == -1) {
        LOGERR("triangle not found at (%d,%d)", (int)pt_x, (int)pt_y);
        return false;
    }

    for (cor = 0; cor < 3; cor++)
    {
        if (triangle_tip_equals(tri, cor, pt_x, pt_y))
        {
            *r_tri = tri;
            *r_cor = cor;
            return true;
        }
    }
    return false;
#endif
}

TbBool insert_point(TrCoord pt_x, TrCoord pt_y)
{
#if 0
    asm volatile (
      "call ASM_insert_point\n"
        : : "a" (pt_x), "d" (pt_y));
    return true;
#else
    TrTriangId tri;

    tri = triangle_find8(pt_x << 8, pt_y << 8);
    if (tri == -1) {
        LOGERR("triangle not found at (%d,%d)", (int)pt_x, (int)pt_y);
        return false;
    }

    if (triangle_has_point_coord(tri, pt_x, pt_y)) {
        return true;
    }

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
#endif
}

void make_clip_list(int x1, int y1, int x2, int y2)
{
    asm volatile (
      "call ASM_make_clip_list\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2));
}

void make_clipped_edges(int en1, int en2)
{
    asm volatile (
      "call ASM_make_clipped_edges\n"
        : : "a" (en1), "d" (en2));
}

void thin_wall(int x1, int y1, int x2, int y2, ubyte en1, ubyte en2)
{
#if 0
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_thin_wall\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (en1), "g" (en2));
#else
    thin_wall_x1 = x1;
    thin_wall_y1 = y1;
    thin_wall_x2 = x2;
    thin_wall_y2 = y2;
    if (!insert_point(x1, y1)) {
        LOGERR("point1 (%d,%d) insertion failed", x1, y1);
        return;
    }
    if (!insert_point(x2, y2)) {
        LOGERR("point2 (%d,%d) insertion failed", x2, y2);
        return;
    }
    if (x1 != x2 || y1 != y2)
    {
        ixE = 0;
        make_clip_list(x1, y1, x2, y2);
        make_clipped_edges(en1, en2);
    }
#endif
}

void brute_fill_rectangle(int x1, int y1, int x2, int y2, ubyte solid)
{
#if 1
    asm volatile (
      "push %4\n"
      "call ASM_brute_fill_rectangle\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (solid));
#else
    int tri;

    for (tri = 0; tri < triangulation[0].ix_Triangles; tri++)
    {
        struct TrTriangle *p_tri;

        if (!tri_is_allocated(tri))
            continue;

        if (!triangle_contained_within_rect_coords(tri, x1, y1, x2, y2))
            continue;

        p_tri = &triangulation[0].Triangles[tri];
        p_tri->solid = solid;
    }
#endif
}

void fill_rectangle(int x1, int y1, int x2, int y2, ubyte solid)
{
#if 1
    asm volatile (
      "push %4\n"
      "call ASM_fill_rectangle\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (solid));
#else
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
#endif
}

void tri_set_rectangle(int x1, int y1, int x2, int y2, ubyte solid)
{
#if 1
    asm volatile (
      "push %4\n"
      "call ASM_tri_set_rectangle\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (solid));
#else
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
#endif
}

void triangulation_initxy(int x1, int y1, int x2, int y2)
{
#if 0
    asm volatile (
      "call ASM_triangulation_initxy\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2));
#else
    int tri, pt;
    struct TrTriangle *p_tri;
    struct TrPoint *p_point;

    triangulation[0].tri_initialised = 1;

    for (tri = 0; tri < triangulation[0].max_Triangles; tri++)
    {
        tri_clear(tri);
    }
    triangulation[0].free_Triangles = -1;

    for (pt = 0; pt < triangulation[0].max_Points; pt++)
    {
        point_clear(pt);
    }
    triangulation[0].free_Points = -1;

    p_point = &triangulation[0].Points[0];
    p_point->x = x1;
    p_point->y = y1;
    p_point = &triangulation[0].Points[1];
    p_point->x = x2;
    p_point->y = y1;
    p_point = &triangulation[0].Points[2];
    p_point->x = x2;
    p_point->y = y2;
    p_point = &triangulation[0].Points[3];
    p_point->x = x1;
    p_point->y = y2;
    triangulation[0].ix_Points = 4;
    triangulation[0].count_Points = 4;

    p_tri = &triangulation[0].Triangles[0];
    p_tri->point[0] = 3;
    p_tri->point[1] = 1;
    p_tri->point[2] = 0;
    p_tri->tri[0] = 1;
    p_tri->tri[1] = -1;
    p_tri->tri[2] = -1;
    p_tri->solid = 0;
    p_tri->enter = 7;
    p_tri = &triangulation[0].Triangles[1];
    p_tri->point[0] = 1;
    p_tri->point[1] = 3;
    p_tri->point[2] = 2;
    p_tri->tri[0] = 0;
    p_tri->tri[1] = -1;
    p_tri->tri[2] = -1;
    p_tri->solid = 0;
    p_tri->enter = 7;
    triangulation[0].ix_Triangles = 2;
    triangulation[0].count_Triangles = 2;
#endif
}

void triangulation_init(void)
{
    int dim_lo, dim_hi;
    dim_lo = -4096;
    dim_hi = 32768+4096+512;

    triangulation[0].triangle_top = triangulation[0].max_Triangles;
    triangulation[0].point_top = triangulation[0].max_Points;
    triangulation_initxy(dim_lo, dim_lo, dim_hi, dim_hi);
    triangulation[0].last_tri = -1;
}

void triangulation_init_edges(void)
{
#if 1
    asm volatile ("call ASM_triangulation_init_edges\n"
        :  :  : "eax" );
#else
    int dim_lo, dim_hi;
    dim_lo = -4096+256;
    dim_hi = 32768+4096+256;

    insert_point(dim_lo, dim_lo);
    insert_point(dim_hi, dim_lo);
    insert_point(dim_lo, dim_hi);
    insert_point(dim_hi, dim_hi);
    make_edge(dim_lo, dim_lo, dim_hi, dim_lo);
    make_edge(dim_hi, dim_lo, dim_hi, dim_hi);
    make_edge(dim_hi, dim_hi, dim_lo, dim_hi);
    make_edge(dim_lo, dim_hi, dim_lo, dim_lo);
#endif
}

/** Part of triangulation allocation.
 */
void triangulation_initialize(void)
{
    asm volatile ("call ASM_triangulation_initialize\n"
        :  :  : "eax" );
}

void triangulation_allocate(int tgnNo, int maxTrigs)
{
    asm volatile (
      "call ASM_triangulation_allocate\n"
        : : "a" (tgnNo), "d" (maxTrigs));
}

void triangulate_area(ubyte *p_map, int x1, int y1, int x2, int y2)
{
#if 1
    asm volatile (
      "push %4\n"
      "call ASM_triangulate_area\n"
        : : "a" (p_map), "d" (x1), "b" (y1), "c" (x2), "g" (y2));
#else
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
#endif
}

void triangulate_map(ubyte *p_map)
{
#if 0
    asm volatile (
      "call ASM_triangulate_map\n"
        : : "a" (p_map));
#else
    triangulate_area(p_map, 0, 0, 256, 256);
#endif
}

void triangulation_clear(void)
{
    triangulation_init();
    triangulation_init_edges();
}

void thin_wall_around_object(ushort obj, ubyte colt)
{
    asm volatile (
      "call ASM_thin_wall_around_object\n"
        : : "a" (obj), "d" (colt));
}

void init_collision_vects(void)
{
    ushort tile_x, tile_z;
    int i, limit;

    limit = get_memory_ptr_allocated_count((void **)&game_col_vects_list);
    for (i = 0; i < limit; i++) {
        game_col_vects_list[i].Vect = 0;
    }
    next_col_vect = 1;
    next_vects_list = 1;
    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            struct MyMapElement *p_mapel;

            p_mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];
            p_mapel->ColHead = 0;
        }
    }
}

/** Adds given walk face to a list of walk items of another face.
 *
 * Can only be called continously for one face, until another face gats it walk head created.
 */
TbBool face_has_walk_item(short face, short walk_face)
{
    struct WalkHeader *p_walk_head;
    ushort wh, wi;

    if (face < 0)
    {
        struct SingleObjectFace4 *p_face;
        p_face = &game_object_faces4[-face];
        wh = p_face->WalkHeader;
    }
    else if (face > 0)
    {
        struct SingleObjectFace3 *p_face;
        p_face = &game_object_faces[face];
        wh = p_face->WalkHeader;
    } else
    {
        return false;
    }

    p_walk_head = &game_walk_headers[wh];

    for (wi = p_walk_head->StartItem;
      wi < p_walk_head->StartItem + p_walk_head->Count; wi++)
    {
        if (game_walk_items[wi] == walk_face)
            return true;
    }
    return false;
}

/** Adds given walk face to a list of walk items of another face.
 *
 * Can only be called continously for one face, until another face gets its walk head created.
 */
void add_face_walk_item(short face, short walk_face)
{
    struct WalkHeader *p_walk_head;
    ushort wh, wi;

    if (face_has_walk_item(face, walk_face)) {
        return;
    }

    if (face < 0)
    {
        struct SingleObjectFace4 *p_face;
        p_face = &game_object_faces4[-face];
        wh = p_face->WalkHeader;
    }
    else if (face > 0)
    {
        struct SingleObjectFace3 *p_face;
        p_face = &game_object_faces[face];
        wh = p_face->WalkHeader;
    } else
    {
        LOGERR("no walk head assigned to face %d", (int)face);
        return;
    }

    p_walk_head = &game_walk_headers[wh];
    assert(p_walk_head->StartItem + p_walk_head->Count == next_walk_item);
    wi = next_walk_item;
    next_walk_item++;

    p_walk_head->Count++;
    game_walk_items[wi] = walk_face;
}

int face_to_object_position(short face, short *x, short *y, short *z)
{
    if (face < 0)
    {
        struct SingleObjectFace4 *p_face;
        struct SingleObject *p_obj;

        p_face = &game_object_faces4[-face];
        p_obj = &game_objects[p_face->Object];
        *x = p_obj->MapX;
        *y = p_obj->OffsetY;
        *z = p_obj->MapZ;
        return 4;
    }
    else if (face > 0)
    {
        struct SingleObjectFace3 *p_face;
        struct SingleObject *p_obj;

        p_face = &game_object_faces[face];
        p_obj = &game_objects[p_face->Object];
        *x = p_obj->MapX;
        *y = p_obj->OffsetY;
        *z = p_obj->MapZ;
        return 3;
    }
    return 0;
}

int add_walk_items_for_face_object(short face, short obj)
{
    short fcobj_x, fcobj_y, fcobj_z;
    short gvobj_x, gvobj_y, gvobj_z;
    short num_points;
    short cface;
    short startface3, endface3;
    short startface4, endface4;
    short cor;

    num_points = face_to_object_position(face, &fcobj_x, &fcobj_y, &fcobj_z);
    {
        struct SingleObject *p_obj;

        p_obj = &game_objects[obj];
        gvobj_x = p_obj->MapX;
        gvobj_y = p_obj->OffsetY;
        gvobj_z = p_obj->MapZ;
        startface3 = p_obj->StartFace;
        endface3 = startface3 + p_obj->NumbFaces;
        startface4 = p_obj->StartFace4;
        endface4 = startface4 + p_obj->NumbFaces4;
    }

    for (cor = 0; cor < num_points; cor++)
    {
        short fcpt_x, fcpt_y, fcpt_z;

        if (face < 0)
        {
            struct SingleObjectFace4 *p_face;
            struct SinglePoint *p_pt;

            p_face = &game_object_faces4[-face];
            p_pt = &game_object_points[p_face->PointNo[cor]];
            fcpt_x = fcobj_x + p_pt->X;
            fcpt_y = fcobj_y + p_pt->Y;
            fcpt_z = fcobj_z + p_pt->Z;
        }
        else if (face > 0)
        {
            struct SingleObjectFace3 *p_face;
            struct SinglePoint *p_pt;

            p_face = &game_object_faces[face];
            p_pt = &game_object_points[p_face->PointNo[cor]];
            fcpt_x = fcobj_x + p_pt->X;
            fcpt_y = fcobj_y + p_pt->Y;
            fcpt_z = fcobj_z + p_pt->Z;
        }

        for (cface = startface3; cface < endface3; cface++)
        {
            struct SingleObjectFace3 *p_face;
            short ccor;
            p_face = &game_object_faces[cface];
            if ((p_face->GFlags & 0x04) == 0) {
                continue;
            }
            if (cface == face) {
                continue;
            }
            if (game_normals[p_face->FaceNormal].NY == 0) {
                continue;
            }
            for (ccor = 0; ccor < 3; ccor++)
            {
                struct SinglePoint *p_pt;
                int delta_x, delta_y, delta_z;

                p_pt = &game_object_points[p_face->PointNo[ccor]];
                delta_x = (gvobj_x + p_pt->X) - fcpt_x;
                delta_y = (gvobj_y + p_pt->Y) - fcpt_y;
                delta_z = (gvobj_z + p_pt->Z) - fcpt_z;

                if (delta_z * delta_z + delta_y * delta_y + delta_x * delta_x < 1900)
                {
                    add_face_walk_item(face, cface);
                    break;
                }
            }
        }

        for (cface = startface4; cface < endface4; cface++)
        {
            struct SingleObjectFace4 *p_face;
            short ccor;

            if (-cface == face) {
                continue;
            }
            p_face = &game_object_faces4[cface];
            if ((p_face->GFlags & 0x04) == 0) {
                continue;
            }
            if (game_normals[p_face->FaceNormal].NY == 0) {
                continue;
            }
            for (ccor = 0; ccor < 4; ccor++)
            {
                struct SinglePoint *p_pt;
                int delta_x, delta_y, delta_z;

                p_pt = &game_object_points[p_face->PointNo[ccor]];
                delta_x = (gvobj_x + p_pt->X) - fcpt_x;
                delta_y = (gvobj_y + p_pt->Y) - fcpt_y;
                delta_z = (gvobj_z + p_pt->Z) - fcpt_z;

                if (delta_z * delta_z + delta_y * delta_y + delta_x * delta_x < 1900)
                {
                    add_face_walk_item(face, -cface);
                    break;
                }
            }
        }
    }
    return 0;
}

void add_walk_items_for_face_thing_objects(short face, struct Thing *p_thing)
{
    ushort beg_obj, end_obj;
    ushort obj;

    beg_obj = p_thing->U.UObject.Object;
    end_obj = beg_obj + p_thing->U.UObject.NumbObjects;
    for (obj = beg_obj; obj < end_obj; obj++) {
        add_walk_items_for_face_object(face, obj);
    }
}

void add_walk_items_for_face_things_near(short x, short y, short z, short radius, short face)
{
    short shift_x, shift_z;
    short range;

    range = ((radius + 127) >> 8);

    for (shift_x = -range; shift_x <= range; shift_x++)
    {
        for (shift_z = -range; shift_z <= range; shift_z++)
        {
            int tile_x, tile_z;
            struct MyMapElement *p_mapel;
            short thing;
            int i;

            tile_x = (x >> 8) + shift_x;
            tile_z = (z >> 8) + shift_z;
            if (tile_x < 0 || tile_x >= MAP_TILE_WIDTH)
                continue;
            if (tile_z < 0 || tile_z >= MAP_TILE_HEIGHT)
                continue;
            p_mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];
            thing = p_mapel->Child;
            for (i = 0; thing != 0 && i < MAX_THINGS_ON_TILE; i++)
            {
                if (thing <= 0) {
                    struct SimpleThing *p_sthing;
                    p_sthing = &sthings[thing];
                    thing = p_sthing->Next;
                } else {
                    struct Thing *p_thing;
                    p_thing = &things[thing];
                    if (p_thing->Type == TT_BUILDING)
                        add_walk_items_for_face_thing_objects(face, p_thing);
                    thing = p_thing->Next;
                }
            }
        }
    }
}

/** Reserves WalkHeader and assigns it to given face.
 *
 * The WalkHeader is initialized to have 0 items starting at given item index.
 */
TbBool prepare_face_for_having_walk_items(short face, ushort walk_item)
{
    struct WalkHeader *p_walk_head;
    ushort new_wh;

    if (face > 0)
    {
        struct SingleObjectFace3 *p_face;

        new_wh = next_walk_header;
        next_walk_header++;
        p_face = &game_object_faces[face];
        p_face->WalkHeader = new_wh;
    }
    else if (face < 0)
    {
        struct SingleObjectFace4 *p_face;

        new_wh = next_walk_header;
        next_walk_header++;
        p_face = &game_object_faces4[-face];
        p_face->WalkHeader = new_wh;
    } else
    {
        return false;
    }
    p_walk_head = &game_walk_headers[new_wh];
    p_walk_head->Count = 0;
    p_walk_head->StartItem = walk_item;
    return true;
}

void add_walk_items_for_face(short face)
{
    short obj_x, obj_y, obj_z;

    prepare_face_for_having_walk_items(face, next_walk_item);
    face_to_object_position(face, &obj_x, &obj_y, &obj_z);
    add_walk_items_for_face_things_near(obj_x, obj_y, obj_z, 31 << 8, face);
}

void generate_walk_items(void)
{
    short face;

    next_walk_header = 1;
    next_walk_item = 1;
    for (face = 1; face < next_object_face; face++)
    {
        if ((game_object_faces[face].GFlags & 0x04) != 0)
            add_walk_items_for_face(face);
    }
    for (face = 1; face < next_object_face4; face++)
    {
        if ((game_object_faces4[face].GFlags & 0x04) != 0)
            add_walk_items_for_face(-face);
    }
}

static void print_walk_items_for_face(short face)
{
    struct WalkHeader *p_walk_head;
    ushort wh, wi;

    if (face > 0)
    {
        struct SingleObjectFace3 *p_face;

        p_face = &game_object_faces[face];
        wh = p_face->WalkHeader;
    }
    else if (face < 0)
    {
        struct SingleObjectFace4 *p_face;

        p_face = &game_object_faces4[-face];
        wh = p_face->WalkHeader;
    } else
    {
        return;
    }
    p_walk_head = &game_walk_headers[wh];
    for (wi = p_walk_head->StartItem;
      wi < p_walk_head->StartItem + p_walk_head->Count; wi++) {
        LOGSYNC("face %d walkface %d", (int)face, (int)game_walk_items[wi]);
    }
}

/** Print walk items into log file, for debug.
 */
void print_walk_items(void)
{
    short face;

    for (face = 1; face < next_object_face; face++)
    {
        if ((game_object_faces[face].GFlags & 0x04) != 0)
            print_walk_items_for_face(face);
    }
    for (face = 1; face < next_object_face4; face++)
    {
        if ((game_object_faces4[face].GFlags & 0x04) != 0)
            print_walk_items_for_face(-face);
    }
}

void set_mapel_col_columns(struct MyMapElement *p_mapel, short setbit, ushort qb)
{
    struct ColColumn *p_ccol;
    ushort ccol;

    if (setbit < 0)
        return;
    ccol = p_mapel->ColumnHead;
    if (ccol == 0)
    {
        int limit;

        limit = get_memory_ptr_allocated_count((void **)&game_col_columns);
        if (next_col_column >= limit) {
            LOGERR("out of col_columns");
            return;
        }
        ccol = next_col_column;
        next_col_column++;

        p_mapel->ColumnHead = ccol;
        p_ccol = &game_col_columns[ccol];
        p_ccol->QBits[0] = 0;
        p_ccol->QBits[1] = 0;
        p_ccol->QBits[2] = 0;
        p_ccol->QBits[3] = 0;
    }
    p_ccol = &game_col_columns[ccol];
    p_ccol->QBits[qb] |= 1 << setbit;
}

ubyte map_coord_to_collision_qbit_index(short x, short z)
{
    ubyte qb;

    if ((x & 0xFF) <= 127) {
        if ((z & 0xFF) <= 127)
            qb = 0;
        else
            qb = 3;
    } else {
        if ((z & 0xFF) <= 127)
            qb = 1;
        else
            qb = 2;
    }
    return qb;
}

void update_mapel_collision_columns_around_triangle(short fcobj_x, short fcobj_y, short fcobj_z,
  struct SinglePoint *p_pt0, struct SinglePoint *p_pt1, struct SinglePoint *p_pt2, ushort flags)
{
    int dist_B, dist_A;
    int incr_B, incr_A;
    int sh_A, sh_B;
    int basept_x, basept_y, basept_z;
    int delta1_x, delta1_y, delta1_z;
    int delta2_x, delta2_y, delta2_z;

    basept_x = fcobj_x + p_pt0->X;
    basept_y = fcobj_y + p_pt0->Y;
    basept_z = fcobj_z + p_pt0->Z;
    delta1_x = p_pt1->X - p_pt0->X;
    delta1_y = p_pt1->Y - p_pt0->Y;
    delta1_z = p_pt1->Z - p_pt0->Z;
    delta2_x = p_pt2->X - p_pt0->X;
    delta2_y = p_pt2->Y - p_pt0->Y;
    delta2_z = p_pt2->Z - p_pt0->Z;
    // Distance between pt0 and pt1, in half-tiles
    dist_A = LbSqrL(delta1_x * delta1_x + delta1_y * delta1_y + delta1_z * delta1_z) >> 7;
    if (dist_A < 2)
        dist_A = 2;
    incr_A = 256 / dist_A;
    // Distance between pt0 and pt2, in half-tiles
    dist_B = LbSqrL(delta2_x * delta2_x + delta2_y * delta2_y + delta2_z * delta2_z) >> 7;
    if (dist_B < 2)
        dist_B = 2;
    incr_B = 256 / dist_B;
    if ((incr_A <= 0) || (incr_B <= 0)) {
        LOGERR("bad increment");
        return;
    }

    for (sh_A = 0; sh_A < 256; sh_A += incr_A)
    {
        for (sh_B = 0; sh_B < 256; sh_B += incr_B)
        {
            struct MyMapElement *p_mapel;
            int ccx, ccy, ccz;
            ushort qb;

            ccx = basept_x + (delta2_x * sh_B >> 8) + (delta1_x * sh_A >> 8);
            ccy = basept_y + (delta2_y * sh_B >> 8) + (delta1_y * sh_A >> 8);
            ccz = basept_z + (delta2_z * sh_B >> 8) + (delta1_z * sh_A >> 8);
            if ((ccx >> 8) < 0 || (ccx >> 8) >= MAP_TILE_WIDTH)
                continue;
            if ((ccz >> 8) < 0 || (ccz >> 8) >= MAP_TILE_HEIGHT)
                continue;

            p_mapel = &game_my_big_map[(ccx >> 8) + (ccz >> 8) * MAP_TILE_WIDTH];
            if ((p_mapel->Alt - 20 <= ccy) && flags) {
                p_mapel->Texture |= 0x8000;
            }
            qb = map_coord_to_collision_qbit_index(ccx, ccz);
            set_mapel_col_columns(p_mapel, (ccy - p_mapel->Alt) >> 8, qb);
        }
    }
}

void update_mapel_collision_columns_around_face(short face, ushort flags)
{
    struct SinglePoint *p_pt0;
    struct SinglePoint *p_pt1;
    struct SinglePoint *p_pt2;
    short fcobj_x, fcobj_y, fcobj_z;
    short num_points;
    short tri;

    num_points = face_to_object_position(face, &fcobj_x, &fcobj_y, &fcobj_z);

    // Do it once for triangular, twice for quad surface
    for (tri = 0; tri < num_points - 2; tri++)
    {
        if (face <= 0)
        {
            struct SingleObjectFace4 *p_face;

            p_face = &game_object_faces4[-face];
            p_pt0 = &game_object_points[p_face->PointNo[0]];
            p_pt1 = &game_object_points[p_face->PointNo[tri+1]];
            p_pt2 = &game_object_points[p_face->PointNo[tri+2]];
        }
        else
        {
            struct SingleObjectFace3 *p_face;

            p_face = &game_object_faces[face];
            p_pt0 = &game_object_points[p_face->PointNo[0]];
            p_pt1 = &game_object_points[p_face->PointNo[1]];
            p_pt2 = &game_object_points[p_face->PointNo[2]];
        }

        update_mapel_collision_columns_around_triangle(fcobj_x, fcobj_y, fcobj_z,
          p_pt0, p_pt1, p_pt2, flags);
    }
}

void update_mapel_collision_columns_around_object(ushort obj, ushort flags)
{
    short startface3, endface3;
    short startface4, endface4;
    short face;

    {
        struct SingleObject *p_obj;

        p_obj = &game_objects[obj];
        startface3 = p_obj->StartFace;
        endface3 = startface3 + p_obj->NumbFaces;
        startface4 = p_obj->StartFace4;
        endface4 = startface4 + p_obj->NumbFaces4;
    }
    for (face = startface3; face < endface3; face++) {
        update_mapel_collision_columns_around_face(face, flags);
    }
    for (face = startface4; face < endface4; face++) {
        update_mapel_collision_columns_around_face(-face, flags);
    }
}

void update_mapel_collision_columns_around_thing_objects(struct Thing *p_thing, ushort flags)
{
    ushort beg_obj, end_obj;
    ushort obj;

    beg_obj = p_thing->U.UObject.Object;
    end_obj = beg_obj + p_thing->U.UObject.NumbObjects;
    for (obj = beg_obj; obj < end_obj; obj++) {
        update_mapel_collision_columns_around_object(obj, flags);
    }
}

void clear_mapel_collision_columns(void)
{
    ushort tile_x, tile_z;

    next_col_column = 1;
    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            struct MyMapElement *p_mapel;

            p_mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];
            p_mapel->ColumnHead = 0;
        }
    }
}

void update_mapel_collision_columns(void)
{
    ushort tile_x, tile_z;

    clear_mapel_collision_columns();

    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            struct MyMapElement *p_mapel;
            short thing;
            int i;

            p_mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];
            thing = p_mapel->Child;
            for (i = 0; thing != 0 && i < MAX_THINGS_ON_TILE; i++)
            {
                if (thing <= 0) {
                    struct SimpleThing *p_sthing;
                    p_sthing = &sthings[thing];
                    thing = p_sthing->Next;
                } else {
                    struct Thing *p_thing;
                    p_thing = &things[thing];
                    if (p_thing->Type == TT_BUILDING)
                        update_mapel_collision_columns_around_thing_objects(p_thing, 0);
                    thing = p_thing->Next;
                }
            }
        }
    }
}

void print_mapel_collision_columns(void)
{
    ushort tile_x, tile_z;

    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            struct MyMapElement *p_mapel;
            struct ColColumn *p_ccol;

            p_mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];
            if (p_mapel->ColumnHead == 0) continue;
            p_ccol = &game_col_columns[p_mapel->ColumnHead];

            LOGSYNC("%02d,%02d qbits %04x %04x %04x %04x", (int)tile_x, (int)tile_z,
              (int)p_ccol->QBits[0], (int)p_ccol->QBits[1],
              (int)p_ccol->QBits[2], (int)p_ccol->QBits[3]);
        }
    }
}

void add_next_col_vect_to_vects_list(short x, short z, short face, ubyte flags)
{
    short tile_x, tile_z;
    ushort next_vl;
    struct MyMapElement *p_mapel;

    tile_x = x >> 8;
    tile_z = z >> 8;
    if (tile_x < 0 || tile_x >= MAP_TILE_WIDTH) {
        return;
    }
    if (tile_z < 0 || tile_z >= MAP_TILE_HEIGHT) {
        return;
    }

    p_mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];

    if (next_col_vect != game_col_vects_list[p_mapel->ColHead].Vect)
    {
        next_vl = next_vects_list;
        game_col_vects_list[next_vl].Vect = next_col_vect;
        game_col_vects_list[next_vl].NextColList = p_mapel->ColHead;
        p_mapel->ColHead = next_vl;
        next_vl++;
        if (flags & 0x01)
        {
            if (face != 0)
                p_mapel->Texture |= 0x8000;
        }
        next_vects_list = next_vl;
    }
}

void add_obj_face_to_col_vect(short x1, short y1, short z1, short x2, short y2, short z2, short face, ushort flags)
{
    int i, limit;

    limit = get_memory_ptr_allocated_count((void **)&game_col_vects);
    if (next_col_vect >= limit) {
        return;
    }
    limit = get_memory_ptr_allocated_count((void **)&game_col_vects_list);
    if (next_vects_list >= limit) {
        return;
    }

    //TODO why generating thin walls here? we have a separate higher level call for that
    //thin_wall(x1 >> 7, z1 >> 7, x2 >> 7, z2 >> 7, 1, 1);
    i = next_col_vect;
    game_col_vects[i].X1 = x1;
    game_col_vects[i].Y1 = y1;
    game_col_vects[i].Z1 = z1;
    game_col_vects[i].X2 = x2;
    game_col_vects[i].Y2 = y2;
    game_col_vects[i].Z2 = z2;
    game_col_vects[i].Face = face;

    int delta_x, delta_z, dist;
    int x, z, step_x, step_z;

    delta_x = x2 - x1;
    delta_z = z2 - z1;
    dist = LbSqrL(delta_x * delta_x + delta_z * delta_z);
    if (dist <= 0) {
        add_next_col_vect_to_vects_list(x1, z1, face, flags);
        return;
    }
    x = x1 << 10;
    z = z1 << 10;
    step_x = (delta_x << 10) / dist;
    step_z = (delta_z << 10) / dist;
    for (; dist >= 0; dist--)
    {
        add_next_col_vect_to_vects_list(x >> 10, z >> 10, face, flags);
        x += step_x;
        z += step_z;
    }
    next_col_vect++;
}

void add_object_face3_to_col_vect(short x, short y, short z, ushort obj, short face, ushort a2)
{
    int alt_cor[4];
    int x_cor[4];
    int y_cor[4];
    int z_cor[4];
    struct SingleObjectFace3 *p_face;
    int cor;

    p_face = &game_object_faces[face];
    for (cor = 0; cor < 3; cor++) {
        struct SinglePoint *p_pt;
        p_pt = &game_object_points[p_face->PointNo[cor]];
        y_cor[cor] = (y + p_pt->Y) >> 3;
    }
    for (cor = 0; cor < 3; cor++) {
        struct SinglePoint *p_pt;
        p_pt = &game_object_points[p_face->PointNo[cor]];
        x_cor[cor] = (x + p_pt->X);
    }
    for (cor = 0; cor < 3; cor++) {
        struct SinglePoint *p_pt;
        p_pt = &game_object_points[p_face->PointNo[cor]];
        z_cor[cor] = (z + p_pt->Z);
    }
    for (cor = 0; cor < 3; cor++) {
        int alt;
        alt = alt_at_point(x_cor[cor], z_cor[cor]);
        alt_cor[cor] = alt >> 8;
    }

    if (alt_cor[0] - 10 < y_cor[0] && alt_cor[0] + 10 > y_cor[0]
      && alt_cor[1] - 10 < y_cor[1] && alt_cor[1] + 10 > y_cor[1]) {
        add_obj_face_to_col_vect(x_cor[0], y_cor[0], z_cor[0],
          x_cor[1], y_cor[1], z_cor[1], (short)face, a2);
    }
    if (alt_cor[0] - 10 < y_cor[0] && alt_cor[0] + 10 > y_cor[0]
      && alt_cor[2] - 10 < y_cor[2] && alt_cor[2] + 10 > y_cor[2]) {
        add_obj_face_to_col_vect(x_cor[0], y_cor[0], z_cor[0],
          x_cor[2], y_cor[2], z_cor[2], (short)face, a2);
    }
    if (alt_cor[1] - 10 < y_cor[1] && alt_cor[1] + 10 > y_cor[1]
      && alt_cor[2] - 10 < y_cor[2] && alt_cor[2] + 10 > y_cor[2]) {
        add_obj_face_to_col_vect(x_cor[1], y_cor[1], z_cor[1],
          x_cor[2], y_cor[2], z_cor[2], face, a2);
    }
}

void add_object_face4_to_col_vect(short x, short y, short z, ushort obj, short face, ushort a2)
{
    int alt_cor[4];
    int x_cor[4];
    int y_cor[4];
    int z_cor[4];
    struct SingleObjectFace4 *p_face4;
    int cor;

    p_face4 = &game_object_faces4[face];
    for (cor = 0; cor < 4; cor++) {
        struct SinglePoint *p_pt;
        p_pt = &game_object_points[p_face4->PointNo[cor]];
        y_cor[cor] = y + p_pt->Y;
    }
    for (cor = 0; cor < 4; cor++) {
        struct SinglePoint *p_pt;
        p_pt = &game_object_points[p_face4->PointNo[cor]];
        x_cor[cor] = x + p_pt->X;
   }
    for (cor = 0; cor < 4; cor++) {
        struct SinglePoint *p_pt;
        p_pt = &game_object_points[p_face4->PointNo[cor]];
        z_cor[cor] = z + p_pt->Z;
    }
    for (cor = 0; cor < 4; cor++) {
        int alt;
        alt = alt_at_point(x_cor[cor], z_cor[cor]);
        alt_cor[cor] = alt >> 8;
    }

    if (-(alt_cor[0] + 10) < y_cor[0] && alt_cor[0] + 10 > y_cor[0]
      && -(alt_cor[1] + 10) < y_cor[1] && alt_cor[1] + 10 > y_cor[1]) {
        add_obj_face_to_col_vect(x_cor[0], y_cor[0], z_cor[0],
          x_cor[1], y_cor[1], z_cor[1], -face, a2);
    }
    if (-(alt_cor[1] + 10) < y_cor[1] && alt_cor[1] + 10 > y_cor[1]
      && -(alt_cor[3] + 10) < y_cor[3] && alt_cor[3] + 10 > y_cor[3]) {
        add_obj_face_to_col_vect(x_cor[1], y_cor[1], z_cor[1],
          x_cor[3], y_cor[3], z_cor[3], -face, a2);
    }
    if (-(alt_cor[3] + 10) < y_cor[3] && alt_cor[3] + 10 > y_cor[3]
    && -(alt_cor[2] + 10) < y_cor[2] && alt_cor[2] + 10 > y_cor[2]) {
        add_obj_face_to_col_vect(x_cor[3], y_cor[3], z_cor[3],
          x_cor[2], y_cor[2], z_cor[2], -face, a2);
    }
    if (-(alt_cor[2] + 10) < y_cor[2] && alt_cor[2] + 10 > y_cor[2]
      && -(alt_cor[0] + 10) < y_cor[0] && alt_cor[0] + 10 > y_cor[0]) {
        add_obj_face_to_col_vect(x_cor[2], y_cor[2], z_cor[2],
          x_cor[0], y_cor[0], z_cor[0], -face, a2);
    }
}

void add_all_object_faces_to_col_vect(ushort obj, ushort a2)
{
    short face;
    short startface3, endface3;
    short startface4, endface4;
    short x, y, z;

    {
        struct SingleObject *p_obj;

        p_obj = &game_objects[obj];
        x = p_obj->MapX;
        z = p_obj->MapZ;
        y = p_obj->OffsetY;
        startface3 = p_obj->StartFace;
        endface3 = startface3 + p_obj->NumbFaces;
        startface4 = p_obj->StartFace4;
        endface4 = startface4 + p_obj->NumbFaces4;
    }
    for (face = startface3; face < endface3; face++)
    {
        add_object_face3_to_col_vect(x, y, z, obj, face, a2);
    }
    for (face = startface4; face < endface4; face++)
    {
        add_object_face4_to_col_vect(x, y, z, obj, face, a2);
    }
}

void generate_collision_vects(void)
{
    ushort tile_x, tile_z;

    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            short thing;
            int i;

            thing = get_mapwho_thing_index(tile_x, tile_z);
            for (i = 0; thing != 0 && i < MAX_THINGS_ON_TILE; i++)
            {
                if (thing <= 0) {
                    struct SimpleThing *p_sthing;
                    p_sthing = &sthings[thing];
                    thing = p_sthing->Next;
                } else {
                    struct Thing *p_thing;
                    p_thing = &things[thing];
                    if (p_thing->Type == TT_BUILDING)
                        add_all_object_faces_to_col_vect(p_thing->U.UObject.Object, 0);
                    thing = p_thing->Next;
                }
            }
        }
    }
}

void thin_wall_around_thing_objects(struct Thing *p_thing, ubyte colt)
{
    ushort beg_obj, end_obj;
    ushort obj;

    beg_obj = p_thing->U.UObject.Object;
    end_obj = beg_obj + p_thing->U.UObject.NumbObjects;
    for (obj = beg_obj; obj < end_obj; obj++) {
        thin_wall_around_object(obj, colt);
    }
}

void generate_thin_walls(void)
{
    ushort tile_x, tile_z;

    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            short thing;
            int i;

            thing = get_mapwho_thing_index(tile_x, tile_z);
            for (i = 0; thing != 0 && i < MAX_THINGS_ON_TILE; i++)
            {
                if (thing <= 0) {
                    struct SimpleThing *p_sthing;
                    p_sthing = &sthings[thing];
                    thing = p_sthing->Next;
                } else {
                    struct Thing *p_thing;
                    p_thing = &things[thing];
                    if (p_thing->Type == TT_BUILDING)
                        thin_wall_around_thing_objects(p_thing, 0);
                    thing = p_thing->Next;
                }
            }
        }
    }
}

void generate_map_triangulation(void)
{
#if 0 // This is done earlier, in triangulation_clear()
    triangulation_init();
    // TODO should this be replaced by triangulation_init_edges()?
    thin_wall(0, 0, 255, 0, 1, 1);
    thin_wall(255, 0, 255, 255, 1, 1);
    thin_wall(255, 255, 0, 255, 1, 1);
    thin_wall(0, 255, 0, 0, 1, 1);
#endif
    init_collision_vects();
    generate_walk_items();
    update_mapel_collision_columns();
    generate_collision_vects();
    generate_thin_walls();
}

/******************************************************************************/
