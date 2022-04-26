/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssemphr.cpp
 *     Semaphores wrapper.
 * @par Purpose:
 *     Creates unified interface for semaphores, which are used for
 *      sharing access to resources on multiple threads.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     21 May 2009 - 20 Jul 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfsemphr.hpp"

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#if defined(WIN32)

// Selected declarations frow Win32 API - we don't want to use whole API
// since it influences everything

#ifndef WINBASEAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif
#endif
#define WINAPI __stdcall
#ifndef CONST
#define CONST const
#endif

typedef unsigned long DWORD;
typedef long LONG;
typedef char CHAR;
typedef int WINBOOL,*PWINBOOL,*LPWINBOOL;
typedef CONST CHAR *LPCCH,*PCSTR,*LPCSTR;
typedef void *PVOID,*LPVOID;
typedef PVOID HANDLE;
#define BOOL WINBOOL
#define DECLARE_HANDLE(n) typedef HANDLE n
typedef HANDLE *PHANDLE,*LPHANDLE;
typedef long *LPLONG;
typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES,*PSECURITY_ATTRIBUTES,*LPSECURITY_ATTRIBUTES;
#define INFINITE    0xFFFFFFFF

WINBASEAPI HANDLE WINAPI CreateSemaphoreA(LPSECURITY_ATTRIBUTES,LONG,LONG,LPCSTR);
WINBASEAPI BOOL WINAPI ReleaseSemaphore(HANDLE,LONG,LPLONG);
WINBASEAPI DWORD WINAPI WaitForSingleObject(HANDLE,DWORD);
WINBASEAPI BOOL WINAPI CloseHandle(HANDLE);

#endif // defined(WIN32)
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/

LbSemaphore::LbSemaphore(void)
{
  this->pHandle = CreateSemaphoreA(NULL, 1, 1, NULL);
}

LbSemaphore::~LbSemaphore(void)
{
  CloseHandle(this->pHandle);
}

/******************************************************************************/

LbSemaLock::LbSemaLock(class LbSemaphore *sem, bool isLocked)
{
  this->pHandle = sem->pHandle;
  this->locked = isLocked;
  this->invalid = 0;
}

LbSemaLock::~LbSemaLock(void)
{
  Release();
}

void LbSemaLock::Release(void)
{
  if ( this->locked )
  {
    if ( !this->invalid )
    {
      ReleaseSemaphore(this->pHandle, 1, 0);
      this->locked = 0;
    }
  }
}

int LbSemaLock::Lock(bool wait_forever)
{
  DWORD wait_retcode;
  if (wait_forever)
  {
    wait_retcode = WaitForSingleObject(this->pHandle, INFINITE);
    this->locked = wait_retcode < 1;
  } else
  {
    wait_retcode = WaitForSingleObject(this->pHandle, 5);
    this->locked = wait_retcode < 1;
  }
  return this->locked;
}

/******************************************************************************/
