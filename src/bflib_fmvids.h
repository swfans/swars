/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_fmvids.h
 *     Header file for bflib_fmvids.c.
 * @par Purpose:
 *     Implement FMV playback support for games.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_FMVIDS_H
#define BFLIB_FMVIDS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
enum SmackerPlayFlags {
    SMK_NoStopOnUserInput  = 0x02,
    SMK_PixelDoubleLine    = 0x04,
    SMK_InterlaceLine      = 0x08,
    SMK_WriteStatusFile    = 0x40,
    SMK_PixelDoubleWidth   = 0x80,
};

typedef void (*SmackDrawCallback)(ubyte *frame_data, long width, long height);

/******************************************************************************/

TbResult play_smk(const char *fname, ulong smkflags, ushort plyflags);

TbResult play_smk_direct(const char *fname, ulong smkflags, ushort plyflags, SmackDrawCallback callback);
TbResult play_smk_via_buffer(const char *fname, ulong smkflags, ushort plyflags, ushort mode);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
