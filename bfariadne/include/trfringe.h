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
#pragma pack(1)


#pragma pack()
/******************************************************************************/
extern TrCoord fringe_y[256];
extern TrCoord fringe_x1, fringe_y1, fringe_x2, fringe_y2;
extern ubyte *fringe_map;


/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
