/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bffile.h
 *     Header file for gfile.c, sfile.c.
 * @par Purpose:
 *     File handling routines wrapper.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFFILE_H_
#define BFLIBRARY_BFFILE_H_

#include "bftypes.h"
#include "bftime.h"

#if defined(WIN32)||defined(DOS)||defined(GO32)
#include <dir.h>
#else
#include <stdio.h>
#include <sys/stat.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//TODO change to build system param
#define LB_FILENAME_TRANSFORM 1


/* File system separator. Use when concatenating path; when checking existing
 * path, it is better to try both separators rather than use these defines.
 */
#if defined WIN32
# define FS_SEP '\\'
# define FS_SEP_STR "\\"
#else
# define FS_SEP '/'
# define FS_SEP_STR "/"
#endif

enum TbFileOpenMode { // type=int8_t
    Lb_FILE_MODE_NEW = 0,
    Lb_FILE_MODE_OLD,
    Lb_FILE_MODE_READ_ONLY,
};

#define LBO_CREAT	Lb_FILE_MODE_NEW
#define LBO_RDWR	Lb_FILE_MODE_OLD
#define LBO_RDONLY	Lb_FILE_MODE_READ_ONLY

typedef enum TbFileOpenMode TbFileOpenMode;

enum TbFileSeekMode { // type=int8_t
    Lb_FILE_SEEK_BEGINNING = 0,
    Lb_FILE_SEEK_CURRENT,
    Lb_FILE_SEEK_END,
};

enum TbFileAttribFlags { // type=int8_t
    /* Normal file. */
    Lb_FILE_ATTR_NORMAL = 0x00,
    /* Read only file. */
    Lb_FILE_ATTR_RDONLY = 0x01,
    /* Hidden file. */
    Lb_FILE_ATTR_HIDDEN = 0x02,
    /* System file. */
    Lb_FILE_ATTR_SYSTEM = 0x03,
    /* Subdirectory. */
    Lb_FILE_ATTR_SUBDIR = 0x10,
    /* Archived file. */
    Lb_FILE_ATTR_ARCH   = 0x20,
};

typedef enum TbFileSeekMode TbFileSeekMode;

#define INVALID_FILE (TbFileHandle)-1

typedef ulong TbFileHandle;

#if defined(_finddata_t) // GCC Windows API
typedef struct _finddata_t TbFILE_FIND;
#elif defined(DOSFINDTYPE) // Watcom C API
typedef DOSFINDTYPE TbFILE_FIND;
#else // POSIX emulation of the findfirst/findnext API
#define LB_FILE_FIND_SIMULATED
struct _finddata_t {
    struct stat st;
    char name[FILENAME_MAX];
};
typedef struct _finddata_t TbFILE_FIND;
#endif

#pragma pack(1)

struct TbFileFind {
    char Filename[144];
    char AlternateFilename[14];
    unsigned long Attributes;
    unsigned long Length;
    TbDate CreationDate;
    TbTime CreationTime;
    TbDate LastWriteDate;
    TbTime LastWriteTime;
    unsigned long ReservedHandle;
    TbFILE_FIND Reserved;
};

typedef struct TbFileFind TbFileFind;

#pragma pack()

#if LB_FILENAME_TRANSFORM

typedef void (*FileNameTransform)(char *out_fname, const char *inp_fname);

/** Callback to be used for transforming all file names before opening.
 */
extern FileNameTransform lbFileNameTransform;

#endif


/** Returns if given file exists.
 */
TbBool LbFileExists(const char *fname);

/** Returns current value of the file position indicator.
 */
long LbFilePosition(TbFileHandle fhandle);

/** Opens a file in given mode.
 */
TbFileHandle LbFileOpen(const char *fname, const TbFileOpenMode accmode);

/** Closes a file.
 */
TbResult LbFileClose(TbFileHandle fhandle);

/** Checks if the file position indicator is placed at end of the file.
 */
TbBool LbFileEof(TbFileHandle fhandle);

/** Changes position in opened file.
 *
 * @param fhandle
 * @param offset
 * @param origin
 * @return Returns new file position, or -1 on error.
 */
TbResult LbFileSeek(TbFileHandle fhandle, long offset, TbFileSeekMode origin);

/**
 * Reads from previously opened disk file.
 *
 * @param fhandle
 * @param buffer
 * @param len
 * @return Gives amount of bytes read, or -1 on error.
 */
long LbFileRead(TbFileHandle fhandle, void *buffer, unsigned long len);

/**
 * Writes data at the operating system level.
 * The number of bytes transmitted is given by len and the data
 * to be transmitted is located at the address specified by buffer.
 *
 * @return Returns the number of bytes (does not include any extra carriage-return
 * characters transmitted) of data transmitted to the file.
 */
long LbFileWrite(TbFileHandle fhandle, const void *buffer, const unsigned long len);

/**
 * Flushes the file buffers, writing all data immediately.
 *
 * @param fhandle Opened file handle
 * @return Returns 1 on success, 0 on error.
 */
TbBool LbFileFlush(TbFileHandle fhandle);

/** Returns size of an already opened file.
 */
long LbFileLengthHandle(TbFileHandle fhandle);

/** Returns disk size of file.
 *
 * @param fname File name string
 * @return
 */
long LbFileLength(const char *fname);

/** Starts listing of directory entries.
 *
 * @param filespec
 * @param ffind
 * @param attributes
 * @return Gives -1 if no match is found, otherwise returns 1 and stores a handle inside
 *  TbFileFind struct which is then used for LbFileFindNext() and LbFileFindEnd() calls.
 */
TbResult LbFileFindFirst(const char *filespec, TbFileFind *ffind,
  unsigned int attributes);

/** Continues listing directory entries.
 *
 * @param ffind
 * @return Gives -1 if no match is found, otherwise returns 1
 */
TbResult LbFileFindNext(TbFileFind *ffind);

/** Ends sequence of listing directory entries.
 *
 * @param ffind
 */
TbResult LbFileFindEnd(TbFileFind *ffind);

/** Renames a disk file.
 */
TbResult LbFileRename(const char *fname_old, const char *fname_new);

/** Removes a disk file.
 *
 * @param fname File name string
 * @return
 */
TbResult LbFileDelete(const char *fname);

long LbFileLengthRnc(const char *fname);
long LbFileLoadAt(const char *fname, void *buffer);
long LbFileSaveAt(const char *fname, const void *buffer, ulong len);
int LbFileStringSearch();

/** Creates a proper disk path from file name, directory, and current directory
 *
 * Merge given directory and filename, and optionally precedes that with CWD.
 */
TbResult LbFileMakeFullPath(const TbBool append_cur_dir,
  const char *directory, const char *filename, char *buf, const ulong len);

int LbFileCopy();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFFILE_H_
/******************************************************************************/
