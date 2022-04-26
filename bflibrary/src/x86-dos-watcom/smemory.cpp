/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smemory.cpp
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
#include "bfmemory.h"

#include "bftypes.h"

struct TbMemoryAvailable { // sizeof=20
    ULONG TotalBytes; // offset=0
    ULONG TotalBytesFree; // offset=4
    ULONG TotalBytesUsed; // offset=8
    ULONG LargestBlock; // offset=12
    ULONG SmallestBlock; // offset=16
};

struct mem_block { // sizeof=12
    UBYTE *Pointer; // offset=0
    ULONG Selector; // offset=4
    ULONG Size; // offset=8
};

struct mem_arena { // sizeof=18
    UBYTE *Pointer; // offset=0
    ULONG Size; // offset=4
    mem_arena *Child; // offset=8
    mem_arena *Parent; // offset=12
    UBYTE Used; // offset=16
    UBYTE Section; // offset=17
};

struct TbMemoryAvailable lbMemoryAvailable;

mem_block memory_blocks[256];

mem_arena memory_arenas[256];

void * LbMemoryAlloc(TbMemSize size)
{
  TbMemSize pad_size;
  struct mem_arena *marena;
  struct mem_arena *match_marena;
  TbMemSize match_size;

  LbMemorySetup();
  pad_size = (size + 3) & ~0x03u;
  match_size = -1;
  match_marena = 0;
  for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
  {
    if ( pad_size <= marena->Size
      && marena->Size < match_size
      && !marena->Used
      && !memory_blocks[marena->Section].Selector )
    {
      match_size = marena->Size;
      match_marena = marena;
    }
  }
  if ( !match_marena || !split_arena(match_marena, pad_size) )
    return LbMemoryAllocLow();
  LbMemoryCheck();
  memset(match_marena->Pointer, 0, pad_size);
  return match_marena->Pointer;
}

void * LbMemoryAllocLow(TbMemSize size)
{
  TbMemSize pad_size;
  struct mem_arena *marena;
  struct mem_arena *match_marena;
  TbMemSize match_size;

  LbMemorySetup();
  pad_size = (v1 + 0x0F) & ~0x0F;
  match_size = -1;
  match_marena = 0;
  for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
  {
    if ( pad_size <= marena->Size && marena->Size < match_size && !marena->Used )
    {
      if ( memory_blocks[marena->Section].Selector )
      {
        match_size = marena->Size;
        match_marena = marena;
      }
    }
  }
  if ( !match_marena || !split_arena(match_marena, pad_size) )
    return 0;
  LbMemoryCheck();
  memset(match_marena->Pointer, 0, pad_size);
  return match_marena->Pointer;
}

int LbMemoryCheck()
{
  struct mem_arena *marena;

  lbMemoryAvailable.TotalBytes = 0;
  lbMemoryAvailable.TotalBytesFree = 0;
  lbMemoryAvailable.TotalBytesUsed = 0;
  lbMemoryAvailable.LargestBlock = 0;
  lbMemoryAvailable.SmallestBlock = -1;
  for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
  {
    if ( marena->Used )
    {
      lbMemoryAvailable.TotalBytesUsed += marena->Size;
      lbMemoryAvailable.TotalBytes += marena->Size;
    }
    else
    {
      lbMemoryAvailable.TotalBytesFree += marena->Size;
      if ( marena->Size > lbMemoryAvailable.LargestBlock )
        lbMemoryAvailable.LargestBlock = marena->Size;
      if ( marena->Size < lbMemoryAvailable.SmallestBlock )
        lbMemoryAvailable.SmallestBlock = marena->Size;
      lbMemoryAvailable.TotalBytes += marena->Size;
    }
  }
  lbMemoryAvailable.TotalBytes &= ~0x03u;
  lbMemoryAvailable.TotalBytesFree &= ~0x03u;
  lbMemoryAvailable.TotalBytesUsed &= ~0x03u;
  lbMemoryAvailable.LargestBlock &= ~0x03u;
  lbMemoryAvailable.SmallestBlock &= ~0x03u;
  return 1;
}

TbResult LbMemoryFree(void *mem_ptr)
{
  struct mem_arena *marena;
  TbBool found;

  if ( ptr == NULL )
    return -1;
  
  found = 0;
  for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
  {
    if ( marena->Pointer == ptr )
    {
      marena->Used = 0;
      found = 1;
      break;
    }
  }
  if ( found != 1 )
    return -1;
  for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
  {
    if ( !marena->Used )
      delete_arena(marena);
  }
  LbMemoryCheck();
  return 1;
}

