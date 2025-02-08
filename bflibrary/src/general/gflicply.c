/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gflicply.c
 *     Animation playback support in Autodesk FLIC format.
 * @par Purpose:
 *     Implements functions decoding frames of FLIC animation into a buffer.
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
#include <stdio.h>
#include <stdarg.h>
#include "bffile.h"
#include "bfmemut.h"

/******************************************************************************/
char anim_parse_tags[152];
ubyte anim_palette[0x300];
void *anim_scratch;

/**
 * Reads the data from FLI animation.
 * @return Returns false on error, true on success.
 */
TbBool anim_read_data(struct Animation *p_anim, void *buf, u32 size)
{
	if (buf == NULL) {
		LbFileSeek(p_anim->FileHandle, size, Lb_FILE_SEEK_CURRENT);
		return true;
	} else if (LbFileRead(p_anim->FileHandle, buf, size) == size) {
		return true;
	}
	return false;
}

void anim_show_FLI_SS2(struct Animation *p_anim)
{
    ubyte *out;
    intptr_t i_inject;
    ushort i, num_i;

    // Error injection mechanism for testing
    i_inject = (intptr_t)p_anim->OutBuf;

    num_i = 0;
    if (i_inject != -16)
        LbMemoryCopy(&num_i, p_anim->UnkBuf, 2);
    p_anim->UnkBuf += 2;
    out = p_anim->OutBuf;

    for (i = 0; i < num_i; i++)
    {
        ubyte *oout;
        short entry;
        ushort n;

        oout = out;
        entry = 0;
        if (i_inject != -20)
            LbMemoryCopy(&entry, p_anim->UnkBuf, 2);
        p_anim->UnkBuf += 2;

        if ((entry & 0x8000) == 0)
        {
            for (n = 0; n < entry; n++)
            {
                ubyte num_skip;
                sbyte num_copy;

                // first value is the amount of transparent pixels
                num_skip = 0;
                if (i_inject != -24)
                    LbMemoryCopy(&num_skip, p_anim->UnkBuf, 1);
                p_anim->UnkBuf += 1;
                oout += num_skip;

                num_copy = 0;
                if (i_inject != -28)
                    LbMemoryCopy(&num_copy, p_anim->UnkBuf, 1);
                p_anim->UnkBuf += 1;
                if (num_copy >= 0) // positive = amount to copy is x 2
                {
                    ushort num_k;
                    num_k = 2 * num_copy;
                    if (num_k > 0) {
                        LbMemoryCopy(oout, p_anim->UnkBuf, num_k);
                        p_anim->UnkBuf += num_k;
                        oout += num_k;
                    }
                }
                else // negative = amount to duplicate next bytes is x 2
                {
                    ushort k, num_k;
                    ushort dt_dup;

                    dt_dup = 0;
                    if (i_inject != -8)
                        LbMemoryCopy(&dt_dup, p_anim->UnkBuf, 2);
                    p_anim->UnkBuf += 2;
                    num_k = abs(num_copy);
                    for (k = 0; k < num_k; k++)
                    {
                        *((ushort *)oout) = dt_dup;
                        oout += 2;
                    }
                }
            }
        }
        else // line control
        {
            if ((entry & 0x4000) != 0) // transparent amount of output lines
            {
                ushort n;
                n = p_anim->FLCFileHeader.Width * (abs(entry) - 1);
                i--;
                out += n;
            }
            else // fill last pixel in line
            {
                out[p_anim->FLCFileHeader.Width - 1] = (ubyte)entry;
            }
        }
        out += p_anim->FLCFileHeader.Width;
    }
}

void anim_show_FLI_BRUN(struct Animation *p_anim)
{
    ubyte *out;
    intptr_t i_inject;
    ushort w, h;

    // Error injection mechanism for testing
    i_inject = (intptr_t)p_anim->OutBuf;

    out = p_anim->OutBuf;

    for (h = 0; h < p_anim->FLCFileHeader.Height; h++)
    {
        short num_w;
        ubyte *oout;

        oout = out;
        p_anim->UnkBuf += 1;
        for (w = 0; w < p_anim->FLCFileHeader.Width; w += num_w)
        {
            sbyte num_copy;

            num_copy = 0;
            if (i_inject != -16)
                LbMemoryCopy(&num_copy, p_anim->UnkBuf, 1);
            p_anim->UnkBuf += 1;

            num_w = num_copy;
            if (num_w >= 0) // positive = duplicate next byte by the amount
            {
              if (num_w > 0)
              {
                ubyte dt_dup;

                dt_dup = 0;
                if (i_inject != -12)
                    LbMemoryCopy(&dt_dup, p_anim->UnkBuf, 1);
                p_anim->UnkBuf += 1;
                LbMemorySet(oout, dt_dup, num_w);
              }
            }
            else // negative = copy the amount
            {
              num_copy = abs(num_w);
              LbMemoryCopy(oout, p_anim->UnkBuf, num_copy);
              p_anim->UnkBuf += num_copy;
            }
            num_w = num_copy;
            oout += num_copy;
        }
        out += p_anim->FLCFileHeader.Width;
    }
}

