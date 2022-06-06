/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file giff.c
 *     Implementation of Interchange File Format image format read/write.
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
#include "bfiff.h"

#include <stdio.h>
#include <errno.h>
#include "bftypes.h"
#include "bffnuniq.h"
#include "bfscreen.h"
#include "bflog.h"

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

int LbIffLoad_UNUSED()
{
// code at 0001:000b2e10
}

TbResult LbIffWrite(FILE *img_fh, unsigned char *inp_buffer,
    unsigned char *pal)
{
    ubyte pxcol;
    unsigned short img_w;
    unsigned short img_h;
    unsigned int i, k;
    ubyte *palptr;
    ubyte transClr;
    unsigned long lenChunk;
    unsigned long pxpos;
    unsigned char *inp;

    palptr = pal;
    pal[0] = 0;
    pal[1] = 0;
    pal[2] = 0;
    transClr = 0;

    img_w = lbDisplay.PhysicalScreenWidth;
    img_h = lbDisplay.PhysicalScreenHeight;
    unsigned long lenCmapChunk, lenTinyChunk, lenBodyChunk;
    unsigned int algn_w = (lbDisplay.PhysicalScreenWidth + 3) & ~0x3;
    unsigned int algn_h = (lbDisplay.PhysicalScreenHeight + 3) & ~0x3;
    unsigned int tiny_w = algn_w >> 2;
    unsigned int tiny_h = algn_h >> 2;
    lenCmapChunk = 3 * 256;
    lenTinyChunk = 4 + tiny_w * tiny_h;
    lenBodyChunk = img_h * img_w;
    // Format descriptor - chunkID
    fputc('F', img_fh);
    fputc('O', img_fh);
    fputc('R', img_fh);
    fputc('M', img_fh);
    // Format descriptor - lenChunk
    lenChunk = 4 + 8 + 20 + 8 + lenCmapChunk + 8 + lenBodyChunk + 8 + lenTinyChunk;
    fputc((lenChunk >> 24) & 0xFF, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xFF, img_fh);
    fputc((lenChunk >>  0) & 0xFF, img_fh);
    // formatID
    fputc('P', img_fh);
    fputc('B', img_fh);
    fputc('M', img_fh);
    fputc(' ', img_fh);
    // Bitmap Header - chunkID
    fputc('B', img_fh);
    fputc('M', img_fh);
    fputc('H', img_fh);
    fputc('D', img_fh);
    // Bitmap Header - lenChunk
    fputc(0, img_fh);
    fputc(0, img_fh);
    fputc(0, img_fh);
    fputc(20, img_fh);
    // Bitmap Header - width
    fputc((img_w >> 8) & 0xFF, img_fh);
    fputc((img_w >> 0) & 0xFF, img_fh);
    // Bitmap Header - height
    fputc((img_h >> 8) & 0xFF, img_fh);
    fputc((img_h >> 0) & 0xFF, img_fh);
    // Bitmap Header - xOrigin
    fputc(0, img_fh);
    fputc(0, img_fh);
    // Bitmap Header - yOrigin
    fputc(0, img_fh);
    fputc(0, img_fh);
    // Bitmap Header - numPlanes
    fputc(8, img_fh);
    // Bitmap Header - mask
    fputc(0, img_fh);
    // Bitmap Header - compression
    fputc(0, img_fh);
    // Bitmap Header - pad1
    fputc(0, img_fh);
    // Bitmap Header - transClr
    fputc(transClr, img_fh);
    fputc(64, img_fh);
    // Bitmap Header - xAspect
    fputc(5, img_fh);
    // Bitmap Header - yAspect
    fputc(6, img_fh);
    // Bitmap Header - pageWidth
    fputc((img_w >> 8) & 0xFF, img_fh);
    fputc((img_w >> 0) & 0xFF, img_fh);
    // Bitmap Header - pageHeight
    fputc((img_h >> 8) & 0xFF, img_fh);
    fputc((img_h >> 0) & 0xFF, img_fh);
    // Palette - chunkID
    fputc('C', img_fh);
    fputc('M', img_fh);
    fputc('A', img_fh);
    fputc('P', img_fh);
    // Palette - lenChunk
    lenChunk = lenCmapChunk;
    fputc((lenChunk >> 24) & 0xff, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xff, img_fh);
    fputc((lenChunk      ) & 0xff, img_fh);
    // Palette - data
    for (i = 0; i != 768; i++) {
        fputc(4 * (*palptr), img_fh);
        palptr++;
    }
    // Miniature - chunkID
    fputc('T', img_fh);
    fputc('I', img_fh);
    fputc('N', img_fh);
    fputc('Y', img_fh);
    // Miniature - lenChunk
    lenChunk = lenTinyChunk;
    fputc((lenChunk >> 24) & 0xff, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xff, img_fh);
    fputc((lenChunk      ) & 0xff, img_fh);
    // Miniature - width
    fputc((tiny_w >> 8) & 0xFF, img_fh);
    fputc((tiny_w >> 0) & 0xFF, img_fh);
    // Miniature - height
    fputc((tiny_h >> 8) & 0xFF, img_fh);
    fputc((tiny_h >> 0) & 0xFF, img_fh);
    // Miniature - data
    inp = inp_buffer;
    for (i = 0; i != (ulong)lbDisplay.PhysicalScreenHeight / 4; i++)
    {
        for (k = 0; k != (ulong)lbDisplay.PhysicalScreenWidth / 4; k++)
        {
            pxcol = *inp;
            inp += 4;
            fputc(pxcol, img_fh);
        }
        inp += 3 * lbDisplay.PhysicalScreenWidth;
    }
    // Image data - chunkID
    fputc('B', img_fh);
    fputc('O', img_fh);
    fputc('D', img_fh);
    fputc('Y', img_fh);
    // Image data - lenChunk
    lenChunk = lenBodyChunk;
    fputc((lenChunk >> 24) & 0xff, img_fh);
    fputc((lenChunk >> 16) & 0xFF, img_fh);
    fputc((lenChunk >>  8) & 0xff, img_fh);
    fputc((lenChunk      ) & 0xff, img_fh);
    // Image data - actual pixel data
    inp = inp_buffer;
    for (pxpos = lenChunk; pxpos > 0; pxpos--)
    {
        pxcol = *inp++;
        fputc(pxcol, img_fh);
    }
    return 1;
}

TbResult LbIffSave(const char *fname, unsigned char *inp_buffer,
    unsigned char *pal, TbBool force_fname)
{
    char full_fname[FILENAME_MAX];
    FILE *img_fh;
    TbResult ret;

    if ( force_fname ) {
        sprintf(full_fname, "%s", fname);
    } else {
        LbPrepareImageFilename(full_fname, fname, "lbm");
    }
    img_fh = fopen(full_fname, "wb");
    if (!img_fh) {
        LIBLOG("%s: Cannot open: %s", full_fname, strerror(errno));
        return 0;
    }
    ret = LbIffWrite(img_fh, inp_buffer, pal);
    fclose(img_fh);
    return ret;
}

/******************************************************************************/
