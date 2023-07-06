#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

#include "bftypes.h"
#include "timer.h"
#include "mssal.h"

#pragma pack(1)

struct Thing;

enum SoundDriverType
{
  SOUND_DRIVER_PCM  = 0,
  SOUND_DRIVER_MIDI = 1
};


typedef struct AudioInitOptions AudioInitOptions;

struct AudioInitOptions {
  const char *IniPath;
  const char *SoundDriverPath;
  const char *SoundDataPath;
  ushort SoundType;
  ushort AbleFlags;
  short SelectedWin95MidiDevice;
  short SelectedWin95WaveDevice;
  ubyte MaxSamples;
  ubyte StereoOption;
  ubyte AutoScan;
  ubyte DisableDangerMusic;
  ubyte DisableLoadSounds;
  ubyte DisableLoadMusic;
  ubyte UseCurrentAwe32Soundfont;
  ubyte UseMultiMediaExtensions;
  ubyte InitStreamedSound;
  ubyte InitRedbookAudio;
};

struct HeapMgrHandle {
  ubyte field_0[14];
  ubyte field_E;
};

struct SampleTable {
  long field_0;
  long field_4;
  struct HeapMgrHandle *hmhandle;
};

struct SampleInfo {
  SNDSAMPLE *SampleHandle;
  long SampleVolume;
  ushort SamplePitch;
  ushort SamplePan;
  ushort FadeToVolume;
  ulong SourceID;
  short SampleNumber;
  ubyte FadeState;
  ubyte FadeStopFlag;
  ubyte FadeStep;
  ubyte UserFlag;
  ubyte SampleType;
};

#pragma pack()

extern short startscr_samplevol;
extern short startscr_midivol;
extern short startscr_cdvolume;

void InitAudio(AudioInitOptions *audOpts);

void StopMusicIfActive(void);
void StopMusic(void);
int LoadMusic(ushort bankNo);

void fill_ail_sample_ids(void);

int LoadSounds(unsigned char a1);

bool sound_initialise (void);
void sound_finalise (void);
TbBool sound_update(void);
bool sound_open_music_file (const char *fname);
void sound_play_music (void);
void sound_stop_music (void);
void sound_pause_music (void);

struct _SEQUENCE;

struct SampleInfo *play_sample_using_heap(ulong a1, short smptbl_id, ulong a3, ulong a4, ulong a5, char a6, ubyte type);
void stop_sample_using_heap(struct _SEQUENCE *source, ulong sample_number);
void play_dist_sample(struct Thing *p_thing, ushort smptbl_id, ushort vol, ushort pan, int pitch, int loop, ubyte type);

#endif
