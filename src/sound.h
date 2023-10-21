#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

#include "bftypes.h"
#include "timer.h"
#include "mssal.h"

#pragma pack(1)

struct Thing;

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

struct SNDSEQUENCE;

struct SampleInfo *play_sample_using_heap(ulong a1, short smptbl_id, ulong a3, ulong a4, ulong a5, char a6, ubyte type);
void stop_sample_using_heap(struct SNDSEQUENCE *source, ulong sample_number);
void play_dist_sample(struct Thing *p_thing, ushort smptbl_id, ushort vol, ushort pan, int pitch, int loop, ubyte type);

#endif
