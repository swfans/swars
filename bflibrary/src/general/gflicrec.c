/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gflicrec.c
 *     Animation recording support to Autodesk FLIC format.
 * @par Purpose:
 *     Implements functions for encoding frames to FLIC animation.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 28 Jan 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfflic.h"

#include <stdlib.h>

#if defined(HAVE_CONFIG_H)
#  include "bfconfig.h"
#endif

#include "bffile.h"
#include "bfmemut.h"
#include "privbflog.h"
/******************************************************************************/
extern char anim_parse_tags[152];

TbBool anim_read_data(struct Animation *p_anim, void *buf, u32 size);

/**
 * Writes the data into FLI animation.
 * @return Returns false on error, true on success.
 */
TbBool anim_write_data(struct Animation *p_anim, void *p_buf, u32 size)
{
    return LbFileWrite(p_anim->FileHandle, p_buf, size) == size;
}

/**
 * Stores data into FLI buffer.
 * @return Returns false on error, true on success.
 */
TbBool anim_store_data(struct Animation *p_anim, void *p_buf, u32 size)
{
    LbMemoryCopy(p_anim->ChunkBuf, p_buf, size);
    p_anim->ChunkBuf += size;
    return true;
}

/**
 * Make copy of the current frame buffer to previous frame buffer.
 * @return Returns false on error, true on success.
 */
TbBool anim_update_prev_frame(struct Animation *p_anim)
{
    ubyte *sbuf;
    ubyte *obuf;
    short h;

    sbuf = p_anim->FrameBuffer;
    obuf = p_anim->PvFrameBuf;

    for (h = p_anim->FLCFileHeader.Height; h > 0; h--)
    {
        LbMemoryCopy(obuf, sbuf, p_anim->FLCFileHeader.Width);
        obuf += p_anim->FLCFileHeader.Width;
        sbuf += p_anim->Scanline;
    }
    return true;
}

u32 anim_make_FLI_COPY(struct Animation *p_anim)
{
    ubyte *blk_begin;
    ubyte *sbuf;
    short h;

    blk_begin = p_anim->ChunkBuf;
    sbuf = p_anim->FrameBuffer;

    for (h = p_anim->FLCFileHeader.Height; h > 0; h--)
    {
        LbMemoryCopy(p_anim->ChunkBuf, sbuf, p_anim->FLCFileHeader.Width);
        p_anim->ChunkBuf += p_anim->FLCFileHeader.Width;
        sbuf += p_anim->Scanline;
    }

    return (p_anim->ChunkBuf - (ubyte *)blk_begin);
}

u32 anim_make_FLI_COLOUR256(struct Animation *p_anim, ubyte *palette)
{
    ushort *change_count;
    ubyte *kept_count;
    short colridx;
    short change_chunk_len;
    short kept_chunk_len;

    if (palette == NULL) {
        return 0;
    }
    if (memcmp(anim_palette, palette, 768) == 0) {
        return 0;
    }
    change_count = (ushort *)p_anim->ChunkBuf;
    p_anim->ChunkBuf += 2;

    change_chunk_len = 0;
    kept_chunk_len = 0;
    kept_count = NULL;
    *change_count = 0;

    for (colridx = 0; colridx < 256; colridx++)
    {
        ubyte *anipal;
        ubyte *srcpal;

        anipal = &anim_palette[3 * colridx];
        srcpal = &palette[3 * colridx];

        if (memcmp(anipal, srcpal, 3) == 0) {
            change_chunk_len = 0;
            kept_chunk_len++;
        } else {
            if (!change_chunk_len) {
                *p_anim->ChunkBuf = kept_chunk_len;
                kept_chunk_len = 0;
                p_anim->ChunkBuf++;
                kept_count = (ubyte *)p_anim->ChunkBuf;
                p_anim->ChunkBuf++;
            }
            ++change_chunk_len;
            *p_anim->ChunkBuf = 4 * srcpal[0];
            p_anim->ChunkBuf++;
            *p_anim->ChunkBuf = 4 * srcpal[1];
            p_anim->ChunkBuf++;
            *p_anim->ChunkBuf = 4 * srcpal[2];
            p_anim->ChunkBuf++;
            ++(*kept_count);
        }
        if (change_chunk_len == 1) {
            ++(*change_count);
        }
    }
    return (p_anim->ChunkBuf - (ubyte *)change_count);
}

