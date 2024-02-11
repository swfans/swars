/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file triangls.h
 *     Header file for triangls.c.
 * @par Purpose:
 *     Pathfinding Triangle declaration, life cycle and usage.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ARIADNE_TRIANGLS_H
#define ARIADNE_TRIANGLS_H

#include "bftypes.h"
#include "trpoints.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#pragma pack(1)

enum TrTriangleEnterFlags {
    /** Triangle in `tri[0]` can be moved to from this triangle. */
    TrEnter_tri0 = 0x01,
    /** Triangle in `tri[1]` can be moved to from this triangle. */
    TrEnter_tri1 = 0x02,
    /** Triangle in `tri[2]` can be moved to from this triangle. */
    TrEnter_tri2 = 0x04,
    /** There is another walkable triangle in `jump` property. */
    TrEnter_has_jump = 0x08,
    /** Triangle in `jump` can be moved to from this triangle, towards the `tri[0]` direction. */
    TrEnter_jump0 = 0x10,
    /** Triangle in `jump` can be moved to from this triangle, towards the `tri[1]` direction. */
    TrEnter_jump1 = 0x20,
    /** Triangle in `jump` can be moved to from this triangle, towards the `tri[2]` direction. */
    TrEnter_jump2 = 0x40,
};

/** Type for storing triangle index.
 */
typedef short TrTriangId;

/** Type for storing triangle tip/corner index.
 */
typedef short TrTipId;

/** Triangulation triangle struct.
 */
struct TrTriangle {
    TrPointId point[3]; /**< offs=0x00 Indexes to TrPoint structs. */
    TrTriangId tri[3]; /**< offs=0x06 Indexes to sibling TrTriangle structs. */
    TrTriangId jump; /**< offs=0x0C Indexes to sibling TrTriangle for vertical movement (up/down). */
    ubyte solid; /**< offs=0x0E Flags whether tiles above this triangle are solid. */
    ubyte enter; /**< offs=0x0F Flags whether sibling triangles can be moved to from this triangle. */
};

#pragma pack()
/******************************************************************************/

/** Reserve a new triangle from free triangles pool, and return its Id.
 */
TrTriangId tri_new(void);

/** Free a triangle, returning it to the from free triangles pool.
 */
void tri_dispose(TrTriangId tri);

/** Clear all properties of the triangle.
 *
 * Prepared for initialization of all triangles, and clearing reserved triangles.
 * Clearing a disposed triangle with this call would break free trangles chain.
 */
void tri_clear(TrTriangId tri);

/** Returns if given triangle is allocated (is not free and is not invalid).
 */
TbBool tri_is_allocated(TrTriangId tri);

/** Find edge index within given triangle which links it to next triangle.
 */
TrTipId link_find(TrTriangId tri, TrTriangId tri_nx);

/** Returns if given triangle tip/corner matches given coords.
 */
TbBool triangle_tip_equals(TrTriangId tri, TrTipId cor, TrCoord pt_x, TrCoord pt_y);

/** Returns if given triangle has one of its tips/corners on given coords.
 */
TbBool triangle_has_point_coord(TrTriangId tri, TrCoord pt_x, TrCoord pt_y);

/** Returns if all points of the triangle are within given coord limits.
 */
TbBool triangle_contained_within_rect_coords(TrTriangId tri,
  TrCoord x1, TrCoord y1, TrCoord x2, TrCoord y2);

/** Computes surface area of the given triangle.
 */
long triangle_area1(TrTriangId tri);

/** Returns if given coords can divide triangle into same areas.
 *
 * @param tri Triangle index.
 * @param cor1 First tip/corner of the edge to be divided.
 * @param cor2 Second tip/corner of the edge to be divided.
 * @param pt_x Coord X of the dividing point.
 * @param pt_y Coord Y of the dividing point.
 * @return Zero if areas do not differ; -1 or 1 otherwise.
 */
sbyte triangle_divide_areas_differ(TrTriangId tri,
  TrTipId cor1, TrTipId cor2, TrCoord pt_x, TrCoord pt_y);

/** Compares particular differences in coords of given points.
 */
sbyte compare_point_cross_distances(TrPointId pt1, TrPointId pt2, TrPointId pt3);

void make_triangle_solid(TrTriangId tri);

/** Link given triangles by their `jump` property.
 */
void triangles_link_by_jump(TrTriangId tri1, TrTriangId tri2);

/** Fix triangulation entries so that entering into solid places is disallowed.
 *
 * Variant for fixing on the ground (walking/riding) triangulation.
 */
void triangulation_clear_enter_into_solid_gnd(void);

/** Fix triangulation entries so that entering into solid places is disallowed.
 *
 * Variant for fixing in the air (flying) triangulation.
 */
void triangulation_clear_enter_into_solid_air(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
