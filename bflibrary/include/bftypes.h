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

#ifndef HAVE_STDINT_H

#if ((ULONG_MAX) == (UINT_MAX))
typedef          long long          s64; /*   signed 64-bit integer */
typedef unsigned long long          u64; /* unsigned 64-bit integer */
#else
typedef               long          s64; /*   signed 64-bit integer */
typedef      unsigned long          u64; /* unsigned 64-bit integer */
#endif

typedef                int          s32; /*   signed 32-bit integer */
typedef       unsigned int          u32; /* unsigned 32-bit integer */
typedef              short          s16; /*   signed 16-bit integer */
typedef     unsigned short          u16; /* unsigned 16-bit integer */
typedef               char           s8; /*   signed  8-bit integer */
typedef      unsigned char           u8; /* unsigned  8-bit integer */

/** Small, single byte boolean. */
typedef      unsigned char       TbBool; /* unsigned  8-bit boolean */

#ifndef false
# define false 0
#endif
#ifndef true
# define true 1
#endif
#ifndef NULL
# define NULL 0
#endif

#else

#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

typedef            int64_t           s64; /*   signed 64-bit integer */
typedef           uint64_t           u64; /* unsigned 64-bit integer */
typedef            int32_t           s32; /*   signed 32-bit integer */
typedef           uint32_t           u32; /* unsigned 32-bit integer */
typedef            int16_t           s16; /*   signed 16-bit integer */
typedef           uint16_t           u16; /* unsigned 16-bit integer */
typedef             int8_t            s8; /*   signed  8-bit integer */
typedef            uint8_t            u8; /* unsigned  8-bit integer */

typedef               bool        TbBool; /*           8-bit boolean */
#endif
typedef             time_t     TbTimeSec; /*  time 32/64-bit integer */
typedef                s64   TbClockMSec; /*   signed 64-bit integer */

/* obsolete definition of ulong  */
/* typedef unsigned long  ulong; */

typedef                u32          uint; /* unsigned 32-bit integer */
typedef                u16        ushort; /* unsigned 16-bit integer */
typedef                 u8         ubyte; /* unsigned  8-bit integer */
typedef                 s8         sbyte; /*   signed  8-bit integer */
typedef      unsigned char         uchar; /* unsigned character type */

typedef             size_t     TbMemSize; /* unsigned 32/64-bit size */

/** Command function result, valid values are of TbErrorCode enumeration. */
typedef                int      TbResult; /*   signed 32-bit integer */

enum TbErrorCode {
    Lb_FAIL                 = -1,
    Lb_OK                   =  0,
    Lb_SUCCESS              =  1,
};

/** Large boolean. */
typedef                u32        DwBool; /* unsigned 32-bit integer */
/** Coordinate on screen. */
typedef                s16 TbScreenCoord; /*   signed 16-bit integer */

/** 8-bit pixel definition.
 * Represents value of one point on the graphics screen.
 */
typedef                 u8       TbPixel; /* unsigned  8-bit integer */      

#pragma pack(1)

struct TbTime {
        u8 Hour;
        u8 Minute;
        u8 Second;
        u8 HSecond;
};

typedef struct TbDate TbDate;

struct TbDate {
        u8  Day;
        u8  Month;
        u16 Year;
        u8  DayOfWeek;
};

typedef struct TbTime TbTime;

#pragma pack()

typedef ulong TbFileHandle;

#if defined(_finddata_t) // GCC Windows API
typedef struct _finddata_t TbFILE_FIND;
#elif defined(DOSFINDTYPE) // Watcom C API
typedef DOSFINDTYPE TbFILE_FIND;
#else // POSIX emulation of the findfirst/findnext API
#define LB_FILE_FIND_SIMULATED
struct _finddata_t {
    struct stat st;
    char name[FILENAME_MAX];
};
typedef struct _finddata_t TbFILE_FIND;
#endif

#pragma pack(1)

struct TbFileFind {
    char Filename[144];
    char AlternateFilename[14];
    unsigned long Attributes;
    unsigned long Length;
    TbDate CreationDate;
    TbTime CreationTime;
    TbDate LastWriteDate;
    TbTime LastWriteTime;
    unsigned long ReservedHandle;
    TbFILE_FIND Reserved;
};

typedef struct TbFileFind TbFileFind;

#pragma pack()

#define LOG_PREFIX_LEN 32

#pragma pack(1)

struct TbLog {
    char Filename[FILENAME_MAX];
    char Prefix[LOG_PREFIX_LEN];
    ulong Flags;
    TbBool Initialised;
    TbBool Created;
    TbBool Suspended;
    ulong Position;
};

#if 0 // original bullfrog structure
struct TbLog { // sizeof=194
    char Filename[144]; // offset=0
    char Prefix[32]; // offset=145
    ulong Flags; // offset=178
    DwBool Initialised; // offset=182
    DwBool Created; // offset=186
    DwBool Suspended; // offset=190
};
#endif

#pragma pack()

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GTYPES_H_
/******************************************************************************/
