/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfmath.h
 *     Header file for arctan.c, gtrig.cpp, sqroot.asm.
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
#ifndef BFLIBRARY_BFMATH_H_
#define BFLIBRARY_BFMATH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bftypes.h"

#define LbFPMath_PI 1024
#define LbFPMath_AngleMask 0x7FF
/** Amount of fractional bits in resulting values of trigonometric operations. */
#define LbFPMath_TrigmBits 16

/** Computes square root of given 32-bit integer.
 * Works like sqrt(x), but is way faster. The input value is treated as unsigned.
 */
long LbSqrL(ulong x);

/** Computes angle between negative Y axis and the line that crosses (0,0) and given (x,y).
 *  Uses arctan(x/y) with proper shift to get the angle.
 *  Returning 0 means direction towards negative y; 512 is towards positive x;
 *  1024 towards positive y, and 1536 towards negative x. Output range is between 0 (=0 rad)
 *  and 2048 (=2*pi rad), zero included.
 *  Value of the angle is properly rounded, up or down.
 *
 * @param x
 * @param y
 * @return
 */
long LbArcTanAngle(long x,long n);

extern long lbSinTable[];

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFMATH_H_
/******************************************************************************/