/**
 * Compress data into FLI's BRUN block (8-bit Run-Length compression).
 * @return Returns packed size of the block which was compressed.
 */
u32 anim_make_FLI_BRUN(struct Animation *p_anim)
{
    ubyte *blk_begin;
    ubyte *sbuf;
    short h;

    blk_begin = p_anim->ChunkBuf;
    sbuf = p_anim->FrameBuffer;

    for (h = p_anim->FLCFileHeader.Height; h > 0; h--)
    {
        ubyte *ssbuf;
        short w;

        ssbuf = sbuf;
        p_anim->ChunkBuf++;
        for (w = p_anim->FLCFileHeader.Width; w > 0; )
        {
            short count;
            short k;

            count = 0;
            // Counting size of RLE block
            for (k = 1; w > 1; k++)
            {
                if (ssbuf[k] != ssbuf[0])
                    break;
                if (count == 127)
                    break;
                w--;
                count++;
            }
            // If RLE block would be valid
            if (count > 0)
            {
                if (count < 127) {
                    count++;
                    w--;
                }
                *p_anim->ChunkBuf = (sbyte)count;
                p_anim->ChunkBuf++;
                *p_anim->ChunkBuf = ssbuf[0];
                p_anim->ChunkBuf++;
                ssbuf += count;
            }
            else
            {
                if (w > 1)
                {
                    count = 0;
                    // Find the next block of at least 4 same pixels
                    for (k = 0; w > 0; k++)
                    {
                        if ((ssbuf[k+1] == ssbuf[k]) &&
                          (ssbuf[k+2] == ssbuf[k]) &&
                          (ssbuf[k+3] == ssbuf[k]))
                            break;
                        if (count == -127)
                            break;
                        count--;
                        w--;
                    }
                }
                else
                {
                    count =- 1;
                    w--;
                }
                if (count != 0)
                {
                    *p_anim->ChunkBuf = (sbyte)count;
                    p_anim->ChunkBuf++;
                    LbMemoryCopy(p_anim->ChunkBuf, ssbuf, -count);
                    ssbuf -= count;
                    p_anim->ChunkBuf -= count;
                }
            }
        }
        sbuf += p_anim->Scanline;
    }
    // Make the block size even (req. by FLI spec)
    if ((intptr_t)p_anim->ChunkBuf & 1) {
        *p_anim->ChunkBuf = '\0';
        p_anim->ChunkBuf++;
    }
    return (p_anim->ChunkBuf - blk_begin);
}

/**
 * Compress data into FLI's SS2 block.
 * @return Returns packed size of the block which was compressed.
 */
