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
#include <string.h>
#include <strings.h>
#include <assert.h>
#include "bflib_snd_sys.h"

#include "bfsvaribl.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bffile.h"
#include "bfendian.h"
#include "rnc_1fm.h"
#include "bfscd.h"
#include "bfsound.h"
#include "ail.h"
#include "aildebug.h"
#include "memfile.h"
#include "sb16.h"
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
extern TbBool DisableLoadMusic;
extern TbBool SoundInstalled;
extern TbBool StreamedSoundAble;
extern uint16_t SongCurrentlyPlaying;
extern int32_t sample_file;
extern uint8_t ssnd_active;
extern TbFileHandle adpcm_handle;
extern long adpcm_file_open;
extern uint8_t mixed_file_open;
extern AIL_INI MusicInstallChoice;
extern AIL_INI SoundInstallChoice;
extern char FullDIG_INIPath[144];
extern char FullMDI_INIPath[144];
extern char SoundDataPath[144];
extern DIG_DRIVER *SoundDriver;
extern TbBool SoundAble;
extern SNDSEQUENCE *SongHandle;
extern ushort CurrentTempo;
extern sbyte CurrentDangerMusicFadeDirection;
extern ubyte DangerMusicFadeActive;
extern ubyte DangerMusicAble;
extern ubyte DangerMusicVolume;
extern sbyte DangerMusicVolumeChange; // = -1;
extern HSNDTIMER DangerMusicFadeHandle;
extern TbBool DisableDangerMusic;
extern ubyte data_1e5edc[16];

extern SNDSAMPLE *sample_handle;
extern int32_t music_allocated;
extern int32_t CurrentMusicMasterVolume;
extern ushort SoundType;
extern TbBool DisableLoadSounds;
extern char MusicType[6];
extern TbBool AutoScanForSoundHardware;
extern MDI_DRIVER *MusicDriver;
extern uint8_t *ssnd_buffer[2];
extern uint8_t *adpcm_source_buffer;
extern int16_t *mixer_buffer;

extern struct SampleInfo sample_id[32];
extern struct SampleInfo *end_sample_id;

extern char full_music_data_path[144];
extern struct MusicBankSizes music_bank_size_info;
extern void *BfMusicData;
extern struct BfMusicInfo *BfMusic;
extern struct BfMusicInfo *BfEndMusic;
extern short NumberOfSongs;

extern struct sound_timer_inf sound_timer[5];

/** Releases sound sample which is playing in a loop.
 *
 * @param thingOffset Index of the related thing; note that it is unsigned;
 *        simple thing offsets should be just treated as unsigned here.
 */
void ReleaseLoopedSample(ushort thingOffset, ushort fx)
{
#if 0
    asm volatile ("call ASM_ReleaseLoopedSample\n"
        : : "a" (thingOffset),  "d" (fx));
#endif
    struct SampleInfo *smpinfo;

    if (!SoundInstalled || !SoundAble || !SoundActive)
        return;

    for (smpinfo = sample_id; smpinfo <= end_sample_id; smpinfo++)
    {
        if (thingOffset == smpinfo->SourceID && fx == smpinfo->SampleNumber) {
            if (AIL_sample_status(smpinfo->SampleHandle) == SNDSMP_PLAYING)
                AIL_set_sample_loop_count(smpinfo->SampleHandle, 1);
        }
    }
}

void SetMusicVolume(int msec, ubyte volume)
{
    if (!MusicAble || !MusicInstalled)
        return;

    if (!MusicActive || !SongCurrentlyPlaying)
        return;

    if (volume > 127)
        return;

    if (AIL_sequence_status(SongHandle) == 2)
        return;

    AIL_set_sequence_volume(SongHandle, volume, msec);
}

