/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file oggvorbis.c
 *     Ogg/Vorbis audio files playback.
 * @par Purpose:
 *     Allows emulating CD Audio using compressed audio files.
 * @par Comment:
 *     None.
 * @author   Gynvael Coldwind
 * @author   Unavowed
 * @date     12 Jan 2010 - 05 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "oggvorbis.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "drv_oal.h"
#include "snderr.h"

/******************************************************************************/

#define SOUND_MUSIC_BUFSIZE   16384

#define check_al(source) check_al_line((source), __LINE__)
bool check_al_line(const char *source, int line);

bool
ogg_vorbis_stream_init (OggVorbisStream *stream)
{
  memset (stream, 0, sizeof (*stream));

  if (!OPENAL_create_source_for_ogg_vorbis(stream))
    return false;

  if (!OPENAL_create_buffers_for_ogg_vorbis(stream))
    return false;

  return true;
}

static void push_free_buffer(ALuint buf, OggVorbisStream *stream)
{
  assert(stream->buffer_count < SOUND_MUSIC_BUFFERS);
  stream->buffers[stream->buffer_count++] = buf;
}

static ALuint pop_free_buffer(OggVorbisStream *stream)
{
  assert(stream->buffer_count > 0);
  return stream->buffers[--stream->buffer_count];
}

void
ogg_vorbis_stream_free(OggVorbisStream *stream)
{
    OPENAL_stop_source_for_ogg_vorbis(stream);
    OPENAL_unqueue_source_buffers(stream->source,
      (SoundNameCallback) push_free_buffer, stream);

    OPENAL_free_buffers_for_ogg_vorbis(stream);

    OPENAL_free_source_for_ogg_vorbis(stream);

    if (stream->file_name != NULL)
    {
        LbMemoryFree(stream->file_name);
        ov_clear(&stream->file);
    }

    memset(stream, 0, sizeof (*stream));
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
ogg_vorbis_stream_clear(OggVorbisStream *stream)
{
    if (stream->file_name == NULL)
        return;

    ogg_vorbis_stream_stop (stream);
    ov_clear (&stream->file);
    LbMemoryFree(stream->file_name);
    stream->file_name = NULL;
}

bool
ogg_vorbis_stream_open(OggVorbisStream *stream, const char *fname)
{
    FILE *f = NULL;
    vorbis_info *info;
    uint32_t sz;

    ogg_vorbis_stream_clear(stream);

    f = fopen (fname, "rb");
    if (f == NULL)
    {
        sprintf(SoundProgressMessage, "%s: Cannot fopen: %s", fname, strerror(errno));
        SoundProgressLog(SoundProgressMessage);
        return false;
    }

    if (ov_open_callbacks (f, &stream->file,
                         NULL, 0, OV_CALLBACKS_DEFAULT) != 0)
    {
        sprintf(SoundProgressMessage, "%s: Invalid Ogg/Vorbis stream.", fname);
        SoundProgressLog(SoundProgressMessage);
        goto err;
    }

    f = NULL;

    info = ov_info (&stream->file, -1);
    if (info == NULL)
    {
        sprintf(SoundProgressMessage, "%s: Failed to read stream information.", fname);
        SoundProgressLog(SoundProgressMessage);
        goto err;
    }

    stream->info = *info;
    sz = strlen(fname) + 1;
    stream->file_name = LbMemoryAlloc(sz);
    if (stream->file_name != NULL)
        LbMemoryCopy(stream->file_name, fname, sz);

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

void
ogg_vorbis_stream_set_gain (OggVorbisStream *stream, float gain)
{
  alSourcef (stream->source, AL_GAIN, gain);
  check_al ("alSourcef (AL_GAIN)");
}

float
ogg_vorbis_stream_get_gain (OggVorbisStream *stream)
{
  ALfloat gain = 0.0f;
  alGetSourcef(stream->source, AL_GAIN, &gain);
  check_al ("alGetSourcef (AL_GAIN)");
  return gain;
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
    OPENAL_unqueue_source_buffers(stream->source,
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
          sprintf(SoundProgressMessage, "Error: Failed to read ogg/vorbis data.");
          SoundProgressLog(SoundProgressMessage);
              stream->playing = false;
              return false;
            }
          else if (count == 0)
            break;

          total += count;
        }

      if (total == 0)
        return ogg_vorbis_stream_restart (stream);

      buf = pop_free_buffer(stream);

      if (stream->info.channels == 2)
        format = AL_FORMAT_STEREO16;
      else
        format = AL_FORMAT_MONO16;

      alBufferData (buf, format, buffer, total, stream->info.rate);
      if (!check_al ("alBufferData"))
        {
          push_free_buffer(buf, stream);
          return false;
        }

      alSourceQueueBuffers (stream->source , 1, &buf);
      if (!check_al ("alSourceQueueBuffers"))
        {
          push_free_buffer(buf, stream);
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

/******************************************************************************/
