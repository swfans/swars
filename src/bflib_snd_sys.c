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

#include "bfsvaribl.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bffile.h"
#include "aildebug.h"
#include "memfile.h"
#include "awe32.h"
#include "dpmi.h"
#include "snderr.h"
#include "msssys.h"
#include "sound.h"
#include "dos.h"
/******************************************************************************/
extern char SoundProgressMessage[256];
extern TbBool MusicAble;
extern TbBool AILStartupAlreadyInitiated;
extern TbBool UseCurrentAwe32Soundfont;
extern TbBool MusicActive;
extern TbBool MusicInstalled;
extern TbBool DisableLoadMusic;
extern TbBool SoundInstalled;
extern TbBool StreamedSoundAble;
extern AIL_INI MusicInstallChoice;
extern char FullMDI_INIPath[144];
extern char SoundDataPath[144];
extern TbBool SoundAble;
extern SNDSEQUENCE *SongHandle;
extern int32_t music_allocated;
extern int32_t CurrentMusicMasterVolume;
extern char MusicType[6];
extern TbBool AutoScanForSoundHardware;
extern MDI_DRIVER *MusicDriver;
extern uint8_t *awe_buffer;
extern uint16_t awe_buffer_seg;
extern uint8_t *awe_preset;
extern uint16_t awe_preset_seg;
extern TbBool Awe32SoundfontLoaded;
extern uint8_t *ssnd_buffer[2];
extern uint8_t *adpcm_source_buffer;
extern int16_t *mixer_buffer;

extern char full_music_data_path[144];
extern struct MusicBankSizes music_bank_size_info;
extern void *BfMusicData;
extern void *BfMusic;

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
      && !strcasecmp(MusicInstallChoice.driver_name, "SBAWE32.MDI")
      && !strcasecmp(MusicType, "w") )
    {
        AWEFreeMem(MusicDriver, 1);
        FreeDOSmem(awe_buffer, awe_buffer_seg);
        FreeDOSmem(awe_preset, awe_preset_seg);
        Awe32SoundfontLoaded = 0;
    }
}

sbyte AllocateMusicBankMemory(void)
{
#if 0
    TbBool ret;
    asm volatile ("call ASM_AllocateMusicBankMemory\n"
        : "=r" (ret) : );
    return ret;
#endif
    ulong musLen, musDataLen;

    {
        TbFileHandle fh;
        long pos;

        sprintf(full_music_data_path, "%s/MUSIC.DAT", SoundDataPath);
        fh = LbFileOpen(full_music_data_path, Lb_FILE_MODE_READ_ONLY);
        if (fh == INVALID_FILE)
            return -1;
        memset(&music_bank_size_info, 0, sizeof(music_bank_size_info));
        LbFileSeek(fh, 0, 2);
        pos = LbFilePosition(fh);
        LbFileSeek(fh, 0, 2);
        LbFileSeek(fh, pos - 36, 0);
        LbFileRead(fh, &music_bank_size_info, sizeof(music_bank_size_info));
        LbFileClose(fh);
    }

    musLen = 0;
    musDataLen = 0;
    switch (MusicType[0])
    {
    case 'G':
    case 'g':
    default:
        musDataLen = music_bank_size_info.mbs4;
        musLen = music_bank_size_info.mbs5;
        break;
    case 'R':
    case 'r':
        musDataLen = music_bank_size_info.mbs2;
        musLen = music_bank_size_info.mbs3;
        break;
    case 'F':
    case 'f':
        musDataLen = music_bank_size_info.mbs0;
        musLen = music_bank_size_info.mbs1;
        break;
    case 'W':
    case 'w':
        musDataLen = music_bank_size_info.mbs6;
        musLen = music_bank_size_info.mbs7;
        break;
    }

    if ((musDataLen == 0) || (musLen == 0))
        return -2;
    BfMusicData = LbMemoryAlloc(musDataLen + 256);
    BfMusic = LbMemoryAlloc(musLen + 256);
    if ((BfMusicData == NULL) || (BfMusic == NULL)) {
        LbMemoryFree(BfMusicData);
        LbMemoryFree(BfMusic);
        return 0;
    }
    LbMemorySet(BfMusicData, 0, musDataLen);
    LbMemorySet(BfMusic, 0, musLen);
    return 1;
}

TbBool fm_instrument_file_exists(const char *fname)
{
    return dos_access(fname, 0) == 0;
}

/** Wrapper for LbMemoryFree(), needed due to return type.
 */
static void LbMemoryFree_wrap(void *ptr)
{
    LbMemoryFree(ptr);
}