u32 anim_make_FLI_SS2(struct Animation *p_anim)
{
    ubyte *blk_begin;
    ubyte *cbuf;
    ubyte *pbuf;
    ubyte *cbf;
    ubyte *pbf;
    ushort *lines_count;
    ushort *pckt_count;
    short h;
    short w;
    short k;
    short nsame;
    short ndiff;
    short wend;
    short wendt;
    cbuf = p_anim->FrameBuffer;
    pbuf = p_anim->PvFrameBuf;

    blk_begin = p_anim->ChunkBuf;
    lines_count = (ushort *)p_anim->ChunkBuf;
    p_anim->ChunkBuf += 2;
    pckt_count = (ushort *)p_anim->ChunkBuf;

    wend = 0;
    for (h = p_anim->FLCFileHeader.Height; h > 0; h--)
    {
        cbf = cbuf;
        pbf = pbuf;
        if (wend == 0) {
            pckt_count = (ushort *)p_anim->ChunkBuf;
            p_anim->ChunkBuf += 2;
            (*lines_count)++;
        }
        for (w = p_anim->FLCFileHeader.Width; w > 0; )
        {
            for (k = 0; w > 0; k++)
            {
                if (*(ushort *)(pbf + 2 * k) != *(ushort *)(cbf + 2 * k))
                    break;
                w -= 2;
            }
            if (2 * k == p_anim->FLCFileHeader.Width)
            {
                wend--;
                cbf += p_anim->Scanline;
                pbf += p_anim->Scanline;
                continue;
            }
            if ( w > 0 ) {
                if (wend != 0) {
                    (*pckt_count) = wend;
                    pckt_count = (ushort *)p_anim->ChunkBuf;
                    p_anim->ChunkBuf += 2;
                }
                wendt = 2*k;
                wend = wendt;
                while (wend > 255) {
                    *(ubyte *)p_anim->ChunkBuf = 255;
                    p_anim->ChunkBuf++;
                    *(ubyte *)p_anim->ChunkBuf = 0;
                    p_anim->ChunkBuf++;
                    wend -= 255;
                    (*pckt_count)++;
                }
                cbf += wendt;
                pbf += wendt;
                for (nsame  =0; nsame < 127; nsame++)
                {
                    if (w <= 2)
                        break;
                    if ((*(ushort *)(pbf + 2 * nsame + 0) == *(ushort *)(cbf + 2 * nsame + 0)) &&
                      (*(ushort *)(pbf + 2 * nsame + 2) == *(ushort *)(cbf + 2 * nsame + 2))) {
                        break;
                    }
                    if (*(ushort *)(cbf + 2 * nsame + 2) != *(ushort *)(cbf))
                        break;
                    w -= 2;
                }
                if (nsame > 0) {
                    if (nsame < 127) {
                        nsame++;
                        w -= 2;
                    }
                    *(ubyte *)p_anim->ChunkBuf = wend;
                    p_anim->ChunkBuf++;
                    *(ubyte *)p_anim->ChunkBuf = -nsame;
                    p_anim->ChunkBuf++;
                    *(ushort *)p_anim->ChunkBuf = *(ushort *)cbf;
                    p_anim->ChunkBuf+=2;
                    pbf += 2*nsame;
                    cbf += 2*nsame;
                    wend = 0;
                    (*pckt_count)++;
                } else {
                    if (w == 2) {
                        ndiff = 1;
                        w -= 2;
                    } else {
                        for (ndiff=0; ndiff<127; ndiff++) {
                            if (w <= 0) break;
                            if (*(ushort *)(pbf + 2 * ndiff) == *(ushort *)(cbf + 2 * ndiff))
                                break;
                            if ((*(ushort *)(cbf + 2 * (ndiff+1)) == *(ushort *)(cbf + 2 * ndiff)) &&
                              (*(ushort *)(cbf + 2 * (ndiff+2)) == *(ushort *)(cbf + 2 * ndiff)) ) {
                                break;
                            }
                            w -= 2;
                        }
                    }
                    if (ndiff > 0) {
                        *(ubyte *)p_anim->ChunkBuf = wend;
                        p_anim->ChunkBuf++;
                        *(ubyte *)p_anim->ChunkBuf = ndiff;
                        p_anim->ChunkBuf++;
                        LbMemoryCopy(p_anim->ChunkBuf, cbf, 2 * ndiff);
                        p_anim->ChunkBuf += 2 * ndiff;
                        pbf += 2 * ndiff;
                        cbf += 2 * ndiff;
                        wend = 0;
                        (*pckt_count)++;
                    }
                }
            }
        }
        cbuf += p_anim->Scanline;
        pbuf += p_anim->Scanline;
    }

    if (p_anim->FLCFileHeader.Height + wend == 0) {
        (*lines_count) = 1;
        (*pckt_count) = 1;
        *(ubyte *)p_anim->ChunkBuf = 0;
        p_anim->ChunkBuf++;
        *(ubyte *)p_anim->ChunkBuf = 0;
        p_anim->ChunkBuf++;
    } else if (wend != 0) {
        p_anim->ChunkBuf -= 2;
        (*lines_count)--;
    }
    // Make the block size even (req. by FLI spec)
    if ((intptr_t)p_anim->ChunkBuf & 1) {
        *p_anim->ChunkBuf = '\0';
        p_anim->ChunkBuf++;
    }
    return p_anim->ChunkBuf - blk_begin;
}

/**
 * Compress data into FLI's LC block.
 * @return Returns packed size of the block which was compressed.
 */
