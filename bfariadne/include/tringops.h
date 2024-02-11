/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file tringops.h
 *     Header file for tringops.c.
 * @par Purpose:
 *     Pathfinding related operations concerning multiple triangles.
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
#ifndef ARIADNE_TRINGOPS_H
#define ARIADNE_TRINGOPS_H

#include "bftypes.h"
#include "triangls.h"
#include "trpoints.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/** Splits a triangle into 2 triangles, making the division at given point and given border.
 *
 * To make a proper corner at the new point, the sibling triangle is divided as well.
 *
 * @param tri Index of the triangle to be divided.
 * @param cor Index of the border to de divided; it is an index in Triangle.tri[] array.
 * @param pt_x New tip point X coordinate.
 * @param pt_y New tip point Y coordinate.
 * @return Index of the new triangle, which shares border with the one given in `tri`.
 */
TrPointId edge_split(TrTriangId tri, TrTipId cor, TrCoord pt_x, TrCoord pt_y);
int edge_rotateAC(TrTriangId tri, TrTipId cor);

/** Insert a new point to the triangulation data, dividing the triangles below it.
 *
 * High level point adding function. Performs all operations neccessary to
 * insert the point, and divide the triangles so that the new point is their tip.
 *
 * @param pt_x New point X coordinate.
 * @param pt_y New point Y coordinate.
 * @return Gives true if the insertion succeeded, false otherwise.
 */
TbBool insert_point(TrCoord pt_x, TrCoord pt_y);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
