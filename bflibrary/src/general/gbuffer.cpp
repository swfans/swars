/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gbuffer.cpp
 *     Implementation of related functions.
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
#include <cstring>
#include "bfbuffer.h"
#include "bffile.h"

#define DEFAULT_BUFFER_SIZE 0x1000

ubyte default_buffer[DEFAULT_BUFFER_SIZE];
void *buffer_ptr = default_buffer;
ulong buffer_size = DEFAULT_BUFFER_SIZE;
TbBool buffer_locked = false;

TbFileHandle bf_file_handle = INVALID_FILE;
ubyte *bf_buffer = NULL;
ubyte *bf_buffer_ptr = NULL;
ubyte *bf_end_buffer_ptr = NULL;
ulong bf_buffer_size = 0;
TbBool bf_finished = false;

TbResult LbBufferSet(void *ptr, ulong size)
{
    if (buffer_locked)
        return Lb_FAIL;
    if (buffer_ptr != default_buffer)
        return Lb_FAIL;
    if (size < DEFAULT_BUFFER_SIZE)
        return Lb_FAIL;

    buffer_ptr = ptr;
    buffer_size = size;
    return Lb_SUCCESS;
}

TbResult LbBufferRelease(void)
{
    if (buffer_locked)
        return Lb_FAIL;
    if (buffer_ptr == default_buffer)
        return Lb_FAIL;

    buffer_ptr = default_buffer;
    buffer_size = DEFAULT_BUFFER_SIZE;
    return Lb_SUCCESS;
}

TbResult LbBufferLock(void **ptr, ulong *size)
{
    *ptr = NULL;
    if (buffer_locked)
        return Lb_FAIL;

    *ptr = buffer_ptr;
    *size = buffer_size;
    buffer_locked = true;
    return Lb_SUCCESS;
}

TbResult LbBufferUnlock(void)
{
    if (!buffer_locked)
        return Lb_FAIL;

    buffer_locked = 0;
    return Lb_SUCCESS;
}

static TbResult fill_buffer(void)
{
    long len;

    if (bf_buffer_ptr == bf_end_buffer_ptr) {
        bf_finished = true;
        return Lb_FAIL;
    }
    bf_buffer_ptr = bf_buffer;
    bf_end_buffer_ptr = bf_buffer;
    len = LbFileRead(bf_file_handle, bf_buffer, bf_buffer_size);
    if (len == -1 || len == 0) {
        bf_finished = 1;
        return Lb_FAIL;
    }
    bf_end_buffer_ptr += len;
    return Lb_SUCCESS;
}

TbResult LbBufferFileSetup(TbFileHandle handle)
{
    if (bf_buffer)
        return Lb_FAIL;
    if (LbBufferLock((void **)&bf_buffer, &bf_buffer_size) != 1)
        return Lb_FAIL;

    bf_file_handle = handle;
    bf_end_buffer_ptr = &bf_buffer_ptr[bf_buffer_size];
    bf_buffer_ptr += bf_buffer_size;
    if (fill_buffer() != Lb_SUCCESS)
    {
        bf_buffer = 0;
        bf_buffer_ptr = NULL;
        bf_end_buffer_ptr = NULL;
        bf_buffer_size = 0;
        bf_file_handle = INVALID_FILE;
        bf_finished = 0;
    }
    return Lb_SUCCESS;
}

TbResult LbBufferFileReset(void)
{
    if (!bf_buffer)
        return Lb_FAIL;

    bf_buffer = 0;
    bf_buffer_ptr = NULL;
    bf_end_buffer_ptr = NULL;
    bf_buffer_size = 0;
    bf_file_handle = INVALID_FILE;
    bf_finished = 0;
    return LbBufferUnlock();
}

TbResult LbBufferFileRead(void *buf, int size)
{
    ulong chunk_len, remain;
    ubyte *p;

    if (!bf_buffer)
        return Lb_FAIL;
    if (bf_finished)
        return Lb_FAIL;

    p = (ubyte *)buf;
    remain = size;
    while (remain)
    {
        chunk_len = bf_end_buffer_ptr - bf_buffer_ptr;
        if (chunk_len >= remain)
            chunk_len = remain;

        memcpy(p, bf_buffer_ptr, chunk_len);
        bf_buffer_ptr += chunk_len;
        remain -= chunk_len;
        if (remain)
        {
            if (fill_buffer() != Lb_SUCCESS)
                return Lb_FAIL;
            p += chunk_len;
        }
    }
    return Lb_SUCCESS;
}

short LbBufferFileReadByte(void)
{
    ubyte *p;

    if (!bf_buffer)
        return Lb_FAIL;
    if (bf_finished)
        return Lb_FAIL;

    if (bf_buffer_ptr == bf_end_buffer_ptr)
    {
        if (fill_buffer() != Lb_SUCCESS)
            return Lb_FAIL;
    }

    p = bf_buffer_ptr++;
    return *p;
}

TbResult LbBufferFileSkip(ulong len)
{
    ulong chunk_len, remain;

    if (!bf_buffer)
        return Lb_FAIL;
    if (bf_finished)
        return Lb_FAIL;

    remain = len;
    chunk_len = bf_end_buffer_ptr - bf_buffer_ptr;
    if (remain > chunk_len)
    {
        ulong needed;
        TbResult ret;

        needed = remain - chunk_len;
        ret = LbFileSeek(bf_file_handle, (len - needed) / bf_buffer_size,
            Lb_FILE_SEEK_CURRENT);
        if (ret == Lb_FAIL) {
            bf_finished = 1;
            return ret;
        }
        remain = needed % bf_buffer_size;
        // If we have skipped to EOF, this is still a successful skip
        if (fill_buffer() != Lb_SUCCESS)
            return Lb_SUCCESS;
    }
    bf_buffer_ptr += remain;
    return Lb_SUCCESS;
}

/******************************************************************************/
