/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfiff.h
 *     Header file for giff.c.
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
#ifndef BFLIBRARY_BFIFF_H_
#define BFLIBRARY_BFIFF_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

int LbIffLoad();

/** Save "ILBM" IFF Interleaved Bitmap
 *
 * @param fname
 * @param a2
 * @param a3
 * @param force_fname
 * @return
 */
TbResult LbIffSave(const char *fname, unsigned char *inp_buffer,
    unsigned char *pal, TbBool force_fname);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFIFF_H_
/******************************************************************************/
