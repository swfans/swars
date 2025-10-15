/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file init_snd.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Sound initialization.
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
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "bfsound.h"
#include "bfmemory.h"
#include "bffile.h"
#include "bfaudio.h"
#include "msssys.h"
#include "ssampply.h"
#include "ail.h"
#include "aildebug.h"
#include "snderr.h"
/******************************************************************************/

#pragma pack(1)

struct SoundBankSizes {
    long DatSize;
    long TabSize;
};

#pragma pack()
/******************************************************************************/

TbBool SixteenBit = true;
ulong SampleRate = 0;

void *Sfx;
void *EndSfxs;
void *SfxData;

long largest_dat_size = 0;
long largest_tab_size = 0;
char full_sound_data_path[224];
struct SoundBankSizes sound_bank_size_info[9];

extern TbBool AILStartupAlreadyInitiated;
extern TbBool AutoScanForSoundHardware;

extern char FullDIG_INIPath[144];
extern AIL_INI SoundInstallChoice;
extern DIG_DRIVER *SoundDriver;
extern TbBool SoundInstalled;
extern TbBool SoundAble;
extern TbBool SoundActive;
extern ushort SoundType;
extern char SoundDataPath[144];

extern ulong MaxNumberOfSamples;
extern TbBool StereoSound;
extern long CurrentSoundMasterVolume;

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

extern TbBool DisableLoadSounds;

extern TbBool MusicInstalled;
extern TbBool MusicAble;

/******************************************************************************/
ubyte GetSoundTpNo(ushort snd_type)
{
    switch (snd_type)
    {
    case 800:
        return 8;
    case 811:
        return 7;
    case 822:
        return 6;
    case 1610:
        return 5;
    case 1611:
        return 4;
    case 1620:
        return 3;
    case 1622:
        return 2;
    case 1640:
        return 1;
    case 1644:
        return 0;
    }
    return 255;
}

ushort GetSoundTypeFromTpNo(ubyte tpno)
{
    switch (tpno)
    {
    case 8:
        return 800;
    case 7:
        return 811;
    case 6:
        return 822;
    case 5:
        return 1610;
    case 4:
        return 1611;
    case 3:
        return 1620;
    case 2:
        return 1622;
    case 1:
        return 1640;
    case 0:
        return 1644;
    }
    return 0;
}

int SetSoundBitsAndRate(ushort snd_type)
{
    switch (snd_type)
    {
    case 800:
    case 811:
        SampleRate = 11025;
        SixteenBit = 0;
        break;
    case 822:
        SampleRate = 22050;
        SixteenBit = 0;
        break;
    case 1610:
    case 1611:
        SampleRate = 11025;
        SixteenBit = 1;
        break;
    case 1620:
    case 1622:
        SampleRate = 22050;
        SixteenBit = 1;
        break;
    case 1640:
    case 1644:
        SampleRate = 44100;
        SixteenBit = 1;
        break;
    case 0:
    default:
        return 0;
    }
    return 1;
}

int AllocateSoundBankMemory(ushort snd_type)
{
    TbFileHandle fh;
    long len;
    ushort tpno;
    long dat_size, tab_size;

    sprintf(full_sound_data_path, "%s/sound.dat", SoundDataPath);
    fh = LbFileOpen(full_sound_data_path, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        return -1;
    }
    len = LbFileLength(full_sound_data_path);
    memset(sound_bank_size_info, 0, 72);
    LbFileSeek(fh, len - 76, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, sound_bank_size_info, 72);
    LbFileClose(fh);

    tpno = GetSoundTpNo(SoundType);
    if (tpno < 255) {
        dat_size = sound_bank_size_info[tpno].DatSize;
        tab_size = sound_bank_size_info[tpno].TabSize;
    } else {
        tab_size = 0;
        dat_size = 0;
    }

    if ((dat_size <= 0) || (tab_size <= 0)) {
        return -2;
    }

    SfxData = LbMemoryAlloc(dat_size + 256);
    SNDLOGSYNC("Sound bank", "Sound Data size = %ld", dat_size + 256);
    Sfx = LbMemoryAlloc(tab_size + 256);

    if ((SfxData == NULL) ||  (Sfx == NULL)) {
        LbMemoryFree(SfxData);
        LbMemoryFree(Sfx);
        return 0;
    }

    largest_dat_size = dat_size;
    largest_tab_size = tab_size;
    memset(SfxData, 0, dat_size);
    memset(Sfx, 0, largest_tab_size);
    return 1;
}

