/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file memfile.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     Memory and file handling functions from MSS API.
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
#include <assert.h>

#include "memfile.h"
#include "msssys.h"
#include "bfmemory.h"
/******************************************************************************/
extern AILMEMALLOCCB MEM_alloc;
extern AILMEMFREECB MEM_free;

int32_t AIL_MEM_alloc_DOS(uint32_t n_paras, void **protected_ptr,
        uint32_t *segment_far_ptr, uint32_t *selector)
{
    void *ptr;

    ptr = LbMemoryAllocLow(16 * n_paras);
    if (ptr == NULL)
        return 0;

    *segment_far_ptr = ((uint32_t)ptr >> 4) << 16;
    *protected_ptr = ptr;
    *selector = 0x4D0;
    AIL_VMM_lock_range((void *)(*segment_far_ptr >> 12),
        (void *)((*segment_far_ptr >> 12) + 16 * n_paras - 1));
    return 1;
}

void AIL_MEM_free_DOS(void *mem_ptr, uint32_t segment_far_ptr, uint32_t selector)
{
    LbMemoryFree(mem_ptr);
}

void *AIL_MEM_alloc_lock(uint32_t len)
{
    void *ptr;

    ptr = MEM_alloc(len);
    if (ptr != NULL)
        AIL_vmm_lock(ptr, len);
    return ptr;
}

void AIL_MEM_free_lock(void *ptr, uint32_t len)
{
    if (ptr == NULL)
        return;

    AIL_vmm_unlock(ptr, len);

    MEM_free(ptr);
}

int32_t AIL_vmm_lock(void *start, uint32_t size)
{
   return AIL_VMM_lock_range(start,(uint8_t *) start + size-1);
}

int32_t AIL_vmm_unlock(void *start, uint32_t size)
{
   return AIL_VMM_unlock_range(start,(uint8_t *) start + size-1);
}

int32_t AIL_VMM_lock_range(void *p1, void *p2)
{
#if 0
    DOS_Registers inregs, outregs;
    uint32_t linear;
    uint32_t len;

    linear =  min(((uint32_t) p1), ((uint32_t) p2));
    len    = (max(((uint32_t) p1), ((uint32_t) p2)) - linear) + 1;

    memset(&inregs, 0, sizeof(inregs));

    inregs.r32.eax = 0x600;
    inregs.r32.ebx = linear >> 16;
    inregs.r32.ecx = linear &  0xffff;
    inregs.r32.esi = len    >> 16;
    inregs.r32.edi = len    &  0xffff;

    dos_int386(0x31, &inregs, &outregs);

    return outregs.r32.eflag ? 0 : 1;
#else
    return 0;
#endif
}

int32_t AIL_VMM_unlock_range(void *p1, void *p2)
{
#if 0
    DOS_Registers inregs, outregs;
    uint32_t linear;
    uint32_t len;

    linear =  min(((uint32_t) p1), ((uint32_t) p2));
    len    = (max(((uint32_t) p1), ((uint32_t) p2)) - linear) + 1;

    memset(&inregs, 0, sizeof(inregs));

    inregs.r32.eax = 0x601;
    inregs.r32.ebx = linear >> 16;
    inregs.r32.ecx = linear &  0xffff;
    inregs.r32.esi = len    >> 16;
    inregs.r32.edi = len    &  0xffff;

    dos_int386(0x31, &inregs, &outregs);

    return outregs.r32.eflag ? 0 : 1;
#else
    return 0;
#endif
}
/******************************************************************************/
