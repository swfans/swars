/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file miscutil.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     Miscelanous utility functions from MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <assert.h>

#include "miscutil.h"
#include "mssal.h"
#include "memfile.h"
#include "aildebug.h"
/******************************************************************************/
extern int32_t disk_err;

int32_t XMI_message_size(int32_t status)
{
    switch (status & 0xf0)
    {
    case MDI_EV_NOTE_OFF:
    case MDI_EV_NOTE_ON:
    case MDI_EV_POLY_PRESS:
    case MDI_EV_CONTROL:
    case MDI_EV_PITCH:
        return 3;

    case MDI_EV_PROGRAM:
    case MDI_EV_CHAN_PRESS:
        return 2;
    }

   return 0;
}

void *AIL_API_file_read(const char *fname, void *dest)
{
    int fh;
    uint32_t i;
    uint32_t len;
    uint32_t readamt;
    uint8_t *buf, *mem;

    disk_err = 0;

    fh = open(fname, O_RDONLY);
    if (fh == -1)
    {
        disk_err = AIL_FILE_NOT_FOUND;
        AIL_set_error("Unable to open file.");
        return NULL;
    }

    len = lseek(fh, 0, SEEK_END);
    lseek(fh, 0, SEEK_SET);

    if ((dest == NULL) || (dest == FILE_READ_WITH_SIZE))
        buf = mem = (uint8_t*) AIL_MEM_alloc_lock(len + 4 + 64 + 128);
    else
        buf = mem = (uint8_t*) dest;

    if (buf == NULL)
    {
        disk_err = AIL_OUT_OF_MEMORY;
        AIL_set_error("Out of memory.");
        close(fh);
        return NULL;
    }

    if (dest == FILE_READ_WITH_SIZE)
    {
        *((int32_t*) buf) = len;
        buf += 4;
    }

    while (len)
    {
        readamt = (uint16_t)((len >= 32768) ? 32768 : len);

        i = read(fh, buf, readamt);

        if (i == 0)
        {
            if (dest != mem)
                AIL_MEM_free_lock(mem, len + 4 + 64 + 128);
            close(fh);
            disk_err = AIL_CANT_READ_FILE;
            AIL_set_error("Unable to read file.");

            return NULL;
        }

        len -= i;
        buf = (uint8_t*) AIL_ptr_add(buf, i);
    }

    close(fh);
    return mem;
}
/******************************************************************************/
