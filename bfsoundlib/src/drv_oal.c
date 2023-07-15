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
static ALCdevice *oal_sound_device = NULL;


/******************************************************************************/
#define check_alc(source) check_alc_line((source), __LINE__)
#define check_al(source) check_al_line((source), __LINE__)

bool check_alc_line(const char *source, int line)
{
    char msg[80];
    ALCenum err;

    assert (oal_sound_device != NULL);

    err = alcGetError(oal_sound_device);
    if (err != ALC_NO_ERROR) {
        snprintf(msg, sizeof(msg), "%s: Error code 0x%x at "__FILE__":%i.\n",
            source, err, line);
        AIL_set_error(msg);
        return false;
    }

    return true;
}

bool check_al_line(const char *source, int line)
{
    char msg[80];
    ALenum err;

    err = alGetError();
    if (err != ALC_NO_ERROR) {
        snprintf(msg, sizeof(msg), "%s: Error code 0x%x at "__FILE__":%i.\n",
            source, err, line);
        AIL_set_error(msg);
        return false;
    }

    return true;
}

int32_t OPENAL_startup(void)
{
    ALCcontext *sound_context;

    sound_context = NULL;
    oal_sound_device = alcOpenDevice(NULL);
    if (oal_sound_device == NULL) {
        AIL_set_error("alcOpenDevice: Failed to open default OpenAL device.");
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
            AIL_set_error("alGenSources: Samples count truncated by OpenAL.");
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
            return removed;

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
    assert(stream->buffer_count == SOUND_MUSIC_BUFFERS);
    alDeleteBuffers(stream->buffer_count, stream->buffers);
    check_al("alDeleteBuffers");
    return 1;
}

int32_t OPENAL_stop_source_for_ogg_vorbis(OggVorbisStream *stream)
{
    alSourceStop(stream->source);
    if (!check_al("alSourceStop"))
        return 0;
    return 1;
}

/******************************************************************************/
