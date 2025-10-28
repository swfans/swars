/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file enginpriobjs.c
 *     Primitive objects support required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling primitive objects in 3D world.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginpriobjs.h"

#include "bffile.h"
#include "enginpritxtr.h"
#include "swlog.h"
/******************************************************************************/

ushort prim_object_points_count = 1;
ushort prim_object_faces_count = 1;
ushort prim_object_faces4_count = 1;
ushort prim_objects_count = 1;

void read_primveh_obj(const char *fname, int a2)
{
    long firstval;
    TbFileHandle fh;

    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if ( fh == INVALID_FILE )
        return;
    LbFileRead(fh, &firstval, sizeof(long));
    if ( firstval != 1 )
    {
      LbFileRead(fh, &prim_object_points_count, sizeof(ushort));
      LbFileRead(fh, &prim_object_faces_count, sizeof(ushort));
      LbFileRead(fh, &prim_object_faces4_count, sizeof(ushort));
      LbFileRead(fh, &prim_objects_count, sizeof(ushort));
      LbFileRead(fh, &prim4_textures_count, sizeof(ushort));
      LbFileRead(fh, &prim_face_textures_count, sizeof(ushort));
      LbFileRead(fh, &prim_unknprop01, sizeof(ushort));
      LbFileRead(fh, prim_object_points, sizeof(PrimObjectPoint) * prim_object_points_count);
      LbFileRead(fh, prim_object_faces, sizeof(PrimObjectFace) * prim_object_faces_count);
      LbFileRead(fh, prim_object_faces4, sizeof(PrimObjectFace4) * prim_object_faces4_count);
      LbFileRead(fh, prim_objects, sizeof(PrimObject) * prim_objects_count);
      LbFileRead(fh, prim4_textures, sizeof(Prim4Texture) * prim4_textures_count);
      LbFileRead(fh, prim_face_textures, sizeof(PrimFaceTexture) * prim_face_textures_count);
    }
    LbFileClose(fh);
}

void sub_6031C(short tx, short tz, short a3, short ty)
{
    asm volatile ("call ASM_sub_6031C\n"
        : : "a" (tx), "d" (tz), "b" (a3), "c" (ty));
}

/******************************************************************************/
