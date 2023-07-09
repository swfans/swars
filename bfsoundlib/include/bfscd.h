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

enum CDDeviceTypes {
    CDTYP_NONE,
    CDTYP_REAL,
    CDTYP_OGG,
};

typedef struct TbsCDTime TbsCDTime;

struct TbsCDTime { // sizeof=unknown
    int a1;
};

//void cbCDCountdown( long unsigned );
//int GetCDCountdown();
TbBool GetCDAble(void);
//ushort CDTrackPlaying();
void InitRedbook(void);
void InitMusicOGG(const char *nmusic_dir);
void PlayCDTrack(ushort trkno);
//int PlayCDChunk();
//int PlayCDFromTrack();
void PauseCD(void);
void ResumeCD(void);
void StopCD(void);
//int NumCDTracks();
//int CDTrackInfo();
//int CDAudioStatus();
void SetCDVolume(short vol);
sbyte GetCDVolume(void);
//long unsigned ConvertCDTime( TbsCDTime );
//TbsCDTime ConvertDOSCDTime( long unsigned );
//int IsCDPlaying();
//TbsCDTime CDHeadPosition();
void FreeCD(void);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_BFSCD_H_
