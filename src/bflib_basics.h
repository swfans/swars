/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_basics.h
 *     Header file for bflib_basics.c.
 * @par Purpose:
 *     Integrates all elements of the library with a common toolkit.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 22 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_BASICS_H
#define BFLIB_BASICS_H

#include "bftypes.h"

#include <time.h>
#include <stdio.h>

#if defined(WIN32)||defined(DOS)||defined(GO32)
#include <io.h>
#endif

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
// Buffer sizes
// Disk path max length - restrictive equivalent for FILENAME_MAX
#ifndef DISKPATH_SIZE
#define DISKPATH_SIZE    144
#endif
#define LINEMSG_SIZE     160

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
