/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_fmvids.c
 *     Full Motion Video playback support.
 * @par Purpose:
 *     Implement FMV playback support for games.
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
#include "bflib_fmvids.h"

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

/******************************************************************************/

TbResult play_smk(const char *fname, ulong smkflags, ushort plyflags)
{
    TbResult ret;
#if 1
    asm volatile ("call ASM_play_smk\n"
        : "=r" (ret) : "a" (fname), "d" (smkflags), "b" (plyflags));
    return ret;
#else
    lbDisplay.LeftButton = 0;
    if ( (smack_draw_callback != NULL) || ((plyflags & SMK_PixelDoubleWidth) != 0)
        || ((plyflags & SMK_InterlaceLine) != 0) || ((plyflags & SMK_PixelDoubleLine) != 0)
        || (LbScreenIsDoubleBufferred()) )
      ret = play_smk_via_buffer(fname, smkflags, plyflags);
    else
      ret = play_smk_direct(fname, smkflags, plyflags);
#endif
    return ret;
}

/******************************************************************************/
