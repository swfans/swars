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
#include "aildebug.h"
/******************************************************************************/

/** Get length of .VOC block.
 */
uint32_t AIL_VOC_block_len(void *block)
{
    return (*(uint32_t *)block) >> 8;
}

/** Terminate playback of .VOC file.
 *
 * Invoke application callback function, if any, and release the sample
 * allocated to play this file.
 */
void AIL_VOC_terminate(SNDSAMPLE *s)
{
    AILSAMPLECB cb;

    cb = (AILSAMPLECB)s->system_data[0];
    if (cb != NULL)
        cb(s);

    if (s->system_data[6] > 0)
        AIL_release_sample_handle(s);

    s->system_data[6] = -1;
}

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