void DetermineSoundType(void)
{
    int ret;
    ubyte tpno;

    while ( 1 )
    {
        SNDLOGSYNC("Sound bank", "Sound Type %d", SoundType);

        ret = AllocateSoundBankMemory(SoundType);
        if (ret > 0) {
            // Success - accept the current SoundType
            SNDLOGSYNC("Sound bank", "allocation successful");
            return;
        }
        else if (ret == -1) {
            // If bank file is missing, no need to check anything further - all SoundTypes would fail
            SNDLOGFAIL("Sound bank", "no 'sound.dat'");
            SoundType = 0;
            return;
        }
        else if (ret == -2) {
            SNDLOGERR("Sound bank", "not present in 'sound.dat'");
        }
        else if (ret == 0) {
            SNDLOGERR("Sound bank", "cannot allocate");
        }
        // Selected SoundType failed to load - try lower quality
        tpno = GetSoundTpNo(SoundType);
        if (tpno >= 255)
            break;
        SoundType = GetSoundTypeFromTpNo(tpno + 1);
        if (SoundType == 0)
            break;
    }
    SoundType = 0;
}

static void DoFreeSound(void)
{
    StopAllSamples();
    if (SoundDriver != NULL) {
        AIL_uninstall_DIG_driver(SoundDriver);
        SoundDriver = NULL;
    }
    // shutdown AIL when either we're shutting down completely after successful setup
    // or when sound install failed and music will not be tried
    if ((!MusicInstalled) ||
        (!SoundInstalled && !MusicAble)) {
        AILStartupAlreadyInitiated = false;
        AIL_shutdown();
    }
    sprintf(SoundInstallChoice.driver_name, "none");
    if (Sfx != NULL) {
        LbMemoryFree(Sfx);
        EndSfxs = NULL;
    }
    if (SfxData != NULL)
        LbMemoryFree(SfxData);
    SoundAble = false;
    SoundActive = false;
}

int InitSoundDriverFromEnvMDS(void)
{
    char *envsound;
    envsound = getenv("MDSOUND");
    if (envsound)
    {
        char drvfile[24];
        struct SNDCARD_IO_PARMS iop;

        SNDLOGSYNC("Sound init", "MDSOUND environment active");
        sscanf(envsound, "%s %hx %hd %hd %hd", drvfile, &iop.IO, &iop.IRQ, &iop.DMA_8_bit, &iop.DMA_16_bit);
        SoundDriver = AIL_install_DIG_driver_file(drvfile, &iop);
        if (!SoundDriver)
        {
            SNDLOGFAIL("Sound init", "MDSOUND environment driver installation unsuccessful");
            DoFreeSound();
            return -1;
        }
        SNDLOGSYNC("Sound init", "MDSOUND environment driver installation passed");
        memset(&SoundInstallChoice, 0, sizeof(SoundInstallChoice));
        sprintf(SoundInstallChoice.driver_name, "%s", drvfile);
        SoundInstallChoice.IO = iop;
        return 1;
    }
    return 0;
}

int InitSoundDriverFromDigINI(void)
{
    if (!AIL_read_INI(&SoundInstallChoice, FullDIG_INIPath))
    {
        sprintf(SoundInstallChoice.driver_name, "none");
        SNDLOGFAIL("Sound init", "search for DIG.INI unsuccessful");
        SNDLOGERR(" -- AIL", "%s", AIL_API_last_error());
        return -1;
    }
    SNDLOGSYNC("Sound init", "search for DIG.INI passed");
    if (strcasecmp(SoundInstallChoice.driver_name, "none") == 0)
    {
        SNDLOGSYNC("Sound init", "user requests no sound in SETSOUND");
        DoFreeSound();
        return 0;
    }
    if (AIL_install_DIG_INI(&SoundDriver) != AIL_INIT_SUCCESS)
    {
        SNDLOGFAIL("Sound init", "DIG.INI driver installation unsuccessful");
        SNDLOGERR(" -- AIL", "%s", AIL_API_last_error());
        DoFreeSound();
        return -1;
    }
    SNDLOGSYNC("Sound init", "DIG.INI driver installation passed");
    return 1;
}

