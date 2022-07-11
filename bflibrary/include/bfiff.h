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

#pragma pack(1)

struct TbIffPalette { // sizeof=4
    long Loaded; // offset=0
    ubyte Palette[256]; // offset=4
};

typedef struct TbIffPalette TbIffPalette;

struct TbIff { // sizeof=18
    ushort Width; // offset=0
    ushort Height; // offset=2
    ulong FileSize; // offset=4
    TbIffPalette *Palette; // offset=8
    ushort XOffset; // offset=12
    ushort YOffset; // offset=14
    ubyte XAspect; // offset=16
    ubyte YAspect; // offset=17
};

typedef struct TbIff TbIff;

#pragma pack()

/** Load "ILBM" IFF Interleaved Bitmap.
 */
TbResult LbIffLoad(const char *fname, ubyte *buf, struct TbIff *iff);

/** Save "ILBM" IFF Interleaved Bitmap.
 *
 * Note that unlike the old API, this function requires width and height to
 * be supplied. To assume screen sized buffer like the old API did, call
 * LbIffSaveScreen() instead.
 *
 * @param fname File name or main component of file name.
 * @param inp_buffer Input buffer pointer.
 * @param width Input buffer width and interline.
 * @param height Input buffer height.
 * @param pal Image colour palette data pointer.
 * @param force_fname
 * @return
 */
TbResult LbIffSave(const char *fname, const TbPixel *inp_buffer,
  ulong width, ulong height, const ubyte *pal, TbBool force_fname);

/** Save "ILBM" IFF Interleaved Bitmap from screen sized buffer.
 *
 * @param fname
 * @param inp_buffer
 * @param pal
 * @param force_fname
 * @return
 */
TbResult LbIffSaveScreen(const char *fname, const TbPixel *inp_buffer,
  const ubyte *pal, TbBool force_fname);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFIFF_H_
/******************************************************************************/
