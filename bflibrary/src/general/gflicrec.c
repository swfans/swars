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

#include "bffile.h"
#include "bfmemut.h"
/******************************************************************************/

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
    LbMemoryCopy(p_anim->UnkBuf, p_buf, size);
    p_anim->UnkBuf += size;
    return true;
}

u32 anim_make_FLI_COPY(struct Animation *p_anim, ubyte *screenbuf)
{
    int scrpoints;

    scrpoints = p_anim->FLCFileHeader.Height * p_anim->FLCFileHeader.Width;
    LbMemoryCopy(p_anim->UnkBuf, screenbuf, scrpoints);
    p_anim->UnkBuf += scrpoints;
    return scrpoints;
}

u32 anim_make_FLI_COLOUR256(struct Animation *p_anim, ubyte *palette)
{
    ushort *change_count;
    ubyte *kept_count;
    short colridx;
    short change_chunk_len;
    short kept_chunk_len;

    if (memcmp(anim_palette, palette, 768) == 0) {
        return 0;
    }
    change_count = (ushort *)p_anim->UnkBuf;
    p_anim->UnkBuf += 2;

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
                *p_anim->UnkBuf = kept_chunk_len;
                kept_chunk_len = 0;
                p_anim->UnkBuf++;
                kept_count = (ubyte *)p_anim->UnkBuf;
                p_anim->UnkBuf++;
            }
            ++change_chunk_len;
            *p_anim->UnkBuf = 4 * srcpal[0];
            p_anim->UnkBuf++;
            *p_anim->UnkBuf = 4 * srcpal[1];
            p_anim->UnkBuf++;
            *p_anim->UnkBuf = 4 * srcpal[2];
            p_anim->UnkBuf++;
            ++(*kept_count);
        }
        if (change_chunk_len == 1) {
            ++(*change_count);
        }
    }
    return (p_anim->UnkBuf - (ubyte *)change_count);
}

/**
 * Compress data into FLI's BRUN block (8-bit Run-Length compression).
 * @return Returns packed size of the block which was compressed.
 */
