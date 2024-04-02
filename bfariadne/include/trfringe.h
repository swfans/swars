/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file trfringe.h
 *     Header file for trfringe.c.
 * @par Purpose:
 *     Triangulation Fringe support and usage.
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
#ifndef ARIADNE_TRFRINGE_H
#define ARIADNE_TRFRINGE_H

#include "bftypes.h"
#include "trpoints.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/** Initialize the fringe map.
 *
 * Fringe map can be used to prepare triangulation data for a map divided into
 * tiles. For each map tile, the fringe map can be sub-divided, and therefore
 * provide a better resolution.
 *
 * After fringe map was used to generate triangulation data, the triangulation
 * can be further modified by adding features not aligned to the bounds of
 * tiles.
 *
 * @param p_map Solid blocks map for the subdivided tiles.
 * @param x1 Starting point subdivided tile index, X dimension.
 * @param y1 Starting point subdivided tile index, Y dimension.
 * @param x2 Final point subdivided tile index, X dimension.
 * @param y2 Final point subdivided tile index, Y dimension.
 */
void fringe_init(ubyte *p_map, TrCoord x1, TrCoord y1, TrCoord x2, TrCoord y2);

/** Retrieve size and solidity for a next rectangle based on fringe map.
 *
 * After fringe map has been initialized, rectangles aggregating tiles
 * with the same solid mask can be acquired by calling this function.
 */
int fringe_get_rectangle(TrCoord *p_x1, TrCoord *p_y1,
  TrCoord *p_x2, TrCoord *p_y2, ubyte *p_solid);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
