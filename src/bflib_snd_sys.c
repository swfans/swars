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
#include "ail.h"
#include "aildebug.h"
#include "memfile.h"
#include "awe32.h"
#include "dpmi.h"
#include "snderr.h"
#include "msssys.h"
#include "sound.h"
#include "dos.h"
/******************************************************************************/
extern ulong MaxNumberOfSamples;
extern TbBool StereoSound;
extern TbBool SixteenBit;
extern ulong SampleRate;
extern char SoundProgressMessage[256];
extern long CurrentSoundMasterVolume;
extern TbBool SoundAble;
extern TbBool SoundActive;
extern TbBool MusicAble;
extern TbBool MusicActive;
extern TbBool MusicInstalled;
extern TbBool AILStartupAlreadyInitiated;
extern TbBool UseCurrentAwe32Soundfont;
extern TbBool DisableLoadMusic;
extern TbBool SoundInstalled;
extern TbBool StreamedSoundAble;
extern AIL_INI MusicInstallChoice;
extern AIL_INI SoundInstallChoice;
extern char FullDIG_INIPath[144];
extern char FullMDI_INIPath[144];
extern char SoundDataPath[144];
extern DIG_DRIVER *SoundDriver;
extern TbBool SoundAble;
extern SNDSEQUENCE *SongHandle;
extern SNDSAMPLE *sample_handle;
extern int32_t music_allocated;
extern int32_t CurrentMusicMasterVolume;
extern ushort SoundType;
extern TbBool DisableLoadSounds;
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

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;
extern ubyte volatile samples_currently_fading;
extern ubyte volatile switch_off_sample_fade_timer;

extern TbBool sample_queue_handle_initiated;
extern TbBool sample_queue_handle_stopped;
extern SNDSAMPLE *sample_queue_handle;
extern long sample_fade_handle;

extern char full_music_data_path[144];
extern struct MusicBankSizes music_bank_size_info;
extern void *BfMusicData;
extern void *BfMusic;

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

void ReleaseLoopedSample(unsigned short ThingOffset, unsigned short fx)
{
    asm volatile ("call ASM_ReleaseLoopedSample\n"
        : : "a" (ThingOffset),  "d" (fx));
}

void StopSampleQueueList(void)
{
    int i;

    if (!sample_queue_handle_initiated)
        return;

    sample_queue_handle_stopped = 1;
    AIL_register_EOS_callback(sample_queue_handle, 0);
    AIL_end_sample(sample_queue_handle);

    for (i = 0; i < 8; i++)
        AIL_set_sample_user_data(sample_queue_handle, i, 0);
}

void StopAllSampleFadeTimers(void)
{
    struct SampleInfo *smpinfo;

    if (samples_currently_fading == 0)
        return;
    switch_off_sample_fade_timer = 1;
    AIL_release_timer_handle(sample_fade_handle);
    samples_currently_fading = 0;
    for (smpinfo = sample_id; smpinfo <= end_sample_id; smpinfo++)
    {
        smpinfo->FadeState = 0;
        smpinfo->FadeStopFlag = 0;
    }
}

void StopAllSamples(void)
{
#if 0
    asm volatile ("call ASM_StopAllSamples\n"
        :  :  : "eax" );
#endif
    struct SampleInfo *smpinfo;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;
    StopAllSampleFadeTimers();
    for (smpinfo = sample_id; smpinfo <= end_sample_id; smpinfo++)
    {
        AIL_end_sample(smpinfo->SampleHandle);
        smpinfo->SampleNumber = 0;
        smpinfo->SourceID = 0;
        smpinfo->FadeState = 0;
        smpinfo->FadeStopFlag = 0;
    }
    StopSampleQueueList();
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

void DetermineSoundType(void)
{
    asm volatile ("call ASM_DetermineSoundType\n"
        :  :  : "eax" );
}

void FiniSound(void)
{
    if (!MusicAble)
        AIL_shutdown();
    sprintf(SoundInstallChoice.driver_name, "none");
    SoundAble = false;
    SoundActive = false;
    if (SoundDriver) {
        AIL_uninstall_DIG_driver(SoundDriver);
        SoundDriver = NULL;
    }
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
            FiniSound();
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
        FiniSound();
        return 0;
    }
    if (AIL_install_DIG_INI(&SoundDriver) != AIL_INIT_SUCCESS)
    {
        sprintf(SoundProgressMessage, "BF14 - DIG.INI driver installation - failed\n");
        SoundProgressLog(SoundProgressMessage);
        sprintf(SoundProgressMessage, " -- AIL: %s\n", AIL_API_last_error());
        SoundProgressLog(SoundProgressMessage);
        FiniSound();
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
        FiniSound();
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
    if (!AILStartupAlreadyInitiated)
    {
        AIL_MEM_use_malloc(LbMemoryAlloc_wrap);
        AIL_MEM_use_free(LbMemoryFree_wrap);
        AIL_startup();
        AILStartupAlreadyInitiated = 1;
    }
    AIL_set_preference(9, 0);
    AIL_set_preference(0, 200);
    AIL_set_preference(8, 1);
    AIL_set_preference(1, 22050);
    AIL_set_preference(3, 100);
    AIL_set_preference(4, MaxNumberOfSamples + 5);
    AIL_set_preference(5, 127);
    AIL_set_preference(6, 655);
    AIL_set_preference(7, StereoSound != 0);
    if (!AutoScanForSoundHardware)
        AIL_set_preference(17, 0);

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
        FiniSound();
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

void FiniMusic(void)
{
    if (Awe32SoundfontLoaded == 1) {
        MusicInstalled = 1;
        FreeAwe32Soundfont();
        MusicInstalled = 0;
        Awe32SoundfontLoaded = 0;
    }
    if (MusicDriver != NULL) {
        AIL_uninstall_MDI_driver(MusicDriver);
        MusicDriver = NULL;
    }
    if (!SoundAble)
        AIL_shutdown();
    MusicAble = 0;
    MusicActive = 0;
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

    if (!AILStartupAlreadyInitiated)
    {
        AIL_MEM_use_malloc(LbMemoryAlloc_wrap);
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

    ret = InitMusicDriver();
    if (ret == -1) {
        FiniMusic();
        return;
    }

    SongHandle = AIL_allocate_sequence_handle(MusicDriver);

    ret = DetermineMusicType();
    if (ret == -1) {
        FiniMusic();
        return;
    }

    ret = InitMusicBanks();
    if (ret == -1) {
        FiniMusic();
        return;
    }

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

#if 0
    flushall(); // Not a standard C func; also why?
    setbuf(stdout, NULL); // No reason for that
#endif

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
