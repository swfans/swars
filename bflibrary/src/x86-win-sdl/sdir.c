/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sdir.c
 *     Directory handling routines wrapper.
 * @par Purpose:
 *     OS separation library for directory handling routines.
 * @par Comment:
 *     Wraps standard c or UNIX directory handling functions.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfdir.h"

#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

TbResult LbDirectoryChange(const char *path)
{
  if ( chdir(path) )
    return -1;
  return 1;
}

TbResult LbDirectoryCreate(const char *path)
{
  if ( mkdir(path) )
    return -1;
  return 1;
}

TbResult LbDirectoryCurrent(char *buf, unsigned long buflen)
{
    if ( getcwd(buf,buflen) == NULL )
      return -1;
    if ( buf[1] == ':' )
      // Unsafe to use strcpy() on overlapping buffers, but we can memmove().
      memmove(buf, buf+2, strlen(buf+2) + 1);
    int len = strlen(buf);
    if ( len > 1 )
    {
      if ( (buf[len-1] == '\\') || (buf[len-1] == '/') )
        buf[len-1] = '\0';
    }
    return 1;
}

TbBool LbDirectoryExists(const char *dirname)
{
  struct stat stbuf;

  if ( access(dirname, 0) || stat(dirname, &stbuf) == -1 )
    return false;
  return S_ISDIR(stbuf.st_mode);
}

TbResult LbDirectoryRemove(const char *path)
{
  if ( rmdir(path) )
    return -1;
  return 1;
}

/******************************************************************************/
