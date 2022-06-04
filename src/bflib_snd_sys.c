/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_snd_sys.c
 *     Sound playback support.
 * @par Purpose:
 *     Implement sound playback support for games.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_snd_sys.h"
/******************************************************************************/
void StopAllSamples(void)
{
    asm volatile ("call ASM_StopAllSamples\n"
        :  :  : "eax" );
}

TbBool IsSamplePlaying(long tng_offs, ushort smp_id, TbSampleHandle handle)
{
    TbBool ret;
    asm volatile ("call ASM_IsSamplePlaying\n"
        : "=r" (ret) : "a" (tng_offs), "d" (smp_id), "b" (handle));
    return ret;
}

/******************************************************************************/
