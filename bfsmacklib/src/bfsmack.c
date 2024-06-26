/******************************************************************************/
// Bullfrog Engine Smacker Playback Library - for use to remake classic games
// like Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfsmack.c
 *     External interface of the smacker playback library.
 * @par Purpose:
 *     Surfaces used for drawing on screen or compositing layers.
 * @par Comment:
 *     Depends on the video support library, which is SDL in this implementation.
 * @author   Tomasz Lis
 * @date     12 Nov 2011 - 13 Jun 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfsmack.h"

#include "bfscreen.h"
/******************************************************************************/
//SmackDrawCallback smack_draw_callback = NULL;
void *(*smack_malloc)(int);
void (*smack_free)(void *);

/******************************************************************************/

void set_smack_malloc(void *(*cb)(int))
{
    smack_malloc = cb;
}

void set_smack_free(void (*cb)(void *ptr))
{
    smack_free = cb;
}

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
    return ret;
}

/******************************************************************************/
