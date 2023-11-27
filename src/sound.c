#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfwindows.h"
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

void snd_unkn1_volume_all_samples(void)
{
    asm volatile ("call ASM_snd_unkn1_volume_all_samples\n"
        :  :  : "eax" );
}

