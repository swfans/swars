/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_memory.c
 *     Memory managing routines.
 * @par Purpose:
 *     Memory related routines - allocation and freeing of memory blocks.
 * @par Comment:
 *     Original SW used different functions for allocating low and extended memory.
 *     This is now outdated way, so functions here are simplified to originals.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_memory.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

/******************************************************************************/
#define AVAILABLE_MEMORY (16*1024*1024)
#define TABLE_SIZE 256 /* hardcoded, do not change */

TbBool lbMemorySetup = false;
char lbEmptyString[] = "";
/******************************************************************************/

#pragma pack(1)

struct mem_block
{
    void    *Pointer;
    uint32_t Selector;
    uint32_t Size;
};

typedef struct mem_block mem_block;

typedef struct mem_arena mem_arena;

struct mem_arena
{
    void    *Pointer;
    uint32_t Size;
    mem_arena *Child;
    mem_arena *Parent;
    uint8_t  Used;
    uint8_t  Section;
};

typedef struct {
    ulong TotalBytes;
    ulong TotalBytesFree;
    ulong TotalBytesUsed;
    ulong LargestBlock;
    ulong SmallestBlock;
} TbMemoryAvailable;

extern mem_block memory_blocks[TABLE_SIZE];
extern mem_arena memory_arenas[TABLE_SIZE];
extern TbMemoryAvailable lbMemoryAvailable;

#pragma pack()

static size_t get_block_count(void)
{
    size_t n;

    for (n = 0; n < TABLE_SIZE; n++)
    {
        if (memory_blocks[n].Pointer == NULL)
            return n;
    }

    return TABLE_SIZE;
}

static void initialise_block_nodes(void)
{
    size_t n;
    size_t count;

    count = get_block_count();

    for (n = 0; n < count; n++)
      {
        memory_arenas[n].Pointer  = memory_blocks[n].Pointer;
        memory_arenas[n].Size = memory_blocks[n].Size;
        memory_arenas[n].Section = n;

        if (n != 0)
            memory_arenas[n].Parent = &memory_arenas[n - 1];
        else
            memory_arenas[n].Parent = NULL;

        if (n + 1 != count)
            memory_arenas[n].Child = &memory_arenas[n + 1];
        else
            memory_arenas[n].Child = NULL;
      }
}

TbResult split_arena(mem_arena *arena, size_t size)
{
    mem_arena *curarena;
    uint8_t sect;
    mem_arena *charena;
    uint32_t n;

    if (size == arena->Size) {
        arena->Used = 1;
        return 1;
    }

    for (n = 0; n < TABLE_SIZE; n++)
    {
        curarena = &memory_arenas[n];
        if (curarena->Size == 0)
          break;
    }
    if (n == TABLE_SIZE) {
        return 0;
    }
    curarena->Size = arena->Size - size;
    curarena->Pointer = (char *)arena->Pointer + size;
    arena->Used = 1;
    sect = arena->Section;
    arena->Size = size;
    curarena->Section = sect;
    curarena->Child = arena->Child;
    curarena->Parent = arena;
    charena = arena->Child;
    if (charena != NULL)
        charena->Parent = curarena;
    arena->Child = curarena;
    return 1;
}

TbResult LbMemorySetup(void)
{
    if (lbMemorySetup)
        return true;

    memset(&memory_blocks, 0, sizeof (memory_blocks));
    memset(&memory_arenas, 0, sizeof (memory_arenas));

    memory_blocks[0].Pointer = malloc(AVAILABLE_MEMORY);
    memory_blocks[0].Size    = AVAILABLE_MEMORY;

    assert (memory_blocks[0].Pointer != NULL);

    initialise_block_nodes();

    lbMemorySetup = true;
    return true;
}

void * LbMemorySet(void *dst, uchar c, ulong length)
{
    return memset(dst, c, length);
}

void * LbMemoryCopy(void *in_dst, const void *in_src, ulong len)
{
    return memcpy(in_dst,in_src,len);
}

/**
 *
 * Appends characters of source to destination, plus a terminating null-character.
 * Prevents string in dst of getting bigger than maxlen characters.
 */
void * LbStringConcat(char *dst, const char *src, const ulong dst_buflen)
{
    int max_num=dst_buflen-strlen(dst);
    if (max_num<=0) return dst;
    strncat(dst, src, max_num);
    dst[dst_buflen-1]='\0';
    return dst;
}

void * LbStringCopy(char *dst, const char *src, const ulong dst_buflen)
{
    if (dst_buflen < 1)
        return dst;
    strncpy(dst, src, dst_buflen);
    dst[dst_buflen-1]='\0';
    return dst;
}

void * LbStringToLowerCopy(char *dst, const char *src, const ulong dst_buflen)
{
    ulong i;
    char chr;
    if (dst_buflen < 1)
        return dst;
    for (i=0; i < dst_buflen; i++)
    {
        chr = tolower(src[i]);
        dst[i] = chr;
        if (chr == '\0')
            break;
    }
    dst[dst_buflen-1]='\0';
    return dst;
}

ulong LbStringLength(const char *str)
{
    if (str == NULL) return 0;
    return strlen(str);
}

void LbMemRegister_Setup(void)
{
    /*memset(lbMemList, 0, 0x800);
    lbMemAllocation = 0;
    lbMemCount = 0;*/
}

int LbMemoryReset(void)
{
    if (lbMemorySetup == 0)
        return 0;
    lbMemorySetup = 0;
    /* Heap handling by application is only required for some platforms
    if (heap_handle != NULL)
    {
        if (!HeapDestroy(heap_handle))
            return -1;
        heap_handle = NULL;
    }
    LbMemRegister_Setup();
    */
    return 1;
}

