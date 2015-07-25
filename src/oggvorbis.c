#include <assert.h>
#include <string.h>

#include "oggvorbis.h"
#include "sound_util.h"
#include "util.h"

#define SOUND_MUSIC_BUFSIZE   16384

#define check_al(msg) sound_check_al (msg)

bool
ogg_vorbis_stream_init (OggVorbisStream *stream)
{
  memset (stream, 0, sizeof (*stream));

  alGenSources (1, &stream->source);
  if (!check_al ("alGenSources"))
    return false;

  alGenBuffers (SOUND_MUSIC_BUFFERS, stream->buffers);
  if (!check_al ("alGenBuffers"))
    return false;

  stream->buffer_count = SOUND_MUSIC_BUFFERS;

  return true;
}

void
ogg_vorbis_stream_free (OggVorbisStream *stream)
{
  if (stream->source != 0)
    sound_delete_source_and_buffers (stream->source);

  if (stream->buffers[0] != 0)
    alDeleteBuffers (SOUND_MUSIC_BUFFERS, stream->buffers);

  if (stream->file_name != NULL)
    {
      xfree (stream->file_name);
      ov_clear (&stream->file);
    }

  memset (stream, 0, sizeof (*stream));
}

void
ogg_vorbis_stream_pause (OggVorbisStream *stream)
{
  alSourcePause (stream->source);
  check_al ("alSourceStop");
}

void
ogg_vorbis_stream_play (OggVorbisStream *stream)
{
  alSourcePlay (stream->source);
  alGetError ();
  stream->playing = true;
}

void
ogg_vorbis_stream_stop (OggVorbisStream *stream)
{
  if (!stream->playing)
    return;

  /*
   * let the music finish playing and stop when the source runs out of
   * buffers, instead of calling alSourceStop immediately
   */
  stream->playing = false;

  if (stream->file_name == NULL)
    return;

  ogg_vorbis_stream_restart (stream);
}

void
ogg_vorbis_stream_clear (OggVorbisStream *stream)
{
  if (stream->file_name == NULL)
    return;

  ogg_vorbis_stream_stop (stream);
  ov_clear (&stream->file);
  xfree (stream->file_name);
  stream->file_name = NULL;
}

bool
ogg_vorbis_stream_open (OggVorbisStream *stream, const char *fname)
{
  FILE *f = NULL;
  vorbis_info *info;

  ogg_vorbis_stream_clear (stream);

  f = fopen (fname, "rb");
  if (f == NULL)
    {
      perror (fname);
      return false;
    }

  if (ov_open_callbacks (f, &stream->file,
			 NULL, 0, OV_CALLBACKS_DEFAULT) != 0)
    {
      fprintf (stderr, "%s: Invalid Ogg/Vorbis stream.\n", fname);
      goto err;
    }

  f = NULL;

  info = ov_info (&stream->file, -1);
  if (info == NULL)
    {
      fprintf (stderr, "%s: Failed to read stream information.\n", fname);
      goto err;
    }

  stream->info	    = *info;
  stream->file_name = xstrdup (fname);

  return true;
err:
  if (f != NULL)
    fclose (f);
  else
    ov_clear (&stream->file);

  return false;
}

bool
ogg_vorbis_stream_restart (OggVorbisStream *stream)
{
  char fname[FILENAME_MAX];

  if (stream->file_name == NULL)
    return false;

  if (ov_pcm_seek (&stream->file, 0) == 0)
    return true;

  snprintf (fname, sizeof (fname), "%s", stream->file_name);

  return ogg_vorbis_stream_open (stream, fname);
}

static void
push_free_buffer (ALuint buf, OggVorbisStream *stream)
{
  stream->buffers[stream->buffer_count++] = buf;
}

void
ogg_vorbis_stream_set_gain (OggVorbisStream *stream, float gain)
{
  alSourcef (stream->source, AL_GAIN, gain);
  check_al ("alSourcef (AL_GAIN)");
}

bool
ogg_vorbis_stream_update (OggVorbisStream *stream)
{
  char buffer[SOUND_MUSIC_BUFSIZE];
  ALuint buf;
  ALint queued;
  ALint processed;
  ALint state;
  ALint n;
  ALenum format;
  long total;
  long count;
#ifdef WORDS_BIGENDIAN
  const int big_endian = 1;
#else
  const int big_endian = 0;
#endif

  alGetSourcei (stream->source, AL_BUFFERS_QUEUED, &queued);
  if (!check_al ("alGetSourcei (AL_BUFFERS_QUEUED)"))
    return false;

  alGetSourcei (stream->source, AL_BUFFERS_PROCESSED, &processed);
  if (!check_al ("alGetSourcei (AL_BUFFERS_PROCESSED)"))
    return false;

  if (queued - processed >= SOUND_MUSIC_BUFFERS)
    return true;

  if (processed > 0)
    sound_unqueue_buffers (stream->source,
			   (SoundNameCallback) push_free_buffer,
			   stream);

  if (stream->file_name == NULL || !stream->playing)
    return true;

  for (n = 0; n < SOUND_MUSIC_BUFFERS - queued; n++)
    {
      total = 0;

      while (total < (long) sizeof (buffer))
	{
	  count = ov_read (&stream->file, buffer + total,
			   sizeof (buffer) - total, big_endian, 2,
			   true, NULL);
	  if (count < 0)
	    {
	      fprintf (stderr, "Error: Failed to read ogg/vorbis data.\n");
	      stream->playing = false;
	      return false;
	    }
	  else if (count == 0)
	    break;

	  total += count;
	}

      if (total == 0)
	return ogg_vorbis_stream_restart (stream);

      assert (stream->buffer_count > 0);
      buf = stream->buffers[--stream->buffer_count];

      if (stream->info.channels == 2)
	format = AL_FORMAT_STEREO16;
      else
	format = AL_FORMAT_MONO16;

      alBufferData (buf, format, buffer, total, stream->info.rate);
      if (!check_al ("alBufferData"))
	{
	  assert (stream->buffer_count < SOUND_MUSIC_BUFFERS);
	  stream->buffers[stream->buffer_count++] = buf;
	  return false;
	}

      alSourceQueueBuffers (stream->source , 1, &buf);
      if (!check_al ("alSourceQueueBuffers"))
	{
	  assert (stream->buffer_count < SOUND_MUSIC_BUFFERS);
	  stream->buffers[stream->buffer_count++] = buf;
	  return false;
	}

      queued += 1;
    }

  if (!stream->playing)
    return true;

  alGetSourcei (stream->source, AL_SOURCE_STATE, &state);
  if (!check_al ("alGetSourcei (AL_SOURCE_STATE)"))
    return false;

  if (state != AL_PLAYING && state != AL_PAUSED && queued > 0)
    {
      alSourcePlay (stream->source);
      if (!check_al ("alSourcePlay"))
	return false;
    }

  return true;
}
