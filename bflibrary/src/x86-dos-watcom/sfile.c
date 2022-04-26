/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sfile.c
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <limits.h>
#include <share.h>
#include <stdio.h>
#include "bflog.h"

#include <dos.h>
#include <direct.h>

TbBool LbFileExists(const char *fname)
{
  return access(fname,F_OK) == 0;
}

long LbFilePosition(TbFileHandle handle)
{
  int result = tell(handle);
  return result;
}

TbFileHandle LbFileOpen(const char *fname, const TbFileOpenMode accmode)
{
  TbFileOpenMode mode = accmode;

  if ( !LbFileExists(fname) )
  {
    LIBLOG("file does not exist: %s", fname);
    if ( mode == Lb_FILE_MODE_READ_ONLY )
      return -1;
    if ( mode == Lb_FILE_MODE_OLD )
      mode = Lb_FILE_MODE_NEW;
  }
  TbFileHandle rc;
  if ( mode == Lb_FILE_MODE_NEW )
  {
    LIBLOG("creating file: %s", fname);
    rc = _sopen(fname, _O_WRONLY|_O_CREAT|_O_TRUNC|_O_BINARY, _SH_DENYNO);
    setmode(rc,_O_TRUNC);
    close(rc);
  }
  rc = -1;
  switch (mode)
  {
  case Lb_FILE_MODE_NEW:
    {
        LIBLOG("LBO_CREAT mode: %s", fname);
        rc = _sopen(fname, O_RDWR|O_CREAT|O_BINARY, SH_DENYNO, S_IREAD|S_IWRITE);
    };break;
  case Lb_FILE_MODE_OLD:
    {
        LIBLOG("LBO_RDWR mode: %s", fname);
        rc = _sopen(fname, O_RDWR|O_BINARY, SH_DENYNO);
    };break;
  case Lb_FILE_MODE_READ_ONLY:
    {
        LIBLOG("LBO_RDONLY mode: %s", fname);
        rc = _sopen(fname, O_RDONLY|O_BINARY, SH_DENYNO);
    };break;
  }
  LIBLOG("out handle = %ld, errno = %d", rc, errno);
  return rc;
}

TbResult LbFileClose(TbFileHandle handle)
{
  // code at 0001:000a13fc
  if ( close(handle) )
    return -1;
  return 1;
}

TbBool LbFileEof(TbFileHandle handle)
{
  if (LbFilePosition(handle) >= LbFileLengthHandle(handle))
    return true;
  return false;
}

TbResult LbFileSeek(TbFileHandle handle, long offset, TbFileSeekMode origin)
{
  // code at 0001:000a1424
  int rc;
  switch (origin)
  {
  case Lb_FILE_SEEK_BEGINNING:
      rc = lseek(handle, offset, SEEK_SET);
      break;
  case Lb_FILE_SEEK_CURRENT:
      rc = lseek(handle, offset, SEEK_CUR);
      break;
  case Lb_FILE_SEEK_END:
      rc = lseek(handle, offset, SEEK_END);
      break;
  default:
      rc = -1;
      break;
  }
  return rc;
}

long LbFileRead(TbFileHandle handle, void *buffer, unsigned long len)
{
  int result;
  //'read' returns (-1) on error
  result = read(handle,buffer,len);
  return result;
}

long LbFileWrite(TbFileHandle handle, const void *buffer, const unsigned long len)
{
  long result;
  result = write(handle, buffer, len);
  return result;
}

TbBool LbFileFlush(TbFileHandle handle)
{
  long result;
  result = fsync(handle);
  return result;
}

long LbFileLengthHandle(TbFileHandle handle)
{
  long result;
  result = filelength(handle);
  return result;
}

long LbFileLength(const char *fname)
{
  TbFileHandle handle;
  handle = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
  long result = handle;
  if ( handle != -1 )
  {
    result = filelength(handle);
    LbFileClose(handle);
  }
  return result;
}

/** @internal
 * Converts file search information from platform-specific into independent form.
 */
static void convert_find_info(struct TbFileFind *ffind)
{
  struct find_t *fdata = &(ffind->Reserved);
  strncpy(ffind->Filename,fdata->name,144);
  ffind->Filename[143] = '\0';
  strncpy(ffind->AlternateFilename,fdata->name,14);
  ffind->AlternateFilename[13] = '\0';
  ffind->Length = fdata->size;
  ffind->Attributes = fdata->attrib;
  ffind->CreationDate.Day = 0;
  ffind->CreationDate.Month = 0;
  ffind->CreationDate.Year = 0;
  ffind->CreationDate.DayOfWeek = 0;
  ffind->CreationTime.HSecond = 0;
  ffind->CreationTime.Second = 0;
  ffind->CreationTime.Minute = 0;
  ffind->CreationTime.Hour = 0;
  ffind->LastWriteDate.Day = fdata->wr_date & 0x1F;
  ffind->LastWriteDate.Month = (fdata->wr_date & 0x1E0u) >> 5;
  ffind->LastWriteDate.Year = ((fdata->wr_date & 0xFE00) >> 9) + 1980;
  ffind->LastWriteDate.DayOfWeek = 0;
  ffind->LastWriteTime.HSecond = 0;
  ffind->LastWriteTime.Second = (fdata->wr_time & 0x1F) << 1;
  ffind->LastWriteTime.Minute = (fdata->wr_time & 0x7E0u) >> 5;
  ffind->LastWriteTime.Hour = (fdata->wr_time & 0xF800u) >> 11;
}

TbResult LbFileFindFirst(const char *filespec, struct TbFileFind *ffind,unsigned int attributes)
{
    ffind->ReservedHandle = -1;
    if (dos_findfirst(filespec, attributes, &(ffind->Reserved)))
        return -1;
    ffind->ReservedHandle = 1;
    convert_find_info(ffind);
    return 1;
}

TbResult LbFileFindNext(struct TbFileFind *ffind)
{
    if (dos_findnext(&(ffind->Reserved)))
        return -1;
    convert_find_info(ffind);
    return 1;
}

TbResult LbFileFindEnd(struct TbFileFind *ffind)
{
    return 1;
}

TbResult LbFileRename(const char *fname_old, const char *fname_new)
{
    if ( rename(fname_old,fname_new) )
        return -1;
    return 1;
}

TbResult LbFileDelete(const char *filename)
{
    if ( remove(filename) )
        return -1;
    return 1;
}

/******************************************************************************/
