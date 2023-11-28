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

extern TbBool AILStartupAlreadyInitiated;
extern TbBool AutoScanForSoundHardware;
extern char SoundDataPath[144];
extern char SoundDriverPath[144];

extern AIL_INI SoundInstallChoice;
extern char FullDIG_INIPath[144];
extern DIG_DRIVER *SoundDriver;
extern TbBool SoundInstalled;
extern TbBool SoundAble;
extern ushort SoundType;
extern TbBool DisableLoadSounds;

extern ulong MaxNumberOfSamples;
extern TbBool StereoSound;
extern TbBool UseMultiMediaExtensions;
extern long CurrentSoundMasterVolume;
extern TbBool ive_got_an_sb16;

extern AIL_INI MusicInstallChoice;
extern char FullMDI_INIPath[144];
extern MDI_DRIVER *MusicDriver;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool DisableLoadMusic;
extern TbBool DisableDangerMusic;

extern long CurrentMusicMasterVolume;

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
#if 0
    asm volatile (
      "call ASM_InitAudio\n"
        : : "a" (audOpts));
#endif
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
    sprintf(SoundProgressMessage, "BF1  - MA   %d\n", MusicAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF2  - SA   %d\n", SoundAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF3  - CDA  %d\n", CDAble);
    SoundProgressLog(SoundProgressMessage);
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
        sprintf(SoundProgressMessage, "BF101 - cd init - disabled\n");
        SoundProgressLog(SoundProgressMessage);
        CDAble = false;
    }

    if (ive_got_an_sb16)
        prepare_SB16_volumes();

    sprintf(SoundProgressMessage, "BF54 - MA   %d\n", MusicAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF55 - SA   %d\n", SoundAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF55 - CDA  %d\n", CDAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF56 - music driver = %s\n", MusicInstallChoice.driver_name);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF57 - sound driver = %s\n", SoundInstallChoice.driver_name);
    SoundProgressLog(SoundProgressMessage);
}

void FreeAudio(void)
{
#if 0
    asm volatile ("call ASM_FreeAudio\n"
        :  :  : "eax" );
#endif
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
#if 0
    asm volatile ("call ASM_SetSoundMasterVolume\n"
        :  : "a" (vol));
#endif
    if (!SoundAble || !SoundInstalled)
        return;
    if (vol == CurrentSoundMasterVolume || vol > 127 || vol < 0)
        return;
    AIL_set_digital_master_volume(SoundDriver, vol);
    CurrentSoundMasterVolume = vol;
}

void SetMusicMasterVolume(long vol)
{
#if 0
    asm volatile ("call ASM_SetMusicMasterVolume\n"
        :  : "a" (vol));
#endif
    if (!MusicAble || !MusicInstalled)
        return;
    if (vol == CurrentMusicMasterVolume || vol > 127 || vol < 0)
        return;
    AIL_set_XMIDI_master_volume(MusicDriver, vol);
    CurrentMusicMasterVolume = vol;
}

/******************************************************************************/