u32 anim_make_FLI_BRUN(struct Animation *p_anim, ubyte *screenbuf)
{
    ubyte *blk_begin;
    short w;
    short h;
    ubyte *sbuf;

    blk_begin = p_anim->UnkBuf;
    sbuf = screenbuf;
    for (h = p_anim->FLCFileHeader.Height; h > 0; h--)
    {
        p_anim->UnkBuf++;
        for (w = p_anim->FLCFileHeader.Width; w > 0; )
        {
            short count;
            short k;

            count = 0;
            // Counting size of RLE block
            for (k = 1; w > 1; k++)
            {
                if (sbuf[k] != sbuf[0])
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
                *p_anim->UnkBuf = (sbyte)count;
                p_anim->UnkBuf++;
                *p_anim->UnkBuf = sbuf[0];
                p_anim->UnkBuf++;
                sbuf += count;
            }
            else
            {
                if (w > 1)
                {
                    count = 0;
                    // Find the next block of at least 4 same pixels
                    for (k = 0; w > 0; k++)
                    {
                        if ((sbuf[k+1] == sbuf[k]) &&
                          (sbuf[k+2] == sbuf[k]) &&
                          (sbuf[k+3] == sbuf[k]))
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
                    *p_anim->UnkBuf = (sbyte)count;
                    p_anim->UnkBuf++;
                    LbMemoryCopy(p_anim->UnkBuf, sbuf, -count);
                    sbuf -= count;
                    p_anim->UnkBuf -= count;
                }
            }
        }
    }
    // Make the block size even
    if ((intptr_t)p_anim->UnkBuf & 1) {
        *p_anim->UnkBuf = '\0';
        p_anim->UnkBuf++;
    }
    return (p_anim->UnkBuf - blk_begin);
}

/**
 * Compress data into FLI's SS2 block.
 * @return Returns packed size of the block which was compressed.
 */
u32 anim_make_FLI_SS2(struct Animation *p_anim, ubyte *curdat, ubyte *prvdat, int interline)
{
    ubyte *blk_begin;
    ubyte *cbuf;
    ubyte *pbuf;
    ubyte *cbf;
    ubyte *pbf;
    short h;
    short w;
    short k;
    short nsame;
    short ndiff;
    short wend;
    short wendt;
    cbuf = curdat;
    pbuf = prvdat;
    ushort *lines_count;
    ushort *pckt_count;

    blk_begin = p_anim->UnkBuf;
    lines_count = (ushort *)p_anim->UnkBuf;
    p_anim->UnkBuf += 2;
    pckt_count = (ushort *)p_anim->UnkBuf;

    wend = 0;
    for (h = p_anim->FLCFileHeader.Height; h > 0; h--)
    {
        cbf = cbuf;
        pbf = pbuf;
        if (wend == 0) {
            pckt_count = (ushort *)p_anim->UnkBuf;
            p_anim->UnkBuf += 2;
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
                cbf += interline;
                pbf += interline;
                continue;
            }
            if ( w > 0 ) {
                if (wend != 0) {
                    (*pckt_count) = wend;
                    pckt_count = (ushort *)p_anim->UnkBuf;
                    p_anim->UnkBuf += 2;
                }
                wendt = 2*k;
                wend = wendt;
                while (wend > 255) {
                    *(ubyte *)p_anim->UnkBuf = 255;
                    p_anim->UnkBuf++;
                    *(ubyte *)p_anim->UnkBuf = 0;
                    p_anim->UnkBuf++;
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
                    *(ubyte *)p_anim->UnkBuf = wend;
                    p_anim->UnkBuf++;
                    *(ubyte *)p_anim->UnkBuf = -nsame;
                    p_anim->UnkBuf++;
                    *(ushort *)p_anim->UnkBuf = *(ushort *)cbf;
                    p_anim->UnkBuf+=2;
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
                        *(ubyte *)p_anim->UnkBuf = wend;
                        p_anim->UnkBuf++;
                        *(ubyte *)p_anim->UnkBuf = ndiff;
                        p_anim->UnkBuf++;
                        LbMemoryCopy(p_anim->UnkBuf, cbf, 2 * ndiff);
                        p_anim->UnkBuf += 2 * ndiff;
                        pbf += 2 * ndiff;
                        cbf += 2 * ndiff;
                        wend = 0;
                        (*pckt_count)++;
                    }
                }
            }
        }
        cbuf += interline;
        pbuf += interline;
    }

    if (p_anim->FLCFileHeader.Height+wend == 0) {
        (*lines_count) = 1;
        (*pckt_count) = 1;
        *(ubyte *)p_anim->UnkBuf = 0;
        p_anim->UnkBuf++;
        *(ubyte *)p_anim->UnkBuf = 0;
        p_anim->UnkBuf++;
    } else if (wend != 0) {
        p_anim->UnkBuf -= 2;
        (*lines_count)--;
    }
    // Make the block size even
    if ((intptr_t)p_anim->UnkBuf & 1) {
        *p_anim->UnkBuf = '\0';
        p_anim->UnkBuf++;
    }
    return p_anim->UnkBuf - blk_begin;
}

/**
 * Compress data into FLI's LC block.
 * @return Returns packed size of the block which was compressed.
 */
