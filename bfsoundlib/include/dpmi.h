/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file dpmi.h
 *     Header file for dpmi.c.
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
#ifndef AIL2OAL_DPMI_H_
#define AIL2OAL_DPMI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

/**
 * Allocates memory block in the first megabyte of RAM.
 *
 * @param size memory block size in bytes
 * @return protected mode pointer to the allocated block, NULL if failed.
 */
void *AllocDOSmem(long size);

/** Frees a memory block allocated by AllocDOSmem().
 *
 * @param block Protected mode pointer to memory block.
 * @param seg Segment of the block; for API compatibility, unused.
 * @return OK (0) or ERROR (-1)
 */
int FreeDOSmem(void *block, uint16_t seg);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_DPMI_H_
