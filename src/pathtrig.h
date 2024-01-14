/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file pathtrig.h
 *     Header file for pathtrig.c.
 * @par Purpose:
 *     Pathfinding triangulation support required by the 3D engine.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef PATHTRIG_H
#define PATHTRIG_H

#include "bftypes.h"
#include "trstate.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Path;

#pragma pack()
/******************************************************************************/

void triangulation_clear(void);
void triangulation_allocate(int tgnNo, int maxTrigs);
void triangulation_select(int trglno);

void triangulate_map(ubyte *p_map);
void triangulate_area(ubyte *p_map, int x1, int x2, int y1, int y2);

void thin_wall_around_object(ushort obj, int a2);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
