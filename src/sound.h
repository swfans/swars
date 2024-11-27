#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

#include "bftypes.h"
#include "timer.h"
#include "mssal.h"

#pragma pack(1)

enum SounsHeapSetupCmd {
    /** Load sounds from a bank selected based on available memory. */
    SHSC_GameSndAutoQ = 0,
    /** Unload sounds and free memory. */
    SHSC_ResetGameSnd,
    /** Load best quality sounds only */
    SHSC_GameSndBestQ,
    /** Load sounds for game credits screen. */
    SHSC_CreditsSnd = 100,
};

struct Thing;
struct SimpleThing;

struct HeapMgrHandle {
  ubyte field_0[14];
  ubyte field_E;
};

struct SampleTable {
  long field_0;
  long field_4;
  struct HeapMgrHandle *hmhandle;
};

#pragma pack()

extern short startscr_samplevol;
extern short startscr_midivol;
extern short startscr_cdvolume;

void fill_ail_sample_ids(void);
void monitor_all_samples(void);

struct SampleInfo *play_sample_using_heap(ulong a1, short smptbl_id, ulong a3, ulong a4, ulong a5, char a6, ubyte type);
void stop_sample_using_heap(long source_id, ulong sample_number);
void play_dist_sample(struct Thing *p_thing, ushort smptbl_id, ushort vol, ushort pan, int pitch, int loop, ubyte type);
void play_dist_ssample(struct SimpleThing *p_sthing, ushort smptbl_id, ushort vol, ushort pan, int pitch, int loop, ubyte type);

void snd_unkn1_volume_all_samples(void);
void wait_for_sound_sample_finish(ushort smpl_id);

void setup_heaps(short setup_cmd, const char *lang);
void reset_heaps(void);

#endif
