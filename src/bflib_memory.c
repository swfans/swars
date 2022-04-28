/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_memory.c
 *     Memory managing routines.
 * @par Purpose:
 *     Memory related routines - allocation and freeing of memory blocks.
 * @par Comment:
 *     Original SW used different functions for allocating low and extended memory.
 *     This is now outdated way, so functions here are simplified to originals.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_memory.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "dos.h"
/******************************************************************************/
#define AVAILABLE_MEMORY (16*1024*1024)
#define TABLE_SIZE 256 /* hardcoded, do not change */


/******************************************************************************/