u32 anim_make_FLI_LC(struct Animation *p_anim)
{
    ubyte *blk_begin;
    ubyte *cbuf;
    ubyte *pbuf;
    ubyte *cbf;
    ubyte *pbf;
    short h;
    short w;
    short hend;
    short wend;
    short hdim;
    short wendt;
    short k;
    short nsame;
    short ndiff;
    int blksize;

    blk_begin = p_anim->ChunkBuf;
    cbuf = p_anim->FrameBuffer;
    pbuf = p_anim->PvFrameBuf;
    // Find first line with differences
    for (hend = p_anim->FLCFileHeader.Height; hend > 0;  hend--)
    {
        wend = 0;
        for (w = p_anim->FLCFileHeader.Width; w > 0; w--)
        {
            if (cbuf[wend] != pbuf[wend])
                break;
            ++wend;
        }
        if (wend != p_anim->FLCFileHeader.Width)
            break;
        cbuf += p_anim->Scanline;
        pbuf += p_anim->Scanline;
    }
    if (hend != 0)
    {
        // Recompute to get first line with differences
        hend = p_anim->FLCFileHeader.Height - hend;
        blksize = p_anim->FLCFileHeader.Width * (p_anim->FLCFileHeader.Height - 1);
        cbuf = p_anim->FrameBuffer + blksize;
        pbuf = p_anim->PvFrameBuf + blksize;
        // Find last line with differences
        for (h = p_anim->FLCFileHeader.Height; h > 0; h--) {
            wend = 0;
            for (w = p_anim->FLCFileHeader.Width; w > 0; w--) {
                if (cbuf[wend] != pbuf[wend])
                    break;
                wend++;
            }
            if (wend != p_anim->FLCFileHeader.Width)
                break;
            cbuf -= p_anim->Scanline;
            pbuf -= p_anim->Scanline;
        }
        hdim = h - hend;
        blksize = p_anim->FLCFileHeader.Width * hend;
        cbuf = p_anim->FrameBuffer + blksize;
        pbuf = p_anim->PvFrameBuf + blksize;
        // Store amount of empty lines to skip
        *(ushort *)p_anim->ChunkBuf = hend;
        p_anim->ChunkBuf += 2;
        // Store amount of following encoded lines
        *(ushort *)p_anim->ChunkBuf = hdim;
        p_anim->ChunkBuf += 2;

        for (h = hdim; h > 0; h--)
        {
            ubyte *pckt_count;

            cbf = cbuf;
            pbf = pbuf;
            // Remember pointer to amount of encoded packets within line
            pckt_count = p_anim->ChunkBuf;
            *pckt_count = 0;
            p_anim->ChunkBuf++;
            for (w = p_anim->FLCFileHeader.Width; w > 0; )
            {
                // Skip identical pixels at line start
                for (wend = 0; w > 0; wend++) {
                    if (cbf[wend] != pbf[wend]) break;
                    w--;
                }
                wendt = wend;
                // If whole line was identical, move to next one
                if (p_anim->FLCFileHeader.Width == wend) continue;
                // If all pixels until EOLN were identical, finish packets for this line
                if (w <= 0) break;
                // Store empty pixel counts above 255 as separate packets
                while (wend > 255) {
                    // Store packet skip pixels count
                    *(ubyte *)p_anim->ChunkBuf = 255;
                    p_anim->ChunkBuf++;
                    // Store packet RLE pixels count
                    *(ubyte *)p_anim->ChunkBuf = 0;
                    p_anim->ChunkBuf++;
                    wend -= 255;
                    (*(ubyte *)pckt_count)++;
                }
                // Now the remaining empty pixel count is guaranteed to fit one byte
                cbf += wendt;
                pbf += wendt;
                // Count consecutive identical pixels
                k = 0;
                nsame = 0;
                // If not in delta mode, store pixels again if it decreases encoded size
                if ((p_anim->Flags & AniFlg_ALL_DELTA) == 0)
                {
                    while (w > 1) {
                        if (nsame == -127) // We can store up to 127 in one packet
                            break;
                        // If 3 pixels in a row did not changed from previous frame, stop counting
                        if ((cbf[k+0] == pbf[k+0]) &&
                          (cbf[k+1] == pbf[k+1]) &&
                          (cbf[k+2] == pbf[k+2]))
                            break;
                        if (cbf[k+1] != cbf[0])
                            break;
                        w--;
                        k++;
                        nsame--;
                    }
                }
                else // Delta only mode - never store unchanged pixels
                {
                    while (w > 1) {
                        if (nsame == -127) // We can store up to 127 in one packet
                            break;
                        // If even one pixel did not changed from previous frame, stop counting
                        if (cbf[k+0] == pbf[k+0])
                            break;
                        if (cbf[k+1] != cbf[0])
                            break;
                        w--;
                        k++;
                        nsame--;
                    }
                }
                if ( nsame ) {
                    if (nsame != -127) {
                        nsame--;
                        w--;
                    }
                    // Store packet skip pixels count
                    *(ubyte *)p_anim->ChunkBuf = wend;
                    p_anim->ChunkBuf++;
                    // Store packet RLE pixels count (negative count means replicate)
                    *(ubyte *)p_anim->ChunkBuf = nsame;
                    p_anim->ChunkBuf++;
                    // Store packet RLE pixel value
                    *(ubyte *)p_anim->ChunkBuf = cbf[0];
                    cbf -= nsame;
                    pbf -= nsame;
                    p_anim->ChunkBuf++;
                    (*(ubyte *)pckt_count)++;
                } else {
                    if (w == 1) {
                        ndiff = nsame + 1;
                        w--;
                    } else {
                        k = 0;
                        ndiff = 0;
                        // If not in delta mode, store pixels again if it decreases encoded size
                        if ((p_anim->Flags & AniFlg_ALL_DELTA) == 0)
                        {
                            while (w != 0) {
                                if (ndiff == 127)
                                    break;
                                if ((cbf[k+0] == pbf[k+0]) &&
                                  (cbf[k+1] == pbf[k+1]) &&
                                  (cbf[k+2] == pbf[k+2]))
                                    break;
                                if ((cbf[k+1] == cbf[k+0]) &&
                                  (cbf[k+2] == cbf[k+0]) &&
                                  (cbf[k+3] == cbf[k+0]))
                                    break;
                                w--;
                                k++;
                                ndiff++;
                            }
                        }
                        else // Delta only mode - never store unchanged pixels
                        {
                            while (w != 0) {
                                if (ndiff == 127)
                                    break;
                                if (cbf[k+0] == pbf[k+0])
                                    break;
                                if ((cbf[k+1] == cbf[k+0]) &&
                                  (cbf[k+2] == cbf[k+0]) &&
                                  (cbf[k+3] == cbf[k+0]))
                                    break;
                                w--;
                                k++;
                                ndiff++;
                            }
                        }
                    }
                    if (ndiff != 0) {
                        *(ubyte *)p_anim->ChunkBuf = wend;
                        p_anim->ChunkBuf++;
                        *(ubyte *)p_anim->ChunkBuf = ndiff;
                        p_anim->ChunkBuf++;
                        LbMemoryCopy(p_anim->ChunkBuf, cbf, ndiff);
                        p_anim->ChunkBuf += ndiff;
                        cbf += ndiff;
                        pbf += ndiff;
                        (*(ubyte *)pckt_count)++;
                    }
                }
            }
            cbuf += p_anim->Scanline;
            pbuf += p_anim->Scanline;
        }
    }
    else // All lines were identical - empty frame
    {
        // Store amount of empty lines to skip
        *(short *)p_anim->ChunkBuf = 0;
        p_anim->ChunkBuf += 2;
        // Store amount of following encoded lines
        *(short *)p_anim->ChunkBuf = 1;
        p_anim->ChunkBuf += 2;
        // Store amount of encoded packets within line
        *(sbyte *)p_anim->ChunkBuf = 0;
        p_anim->ChunkBuf++;
    }
    // Make the block size even (req. by FLI spec)
    if ((intptr_t)p_anim->ChunkBuf & 1) {
        *p_anim->ChunkBuf = '\0';
        p_anim->ChunkBuf++;
    }
    return p_anim->ChunkBuf - blk_begin;
}

