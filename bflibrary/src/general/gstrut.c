/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gstrut.c
 *     String utility routines wrapper.
 * @par Purpose:
 *     Utilities for string handling.
 * @par Comment:
 *     Wraps standard c string routines with additional checks. Implements
 *     a few new ones as well.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 26 Apr 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfstrut.h"
#include <string.h>
#include <ctype.h>

char lbEmptyString[] = "";
/******************************************************************************/

void * LbStringConcat(char *dst, const char *src, const ulong dst_buflen)
{
    int max_num = dst_buflen - strlen(dst);
    if (max_num <= 0) return dst;
    strncat(dst, src, max_num);
    dst[dst_buflen-1] = '\0';
    return dst;
}

void * LbStringCopy(char *dst, const char *src, const ulong dst_buflen)
{
    if (dst_buflen < 1)
        return dst;
    strncpy(dst, src, dst_buflen);
    dst[dst_buflen-1] = '\0';
    return dst;
}

void * LbStringToLowerCopy(char *dst, const char *src, const ulong dst_buflen)
{
    ulong i;
    char chr;
    if (dst_buflen < 1)
        return dst;
    for (i=0; i < dst_buflen; i++)
    {
        chr = tolower(src[i]);
        dst[i] = chr;
        if (chr == '\0')
            break;
    }
    dst[dst_buflen-1] = '\0';
    return dst;
}

void * LbStringToLower(char *str)
{
    ulong i;
    char chr;
    for (i=0; true; i++)
    {
        chr = tolower(str[i]);
        str[i] = chr;
        if (chr == '\0')
            break;
    }
    return str;
}

ulong LbStringLength(const char *str)
{
    if (str == NULL) return 0;
    return strlen(str);
}
/******************************************************************************/
