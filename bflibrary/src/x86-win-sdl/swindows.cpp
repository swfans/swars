/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file swindows.cpp
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
#include <unistd.h>
#include "bfwindows.h"

#if defined(WIN32)
#  include <synchapi.h>
#endif

void LbDoMultitasking(void)
{
#if defined(WIN32)
    Sleep(LB_LARGE_DELAY_TIME>>1); // This switches to other tasks
#else
    sleep(LB_LARGE_DELAY_TIME);
#endif
}

int LbWindowsControl_UNUSED()
{
// code at 0001:000b1350
}


/******************************************************************************/
