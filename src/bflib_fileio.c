/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_fileio.c
 *     File handling routines wrapper.
 * @par Purpose:
 *     Buffer library for file i/o and directory manage routines.
 *     These should be used for all file access in the game.
 * @par Comment:
 *     Wraps standard c file handling routines. You could say this has no purpose,
 *     but here it is anyway.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_fileio.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include "bflib_basics.h"
#include "dos.h"
/* #include "bflib_datetm.h" */

#if defined(WIN32)||defined(DOS)||defined(GO32)
#include <io.h>
#include <share.h>
#include <dos.h>
#include <direct.h>
#endif

#if defined(WIN32)
#ifdef __cplusplus
extern "C" {
#endif
//Selected declarations frow Win32 API - I don't want to use whole API
// since it influences everything
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
#endif
/******************************************************************************/
// Internal declarations
void convert_find_info(struct TbFileFind *ffind);
// Globals
FileNameTransform lbFileNameTransform = NULL;
/******************************************************************************/

int LbDriveCurrent(unsigned int *drive)
{
#if defined(WIN32)||defined(DOS)||defined(GO32)
    *drive=_getdrive();
#else
    // Let's assume we're on 'C' drive on Unix ;)
    *drive = 3;
#endif
    return 1;
}

/** Changes the current disk drive into given one
 *
 * @param drive
 * @return
 */
int LbDriveChange(const unsigned int drive)
{
    int result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    int reterror = _chdrive(drive);
    if (reterror) {
        result = -1;
    } else {
        result = 1;
    }
#else
    // Let's assume we can only be on 'C' drive on Unix
    if (drive != 3) {
        result = -1;
    } else {
        result = 1;
    }
#endif
    return result;
}

/** Returns if a given drive exists.
 *
 * @param drive
 * @return
 */
int LbDriveExists(const unsigned int drive)
{
    int result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    unsigned int lastdrive=_getdrive();
    if (_chdrive(drive)) {
        result = -1;
    } else {
        result = 1;
        _chdrive(lastdrive);
    }
#else
    // Let's assume we have only 'C' drive on Unix
    if (drive != 3) {
        result = -1;
    } else {
        result = 1;
    }
#endif
    return result;
}

/** Changes the current directory on the specified drive to the specified path.
 *  If no drive is specified in path then the current drive is assumed.
 *  The path can be either relative to the current directory
 *  on the specified drive or it can be an absolute path name.
 *
 * @param path
 * @return
 */
int LbDirectoryChange(const char *path)
{
    int result;
    if (chdir(path))
        result = -1;
    else
        result = 1;
    return result;
}

int LbDriveFreeSpace(const unsigned int drive, struct TbDriveInfo *drvinfo)
{
  int result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    struct _diskfree_t diskspace;
    int reterror = _getdiskfree(drive, &diskspace);
    if ( reterror )
    {
        result = -1;
    } else
    {
        drvinfo->TotalClusters = diskspace.total_clusters;
        drvinfo->FreeClusters = diskspace.avail_clusters;
        drvinfo->SectorsPerCluster = diskspace.sectors_per_cluster;
        drvinfo->BytesPerSector = diskspace.bytes_per_sector;
        result = 1;
    }
#else
    //On non-win32 systems - return anything big enough
    drvinfo->TotalClusters = 65535;
    drvinfo->FreeClusters = 65535;
    drvinfo->SectorsPerCluster = 512;
    drvinfo->BytesPerSector = 512;
    result = 1;
#endif
    return result;
}

TbBool LbFileExists(const char *fname)
{
    char real_fname[DISKPATH_SIZE];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname, fname);
        fname = real_fname;
    }
    return access(fname,F_OK) == 0;
}

int LbFilePosition(TbFileHandle handle)
{
    int result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    result = tell(handle);
#else
    result = lseek(handle, 0, SEEK_CUR);
#endif
    return result;
}

