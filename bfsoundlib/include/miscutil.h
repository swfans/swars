/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file miscutil.h
 *     Header file for miscutil.c.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
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
#ifndef AIL2OAL_MISCUTIL_H_
#define AIL2OAL_MISCUTIL_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

extern char SoundDriverPath[144];
#define AIL_redist_directory SoundDriverPath

#define AIL_ptr_add(ptr,off) ((void*)(((uint8_t*)(ptr))+(off)))

#define MSSLockedIncrement(var) (++var)
#define MSSLockedDecrement(var) (--var)

#define MSSLockedIncrementPtr(var) (++var)
#define MSSLockedDecrementPtr(var) (--var)

/** Return size in bytes of MIDI channel voice message, based on type.
 */
int32_t XMI_message_size(int32_t status);

/** Find nth sequence in XMIDI IFF file.
 *
 * @return Gives NULL if not found, else pointer to FORM XMID
 */
const void *XMI_find_sequence(const uint8_t *image, int32_t sequence);

/** Calculates whole size on an XMI file within buffer.
 *
 * This is not a standard AIL function, it was added because AIL
 * API provides us with image without information on its size.
 * Modern functions are always expected to receive size, due
 * to security implications and posasible crashes.
 */
uint32_t XMI_whole_size(const uint8_t *image);

/** Read MIDI variable-length number.
 */
uint32_t XMI_read_VLN(const uint8_t **ptr);

void *AIL_API_file_read(const char *filename, void *dest);

static inline uint32_t XMI_swap32(uint32_t v)
{
    return (v << 24) |
        ( (v << 8) & 0x00ff0000 ) |
        ( (v >> 8) & 0x0000ff00 ) |
        ( (v >> 24) & 0x000000ff);
}

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MISCUTIL_H_
