#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include OPENAL_ALC_H
#include OPENAL_AL_H

#include "bfwindows.h"
#include "bflib_snd_sys.h"
#include "bffile.h"
#include "bfscd.h"
#include "snderr.h"
#include "oggvorbis.h"
#include "sound.h"
#include "ailss.h"
#include "sound_util.h"
#include "game_data.h"
#include "util.h"


#define SOUND_MAX_SOURCES     64
#define SOUND_BUFFERS_PER_SRC 3
#define SOUND_MAX_BUFFERS     (SOUND_MAX_SOURCES * SOUND_BUFFERS_PER_SRC \
                       + SOUND_MUSIC_BUFFERS)
#define SOUND_MAX_BUFSIZE     2048


#define check_al(msg) sound_check_al (msg)


struct SourceDescriptor
{
  SNDSAMPLE *sample;
  ALuint       name;
  size_t       buffers_used;
};

typedef struct SourceDescriptor SourceDescriptor;


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
extern TbBool UseCurrentAwe32Soundfont;
extern TbBool ive_got_an_sb16;
extern ulong MaxNumberOfSamples;

size_t sound_source_count    = 0;
static size_t        sound_free_buffer_count    = 0;
static ALuint        sound_free_buffers[SOUND_MAX_BUFFERS];
static SourceDescriptor sound_sources[SOUND_MAX_SOURCES];
SNDSAMPLE sound_samples[SOUND_MAX_SOURCES];
extern OggVorbisStream  sound_music_stream;

extern MDI_DRIVER *MusicDriver;
extern DIG_DRIVER *SoundDriver;


#define check_alc(source) check_alc_line ((source), __LINE__)
bool check_alc_line(const char *source, int line);

static void
initialise_descriptor (SNDSAMPLE *samples, size_t index, ALuint name)
{
  SourceDescriptor *desc = &sound_sources[index];

  desc->sample         = &samples[index];
  desc->name         = name;
  desc->buffers_used = 0;
}

static ALuint
pop_free_buffer (void)
{
  assert (sound_free_buffer_count > 0);
  return sound_free_buffers[--sound_free_buffer_count];
}

static void
push_free_buffer (ALuint buf, void *user_data)
{
  assert (sound_free_buffer_count < SOUND_MAX_BUFFERS);
  sound_free_buffers[sound_free_buffer_count++] = buf;
}

static bool create_sources(void)
{
    size_t n, m;
    ALuint buffers[SOUND_BUFFERS_PER_SRC];

    for (n = 0; n < SOUND_MUSIC_BUFFERS; n++)
    {
        alGenBuffers(1, &buffers[0]);
        if (!check_al ("alGenBuffers"))
            goto err;

        push_free_buffer(buffers[0], NULL);
    }

    for (n = 0; n < SOUND_MAX_SOURCES; n++)
    {
        ALuint source;
        alGenSources(1, &source);
        if (alGetError() != AL_NO_ERROR) {
            n--;
            break;
        }

        alGenBuffers(SOUND_BUFFERS_PER_SRC, buffers);
        if (alGetError() != AL_NO_ERROR) {
          sound_delete_source_and_buffers(source);
          n--;
          break;
        }

        initialise_descriptor(sound_samples, sound_source_count++, source);

        for (m = 0; m < SOUND_BUFFERS_PER_SRC; m++)
            push_free_buffer(buffers[m], NULL);
    }

    if (sound_source_count == 0)
    {
        fprintf (stderr, "Error: OpenAL: "
            "Failed to create sound sources.\n");
        goto err;
    }

#ifdef DEBUG
    printf ("OpenAL: Created %zu sound sources.\n", sound_source_count);
#endif

    return true;

err:
  /* TODO: clean up */
  return false;
}

int oal_sound_initialise(void)
{
  if (!create_sources ())
    goto err;

  if (LbRegisterIdleHandler(sound_update) != Lb_SUCCESS)
    goto err;

  return true;

err:
  return false;
}

