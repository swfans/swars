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
#include <string.h>
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
char MusicType[6];

void *BfMusicData = NULL;
struct BfMusicInfo *BfMusic = NULL;
struct BfMusicInfo *BfEndMusic = NULL;
int32_t music_allocated = 0;
char full_music_data_path[144];
struct MusicBankSizes music_bank_size_info;

ushort SongCurrentlyPlaying = 0;

extern TbBool AILStartupAlreadyInitiated;
extern TbBool AutoScanForSoundHardware;

extern char FullMDI_INIPath[144];
extern AIL_INI MusicInstallChoice;
extern MDI_DRIVER *MusicDriver;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;

extern long CurrentMusicMasterVolume;

extern TbBool DisableLoadMusic;

extern TbBool SoundInstalled;
extern char SoundDataPath[144];

extern SNDSEQUENCE *SongHandle;

/******************************************************************************/

TbBool fm_instrument_file_exists(const char *fname)
{
    return LbFileExists(fname);
}

sbyte AllocateMusicBankMemory(void)
{
    ulong musLen, musDataLen;

    {
        TbFileHandle fh;
        long pos;
        int len;

        len = snprintf(full_music_data_path, sizeof(full_music_data_path),
          "%s/music.dat", SoundDataPath);
        if ((len < 1) || (len > (int)sizeof(full_music_data_path))) {
            SNDLOGFAIL("Music bank", "unacceptable sound path");
        }
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

        SNDLOGSYNC("Music init", "MDMUSIC environment active");
        sscanf(envmusic, "%s %hx %hd %hd %hd", drvfile, &iop.IO, &iop.IRQ, &iop.DMA_8_bit, &iop.DMA_16_bit);
        MusicDriver = AIL_install_MDI_driver_file(drvfile, &iop);
        if (MusicDriver == NULL)
        {
            SNDLOGFAIL("Music init", "MDMUSIC environment driver installation unsuccessful");
            SNDLOGERR(" -- AIL", "%s", AIL_API_last_error());
            return -1;
        }
        SNDLOGSYNC("Music init", "MDMUSIC environment driver installation passed");
        sprintf(MusicInstallChoice.driver_name, "%s", drvfile);
        MusicInstallChoice.IO = iop;
        return 1;
    }
    SNDLOGSYNC("Music init", "MDMUSIC environment not set");
    return 0;
}

int InitMusicDriverFromMdiINI(void)
{
    if (!AIL_read_INI(&MusicInstallChoice, FullMDI_INIPath))
    {
        SNDLOGFAIL("Music init", "search for MDI.INI unsuccessful");
        SNDLOGERR(" -- AIL", "%s", AIL_API_last_error());
        return 0;
    }

    SNDLOGSYNC("Music init", "search for MDI.INI passed");

    if (strcasecmp(MusicInstallChoice.driver_name, "none") == 0)
    {
        SNDLOGSYNC("Music init", "user requests no music in SETSOUND");
        return -1;
    }

    if (AIL_install_MDI_INI(&MusicDriver) != 0) {
        SNDLOGFAIL("Music init", "MDI.INI driver installation");
        SNDLOGERR(" -- AIL", "%s", AIL_API_last_error());
        return -1;
    }
    SNDLOGSYNC("Music init", "MDI.INI driver installation passed");
    return 1;
}

int InitMusicDriverFromOS(void)
{
    SNDLOGSYNC("Music init", "generic OPL3 driver init");
    MusicDriver = AIL_open_XMIDI_driver(0);
    if (!MusicDriver)
    {
        SNDLOGFAIL("Music init", "generic OPL3 driver installation unsuccessful");
        SNDLOGERR(" -- AIL", "%s", AIL_API_last_error());
        return -1;
    }
    SNDLOGSYNC("Music init", "generic OPL3 driver installation passed");
    // Fill DOS driver information with something which makes sense
    memset(&MusicInstallChoice, 0, sizeof(MusicInstallChoice));
    sprintf(MusicInstallChoice.driver_name, "%s", "opl3.mdi");
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
        SNDLOGFAIL("Music init", "all music driver installation attempts unsuccessful");
        return -1;
    }
    return 1;
}

/** Sets MusicType based on loaded driver and its parameters.
 * "f" - FM Synthesis MIDI
 * "g" - General MIDI
 * "w" - Wave sound bank capable MIDI
 */
