/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file delaunay.h
 *     Header file for delaunay.c.
 * @par Purpose:
 *     Triangulation optimization support using Delaunay algorithm.
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
#ifndef ARIADNE_DELAUNAY_H
#define ARIADNE_DELAUNAY_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

/** Optimize the triangulation data using Delaunay mesh generation.
 *
 * The map triangulation data is prepared by adding points and dividing
 * nearby triangles. This leads to some triangles being unnecesarily
 * long, and makes some paths go through much more triangles than the
 * optimal mesh would require. This function re-geerates the mesh to
 * optimal state, using Delaunay algoithm.
 */
void delaunay_step(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
