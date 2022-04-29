/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bffnuniq.h
 *     Header file for gfnuniq.c.
 * @par Purpose:
 *     Unique file names generation.
 * @par Comment:
 *     Prepares unique file names, to be used for storing images and logs.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFFNUNIQ_H_
#define BFLIBRARY_BFFNUNIQ_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Gives highest file number in existing files matching given mask.
 * Questionable API, but left for compatibility with Bullfrog games.
 *
 * @param fnmask the mask to match
 * @param no_pos_in_fnmask position within the mask where the number is
 * @param fndir directory part of the mask (not really needed, we could use
 *          strrchr('/') instead; but this is how it was implemented by Bullfrog)
 * @return
 */
unsigned int get_highest_file_no(const char *fnmask, int no_pos_in_fnmask, const char *fndir);

/** Set the directory where images are stored.
 * Image saving functions can force use of a specified directory,
 * or use default one. The default directory is the one set here.
 */
void LbSetImagesDirectory(const char *dir_name);

/** Prepare unused and unique name for an image.
 * Image saving functions call this to get default destination directory.
 */
TbResult LbPrepareImageFilename(char *out_fname, const char *fn_base,
    const char *ext);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFFNUNIQ_H_
/******************************************************************************/
