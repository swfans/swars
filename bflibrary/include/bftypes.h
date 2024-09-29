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

#if defined(LB_HAVE_STDINT_H)
# include <stdint.h>
#elif defined(LB_HAVE_INTTYPES_H)
# include <inttypes.h>
#endif

// Fixed size int types: autoconf should ensure that we either have the
// intX_t/uintX_t types available in one of generic headers, or we have
// lb_intX_t/lb_uintX_t defined directly by autoconf. With that in mind:

#if !defined(INT8_MAX) && !defined(int8_t)
# if defined(lb_int8_t)
#  define int8_t lb_int8_t
#  define INT8_MAX 0x7f
# else
#  error No definition of int8_t type
# endif
#endif

#if !defined(UINT8_MAX) && !defined(uint8_t)
# if defined(lb_uint8_t)
#  define uint8_t lb_uint8_t
#  define UINT8_MAX 0xff
# else
#  error No definition of uint8_t type
# endif
#endif

#if !defined(INT16_MAX) && !defined(int16_t)
# if defined(lb_int16_t)
#  define int16_t lb_int16_t
#  define INT16_MAX 0x7fff
# else
#  error No definition of int16_t type
# endif
#endif

#if !defined(UINT16_MAX) && !defined(uint16_t)
# if defined(lb_uint16_t)
#  define uint16_t lb_uint16_t
#  define UINT16_MAX 0xffff
# else
#  error No definition of uint16_t type
# endif
#endif

#if !defined(INT32_MAX) && !defined(int32_t)
# if defined(lb_int32_t)
#  define int32_t lb_int32_t
#  define INT32_MAX 0x7fffffff
# else
#  error No definition of int32_t type
# endif
#endif

#if !defined(UINT32_MAX) && !defined(uint32_t)
# if defined(lb_uint32_t)
#  define uint32_t lb_uint32_t
#  define UINT32_MAX 0xffffffff
# else
#  error No definition of uint32_t type
# endif
#endif

#if !defined(INT64_MAX) && !defined(int64_t)
# if defined(lb_int64_t)
#  define int64_t lb_int64_t
#  define INT64_MAX 0x7fffffffffffffff
# else
#  error No definition of int64_t type
# endif
#endif

#if !defined(UINT64_MAX) && !defined(uint64_t)
# if defined(lb_uint64_t)
#  define uint64_t lb_uint64_t
#  define UINT64_MAX 0xffffffffffffffff
# else
#  error No definition of uint64_t type
# endif
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

typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

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
