/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file streamfx.h
 *     Header file for streamfx.c.
 * @par Purpose:
 *     Streamed digital sound playback implementation.
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
#ifndef BFSOUNDLIB_STREAMFX_H_
#define BFSOUNDLIB_STREAMFX_H_

#include <stdint.h>
#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

//int StreamedSampleFinished();
void InitStreamedSound(void);
void FreeStreamedSound(void);
//int PlayStreamedSample();
//int MonitorStreamedSoundTrack();
//int SetStreamedSampleVolume();
//int StopStreamedSample();
void SwitchOffStreamedSound(void);
//void SwitchOnStreamedSound();
//int GetStreamedSoundPosition();
//int SetStreamedSoundPosition();
//int SwapStreamedSoundTrack();

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_STREAMFX_H_