u32 anim_frame_size(int width, int height, int depth)
{
    return abs(width) * abs(height) * ((depth + 7) / 8);
}

u32 anim_buffer_size(int width, int height, int depth)
{
    int n;
    n = (depth >> 3);
    if (depth % 8) n++;
    return abs(width)*abs(height)*n + 32767;
}

TbResult anim_flic_make_open(struct Animation *p_anim, int width, int height, int bpp, uint flags)
{
    if (flags & p_anim->Flags) {
        LOGERR("Cannot record anim");
        return Lb_FAIL;
    }
    if ((flags & AniFlg_RECORD) != 0) {
        LOGSYNC("Record new anim, '%s' file", p_anim->Filename);
        p_anim->Flags |= flags;

        p_anim->FileHandle = LbFileOpen(p_anim->Filename, Lb_FILE_MODE_NEW);
        if (p_anim->FileHandle == INVALID_FILE) {
            LOGERR("Cannot open anim file");
            return Lb_FAIL;
        }
        p_anim->FLCFileHeader.Magic = 0xAF12;
        p_anim->FLCFileHeader.NumberOfFrames = 0;
        p_anim->FLCFileHeader.Width = width;
        p_anim->FLCFileHeader.Height = height;
#if defined(LB_ENABLE_FLIC_FULL_HEADER)
        p_anim->FLCFileHeader.Size = 128;
        p_anim->FLCFileHeader.Depth = bpp;
        p_anim->FLCFileHeader.Flags = 0x03;
        p_anim->FLCFileHeader.FrameSpeed = 57;
        p_anim->FLCFileHeader.Created = 0;
        p_anim->FLCFileHeader.Creator = 0x464C4942;//'BILF'
        p_anim->FLCFileHeader.Updated = 0;
        p_anim->FLCFileHeader.Updater = 0x464C4942;
        p_anim->FLCFileHeader.AspectX = 6;
        p_anim->FLCFileHeader.AspectY = 5;
        p_anim->FLCFileHeader.Reserved2 = 0;
        LbMemorySet(p_anim->FLCFileHeader.Reserved3, 0, sizeof(p_anim->FLCFileHeader.Reserved3));
        p_anim->FLCFileHeader.OffsetFrame1 = 0;
        p_anim->FLCFileHeader.OffsetFrame2 = 0;
        LbMemorySet(p_anim->FLCFileHeader.Reserved4, 0, sizeof(p_anim->FLCFileHeader.Reserved4));
#endif
        if (!anim_write_data(p_anim, &p_anim->FLCFileHeader, sizeof(struct FLCFileHeader))) {
            LOGERR("Anim write error");
            LbFileClose(p_anim->FileHandle);
            p_anim->FileHandle = INVALID_FILE;
            return Lb_FAIL;
        }
        LbMemorySet(anim_palette, -1, sizeof(anim_palette));
    }
    if ((flags & AniFlg_APPEND) != 0)  {
        LOGSYNC("Resume recording, '%s' file",p_anim->Filename);
        p_anim->Flags |= flags;
        p_anim->FileHandle = LbFileOpen(p_anim->Filename, Lb_FILE_MODE_OLD);
        if (p_anim->FileHandle == INVALID_FILE) {
            LOGERR("Cannot open anim file");
            return Lb_FAIL;
        }
        // Reading header
        if (!anim_read_data(p_anim, &p_anim->FLCFileHeader, sizeof(struct FLCFileHeader))) {
            LOGERR("Anim header read error");
            LbFileClose(p_anim->FileHandle);
            p_anim->FileHandle = INVALID_FILE;
            return Lb_FAIL;
        }

        if (!anim_read_data(p_anim, &p_anim->FLCFileHeader, sizeof(struct FLCFileHeader))) {
            LOGERR("Header read failed, `%s` file", p_anim->Filename);
            p_anim->FLCFileHeader.Size = 0;
            LbFileClose(p_anim->FileHandle);
            p_anim->FileHandle = INVALID_FILE;
            return Lb_FAIL;
        }
        LOGDBG("Frame count %d, res %dx%d, `%s` file",
          (int)p_anim->FLCFileHeader.NumberOfFrames,
          (int)p_anim->FLCFileHeader.Height, (int)p_anim->FLCFileHeader.Width,
          p_anim->Filename);

        // Note that we do not support appending to files with PREFIX header
        LbFileSeek(p_anim->FileHandle, 0, Lb_FILE_SEEK_END);
        p_anim->FrameNumber = p_anim->FLCFileHeader.NumberOfFrames;
    }
    return Lb_SUCCESS;
}

