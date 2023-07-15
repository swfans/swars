/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file drv_oal.h
 *     Header file for drv_oal.c.
 * @par Purpose:
 *     OpenAL specific functions.
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
#ifndef DRV_OAL_H_
#define DRV_OAL_H_

#include "mssal.h"
#include "oggvorbis.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
typedef void (*SoundNameCallback)(ALuint name, void *user_data);

/******************************************************************************/
int32_t OPENAL_startup(void);
int32_t OPENAL_shutdown(void);

int oal_sound_initialise(void);
void oal_sound_finalise(void);
int32_t OPENAL_create_sources_for_samples(DIG_DRIVER *digdrv);
int32_t OPENAL_free_sources_for_samples(DIG_DRIVER *digdrv);

/** Create source required for playback with the Ogg/Vorbis stream.
 */
int32_t OPENAL_create_source_for_ogg_vorbis(OggVorbisStream *stream);

/** Delete the source used for playback with the Ogg/Vorbis stream.
 */
int32_t OPENAL_free_source_for_ogg_vorbis(OggVorbisStream *stream);

/** Create buffers required for playback with the Ogg/Vorbis stream.
 *
 * Buffers are the data chunks used by the source while playing.
 */
int32_t OPENAL_create_buffers_for_ogg_vorbis(OggVorbisStream *stream);

/** Free all buffers within the Ogg/Vorbis stream.
 */
int32_t OPENAL_free_buffers_for_ogg_vorbis(OggVorbisStream *stream);

/** Unqueue all buffers from given source, callback for each.
 *
 * All buffers are unqueued. Currently playing buffers may remain,
 * so it is best to stop the stream first.
 */
int32_t OPENAL_unqueue_source_buffers(ALuint source,
    SoundNameCallback callback, void *user_data);

/** Stop playback on the Ogg/Vorbis stream.
 *
 * Any queued buffers will wait for the stream to start playing again.
 */
int32_t OPENAL_stop_source_for_ogg_vorbis(OggVorbisStream *stream);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // DRV_OAL_H_
