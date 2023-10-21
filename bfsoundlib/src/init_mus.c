/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file init_mus.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Music initialization.
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
#include <stdbool.h>
#include <assert.h>

#include "bfmusic.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bffile.h"
#include "bfaudio.h"
#include "ail.h"
#include "aildebug.h"
#include "awe32.h"
#include "snderr.h"
/******************************************************************************/

extern TbBool AILStartupAlreadyInitiated;
extern TbBool AutoScanForSoundHardware;

extern char FullMDI_INIPath[144];
extern AIL_INI MusicInstallChoice;
extern MDI_DRIVER *MusicDriver;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;
extern char MusicType[6];

extern long CurrentMusicMasterVolume;

extern TbBool DisableLoadMusic;
extern void *BfMusicData;
extern struct BfMusicInfo *BfMusic;
extern struct BfMusicInfo *BfEndMusic;
extern int32_t music_allocated;
extern char full_music_data_path[144];
extern struct MusicBankSizes music_bank_size_info;

extern TbBool SoundInstalled;
extern char SoundDataPath[144];

extern ushort SongCurrentlyPlaying;
extern SNDSEQUENCE *SongHandle;

/******************************************************************************/

TbBool fm_instrument_file_exists(const char *fname)
{
    return LbFileExists(fname);
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

static void DoFreeMusic(void)
{
    if (SongCurrentlyPlaying)
    {
        AIL_stop_sequence(SongHandle);
        AIL_end_sequence(SongHandle);
        SongCurrentlyPlaying = 0;
    }
    if (MusicDriver != NULL) {
        AIL_uninstall_MDI_driver(MusicDriver);
        MusicDriver = NULL;
    }
    if (Awe32SoundfontLoaded == 1) {
        MusicInstalled = 1;
        FreeAwe32Soundfont();
        MusicInstalled = 0;
        Awe32SoundfontLoaded = 0;
    }
    if (!SoundInstalled) {
        AILStartupAlreadyInitiated = false;
        AIL_shutdown();
    }
    if (BfMusic) {
        LbMemoryFree(BfMusic);
        BfEndMusic = 0;
    }
    if (BfMusicData)
        LbMemoryFree(BfMusicData);
    MusicAble = false;
    MusicActive = false;
}

int InitMusicDriverFromEnvMDM(void)
{
    char *envmusic;
    envmusic = getenv("MDMUSIC");
    if (envmusic != NULL)
    {
        char drvfile[24];
        struct SNDCARD_IO_PARMS iop;

        sprintf(SoundProgressMessage, "BF29 - MDMUSIC environment active\n");
        SoundProgressLog(SoundProgressMessage);
        sscanf(envmusic, "%s %hx %hd %hd %hd", drvfile, &iop.IO, &iop.IRQ, &iop.DMA_8_bit, &iop.DMA_16_bit);
        MusicDriver = AIL_install_MDI_driver_file(drvfile, &iop);
        if (MusicDriver == NULL)
        {
            sprintf(SoundProgressMessage, "BF30 - MDMUSIC environment driver installation - failed\n");
            SoundProgressLog(SoundProgressMessage);
            sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
            SoundProgressLog(SoundProgressMessage);
            return -1;
        }
        sprintf(SoundProgressMessage, "BF31 - MDMUSIC environment driver installation - passed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(MusicInstallChoice.driver_name, "%s", drvfile);
        MusicInstallChoice.IO = iop;
        return 1;
    }
    sprintf(SoundProgressMessage, "BF28 - MDMUSIC environment not set\n");
    SoundProgressLog(SoundProgressMessage);
    return 0;
}

int InitMusicDriverFromMdiINI(void)
{
    if (!AIL_read_INI(&MusicInstallChoice, FullMDI_INIPath))
    {
        sprintf(SoundProgressMessage, "BF36 - Search for MDI.INI - failed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
        SoundProgressLog(SoundProgressMessage);
        return 0;
    }

    sprintf(SoundProgressMessage, "BF32 - Search for MDI.INI - passed\n");
    SoundProgressLog(SoundProgressMessage);

    if (strcasecmp(MusicInstallChoice.driver_name, "none") == 0)
    {
        sprintf(SoundProgressMessage, "BF33 - user requests no music in SETSOUND\n");
        SoundProgressLog(SoundProgressMessage);
        return -1;
    }

    if (AIL_install_MDI_INI(&MusicDriver) != 0) {
        sprintf(SoundProgressMessage, "BF35 - MDI.INI driver installation - failed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
        SoundProgressLog(SoundProgressMessage);
        return -1;
    }
    sprintf(SoundProgressMessage, "BF34 - MDI.INI driver installation - passed\n");
    SoundProgressLog(SoundProgressMessage);
    return 1;
}

int InitMusicDriverFromOS(void)
{
    {
        sprintf(SoundProgressMessage, "BF29 - generic OPL3 driver init\n");
        SoundProgressLog(SoundProgressMessage);
        MusicDriver = AIL_open_XMIDI_driver(0);
        if (!MusicDriver)
        {
            sprintf(SoundProgressMessage, "BF30 - generic OPL3 driver installation - failed\n");
            SoundProgressLog(SoundProgressMessage);
            sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
            SoundProgressLog(SoundProgressMessage);
            return -1;
        }
        sprintf(SoundProgressMessage, "BF31 - generic OPL3 driver installation - passed\n");
        SoundProgressLog(SoundProgressMessage);
        // Fill DOS driver information with something which makes sense
        memset(&MusicInstallChoice, 0, sizeof(MusicInstallChoice));
        sprintf(MusicInstallChoice.driver_name, "%s", "OPL3.MDI");
    }
    return 1;
}

int InitMusicDriver(void)
{
    int ret = 0;

#if defined(DOS)||defined(GO32)
    if (ret != 1)
        ret = InitMusicDriverFromEnvMDM();

    if (ret != 1)
        ret = InitMusicDriverFromMdiINI();
#else
    if (ret != 1)
        ret = InitMusicDriverFromOS();
#endif

    if (ret != 1)
    {
        sprintf(SoundProgressMessage, "BF37 - all music driver installation attempts failed\n");
        SoundProgressLog(SoundProgressMessage);
        return -1;
    }
    return 1;
}

int DetermineMusicType(void)
{
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
        sprintf(SoundProgressMessage, "BF39 - music driver type not supported\n");
        SoundProgressLog(SoundProgressMessage);
        return -1;
    }
    return 1;
}

int InitMusicBanks(void)
{
    if (DisableLoadMusic) {
        sprintf(SoundProgressMessage, "BF40 - LoadMusic Disabled\n");
        SoundProgressLog(SoundProgressMessage);
        return 0;
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
        return -1;
    }
    sprintf(SoundProgressMessage, " - allocation successful\n");
    SoundProgressLog(SoundProgressMessage);
    return 1;
}

void InitMusic(void)
{
#if 0
    asm volatile ("call ASM_InitMusic\n"
        :  :  : "eax" );
#endif
    char locnoext[144];
    char locfname[144];
    int ret;

    if (!MusicAble)
        return;

    sprintf(SoundProgressMessage, "BF25 - Init Music\n");
    SoundProgressLog(SoundProgressMessage);

    EnsureAILStartup();
    AIL_set_preference(MDI_SERVICE_RATE, 120);
    AIL_set_preference(MDI_SEQUENCES, 3);
    AIL_set_preference(MDI_DEFAULT_VOLUME, 127);
    AIL_set_preference(MDI_QUANT_ADVANCE, 1);
    AIL_set_preference(MDI_ALLOW_LOOP_BRANCHING, 0);
    AIL_set_preference(MDI_DEFAULT_BEND_RANGE, 12);

    if (!AutoScanForSoundHardware)
        AIL_set_preference(AIL_SCAN_FOR_HARDWARE, 0);

    snprintf(locnoext, sizeof(locnoext), "%s/SAMPLE", SoundDataPath);
    AIL_set_GTL_filename_prefix(locnoext);

    snprintf(locfname, sizeof(locfname),"%s.ad", locnoext);
    if (!fm_instrument_file_exists(locfname)) {
        sprintf(SoundProgressMessage, "BF26 - sample.ad not found\n");
        SoundProgressLog(SoundProgressMessage);
    }

    snprintf(locfname, sizeof(locfname), "%s.opl", locnoext);
    if (!fm_instrument_file_exists(locfname)) {
        sprintf(SoundProgressMessage, "BF27 - sample.opl not found\n");
        SoundProgressLog(SoundProgressMessage);
    }

    ret = InitMusicDriver();
    if (ret == -1) {
        DoFreeMusic();
        return;
    }

    SongHandle = AIL_allocate_sequence_handle(MusicDriver);

    ret = DetermineMusicType();
    if (ret == -1) {
        DoFreeMusic();
        return;
    }

    ret = InitMusicBanks();
    if (ret == -1) {
        DoFreeMusic();
        return;
    }

    sprintf(SoundProgressMessage, "BF41 - Init music completed\n");
    SoundProgressLog(SoundProgressMessage);
    MusicInstalled = 1;
    SetMusicMasterVolume(CurrentMusicMasterVolume);
}

void FreeMusic(void)
{
    if (!MusicInstalled)
        return;
    DoFreeMusic();
    MusicInstalled = false;
}

/******************************************************************************/
