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

#include <string.h>
#include <errno.h>
#include "bfdir.h"
#include "bfendian.h"
#include "rnc_1fm.h"
#include "privbflog.h"

long LbFileLengthRnc(const char *fname)
{
    TbFileHandle handle = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if ( handle == -1 )
        return -1;
    LOGDBG("%s: file opened", fname);
    unsigned char buffer[RNC_HEADER_LEN+1];
    if ( LbFileRead(handle,buffer,RNC_HEADER_LEN) == -1 )
    {
        LOGWARN("%s: cannot read even %d bytes", fname, RNC_HEADER_LEN);
        LbFileClose(handle);
        return -1;
    }
    long flength;
    if (blong(buffer+0) == RNC_SIGNATURE)
    {
        flength = blong(buffer+4);
        LOGDBG("%s: file size from RNC header: %ld bytes", fname, RNC_HEADER_LEN, flength);
    } else
    {
        flength = LbFileLengthHandle(handle);
        LOGDBG("%s: file is not RNC, size: %ld bytes", fname, RNC_HEADER_LEN, flength);
    }
    LbFileClose(handle);
    return flength;
}

long LbFileLoadAt(const char *fname, void *buffer)
{
    long filelength = LbFileLengthRnc(fname);
    TbFileHandle handle = -1;
    if (filelength != -1)
    {
        handle = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    }
    int read_status = -1;
    if (handle != -1)
    {
        read_status = LbFileRead(handle, buffer, filelength);
        LbFileClose(handle);
    }
    if (read_status == -1)
    {
        LOGERR("%s: could not read (expected size %ld): %s", fname, filelength, strerror(errno));
        return -1;
    }
    long unp_length = UnpackM1((unsigned char *)buffer, filelength);
    long result;
    if ( unp_length >= 0 )
    {
        if (unp_length != 0)
          result = unp_length;
        else
          result = filelength;
    } else
    {
        LOGERR("%s: could not decompress: %s", fname, rnc_error(unp_length));
        result = -1;
    }
    return result;
}

long LbFileSaveAt(const char *fname, const void *buffer, ulong len)
{
    TbFileHandle handle;
    int result;

    handle = LbFileOpen(fname, Lb_FILE_MODE_NEW);
    if ( handle == -1 )
        return -1;
    result = LbFileWrite(handle,buffer,len);
    LbFileClose(handle);
    return result;
}

int LbFileStringSearch_UNUSED()
{
// code at 0001:00096658
}

TbResult LbFileMakeFullPath(const TbBool append_cur_dir,
  const char *directory, const char *filename, char *buf, const ulong len)
{
  if (filename == NULL) {
      buf[0]='\0';
      return Lb_FAIL;
  }
  unsigned long namestart;
  if (append_cur_dir)
  {
    if (LbDirectoryCurrent(buf, len-2) == -1) {
        buf[0] = '\0';
        return Lb_FAIL;
    }
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
      return Lb_FAIL;
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
        if (*ptr++ == 0) {
            invlen--;
            break;
        }
    }
    int copy_len;
    const char *copy_src;
    char *copy_dst;
    copy_len = ~invlen;
    copy_src = &ptr[-copy_len];
    copy_dst = buf;
    for (invlen=-1; invlen != 0; invlen--)
    {
        if (*copy_dst++ == 0) {
            invlen--;
            break;
        }
    }
    memcpy(copy_dst-1, copy_src, copy_len);
    return Lb_SUCCESS;
  }
  return Lb_FAIL;
}

int LbFileCopy_UNUSED()
{
// code at 0001:0009696c
}


/******************************************************************************/
