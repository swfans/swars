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

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#pragma pack(1)

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

TbBool point_set(TrPointId pt, TrCoord pt_x, TrCoord pt_y);
TrPointId point_new(void);
void point_dispose(TrPointId pt);
TrPointId point_set_new_or_reuse(TrCoord pt_x, TrCoord pt_y);

TbBool point_equals(TrPointId pt, TrCoord pt_x, TrCoord pt_y);
TbBool point_within_rect_coords(TrPointId pt, TrCoord x1,
  TrCoord y1, TrCoord x2, TrCoord y2);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
