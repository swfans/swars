/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sqroot.asm
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
#include "bfmath.h"

unsigned short const lbSqrTable[] = {
       1,    2,    2,    4,    5,    8,    11,    16,
      22,   32,   45,   64,   90,  128,   181,   256,
     362,  512,  724, 1024, 1448, 2048,  2896,  4096,
    5792, 8192,11585,16384,23170,32768, 46340, 65535,
};


long LbSqrL(long x)
{
  // code at 0001:00093b30
  long y;

  if (x <= 0)
    return 0;
  //_BitScanReverse(&y, x);
  asm ("bsrl     %1, %%eax;\n"
       "movl %%eax, %0;\n"
       :"=r"(y)  // output
       :"r"(x)   // input
       :"%eax"   // clobbered register
       );
  y = lbSqrTable[y];
  while ((x/y) < y)
    y = ((x/y) + y) >> 1;
  return y;
}


/******************************************************************************/
