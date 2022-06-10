/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gexe_key.cpp
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
#include "bfexe_key.h"

const unsigned char exe_key[] = {
    0x89, 0x0C, 0x0D5, 0x11,
    0x00, 0x00, 0x00, 0x00,
    0xE9, 0x2A, 0x80, 0x0C5,
};

unsigned long LbExeReferenceNumber(void)
{
    return *(unsigned long *)&exe_key[4];
}


/******************************************************************************/
