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

#include "bfsound.h"
#include "bfmemory.h"
#include "bfaudio.h"
#include "msssys.h"
#include "ssampply.h"
#include "ail.h"
#include "aildebug.h"
#include "snderr.h"
/******************************************************************************/

extern TbBool AILStartupAlreadyInitiated;
extern TbBool AutoScanForSoundHardware;

extern char FullDIG_INIPath[144];
extern AIL_INI SoundInstallChoice;
extern DIG_DRIVER *SoundDriver;
extern TbBool SoundInstalled;
extern TbBool SoundAble;
extern TbBool SoundActive;
extern ushort SoundType;

extern ulong MaxNumberOfSamples;
extern TbBool StereoSound;
extern TbBool SixteenBit;
extern ulong SampleRate;
extern long CurrentSoundMasterVolume;

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

extern TbBool DisableLoadSounds;
extern void *SfxData;
extern void *Sfx;
extern void *EndSfxs;

extern TbBool MusicInstalled;
extern TbBool MusicAble;

/******************************************************************************/
void DetermineSoundType(void)
{
    asm volatile ("call ASM_DetermineSoundType\n"
        :  :  : "eax" );
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
        EndSfxs = 0;
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

        sprintf(SoundProgressMessage, "BF8  - MDSOUND environment active\n");
        SoundProgressLog(SoundProgressMessage);
        sscanf(envsound, "%s %hx %hd %hd %hd", drvfile, &iop.IO, &iop.IRQ, &iop.DMA_8_bit, &iop.DMA_16_bit);
        SoundDriver = AIL_install_DIG_driver_file(drvfile, &iop);
        if (!SoundDriver)
        {
            sprintf(SoundProgressMessage, "BF9  - MDSOUND environment driver installation - failed\n");
            SoundProgressLog(SoundProgressMessage);
            DoFreeSound();
            return -1;
        }
        sprintf(SoundProgressMessage, "BF10 - MDSOUND environment driver installation - passed\n");
        SoundProgressLog(SoundProgressMessage);
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
        sprintf(SoundProgressMessage, "BF15 - Search for DIG.INI - failed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
        SoundProgressLog(SoundProgressMessage);
        return -1;
    }
    sprintf(SoundProgressMessage, "BF11 - Search for DIG.INI - passed   \n");
    SoundProgressLog(SoundProgressMessage);
    if (strcasecmp(SoundInstallChoice.driver_name, "none") == 0)
    {
        sprintf(SoundProgressMessage, "BF12 - user requests no sound in SETSOUND   \n");
        SoundProgressLog(SoundProgressMessage);
        DoFreeSound();
        return 0;
    }
    if (AIL_install_DIG_INI(&SoundDriver) != AIL_INIT_SUCCESS)
    {
        sprintf(SoundProgressMessage, "BF14 - DIG.INI driver installation - failed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
        SoundProgressLog(SoundProgressMessage);
        DoFreeSound();
        return -1;
    }
    sprintf(SoundProgressMessage, "BF13 - DIG.INI driver installation - passed\n");
    SoundProgressLog(SoundProgressMessage);
    return 1;
}

int InitSoundDriverFromOS(void)
{
    //TODO AIL_open_digital_driver() would make more sense here; even if it only calls the same inside
    if (AIL_install_DIG_INI(&SoundDriver) != AIL_INIT_SUCCESS)
    {
        sprintf(SoundProgressMessage, "BF14 - OS provided driver connection - failed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
        SoundProgressLog(SoundProgressMessage);
        DoFreeSound();
        return -1;
    }
    sprintf(SoundProgressMessage, "BF13 - OS provided driver connection - passed\n");
    SoundProgressLog(SoundProgressMessage);
    // Fill DOS driver information with something which makes sense
    memset(&SoundInstallChoice, 0, sizeof(SoundInstallChoice));
    sprintf(SoundInstallChoice.driver_name, "%s", "SB16.DIG");
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
#if 0
    asm volatile ("call ASM_InitSound\n"
        :  :  : "eax" );
#else
    int ret;

    if (!SoundAble)
        return;
    sprintf(SoundProgressMessage, "BF3  - Init sound\n");
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF4  - Default sound type -  %d\n", (int)SoundType);
    SoundProgressLog(SoundProgressMessage);
    if (!SoundType)
    {
        if (!MusicAble)
        {
            if (AILStartupAlreadyInitiated)
            {
                AILStartupAlreadyInitiated = false;
                AIL_shutdown();
                sprintf(SoundProgressMessage, "BF5  - No samples requested - AIL shutdown\n");
                SoundProgressLog(SoundProgressMessage);
            }
        }
        SoundAble = false;
        SoundActive = false;
        sprintf(SoundProgressMessage, "BF6  - No samples requested  \n");
        SoundProgressLog(SoundProgressMessage);
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
        sprintf(SoundProgressMessage, "BF16 - all dig driver installation attempts failed\n");
        SoundProgressLog(SoundProgressMessage);
        return;
    }
    sprintf(SoundProgressMessage, "BF18 - determine sound type to be loaded\n");
    SoundProgressLog(SoundProgressMessage);
    if (DisableLoadSounds)  {
        sprintf(SoundProgressMessage, "BF19 - LoadSounds disabled\n");
        SoundProgressLog(SoundProgressMessage);
    } else {
        DetermineSoundType();
    }

    if (!SoundType)
    {
        sprintf(SoundProgressMessage, "BF20 - cannot allocate for digital samples\n");
        SoundProgressLog(SoundProgressMessage);
        DoFreeSound();
        return;
    }

    switch (SoundType)
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
        sprintf(SoundProgressMessage, "BF19 - Unexpected SoundType - disabling sound\n");
        SoundAble = 0;
        SoundActive = 0;
        break;
    }

    if (SoundAble)
    {
        InitAllSamples();
        SoundInstalled = 1;
        sprintf(SoundProgressMessage, "BF24 - Init sound completed\n");
        SoundProgressLog(SoundProgressMessage);
        SetSoundMasterVolume(CurrentSoundMasterVolume);
    }
#endif
}

void FreeSound(void)
{
    if (!SoundInstalled)
        return;
    DoFreeSound();
    SoundInstalled = false;
}

/******************************************************************************/
