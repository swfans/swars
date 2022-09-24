/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_snd_sys.c
 *     Sound playback support.
 * @par Purpose:
 *     Implement sound playback support for games.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "bflib_snd_sys.h"

#include "bfmemory.h"
#include "awe32.h"
#include "dpmi.h"
#include "snderr.h"
#include "msssys.h"
#include "sound.h"
/******************************************************************************/
extern char SoundProgressMessage[256];
extern TbBool MusicAble;
extern TbBool AILStartupAlreadyInitiated;
extern TbBool UseCurrentAwe32Soundfont;
extern TbBool MusicInstalled;
extern AudioDriverChoice MusicInstallChoice;
extern char FullMDI_INIPath[144];
extern char SoundDataPath[144];
extern char MusicType[6];
extern TbBool AutoScanForSoundHardware;
extern MDI_DRIVER *MusicDriver;
extern uint8_t *awe_buffer;
extern uint16_t awe_buffer_seg;
extern uint8_t *awe_preset;
extern uint16_t awe_preset_seg;
extern TbBool Awe32SoundfontLoaded;

void StopAllSamples(void)
{
    asm volatile ("call ASM_StopAllSamples\n"
        :  :  : "eax" );
}

TbBool IsSamplePlaying(long tng_offs, ushort smp_id, TbSampleHandle handle)
{
    TbBool ret;
    asm volatile ("call ASM_IsSamplePlaying\n"
        : "=r" (ret) : "a" (tng_offs), "d" (smp_id), "b" (handle));
    return ret;
}

void FreeAudio(void)
{
    asm volatile ("call ASM_FreeAudio\n"
        :  :  : "eax" );
}

void SetSoundMasterVolume(long vol)
{
    asm volatile ("call ASM_SetSoundMasterVolume\n"
        :  : "a" (vol));
}

void SetMusicMasterVolume(long vol)
{
    asm volatile ("call ASM_SetMusicMasterVolume\n"
        :  : "a" (vol));
}

void SetCDVolume(long vol)
{
    asm volatile ("call ASM_SetCDVolume\n"
        :  : "a" (vol));
}

void InitSound(void)
{
    asm volatile ("call ASM_InitSound\n"
        :  :  : "eax" );
}

void LoadAwe32Soundfont(const char *str)
{
    asm volatile ("call ASM_LoadAwe32Soundfont\n"
        :  :  "a" (str) );
}

void FreeAwe32Soundfont(void)
{
    if (!UseCurrentAwe32Soundfont && MusicInstalled  && MusicAble
      && !strcasecmp(MusicInstallChoice.str2, "SBAWE32.MDI")
      && !strcasecmp(MusicType, "w") )
    {
        AWEFreeMem(MusicDriver, 1);
        FreeDOSmem(awe_buffer, awe_buffer_seg);
        FreeDOSmem(awe_preset, awe_preset_seg);
        Awe32SoundfontLoaded = 0;
    }
}

/** Wrapper for LbMemoryFree(), needed due to return type.
 */
static void LbMemoryFree_wrap(void *ptr)
{
    LbMemoryFree(ptr);
}

void InitMusic(void)
{
    asm volatile ("call ASM_InitMusic\n"
        :  :  : "eax" );
}

void InitStreamedSound(void)
{
    asm volatile ("call ASM_InitStreamedSound\n"
        :  :  : "eax" );
}

void InitAllBullfrogSoundTimers(void)
{
    asm volatile ("call ASM_InitAllBullfrogSoundTimers\n"
        :  :  : "eax" );
}

void prepare_SB16_volumes(void)
{
    asm volatile ("call ASM_prepare_SB16_volumes\n"
        :  :  : "eax" );
}

/******************************************************************************/
