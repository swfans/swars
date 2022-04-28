/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfmemut.h
 *     Header file for gmemut.cpp.
 * @par Purpose:
 *     Memory related utility functions.
 * @par Comment:
 *     Provides custom wrappers to memory routines, helpful for debug.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFMEMUT_H_
#define BFLIBRARY_BFMEMUT_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Fills memory range with given byte value.
 */
void * LbMemorySet(void *dst, unsigned char c, TbMemSize size);

/** Copies memory range over given destination.
 */
void * LbMemoryCopy(void *in_dst, const void *in_src, TbMemSize size);

/** Copies memory range over given destination, allows overlapping.
 */
void * LbMemoryMove(void *in_dst, const void *in_src, TbMemSize size);

/** Compare two memory blocks.
 *
 * @param ptr1 The first pointer.
 * @param size Amount of bytes to compare.
 */
int LbMemoryCompare(void *ptr1, void *ptr2, TbMemSize size);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFMEMUT_H_
/******************************************************************************/
