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

#include "bfscreen.h"
/******************************************************************************/
//SmackDrawCallback smack_draw_callback = NULL;

TbResult play_smk_direct(const char *fname, ulong smkflags, ushort plyflags, ushort mode)
{
    TbResult ret;
    asm volatile ("call ASM_play_smk_direct\n"
        : "=r" (ret) : "a" (fname), "d" (smkflags), "b" (plyflags), "c" (mode));
    return ret;
}

TbResult play_smk_via_buffer(const char *fname, ulong smkflags, ushort plyflags, SmackDrawCallback callback)
{
    TbResult ret;
    asm volatile ("call ASM_play_smk_via_buffer\n"
        : "=r" (ret) : "a" (fname), "d" (smkflags), "b" (plyflags), "c" (callback));
    return ret;
}

TbResult play_smk(const char *fname, ulong smkflags, ushort plyflags)
{
    TbResult ret;
#if 0
    asm volatile ("call ASM_play_smk\n"
        : "=r" (ret) : "a" (fname), "d" (smkflags), "b" (plyflags));
#else
    lbDisplay.MLeftButton = 0;
    if ( (smack_draw_callback != NULL) || ((plyflags & SMK_PixelDoubleWidth) != 0)
        /*|| ((plyflags & SMK_InterlaceLine) != 0)*/ || ((plyflags & SMK_PixelDoubleLine) != 0)
        /*|| (LbScreenIsDoubleBufferred())*/ ) {
        plyflags &= ~SMK_UnknFlag100;
        ret = play_smk_via_buffer(fname, smkflags, plyflags, smack_draw_callback);
    } else {
        if (lbDisplay.GraphicsScreenHeight >= 400)
          plyflags &= ~SMK_UnknFlag100;
        ret = play_smk_direct(fname, smkflags, plyflags, lbDisplay.ScreenMode);
    }
#endif
    return ret;
}

/******************************************************************************/
