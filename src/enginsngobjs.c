/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginsngobjs.c
 *     Engine single objects required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling single objects in 3D world.
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
#include "enginsngobjs.h"

#include "bfmemut.h"
#include "swlog.h"
/******************************************************************************/

void refresh_old_object_face_format(struct SingleObjectFace3 *p_objface,
  struct SingleObjectFace3OldV7 *p_oldobjface, ulong fmtver)
{
    int i;

    //TODO make sane matching for old fields
    LbMemoryCopy(p_objface, p_oldobjface, 32);

    for (i = 0; i < 3; i++)
        p_objface->PointNo[i] = p_oldobjface->PointNo[i];
    p_objface->Texture = p_oldobjface->Texture;
    p_objface->Object = p_oldobjface->Object;
    p_objface->FaceNormal = p_oldobjface->FaceNormal;
    p_objface->WalkHeader = p_oldobjface->WalkHeader;
}

void refresh_old_object_face4_format(struct SingleObjectFace4 *p_objface4,
  struct SingleObjectFace4OldV7 *p_oldobjface4, ulong fmtver)
{
    int i;

    //TODO make sane matching for old fields
    LbMemoryCopy(p_objface4, p_oldobjface4, 40);

    for (i = 0; i < 4; i++)
        p_objface4->PointNo[i] = p_oldobjface4->PointNo[i];
    p_objface4->Texture = p_oldobjface4->Texture;
    p_objface4->Object = p_oldobjface4->Object;
    p_objface4->FaceNormal = p_oldobjface4->FaceNormal;
    p_objface4->WalkHeader = p_oldobjface4->WalkHeader;
}

void unkn_object_shift_03(ushort objectno)
{
    asm volatile ("call ASM_unkn_object_shift_03\n"
        : : "a" (objectno));
}

void unkn_object_shift_02(int norm1, int norm2, ushort objectno)
{
    asm volatile ("call ASM_unkn_object_shift_02\n"
        : : "a" (norm1), "d" (norm2), "b" (objectno));
}

/******************************************************************************/
