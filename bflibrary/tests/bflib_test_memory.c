/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_test_memory.c
 *     Test application for allocating and freeing memory.
 * @par Purpose:
 *     Testing implementation of bflibrary routines.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 May 2022 - 29 Jun 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfmemory.h"
#include "mock_bfwindows.h"
#include "bftstlog.h"

#include <SDL.h>

#if defined WIN32 && defined main
// Anti SDL
# undef main
#endif

/******************************************************************************/

TbBool test_memory(void)
{
    void *p;

    if (MockBaseInitialise() != Lb_SUCCESS) {
        LOGERR("bullfrog Library initialization failed");
        return false;
    }
    LbMemorySetup();

    p = LbMemoryAlloc(2048);
    LbMemoryFree(p);

    LbMemoryReset();
    LOGSYNC("passed");
    return true;
}

int main(int argc, char *argv[])
{
    if (!test_memory())
        exit(52);
    exit(0);
}

/******************************************************************************/
