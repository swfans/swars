#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfwindows.h"
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfmusic.h"
#include "bfsound.h"
#include "bfscd.h"
#include "sb16.h"
#include "awe32.h"
#include "streamfx.h"
#include "sndtimer.h"
#include "snderr.h"
#include "ssampply.h"
#include "ssamplst.h"
#include "sound.h"
#include "ailss.h"
#include "aila.h"
#include "display.h"
#include "game_data.h"
#include "game_speed.h"
#include "game.h"
#include "util.h"

extern long sound_heap_size;
extern struct SampleTable *sound_heap_memory;
extern TbFileHandle sound_file; // = INVALID_FILE;
extern struct HeapMgrHeader *hmhead;
extern long samples_in_bank;
extern struct SampleTable *sample_table;

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

void stop_sample_using_heap(long source_id, ulong sample_number)
{
    asm volatile (
      "call ASM_stop_sample_using_heap\n"
        : : "a" (source_id), "d" (sample_number));
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

void wait_for_sound_sample_finish(ushort smpl_id)
{
    while (IsSamplePlaying(0, smpl_id, NULL)) {
        swap_wscreen();
        game_update();
    }
}

struct HeapMgrHeader *heapmgr_init(struct HeapMgrHeader *head, int a2, int a3)
{
    struct HeapMgrHeader *ret;
    asm volatile ("call ASM_heapmgr_init\n"
        : "=r" (ret) : "a" (head), "d" (a2), "b" (a3));
    return ret;
}

int setup_heap_manager(struct SampleTable *smptable, size_t smptb_len, const char *fname, ushort sndtype)
{
    TbFileHandle fh;
    ubyte tpno;
    long totlen;
    long pos;
    struct SampleTable *smptb;
    size_t tab_smptb_len;
    struct BfSoundBankHead *sbh;
    int i;
    long smptb_len_diff;
    ubyte *p_smptb_end;
    struct BfSoundBankHead sbharr[9];
    struct BfSfxInfo sfxi;
    ubyte unkhead1[18];

    LbMemorySet(smptable, 0, smptb_len);
    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    sound_file = fh;
    if (fh == INVALID_FILE) {
        reset_heaps();
        return 0;
    }
    tpno = GetSoundTpNo(sndtype);
    if (tpno >= 255) {
        reset_heaps();
        return 0;
    }

    totlen = LbFileLengthHandle(fh);
    LbFileSeek(fh, totlen - 4, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, &pos, 4);
    LbFileSeek(fh, pos, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, &unkhead1, 18);
    LbFileRead(fh, &sbharr, sizeof(struct BfSoundBankHead) * 9);
    sbh = &sbharr[tpno];
    if ((sbh->TabPos == 0) || (sbh->TabSize == 0)) {
        reset_heaps();
        return 0;
    }
    samples_in_bank = sbh->TabSize / sizeof(struct BfSfxInfo);
    tab_smptb_len = sizeof(struct SampleTable) * samples_in_bank;
    if (tab_smptb_len >= smptb_len) {
        reset_heaps();
        return 0;
    }
    LbFileSeek(fh, sbh->TabPos, Lb_FILE_SEEK_BEGINNING);
    smptb = smptable;
    for (i = 0; i < samples_in_bank; i++)
    {
        LbFileRead(fh, &sfxi, sizeof(struct BfSfxInfo));
        smptb->field_0 = sbh->DatPos + (long)sfxi.DataBeg;
        smptb->hmhandle = 0;
        smptb->field_4 = (long)sfxi.DataEnd;
        smptb++;
    }
    if (samples_in_bank <= 0) {
        reset_heaps();
        return 0;
    }
    sample_table = smptable;
    smptb_len_diff = smptb_len - tab_smptb_len;
    p_smptb_end = (ubyte *)smptable + tab_smptb_len;
    if (smptb_len_diff <= 0)
        return 0;
    hmhead = heapmgr_init((struct HeapMgrHeader *)p_smptb_end, smptb_len_diff, samples_in_bank);
    if (hmhead == NULL) {
        reset_heaps();
        return 0;
    }
    reset_heaps();
    return 1;
}

void reset_heaps(void)
{
}

void setup_heaps(int a1)
{
    long sz;
    unsigned int n;

    if ((ingame.Flags & GamF_Unkn00020000) == 0)
        return;

    switch (a1)
    {
    case 100:
        sz = sound_heap_size;
        setup_heap_manager(sound_heap_memory, sz, "sound/syncreds.dat", 1622);
        break;
    case 99:
        break;
    case 0:
        sz = 1500000;
        while ((sound_heap_memory == NULL) && (sz > 100))
        {
            sound_heap_memory = LbMemoryAlloc(sz);
            if (sound_heap_memory == NULL)
                sz -= 30000;
        }
        if ((sound_heap_memory == NULL) || sz < 120000) {
            ingame.Flags &= GamF_Unkn00020000;
            break;
        }
        if (sz <= 500000)
            n = 822;
        else
            n = 1622;
        if (!setup_heap_manager(sound_heap_memory, sz, "sound/sound.dat", n))
            ingame.Flags &= GamF_Unkn00020000;
        sound_heap_size = sz;
        break;
    case 2:
        sz = 1500000;
        if (!setup_heap_manager(sound_heap_memory, sz, "sound/sound.dat", 1622))
            ingame.Flags &= GamF_Unkn00020000;
        sound_heap_size = sz;
        break;
    case 1:
    default:
        sz = 1500000;
        reset_heaps();
        LbMemoryFree(sound_heap_memory);
        sound_heap_size = sz;
        break;
    }
}