void anim_make_prep_next_frame(struct Animation *p_anim, ubyte *frmbuf)
{
    u32 max_chunk_size;
    int width, height, depth;

    if (((p_anim->Flags & AniFlg_APPEND) != 0) && (frmbuf != NULL)) {
        uint pos;
        pos = p_anim->Xpos + p_anim->Scanline * p_anim->Ypos;
        p_anim->FrameBuffer = frmbuf + pos;
    }

    width = p_anim->FLCFileHeader.Width;
    height = p_anim->FLCFileHeader.Height;
#if defined(LB_ENABLE_FLIC_FULL_HEADER)
    depth = p_anim->FLCFileHeader.Depth;
#else
    depth = 8;
#endif
    p_anim->PvFrameBuf = anim_scratch;
    p_anim->ChunkBuf = anim_scratch + anim_frame_size(width, height, depth);
    max_chunk_size = anim_buffer_size(width, height, depth);
    LbMemorySet(p_anim->ChunkBuf, 0, max_chunk_size);

    // Store frame chunk
    p_anim->FLCFrameChunk.Type = FLI_FRAME_CHUNK;
    p_anim->FLCFrameChunk.Chunks = 0;
    p_anim->FLCFrameChunk.Size = 0;
    LbMemorySet(p_anim->FLCFrameChunk.Reserved_0, 0, sizeof(p_anim->FLCFrameChunk.Reserved_0));
}

