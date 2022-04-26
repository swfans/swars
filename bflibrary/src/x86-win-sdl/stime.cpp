/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file stime.cpp
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
#include "bftime.h"
#include <time.h>

int LbDate()
{
// code at 0001:000b0f50
}

int LbTime()
{
// code at 0001:000b0f90
}

/**
 * Returns the number of milliseconds elapsed since the program was launched.
 */
TbClockMSec LbTimerClock(void)
{
  // code at 0001:000b0fd0
#if CLOCKS_PER_SEC >= 10000
  return clock() / (CLOCKS_PER_SEC / 1000);
#elif CLOCKS_PER_SEC > 1000
  return ((TbClockMSec)clock() << 3) / (CLOCKS_PER_SEC / 125);
#elif CLOCKS_PER_SEC == 1000
  return clock();
#elif CLOCKS_PER_SEC >= 500
  return ((TbClockMSec)clock() * (8000 / CLOCKS_PER_SEC)) >> 3;
#else
  return (TbClockMSec)clock() * (1000 / CLOCKS_PER_SEC);
#endif
}

/******************************************************************************/
