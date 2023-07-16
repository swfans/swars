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

/** Activates fake timer, as the real AIL timers do not work.
 */
int32_t sound_fake_timer_initialize(void);

/** Creates more OpenAL buffers used for both DIG and MID drivers playback.
 */
int32_t OPENAL_create_buffers(uint32_t n_sources);

/** Frees some or all of OpenAL buffers.
 */
int32_t OPENAL_free_buffers(uint32_t n_sources);

/** Creates OpenAL sources used for DIG driver playback.
 */
int32_t OPENAL_create_sources_for_samples(DIG_DRIVER *digdrv);
int32_t OPENAL_free_sources_for_samples(DIG_DRIVER *digdrv);

/** Creates OpenAL sources used for MIDI driver playback.
 */
int32_t OPENAL_create_sources_for_sequences(MDI_DRIVER *mdidrv);
int32_t OPENAL_free_sources_for_sequences(MDI_DRIVER *mdidrv);

/** Function to be called within a timer while DIG playback is able.
 *
 * This part is to be called before the sample mixing loop.
 */
void OPENAL_unqueue_finished_dig_samples(DIG_DRIVER *digdrv);

/** Function to be called within a timer while DIG playback is able.
 *
 * This part is to be called within the sample mixing loop.
 *
 * @return Gives 1 if the sample is done playing.
 */
int32_t OPENAL_update_dig_sample(SNDSAMPLE *s);

/** Function to be called within a timer while MIDI playback is able.
 *
 * This part is to be called before the sequence mixing loop.
 */
void OPENAL_unqueue_finished_mdi_sequences(MDI_DRIVER *mdidrv);

/** Function to be called within a timer while MIDI playback is able.
 *
 * This part is to be called within the sequence mixing loop.
 *
 * @return Gives 1 if the sequence is done playing.
 */
int32_t OPENAL_update_mdi_sequence(SNDSEQUENCE *seq);

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

/** Unqueue buffers which finished playing on given source, callback for each.
 *
 * Only finished buffers are unqueued. Currently playing and queued buffers
 * may remain, so it is best to stop the stream first.
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