unsigned char * LbMemoryAllocLow(ulong size)
{
    mem_arena *curarena;
    unsigned long last_size;
    mem_arena *splarena;
    unsigned long algn_size;
    int sect;

    LbMemorySetup();
    algn_size = (size + 0x0f) & ~0x0f;
    last_size = -1;
    splarena = NULL;
    for (curarena = memory_arenas; curarena != NULL; curarena = curarena->Child)
    {
        if (algn_size <= curarena->Size && last_size > curarena->Size)
        {
            if ( !curarena->Used )
            {
                sect = curarena->Section;
                if ( memory_blocks[sect].Selector )
                {
                  splarena = curarena;
                  last_size = curarena->Size;
                }
            }
        }
    }
    if (splarena == NULL || !split_arena(splarena, algn_size)) {
        ERRORLOG("Memory allocation failed");
        return 0;
    }
    LbMemoryCheck();
    memset(splarena->Pointer, 0, algn_size);
    return splarena->Pointer;
}

unsigned char * LbMemoryAlloc(ulong size)
{
    mem_arena *curarena;
    unsigned long last_size;
    mem_arena *splarena;
    unsigned long algn_size;
    int sect;

    LbMemorySetup();
    algn_size = (size + 0x03) & ~0x03;
    last_size = -1;
    splarena = NULL;
    for (curarena = memory_arenas; curarena != NULL; curarena = curarena->Child)
    {
        if (algn_size <= curarena->Size && last_size > curarena->Size)
        {
            if ( !curarena->Used )
            {
                sect = curarena->Section;
                if ( !memory_blocks[sect].Selector )
                {
                    splarena = curarena;
                    last_size = curarena->Size;
                }
            }
        }
    }
    if (splarena == NULL || !split_arena(splarena, algn_size)) {
        return LbMemoryAllocLow(algn_size);
    }
    LbMemoryCheck();
    memset(splarena->Pointer, 0, algn_size);
    return splarena->Pointer;
}

int LbMemoryFree(void *mem_ptr)
{
    if (mem_ptr==NULL) return -1;
    free(mem_ptr);
    return 1;
}

TbResult LbMemoryCheck(void)
{
    mem_arena *arena;
    unsigned int csize;

    lbMemoryAvailable.TotalBytes = 0;
    lbMemoryAvailable.TotalBytesFree = 0;
    lbMemoryAvailable.TotalBytesUsed = 0;
    lbMemoryAvailable.LargestBlock = 0;
    lbMemoryAvailable.SmallestBlock = -1;
    for (arena = memory_arenas; arena != NULL; arena = arena->Child)
    {
        if (arena->Used) {
            csize = arena->Size;
            lbMemoryAvailable.TotalBytesUsed += csize;
            lbMemoryAvailable.TotalBytes += csize;
        } else {
            csize = arena->Size;
            lbMemoryAvailable.TotalBytesFree += csize;
            if (csize > lbMemoryAvailable.LargestBlock)
                lbMemoryAvailable.LargestBlock = csize;
            if (arena->Size < lbMemoryAvailable.SmallestBlock)
                lbMemoryAvailable.SmallestBlock = arena->Size;
            lbMemoryAvailable.TotalBytes += arena->Size;
        }
    }
    lbMemoryAvailable.TotalBytes &= ~0x03;
    lbMemoryAvailable.TotalBytesFree &= ~0x03;
    lbMemoryAvailable.LargestBlock &= ~0x03;
    lbMemoryAvailable.TotalBytesUsed &= ~0x03;
    lbMemoryAvailable.SmallestBlock &= ~0x03;
    return 1;
}

/** Enlarge previously allocated memory block.
 *  The size of the memory block pointed to by the ptr parameter is
 *  changed to the size bytes, expanding the amount of memory available
 *  in the block. A pointer to the reallocated memory block is returned,
 *  which may be either the same as the ptr argument or a new location.
 *  If the function failed to allocate the requested block of memory,
 *  a NULL pointer is returned.
 *
 * @param ptr The previously allocated memory block.
 * @param size New size of the block.
 */
TbResult LbMemoryGrow(void **ptr, unsigned long size)
{
    void *outptr;
    outptr = realloc(*ptr,size);
    if (outptr == NULL)
        return 0;
    *ptr = outptr;
    return 1;
}

/** Reduce previously allocated memory block.
 *  The size of the memory block pointed to by the ptr parameter is
 *  changed to the size bytes, reducing the amount of memory available
 *  in the block. A pointer to the reallocated memory block is returned,
 *  which usually is the same as the ptr argument.
 *
 * @param ptr The previously allocated memory block.
 * @param size New size of the block.
 */
TbResult LbMemoryShrink(void **ptr, unsigned long size)
{
    void *outptr;
    outptr = realloc(*ptr,size);
    if (outptr == NULL)
        return 0;
    *ptr = outptr;
    return 1;
}

/** Reduce previously allocated memory block.
 *  The size of the memory block pointed to by the ptr parameter is
 *  changed to the size bytes, reducing the amount of memory available
 *  in the block. A pointer to the reallocated memory block is returned,
 *  which usually is the same as the ptr argument.
 *
 * @param ptr The previously allocated memory block.
 * @param size New size of the block.
 */
int LbMemoryCompare(void *ptr1, void *ptr2, unsigned long size)
{
    return memcmp(ptr1,ptr2,size);
}

/******************************************************************************/