TbFileHandle LbFileOpen(const char *fname, const enum TbFileOpenMode accmode)
{
  unsigned char mode = accmode;
  char real_fname[DISKPATH_SIZE];
  TbFileHandle rc;

  if ( !LbFileExists(fname) )
  {
      BFLIB_DEBUGLOG(0, "%s: file doesn't exist", fname);
      if ( mode == Lb_FILE_MODE_READ_ONLY )
          return -1;
      /* DISABLED - this is only used in games newer than SW.
       * In older games, this causes successful creation of files
       * which should fail, resulting in changed behavior of the game.
      if ( mode == Lb_FILE_MODE_OLD )
          mode = Lb_FILE_MODE_NEW;
      */
  }

  if (lbFileNameTransform != NULL) {
      lbFileNameTransform(real_fname, fname);
      fname = real_fname;
  }

  /* DISABLED - NOT NEEDED
  if ( mode == Lb_FILE_MODE_NEW )
  {
    BFLIB_DEBUGLOG(0,"%s: truncating file", fname);
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
        BFLIB_DEBUGLOG(1, "%s: LBO_CREAT mode", fname);
#if defined(WIN32)||defined(DOS)||defined(GO32)
        rc = _sopen(fname, O_RDWR|O_CREAT|O_BINARY, SH_DENYNO, S_IREAD|S_IWRITE);
#else
        rc = open(fname, O_RDWR|O_CREAT, S_IRUSR|S_IRGRP|S_IWUSR|S_IWGRP);
#endif
    };break;
  case Lb_FILE_MODE_OLD:
    {
        BFLIB_DEBUGLOG(1,"%s: LBO_RDWR mode", fname);
#if defined(WIN32)||defined(DOS)||defined(GO32)
        rc = _sopen(fname, O_RDWR|O_BINARY, SH_DENYNO);
#else
        rc = open(fname, O_RDWR);
#endif
    };break;
  case Lb_FILE_MODE_READ_ONLY:
    {
        BFLIB_DEBUGLOG(1,"%s: LBO_RDONLY mode", fname);
#if defined(WIN32)||defined(DOS)||defined(GO32)
        rc = _sopen(fname, O_RDONLY|O_BINARY, SH_DENYNO);
#else
        rc = open(fname, O_RDONLY);
#endif
    };break;
  }
  BFLIB_DEBUGLOG(0,"%s: out handle = %ld, errno = %d", fname, (long)rc, errno);
  return rc;
}

/** Closes a file
 *
 * @param handle
 * @return
 */
int LbFileClose(TbFileHandle handle)
{
  if ( close(handle) )
    return -1;
  else
    return 1;
}

/** Checks if the file position indicator is placed at end of the file.
 */
TbBool LbFileEof(TbFileHandle handle)
{
  if (LbFilePosition(handle) >= LbFileLengthHandle(handle))
    return 1;
  return 0;
}

/** Changes position in opened file.
 *
 * @param handle
 * @param offset
 * @param origin
 * @return Returns new file position, or -1 on error.
 */
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

/** Reads from previously opened disk file.
 *
 * @param handle
 * @param buffer
 * @param len
 * @return Gives amount of bytes read, or -1 on error.
 */
long LbFileRead(TbFileHandle handle, void *buffer, unsigned long len)
{
  int result;
  //'read' returns (-1) on error
  result = read(handle,buffer,len);
  return result;
}

/**
 * Writes data at the operating system level.
 * The number of bytes transmitted is given by len and the data
 * to be transmitted is located at the address specified by buffer.
 * @return Returns the number of bytes (does not include any extra carriage-return
 * characters transmitted) of data transmitted to the file.
*/
long LbFileWrite(TbFileHandle handle, const void *buffer, const unsigned long len)
{
    long result;
    result = write(handle, buffer, len);
    return result;
}

/**
 * Flushes the file buffers, writing all data immediately.
 * @return Returns 1 on success, 0 on error.
*/
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
#elif defined(DOS)||defined(GO32)
    // No idea how to do this on old systems
    return 1;
#else
    // For normal POSIX systems
    // (should also work on Win, as its IEEE standard... but it currently isn't)
    return (fsync(handle) != -1);
#endif

}

long LbFileLengthHandle(TbFileHandle handle)
{
    long result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    result = filelength(handle);
#else
    struct stat buf;
    fstat(handle, &buf);
    result = buf.st_size;
#endif
    return result;
}

/** Returns disk size of file
 *
 * @param fname File name string
 */
