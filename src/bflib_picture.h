/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_picture.h
 *     Header file for bflib_picture.c.
 * @par Purpose:
 *     Pictures handling routines.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Oct 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_PICTURE_H
#define BFLIB_PICTURE_H

#include "bflib_basics.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
TbResult LbIffSave(const char *fname, unsigned char *inp_buffer,
    unsigned char *pal, TbBool auto_fname);

TbResult LbPngSave(const char *fname, unsigned char *inp_buffer,
    unsigned char *pal, TbBool force_fname);

unsigned int get_highest_file_no(const char *fnmask, int no_pos_in_fnmask, const char *fndir);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
