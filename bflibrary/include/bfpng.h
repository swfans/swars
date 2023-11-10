/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfpng.h
 *     Header file for gpng.c.
 * @par Purpose:
 *     Routines for support of PNG image file format.
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

/** Load PNG Portable Network Graphics Bitmap.
 *
 * @param fname File name to read from.
 * @param out_buffer Output image data buffer pointer.
 * @param width Output image width and interline.
 * @param height Output image height.
 * @param pal Pointer to image colour palette output buffer.
 * @return Lb_* error code.
 */
TbResult LbPngLoad(const char *fname, TbPixel *out_buffer,
  u32 *width, u32 *height, u8 *pal);

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
  u32 width, u32 height, const u8 *pal, TbBool force_fname);

/** Save PNG Portable Network Graphics Bitmap from screen sized buffer.
 *
 * @param fname File name or main component of file name.
 * @param inp_buffer Input buffer pointer.
 * @param pal Image colour palette data pointer.
 * @param force_fname
 * @return
 */
TbResult LbPngSaveScreen(const char *fname, const TbPixel *inp_buffer,
  const u8 *pal, TbBool force_fname);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFPNG_H_
/******************************************************************************/
