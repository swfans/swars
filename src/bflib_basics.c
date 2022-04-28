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

// Functions which were previously defined as Inline,
// but redefined for compatibility with both Ansi-C and C++.

/** Return the big-endian longword at p. */
unsigned long blong (unsigned char *p)
{
    unsigned long n;
    n = p[0];
    n = (n << 8) + p[1];
    n = (n << 8) + p[2];
    n = (n << 8) + p[3];
    return n;
}

/** Return the little-endian longword at p. */
unsigned long llong (unsigned char *p)
{
    unsigned long n;
    n = p[3];
    n = (n << 8) + p[2];
    n = (n << 8) + p[1];
    n = (n << 8) + p[0];
    return n;
}

/** Return the big-endian word at p. */
unsigned long bword (unsigned char *p)
{
    unsigned long n;
    n = p[0];
    n = (n << 8) + p[1];
    return n;
}

/* Return the little-endian word at p. */
unsigned long lword (unsigned char *p)
{
    unsigned long n;
    n = p[1];
    n = (n << 8) + p[0];
    return n;
}

/**
 * Toggles a masked bit in the flags field to the value.
 * This version assumes the flag field is 1 byte long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 */
void toggle_flag_byte(unsigned char *flags,unsigned char mask)
{
  if ((*flags & mask) == 0)
    *flags |= mask;
  else
    *flags ^= mask;
}

/**
 * Toggles a masked bit in the flags field to the value.
 * This version assumes the flag field is 4 bytes long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 */
void toggle_flag_dword(unsigned long *flags,unsigned long mask)
{
  if ((*flags & mask) == 0)
    *flags |= mask;
  else
    *flags ^= mask;
}

/**
 * Sets a masked bit in the flags field to the value.
 * This version assumes the flag field is 2 bytes long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 * @param value The new logic value.
 */
void set_flag_word(unsigned short *flags,unsigned short mask,short value)
{
  if (value)
    *flags |= mask;
  else
    *flags ^= *flags & mask;
}

/**
 * Sets a masked bit in the flags field to the value.
 * This version assumes the flag field is 1 byte long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 * @param value The new logic value.
 */
void set_flag_byte(unsigned char *flags,unsigned char mask,short value)
{
  if (value)
    *flags |= mask;
  else
    *flags ^= *flags & mask;
}

/**
 * Sets a masked bit in the flags field to the value.
 * This version assumes the flag field is 4 bytes long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 * @param value The new logic value.
 */
void set_flag_dword(unsigned long *flags,unsigned long mask,short value)
{
  if (value)
    *flags |= mask;
  else
    *flags ^= *flags & mask;
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
