/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smemory.cpp
 *     Implementation of memory managing routines.
 * @par Purpose:
 *     Memory related routines - allocation and freeing of memory blocks.
 * @par Comment:
 *     Original games used different funcs for allocating low and extended mem.
 *     This is now outdated way, so functions here are simplified to originals.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfmemory.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bftypes.h"

#if defined(WIN32)

// Selected declarations frow Win32 API - we don't want to use whole API
// since it influences everything

#ifndef VOID
#define VOID void
#endif

#ifndef WINBASEAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif
#endif
#define WINAPI __stdcall

typedef unsigned long DWORD;

typedef struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORD dwTotalPhys;
    DWORD dwAvailPhys;
    DWORD dwTotalPageFile;
    DWORD dwAvailPageFile;
    DWORD dwTotalVirtual;
    DWORD dwAvailVirtual;
} MEMORYSTATUS,*LPMEMORYSTATUS;

WINBASEAPI VOID WINAPI GlobalMemoryStatus(LPMEMORYSTATUS);

#endif // defined(WIN32)

struct TbMemoryAvailable { // sizeof=20
    TbMemSize TotalBytes; // offset=0
    TbMemSize TotalBytesFree; // offset=4
    TbMemSize TotalBytesUsed; // offset=8
    TbMemSize LargestBlock; // offset=12
    TbMemSize SmallestBlock; // offset=16
};

struct TbMemoryAvailable lbMemoryAvailable;

TbBool lbMemorySetup = false;

void * LbMemorySet(void *dst, unsigned char c, TbMemSize size)
{
  return memset(dst, c, size);
}

void * LbMemoryCopy(void *in_dst, const void *in_src, TbMemSize size)
{
  return memcpy(in_dst, in_src, size);
}

void * LbMemoryMove(void *in_dst, const void *in_src, TbMemSize size)
{
  return memmove(in_dst, in_src, size);
}

void * LbMemoryAllocLow(TbMemSize size)
{
    void *ptr;
    ptr = malloc(size);
    if (ptr != NULL)
      memset(ptr, 0, size);
    return ptr;
}

void * LbMemoryAlloc(TbMemSize size)
{
    void *ptr;
    ptr = malloc(size);
    if (ptr != NULL)
      memset(ptr, 0, size);
    return ptr;
}

TbResult LbMemoryFree(void *mem_ptr)
{
    if (mem_ptr == NULL)
        return Lb_FAIL;
    free(mem_ptr);
    return Lb_SUCCESS;
}

TbResult LbMemoryCheck(void)
{
  struct _MEMORYSTATUS msbuffer;
  msbuffer.dwLength = 32;
  GlobalMemoryStatus(&msbuffer);
  lbMemoryAvailable.TotalBytes = msbuffer.dwTotalPhys;
  // We no longer care for the rest - memory usage is not tracked on
  // application level
  lbMemoryAvailable.TotalBytesFree = msbuffer.dwAvailPhys;
  lbMemoryAvailable.TotalBytesUsed = 0;
  lbMemoryAvailable.LargestBlock = msbuffer.dwAvailPhys;
  lbMemoryAvailable.SmallestBlock = 4;
  return Lb_SUCCESS;
}

TbResult LbMemorySetup(void)
{
    if (lbMemorySetup)
        return Lb_OK;
    lbMemorySetup = true;
    return Lb_SUCCESS;
}

TbResult LbMemoryReset(void)
{
    if (!lbMemorySetup)
        return Lb_OK;
    lbMemorySetup = false;
    return Lb_SUCCESS;
}

void * LbMemoryGrow(void *ptr, TbMemSize size)
{
    return realloc(ptr,size);
}

void * LbMemoryShrink(void *ptr, TbMemSize size)
{
    return realloc(ptr, size);
}

int LbMemoryCompare(void *ptr1, void *ptr2, TbMemSize size)
{
    return memcmp(ptr1, ptr2, size);
}

/******************************************************************************/
