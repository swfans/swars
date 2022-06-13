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
#include "bfbuffer.h"

#define DEFAULT_BUFFER_SIZE 0x1000

ubyte default_buffer[DEFAULT_BUFFER_SIZE];
void *buffer_ptr = &default_buffer;
ulong buffer_size = DEFAULT_BUFFER_SIZE;
TbBool buffer_locked = false;

TbResult LbBufferSet(void *ptr, ulong size)
{
    if (buffer_locked)
        return Lb_FAIL;
    if (buffer_ptr != &default_buffer)
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
    if (buffer_ptr == &default_buffer)
        return Lb_FAIL;

    buffer_ptr = &default_buffer;
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

int LbBufferUnlock_UNUSED()
{
// code at 0001:000c9d64
}

int LbBufferFileSetup_UNUSED()
{
// code at 0001:000c9d8c
}

int LbBufferFileReset_UNUSED()
{
// code at 0001:000c9e2c
}

int LbBufferFileRead_UNUSED()
{
// code at 0001:000c9e74
}

int LbBufferFileReadByte_UNUSED()
{
// code at 0001:000c9f30
}

int LbBufferFileSkip_UNUSED()
{
// code at 0001:000c9f7c
}


/******************************************************************************/
