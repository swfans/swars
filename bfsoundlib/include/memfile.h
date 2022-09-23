/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file memfile.h
 *     Header file for memfile.c.
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
#ifndef AIL2OAL_MEMFILE_H_
#define AIL2OAL_MEMFILE_H_

#include <stdint.h>
#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Allocate memory from first megabyte of physical RAM.
 *
 * This memory is automatically locked to inhibit VMM swapping
 */
int32_t AIL_MEM_alloc_DOS(uint32_t  n_paras, void **protected_ptr,
        uint32_t *segment_far_ptr, uint32_t *selector);

/** Free memory allocated from first megabyte of physical RAM.
 */
void AIL_MEM_free_DOS(void *mem_ptr, uint32_t segment_far_ptr, uint32_t selector);

/** Allocate and free page-locked global memory for AIL resources.
 *
 * These routines should not be used for allocation of numerous small
 * objects, due to limited LDT handle space.
 *
 * Memory allocated is owned by the library, and is allocated with
 * GMEM_SHARE and GMEM_ZEROINIT attributes (MOVEABLE attribute is
 * disabled by GlobalPageLock())
 */
void *AIL_MEM_alloc_lock(uint32_t len);

void AIL_MEM_free_lock(void *ptr, uint32_t len);

/** Lock linear memory region at *start, preventing virtual-memory systems
 * from swapping it to disk.
 */
int32_t AIL_vmm_lock(void *start, uint32_t size);

/** Unlock linear memory region at *start, allowing virtual-memory systems
 * to swap it to disk.
 */
int32_t AIL_vmm_unlock(void *start, uint32_t size);

/** Lock linear memory region between *p1 and *p2, preventing
 * virtual-memory systems from swapping it to disk.
 *
 * Modern operating systems can figure out quite well which page should be
 * locked, and which shouldn't. Even if the page is swapped to disk, it will
 * get restored when page fault is generated. So outside of kernel space,
 * there is no need for memory page locking.
 *
 * p1 and p2 are linear addresses passed as near 32-bit pointers, and may
 * be specified in either order
 */
int32_t AIL_VMM_lock_range(void *p1, void *p2);

/** Unlock linear memory region between *p1 and *p2, allowing
 * virtual-memory systems to swap it to disk.
 *
 * There is no need for memory page locking on modern OSes - see details in
 * the comment for locking function.
 *
 * p1 and p2 are linear addresses passed as near 32-bit pointers, and may
 * be specified in either order
 */
int32_t AIL_VMM_unlock_range(void *p1, void *p2);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MEMFILE_H_
