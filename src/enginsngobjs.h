/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginsngobjs.h
 *     Header file for enginsngobjs.c.
 * @par Purpose:
 *     Engine single objects required by the 3D engine.
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
#ifndef ENGINSNGOBJS_H
#define ENGINSNGOBJS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct SingleObjectFace3 { // sizeof=32
    short PointNo[3];
    ushort Texture;
    ubyte GFlags;
    ubyte Flags;
    ushort field_A;
    ushort Object; /**< offs=0x0C Index of the SingleObject associated to this face. */
    short Shade0;
    short Shade1;
    short Shade2;
    ushort Light0;
    ushort Light1;
    ushort Light2;
    ushort FaceNormal;
    ushort WalkHeader; /**< offs=0x1C Index of the WalkHeader created for this face. */
    ubyte field_1E;
    ubyte field_1F;
};

struct SingleObjectFace3OldV7 { // sizeof=48
    short PointNo[3];
    ushort Texture;
    ubyte GFlags;
    ubyte Flags;
    ushort field_A;
    ushort Object; /**< offs=0x0C Index of the SingleObject associated to this face. */
    short Shade0;
    short Shade1;
    short Shade2;
    ushort Light0;
    ushort Light1;
    ushort Light2;
    ushort FaceNormalDoubt;
    ubyte field_1C[3];
    ubyte field_1F;
    ubyte field_20[6];
    ushort FaceNormal; /**< offs=0x26 Index of the Normal vector created for this face. */
    ushort WalkHeader; /**< offs=0x28 Index of the WalkHeader created for this face. */
    ubyte field_2A[6];
};

struct SingleObjectFace4 { // sizeof=40
    short PointNo[4];
    ushort Texture;
    ubyte GFlags;
    ubyte Flags;
    ubyte field_C[2];
    ushort Object; /**< offs=0x0E Index of the SingleObject associated to this face. */
    short Shade0;
    short Shade1;
    short Shade2;
    short Shade3;
    short Light0;
    short Light1;
    short Light2;
    short Light3;
    ushort FaceNormal;
    ushort WalkHeader; /**< offs=0x22 Index of the WalkHeader created for this face. */
    ubyte field_24[3];
    ubyte field_27;
};

struct SingleObjectFace4OldV7 { // sizeof=60
    short PointNo[4];
    ushort Texture;
    ubyte GFlags;
    ubyte Flags;
    ushort field_C;
    ubyte field_E[2];
    ushort Object; /**< offs=0x10 Index of the SingleObject associated to this face. */
    short Shade1;
    short Shade2;
    short Shade3;
    short Light0;
    short Light1;
    short Light2;
    short Light3;
    ushort FaceNormalDoubt;
    ubyte field_22[5];
    ubyte field_27;
    ubyte field_28[10];
    ushort FaceNormal; /**< offs=0x32 Index of the Normal vector created for this face. */
    ushort WalkHeader; /**< offs=0x34 Index of the WalkHeader created for this face. */
    ubyte field_36[4];
    ushort field_3A;
};

struct SinglePoint { // sizeof=10
    ushort PointOffset;
    short X;
    short Y;
    short Z;
    ubyte Pad1;
    ubyte Flags;
};

struct Normal { // sizeof=16
    int NX;
    int NY;
    int NZ;
    int LightRatio;
};

struct SingleObject { // sizeof=36
  ushort StartFace;
  ushort NumbFaces;
  ushort NextObject;
  ushort StartFace4;
  ushort NumbFaces4;
  ushort ZScale;
  short OffsetX;
  short OffsetY;
  short OffsetZ;
  short ObjectNo;
  short MapX;
  short MapZ;
  ushort StartPoint;
  ushort EndPoint;
  ubyte field_1C[7];
  ubyte field_23;
};

#pragma pack()
/******************************************************************************/
extern struct SingleObjectFace3 *game_object_faces;
extern struct SingleObjectFace4 *game_object_faces4;
extern struct SinglePoint *game_object_points;
extern struct Normal *game_normals;
extern struct SingleObject *game_objects;
extern ushort next_object_face;
extern ushort next_object_face4;
extern ushort next_object_point;
extern ushort next_normal;
extern ushort next_object;

/** Maps fields from old SingleObjectFace3 struct to the current one.
 */
void refresh_old_object_face_format(struct SingleObjectFace3 *p_objface,
  struct SingleObjectFace3OldV7 *p_oldobjface, ulong fmtver);

/** Maps fields from old SingleObjectFace4 struct to the current one.
 */
void refresh_old_object_face4_format(struct SingleObjectFace4 *p_objface4,
  struct SingleObjectFace4OldV7 *p_oldobjface4, ulong fmtver);

void unkn_object_shift_03(ushort objectno);
void unkn_object_shift_02(int norm1, int norm2, ushort objectno);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
