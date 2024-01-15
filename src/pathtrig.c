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

#include <string.h>
#include <limits.h>
#include "bigmap.h"
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

int pointed_at8(int a1, int a2, int *a3, int *a4)
{
    int ret;
    asm volatile ("call ASM_pointed_at8\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (a3), "c" (a4));
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
 * @param rtri Output pointer for triangle index.
 * @param rcor Output corner/tip index within the triangle.
 * @return Gives true if a point was found and output pointers set.
 */
TbBool point_find(TrCoord pt_x, TrCoord pt_y, TrTriangId *rtri, TrTipId *rcor)
{
#if 1
    int ret;
    asm volatile (
      "call ASM_point_find\n"
        : "=r" (ret) : "a" (pt_x), "d" (pt_y), "b" (rtri), "c" (rcor));
    return ret;
#else
    TrTriangId tri;
    TrTipId cor;

    tri = triangle_find8(pt_x << 8, pt_y << 8);
    if (tri < 0)
        return false;

    for (cor = 0; cor < 3; cor++)
    {
        if (triangle_tip_equals(tri, cor, pt_x, pt_y))
        {
            *rtri = tri;
            *rcor = cor;
            return true;
        }
    }
    return false;
#endif
}

TbBool insert_point(int pt_x, int pt_y)
{
#if 1
    asm volatile (
      "call ASM_insert_point\n"
        : : "a" (pt_x), "d" (pt_y));
    return true;
#else
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
#endif
}

void thin_wall(int x1, int y1, int x2, int y2, ubyte en1, ubyte en2)
{
#if 1
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
    insert_point(x1, y1);
    insert_point(x2, y2);
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

        p_tri = &triangulation[0].Triangles[tri];
        if (p_tri->solid == 255)
            continue;

        if (!triangle_contained_within_rect_coords(tri, x1, y1, x2, y2))
            continue;

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
    asm volatile (
      "call ASM_triangulation_initxy\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2));
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
#if 1
    asm volatile ("call ASM_triangulation_init_edges\n"
        :  :  : "eax" );
#else
    insert_point(-3840, -3840);
    insert_point(37120, -3840);
    insert_point(-3840, 37120);
    insert_point(37120, 37120);
    make_edge(-3840, -3840, 37120, -3840);
    make_edge(37120, -3840, 37120, 37120);
    make_edge(37120, 37120, -3840, 37120);
    make_edge(-3840, 37120, -3840, -3840);
#endif
}

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
#if 1
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

void thin_wall_around_object(ushort obj, int a2)
{
    asm volatile (
      "call ASM_thin_wall_around_object\n"
        : : "a" (obj), "d" (a2));
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
            struct MyMapElement *mapel;

            mapel = &game_my_big_map[MAP_TILE_WIDTH * tile_z + tile_x];
            mapel->ColHead = 0;
        }
    }
}

void generate_walk_items(void)
{
    //TODO implement
}

void update_mapel_qbits(void)
{
    //TODO implement
}

void add_all_object_faces_to_col_vect(ushort obj, int a2)
{
    //TODO implement
}

void generate_collision_vects(void)
{
    ushort tile_x, tile_z;

    for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++)
    {
        for (tile_z = 0; tile_z < MAP_TILE_HEIGHT; tile_z++)
        {
            short thing;
            thing = get_mapwho_thing_index(tile_x, tile_z);
            while (thing != 0)
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

void generate_map_triangulation(void)
{
    triangulation_init();
    // TODO should this be replaced by triangulation_init_edges()?
    thin_wall(0, 0, 255, 0, 1, 1);
    thin_wall(255, 0, 255, 255, 1, 1);
    thin_wall(255, 255, 0, 255, 1, 1);
    thin_wall(0, 255, 0, 0, 1, 1);
    init_collision_vects();
    generate_walk_items();
    update_mapel_qbits();
    generate_collision_vects();
}

/******************************************************************************/
