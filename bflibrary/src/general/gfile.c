/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gfile.c
 *     File handling routines wrapper.
 * @par Purpose:
 *     OS separation library for file i/o and compression routines.
 *     These should be used for all file access in the game.
 * @par Comment:
 *     Wraps standard c file handling routines. You could say this has no purpose,
 *     but here it is anyway.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bffile.h"

#include "bfdir.h"

int LbFileLengthRnc()
{
// code at 0001:000964a0
}

int LbFileLoadAt()
{
// code at 0001:0009656c
}

int LbFileSaveAt()
{
// code at 0001:0009660c
}

int LbFileStringSearch()
{
// code at 0001:00096658
}

TbResult LbFileMakeFullPath(const short append_cur_dir,
  const char *directory, const char *filename, char *buf, const unsigned long len)
{
  // code at 0001:00096834
  if (filename == NULL)
    { buf[0]='\0'; return -1; }
  unsigned long namestart;
  if ( append_cur_dir )
  {
    if ( LbDirectoryCurrent(buf, len-2) == -1 )
    { buf[0] = '\0'; return -1; }
    namestart = strlen(buf);
    if ( (namestart > 0) && (buf[namestart-1] != '\\') && (buf[namestart-1] != '/'))
    {
      buf[namestart] = '/';
      namestart++;
    }
  } else
  {
    namestart = 0;
  }
  buf[namestart] = '\0';

  if ( directory != NULL )
  {
    int copy_len;
    copy_len = strlen(directory);
    if ( len-2 <= namestart+copy_len-1 )
      return -1;
    memcpy(buf+namestart, directory, copy_len);
    namestart += copy_len-1;
    if ( (namestart > 0) && (buf[namestart-1] != '\\') && (buf[namestart-1] != '/'))
    {
      buf[namestart] = '/';
      namestart++;
    }
    buf[namestart] = '\0';
  }
  if ( strlen(filename)+namestart-1 < len )
  {
    const char *ptr = filename;
    int invlen;
    for (invlen=-1; invlen != 0; invlen--)
    {
     if (*ptr++ == 0)
       { invlen--; break; }
    }
    int copy_len;
    const char *copy_src;
    char *copy_dst;
    copy_len = ~invlen;
    copy_src = &ptr[-copy_len];
    copy_dst = buf;
    for (invlen=-1; invlen != 0; invlen--)
    {
     if (*copy_dst++ == 0)
       { invlen--; break; }
    }
    memcpy(copy_dst-1, copy_src, copy_len);
    return 1;
  }
  return -1;
}

int LbFileCopy()
{
// code at 0001:0009696c
}


/******************************************************************************/
