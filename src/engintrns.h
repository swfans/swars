/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engintrns.h
 *     Header file for engintrns.c.
 * @par Purpose:
 *     Math transformations required by the 3D engine.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ENGINTRNS_H
#define ENGINTRNS_H

#include "bftypes.h"
#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

// TODO figure out why we have PolyPoint and EnginePoint
struct EnginePoint {
  struct PolyPoint pp;
  /*long X;
  long Y;
  long TMapX;
  long TMapY;
  long Shade;*/
  long X3d;
  long Y3d;
  long Z3d;
  long DistSqr;
  short EPUnkn24;
  ubyte Flags; // TODO check if it is ushort
};

#pragma pack()
/******************************************************************************/

void transform_point(struct EnginePoint *eptr);

void draw_trigpoly(struct PolyPoint *point_a, struct PolyPoint *point_b, struct PolyPoint *point_c);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
