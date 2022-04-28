/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gendian.c
 *     Integer endianness convert function, and bit setting.
 * @par Purpose:
 *     Allows to read integers at given endianness.
 *     Also includes bit flags setting functions.
 * @par Comment:
 *     A set of simple, self-containing routines.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfendian.h"

unsigned long blong (unsigned char *p)
{
    unsigned long n;
    n = p[0];
    n = (n << 8) + p[1];
    n = (n << 8) + p[2];
    n = (n << 8) + p[3];
    return n;
}

unsigned long llong (unsigned char *p)
{
    unsigned long n;
    n = p[3];
    n = (n << 8) + p[2];
    n = (n << 8) + p[1];
    n = (n << 8) + p[0];
    return n;
}

unsigned long bword (unsigned char *p)
{
    unsigned long n;
    n = p[0];
    n = (n << 8) + p[1];
    return n;
}

unsigned long lword (unsigned char *p)
{
    unsigned long n;
    n = p[1];
    n = (n << 8) + p[0];
    return n;
}

void toggle_flag_byte(unsigned char *flags, unsigned char mask)
{
  if ((*flags & mask) == 0)
    *flags |= mask;
  else
    *flags ^= mask;
}

void toggle_flag_dword(unsigned long *flags, unsigned long mask)
{
  if ((*flags & mask) == 0)
    *flags |= mask;
  else
    *flags ^= mask;
}

void set_flag_word(unsigned short *flags, unsigned short mask, short value)
{
  if (value)
    *flags |= mask;
  else
    *flags ^= *flags & mask;
}

void set_flag_byte(unsigned char *flags, unsigned char mask, short value)
{
  if (value)
    *flags |= mask;
  else
    *flags ^= *flags & mask;
}

void set_flag_dword(unsigned long *flags, unsigned long mask, short value)
{
  if (value)
    *flags |= mask;
  else
    *flags ^= *flags & mask;
}
/******************************************************************************/
