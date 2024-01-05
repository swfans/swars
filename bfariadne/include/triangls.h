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

/** Type for storing triangle index.
 */
typedef short TrTriangId;

/** Type for storing triangle tip/corner index.
 */
typedef short TrTipId;

/** Triangulation triangle struct.
 */
struct TrTriangle {
    TrPointId point[3];
    TrTriangId tri[3];
    short jump;
    ubyte solid;
    ubyte enter;
};

#pragma pack()
/******************************************************************************/

/** Reserve a new triangle from free triangles pool, and return its Id.
 */
TrTriangId tri_new(void);

/** Free a triangle, returning it to the from free triangles pool.
 */
void tri_dispose(TrTriangId tri);

/** Find edge index within given triangle which links it to next triangle.
 */
TrTipId link_find(TrTriangId tri, TrTriangId tri_nx);

/** Returns if given triangle tip/corner matches given coords.
 */
TbBool triangle_tip_equals(TrTriangId tri, TrTipId cor, TrCoord pt_x, TrCoord pt_y);

/** Returns if given triangle has one of its tips/corners on given coords.
 */
TbBool triangle_has_point_coord(TrTriangId tri, TrCoord pt_x, TrCoord pt_y);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
