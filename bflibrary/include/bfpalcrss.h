/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfpalcrss.h
 *     Header file for bfpalcrss.c.
 * @par Purpose:
 *     Palette changing functions.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2021 - 05 Jun 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFPALCRSS_H_
#define BFLIBRARY_BFPALCRSS_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Generate a table for converting an image between two palettes.
 *
 * @param cross_lut Output lookup table for converting each pixel.
 * @param src_palette Source palette.
 * @param dst_palette Destination palette.
 */
TbResult LbCrossPaletteLutGen(ubyte *cross_lut, const ubyte *src_palette,
  const ubyte *dst_palette);

/** Computes histogram for given memory buffer.
 *
 * @param hist Output histogram array.
 * @param buf Input data.
 * @param buf_len Length of the input data.
 */
TbResult LbBufferHistogram(ulong *hist, const TbPixel *buf, ulong buf_len);

short LbImageBuffersMaxDifference(const TbPixel *buf1, const ubyte *pal1,
  const TbPixel *buf2, const ubyte *pal2, ulong buf_len, ulong *pos);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFPALCRSS_H_
/******************************************************************************/
