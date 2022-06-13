/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfbuffer.h
 *     Header file for gbuffer.cpp.
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
#ifndef BFLIBRARY_GBUFFER_H_
#define BFLIBRARY_GBUFFER_H_

#include "bftypes.h"
#include "bffile.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Initializes a lockable buffer singleton.
 *
 *  Provides pointer and size to a lockable buffer feature. Only one such buffer
 *  can be set - its internal state is static. The memory pointed can then
 *  be accessed and utilized using LbBuffer*() functions.
 *  Requires the buffer to be unlocked.
 *  The default buffer has size of 4 kbytes; this function shall be used only if
 *  the buffer size needed is larger.
 */
TbResult LbBufferSet(void *ptr, ulong size);

/** Releases/resets a lockable buffer singleton.
 *
 *  Replaces the data in internal state with one for default buffer.
 * Requires the buffer to be unlocked.
 */
TbResult LbBufferRelease(void);

/** Locks the buffer, and returns pointer and size required to its access.
 */
TbResult LbBufferLock(void **ptr, ulong *size);

/** Unlocks the previously locked buffer.
 */
TbResult LbBufferUnlock(void);

/** Sets the buffer up for file reading.
 */
TbResult LbBufferFileSetup(TbFileHandle handle);

/** Removes file reading state from the buffer.
 */
TbResult LbBufferFileReset(void);

/** Reads from associated file through the buffer.
 */
TbResult LbBufferFileRead(void *buf, int size);

/** Reads one byte from associated file through the buffer.
 * @return Gives unsigned byte value, or Lb_FAIL in case of error.
 */
short LbBufferFileReadByte(void);

/** Skip reading given amount of byte in associated file and buffer.
 */
TbResult LbBufferFileSkip(ulong len);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GBUFFER_H_
/******************************************************************************/
