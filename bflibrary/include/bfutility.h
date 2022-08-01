/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfutility.h
 *     Header file for gutility.cpp.
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
#ifndef BFLIBRARY_BFUTILITY_H_
#define BFLIBRARY_BFUTILITY_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#endif

/**
 * Seed for LbRandomAnyShort() function; originally named just `seed`.
 */
extern ulong lbSeed;

/**
 * Generates a random number, by using just a simple multiplication
 * and simple addition on the seed. Very simplistic, but fast.
 * Uses well though constants: 3141592621 (pi) for multiplier,
 * 0xD15C (disc) for addition.
 */
ushort LbRandomAnyShort(void);


#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFUTILITY_H_
/******************************************************************************/
