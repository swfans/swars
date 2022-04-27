/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_fileio.h
 *     Header file for bflib_fileio.c.
 * @par Purpose:
 *     File handling routines wrapper.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_FILEIO_H
#define BFLIB_FILEIO_H

#include "bflib_basics.h"
#include "bffile.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#ifdef __unix__
# define FS_SEP '/'
# define FS_SEP_STR "/"
#elif defined WIN32
# define FS_SEP '\\'
# define FS_SEP_STR "\\"
#endif

/******************************************************************************/
#pragma pack(1)

struct TbDriveInfo {
        unsigned long TotalClusters;
        unsigned long FreeClusters;
        unsigned long SectorsPerCluster;
        unsigned long BytesPerSector;
};

#pragma pack()

/******************************************************************************/

int LbDriveCurrent(unsigned int *drive);
int LbDriveChange(const unsigned int drive);
int LbDriveExists(const unsigned int drive);
int LbDirectoryChange(const char *path);
TbResult LbDirectoryMake(const char *path, TbBool recursive);
int LbDriveFreeSpace(const unsigned int drive, struct TbDriveInfo *drvinfo);
char *LbGetCurrWorkDir(char *dest, const unsigned long maxlen);
int LbDirectoryCurrent(char *buf, unsigned long buflen);
TbResult LbFileMakeFullPath(const TbBool append_cur_dir, const char *directory,
    const char *filename, char *buf, const unsigned long len);


/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
