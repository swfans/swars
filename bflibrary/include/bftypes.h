/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bftypes.h
 *     Header with basic type definitions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_GTYPES_H_
#define BFLIBRARY_GTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <time.h>

#include "bfconfig.h"

#ifdef LB_HAVE_STDINT_H
# include <stdint.h>
#endif

#ifndef false
# define false 0
#endif
#ifndef true
# define true 1
#endif
#ifndef NULL
# define NULL 0
#endif

/** Short disk path max length - restrictive equivalent for FILENAME_MAX.
 *
 * Should be used for relative paths within the application folder, as
 * long as the restriction was taken into account while placing files there.
 */
#define DISKPATH_SIZE    144

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned char ubyte;
typedef signed char sbyte;

enum TbErrorCode {
    Lb_FAIL                 = -1,
    Lb_OK                   =  0,
    Lb_SUCCESS              =  1,
};

/** Command function result, valid values are of TbErrorCode enumeration. */
typedef int TbResult;
/** Small, single byte boolean. */
typedef unsigned char TbBool;
/** Large boolean. */
typedef unsigned long DwBool;

typedef size_t TbMemSize;

/** Coordinate on screen. */
typedef short TbScreenCoord;

/** 8-bit pixel definition.
 * Represents value of one point on the graphics screen.
 */
typedef unsigned char TbPixel;

typedef long long TbClockMSec;
typedef time_t TbTimeSec;

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GTYPES_H_
/******************************************************************************/
