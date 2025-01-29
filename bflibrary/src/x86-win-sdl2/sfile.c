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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "privbflog.h"

#if defined(WIN32)||defined(DOS)||defined(GO32)
#include <share.h>
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

WINBASEAPI BOOL WINAPI FlushFileBuffers(HANDLE);
WINBASEAPI DWORD WINAPI GetLastError(void);

#ifdef __cplusplus
}
#endif

#endif // defined(WIN32)

// Globals
#if LB_FILENAME_TRANSFORM
FileNameTransform lbFileNameTransform = NULL;
#endif


TbBool LbFileExists(const char *fname)
{
#if LB_FILENAME_TRANSFORM
    char real_fname[FILENAME_MAX];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname, fname);
        fname = real_fname;
    }
#endif
    return access(fname,F_OK) == 0;
}

long LbFilePosition(TbFileHandle fhandle)
{
    long result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    result = tell(fhandle);
#else
    result = lseek(fhandle, 0, SEEK_CUR);
#endif
    return result;
}

TbFileHandle LbFileOpen(const char *fname, const TbFileOpenMode accmode)
{
    TbFileOpenMode mode = accmode;
#if LB_FILENAME_TRANSFORM
    char real_fname[FILENAME_MAX];
#endif
    TbFileHandle rc;

    if ( !LbFileExists(fname) )
    {
        if ( mode == Lb_FILE_MODE_READ_ONLY )
        {
            LOGERR("file does not exist: \"%s\"", fname);
            return INVALID_FILE;
        }
        LOGSYNC("file does not exist: \"%s\"", fname);
        if ( mode == Lb_FILE_MODE_OLD )
            mode = Lb_FILE_MODE_NEW;
    }

#if LB_FILENAME_TRANSFORM
    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname, fname);
        fname = real_fname;
    }
#endif

/* DISABLED - NOT NEEDED
    if ( mode == Lb_FILE_MODE_NEW )
    {
        LOGSYNC("creating file: %s", fname);
        rc = _sopen(fname, _O_WRONLY|_O_CREAT|_O_TRUNC|_O_BINARY, _SH_DENYNO);
        setmode(rc,_O_TRUNC);
        close(rc);
    }
*/
    rc = INVALID_FILE;
    switch (mode)
    {
    case Lb_FILE_MODE_NEW:
        LOGDBG("LBO_CREAT mode: \"%s\"", fname);
#if defined(WIN32)||defined(DOS)||defined(GO32)
        rc = _sopen(fname, O_RDWR|O_CREAT|O_BINARY, SH_DENYNO, S_IREAD|S_IWRITE);
#else
        rc = open(fname, O_RDWR|O_CREAT, S_IRUSR|S_IRGRP|S_IWUSR|S_IWGRP);
#endif
        break;
    case Lb_FILE_MODE_OLD:
        LOGDBG("LBO_RDWR mode: \"%s\"", fname);
#if defined(WIN32)||defined(DOS)||defined(GO32)
        rc = _sopen(fname, O_RDWR|O_BINARY, SH_DENYNO);
#else
        rc = open(fname, O_RDWR);
#endif
        break;
  case Lb_FILE_MODE_READ_ONLY:
        LOGDBG("LBO_RDONLY mode: \"%s\"", fname);
#if defined(WIN32)||defined(DOS)||defined(GO32)
        rc = _sopen(fname, O_RDONLY|O_BINARY, SH_DENYNO);
#else
        rc = open(fname, O_RDONLY);
#endif
        break;
  }
  LOGSYNC("out handle = %ld, errno = %d", rc, errno);
  return rc; // sopen returns -1 on fail, which is equal to our INVALID_FILE
}

TbResult LbFileClose(TbFileHandle fhandle)
{
  if ( close(fhandle) )
    return Lb_FAIL;
  return 1;
}

TbBool LbFileEof(TbFileHandle fhandle)
{
    if (LbFilePosition(fhandle) >= LbFileLengthHandle(fhandle))
        return true;
    return false;
}

TbResult LbFileSeek(TbFileHandle fhandle, long offset, TbFileSeekMode origin)
{
  int rc;
  switch (origin)
  {
  case Lb_FILE_SEEK_BEGINNING:
      rc = lseek(fhandle, offset, SEEK_SET);
      break;
  case Lb_FILE_SEEK_CURRENT:
      rc = lseek(fhandle, offset, SEEK_CUR);
      break;
  case Lb_FILE_SEEK_END:
      rc = lseek(fhandle, offset, SEEK_END);
      break;
  default:
      rc = Lb_FAIL;
      break;
  }
  return rc;
}

long LbFileRead(TbFileHandle fhandle, void *buffer, unsigned long len)
{
  int result;
  //'read' returns (-1) on error
  result = read(fhandle,buffer,len);
  return result;
}

long LbFileWrite(TbFileHandle fhandle, const void *buffer, const unsigned long len)
{
  long result;
  result = write(fhandle, buffer, len);
  return result;
}

TbBool LbFileFlush(TbFileHandle fhandle)
{
#if defined(WIN32)
    int result;
    // Crappy Windows has its own
    result = FlushFileBuffers((HANDLE)fhandle);
    // It returns 'invalid handle' error sometimes for no reason.. so disabling this error
    if (result != 0)
        return 1;
    result = GetLastError();
    return ((result == 0) || (result == 6));
#else
    // For normal POSIX systems
    // (should also work on Win, as its IEEE standard... but it currently isn't)
# if 1
    return (fsync(fhandle) != -1);
# else
    return (ioctl(fhandle,I_FLUSH,FLUSHRW) != -1); // another way to do the same thing
# endif
#endif
}

long LbFileLengthHandle(TbFileHandle fhandle)
{
    long result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    result = filelength(fhandle);
#else
    struct stat buf;
    fstat(fhandle, &buf);
    result = buf.st_size;
#endif
    return result;
}

long LbFileLength(const char *fname)
{
    long result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    TbFileHandle fhandle;

    fhandle = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    result = fhandle;
    if (fhandle != -1) {
      result = filelength(fhandle);
      LbFileClose(fhandle);
    }
    // error message, if any, should already be printed
#else
    struct stat st;
# if LB_FILENAME_TRANSFORM
    char real_fname[FILENAME_MAX];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname, fname);
        fname = real_fname;
    }
# endif

    if (stat (fname, &st) != 0) {
        LOGERR("while getting stats on \"%s\": %s", fname, strerror(errno));
        result = -1;
    } else {
        result = st.st_size;
    }
#endif

    return result;
}

TbResult LbFileRename(const char *fname_old, const char *fname_new)
{
#if LB_FILENAME_TRANSFORM
    char real_fname_old[FILENAME_MAX];
    char real_fname_new[FILENAME_MAX];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname_old, fname_old);
        lbFileNameTransform(real_fname_new, fname_new);
        fname_old = real_fname_old;
        fname_new = real_fname_new;
    }
#endif

    if (rename(fname_old,fname_new) != 0) {
        LOGERR("while renaming \"%s\": %s", fname_old, strerror(errno));
        return Lb_FAIL;
    }
    return Lb_SUCCESS;
}

TbResult LbFileDelete(const char *fname)
{
#if LB_FILENAME_TRANSFORM
    char real_fname[FILENAME_MAX];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname, fname);
        fname = real_fname;
    }
#endif

    if (remove(fname) != 0) {
        LOGERR("while removing \"%s\": %s", fname, strerror(errno));
        return Lb_FAIL;
    }
    return Lb_SUCCESS;
}

/******************************************************************************/
