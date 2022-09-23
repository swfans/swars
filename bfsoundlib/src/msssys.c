/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file msssys.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     SS functions from MSS API.
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

#include "msssys.h"
/******************************************************************************/

extern AILMEMALLOCCB MEM_alloc;
extern AILMEMFREECB MEM_free;

void *AIL_MEM_use_malloc(AILMEMALLOCCB fn)
{
    void *ret = MEM_alloc;
    MEM_alloc = fn;
    return ret;
}

void *AIL_MEM_use_free(AILMEMFREECB fn)
{
    void *ret = MEM_free;
    MEM_free = fn;
    return ret;
}

/******************************************************************************/
