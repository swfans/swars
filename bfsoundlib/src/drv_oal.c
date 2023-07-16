/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file drv_oal.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     OpenAL output support.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Jun 2023 - 15 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "drv_oal.h"
#include "aildebug.h"
#include OPENAL_ALC_H
#include OPENAL_AL_H
#include "oggvorbis.h"
/******************************************************************************/
#define SOUND_MAX_SOURCES 64
#define SOUND_BUFFERS_PER_SRC 3
#define SOUND_MAX_BUFFERS (SOUND_MAX_SOURCES * SOUND_BUFFERS_PER_SRC)
#define SOUND_MAX_BUFSIZE 2048

static ALCdevice *oal_sound_device = NULL;

size_t sound_free_buffer_count = 0;
static ALuint sound_free_buffers[SOUND_MAX_BUFFERS];
/******************************************************************************/
#define check_alc(source) check_alc_line((source), __LINE__)
#define check_al(source) check_al_line((source), __LINE__)

bool check_alc_line(const char *source, int line)
{
    char msg[120];
    ALCenum err;

    assert (oal_sound_device != NULL);

    err = alcGetError(oal_sound_device);
    if (err != ALC_NO_ERROR) {
        snprintf(msg, sizeof(msg), "%s: Error code 0x%x at "__FILE__":%i",
            source, err, line);
        AIL_set_error(msg);
        return false;
    }

    return true;
}

bool check_al_line(const char *source, int line)
{
    char msg[120];
    ALenum err;

    err = alGetError();
    if (err != ALC_NO_ERROR) {
        snprintf(msg, sizeof(msg), "%s: Error code 0x%x at "__FILE__":%i",
            source, err, line);
        AIL_set_error(msg);
        return false;
    }

    return true;
}

static ALuint pop_free_buffer(void)
{
    assert(sound_free_buffer_count > 0);
    return sound_free_buffers[--sound_free_buffer_count];
}

static void push_free_buffer(ALuint buf, void *user_data)
{
    assert(sound_free_buffer_count < SOUND_MAX_BUFFERS);
    sound_free_buffers[sound_free_buffer_count++] = buf;
}

int32_t OPENAL_startup(void)
{
    ALCcontext *sound_context;

    sound_context = NULL;
    oal_sound_device = alcOpenDevice(NULL);
    if (oal_sound_device == NULL) {
        AIL_set_error("alcOpenDevice: Failed to open default OpenAL device");
        goto err;
    }

    sound_context = alcCreateContext(oal_sound_device, NULL);
    if (!check_alc("alcCreateContext"))
        goto err;

    alcMakeContextCurrent(sound_context);
    if (!check_alc("alcMakeContextCurrent"))
        goto err;

    return 1;

err:
    if (sound_context) {
        alcDestroyContext(sound_context);
        check_alc("alcDestroyContext");
    }
    if (oal_sound_device) {
        alcCloseDevice(oal_sound_device);
        oal_sound_device = NULL;
    }
    return 0;
}

int32_t OPENAL_shutdown(void)
{
    alcDestroyContext(alcGetCurrentContext());
    check_alc("alcDestroyContext");
    alcCloseDevice(oal_sound_device);
    return 1;
}

int32_t OPENAL_create_sources_for_samples(DIG_DRIVER *digdrv)
{
    int32_t i;

    for (i = 0; i < digdrv->n_samples; i++)
    {
        SNDSAMPLE *s = &digdrv->samples[i];
        ALuint source;

        alGenSources(1, &source);
        if (alGetError() != AL_NO_ERROR) {
            AIL_set_error("alGenSources: Samples count truncated by OpenAL");
            digdrv->n_samples = i;
            break;
        }
        s->system_data[5] = source;
        s->system_data[4] = 0; // buffers used
    }
    return (digdrv->n_samples > 0);
}

int32_t OPENAL_free_sources_for_samples(DIG_DRIVER *digdrv)
{
    int32_t i;

    for (i = 0; i < digdrv->n_samples; i++)
    {
        SNDSAMPLE *s = &digdrv->samples[i];
        ALuint source;

        source = s->system_data[5];
        alDeleteSources(1, &source);
        check_al("alDeleteSources");
        s->system_data[5] = source;
    }
    return 1;
}