u32 anim_make_FLI_LC(struct Animation *p_anim, ubyte *curdat, ubyte *prvdat, int interline)
{
    ubyte *blk_begin;
    ubyte *cbuf;
    ubyte *pbuf;
    ubyte *cbf;
    ubyte *pbf;
    ubyte *outptr;
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

    blk_begin = p_anim->UnkBuf;
    cbuf = curdat;
    pbuf = prvdat;
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
        cbuf += interline;
        pbuf += interline;
    }
    if (hend != 0) {
        hend = p_anim->FLCFileHeader.Height - hend;
        blksize = p_anim->FLCFileHeader.Width * (p_anim->FLCFileHeader.Height - 1);
        cbuf = curdat + blksize;
        pbuf = prvdat + blksize;
        for (h = p_anim->FLCFileHeader.Height; h > 0; h--) {
            wend = 0;
            for (w = p_anim->FLCFileHeader.Width; w > 0; w--) {
                if (cbuf[wend] != pbuf[wend])
                    break;
                wend++;
            }
            if (wend != p_anim->FLCFileHeader.Width)
                break;
            cbuf -= interline;
            pbuf -= interline;
        }
        hdim = h - hend;
        blksize = p_anim->FLCFileHeader.Width * hend;
        cbuf = curdat + blksize;
        pbuf = prvdat + blksize;
        *(ushort *)p_anim->UnkBuf = hend;
        p_anim->UnkBuf += 2;
        *(ushort *)p_anim->UnkBuf = hdim;
        p_anim->UnkBuf += 2;

        for (h = hdim; h>0; h--) {
            cbf = cbuf;
            pbf = pbuf;
            outptr = p_anim->UnkBuf++;
            for (w=p_anim->FLCFileHeader.Width; w>0; ) {
                for ( wend=0; w>0; wend++) {
                    if ( cbf[wend] != pbf[wend]) break;
                    w--;
                }
                wendt = wend;
                if (p_anim->FLCFileHeader.Width == wend) continue;
                if ( w <= 0 ) break;
                while ( wend > 255 ) {
                    *(ubyte *)p_anim->UnkBuf = 255;
                    p_anim->UnkBuf++;
                    *(ubyte *)p_anim->UnkBuf = 0;
                    p_anim->UnkBuf++;
                    wend -= 255;
                    (*(ubyte *)outptr)++;
                }
                cbf += wendt;
                pbf += wendt;
                k = 0;
                nsame = 0;
                while (w > 1) {
                    if (nsame == -127)
                        break;
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
                if ( nsame ) {
                    if ( nsame != -127 ) {
                        nsame--;
                        w--;
                    }
                    *(ubyte *)p_anim->UnkBuf = wend;
                    p_anim->UnkBuf++;
                    *(ubyte *)p_anim->UnkBuf = nsame;
                    p_anim->UnkBuf++;
                    *(ubyte *)p_anim->UnkBuf = cbf[0];
                    cbf -= nsame;
                    pbf -= nsame;
                    p_anim->UnkBuf++;
                    (*(ubyte *)outptr)++;
                } else {
                    if (w == 1) {
                        ndiff = nsame + 1;
                        w--;
                    } else {
                        k = 0;
                        ndiff = 0;
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
                    if (ndiff != 0) {
                        *(ubyte *)p_anim->UnkBuf = wend;
                        p_anim->UnkBuf++;
                        *(ubyte *)p_anim->UnkBuf = ndiff;
                        p_anim->UnkBuf++;
                        LbMemoryCopy(p_anim->UnkBuf, cbf, ndiff);
                        p_anim->UnkBuf += ndiff;
                        cbf += ndiff;
                        pbf += ndiff;
                        (*(ubyte *)outptr)++;
                    }
                }
            }
            cbuf += interline;
            pbuf += interline;
        }
    } else {
        *(short *)p_anim->UnkBuf = 0;
        p_anim->UnkBuf += 2;
        *(short *)p_anim->UnkBuf = 1;
        p_anim->UnkBuf += 2;
        *(sbyte *)p_anim->UnkBuf = 0;
        p_anim->UnkBuf++;
    }
    // Make the block size even
    if ((intptr_t)p_anim->UnkBuf & 1) {
        *p_anim->UnkBuf = '\0';
        p_anim->UnkBuf++;
    }
    return p_anim->UnkBuf - blk_begin;
}

/******************************************************************************/
