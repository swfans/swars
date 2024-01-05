/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file trpoints.h
 *     Header file for trpoints.c.
 * @par Purpose:
 *     Triangle points declaration, life cycle and usage.
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
#ifndef ARIADNE_TRPOINTS_H
#define ARIADNE_TRPOINTS_H

#include "bftypes.h"
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#pragma pack(1)

/** Value stored in point Y coord to mark it as unused.
 * Needs to fit TrCoord type.
 */
#define POINT_UNALLOCATED_MARK INT_MIN

/** Type for storing triangulated coordinate.
 */
typedef int TrCoord;

/** Type for storing point index.
 */
typedef short TrPointId;

/** Triangulation point struct.
 */
struct TrPoint {
    TrCoord x;
    TrCoord y;
};

#pragma pack()
/******************************************************************************/

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
