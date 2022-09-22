/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfscd.h
 *     Header file for cd.c.
 * @par Purpose:
 *     Functions for CD Audio playback control.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFSOUNDLIB_BFSCD_H_
#define BFSOUNDLIB_BFSCD_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

typedef struct TbsCDTime TbsCDTime;

struct TbsCDTime { // sizeof=unknown
    int a1;
};

//void cbCDCountdown( long unsigned );
//int GetCDCountdown();
//int GetCDAble();
//short unsigned CDTrackPlaying();
//int InitRedbook();
//int PlayCDTrack();
//int PlayCDChunk();
//int PlayCDFromTrack();
//int PauseCD();
//int ResumeCD();
//int StopCD();
//int NumCDTracks();
//int CDTrackInfo();
//int CDAudioStatus();
//int SetCDVolume();
//int GetCDVolume();
//long unsigned ConvertCDTime( TbsCDTime );
//TbsCDTime ConvertDOSCDTime( long unsigned );
//int IsCDPlaying();
//TbsCDTime CDHeadPosition();
//int FreeCD();

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_BFSCD_H_
