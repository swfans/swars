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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

#ifdef GLCOREARB
#include <GL/glcorearb.h>
#else
typedef char          GLchar;
typedef unsigned char GLuchar;
typedef int8_t        GLbyte;
typedef uint8_t       GLubyte;
typedef int16_t       GLshort;
typedef uint16_t      GLushort;
typedef int32_t       GLint;
typedef uint32_t      GLuint;
typedef int64_t       GLint64;
typedef uint64_t      GLuint64;
typedef int           GLsizei;
typedef unsigned int  GLbitfield;
typedef unsigned char GLboolean;
typedef float         GLfloat;
typedef double        GLdouble;
typedef ssize_t       khronos_ssize_t;
typedef size_t        khronos_usize_t;
typedef intptr_t      khronos_intptr_t;
typedef uintptr_t     khronos_uintptr_t;
typedef GLint64       khronos_stime_nanoseconds_t;
typedef GLuint64      khronos_utime_nanoseconds_t;
typedef suseconds_t   khronos_stime_microseconds_t;
typedef time_t        khronos_time_t;
#endif

typedef GLuint64 u64;
typedef GLuint   u32;
typedef GLushort u16;
typedef GLubyte  u8;
typedef GLuchar  uc8;
typedef GLchar   c8;
typedef GLint64  i64;
typedef GLint    i32;
typedef GLshort  i16;
typedef GLbyte   i8;

/** Command function result, valid values are of TbErrorCode enumeration. */
typedef int TbResult;

enum TbErrorCode {
    Lb_FAIL                 = -1,
    Lb_OK                   =  0,
    Lb_SUCCESS              =  1,
};

/** Small, single byte boolean. */
typedef GLboolean TbBool;

/** Large boolean. */
typedef u32    DwBool;

typedef size_t TbMemSize;

/** Coordinate on screen. */
typedef i16 TbScreenCoord;

/** 8-bit pixel definition.
 * Represents value of one point on the graphics screen.
 */
typedef u8 TbPixel;

typedef khronos_stime_microseconds_t TbClockMSec;
typedef time_t TbTimeSec;

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GTYPES_H_
/******************************************************************************/
