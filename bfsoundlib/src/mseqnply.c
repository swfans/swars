/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssamplst.c
 *     Music Sequence playback routines.
 * @par Purpose:
 *     Functions for playing/stopping music sequences.
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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "bfmusic.h"
#include "aildebug.h"
#include "mssal.h"
/******************************************************************************/

extern MDI_DRIVER *MusicDriver;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;

extern u16 SongCurrentlyPlaying;
extern SNDSEQUENCE *SongHandle;
extern u16 CurrentTempo;
extern i16 NumberOfSongs;

extern u8 DangerMusicVolume;
extern i8 DangerMusicVolumeChange; // = -1;
extern i8 CurrentDangerMusicFadeDirection;
extern u8 DangerMusicFadeActive;
extern HSNDTIMER DangerMusicFadeHandle;
extern u8 DangerMusicAble;
extern TbBool DisableDangerMusic;

extern struct BfMusicInfo *BfMusic;
extern u8 data_1e5edc[16];

/******************************************************************************/

void cb_get_trigger_info(SNDSEQUENCE *seq, int32_t log, int32_t data)
{
    if (data == 0)
    {
        DangerMusicAble = 1;
        if (DisableDangerMusic) {
            AIL_send_channel_voice_message(MusicDriver, SongHandle, log | MDI_EV_CONTROL,
                MDI_CTR_CHAN_MUTE, 127);
            DangerMusicAble = 0;
        } else {
            AIL_send_channel_voice_message(MusicDriver, SongHandle, log | MDI_EV_CONTROL,
                MDI_CTR_EXPRESSION, 0);
            data_1e5edc[log] = 1;
        }
    }
    if (data == 1)
    {
        AIL_stop_sequence(SongHandle);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, log | MDI_EV_CONTROL,
            MDI_CTR_GM_BANK_MSB, 1);
        AIL_resume_sequence(SongHandle);
    }
}

void StartMusic(int songNo, u8 volume)
{
    int i;

    if (!MusicInstalled || !MusicAble)
        return;
    if (!MusicActive)
        return;
    if (songNo > NumberOfSongs) {
        return;
    }
    if (SongCurrentlyPlaying == songNo)
        return;

    if (DangerMusicFadeActive)
        AIL_release_timer_handle(DangerMusicFadeHandle);

    DangerMusicAble = 0;
    DangerMusicFadeActive = 0;
    CurrentDangerMusicFadeDirection = 1;
    DangerMusicVolume = 0;
    DangerMusicVolumeChange = -1;

    if (SongCurrentlyPlaying != 0)
    {
        if (AIL_sequence_status(SongHandle) != SNDSEQ_DONE) {
            AIL_stop_sequence(SongHandle);
            AIL_end_sequence(SongHandle);
        }
        SongCurrentlyPlaying = 0;
    }

    AIL_init_sequence(SongHandle, BfMusic[songNo].DataBeg, 0);
    AIL_register_trigger_callback(SongHandle, cb_get_trigger_info);

    for (i = 0; i < 16; i++)
    {
        data_1e5edc[i] = 0;
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_CONTROL,
            MDI_CTR_GM_BANK_MSB, 0);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_CONTROL,
            MDI_CTR_PART_VOLUME, 0);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_PITCH,
            0, 64);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_CONTROL,
            MDI_CTR_EXPRESSION, 127);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_CONTROL,
            MDI_CTR_MODULATION, 0);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_CONTROL,
            MDI_CTR_REVERB, 0);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_CONTROL,
            MDI_CTR_CHORUS, 0);
        AIL_send_channel_voice_message(MusicDriver, SongHandle, i | MDI_EV_CONTROL,
            MDI_CTR_CHAN_MUTE, 0);
    }

    if (volume < 127) {
        AIL_set_sequence_volume(SongHandle, volume, 0);
    }
    AIL_start_sequence(SongHandle);
    SongCurrentlyPlaying = songNo;
    CurrentTempo = 100;
}

void StopMusic(void)
{
    asm volatile ("call ASM_StopMusic\n"
        :  :  : "eax" );
}

void StopMusicIfActive(void)
{
    if (!MusicInstalled || !MusicAble)
        return;
    if (MusicActive)
    {
        StopMusic();
        MusicActive = 0;
    }
}

/******************************************************************************/
