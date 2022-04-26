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

#if defined(WIN32)||defined(DOS)||defined(GO32)
#include <dos.h>
#include <direct.h>
#endif

#if defined(WIN32)

// Selected declarations frow Win32 API - we don't want to use whole API
// since it influences everything

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINBASEAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif
#endif
#define WINAPI __stdcall

typedef char *PCHAR,*LPCH,*PCH,*NPSTR,*LPSTR,*PSTR;
typedef const char *LPCCH,*PCSTR,*LPCSTR;
typedef unsigned long DWORD;
typedef int WINBOOL,*PWINBOOL,*LPWINBOOL;
#define BOOL WINBOOL
typedef void *PVOID,*LPVOID;
typedef PVOID HANDLE;
#define DECLARE_HANDLE(n) typedef HANDLE n
typedef HANDLE *PHANDLE,*LPHANDLE;

WINBASEAPI DWORD WINAPI GetShortPathNameA(LPCSTR,LPSTR,DWORD);
#define GetShortPathName GetShortPathNameA
WINBASEAPI BOOL WINAPI FlushFileBuffers(HANDLE);
WINBASEAPI DWORD WINAPI GetLastError(void);

#ifdef __cplusplus
}
#endif

#endif // defined(WIN32)

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
/* DISABLED - NOT NEEDED
  if ( mode == Lb_FILE_MODE_NEW )
  {
    LIBLOG("creating file: %s", fname);
    rc = _sopen(fname, _O_WRONLY|_O_CREAT|_O_TRUNC|_O_BINARY, _SH_DENYNO);
    setmode(rc,_O_TRUNC);
    close(rc);
  }
*/
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
#if defined(WIN32)
  int result;
  // Crappy Windows has its own
  result = FlushFileBuffers((HANDLE)handle);
  // It returns 'invalid handle' error sometimes for no reason.. so disabling this error
  if (result != 0)
      return 1;
  result = GetLastError();
  return ((result == 0) || (result == 6));
#else
  // For normal POSIX systems
  // (should also work on Win, as its IEEE standard... but it currently isn't)
  return (ioctl(handle,I_FLUSH,FLUSHRW) != -1);
#endif
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
 * Yeah, right...
 */
static void convert_find_info(struct TbFileFind *ffind)
{
  struct _finddata_t *fdata=&(ffind->Reserved);
  strncpy(ffind->Filename,fdata->name,144);
  ffind->Filename[143]='\0';
#if defined(WIN32)
  GetShortPathName(fdata->name,ffind->AlternateFilename,14);
#else
  strncpy(ffind->AlternateFilename,fdata->name,14);
#endif
  ffind->AlternateFilename[13]='\0';
  if (fdata->size>ULONG_MAX)
    ffind->Length=ULONG_MAX;
  else
    ffind->Length = fdata->size;
  ffind->Attributes = fdata->attrib;
  LbDateTimeDecode(&fdata->time_create,&ffind->CreationDate,&ffind->CreationTime);
  LbDateTimeDecode(&fdata->time_write,&ffind->LastWriteDate,&ffind->LastWriteTime);
}

TbResult LbFileFindFirst(const char *filespec, struct TbFileFind *ffind,unsigned int attributes)
{
    // We skip 'attributes' as Win32 prototypes seem not to use them
    ffind->ReservedHandle = _findfirst(filespec,&(ffind->Reserved));
    int result;
    if (ffind->ReservedHandle == -1)
    {
      result = -1;
    } else
    {
      convert_find_info(ffind);
      result = 1;
    }
    return result;
}

TbResult LbFileFindNext(struct TbFileFind *ffind)
{
    int result;
    if ( _findnext(ffind->ReservedHandle,&(ffind->Reserved)) < 0 )
    {
        _findclose(ffind->ReservedHandle);
        ffind->ReservedHandle = -1;
        result = -1;
    } else
    {
        convert_find_info(ffind);
        result = 1;
    }
    return result;
}

TbResult LbFileFindEnd(struct TbFileFind *ffind)
{
    if (ffind->ReservedHandle != -1)
    {
        _findclose(ffind->ReservedHandle);
    }
    return 1;
}

TbResult LbFileRename(const char *fname_old, const char *fname_new)
{
  int result;
  if ( rename(fname_old,fname_new) )
    result = -1;
  else
    result = 1;
  return result;
}

TbResult LbFileDelete(const char *filename)
{
  int result;
  if ( remove(filename) )
    result = -1;
  else
    result = 1;
  return result;
}

/******************************************************************************/
