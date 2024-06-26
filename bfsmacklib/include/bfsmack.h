/******************************************************************************/
// Bullfrog Engine Smacker Playback Library - for use to remake classic games
// like Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfsmack.h
 *     Header file for bfsmack.c.
 * @par Purpose:
 *     External interface of the smacker playback library.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2011 - 13 Jun 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFSMACKLIB_BFSMACK_H_
#define BFSMACKLIB_BFSMACK_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

enum SmackerPlayFlags {
    SMK_NoStopOnUserInput  = 0x0002,
    SMK_PixelDoubleLine    = 0x0004,
    SMK_InterlaceLine      = 0x0008,
    SMK_WriteStatusFile    = 0x0040,
    SMK_PixelDoubleWidth   = 0x0080,
    SMK_UnknFlag100        = 0x0100,
};

typedef void (*SmackDrawCallback)(ubyte *frame_data, long width, long height);

/******************************************************************************/
extern SmackDrawCallback smack_draw_callback;

TbResult play_smk(const char *fname, ulong smkflags, ushort plyflags);

void set_smack_malloc(void *(*cb)(int));
void set_smack_free(void (*cb)(void *ptr));

/******************************************************************************/

#ifdef __cplusplus
};
#endif

#endif // BFSMACKLIB_BFSMACK_H_
/******************************************************************************/