void InitMusic(void)
{
#if 0
    asm volatile ("call ASM_InitMusic\n"
        :  :  : "eax" );
#endif
    char locnoext[144];
    char locfname[144];
    char *envmusic;

    if (!MusicAble)
        return;

    sprintf(SoundProgressMessage, "BF25 - Init Music\n");
    SoundProgressLog(SoundProgressMessage);

    if (!AILStartupAlreadyInitiated)
    {
        AIL_MEM_use_malloc(LbMemoryAlloc);
        AIL_MEM_use_free(LbMemoryFree_wrap);
        AIL_startup();
        AILStartupAlreadyInitiated = 1;
    }
    AIL_set_preference(11, 120);
    AIL_set_preference(12, 1);
    AIL_set_preference(13, 127);
    AIL_set_preference(14, 1);
    AIL_set_preference(15, 0);
    AIL_set_preference(16, 12);

    if (!AutoScanForSoundHardware)
        AIL_set_preference(17, 0);

    sprintf(locnoext, "%s/SAMPLE", SoundDataPath);
    AIL_set_GTL_filename_prefix(locnoext);

    sprintf(locfname, "%s.ad", locnoext);
    if (!fm_instrument_file_exists(locfname)) {
        sprintf(SoundProgressMessage, "BF26 - sample.ad not found\n");
        SoundProgressLog(SoundProgressMessage);
    }

    sprintf(locfname, "%s.opl", locnoext);
    if (!fm_instrument_file_exists(locfname)) {
        sprintf(SoundProgressMessage, "BF27 - sample.opl not found\n");
        SoundProgressLog(SoundProgressMessage);
    }

    envmusic = getenv("MDMUSIC");
    if (envmusic != NULL)
    {
        char drvfile[24];
        struct SNDCARD_IO_PARMS iop;

        sprintf(SoundProgressMessage, "BF29 - MDMUSIC environment active\n");
        SoundProgressLog(SoundProgressMessage);
        sscanf(envmusic, "%s %hx %hd %hd %hd", drvfile, &iop.IO, &iop.IRQ, &iop.DMA_8_bit, &iop.DMA_16_bit);
        MusicDriver = AIL_install_MDI_driver_file(drvfile, &iop);
        if (!MusicDriver)
        {
            sprintf(SoundProgressMessage, "BF30 - MDMUSIC environment driver installation - failed\n");
            SoundProgressLog(SoundProgressMessage);
            if (!SoundAble)
                AIL_shutdown();
            MusicAble = 0;
            MusicActive = 0;
            return;
        }
        sprintf(SoundProgressMessage, "BF31 - MDMUSIC environment driver installation - passed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(MusicInstallChoice.driver_name, "%s", drvfile);
    }
    else
    {
      bool music_installed = 0;
      if (AIL_read_INI(&MusicInstallChoice, FullMDI_INIPath))
      {
        sprintf(SoundProgressMessage, "BF32 - Search for MDI.INI - passed\n");
        SoundProgressLog(SoundProgressMessage);

        if (strcasecmp(MusicInstallChoice.driver_name, "None") == 0)
        {
            sprintf(SoundProgressMessage, "BF33 - user requests no music in SETSOUND\n");
            SoundProgressLog(SoundProgressMessage);
            if (!SoundAble)
                AIL_shutdown();
            MusicAble = 0;
            MusicActive = 0;
            return;
        }

        if (AIL_install_MDI_INI(&MusicDriver)) {
            sprintf(SoundProgressMessage, "BF35 - MDI.INI driver installation - failed\n");
            SoundProgressLog(SoundProgressMessage);
        } else {
            sprintf(SoundProgressMessage, "BF34 - MDI.INI driver installation - passed\n");
            SoundProgressLog(SoundProgressMessage);
            music_installed = 1;
        }
      }
      else
      {
          sprintf(SoundProgressMessage, "BF36 - Search for MDI.INI - failed\n");
          SoundProgressLog(SoundProgressMessage);
      }

      if (!music_installed)
      {
        sprintf(SoundProgressMessage, "BF37 - all music driver installation attempts failed\n");
        SoundProgressLog(SoundProgressMessage);
        if ( !SoundAble )
          AIL_shutdown();
        MusicAble = 0;
        MusicActive = 0;
        return;
      }
    }
    SongHandle = AIL_allocate_sequence_handle(MusicDriver);

    if (strcasecmp(MusicInstallChoice.driver_name, "ADLIB.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "ADLIBG.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "OPL3.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "PAS.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "PASPLUS.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "SBLASTER.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "SBPRO1.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "SBPRO2.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "ESFM.MDI") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "SBAWE32.MDI") == 0) {
        sprintf(MusicType, "w");
        MusicInstalled = 1;
        LoadAwe32Soundfont("Bullfrog");
        MusicInstalled = 0;
        if (Awe32SoundfontLoaded) {
            sprintf(SoundProgressMessage, "BF38 - awe32 soundfont loaded\n");
            SoundProgressLog(SoundProgressMessage);
        } else {
            sprintf(MusicType, "g");
            sprintf(SoundProgressMessage, "BF38 - awe32 soundfont not found\n");
            SoundProgressLog(SoundProgressMessage);
        }
    } else if (strcasecmp(MusicInstallChoice.driver_name, "MPU401.MDI") == 0) {
        sprintf(MusicType, "g");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "SNDSCAPE.MDI") == 0) {
        sprintf(MusicType, "g");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "ULTRA.MDI") == 0) {
        sprintf(MusicType, "g");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "MT32MPU.MDI") == 0) {
        sprintf(MusicType, "r");
    } else if (!AIL_MDI_driver_type(MusicDriver)) {
        sprintf(MusicType, "g");
    } else if (AIL_MDI_driver_type(MusicDriver) == 1) {
        sprintf(MusicType, "f");
    } else if (AIL_MDI_driver_type(MusicDriver) == 2) {
        sprintf(MusicType, "f");
    } else {
        AIL_uninstall_MDI_driver(MusicDriver);
        if ( !SoundAble )
          AIL_shutdown();
        MusicAble = 0;
        MusicActive = 0;
        sprintf(SoundProgressMessage, "BF39 - music driver type not supported\n");
        SoundProgressLog(SoundProgressMessage);
    }

    if (DisableLoadMusic) {
        sprintf(SoundProgressMessage, "BF40 - LoadMusic Disabled\n");
        SoundProgressLog(SoundProgressMessage);
        goto completed;
    }

    sprintf(SoundProgressMessage, "BF40 - music%s bank allocation", MusicType);
    SoundProgressLog(SoundProgressMessage);

    music_allocated = AllocateMusicBankMemory();
    if ((strcasecmp(MusicType, "w") == 0) && (music_allocated == -2))
    {
        sprintf(SoundProgressMessage, " - not present in music.dat\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(MusicType, "g");
        sprintf(SoundProgressMessage, "BF40A - music%s bank allocation", MusicType);
        SoundProgressLog(SoundProgressMessage);
        music_allocated = AllocateMusicBankMemory();
    }
    if (music_allocated <= 0)
    {
        if (music_allocated == 0) {
            sprintf(SoundProgressMessage, " - cannot allocate\n");
            SoundProgressLog(SoundProgressMessage);
        } else if (music_allocated == -1) {
            sprintf(SoundProgressMessage, " - no music.dat\n");
            SoundProgressLog(SoundProgressMessage);
        } else if (music_allocated == -2) {
            sprintf(SoundProgressMessage, " - not present in music.dat\n");
            SoundProgressLog(SoundProgressMessage);
        }
        if (Awe32SoundfontLoaded == 1) {
            MusicInstalled = 1;
            FreeAwe32Soundfont();
            MusicInstalled = 0;
            Awe32SoundfontLoaded = 0;
        }
        AIL_uninstall_MDI_driver(MusicDriver);
        if (!SoundAble)
            AIL_shutdown();
        MusicAble = 0;
        MusicActive = 0;
        return;
    }
    sprintf(SoundProgressMessage, " - allocation successful\n");
    SoundProgressLog(SoundProgressMessage);

completed:
    sprintf(SoundProgressMessage, "BF41 - Init music completed\n");
    SoundProgressLog(SoundProgressMessage);
    MusicInstalled = 1;
    SetMusicMasterVolume(CurrentMusicMasterVolume);
}

