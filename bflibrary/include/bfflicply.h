/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfflicply.h
 *     Header file for gflicply.c.
 * @par Purpose:
 *     Animation playback support in Autodesk FLIC format.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 28 Jan 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFFLICPLY_H
#define BFFLICPLY_H

#include "bftypes.h"
#include "bffile.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)

enum FLI_Ani_Consts {
    FLI_COLOUR256    = 0x0004,
    FLI_SS2          = 0x0007,
    FLI_COLOUR       = 0x000B,
    FLI_LC           = 0x000C,
    FLI_BLACK        = 0x000D,
    FLI_BRUN         = 0x000F,
    FLI_COPY         = 0x0010,
    FLI_PSTAMP       = 0x0012,
    FLI_FILE_HEADER  = 0x0AF12,
    FLI_PREFIX_CHUNK = 0x0F100,
    FLI_FRAME_CHUNK  = 0x0F1FA,
};

struct FLCFileHeader {
    u32 Size;
    ushort Magic;
    ushort NumberOfFrames;
    ushort Width;
    ushort Height;
};

struct FLCPrefixChunk {
    u32 Size;
    ushort Type;
};

struct FLCFrameChunk {
    u32 Size;
    ushort Type;
    ushort Chunks;
    ubyte Reserved_0[8];
};

struct FLCFrameDataChunk {
    u32 Size;
    ushort Type;
};

struct Animation {
    ubyte *OutBuf;
    long anfield_4;
    short Xpos;
    short Ypos;
    short FrameNumber;
    ushort Flags;
    ubyte *UnkBuf;
    struct FLCFileHeader FLCFileHeader;
    struct FLCFrameChunk FLCFrameChunk;
    long anfield_30;
    long anfield_34;
    TbFileHandle FileHandle;
    char Filename[48];
    short anfield_6C;
    short Type;
};

#pragma pack()
/******************************************************************************/
extern ubyte anim_palette[0x300];
extern void *anim_scratch;

void anim_show_prep_next_frame(struct Animation *p_anim);
ubyte anim_show_frame(struct Animation *p_anim);

TbResult anim_flic_open(struct Animation *p_anim);
TbBool anim_is_opened(struct Animation *p_anim);
void anim_flic_close(struct Animation *p_anim);

// Low level interface
void anim_show_FLI_SS2(struct Animation *p_anim);
void anim_show_FLI_BRUN(struct Animation *p_anim);
void anim_show_FLI_LC(struct Animation *p_anim);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
