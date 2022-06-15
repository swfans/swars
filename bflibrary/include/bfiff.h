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
 * @param fname
 * @param inp_buffer
 * @param pal
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
