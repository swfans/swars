/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfdir.h
 *     Header file for sdir.cpp.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFDIR_H_
#define BFLIBRARY_BFDIR_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Changes the current directory on the specified drive to the specified path.
 *  If no drive is specified in path then the current drive is assumed.
 *  The path can be either relative to the current directory
 *  on the specified drive or it can be an absolute path name.
 *
 * @param path
 * @return
 */
TbResult LbDirectoryChange(const char *path);

/** Creates last directory in provided path.
 */
TbResult LbDirectoryCreate(const char *path);

/** Acquires current working directory.
 *
 * @return Returns the directory, without drive letter and without '/' at end.
 */
TbResult LbDirectoryCurrent(char *buf, unsigned long buflen);

/** Checks if a given file entry exist and is a directory.
 */
TbBool LbDirectoryExists(const char *dirname);

/** Removes given empty directory entry.
 */
TbResult LbDirectoryRemove(const char *path);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFDIR_H_
/******************************************************************************/