long LbFileLength(const char *fname)
{
    /* We will switch to original version later, when OS-dependent paths
     * are handled within the game. */
    /*TbFileHandle handle;
    handle = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    long result = handle;
    if ( handle != -1 )
    {
      result = filelength(handle);
      LbFileClose(handle);
    }
    return result;*/
    struct stat st;
    char real_fname[DISKPATH_SIZE];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname, fname);
        fname = real_fname;
    }

    if (stat (fname, &st) != 0) {
        BFLIB_ERRORLOG("%s: Cannot get file stats: %s", fname, strerror(errno));
        return -1;
    }

    return st.st_size;
}

//Converts file search information from platform-specific into independent form
//Yeah, right...
void convert_find_info(struct TbFileFind *ffind)
{
#if defined(WIN32)||defined(DOS)||defined(GO32)
  struct _finddata_t *fdata=&(ffind->Reserved);
  strncpy(ffind->Filename, fdata->name, sizeof(ffind->Filename));
  ffind->Filename[sizeof(ffind->Filename)-1] = '\0';
#if defined(WIN32)
  GetShortPathName(fdata->name, ffind->AlternateFilename, sizeof(ffind->AlternateFilename));
#else
  strncpy(ffind->AlternateFilename, fdata->name, sizeof(ffind->AlternateFilename));
#endif
  ffind->AlternateFilename[sizeof(ffind->AlternateFilename)-1] = '\0';
  if (fdata->size > ULONG_MAX)
    ffind->Length = ULONG_MAX;
  else
    ffind->Length = fdata->size;
  ffind->Attributes = fdata->attrib;
/* FIXME re-enable - this was temporarely disabled to remove datetime module dependence
  LbDateTimeDecode(&fdata->time_create,&ffind->CreationDate,&ffind->CreationTime);
  LbDateTimeDecode(&fdata->time_write,&ffind->LastWriteDate,&ffind->LastWriteTime);
*/
#else
  assert(!"not implemented");
#endif
}

// returns -1 if no match is found. Otherwise returns 1 and stores a handle
// to be used in _findnext and _findclose calls inside TbFileFind struct.
int LbFileFindFirst(const char *filespec, struct TbFileFind *ffind, unsigned int attributes)
{
#if defined(WIN32)||defined(DOS)||defined(GO32)
    // original Watcom code was
    //dos_findfirst_(path, attributes,&(ffind->Reserved))
    //The new code skips 'attributes' as Win32 prototypes seem not to use them
    char real_filespec[DISKPATH_SIZE];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_filespec, filespec);
        filespec = real_filespec;
    }

    ffind->ReservedHandle = _findfirst(filespec, &(ffind->Reserved));
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
#else
    assert(!"not implemented");
#endif
}

// returns -1 if no match is found, otherwise returns 1
int LbFileFindNext(struct TbFileFind *ffind)
{
#if defined(WIN32)||defined(DOS)||defined(GO32)
    int result;
    if ( _findnext(ffind->ReservedHandle, &(ffind->Reserved)) < 0 )
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
#else
    assert(!"not implemented");
#endif
}

//Ends file searching sequence
int LbFileFindEnd(struct TbFileFind *ffind)
{
#if defined(WIN32)||defined(DOS)||defined(GO32)
    if (ffind->ReservedHandle != -1)
    {
        _findclose(ffind->ReservedHandle);
    }
    return 1;
#else
    assert(!"not implemented");
#endif
}

//Renames a disk file
int LbFileRename(const char *fname_old, const char *fname_new)
{
    int result;
    char real_fname_old[DISKPATH_SIZE];
    char real_fname_new[DISKPATH_SIZE];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname_old, fname_old);
        lbFileNameTransform(real_fname_new, fname_new);
        fname_old = real_fname_old;
        fname_new = real_fname_new;
    }
    if (rename(fname_old,fname_new))
        result = -1;
    else
        result = 1;
    return result;
}

/** Removes a disk file
 *
 * @param fname
 * @return
 */
int LbFileDelete(const char *fname)
{
    char real_fname[DISKPATH_SIZE];
    int result;

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_fname, fname);
        fname = real_fname;
    }
    if (remove(fname))
        result = -1;
    else
        result = 1;
    return result;
}

char *LbGetCurrWorkDir(char *dest, const unsigned long maxlen)
{
  return getcwd(dest,maxlen);
}