int InitSoundDriverFromOS(void)
{
    //TODO AIL_open_digital_driver() would make more sense here; even if it only calls the same inside
    if (AIL_install_DIG_INI(&SoundDriver) != AIL_INIT_SUCCESS)
    {
        SNDLOGFAIL("Sound init", "OS provided driver connection unsuccessful");
        SNDLOGERR(" -- AIL", "%s", AIL_API_last_error());
        DoFreeSound();
        return -1;
    }
    SNDLOGSYNC("Sound init", "OS provided driver connection passed");
    // Fill DOS driver information with something which makes sense
    memset(&SoundInstallChoice, 0, sizeof(SoundInstallChoice));
    sprintf(SoundInstallChoice.driver_name, "%s", "sb16.dig");
    return 1;
}

void InitAllSamples(void)
{
    struct SampleInfo *smpinfo;
    end_sample_id = &sample_id[MaxNumberOfSamples - 1];
    for (smpinfo = &sample_id[0]; smpinfo <= end_sample_id; smpinfo++)
    {
        smpinfo->SampleHandle = AIL_allocate_sample_handle(SoundDriver);
        smpinfo->FadeState = 0;
        smpinfo->UserFlag = 0;
    }
}

void InitSound(void)
{
    int ret;

    if (!SoundAble)
        return;
    SNDLOGSYNC("Sound init", "start init");
    SNDLOGSYNC("Sound init", "default sound type -  %d", (int)SoundType);
    if (!SoundType)
    {
        if (!MusicAble)
        {
            if (AILStartupAlreadyInitiated)
            {
                AILStartupAlreadyInitiated = false;
                AIL_shutdown();
                SNDLOGSYNC("Sound init", "no samples requested - AIL shutdown");
            }
        }
        SoundAble = false;
        SoundActive = false;
        SNDLOGSYNC("Sound init", "no samples requested");
        return;
    }
    EnsureAILStartup();
    AIL_set_preference(DIG_ALLOW_16_BIT_DMA, 0);
    AIL_set_preference(DIG_SERVICE_RATE, 200);
    AIL_set_preference(DIG_USE_16_BITS, 1);
    AIL_set_preference(DIG_HARDWARE_SAMPLE_RATE, 22050);
    AIL_set_preference(DIG_LATENCY, 100);
    AIL_set_preference(DIG_MIXER_CHANNELS, MaxNumberOfSamples + 5);
    AIL_set_preference(DIG_DEFAULT_VOLUME, 127);
    AIL_set_preference(DIG_RESAMPLING_TOLERANCE, 655);
    AIL_set_preference(DIG_USE_STEREO, StereoSound != 0);
    if (!AutoScanForSoundHardware)
        AIL_set_preference(AIL_SCAN_FOR_HARDWARE, 0);

    ret = 0;

#if defined(DOS)||defined(GO32)
    if (ret != 1)
        ret = InitSoundDriverFromEnvMDS();

    if (ret != 1)
        ret = InitSoundDriverFromDigINI();
#else
    if (ret != 1)
        ret = InitSoundDriverFromOS();
#endif

    if (ret != 1)
    {
        SNDLOGFAIL("Sound init", "all dig driver installation attempts failed");
        return;
    }
    SNDLOGSYNC("Sound init", "determine sound type to be loaded");
    if (DisableLoadSounds)  {
        SNDLOGSYNC("Sound init", "LoadSounds disabled");
    } else {
        DetermineSoundType();
    }

    if (!SoundType)
    {
        SNDLOGFAIL("Sound init", "cannot allocate for digital samples");
        DoFreeSound();
        return;
    }

    ret = SetSoundBitsAndRate(SoundType);
    if (ret != 1)
    {
        SNDLOGFAIL("Sound init", "unexpected SoundType - disabling sound");
        SoundAble = 0;
        SoundActive = 0;
        return;
    }

    if (SoundAble)
    {
        InitAllSamples();
        SoundInstalled = 1;
        SNDLOGSYNC("Sound init", "init completed");
        SetSoundMasterVolume(CurrentSoundMasterVolume);
    }
}

void FreeSound(void)
{
    if (!SoundInstalled)
        return;
    DoFreeSound();
    SoundInstalled = false;
}

/******************************************************************************/
