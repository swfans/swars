/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfiff.h
 *     Header file for giff.cpp.
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
#ifndef BFLIBRARY_GIFF_H_
#define BFLIBRARY_GIFF_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TbIffPalette { // sizeof=4
    SLONG Loaded; // offset=0
    UBYTE Palette[255]; // offset=4
};

typedef struct TbIffPalette TbIffPalette;

struct TbIff { // sizeof=18
    UWORD Width; // offset=0
    UWORD Height; // offset=2
    ULONG FileSize; // offset=4
    TbIffPalette *Palette; // offset=8
    UWORD XOffset; // offset=12
    UWORD YOffset; // offset=14
    UBYTE XAspect; // offset=16
    UBYTE YAspect; // offset=17
};

typedef struct TbIff TbIff;

struct iff_header { // sizeof=40
    UBYTE Id[3]; // offset=0
    ULONG FileLength; // offset=4
    UBYTE FileType[7]; // offset=8
    ULONG HeaderLength; // offset=16
    UWORD Width; // offset=20
    UWORD Height; // offset=22
    SWORD XOffset; // offset=24
    SWORD YOffset; // offset=26
    UBYTE NumberOfPlanes; // offset=28
    UBYTE Mask; // offset=29
    UBYTE Encoding; // offset=30
    UBYTE Padding; // offset=31
    UWORD Transparent; // offset=32
    UBYTE XAspectRatio; // offset=34
    UBYTE YAspectRatio; // offset=35
    UWORD PageWidth; // offset=36
    UWORD PageHeight; // offset=38
};

typedef struct iff_header iff_header;

struct ChunkHeader { // sizeof=8
    UBYTE Id[3]; // offset=0
    ULONG ChunkLength; // offset=4
};

typedef struct ChunkHeader ChunkHeader;

int LbIffLoad();
int LbIffSave();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GIFF_H_
/******************************************************************************/
