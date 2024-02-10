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

TrPointId edge_split(TrTriangId tri, TrTipId cor, TrCoord pt_x, TrCoord pt_y);
int edge_rotateAC(TrTriangId tri, TrTipId cor);

//TODO make it static, when possible
TrPointId tri_split3(TrTriangId btri, TrCoord pt_x, TrCoord pt_y);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
