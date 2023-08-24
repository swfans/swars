/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engintrns.c
 *     Math transformations required by the 3D engine.
 * @par Purpose:
 *     Implement functions for transforming positions in 3D world.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "engintrns.h"

#include "swlog.h"
/******************************************************************************/

void transform_point(struct EnginePoint *eptr)
{
    asm volatile ("call ASM_transform_point\n"
        :  : "a" (eptr));
}

/******************************************************************************/
