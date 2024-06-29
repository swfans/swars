/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file dpmi.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Protected Mode Interface calls.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "dpmi.h"
/******************************************************************************/


/******************************************************************************/

void *AllocDOSmem(long size)
{
#if defined(DOS)||defined(GO32)
    DOS_Registers r;

    r.r32.eax = 0x0100; // DPMI allocate DOS memory
    r.r32.ebx = (size + 15) >> 4; // Number of paragraphs requested
    dos_int386(0x31, &r, &r);

    if (r.r32.eflag) // Failed?
        return NULL;
    return MK_FP(r.r32.edx, 0);
#else
    assert(!"not implemented");
    return NULL;
#endif
}

int FreeDOSmem(void *block, uint16_t seg)
{
#if defined(DOS)||defined(GO32)
    DOS_Registers r;

    r.r32.eax = 0x0101; // DPMI free DOS memory
    r.r32.edx = FP_SEG(block); // selector to free
    dos_int386(0x31, &r, &r);

    if (r.r32.eflag) // Failed?
        return -1;
#else
    assert(!"not implemented");
#endif
    return 0;
}

/******************************************************************************/
