/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file giff.c
 *     Implementation of Interchange File Format image format read/write.
 * @par Purpose:
 *     Ability to save and load images to/from screen buffers.
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
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "bfiff.h"

#include "bftypes.h"
#include "bffnuniq.h"
#include "bfscreen.h"
#include "bfbuffer.h"
#include "privbflog.h"

#pragma pack(1)

struct iff_header { // sizeof=40
    ubyte Id[4]; // offset=0
    ulong FileLength; // offset=4
    ubyte FileType[8]; // offset=8
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
    ubyte Id[4]; // offset=0
    ulong ChunkLength; // offset=4
};

typedef struct ChunkHeader ChunkHeader;

#pragma pack()

ulong swap_long_bytes(ulong v)
{
    return ((v & 0xFF00) << 8) | ((v & 0xFF0000) >> 8) | ((v & 0xFF000000) >> 24) | ((v & 0xFF) << 24);
}

ushort swap_word_bytes(ushort v)
{
    return ((v & 0x00FF) << 8) | ((v & 0xFF00) >> 8);
}

static TbResult read_body(struct iff_header *iffhead, struct ChunkHeader *chead, ubyte *buf)
{
    assert(!"Not implemented");
    return Lb_FAIL;
}

static TbResult read_cmap(struct ChunkHeader *chead, ubyte *pal)
{
    assert(!"Not implemented");
    return Lb_FAIL;
}

static TbResult read_iff(struct TbIff *iff, ubyte *buf)
{
    ulong fourcc;
    struct iff_header iffhead;
    struct ChunkHeader chead;

    if (iff->Palette != NULL)
        iff->Palette->Loaded = 0;
    if (LbBufferFileRead(&iffhead, 40) != Lb_SUCCESS)
        return Lb_FAIL;

    fourcc = iffhead.Id[3]
         + (iffhead.Id[2] << 8)
         + (iffhead.Id[1] << 16)
         + (iffhead.Id[0] << 24);
    if (fourcc != 0x464F524D) // 'FORM'
        return Lb_FAIL;

    fourcc = iffhead.FileType[3]
         + (iffhead.FileType[2] << 8)
         + (iffhead.FileType[1] << 16)
         + (iffhead.FileType[0] << 24);
    if (fourcc != 0x50424D20 && // 'PBM '
        fourcc != 0x494C424D) // 'ILBM'
        return Lb_FAIL;

    fourcc = iffhead.FileType[7]
         + (iffhead.FileType[6] << 8)
         + (iffhead.FileType[5] << 16)
         + (iffhead.FileType[4] << 24);
    if (fourcc != 0x424D4844) // 'BMHD'
        return Lb_FAIL;

    iffhead.FileLength = swap_long_bytes(iffhead.FileLength);
    iff->FileSize = iffhead.FileLength;
    iffhead.Width = swap_word_bytes(iffhead.Width);
    iff->Width = iffhead.Width;
    iffhead.Height = swap_word_bytes(iffhead.Height);
    iff->Height = iffhead.Height;
    iffhead.XOffset = swap_word_bytes(iffhead.XOffset);
    iff->XOffset = iffhead.XOffset;
    iffhead.YOffset = swap_word_bytes(iffhead.YOffset);
    iff->YOffset = iffhead.YOffset;
    iff->XAspect = iffhead.XAspectRatio;
    iff->YAspect = iffhead.YAspectRatio;
    iffhead.HeaderLength = swap_long_bytes(iffhead.HeaderLength);
    iffhead.Transparent = swap_word_bytes(iffhead.Transparent);
    iffhead.PageWidth = swap_word_bytes(iffhead.PageWidth);
    iffhead.PageHeight = swap_word_bytes(iffhead.PageHeight);
    if (LbBufferFileRead(&chead, 8) != Lb_SUCCESS)
        return Lb_FAIL;
    do
    {
        chead.ChunkLength = swap_long_bytes(chead.ChunkLength);
        if (chead.ChunkLength & 1)
            chead.ChunkLength++;
        fourcc = (chead.Id[3]) + (chead.Id[2] << 8) + (chead.Id[1] << 16) + (chead.Id[0] << 24);
        if (fourcc == 0x424F4459) // 'BODY'
        {
            if (read_body(&iffhead, &chead, buf) == Lb_FAIL)
                return Lb_FAIL;
        }
        else if (fourcc == 0x434D4150 && iff->Palette != NULL) // 'CMAP'
        {
            if (read_cmap(&chead, iff->Palette->Palette) == Lb_FAIL)
                return Lb_FAIL;
            iff->Palette->Loaded = 1;
        }
        else
        {
            LbBufferFileSkip(chead.ChunkLength);
            continue;
        }
    }
    while (LbBufferFileRead(&chead, 8) == Lb_SUCCESS);
    return Lb_SUCCESS;
}

TbResult LbIffLoad(const char *fname, ubyte *buf, struct TbIff *iff)
{
    TbFileHandle handle;
    TbResult ret;

    handle = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if (handle == INVALID_FILE) {
        return Lb_FAIL;
    }
    if (LbBufferFileSetup(handle) == Lb_SUCCESS)
    {
        ret = read_iff(iff, buf);
        LbBufferFileReset();
    }
    LbFileClose(handle);
    return ret;
}

TbResult LbIffWrite(FILE *img_fh, const TbPixel *inp_buffer,
    ulong width, ulong height, const ubyte *pal)
{
    ubyte pxcol;
    ushort img_w;
    ushort img_h;
    ulong i, k;
    const ubyte *palptr;
    ubyte transClr;
    ulong lenChunk;
    ulong pxpos;
    const TbPixel *inp;

    palptr = pal;
#if 0
    // Not sure why this was done, but not a good idea to modify the
    // palette we get as input
    pal[0] = 0;
    pal[1] = 0;
    pal[2] = 0;
#endif
    transClr = 0;

    img_w = width;
    img_h = height;
    unsigned long lenCmapChunk, lenTinyChunk, lenBodyChunk;
    unsigned int algn_w = (img_w + 3) & ~0x3;
    unsigned int algn_h = (img_h + 3) & ~0x3;
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
    for (i = 0; i != img_w / 4; i++)
    {
        for (k = 0; k != img_h / 4; k++)
        {
            pxcol = *inp;
            inp += 4;
            fputc(pxcol, img_fh);
        }
        inp += 3 * img_w;
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
    return Lb_SUCCESS;
}

TbResult LbIffSave(const char *fname, const TbPixel *inp_buffer,
  ulong width, ulong height, const ubyte *pal, TbBool force_fname)
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
        LOGERR("%s: cannot open: %s", full_fname, strerror(errno));
        return 0;
    }
    ret = LbIffWrite(img_fh, inp_buffer, width, height, pal);
    fclose(img_fh);
    return ret;
}

TbResult LbIffSaveScreen(const char *fname, const TbPixel *inp_buffer,
  const ubyte *pal, TbBool force_fname)
{
    return LbIffSave(fname, inp_buffer, lbDisplay.PhysicalScreenWidth,
      lbDisplay.PhysicalScreenHeight, pal, force_fname);
}

/******************************************************************************/
