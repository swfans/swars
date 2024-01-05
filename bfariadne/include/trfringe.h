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

void fringe_init(ubyte *p_map, TrCoord x1, TrCoord y1, TrCoord x2, TrCoord y2);
int fringe_get_rectangle(TrCoord *p_x1, TrCoord *p_y1,
  TrCoord *p_x2, TrCoord *p_y2, ubyte *p_solid);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
