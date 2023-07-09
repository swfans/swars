/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_snd_cda.h
 *     Header file for bflib_snd_cda.c.
 * @par Purpose:
 *     CD Audio playback support.
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
#ifndef BFLIB_SNDCDA_H
#define BFLIB_SNDCDA_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
enum CDDeviceTypes {
    CDTYP_NONE,
    CDTYP_REAL,
    CDTYP_OGG,
};

/******************************************************************************/
TbBool GetCDAble(void);
void InitRedbook(void);
void InitMusicOGG(void);
void PlayCDTrack(ushort trkno);
void SetCDVolume(short vol);
void StopCD(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
