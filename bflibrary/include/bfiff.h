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

#pragma pack(1)

struct TbIffPalette { // sizeof=4
    long Loaded; // offset=0
    ubyte Palette[255]; // offset=4
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

struct iff_header { // sizeof=40
    ubyte Id[3]; // offset=0
    ulong FileLength; // offset=4
    ubyte FileType[7]; // offset=8
    ulong HeaderLength; // offset=16
    ushort Width; // offset=20
    ushort Height; // offset=22
    short XOffset; // offset=24
    short YOffset; // offset=26
    ubyte NumberOfPlanes; // offset=28
    ubyte Mask; // offset=29
    ubyte Encoding; // offset=30
    ubyte Padding; // offset=31
    ushort Transparent; // offset=32
    ubyte XAspectRatio; // offset=34
    ubyte YAspectRatio; // offset=35
    ushort PageWidth; // offset=36
    ushort PageHeight; // offset=38
};

typedef struct iff_header iff_header;

struct ChunkHeader { // sizeof=8
    ubyte Id[3]; // offset=0
    ulong ChunkLength; // offset=4
};

typedef struct ChunkHeader ChunkHeader;

#pragma pack()

int LbIffLoad();
int LbIffSave();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GIFF_H_
/******************************************************************************/
