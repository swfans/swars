/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfiff.h
 *     Header file for giff.c.
 * @par Purpose:
 *     Routines for support of IFF image file format.
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
    u8 Palette[256]; // offset=4
};

typedef struct TbIffPalette TbIffPalette;

struct TbIff { // sizeof=18
    ushort Width; // offset=0
    ushort Height; // offset=2
    u32 FileSize; // offset=4
    TbIffPalette *Palette; // offset=8
    ushort XOffset; // offset=12
    ushort YOffset; // offset=14
    u8 XAspect; // offset=16
    u8 YAspect; // offset=17
};

typedef struct TbIff TbIff;

#pragma pack()

/** Load "ILBM" IFF Interleaved Bitmap.
 *
 * @param fname File name to read from.
 * @param buf Output image data buffer pointer.
 * @param iff Pointer image information structure.
 * @return Lb_* error code.
 */
TbResult LbIffLoad(const char *fname, u8 *buf, struct TbIff *iff);

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
  u32 width, u32 height, const u8 *pal, TbBool force_fname);

/** Save "ILBM" IFF Interleaved Bitmap from screen sized buffer.
 *
 * @param fname
 * @param inp_buffer
 * @param pal
 * @param force_fname
 * @return
 */
TbResult LbIffSaveScreen(const char *fname, const TbPixel *inp_buffer,
  const u8 *pal, TbBool force_fname);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFIFF_H_
/******************************************************************************/