void format_music(void)
{
    short nsongs;
    ubyte *dt;
    struct BfMusicInfo *musinf;
    struct BfMusicInfo *musinfend;
    ulong offs;

    nsongs = NumberOfSongs;
    musinfend = BfEndMusic;
    dt = BfMusicData;
    if ((BfMusic != NULL) && (BfMusicData != NULL))
    {
        nsongs = 0;
        for (musinf = &BfMusic[1]; musinf < musinfend; musinf++)
        {
            offs = (ulong)musinf->DataBeg;
            musinf->DataBeg = &dt[offs];
            offs = (ulong)musinf->DataEnd;
            musinf->DataEnd = &dt[offs];
            nsongs++;
        }
    }
    BfMusicData = dt;
    BfEndMusic = musinfend;
    NumberOfSongs = nsongs;
}

void init_music_bank_songs(void)
{
    // No action needed, song will be initiated by StartMusic()
}

ubyte load_music_bank(TbFileHandle fh, ubyte bankId)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_load_music_bank\n"
        : "=r" (ret) : "a" (fh),  "d" (bankId));
    return ret;
#endif
    ubyte *m;
    ubyte *dt;
    struct MusicBankHead mbhead[4];

    LbFilePosition(fh);
    LbFileRead(fh, &mbhead, sizeof(mbhead));
    if (mbhead[bankId].data_offset == -1)
        return 0;

    dt = BfMusicData;
    if (dt == NULL)
        return 0;
    m = (ubyte *)BfMusic;
    if (m == NULL)
        return 0;
    // Read songs data
    BfEndMusic = (struct BfMusicInfo *)(m + mbhead[bankId].info_size);
    LbFileSeek(fh, mbhead[bankId].data_offset, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, dt, 8);
    if (blong(dt+0) == RNC_SIGNATURE)
    {
        long flength;
        flength = blong(dt+4);
        LbFileRead(fh, dt + 8, flength - 8);
        UnpackM1(dt, flength);
    }
    else
    {
        LbFileRead(fh, dt + 8, mbhead[bankId].data_size - 8);
    }
    // Read songs info
    LbFileSeek(fh, mbhead[bankId].info_offset, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, m, 8);
    if (blong(m+0) == RNC_SIGNATURE)
    {
        long flength;
        flength = blong(m+4);
        LbFileRead(fh, m + 8, flength - 8);
        UnpackM1(m, flength);
    }
    else
    {
        LbFileRead(fh, m + 8, mbhead[bankId].info_size - 8);
    }

    format_music();
    init_music_bank_songs();

    sprintf(SoundProgressMessage, "BF113 - loaded music bank %d, %d songs\n",
            (int)bankId, (int)NumberOfSongs);
    SoundProgressLog(SoundProgressMessage);

    MusicAble = true;
    return 1;
}

