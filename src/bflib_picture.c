/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_picture.c
 *     Pictures handling routines.
 * @par Purpose:
 *     Functions to save or load pictures.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Oct 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_picture.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <png.h>

#include "bflib_basics.h"
#include "bfmemory.h"
#include "bflib_video.h"
#include "bffile.h"
#include "bfdir.h"
#include "game_data.h"


/******************************************************************************/
