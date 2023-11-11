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
  u8 field_0[14];
  u8 field_E;
};

struct SampleTable {
  i32 field_0;
  i32 field_4;
  struct HeapMgrHandle *hmhandle;
};

#pragma pack()

extern i16 startscr_samplevol;
extern i16 startscr_midivol;
extern i16 startscr_cdvolume;

void fill_ail_sample_ids(void);

struct SNDSEQUENCE;

struct SampleInfo *play_sample_using_heap(u32 a1, i16 smptbl_id, u32 a3, u32 a4, u32 a5, char a6, u8 type);
void stop_sample_using_heap(struct SNDSEQUENCE *source, u32 sample_number);
void play_dist_sample(struct Thing *p_thing, u16 smptbl_id, u16 vol, u16 pan, int pitch, int loop, u8 type);

#endif