int LbDirectoryCurrent(char *buf, unsigned long buflen)
{
    if ( getcwd(buf,buflen) != NULL )
    {
        if (buf[1] == ':')
            strcpy(buf, buf+2);
        int len = strlen(buf);
        if ( len>1 )
        {
            if (buf[len-2] == '\\')
                buf[len-2] = '\0';
        }
        return 1;
    }
    return -1;
}

TbResult LbFileMakeFullPath(const TbBool append_cur_dir,
  const char *directory, const char *filename, char *buf, const unsigned long len)
{
  if (filename == NULL) {
      buf[0] = '\0';
      return -1;
  }
  unsigned long namestart;
  if ( append_cur_dir )
  {
      if (LbDirectoryCurrent(buf, len-2) == -1) {
          buf[0] = '\0';
          return -1;
      }
      namestart = strlen(buf);
      if ( (namestart>0) && (buf[namestart-1]!='\\') && (buf[namestart-1]!='/')) {
          buf[namestart] = '/';
          namestart++;
      }
  } else
  {
    namestart = 0;
  }
  buf[namestart] = '\0';

  if (directory != NULL)
  {
      int copy_len;
      copy_len = strlen(directory);
      if ( len-2 <= namestart+copy_len-1 )
          return -1;
      memcpy(buf+namestart, directory, copy_len);
      namestart += copy_len-1;
      if ((namestart > 0) && (buf[namestart-1] != '\\') && (buf[namestart-1] != '/')) {
          buf[namestart] = '/';
          namestart++;
      }
      buf[namestart] = '\0';
  }
  if ( strlen(filename)+namestart-1 < len )
  {
      const char *ptr = filename;
      int invlen;
      for (invlen = -1; invlen != 0; invlen--)
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
      for (invlen = -1; invlen != 0; invlen--)
      {
          if (*copy_dst++ == 0) {
              invlen--;
              break;
          }
      }
      memcpy(copy_dst-1, copy_src, copy_len);
      return 1;
  }
  return -1;
}

TbResult
LbDirectoryMake(const char *path, TbBool recursive)
{
    char buffer[FILENAME_MAX];
    char fname[DISKPATH_SIZE];
    char *p;
    size_t len;
    struct stat st;
    int err;
    mode_t __attribute__((unused)) mode = 0755;
    int num_levels = 0;

    // We need to transform the path here - if we did it later,
    // then we would skip base directories in recursion.
    // Also, the function expects file name, not path - make one
    if (lbFileNameTransform != NULL) {
        strncpy(fname, path, DISKPATH_SIZE-2);
        strcat(fname, "/a");
        lbFileNameTransform(buffer, fname);
        len = strlen(buffer) - 2;
        buffer[len] = '\0';
    } else {
        len = snprintf(buffer, sizeof(buffer), "%s", path);
    }

    /* First, find the longest existing path */
    do
    {
        err = stat(buffer, &st);
        if (err == 0)
        {
            if (!S_ISDIR(st.st_mode))
            {
                BFLIB_ERRORLOG("%s: Not a directory", buffer);
                return Lb_FAIL;
            }
        }
        else
        {
            if (errno != ENOENT)
            {
                BFLIB_ERRORLOG("%s: Cannot stat dir: %s", buffer, strerror(errno));
                return Lb_FAIL;
            }

            p = strrchr(buffer, FS_SEP);
            if (p == NULL)
                break;
            num_levels++;

            *p = 0;
        }
    }
    while (err != 0);

    if ((num_levels > 1) && (!recursive))
    {
        BFLIB_ERRORLOG("%s: Cannot create %d dirs - recursion disabled", buffer, num_levels);
        return Lb_FAIL;
    }
    /*
     * At this point, buffer contains the longest existing path.  Go forward
     * through the rest of the path and create the missing directories.
     */
    p = buffer;

    for (;;)
    {
        while (*p != '\0')
            p++;

        if (p >= buffer + len)
            break;

        *p = FS_SEP;

        BFLIB_DEBUGLOG(1,"%s: Creating directory", buffer);
#if defined(WIN32)
        err = mkdir(buffer);
#else
        err = mkdir(buffer, mode);
#endif
        if (err != 0)
        {
            BFLIB_ERRORLOG("%s: Cannot create dir: %s", buffer, strerror(errno));
            return Lb_FAIL;
        }
    }

    return Lb_SUCCESS;
}
/******************************************************************************/