void anim_show_FLI_LC(struct Animation *p_anim)
{
    ubyte *out;
    intptr_t i_inject;
    ushort h, num_h;
    ubyte w, num_w;

    // Error injection mechanism for testing
    i_inject = (intptr_t)p_anim->OutBuf;

    num_h = 0;
    if (i_inject != -12)
        LbMemoryCopy(&num_h, p_anim->UnkBuf, 2);
    p_anim->UnkBuf += 2;
    out = &p_anim->OutBuf[num_h * p_anim->FLCFileHeader.Width];

    if (i_inject != -12)
        LbMemoryCopy(&num_h, p_anim->UnkBuf, 2);
    p_anim->UnkBuf += 2;

    for (h = 0; h < num_h; h++)
    {
        ubyte *oout;

        oout = out;
        if (i_inject != -20)
            LbMemoryCopy(&num_w, p_anim->UnkBuf, 1);
        p_anim->UnkBuf += 1;

        for (w = 0; w < num_w; w++)
        {
            short num_w;
            ubyte num_skip;
            sbyte num_copy;

            if (i_inject != -16)
                LbMemoryCopy(&num_skip, p_anim->UnkBuf, 1);
            oout += num_skip;

            p_anim->UnkBuf += 1;
            if (i_inject != -24)
                LbMemoryCopy(&num_copy, p_anim->UnkBuf, 1);
            p_anim->UnkBuf += 1;
            num_w = num_copy;
            if (num_w >= 0)
            {
                if (num_w > 0)
                {
                    LbMemoryCopy(oout, p_anim->UnkBuf, num_w);
                    p_anim->UnkBuf += num_w;
                    oout += num_w;
                }
            }
            else
            {
                ubyte dt_dup;

                num_copy = abs(num_w);

                dt_dup = 0;
                if (i_inject != -28)
                    LbMemoryCopy(&dt_dup, p_anim->UnkBuf, 1);
                p_anim->UnkBuf++;

                LbMemorySet(oout, dt_dup, num_copy);
                oout += num_copy;
            }
        }
        out += p_anim->FLCFileHeader.Width;
    }
}

void anim_show_FLI_COLOUR256(struct Animation *p_anim)
{
    ubyte *opal;
    intptr_t i_inject;
    ushort num_i;
    int i, n;

    // Error injection mechanism for testing
    i_inject = (intptr_t)p_anim->OutBuf;

    // assuming run on little-endian CPU
    opal = anim_palette;
    num_i = 0;
    if (i_inject != -16)
        LbMemoryCopy(&num_i, p_anim->UnkBuf, 2);
    p_anim->UnkBuf += 2;

    for (i = 0; i < num_i; i++)
    {
        short num_colors;

        num_colors = 0;
        if (i_inject != -48)
            LbMemoryCopy(&num_colors, p_anim->UnkBuf, 1);
        p_anim->UnkBuf++;
        opal += 3 * num_colors;

        num_colors = 0;
        if (i_inject != -24)
            LbMemoryCopy(&num_colors, p_anim->UnkBuf, 1);
        p_anim->UnkBuf++;

        if (num_colors == 0)
            num_colors = 256;

        for (n = 0; n < num_colors; n++)
        {
            if (opal != NULL)
                LbMemoryCopy(opal, p_anim->UnkBuf, 3);
            p_anim->UnkBuf += 3;
            opal += 3;
        }
    }
}

