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

ushort CurrentTempo = 0;
ubyte DangerMusicVolume = 0;
sbyte DangerMusicVolumeChange = -1;
sbyte CurrentDangerMusicFadeDirection = 1;
ubyte DangerMusicFadeActive = 0;
TbBool DangerMusicFadeRelease = false;
HSNDTIMER DangerMusicFadeHandle = 0;
TbBool DangerMusicAble = false;
ubyte DangerMusicVoiceMessages[16] = {0};

extern struct BfMusicInfo *BfMusic;

extern MDI_DRIVER *MusicDriver;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;

extern uint16_t SongCurrentlyPlaying;
extern SNDSEQUENCE *SongHandle;
extern short NumberOfSongs;
extern TbBool DisableDangerMusic;

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
            DangerMusicVoiceMessages[log] = 1;
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

void StartMusic(int songNo, ubyte volume)
{
    int i;

    if (!MusicInstalled || !MusicAble || !MusicActive)
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
        DangerMusicVoiceMessages[i] = 0;
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
    if (!MusicInstalled || !MusicAble || !MusicActive)
        return;

    if (SongCurrentlyPlaying == 0)
        return;

    if (DangerMusicFadeActive)
        AIL_release_timer_handle(DangerMusicFadeHandle);

    DangerMusicAble = 0;
    DangerMusicFadeActive = 0;
    CurrentDangerMusicFadeDirection = 1;
    DangerMusicVolume = 0;
    DangerMusicVolumeChange = -1;

    if (AIL_sequence_status(SongHandle) != SNDSEQ_DONE)
    {
        AIL_stop_sequence(SongHandle);
        AIL_end_sequence(SongHandle);
    }
    SongCurrentlyPlaying = 0;
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

void DangerMusicFadeTick(void *clientval)
{
    if (!MusicInstalled || !MusicAble || !MusicActive
      || !SongCurrentlyPlaying || AIL_sequence_status(SongHandle) == SNDSEQ_DONE)
    {
        AIL_release_timer_handle(DangerMusicFadeHandle);
        DangerMusicFadeActive = 0;
        CurrentDangerMusicFadeDirection = 1;
        DangerMusicVolume = 0;
        DangerMusicVolumeChange = -1;
        return;
    }

    if (DangerMusicFadeRelease)
    {
        AIL_release_timer_handle(DangerMusicFadeHandle);
        return;
    }

    if ((DangerMusicVolume == 127) && (CurrentDangerMusicFadeDirection == 2))
    {
        DangerMusicFadeRelease = 1;
        DangerMusicFadeActive = 0;
    }
    else if ((DangerMusicVolume == 0) && (CurrentDangerMusicFadeDirection == 1))
    {
        DangerMusicFadeRelease = 1;
        DangerMusicFadeActive = 0;
    }
    else if (DangerMusicFadeActive)
    {
        int i;

        DangerMusicVolume += DangerMusicVolumeChange;
        for (i = 0; i < 16; i++)
        {
            if (DangerMusicVoiceMessages[i] != 0) {
              AIL_send_channel_voice_message(MusicDriver, SongHandle,
                i | 0xB0, 11, DangerMusicVolume);
            }
        }
    }
}

void DangerMusicFadeSwitch(ubyte direction, ubyte freq)
{
    if (DisableDangerMusic || DangerMusicAble
      || MusicInstalled || MusicAble || MusicActive)
        return;
    if (SongCurrentlyPlaying
      || AIL_sequence_status(SongHandle) != SNDSEQ_DONE
      || CurrentDangerMusicFadeDirection != direction)
        return;
    if (DangerMusicFadeActive)
        AIL_release_timer_handle(DangerMusicFadeHandle);

    CurrentDangerMusicFadeDirection = direction;
    DangerMusicVolumeChange = -DangerMusicVolumeChange;
    DangerMusicFadeActive = 1;
    if (freq <= 4 && freq != 0)
    {
        DangerMusicFadeRelease = 0;
        DangerMusicFadeHandle = AIL_register_timer(DangerMusicFadeTick);
        AIL_set_timer_frequency(DangerMusicFadeHandle, 30 * freq);
        AIL_start_timer(DangerMusicFadeHandle);
    }
    else
    {
        DangerMusicFadeRelease = 0;
        DangerMusicFadeHandle = AIL_register_timer(DangerMusicFadeTick);
        AIL_set_timer_frequency(DangerMusicFadeHandle, 30);
        AIL_start_timer(DangerMusicFadeHandle);
    }
}

void SetMusicTempo(int tempo, int msec)
{
    if (!MusicInstalled || !MusicAble || !MusicActive)
        return;
    if (CurrentTempo == tempo)
        return;
    if (SongCurrentlyPlaying)
    {
        AIL_set_sequence_tempo(SongHandle, tempo, msec);
        CurrentTempo = tempo;
    }
}

void SetMusicTempoNormal(void)
{
    SetMusicTempo(100, 0);
}
/******************************************************************************/