int LoadMusic(ushort bankNo)
{
#if 0
    int ret;
    asm volatile ("call ASM_LoadMusic\n"
        : "=r" (ret) : "a" (a1));
    return ret;
#endif
    TbFileHandle fh;
    long fsize;
    ulong nbanks_offs;
    ushort nbanks[4];
    ubyte bankId;

    sprintf(SoundProgressMessage, "BF48 - load music bank %d\n", bankNo);
    SoundProgressLog(SoundProgressMessage);

    if (!MusicInstalled) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - music not installed\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    if (!MusicAble) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - MusicAble = 0\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    if (DisableLoadMusic) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - LoadMusic Disabled = 0\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    StopMusic();
    fh = LbFileOpen(full_music_data_path, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        sprintf(SoundProgressMessage, "BF52 - load music bank - failed - no music.dat\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileSeek(fh, 0, Lb_FILE_SEEK_END);
    fsize = LbFilePosition(fh);

    // The bank file ends with offset to array of banks
    LbFileSeek(fh, fsize - 4, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, &nbanks_offs, 4);
    LbFileSeek(fh, nbanks_offs, Lb_FILE_SEEK_BEGINNING);
    // Array of banks has 4 16-bit entries
    LbFileRead(fh, nbanks, 8);

    switch (MusicType[0])
    {
    case 'G':
    case 'g':
    default:
        bankId = 0;
        break;
    case 'R':
    case 'r':
        bankId = 1;
        break;
    case 'F':
    case 'f':
        bankId = 2;
        break;
    case 'W':
    case 'w':
        bankId = 3;
        break;
    }

    if (bankNo + 1 > nbanks[bankId]) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF49 - load music bank - failed - bank %d not found\n", (int)bankId);
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    // Each bank has a 64-byte header, place the file at the one we want
    LbFileSeek(fh, bankNo * 64, Lb_FILE_SEEK_CURRENT);
    // Load the music tracks and info on each track
    if (!load_music_bank(fh, bankId)) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF50 - load music bank - failed - cannot allocate\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileClose(fh);
    sprintf(SoundProgressMessage, "BF51 - load music bank - passed\n");
    SoundProgressLog(SoundProgressMessage);
    return 0;
}

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
            data_1e5edc[log] = 1;
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

    if (!MusicInstalled || !MusicAble)
        return;
    if (!MusicActive)
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
        data_1e5edc[i] = 0;
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

void close_adpcm_file(void)
{
    if (adpcm_handle == INVALID_FILE)
        return;
    LbFileClose(adpcm_handle);
    adpcm_handle = INVALID_FILE;
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
    StreamedSoundAble = true;
#endif
}

void SwitchOffStreamedSound(void)
{
    if (!StreamedSoundAble || !ssnd_active)
        return;
    AIL_end_sample(sample_handle);
    memset(ssnd_buffer[0], 0, 0x4000u);
    memset(ssnd_buffer[1], 0, 0x4000u);
    if (sample_file != -1) {
        LbFileClose(sample_file);
        sample_file = -1;
    }
    if (adpcm_file_open) {
        close_adpcm_file();
        adpcm_file_open = 0;
    }
    if (mixed_file_open)
        mixed_file_open = 0;
    ssnd_active = false;
}

void FreeStreamedSound(void)
{
    if (sample_file != -1) {
        LbFileClose(sample_file);
        sample_file = -1;
    }
    SwitchOffStreamedSound();
    free_buffers();
    StreamedSoundAble = false;
}

void InitAllBullfrogSoundTimers(void)
{
#if 0
    asm volatile ("call ASM_InitAllBullfrogSoundTimers\n"
        :  :  : "eax" );
#endif
    if (!MusicInstalled && !SoundInstalled)
        return;
    LbMemorySet(&sound_timer, 0, 5 * sizeof(struct sound_timer_inf));
}

ushort SetupBullfrogSoundTimer(ushort freq, void (*cb)())
{
    struct sound_timer_inf *stinf;
    ushort st;

    if (!MusicInstalled && !SoundInstalled)
        return 0;

    for (st = 0; st < 5; st++)
    {
        stinf = &sound_timer[st];
        if (stinf->used)
            break;
    }
    if (st >= 5)
        return 0;

    stinf = &sound_timer[st];
    stinf->handle = AIL_register_timer(cb);
    AIL_set_timer_frequency(stinf->handle, freq);
    AIL_start_timer(stinf->handle);
    stinf->used = 1;
    return st+1;
}

void ReleaseBullfrogSoundTimer(ushort st)
{
    struct sound_timer_inf *stinf;

    if (!MusicInstalled && !SoundInstalled)
        return;

    stinf = &sound_timer[st-1];
    if (stinf->used == 1)
    {
        AIL_release_timer_handle(stinf->handle);
        stinf->used = 0;
    }
}

void FreeAudio(void)
{
#if 0
    asm volatile ("call ASM_FreeAudio\n"
        :  :  : "eax" );
#endif
    if (GetCDAble()) {
        FreeCD();
        if (!GetSoundAble() && !GetMusicAble())
            AIL_shutdown();
    }
    if (StreamedSoundAble)
        FreeStreamedSound();
    FreeMusic();
    FreeSound();
    if (sb16_mixer_set)
        reset_SB16_volumes();
}

/******************************************************************************/
