/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_render.c
 *     Rendering the 3D view elements functions.
 * @par Purpose:
 *     Some functions to be moved to bflibrary.
 * @par Comment:
 *     To be moved to bflibrary tests.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 30 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "bflib_render.h"

#include "bfscreen.h"
#include "bfwindows.h"
#include "bfpalette.h"
#include "bfutility.h"
#include "bfmemory.h"
#include "bfplanar.h"
#include "bfanywnd.h"
#include "bfmouse.h"
#include "bffile.h"
#include "bfpng.h"
#include "bfscrsurf.h"
#include "bfgentab.h"
#include "bfpalcrss.h"
#include "poly.h"
#include "swlog.h"

/******************************************************************************/



/******************************************************************************/
