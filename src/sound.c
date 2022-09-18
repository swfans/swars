#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include OPENAL_ALC_H
#include OPENAL_AL_H

#include "bfwindows.h"
#include "bffile.h"
#include "bflib_snd_sys.h"
#include "bflib_snd_cda.h"
#include "oggvorbis.h"
#include "sound.h"
#include "ailss.h"
#include "sound_util.h"
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


void SS_serve (DIG_DRIVER *driver);
DIG_DRIVER *SS_construct_DIG_driver (AIL_DRIVER *driver,
                           const SNDCARD_IO_PARMS *iop);

extern char SoundProgressMessage[256];
extern long DebugAudio;

extern char FullDIG_INIPath[144];
extern char FullMDI_INIPath[144];
extern char SoundDataPath[144];
extern char SoundDriverPath[144];

extern AudioDriverChoice MusicInstallChoice;
extern char full_music_data_path[144];
extern char MusicType[6];
extern TbBool DisableLoadMusic;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;
extern TbBool DisableDangerMusic;

extern AudioDriverChoice SoundInstallChoice;
extern ushort SoundType;
extern TbBool DisableLoadSounds;
extern TbBool SoundInstalled;
extern TbBool SoundAble;
extern TbBool SoundActive;

extern TbBool CDAble;

extern TbBool UseMultiMediaExtensions;
extern TbBool AutoScanForSoundHardware;
extern TbBool StereoSound;
extern TbBool UseCurrentAwe32Soundfont;
extern TbBool ive_got_an_sb16;
extern ulong MaxNumberOfSamples;

bool sound_initialised    = false;
static DIG_DRIVER  *sound_driver        = NULL;
static ALCdevice       *sound_device        = NULL;
static ALCcontext      *sound_context        = NULL;
size_t sound_source_count    = 0;
static size_t        sound_free_buffer_count    = 0;
static ALuint        sound_free_buffers[SOUND_MAX_BUFFERS];
static SourceDescriptor sound_sources[SOUND_MAX_SOURCES];
SNDSAMPLE sound_samples[SOUND_MAX_SOURCES];
static OggVorbisStream  sound_music_stream;


#define check_alc(source) check_alc_line ((source), __LINE__)
bool
check_alc_line (const char *source, int line)
{
  ALCenum err;

  assert (sound_device != NULL);

  err = alcGetError (sound_device);
  if (err != ALC_NO_ERROR)
    {
      fprintf (stderr,
               "Error: %s: 0x%x at "__FILE__":%i.\n", source, err, line);
      return false;
    }

  return true;
}

void SoundProgressLog(const char *msg)
{
    if (DebugAudio)
        printf(msg);
}