void InitAudio(AudioInitOptions *audOpts)
{
#if 0
    asm volatile (
      "call ASM_InitAudio\n"
        : : "a" (audOpts));
#endif
    if (getenv("DEBUG_BF_AUDIO"))
        DebugAudio = 1;

    sprintf(FullDIG_INIPath, "sound/DIG.INI");
    sprintf(FullMDI_INIPath, "sound/MDI.INI");
    strcpy(SoundDataPath, "sound");
    strcpy(SoundDriverPath, "sound");
    if (audOpts->IniPath) {
        sprintf(FullDIG_INIPath, "%s/DIG.INI", audOpts->IniPath);
        sprintf(FullMDI_INIPath, "%s/MDI.INI", audOpts->IniPath);
    }
    if (audOpts->SoundDataPath != NULL) {
        strcpy(SoundDataPath, audOpts->SoundDataPath);
    }
    if (audOpts->SoundDriverPath != NULL) {
        strcpy(SoundDriverPath, audOpts->SoundDriverPath);
    }

    MaxNumberOfSamples = audOpts->MaxSamples;
    SoundType = audOpts->SoundType;
    if (!audOpts->AutoScan)
        AutoScanForSoundHardware = 0;
    if (!audOpts->StereoOption)
        StereoSound = 0;
    if (audOpts->DisableDangerMusic == 1)
        DisableDangerMusic = 1;
    if (audOpts->DisableLoadSounds == 1)
        DisableLoadSounds = 1;
    if (audOpts->DisableLoadMusic == 1)
        DisableLoadMusic = 1;
    if (audOpts->UseCurrentAwe32Soundfont == 1)
        UseCurrentAwe32Soundfont = 1;
    if (audOpts->UseMultiMediaExtensions == 1)
        UseMultiMediaExtensions = 1;
    sprintf(SoundProgressMessage, "BF1  - MA   %d\n", MusicAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF2  - SA   %d\n", SoundAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF3  - CDA  %d\n", CDAble);
    SoundProgressLog(SoundProgressMessage);
    if (!(audOpts->AbleFlags & 0x01))
        MusicAble = 0;
    if (!(audOpts->AbleFlags & 0x02))
        SoundAble = 0;

    InitSound();
    InitMusic();
    InitAllBullfrogSoundTimers();

    if (audOpts->InitStreamedSound == 1)
        InitStreamedSound();

    if (audOpts->InitRedbookAudio == 1) {
        InitRedbook();
    } else if (audOpts->InitRedbookAudio == 2) {
        char mdir[FILENAME_MAX];
        snprintf(mdir, sizeof(mdir),
              "%s" FS_SEP_STR "music", GetDirectoryHdd());
        InitMusicOGG(mdir);
    } else {
        sprintf(SoundProgressMessage, "BF101 - cd init - disabled\n");
        SoundProgressLog(SoundProgressMessage);
        CDAble = false;
    }

    if (ive_got_an_sb16)
        prepare_SB16_volumes();

    sprintf(SoundProgressMessage, "BF54 - MA   %d\n", MusicAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF55 - SA   %d\n", SoundAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF55 - CDA  %d\n", CDAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF56 - music driver = %s\n", MusicInstallChoice.driver_name);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF57 - sound driver = %s\n", SoundInstallChoice.driver_name);
    SoundProgressLog(SoundProgressMessage);
}

static void
destroy_sources (void)
{
  size_t n;

  for (n = 0; n < SOUND_MAX_SOURCES; n++)
    {
      if (sound_sources[n].name == 0)
        continue;

      sound_delete_source_and_buffers (sound_sources[n].name);
    }

  alDeleteBuffers (sound_free_buffer_count, sound_free_buffers);
  check_al ("alDeleteBuffers");
}

void oal_sound_finalise(void)
{
    destroy_sources();
}

static ALenum
get_pcm_format (SNDSAMPLE *s)
{
  switch (s->format)
    {
    case 0: return AL_FORMAT_MONO8;
    case 1: return AL_FORMAT_MONO16;
    case 2: return AL_FORMAT_STEREO8;
    case 3: return AL_FORMAT_STEREO16;
    default:
      abort ();
    }
}

