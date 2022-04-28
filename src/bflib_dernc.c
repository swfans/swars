/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_dernc.c
 *     Rob Northern Computing compression support.
 * @par Purpose:
 *     Decompress RNC data, load disk files with decompression.
 * @par Comment:
 *     Modified version of the Jon Skeet's code.
 * @author   Tomasz Lis, Jon Skeet
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#define INTERNAL
#include "bflib_dernc.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include "bflib_basics.h"
#include "bfmemut.h"
#include "globals.h"
#include "bfmemory.h"
#include "bffile.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
#ifdef __cplusplus
}
#endif
