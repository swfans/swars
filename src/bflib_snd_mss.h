/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_snd_mss.h
 *     Header file for bflib_snd_mss.c.
 * @par Purpose:
 *     Sound playback support.
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
#ifndef BFLIB_SNDMSS_H
#define BFLIB_SNDMSS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#define DXDEC extern
#define AILCALL __cdecl

typedef long HTIMER;                             // Handle to timer

/******************************************************************************/
DXDEC  void    AILCALL  AIL_release_timer_handle      (HTIMER      timer);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
