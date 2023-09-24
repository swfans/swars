#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfwindows.h"
#include "bflib_snd_sys.h"
#include "bffile.h"
#include "bfmusic.h"
#include "bfsound.h"
#include "bfscd.h"
#include "drv_oal.h"
#include "sb16.h"
#include "awe32.h"
#include "streamfx.h"
#include "sndtimer.h"
#include "snderr.h"
#include "oggvorbis.h"
#include "sound.h"
#include "ailss.h"
#include "aila.h"
#include "game_data.h"
#include "util.h"

extern char FullDIG_INIPath[144];
extern char FullMDI_INIPath[144];
extern char SoundDataPath[144];
extern char SoundDriverPath[144];

extern AIL_INI MusicInstallChoice;
extern char full_music_data_path[144];
extern char MusicType[6];
extern TbBool DisableLoadMusic;
extern TbBool DisableDangerMusic;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;

extern AIL_INI SoundInstallChoice;
extern ushort SoundType;
extern TbBool DisableLoadSounds;
extern TbBool SoundAble;

extern TbBool CDAble;

extern TbBool UseMultiMediaExtensions;
extern TbBool AutoScanForSoundHardware;
extern TbBool StereoSound;
extern TbBool ive_got_an_sb16;
extern ulong MaxNumberOfSamples;
extern TbBool AILStartupAlreadyInitiated;

extern OggVorbisStream  sound_music_stream;
extern MDI_DRIVER *MusicDriver;
extern DIG_DRIVER *SoundDriver;


struct SampleInfo *play_sample_using_heap(ulong a1, short smptbl_id, ulong a3, ulong a4, ulong a5, char a6, ubyte type)
{
    struct SampleInfo *ret;
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "call ASM_play_sample_using_heap\n"
        : "=r" (ret) : "a" (a1), "d" (smptbl_id), "b" (a3), "c" (a4), "g" (a5), "g" (a6), "g" (type));
    return ret;
}

void stop_sample_using_heap(struct SNDSEQUENCE *source, ulong sample_number)
{
    asm volatile (
      "call ASM_stop_sample_using_heap\n"
        : : "a" (source), "d" (sample_number));
}

void play_dist_sample(struct Thing *p_thing, ushort smptbl_id, ushort vol, ushort pan, int pitch, int loop, ubyte type)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_play_dist_sample\n"
        : : "a" (p_thing), "d" (smptbl_id), "b" (vol), "c" (pan), "g" (pitch), "g" (loop), "g" (type));
}

//TODO better name?
void fill_ail_sample_ids(void)
{
    asm volatile ("call ASM_fill_ail_sample_ids\n"
        :  :  : "eax" );
}

