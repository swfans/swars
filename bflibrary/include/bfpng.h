/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfpng.h
 *     Header file for gpng.c.
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
#ifndef BFLIBRARY_BFPNG_H_
#define BFLIBRARY_BFPNG_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Save PNG Portable Network Graphics Bitmap.
 *
 * @param fname File name or main component of file name.
 * @param inp_buffer Input buffer pointer.
 * @param width Input buffer width and interline.
 * @param height Input buffer height.
 * @param pal Image colour palette data pointer.
 * @param force_fname
 * @return
 */
TbResult LbPngSave(const char *fname, const TbPixel *inp_buffer,
  ulong width, ulong height, const ubyte *pal, TbBool force_fname);

/** Save PNG Portable Network Graphics Bitmap from screen sized buffer.
 *
 * @param fname File name or main component of file name.
 * @param inp_buffer Input buffer pointer.
 * @param pal Image colour palette data pointer.
 * @param force_fname
 * @return
 */
TbResult LbPngSaveScreen(const char *fname, const TbPixel *inp_buffer,
  const ubyte *pal, TbBool force_fname);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFPNG_H_
/******************************************************************************/
