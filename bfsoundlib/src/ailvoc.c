/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ailss.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     SS functions from MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>

#include "ailss.h"
/******************************************************************************/

void AIL_process_VOC_block(SNDSAMPLE *s, int32_t play_flag)
{
    asm volatile (
      "push %1\n"
      "push %0\n"
      "call ASM_AIL_process_VOC_block\n"
      "add $0x8, %%esp\n"
        :  : "g" (s), "g" (play_flag) : "eax" );
}

/******************************************************************************/
