/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfflic.h
 *     Header file for gflicply.c and gflicrec.c.
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
#ifndef BFFLIC_H
#define BFFLIC_H

#include "bftypes.h"
#include "bffile.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)

enum FLI_Ani_Consts {
    FLI_COLOUR256    = 0x0004, /**< 256-level color palette */
    FLI_SS2          = 0x0007, /**< word oriented RLE frame data, delta image */
    FLI_COLOUR       = 0x000B, /**< 64-level color palette */
    FLI_LC           = 0x000C, /**< byte oriented RLE frame data, delta image */
    FLI_BLACK        = 0x000D, /**< Black frame */
    FLI_BRUN         = 0x000F, /**< byte oriented RLE-compressed full image */
    FLI_COPY         = 0x0010, /**< Uncompressed frame, full image */
    FLI_PSTAMP       = 0x0012, /**< Postage stamp (icon) image */
    FLI_FILE_HEADER  = 0x0AF12,
    FLI_PREFIX_CHUNK = 0x0F100,
    FLI_FRAME_CHUNK  = 0x0F1FA,
    FLI_SEGMENT_TABLE= 0x0F1FB,
};

struct FLCFileHeader {
    u32 Size;
    ushort Magic;
    ushort NumberOfFrames;
    ushort Width;
    ushort Height;
#if defined(LB_ENABLE_FLIC_FULL_HEADER)
	ushort Depth;
	ushort Flags;
	u32 FrameSpeed;
	short Reserved2;
	u32 Created;
	u32 Creator;
	u32 Updated;
	u32 Updater;
	short AspectX;
	short AspectY;
	ubyte Reserved3[38];
	u32 OffsetFrame1;
	u32 OffsetFrame2;
	ubyte Reserved4[40];
#endif
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
	/** Buffer with animation frame pixel data.
     * Can be a screen buffer, or another chunkof memory where decoded frame
     * will be put, or from where data will be used to encode next frame.
     */
    ubyte *FrameBuffer;
    long anfield_4;
    short Xpos;
    short Ypos;
	/** Zero-based number of the frame to be played / recorded next.
     */
    short FrameNumber;
    ushort Flags;
    ubyte *ChunkBuf;
	/** Main FLI header for the currenly played / recorded file.
     */
    struct FLCFileHeader FLCFileHeader;
	/** Frame FLI header for the last played played / recorded frame.
     */
    struct FLCFrameChunk FLCFrameChunk;
    long anfield_30;
	/** Buffer with previously encoded animation frame pixel data.
     * Used only for recording, unused during playback.
     */
    ubyte *PvFrameBuf;
	/** File handle, for either playback or writing the FLI data.
     */
    TbFileHandle FileHandle;
	/** FLI File name string.
     */
    char Filename[48];
	/** Line length of the currently set Frame Buffer.
     */
    short Scanline;
	/** Animation type, defined on the app side.
     * This value is irrelevant for playback / record, but can be used
     * to store information by the app using this Animation.
     */
    short Type;
};

#pragma pack()
/******************************************************************************/
extern ubyte anim_palette[0x300];
extern void *anim_scratch;

void anim_show_prep_next_frame(struct Animation *p_anim);
ubyte anim_show_frame(struct Animation *p_anim);

void anim_flic_init(struct Animation *p_anim, short anmtype, ushort flags);
void anim_flic_set_frame_buffer(struct Animation *p_anim, ubyte *obuf,
  short x, short y, short scanln, ushort flags);
void anim_flic_set_fname(struct Animation *p_anim, const char *format, ...);
TbResult anim_flic_open(struct Animation *p_anim);
TbBool anim_is_opened(struct Animation *p_anim);
void anim_flic_close(struct Animation *p_anim);

TbResult anim_make_open(struct Animation *p_anim, int width, int height,
  int bpp, uint flags);

// Low level interface
void anim_show_FLI_SS2(struct Animation *p_anim);
void anim_show_FLI_BRUN(struct Animation *p_anim);
void anim_show_FLI_LC(struct Animation *p_anim);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
