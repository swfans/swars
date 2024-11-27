/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginfexpl.h
 *     Header file for enginfexpl.c.
 * @par Purpose:
 *     Make 3D objects explode into a cloud of faces.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     13 Oct 2024 - 06 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ENGINFEXPL_H
#define ENGINFEXPL_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define EXPLODE_FACES_COUNT 1024

struct ExplodeFace3 { // sizeof=46
    ushort Texture;
    ushort Flags;
    ubyte Type;
    ubyte Col;
    short X0;
    short Y0;
    short Z0;
    short X1;
    short Y1;
    short Z1;
    short X2;
    short Y2;
    short Z2;
    short X3;
    short Y3;
    short Z3;
    short PointOffset;
    short Timer;
    short X;
    short Y;
    short Z;
    sbyte DX;
    sbyte DY;
    sbyte DZ;
    sbyte AngleDX;
    sbyte AngleDY;
    sbyte AngleDZ;
};

#pragma pack()
/******************************************************************************/
extern struct ExplodeFace3 ex_faces[EXPLODE_FACES_COUNT];
extern ulong dont_bother_with_explode_faces;

void init_free_explode_faces(void);
void draw_explode(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