TbBool allocate_buffers(void)
{
    mixer_buffer = LbMemoryAlloc(0x8000u);
    if (mixer_buffer == NULL)
        return false;
    adpcm_source_buffer = LbMemoryAlloc(0x800u);
    if (adpcm_source_buffer == NULL)
        return false;
    ssnd_buffer[0] = AIL_MEM_alloc_lock(0x4000);
    if (ssnd_buffer[0] == NULL)
        return false;
    ssnd_buffer[1] = AIL_MEM_alloc_lock(0x4000);
    if (ssnd_buffer[1] == NULL)
        return false;
    return true;
}

void free_buffers(void)
{
    if (ssnd_buffer[0] != NULL) {
        AIL_MEM_free_lock(ssnd_buffer[0], 0x4000);
        ssnd_buffer[0] = NULL;
    }
    if (ssnd_buffer[1] != NULL) {
        AIL_MEM_free_lock(ssnd_buffer[1], 0x4000);
        ssnd_buffer[1] = NULL;
    }
    if (adpcm_source_buffer != NULL) {
        LbMemoryFree(adpcm_source_buffer);
        adpcm_source_buffer = NULL;
    }
    if (mixer_buffer != NULL) {
        LbMemoryFree(mixer_buffer);
        mixer_buffer = NULL;
    }
}

void InitStreamedSound(void)
{
#if 1
    asm volatile ("call ASM_InitStreamedSound\n"
        :  :  : "eax" );
#else
    if (!SoundInstalled || !SoundAble || StreamedSoundAble)
        return;

    flushall();
    setbuf(stdout, NULL);

    if (!allocate_buffers())
    {
        free_buffers();
        sprintf(SoundProgressMessage,
            "BF100 - Cannot allocate buffers for streamed sound\n");
        SoundProgressLog(SoundProgressMessage);
        return;
    }

    sample_handle = AIL_allocate_sample_handle(SoundDriver);
    if (!sample_handle)
    {
        free_buffers();
        sprintf(SoundProgressMessage,
            "BF100 - Cannot allocate handle for streamed sound\n");
        SoundProgressLog(SoundProgressMessage);
        return;
    }
    StreamedSoundAble = 1;
#endif
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
