/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gutility.c
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
#include "bfutility.h"

u32 lbSeed = 0xD15C1234;
u32 lbSeed_ps = 0xD15C1234;

ushort LbRandomAnyShort(void)
{
    lbSeed = 3141592621 * lbSeed + 0xD15C;
    return (lbSeed >> 8);
}

short LbRandomPosShort(void)
{
    lbSeed_ps *= 3141592621;
    return (lbSeed_ps >> 8) & 0x7FFF;
}

/******************************************************************************/
