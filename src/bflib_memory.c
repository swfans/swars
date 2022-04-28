/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_memory.c
 *     Memory managing routines.
 * @par Purpose:
 *     Memory related routines - allocation and freeing of memory blocks.
 * @par Comment:
 *     Original SW used different functions for allocating low and extended memory.
 *     This is now outdated way, so functions here are simplified to originals.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_memory.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "dos.h"
/******************************************************************************/
#define AVAILABLE_MEMORY (16*1024*1024)
#define TABLE_SIZE 256 /* hardcoded, do not change */

char lbEmptyString[] = "";
/******************************************************************************/

/**
 *
 * Appends characters of source to destination, plus a terminating null-character.
 * Prevents string in dst of getting bigger than maxlen characters.
 */
void * LbStringConcat(char *dst, const char *src, const ulong dst_buflen)
{
    int max_num=dst_buflen-strlen(dst);
    if (max_num<=0) return dst;
    strncat(dst, src, max_num);
    dst[dst_buflen-1]='\0';
    return dst;
}

void * LbStringCopy(char *dst, const char *src, const ulong dst_buflen)
{
    if (dst_buflen < 1)
        return dst;
    strncpy(dst, src, dst_buflen);
    dst[dst_buflen-1]='\0';
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
    dst[dst_buflen-1]='\0';
    return dst;
}

ulong LbStringLength(const char *str)
{
    if (str == NULL) return 0;
    return strlen(str);
}

/******************************************************************************/