int32_t OPENAL_create_buffers_for_samples(DIG_DRIVER *digdrv)
{
    alGenBuffers(SOUND_MAX_BUFFERS, &sound_free_buffers[0]);
    if (!check_al("alGenBuffers")) {
        goto err;
    }
    sound_free_buffer_count = SOUND_MAX_BUFFERS;
    {
        char msg[120];
        snprintf(msg, sizeof(msg), "%s: Created %zu sound buffers",
            __func__, sound_free_buffer_count);
        AIL_set_error(msg);
    }
    return 1;

err:
    return 0;
}

int32_t OPENAL_free_buffers_for_samples(DIG_DRIVER *digdrv)
{
    size_t n_buffers;
    n_buffers = sound_free_buffer_count;
    sound_free_buffer_count = 0;
    alDeleteBuffers(n_buffers, sound_free_buffers);
    check_al ("alDeleteBuffers");
    return 1;
}

int32_t OPENAL_unqueue_source_buffers(ALuint source,
    SoundNameCallback callback, void *user_data)
{
    ALint count;
    ALuint buf;
    size_t removed = 0;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &count);
    if (!check_al("alGetSourcei (AL_BUFFERS_PROCESSED)"))
        return 0;

    while (count-- > 0)
    {
        alSourceUnqueueBuffers (source, 1, &buf);
        if (!check_al ("alSourceUnqueueBuffers"))
            continue;

        if (callback != NULL)
            callback (buf, user_data);

      removed++;
    }

  return removed;
}

int32_t OPENAL_create_source_for_ogg_vorbis(OggVorbisStream *stream)
{
    alGenSources(1, &stream->source);
    if (!check_al("alGenSources"))
        return 0;
    return 1;
}

int32_t OPENAL_free_source_for_ogg_vorbis(OggVorbisStream *stream)
{
    alDeleteSources(1, &stream->source);
    if (!check_al("alDeleteSources"))
        return 0;
    return 1;
}

int32_t OPENAL_create_buffers_for_ogg_vorbis(OggVorbisStream *stream)
{
    alGenBuffers(SOUND_MUSIC_BUFFERS, stream->buffers);
    if (!check_al("alGenBuffers"))
        return 0;

    stream->buffer_count = SOUND_MUSIC_BUFFERS;
    return 1;
}

int32_t OPENAL_free_buffers_for_ogg_vorbis(OggVorbisStream *stream)
{
    size_t n_buffers;
    n_buffers = stream->buffer_count;
    stream->buffer_count = 0;
    alDeleteBuffers(n_buffers, stream->buffers);
    check_al("alDeleteBuffers");
    if (n_buffers != SOUND_MUSIC_BUFFERS) {
        char msg[120];
        snprintf(msg, sizeof(msg), "%s: From %d buffers, %d escaped deletion",
            __func__, SOUND_MUSIC_BUFFERS, SOUND_MUSIC_BUFFERS - n_buffers);
        AIL_set_error(msg);
    }
    return 1;
}

int32_t OPENAL_stop_source_for_ogg_vorbis(OggVorbisStream *stream)
{
    if (stream->playing)
        stream->playing = false;
    alSourceStop(stream->source);
    if (!check_al("alSourceStop"))
        return 0;
    // Detach all buffers, so that they all join the PROCESSED list
    alSourcei(stream->source, AL_BUFFER, 0);
    if (!check_al("alSourcei (AL_BUFFER,0)"))
        return 0;
    return 1;
}

static ALenum
get_pcm_format(SNDSAMPLE *s)
{
    switch (s->format)
    {
    case DIG_F_MONO_8:
        return AL_FORMAT_MONO8;
    case DIG_F_MONO_16:
        return AL_FORMAT_MONO16;
    case DIG_F_STEREO_8:
        return AL_FORMAT_STEREO8;
    case DIG_F_STEREO_16:
        return AL_FORMAT_STEREO16;
    case DIG_F_MULTICHANNEL_8:
    case DIG_F_MULTICHANNEL_16:
    default:
        abort();
    }
}

