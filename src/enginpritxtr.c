/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file enginpritxtr.c
 *     Primitive textures support required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling primitive textures in 3D world.
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
#include "enginpritxtr.h"

#include "swlog.h"
/******************************************************************************/

ushort prim4_textures_count = 1;
ushort prim_face_textures_count = 1;

/******************************************************************************/
