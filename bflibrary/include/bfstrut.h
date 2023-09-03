/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfstrut.h
 *     Header file for sstrut.c.
 * @par Purpose:
 *     String utility routines wrapper.
 * @par Comment:
 *     Wraps standard c string routines with additional checks. Implements
 *     a few new ones as well.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 Apr 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFSTRUT_H_
#define BFLIBRARY_BFSTRUT_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char lbEmptyString[];


void * LbStringCopy(char *dst, const char *src, const ulong dst_buflen);

/** Appends characters of source to destination, plus a terminating null-character.
 * Prevents string in dst of getting bigger than dst_buflen characters.
 */
void * LbStringConcat(char *dst, const char *src, const ulong dst_buflen);

ulong LbStringLength(const char *str);

void * LbStringToLower(char *str);
void * LbStringToLowerCopy(char *dst, const char *src, const ulong dst_buflen);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFSTRUT_H_
/******************************************************************************/
