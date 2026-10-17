/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file fmvplay.h
 *     Header file for fmvplay.c.
 * @par Purpose:
 *     Wrapper of Smack player which help with Full Motion Video playback.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     13 Apr 2023 - 16 Oct 2026
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef FMVPLAY_H
#define FMVPLAY_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum MissionFMVPlay {
    MPly_Intro,
    MPly_MissiComplete,
    MPly_MissiFail,
    MPly_MPartComplete,
    MPly_GameOver,
    MPly_Outro,
};

#pragma pack()
/******************************************************************************/

void play_smacker(ushort vid_type);
void play_smacker_then_back_to_engine(ushort vid_type);
void play_smacker_then_back_to_menu(ushort vid_type);
void play_intro(void);

void smack_malloc_setup(void);
void smack_malloc_free_all(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