static void
queue_source_buffers (DIG_DRIVER *digdrv, SourceDescriptor *src)
{
  size_t len, total_len;
  void *data;
  float x_pos;
  ALint state;
  SNDSAMPLE *s;
  ALuint buf = 0;

  if (src->buffers_used >= SOUND_BUFFERS_PER_SRC)
    return;

  s = src->sample;

  total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];

  if (total_len == 0)
    {
      if (s->done[s->current_buffer ^ 1] == 0)
        s->current_buffer ^= 1;
      else
        return;

      total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];
    }


  while (total_len > 0 && src->buffers_used < SOUND_BUFFERS_PER_SRC)
    {
      data = s->start[s->current_buffer] + s->pos[s->current_buffer];
      len = MIN (total_len, SOUND_MAX_BUFSIZE);

      assert ((s->flags & 1) != 0);

      buf = pop_free_buffer ();
      alBufferData (buf, get_pcm_format (s), data, len, s->playback_rate);
      if (!check_al ("alBufferData"))
        goto err;

      alSourceQueueBuffers (src->name, 1, &buf);
      if (!check_al ("alSourceQueueBuffers"))
        goto err;

      src->buffers_used++;

      alGetSourcei (src->name, AL_SOURCE_STATE, &state);
      if (!check_al ("alGetSourcei (AL_SOURCE_STATE)"))
        goto err;

      alSourcef (src->name, AL_GAIN,
             (s->volume * (1.f / 127.f)
              * (digdrv->master_volume * (1.f / 127.f))));
      if (!check_al ("alSourcef (AL_GAIN)"))
        goto err;

      /* XXX: check if panning/position is OK */
      if (s->pan == 0)
        x_pos = 0.f;
      else
        x_pos = (127 - s->pan - 64) * (1.f / 64.f);

      alSource3f (src->name, AL_POSITION, x_pos, 0.f, -.25f);
      if (!check_al ("alSource3f (AL_POSITION)"))
        goto err;

      if (state != AL_PLAYING)
        {
          alSourcePlay (src->name);
          if (!check_al ("alSourcePlay"))
            goto err;
        }

      s->pos[s->current_buffer] += len;
      total_len -= len;

      if (total_len == 0 && s->loop_count == 0)
        {
          s->pos[s->current_buffer] = 0;
          total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];
        }
    }

  return;

err:
  if (buf != 0)
    push_free_buffer (buf, NULL);
}

static void
unqueue_source_buffers (SourceDescriptor *src)
{
    SNDSAMPLE *s;

    if (src->buffers_used == 0)
        return;

    s = src->sample;

    src->buffers_used -=
        sound_unqueue_buffers (src->name,
                   (SoundNameCallback) push_free_buffer,
                   NULL);

    if (src->buffers_used > 0
        || s->pos[s->current_buffer] < s->len[s->current_buffer]
        || s->loop_count == 0)
      return;

    /* this check prevents the sound in the intro from abruptly stopping */
    if (!s->done[0] && !s->done[1])
        return;

    alSourceStop (src->name);
    check_al ("alSourceStop");
    s->status = 2;
}

void sound_update_dig_samples(DIG_DRIVER *digdrv)
{
    int32_t n;
    SourceDescriptor *src;
    SNDSAMPLE *s;

    digdrv->n_active_samples = 0;

    for (n = 0; n < digdrv->n_samples; n++)
    {
      src = &sound_sources[n];
      s = src->sample;

      if (s->status == 1)
        continue;

#if 0
      printf ("sample %i loops:%i ([%i/%i] [%i/%i]) (%i %i) %zu\n", n,
              s->loop_count,
              s->pos[0], s->len[0],
              s->pos[1], s->len[1],
              s->done[0], s->done[1],
              src->buffers_used);
#endif

      unqueue_source_buffers (src);

      if (s->status != 4)
        continue;

      digdrv->n_active_samples++;

      queue_source_buffers (digdrv, src);
    }
}

void sound_update_mdi_sequences(MDI_DRIVER *mdidrv)
{
    int32_t i;

    for (i = 0; i < mdidrv->n_sequences; i++)
    {
        SNDSEQUENCE *seq;

        seq = &mdidrv->sequences[i];

        if (seq->status != SNDSEQ_PLAYING)
            continue;

        //TODO play MIDI

    }
}

TbBool sound_update(void)
{
    if (!SoundDriver)
        return false;
    if (!SoundDriver->drvr->initialized)
        return false;

    if (SoundDriver != NULL)
        sound_update_dig_samples(SoundDriver);
    if (MusicDriver != NULL)
        sound_update_mdi_sequences(MusicDriver);

    ogg_vorbis_stream_update(&sound_music_stream);
    return true;
}

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

void StopMusicIfActive(void)
{
    if (!MusicInstalled || !MusicAble)
        return;
    if (MusicActive)
    {
        StopMusic();
        MusicActive = 0;
    }
}

void StopMusic(void)
{
    asm volatile ("call ASM_StopMusic\n"
        :  :  : "eax" );
}

//TODO better name?
void fill_ail_sample_ids(void)
{
    asm volatile ("call ASM_fill_ail_sample_ids\n"
        :  :  : "eax" );
}

int LoadSounds(unsigned char a1)
{
    int ret;
    asm volatile ("call ASM_LoadSounds\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}
