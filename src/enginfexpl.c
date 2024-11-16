/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginfexpl.c
 *     Make 3D objects explode into a cloud of faces.
 * @par Purpose:
 *     Implement functions for handling object explosions and debrit movement.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     13 Oct 2024 - 06 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginfexpl.h"

#include "swlog.h"
/******************************************************************************/
extern ulong next_ex_face;

void init_free_explode_faces(void)
{
    asm volatile ("call ASM_init_free_explode_faces\n"
        :  :  : "eax" );
}

void animate_explode(void)
{
    asm volatile ("call ASM_animate_explode\n"
        :  :  : "eax" );
}


void draw_explode(void)
{
#if 1
    asm volatile ("call ASM_draw_explode\n"
        :  :  : "eax" );
    return;
#endif
}

/******************************************************************************/
