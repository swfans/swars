/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file matrix.c
 *     Routines implementing matrix support.
 * @par Purpose:
 *     Implement functions for handling multi-dimensional matrices.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "matrix.h"
#include "swlog.h"
/******************************************************************************/

void matrix_identity_fill(struct M33 *p_mat)
{
    asm volatile ("call ASM_matrix_identity_fill\n"
        : : "a" (p_mat));
}

void rotate_object_axis(struct M33 *p_base, short xangle, short yangle, short zangle)
{
    asm volatile ("call ASM_rotate_object_axis\n"
        : : "a" (p_base), "d" (xangle), "b" (yangle), "c" (zangle));
}

/******************************************************************************/