int DetermineMusicType(void)
{
    if (strcasecmp(MusicInstallChoice.driver_name, "adlib.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "adlibg.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "opl3.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "pas.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "pasplus.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "sblaster.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "sbpro1.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "sbpro2.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "esfm.mdi") == 0) {
        sprintf(MusicType, "f");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "sbawe32.mdi") == 0) {
        sprintf(MusicType, "w");
        MusicInstalled = 1;
        LoadAwe32Soundfont("Bullfrog");
        MusicInstalled = 0;
        if (Awe32SoundfontLoaded) {
            SNDLOGSYNC("Music init", "awe32 soundfont loaded");
        } else {
            sprintf(MusicType, "g");
            SNDLOGSYNC("Music init", "awe32 soundfont not found");
        }
    } else if (strcasecmp(MusicInstallChoice.driver_name, "mpu401.mdi") == 0) {
        sprintf(MusicType, "g");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "sndscape.mdi") == 0) {
        sprintf(MusicType, "g");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "ultra.mdi") == 0) {
        sprintf(MusicType, "g");
    } else if (strcasecmp(MusicInstallChoice.driver_name, "mt32mpu.mdi") == 0) {
        sprintf(MusicType, "r");
    } else if (!AIL_MDI_driver_type(MusicDriver)) {
        sprintf(MusicType, "g");
    } else if (AIL_MDI_driver_type(MusicDriver) == 1) {
        sprintf(MusicType, "f");
    } else if (AIL_MDI_driver_type(MusicDriver) == 2) {
        sprintf(MusicType, "f");
    } else {
        SNDLOGERR("Music init", "music driver type not supported");
        return -1;
    }
    return 1;
}

int InitMusicBanks(void)
{
    if (DisableLoadMusic) {
        SNDLOGSYNC("Music init", "LoadMusic disabled");
        return 0;
    }

    SNDLOGSYNC("Music init", "'music%s' bank allocation begin", MusicType);

    music_allocated = AllocateMusicBankMemory();
    if ((strcasecmp(MusicType, "w") == 0) && (music_allocated == -2))
    {
        SNDLOGSYNC("Music init", "'music%s' bank not present in 'music.dat'", MusicType);
        sprintf(MusicType, "g");
        music_allocated = AllocateMusicBankMemory();
    }
    if (music_allocated <= 0)
    {
        if (music_allocated == 0) {
            SNDLOGERR("Music init", "'music%s' bank cannot allocate", MusicType);
        } else if (music_allocated == -1) {
            SNDLOGERR("Music init", "'music%s' bank empty as cannot open 'music.dat'", MusicType);
        } else if (music_allocated == -2) {
            SNDLOGERR("Music init", "'music%s' bank not present in 'music.dat'", MusicType);
        }
        return -1;
    }
    SNDLOGSYNC("Music init", "'music%s' bank allocation successful", MusicType);
    return 1;
}

void InitMusic(void)
{
    char locnoext[144];
    char locfname[144];
    int len;
    int ret;

    if (!MusicAble)
        return;

    SNDLOGSYNC("Music init", "begin init");

    EnsureAILStartup();
    AIL_set_preference(MDI_SERVICE_RATE, 120);
    AIL_set_preference(MDI_SEQUENCES, 3);
    AIL_set_preference(MDI_DEFAULT_VOLUME, 127);
    AIL_set_preference(MDI_QUANT_ADVANCE, 1);
    AIL_set_preference(MDI_ALLOW_LOOP_BRANCHING, 0);
    AIL_set_preference(MDI_DEFAULT_BEND_RANGE, 12);

    if (!AutoScanForSoundHardware)
        AIL_set_preference(AIL_SCAN_FOR_HARDWARE, 0);

    len = snprintf(locnoext, sizeof(locnoext), "%s/sample", SoundDataPath);
    if ((len < 1) || (len > (int)sizeof(locnoext))) {
        SNDLOGERR("Music init", "sample check - unacceptable sound path");
    }
    AIL_set_GTL_filename_prefix(locnoext);

    len = snprintf(locfname, sizeof(locfname),"%s.ad", locnoext);
    if ((len < 1) || (len > (int)sizeof(locfname))) {
        SNDLOGERR("Music init", "`sample.ad` check - unacceptable sound path");
    }
    if (!fm_instrument_file_exists(locfname)) {
        SNDLOGSYNC("Music init", "'sample.ad' not found");
    }

    len = snprintf(locfname, sizeof(locfname), "%s.opl", locnoext);
    if ((len < 1) || (len > (int)sizeof(locfname))) {
        SNDLOGERR("Music init", "`sample.opl` check - unacceptable sound path");
    }
    if (!fm_instrument_file_exists(locfname)) {
        SNDLOGSYNC("Music init", "'sample.opl' not found");
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

    SNDLOGSYNC("Music init", "init completed");
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
