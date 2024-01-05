/******************************************************************************/
// Bullfrog Ariadne Pathfinding Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file trstate.h
 *     Header file for trstate.c.
 * @par Purpose:
 *     Triangulation state declaration and support.
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
#ifndef ARIADNE_TRSTATE_H
#define ARIADNE_TRSTATE_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#pragma pack(1)

#define TRIANGULATIONS_COUNT 10

struct TrTriangle;
struct TrPoint;

/** Triangulation state structure; stores everything required for tracing paths.
 */
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

#pragma pack()
/******************************************************************************/
/** Whether the triangulation states were initialized.
 */
extern int triangulation_initied; // = 0;

/** Triangulation states, active one and extras available for activation.
 */
extern struct Triangulation triangulation[TRIANGULATIONS_COUNT];

/** Index of selected triangulation state.
 */
extern int selected_triangulation_no; // = -1;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
