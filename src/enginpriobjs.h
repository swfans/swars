/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file enginpriobjs.h
 *     Header file for enginpriobjs.c.
 * @par Purpose:
 *     Primitive objects support required by the 3D engine.
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
#ifndef ENGINPRIOBJS_H
#define ENGINPRIOBJS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

typedef struct {
  char field_0[35];
  char field_23;
} PrimObject;

typedef struct {
  char field_0[39];
  char field_27;
} PrimObjectFace4;

typedef struct {
  char field_0[31];
  char field_1F;
} PrimObjectFace;

typedef struct {
  char field_0[9];
  char field_9;
} PrimObjectPoint;

#pragma pack()
/******************************************************************************/
extern PrimObjectPoint *prim_object_points;
extern PrimObjectFace *prim_object_faces;
extern PrimObjectFace4 *prim_object_faces4;
extern PrimObject *prim_objects;
extern ushort prim_object_points_count;
extern ushort prim_object_faces_count;
extern ushort prim_object_faces4_count;
extern ushort prim_objects_count;
extern ushort prim_unknprop01;

void read_primveh_obj(const char *fname, int a2);

void sub_6031C(short tx, short tz, short a3, short ty);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
