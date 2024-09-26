/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bflib_vidraw.c
 *     Routines to be moved to bflibrary.
 * @par Purpose:
 *     Implement functions from bflibrary but not yet integrated there.
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
#include "bflib_vidraw.h"
#include "bftringl.h"

#include <stdlib.h>
#include <string.h>
#include "bfscreen.h"
#include "bfline.h"
/******************************************************************************/

void LbDrawSlantBox(short X, short Y, ushort a3, ushort a4)
{
    asm volatile (
      "call ASM_LbDrawSlantBox\n"
        :  : "a" (X), "d" (Y), "b" (a3), "c" (a4));
}

/******************************************************************************/