TbBool anim_make_next_frame(struct Animation *p_anim, ubyte *palette)
{
    struct FLCFrameDataChunk lochunk;
    struct FLCFrameDataChunk *p_fdthunk;
    s32 scrpoints, brun_size, lc_size, ss2_size;

    LOGDBG("Start making frame %d", (int)p_anim->FrameNumber);
    anim_parse_tags[0] = 0;
    // Store frame header initially filled by `prep_next_frame`
    anim_store_data(p_anim, &p_anim->FLCFrameChunk, sizeof(struct FLCFrameChunk));

    // Remember where chunk header starts
    p_fdthunk = (struct FLCFrameDataChunk *)p_anim->ChunkBuf;
    // Store zeroed out chunk
    lochunk.Type = 0;
    lochunk.Size = 0;
    anim_store_data(p_anim, &lochunk, sizeof(struct FLCFrameDataChunk));

#if defined(LB_ENABLE_FLIC_FULL_HEADER)
    if (p_anim->FrameNumber == 0) {
        p_anim->FLCFileHeader.OffsetFrame1 = p_anim->FLCFileHeader.Size;
    } else if (p_anim->FrameNumber == 1) {
        p_anim->FLCFileHeader.OffsetFrame2 = p_anim->FLCFileHeader.Size;
    }
#endif
    if (anim_make_FLI_COLOUR256(p_anim, palette)) {
        p_anim->FLCFrameChunk.Chunks++;
        p_fdthunk->Type = FLI_COLOUR256;
        p_fdthunk->Size = p_anim->ChunkBuf - (ubyte *)p_fdthunk;
        strncat(anim_parse_tags, "COLOUR256 ", sizeof(anim_parse_tags)-1);

        // Remember where chunk header starts
        p_fdthunk = (struct FLCFrameDataChunk *)p_anim->ChunkBuf;
        // Store zeroed out chunk
        lochunk.Type = 0;
        lochunk.Size = 0;
        anim_store_data(p_anim, &lochunk, sizeof(struct FLCFrameDataChunk));
    }
    scrpoints = p_anim->FLCFileHeader.Height * p_anim->FLCFileHeader.Width;
    if ((p_anim->Flags & AniFlg_ALL_DELTA) != 0)
    {
        ubyte *dataptr;
        // Only one compression method fully supports transparency
        // Note that SS2 compression stores pixels in pairs, so
        // odd transperent pixels would be turned solid - hence no SS2
        dataptr = p_anim->ChunkBuf;
        p_anim->ChunkBuf = dataptr;
        lc_size = anim_make_FLI_LC(p_anim);
        {
            // Store the LC compressed data
            p_anim->FLCFrameChunk.Chunks++;
            p_fdthunk->Type = FLI_LC;
            strncat(anim_parse_tags, "LC ", sizeof(anim_parse_tags)-1);
        }
    }
    else if (p_anim->FrameNumber == 0)
    {
        if (anim_make_FLI_BRUN(p_anim)) {
            p_anim->FLCFrameChunk.Chunks++;
            p_fdthunk->Type = FLI_BRUN;
            strncat(anim_parse_tags, "BRUN ", sizeof(anim_parse_tags)-1);
        } else {
            anim_make_FLI_COPY(p_anim);
            p_anim->FLCFrameChunk.Chunks++;
            p_fdthunk->Type = FLI_COPY;
            strncat(anim_parse_tags, "COPY ", sizeof(anim_parse_tags)-1);
        }
    }
    else
    {
        ubyte *dataptr;
        // Determining the best compression method
        dataptr = p_anim->ChunkBuf;
        brun_size = anim_make_FLI_BRUN(p_anim);
        LbMemorySet(dataptr, 0, brun_size);
        p_anim->ChunkBuf = dataptr;
        ss2_size = anim_make_FLI_SS2(p_anim);
        LbMemorySet(dataptr, 0, ss2_size);
        p_anim->ChunkBuf = dataptr;
        lc_size = anim_make_FLI_LC(p_anim);
        if ((lc_size < ss2_size) && (lc_size < brun_size)) {
            // Store the LC compressed data
            p_anim->FLCFrameChunk.Chunks++;
            p_fdthunk->Type = FLI_LC;
            strncat(anim_parse_tags, "LC ", sizeof(anim_parse_tags)-1);
        } else if (ss2_size < brun_size) {
            // Clear the LC compressed data
            LbMemorySet(dataptr, 0, lc_size);
            p_anim->ChunkBuf = dataptr;
            // Compress with SS2 method
            anim_make_FLI_SS2(p_anim);
            p_anim->FLCFrameChunk.Chunks++;
            p_fdthunk->Type = FLI_SS2;
            strncat(anim_parse_tags, "SS2 ", sizeof(anim_parse_tags)-1);
        } else if (brun_size < scrpoints + 16) {
            // Clear the LC compressed data
            LbMemorySet(dataptr, 0, lc_size);
            p_anim->ChunkBuf = dataptr;
            // Compress with BRUN method
            anim_make_FLI_BRUN(p_anim);
            p_anim->FLCFrameChunk.Chunks++;
            p_fdthunk->Type = FLI_BRUN;
            strncat(anim_parse_tags, "BRUN ", sizeof(anim_parse_tags)-1);
        } else {
            // Clear the LC compressed data
            LbMemorySet(dataptr, 0, lc_size);
            p_anim->ChunkBuf = dataptr;
            // Store uncompressed frame data
            anim_make_FLI_COPY(p_anim);
            p_anim->FLCFrameChunk.Chunks++;
            p_fdthunk->Type = FLI_COPY;
            strncat(anim_parse_tags, "COPY ", sizeof(anim_parse_tags)-1);
        }
    }
    p_fdthunk->Size = p_anim->ChunkBuf - (ubyte *)p_fdthunk;
    LOGDBG("Chunks: %s", anim_parse_tags);
    {
        ubyte *chunk_buf_start;
        int width, height, depth;

        width = p_anim->FLCFileHeader.Width;
        height = p_anim->FLCFileHeader.Height;
#if defined(LB_ENABLE_FLIC_FULL_HEADER)
        depth = p_anim->FLCFileHeader.Depth;
#else
        depth = 8;
#endif
        chunk_buf_start = anim_scratch + anim_frame_size(width, height, depth);

        p_anim->FLCFrameChunk.Size = p_anim->ChunkBuf - chunk_buf_start;
        LbMemoryCopy(chunk_buf_start, &p_anim->FLCFrameChunk, sizeof(struct FLCFrameChunk));

        if (!anim_write_data(p_anim, chunk_buf_start, p_anim->FLCFrameChunk.Size)) {
            LOGDBG("Finished frame with error");
            return false;
        }
    }
    anim_update_prev_frame(p_anim);
    if (palette != NULL)
        LbMemoryCopy(anim_palette, palette, sizeof(anim_palette));
    p_anim->FLCFileHeader.NumberOfFrames++;
    p_anim->FrameNumber++;
    p_anim->FLCFileHeader.Size += p_anim->FLCFrameChunk.Size;
    LOGNO("Finished frame ok");
    return true;
}

/******************************************************************************/