static void
queue_source_buffers(DIG_DRIVER *digdrv, SNDSAMPLE *s)
{
    size_t len, total_len;
    size_t buffers_used;
    ALuint source;
    void *data;
    float x_pos;
    ALint state;
    ALuint buf = 0;

    source = s->system_data[5];
    buffers_used = s->system_data[4];

    if (buffers_used >= SOUND_BUFFERS_PER_SRC)
        return;

    total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];

    if (total_len == 0)
    {
        if (s->done[s->current_buffer ^ 1] == 0)
            s->current_buffer ^= 1;
        else
            return;

        total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];
    }


    while (total_len > 0 && buffers_used < SOUND_BUFFERS_PER_SRC)
    {
        data = s->start[s->current_buffer] + s->pos[s->current_buffer];
        len = total_len;
        if (len > SOUND_MAX_BUFSIZE)
            len = SOUND_MAX_BUFSIZE;

        assert ((s->flags & 1) != 0);

        buf = pop_free_buffer ();
        alBufferData (buf, get_pcm_format (s), data, len, s->playback_rate);
        if (!check_al ("alBufferData"))
            goto err;

        alSourceQueueBuffers(source, 1, &buf);
        if (!check_al ("alSourceQueueBuffers"))
            goto err;

        buffers_used++;

        alGetSourcei (source, AL_SOURCE_STATE, &state);
        if (!check_al ("alGetSourcei (AL_SOURCE_STATE)"))
            goto err;

        alSourcef (source, AL_GAIN,
             (s->volume * (1.f / 127.f)
              * (digdrv->master_volume * (1.f / 127.f))));
        if (!check_al ("alSourcef (AL_GAIN)"))
            goto err;

        /* XXX: check if panning/position is OK */
        if (s->pan == 0)
            x_pos = 0.f;
        else
            x_pos = (127 - s->pan - 64) * (1.f / 64.f);

        alSource3f (source, AL_POSITION, x_pos, 0.f, -.25f);
        if (!check_al ("alSource3f (AL_POSITION)"))
            goto err;

        if (state != AL_PLAYING)
        {
            alSourcePlay (source);
            if (!check_al ("alSourcePlay"))
                goto err;
        }

        s->pos[s->current_buffer] += len;
        total_len -= len;

        if (total_len == 0 && s->loop_count == 0) {
            s->pos[s->current_buffer] = 0;
            total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];
        }
    }

    s->system_data[4] = buffers_used;
    return;

err:
    s->system_data[4] = buffers_used;
    if (buf != 0)
        push_free_buffer (buf, NULL);
}

static void
unqueue_source_buffers(SNDSAMPLE *s)
{
    size_t buffers_used;
    ALuint source;

    source = s->system_data[5];
    buffers_used = s->system_data[4];

    if (buffers_used == 0)
        return;

    buffers_used -=
        OPENAL_unqueue_source_buffers(source,
                   (SoundNameCallback) push_free_buffer,
                   NULL);

    s->system_data[4] = buffers_used;

    if (buffers_used > 0
        || s->pos[s->current_buffer] < s->len[s->current_buffer]
        || s->loop_count == 0)
      return;

    /* this check prevents the sound in the intro from abruptly stopping */
    if (!s->done[0] && !s->done[1])
        return;

    alSourceStop(source);
    check_al("alSourceStop");
    s->status = SNDSMP_DONE;
}

void OPENAL_unqueue_finished_dig_samples(DIG_DRIVER *digdrv)
{
    int32_t i;

    for (i = 0; i < digdrv->n_samples; i++)
    {
        SNDSAMPLE *s;

        s = &digdrv->samples[i];

        if (s->status == SNDSMP_FREE)
            continue;

#if 0 // verbose debug code
        printf ("sample %i loops:%i ([%i/%i] [%i/%i]) (%i %i) %zu\n", n,
              s->loop_count,
              s->pos[0], s->len[0],
              s->pos[1], s->len[1],
              s->done[0], s->done[1],
              buffers_used);
#endif

        unqueue_source_buffers(s);
    }
}

int32_t OPENAL_update_dig_sample(SNDSAMPLE *s)
{
    queue_source_buffers(s->driver, s);
    return (s->status == SNDSMP_DONE) ? 1 : 0;
}

void OPENAL_unqueue_finished_mdi_sequences(MDI_DRIVER *mdidrv)
{
    //TODO
}

int32_t OPENAL_update_mdi_sequence(SNDSEQUENCE *seq)
{
    //TODO
    return (seq->status == SNDSEQ_DONE) ? 1 : 0;
}

/******************************************************************************/