static void
initialise_descriptor (size_t index, ALuint name)
{
  SourceDescriptor *desc = &sound_sources[index];

  desc->sample         = &sound_samples[index];
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

static bool
create_sources (void)
{
  size_t n, m;
  ALuint source;
  ALuint buffers[SOUND_BUFFERS_PER_SRC];

  for (n = 0; n < SOUND_MUSIC_BUFFERS; n++)
    {
      alGenBuffers (1, &buffers[0]);
      if (!check_al ("alGenBuffers"))
        goto err;

      push_free_buffer (buffers[0], NULL);
    }

  for (n = 0; n < SOUND_MAX_SOURCES; n++)
    {
      alGenSources (1, &source);
      if (alGetError () != AL_NO_ERROR)
        {
          n--;
          break;
        }

      alGenBuffers (SOUND_BUFFERS_PER_SRC, buffers);
      if (alGetError () != AL_NO_ERROR)
        {
          sound_delete_source_and_buffers (source);
          n--;
          break;
        }

      initialise_descriptor (sound_source_count++, source);

      for (m = 0; m < SOUND_BUFFERS_PER_SRC; m++)
        push_free_buffer (buffers[m], NULL);
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

bool
sound_initialise (void)
{
  sound_device = alcOpenDevice (NULL);
  if (sound_device == NULL)
    {
      fprintf (stderr, "Error: alcOpenDevice: "
                       "Failed to open default OpenAL device.\n");
      goto err;
    }

  sound_context = alcCreateContext (sound_device, NULL);
  if (!check_alc ("alcCreateContext"))
    goto err;

  alcMakeContextCurrent (sound_context);
  if (!check_alc ("alcMakeContextCurrent"))
    goto err;

  if (!create_sources ())
    goto err;

  if (!ogg_vorbis_stream_init (&sound_music_stream))
    goto err;
  if (LbRegisterIdleHandler(sound_update) != Lb_SUCCESS)
    goto err;

  sound_initialised = true;
  return true;

err:
  sound_initialised = false;
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
    if (!(audOpts->AbleFlags & 0x01))
        MusicAble = 0;
    if (!(audOpts->AbleFlags & 0x02))
        SoundAble = 0;

    InitSound();
    InitMusic();
    InitAllBullfrogSoundTimers();

    if (audOpts->InitStreamedSound == 1)
        InitStreamedSound();

    if (audOpts->InitRedbookAudio == 1)
        InitRedbook();
    else
        CDAble = 0;

    if (ive_got_an_sb16)
        prepare_SB16_volumes();

    sprintf(SoundProgressMessage, "BF54 - MA   %d\n", MusicAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF55 - SA   %d\n", SoundAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF56 - music driver = %s\n", MusicInstallChoice.str2);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF57 - sound driver = %s\n", SoundInstallChoice.str2);
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

void
sound_finalise (void)
{
  if (!sound_initialised)
    return;

  ogg_vorbis_stream_free (&sound_music_stream);
  destroy_sources ();

  alcDestroyContext (alcGetCurrentContext ());
  check_alc ("alcDestroyContext");

  sound_initialised = false;
}

bool
sound_open_music_file (const char *fname)
{
  fprintf (stdout, "%s: Music track selected.\n", fname);
  if (!sound_initialised)
    return false;

  return ogg_vorbis_stream_open (&sound_music_stream, fname);
}

void
sound_play_music (void)
{
  if (!sound_initialised)
    return;

  ogg_vorbis_stream_play (&sound_music_stream);
}

void
sound_stop_music (void)
{
  if (!sound_initialised)
    return;

  ogg_vorbis_stream_stop (&sound_music_stream);
}

void
sound_pause_music (void)
{
  if (!sound_initialised)
    return;

  ogg_vorbis_stream_pause (&sound_music_stream);
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

TbBool sound_update(void)
{
  int32_t n;
  SourceDescriptor *src;
  SNDSAMPLE *s;
  DIG_DRIVER *digdrv = sound_driver;

  if (!sound_initialised || sound_driver == NULL)
    return false;

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

#if 0
    {
      static size_t cnt;
      static uint32_t start_ticks;

      if (start_ticks == 0)
        start_ticks = SDL_GetTicks ();

      cnt++;

      if (digdrv->n_active_samples > 0)
        {
          uint32_t ticks;

          ticks = SDL_GetTicks ();
          printf ("fps: %f\n", (float) cnt * 1000.f / (ticks - start_ticks));
        }
    }
#endif

  ogg_vorbis_stream_set_gain (&sound_music_stream,
                      startscr_cdvolume * (1.f / 322.f));
  ogg_vorbis_stream_update (&sound_music_stream);
  return true;
}

int32_t
AIL2OAL_API_install_DIG_INI(DIG_DRIVER **digdrv)
{
  if (sound_driver != NULL)
    return -1;

  sound_driver = AIL2OAL_API_install_DIG_driver_file(NULL, NULL);
  *digdrv = sound_driver;

  if (sound_driver == NULL)
    return -1;

  return 0;
}

void
AIL2OAL_API_call_driver(AIL_DRIVER *drv, int32_t fn,
               VDI_CALL *in, VDI_CALL *out)
{
#if 0
  printf ("AIL2OAL_API_call_driver (%p, 0x%x, %p, %p)\n",
          drv, fn, in, out);
#endif
}

const SNDCARD_IO_PARMS *
AIL2OAL_API_get_IO_environment(AIL_DRIVER *drvr)
{
    static SNDCARD_IO_PARMS iop = {0x220, 7, 1, 1, {0, 0, 0, 0}};
    return &iop;
}

const SNDCARD_IO_PARMS *
AIL2OAL_get_IO_environment(AIL_DRIVER *drvr)
{
    const SNDCARD_IO_PARMS *iop;
    iop = AIL2OAL_API_get_IO_environment(drvr);
    return iop;
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

void stop_sample_using_heap(struct _SEQUENCE *source, ulong sample_number)
{
    asm volatile (
      "call ASM_stop_sample_using_heap\n"
        : : "a" (source), "d" (sample_number));
}

TbBool GetSoundInstalled(void)
{
    return SoundInstalled;
}

TbBool GetSoundAble(void)
{
    return SoundAble;
}

TbBool GetSoundActive(void)
{
    return SoundActive;
}

TbBool GetMusicAble(void)
{
    return MusicAble;
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

ubyte load_music_bank(TbFileHandle fh, ubyte bankId)
{
    ubyte ret;
    asm volatile ("call ASM_load_music_bank\n"
        : "=r" (ret) : "a" (fh),  "d" (bankId));
    return ret;
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

    LbFileSeek(fh, fsize - 4, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, &nbanks_offs, 4);
    LbFileSeek(fh, nbanks_offs, Lb_FILE_SEEK_BEGINNING);
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
        sprintf(SoundProgressMessage, "BF49 - load music bank - failed - bank not found\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileSeek(fh, bankNo << 6, 1u);
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
