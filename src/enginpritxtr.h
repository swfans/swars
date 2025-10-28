/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file enginpritxtr.h
 *     Header file for enginpritxtr.c.
 * @par Purpose:
 *     Primitive textures support required by the 3D engine.
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
#ifndef ENGINPRITXTR_H
#define ENGINPRITXTR_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

typedef struct {
  char field_0[15];
  char field_F;
} PrimFaceTexture;

typedef struct {
  char field_0[17];
  char field_11;
} Prim4Texture;

#pragma pack()
/******************************************************************************/
extern Prim4Texture *prim4_textures;
extern PrimFaceTexture *prim_face_textures;
extern ushort prim4_textures_count;
extern ushort prim_face_textures_count;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
