/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfaudio.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Set of general audio-related calls.
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
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "bfaudio.h"
#include "bfmemory.h"
#include "bffile.h"
#include "bfwindows.h"
#include "bfsound.h"
#include "bfmusic.h"
#include "bfscd.h"
#include "streamfx.h"
#include "aila.h"
#include "aildebug.h"
#include "msssys.h"
#include "sndtimer.h"
#include "snderr.h"
#include "sb16.h"
#include "awe32.h"
#include "oggvorbis.h"
/******************************************************************************/

TbBool AILStartupAlreadyInitiated = false;
TbBool AutoScanForSoundHardware = true;
char SoundDataPath[144];
char SoundDriverPath[144];

AIL_INI SoundInstallChoice;
char FullDIG_INIPath[144];
TbBool SoundInstalled = false;
TbBool SoundAble = true;
ushort SoundType = 0;
TbBool DisableLoadSounds = false;

ulong MaxNumberOfSamples = 10;
TbBool StereoSound = true;
TbBool UseMultiMediaExtensions = false;
long CurrentSoundMasterVolume = 127;
TbBool ive_got_an_sb16 = false;

AIL_INI MusicInstallChoice;
char FullMDI_INIPath[144];
TbBool MusicAble = true;
TbBool DisableLoadMusic = false;
TbBool DisableDangerMusic = false;

long CurrentMusicMasterVolume = 127;

extern DIG_DRIVER *SoundDriver;
extern MDI_DRIVER *MusicDriver;
extern TbBool MusicInstalled;
extern TbBool CDAble;
extern TbBool StreamedSoundAble;

/******************************************************************************/

/** Wrapper for LbMemoryAlloc(), needed to make sure data sizes match.
 */
void *LbMemoryAlloc_wrap(uint32_t size)
{
    return LbMemoryAlloc(size);
}

/** Wrapper for LbMemoryFree(), needed to make sure data sizes match.
 */
void LbMemoryFree_wrap(void *ptr)
{
    LbMemoryFree(ptr);
}

void EnsureAILStartup(void)
{
    if (!AILStartupAlreadyInitiated)
    {
        AIL_MEM_use_malloc(LbMemoryAlloc_wrap);
        AIL_MEM_use_free(LbMemoryFree_wrap);
        AIL_startup();
        AILStartupAlreadyInitiated = 1;
    }
}

TbBool sound_update(void)
{
    if (AILStartupAlreadyInitiated)
        AIL_API_timer();
    return true;
}

int32_t sound_fake_timer_initialize(void)
{
    if (LbRegisterIdleHandler(sound_update) != Lb_SUCCESS)
        return 0;
    return 1;
}

void InitAudio(AudioInitOptions *audOpts)
{
    InitDebugAudio();

    sprintf(FullDIG_INIPath, "sound/DIG.INI");
    sprintf(FullMDI_INIPath, "sound/MDI.INI");
    strcpy(SoundDataPath, "sound");
    strcpy(SoundDriverPath, "sound");
    if (audOpts->IniPath) {
        sprintf(FullDIG_INIPath, "%s/DIG.INI", audOpts->IniPath);
        sprintf(FullMDI_INIPath, "%s/MDI.INI", audOpts->IniPath);
    }
    if (audOpts->SoundDataPath != NULL) {
        strcpy(SoundDataPath, audOpts->SoundDataPath);
    }
    if (audOpts->SoundDriverPath != NULL) {
        strcpy(SoundDriverPath, audOpts->SoundDriverPath);
    }

    MaxNumberOfSamples = audOpts->MaxSamples;
    SoundType = audOpts->SoundType;
    if (!audOpts->AutoScan)
        AutoScanForSoundHardware = 0;
    if (!audOpts->StereoOption)
        StereoSound = 0;
    if (audOpts->DisableDangerMusic == 1)
        DisableDangerMusic = 1;
    if (audOpts->DisableLoadSounds == 1)
        DisableLoadSounds = 1;
    if (audOpts->DisableLoadMusic == 1)
        DisableLoadMusic = 1;
    if (audOpts->UseCurrentAwe32Soundfont == 1)
        UseCurrentAwe32Soundfont = 1;
    if (audOpts->UseMultiMediaExtensions == 1)
        UseMultiMediaExtensions = 1;
    SNDLOGSYNC("Init audio", "initial MA   %d", MusicAble);
    SNDLOGSYNC("Init audio", "initial SA   %d", SoundAble);
    SNDLOGSYNC("Init audio", "initial CDA  %d", CDAble);
    if (!(audOpts->AbleFlags & 0x01))
        MusicAble = 0;
    if (!(audOpts->AbleFlags & 0x02))
        SoundAble = 0;

    InitSound();
    InitMusic();
    InitAllBullfrogSoundTimers();

    if (audOpts->InitStreamedSound == 1)
        InitStreamedSound();

    if (audOpts->InitRedbookAudio == 1) {
        InitRedbook();
    } else if (audOpts->InitRedbookAudio == 2) {
        InitMusicOGG("music");
    } else {
        SNDLOGSYNC("Init audio", "cd init - disabled");
        CDAble = false;
    }

    if (ive_got_an_sb16)
        prepare_SB16_volumes();

    SNDLOGSYNC("Init audio", "MA   %d", MusicAble);
    SNDLOGSYNC("Init audio", "SA   %d", SoundAble);
    SNDLOGSYNC("Init audio", "CDA  %d", CDAble);
    SNDLOGSYNC("Init audio", "music driver = %s", MusicInstallChoice.driver_name);
    SNDLOGSYNC("Init audio", "sound driver = %s", SoundInstallChoice.driver_name);
}

void FreeAudio(void)
{
    if (GetCDAble()) {
        FreeCD();
        if (!SoundAble && !MusicAble)
            AIL_shutdown();
    }
    if (StreamedSoundAble)
        FreeStreamedSound();
    FreeMusic();
    FreeSound();
    if (sb16_mixer_set)
        reset_SB16_volumes();
}

void SetSoundMasterVolume(long vol)
{
    if (!SoundAble || !SoundInstalled)
        return;
    if (vol == CurrentSoundMasterVolume || vol > 127 || vol < 0)
        return;
    AIL_set_digital_master_volume(SoundDriver, vol);
    CurrentSoundMasterVolume = vol;
}

void SetMusicMasterVolume(long vol)
{
    if (!MusicAble || !MusicInstalled)
        return;
    if (vol == CurrentMusicMasterVolume || vol > 127 || vol < 0)
        return;
    AIL_set_XMIDI_master_volume(MusicDriver, vol);
    CurrentMusicMasterVolume = vol;
}

/******************************************************************************/
