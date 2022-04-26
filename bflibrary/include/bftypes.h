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

typedef char BYTE;
typedef char CBYTE;
typedef uint8_t UBYTE;
typedef int8_t SBYTE;

typedef int16_t WORD;
typedef uint16_t UWORD;
typedef int16_t SWORD;

typedef int32_t LONG;
typedef int32_t SLONG;
typedef uint32_t ULONG;

typedef uint8_t BBOOL;
typedef uint32_t BOOL;

typedef CBYTE *PCBYTE;
typedef UBYTE *PUBYTE;
typedef SBYTE *PSBYTE;

typedef UWORD *PUWORD;
typedef SWORD *PSWORD;

typedef ULONG *PULONG;
typedef SLONG *PSLONG;

typedef void *CALLBACK;

typedef uint32_t REALFAR;

typedef int32_t HTIMER;

enum TbErrorCode {
    Lb_FAIL                 = -1,
    Lb_OK                   =  0,
    Lb_SUCCESS              =  1,
};

/** Command function result, valid values are of TbErrorCode enumeration. */
typedef int TbResult;
typedef unsigned char TbBool;

typedef size_t TbMemSize;

typedef long TbScreenCoord;

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
