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
#ifndef BFLIBRARY_GMATH_H_
#define BFLIBRARY_GMATH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LbFPMath_PI 1024
#define LbFPMath_AngleMask 0x7FF
/** Amount of fractional bits in resulting values of trigonometric operations. */
#define LbFPMath_TrigmBits 16

long LbSqrL(long x);

long LbArcTanAngle(long x,long n);

extern long lbSinTable[];

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GMATH_H_
/******************************************************************************/
