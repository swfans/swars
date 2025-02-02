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

/******************************************************************************/
