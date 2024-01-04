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

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct TrTriangle {
    ushort point[3];
    short tri[3];
    short jump;
    ubyte solid;
    ubyte enter;
};

struct TrPoint {
    int x;
    int y;
};

struct Triangulation {
    int tri_allocated;
    int tri_initialised;
    int last_tri;
    int ix_Triangles;
    int count_Triangles;
    int free_Triangles;
    int triangle_top;
    int max_Triangles;
    struct TrTriangle *Triangles;
    int ix_Points;
    int count_Points;
    int free_Points;
    int point_top;
    int max_Points;
    struct TrPoint *Points;
};

struct Path;

#pragma pack()
/******************************************************************************/
extern struct Triangulation triangulation[10];
extern long selected_triangulation_no; // = -1;
extern long tri_module_init;

void triangulation_select(int tgnNo);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
