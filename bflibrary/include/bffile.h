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
#include "dir.h"

#ifdef __cplusplus
extern "C" {
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

typedef enum TbFileSeekMode TbFileSeekMode;

typedef int32_t TbFileHandle;

#if defined(_finddata_t) // GCC Windows API
typedef struct _finddata_t TbFILE_FIND;
#elif defined(DOSFINDTYPE) // Watcom C API
typedef DOSFINDTYPE TbFILE_FIND;
#else
#error Data type for findfirst()/findnext() not recognized
#endif

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

/** Returns if given file exists.
 */
TbBool LbFileExists(const char *fname);

/** Returns current value of the file position indicator.
 */
long LbFilePosition(TbFileHandle handle);

/** Opens a file in given mode.
 */
TbFileHandle LbFileOpen(const char *fname, const TbFileOpenMode accmode);

/** Closes a file.
 */
TbResult LbFileClose(TbFileHandle handle);

/** Checks if the file position indicator is placed at end of the file.
 */
TbBool LbFileEof(TbFileHandle handle);

/** Changes position in opened file.
 *
 * @param handle
 * @param offset
 * @param origin
 * @return Returns new file position, or -1 on error.
 */
TbResult LbFileSeek(TbFileHandle handle, long offset, TbFileSeekMode origin);

/**
 * Reads from previously opened disk file.
 *
 * @param handle
 * @param buffer
 * @param len
 * @return Gives amount of bytes read, or -1 on error.
 */
long LbFileRead(TbFileHandle handle, void *buffer, unsigned long len);

/**
 * Writes data at the operating system level.
 * The number of bytes transmitted is given by len and the data
 * to be transmitted is located at the address specified by buffer.
 * @return Returns the number of bytes (does not include any extra carriage-return
 * characters transmitted) of data transmitted to the file.
 */
long LbFileWrite(TbFileHandle handle, const void *buffer, const unsigned long len);

/**
 * Flushes the file buffers, writing all data immediately.
 * @return Returns 1 on success, 0 on error.
 */
TbBool LbFileFlush(TbFileHandle handle);

/** Returns size of an already opened file.
 */
long LbFileLengthHandle(TbFileHandle handle);

/** Returns disk size of file.
 */
long LbFileLength(const char *fname);

/** Starts listing of directory entries.
 *
 * @return Gives -1 if no match is found, otherwise returns 1 and stores a handle inside
 *  TbFileFind struct which is then used for LbFileFindNext() and LbFileFindEnd() calls.
 */
TbResult LbFileFindFirst(const char *filespec, struct TbFileFind *ffind,unsigned int attributes);

/** Continues listing directory entries.
 *
 * @return Gives -1 if no match is found, otherwise returns 1
 */
TbResult LbFileFindNext(struct TbFileFind *ffind);

/** Ends sequence of listing directory entries.
 */
TbResult LbFileFindEnd(struct TbFileFind *ffind);

/** Renames a disk file.
 */
TbResult LbFileRename(const char *fname_old, const char *fname_new);

/** Removes a disk file.
 */
TbResult LbFileDelete(const char *filename);


int LbFileLengthRnc();
int LbFileLoadAt();
int LbFileSaveAt();
int LbFileStringSearch();

TbResult LbFileMakeFullPath(const short append_cur_dir,
  const char *directory, const char *filename, char *buf, const unsigned long len);

int LbFileCopy();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFFILE_H_
/******************************************************************************/
