#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include OPENAL_ALC_H
#include OPENAL_AL_H

#include "oggvorbis.h"
#include "sound.h"
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
  SoundSample *sample;
  ALuint       name;
  size_t       buffers_used;
};

typedef struct SourceDescriptor SourceDescriptor;


void func_112910 (SoundPCMDriver *driver);
SoundPCMDriver *ail_create_dig_driver (SoundDriver *driver,
				       const SoundIOParameters *iop);


extern uint16_t		sound_volume;
extern uint16_t		sound_tension_music_volume;
extern uint16_t		sound_music_volume;

static bool		sound_initialised	= false;
static SoundPCMDriver  *sound_driver		= NULL;
static ALCdevice       *sound_device		= NULL;
static ALCcontext      *sound_context		= NULL;
static size_t		sound_source_count	= 0;
static size_t		sound_free_buffer_count	= 0;
static ALuint		sound_free_buffers[SOUND_MAX_BUFFERS];
static SourceDescriptor sound_sources[SOUND_MAX_SOURCES];
static SoundSample	sound_samples[SOUND_MAX_SOURCES];
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

static void
initialise_descriptor (size_t index, ALuint name)
{
  SourceDescriptor *desc = &sound_sources[index];

  desc->sample	     = &sound_samples[index];
  desc->name	     = name;
  desc->buffers_used = 0;
}

static ALuint
pop_free_buffer (void)
{
  assert (sound_free_buffer_count > 0);
  return sound_free_buffers[--sound_free_buffer_count];
}

static void
push_free_buffer (ALuint buf)
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

      push_free_buffer (buffers[0]);
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
	push_free_buffer (buffers[m]);
    }

  if (sound_source_count == 0)
    {
      fprintf (stderr, "Error: OpenAL: "
	               "Failed to create sound sources.\n");
      goto err;
    }

#ifdef ENABLE_DEBUG
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

  sound_initialised = true;
  return true;

err:
  sound_initialised = false;
  return false;
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
get_pcm_format (SoundSample *s)
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
queue_source_buffers (SoundPCMDriver *pcmdrv, SourceDescriptor *src)
{
  size_t len, total_len;
  void *data;
  float x_pos;
  ALint state;
  SoundSample *s;
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
		  * (pcmdrv->master_volume * (1.f / 127.f))));
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
    push_free_buffer (buf);
}

static void
unqueue_source_buffers (SourceDescriptor *src)
{
  SoundSample *s;

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

void
sound_update (void)
{
  int32_t n;
  SourceDescriptor *src;
  SoundSample *s;
  SoundPCMDriver *pcmdrv = sound_driver;

  if (!sound_initialised || sound_driver == NULL)
    return;

  pcmdrv->n_active_samples = 0;

  for (n = 0; n < pcmdrv->n_samples; n++)
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

      pcmdrv->n_active_samples++;

      queue_source_buffers (pcmdrv, src);
    }

#if 0
    {
      static size_t cnt;
      static uint32_t start_ticks;

      if (start_ticks == 0)
	start_ticks = SDL_GetTicks ();

      cnt++;

      if (pcmdrv->n_active_samples > 0)
	{
	  uint32_t ticks;

	  ticks = SDL_GetTicks ();
	  printf ("fps: %f\n", (float) cnt * 1000.f / (ticks - start_ticks));
	}
    }
#endif

  ogg_vorbis_stream_set_gain (&sound_music_stream,
			      sound_music_volume * (1.f / 322.f));
  ogg_vorbis_stream_update (&sound_music_stream);
}

SoundPCMDriver *
sound_install_dig_driver_file (const char *fname,
			       const SoundIOParameters *iop)
{
  SoundPCMDriver *pcmdrv;
  SoundDriver *drv;
  int32_t n;

  if (!sound_initialised)
    return NULL;

  drv	 = xcalloc (sizeof (*drv));
  pcmdrv = xcalloc (sizeof (*pcmdrv));

  pcmdrv->n_samples	      = sound_source_count;
  pcmdrv->buffer_flag	      = xcalloc (sizeof (int16_t));
  pcmdrv->build_buffer	      = xcalloc (4 * pcmdrv->n_samples);
  pcmdrv->build_size          = 4 * pcmdrv->n_samples;
  pcmdrv->bytes_per_channel   = 2;
  pcmdrv->channels_per_sample = 2;
  pcmdrv->channels_per_buffer = 2;
  pcmdrv->drvr		      = drv;
  pcmdrv->hw_format	      = 3;
  pcmdrv->master_volume	      = 127;
  pcmdrv->playing	      = 1;
  pcmdrv->DMA_rate	      = 44100;
  pcmdrv->half_buffer_size    = 2048;
  pcmdrv->samples	      = sound_samples;

  assert (sizeof (SoundSample) == 0x894);

#if 0
  pcmdrv->timer = timer_register_callback ((TimerCallback) update_sound);
  timer_set_user_data (pcmdrv->timer, pcmdrv);

  pcmdrv->timer = timer_register_callback ((TimerCallback) &func_112910);
  timer_set_user_data (pcmdrv->timer, pcmdrv);
#endif

  for (n = 0; n < pcmdrv->n_samples; n++)
    {
      pcmdrv->samples[n].driver = drv;
      pcmdrv->samples[n].status = 1;
    }

  drv->descriptor = pcmdrv;
  drv->initialized = 1;
  drv->type = 0;

  return pcmdrv;
}

int32_t
sound_install_dig_ini (SoundPCMDriver **pcmdrv)
{
  if (sound_driver != NULL)
    return -1;

  sound_driver = sound_install_dig_driver_file (NULL, NULL);
  *pcmdrv = sound_driver;

  if (sound_driver == NULL)
    return -1;

  return 0;
}

void
sound_call_driver (SoundDriver *drv, int32_t fn,
		   SoundDriverCallParameters *in,
		   SoundDriverCallParameters *out)
{
#if 0
  printf ("sound_call_driver (%p, 0x%x, %p, %p)\n",
	  drv, fn, in, out);
#endif
}

const SoundIOParameters *
sound_get_io_environment (SoundDriver *drv)
{
  static SoundIOParameters iop = {0x220, 7, 1, 1, {0, 0, 0, 0}};
  return &iop;
}
