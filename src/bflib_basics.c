/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_basics.c
 *     Basic definitions and global routines for all library files.
 * @par Purpose:
 *     Integrates all elements of the library with a common toolkit.
 * @par Comment:
 *     Only simple, basic functions which can be used in every library file.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 22 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_basics.h"
#include "globals.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#if defined(WIN32)||defined(DOS)||defined(GO32)
#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
TbBool emulate_integer_overflow(unsigned short nbits)
{
    return false;
}


/**
 * Returns a signed value, which is equal to val if it fits in nbits.
 * Otherwise, returns max value that can fit in nbits.
 * @param val the value to be saturated.
 * @param nbits Max bits size, including sign bit.
 */
long saturate_set_signed(long long val,unsigned short nbits)
{
  long long max = (1 << (nbits-1)) - 1;
  if (val >= max)
    return max;
  if (val <= -max)
    return -max;
  return val;
}

/**
 * Returns an unsigned value, which is equal to val if it fits in nbits.
 * Otherwise, returns max value that can fit in nbits.
 * @param val the value to be saturated.
 * @param nbits Max bits size, including sign bit.
 */
unsigned long saturate_set_unsigned(unsigned long long val,unsigned short nbits)
{
    unsigned long long max = (1 << (nbits)) - 1;
    if (emulate_integer_overflow(nbits))
        return (val & max);
    if (val >= max)
        return max;
    return val;
}

/******************************************************************************/
#ifdef __cplusplus
}
#endif