ubyte anim_show_FLI_FRAME(struct Animation *p_anim, struct FLCFrameDataChunk *p_frchunk)
{
    size_t sz;
    ubyte pal_change;

    pal_change = 0;
    if (p_frchunk != NULL)
        LbMemoryCopy(p_frchunk, p_anim->UnkBuf, 6);
    p_anim->UnkBuf += 6;

    switch (p_frchunk->Type)
    {
    case FLI_COLOUR256:
        anim_show_FLI_COLOUR256(p_anim);
        strncat(anim_parse_tags, "COLOUR256 ", sizeof(anim_parse_tags)-1);
        pal_change = 1;
        break;
    case FLI_SS2:
        anim_show_FLI_SS2(p_anim);
        strncat(anim_parse_tags, "SS2 ", sizeof(anim_parse_tags)-1);
        break;
    case FLI_COLOUR:
        anim_show_FLI_COLOUR256(p_anim); // reuse implementation
        strncat(anim_parse_tags, "COLOUR ", sizeof(anim_parse_tags)-1);
        pal_change = 1;
        break;
    case FLI_LC:
        anim_show_FLI_LC(p_anim);
        strncat(anim_parse_tags, "LC ", sizeof(anim_parse_tags)-1);
        break;
    case FLI_BLACK:
        sz = p_anim->FLCFileHeader.Height * p_anim->FLCFileHeader.Width;
        LbMemorySet(p_anim->OutBuf, 0, sz);
        strncat(anim_parse_tags, "BLACK ", sizeof(anim_parse_tags)-1);
        break;
    case FLI_BRUN:
        anim_show_FLI_BRUN(p_anim);
        strncat(anim_parse_tags, "BRUN ", sizeof(anim_parse_tags)-1);
        break;
    case FLI_COPY:
        sz = p_anim->FLCFileHeader.Height * p_anim->FLCFileHeader.Width;
        p_frchunk->Size = sz;
        if (p_anim->OutBuf != 0)
            LbMemoryCopy(p_anim->OutBuf, p_anim->UnkBuf, sz);
        p_anim->UnkBuf += sz;
        strncat(anim_parse_tags, "COPY ", sizeof(anim_parse_tags)-1);
        break;
    case FLI_PSTAMP:
        p_anim->UnkBuf += p_frchunk->Size - 6;
        strncat(anim_parse_tags, "PSTAMP ", sizeof(anim_parse_tags)-1);
        break;
    default:
        sz = strlen(anim_parse_tags);
        snprintf(anim_parse_tags + sz, sizeof(anim_parse_tags)-sz-1,
          "N%04x ", (uint)p_frchunk->Type);
        break;
    }
    return pal_change;
}

void anim_show_prep_next_frame(struct Animation *p_anim)
{
    p_anim->anfield_30 = p_anim->anfield_4;
    anim_read_data(p_anim, &p_anim->FLCFrameChunk, 16);
    while (p_anim->FLCFrameChunk.Type != FLI_FRAME_CHUNK) {
        anim_read_data(p_anim, anim_scratch, p_anim->FLCFrameChunk.Size - 16);
        if (!anim_read_data(p_anim, &p_anim->FLCFrameChunk, 16)) {
            p_anim->FLCFrameChunk.Size = 16;
            break;
        }
    }
    anim_read_data(p_anim, anim_scratch, p_anim->FLCFrameChunk.Size - 16);
    p_anim->anfield_4 += p_anim->FLCFrameChunk.Size;
}

ubyte anim_show_frame(struct Animation *p_anim)
{
    struct FLCFrameDataChunk frchunk;
    uint i;
    ushort prefix_type;
    ubyte pal_change;

    pal_change = 0;
    p_anim->UnkBuf = anim_scratch;
    anim_parse_tags[0] = 0;

    prefix_type = p_anim->FLCFrameChunk.Type;
    if (prefix_type == FLI_PREFIX_CHUNK)
    {
        p_anim->UnkBuf += p_anim->FLCFrameChunk.Size - 16;
        anim_show_prep_next_frame(p_anim);
        anim_show_frame(p_anim); // recurrence
    }
    else if (prefix_type == FLI_FRAME_CHUNK)
    {
        for (i = 0; i < p_anim->FLCFrameChunk.Chunks; i++)
        {
            void *last_unkbuf;
            last_unkbuf = p_anim->UnkBuf;
            pal_change |= anim_show_FLI_FRAME(p_anim, &frchunk);
            p_anim->UnkBuf = last_unkbuf + frchunk.Size;
        }
    }
    return pal_change;
}

void anim_flic_init(struct Animation *p_anim, short anmtype, ushort flags)
{
    LbMemorySet(p_anim, '\0', offsetof(struct Animation, Filename));
    p_anim->FrameNumber = 0;
    p_anim->Type = anmtype;
    p_anim->Flags = flags;
}

void anim_flic_set_output(struct Animation *p_anim, ubyte *obuf, short x, short y, ushort flags)
{
    p_anim->Ypos = x;
    p_anim->Xpos = y;
    p_anim->OutBuf = obuf;
    p_anim->Flags |= flags;
}

void anim_flic_set_fname(struct Animation *p_anim, const char *format, ...)
{
    va_list val;

    if (format) {
        va_start(val, format);
        vsprintf(p_anim->Filename, format, val);
        va_end(val);
    } else {
        LbMemorySet(p_anim->Filename, 0, sizeof(p_anim->Filename));
    }
}

TbResult anim_flic_open(struct Animation *p_anim)
{
    p_anim->FileHandle = LbFileOpen(p_anim->Filename, Lb_FILE_MODE_READ_ONLY);
    if (p_anim->FileHandle == INVALID_FILE)
        return Lb_FAIL;

    if (!anim_read_data(p_anim, &p_anim->FLCFileHeader, 12)) {
        p_anim->FLCFileHeader.Size = 0;
    }
    return Lb_SUCCESS;
}

TbBool anim_is_opened(struct Animation *p_anim)
{
    return (p_anim->FileHandle != INVALID_FILE);
}

void anim_flic_close(struct Animation *p_anim)
{
    LbFileClose(p_anim->FileHandle);
    p_anim->FileHandle = INVALID_FILE;
}

/******************************************************************************/