void * LbMemoryGrow(void *ptr, TbMemSize size)
{
  struct mem_arena *marena;
  TbBool found;

  pad_size = (size + 3) & ~0x03u;
  found = 0;
  for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
  {
      if ( marena->Pointer != ptr )
        continue;
      if ( pad_size <= marena->Size )
        return ptr;
      marena->Used = 0;
      if ( !marena->Child || marena->Child->Used )
        return NULL;
      if ( marena->Size + marena->Child->Size <= pad_size )
      {
        if ( marena->Size + marena->Child->Size < pad_size )
          return NULL;
        delete_arena(marena->Child);
      }
      else
      {
        marena->Child->Size = marena->Size + marena->Child->Size - pad_size;
        marena->Size = pad_size;
        marena->Child->Used = 1;
      }
      marena->Used = 1;
      found = 1;
      break;
  }
  if ( !found )
    return NULL;
  for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
  {
    if ( !marena->Used )
      delete_arena(marena);
  }
  LbMemoryCheck();
  return ptr;
}

TbResult LbMemoryReset(void)
{
  struct mem_block *mblock;

  for ( mblock = memory_blocks; mblock->Size; ++mblock )
  {
    if ( mblock->Selector )
      dos_free(mblock->Selector);
    else
      nfree(mblock->Pointer);
    mblock->Size = 0;
    mblock->Pointer = 0;
    mblock->Selector = 0;
  }
  return 1;
}

TbResult LbMemorySetup(void)
{
    TbMemSize sz;
    void * low_ptr;
    void * ptr;
    int i, n, nblks;

    if ( memory_blocks[0].Size )
      return 1;
    low_ptr = dos_alloc(0x10000u);
    for ( i = 0; i < 256; ++i )
      memset(&memory_blocks[i], 0, sizeof(memory_blocks[0]));
    for ( i = 0; i < 256; ++i )
      memset(&memory_arenas[i], 0, sizeof(memory_arenas[0]));

    nblks = 0;
    for ( sz = 0x0A0000; sz >= 1024; sz -= 1024 )
    {
      ptr = dos_alloc(sz);
      if ( ptr != NULL )
      {
        memory_blocks[nblks].Size = sz;
        memory_blocks[nblks].Pointer = (16 * ptr);
        memory_blocks[nblks].Selector = ptr >> 16;
        sz += 1024;
        ++nblks;
      }
    }

    for ( sz = 0x1000000; sz >= 4096; sz -= 4096 )
    {
      memory_blocks[nblks].Pointer = nmalloc(sz);
      if ( memory_blocks[nblks].Pointer )
      {
        for ( n = 4096; n > 0; n -= 16 )
        {
          if ( nexpand(memory_blocks[nblks].Pointer, n + sz) )
          {
            memory_blocks[nblks].Size = n + sz;
            memory_blocks[nblks].Selector = 0;
            break;
          }
        }
        sz += 4096;
        ++nblks;
      }
    }
    for ( n = 4096; n >= 16; n -= 16 )
    {
      memory_blocks[nblks].Pointer = (UBYTE *)nmalloc(n);
      if ( memory_blocks[nblks].Pointer )
      {
        memory_blocks[nblks].Size = n;
        memory_blocks[nblks].Selector = 0;
        n += 4096;
        ++nblks;
      }
    }

    qsort(memory_blocks, nblks, sizeof(memory_blocks[0]), compare);
    for ( i = 0; i < 256; ++i )
      memory_arenas[i].Size = 0;
    for ( i = 0; i < nblks; ++i )
    {
      memory_arenas[i].Pointer = memory_blocks[i].Pointer;
      memory_arenas[i].Size = memory_blocks[i].Size;
      if ( i )
        memory_arenas[i].Parent = &memory_arenas[i - 1];
      else
        memory_arenas[0].Parent = 0;
      memory_arenas[i].Child = &memory_arenas[i + 1];
      memory_arenas[i].Used = 0;
      memory_arenas[i].Section = i;
    }
    memory_arenas[nblks - 1].Child = NULL;
    dos_free(low_ptr >> 16);
    return 1;
}

void * LbMemoryShrink(void *ptr, TbMemSize size)
{
    struct mem_arena *marena;
    TbBool found;

    pad_size = (size + 3) & ~0x03u;
    found = 0;
    for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
    {
      if ( marena->Pointer == ptr )
      {
          if ( pad_size >= marena->Size ) {
            return ptr;
          }
          marena->Used = 0;
          split_arena(marena, pad_size);
          found = 1;
          break;
      }
    }
    if ( !found )
      return NULL;
    for ( marena = memory_arenas; marena != NULL; marena = marena->Child )
    {
      if ( !marena->Used )
        delete_arena(marena);
    }
    LbMemoryCheck();
    return ptr;
}

int LbMemoryCompare(void *ptr1, void *ptr2, TbMemSize size)
{
    return memcmp(ptr1, ptr2, size);
}

/******************************************************************************/
