/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ailss.h
 *     Header file for ailss.c.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef AIL2OAL_AILSS_H_
#define AIL2OAL_AILSS_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

void SS_build_amplitude_tables(SNDSAMPLE *s);

/** Flush mixer buffer.
 *
 * Also called AILSSA_flush_buffer().
 */
void SS_flush(DIG_DRIVER *digdrv);

/** Copy mixer buffer to DMA buffer.
 *
 * Also called AILSSA_DMA_copy(), though that one has different params.
 */
void SS_copy(DIG_DRIVER *digdrv, void *pWaveAddr);

/** Configure DMA half-buffers.
 *
 * @return Givess 1 if configuration valid, else 0.
 */
int32_t SS_configure_buffers(DIG_DRIVER *digdrv);

/** Copy data from source sample to build buffer, with mixing and ASI
 * decompression.
 *
 * Or provide th data to external library, if that is what
 * the current platform requires.
 *
 * Also named AILSSA_merge(), depending on lib version.
 */
void SS_stream_to_buffer(SNDSAMPLE *s);


SNDSAMPLE *AIL2OAL_API_allocate_sample_handle(DIG_DRIVER *digdrv);

void AIL2OAL_API_set_digital_master_volume(DIG_DRIVER *digdrv, int32_t master_volume);

void AIL2OAL_API_init_sample(SNDSAMPLE *s);

uint32_t AIL2OAL_API_sample_status(SNDSAMPLE *s);

int32_t AIL2OAL_API_sample_buffer_ready(SNDSAMPLE *s);

int32_t AIL2OAL_API_set_sample_file(SNDSAMPLE *s, const void *file_image, int32_t block);

AILSAMPLECB AIL2OAL_API_register_EOS_callback(SNDSAMPLE *s, AILSAMPLECB EOS);

HAILPROVIDER AIL2OAL_API_set_sample_processor(SNDSAMPLE *s,
    uint32_t pipeline_stage, HAILPROVIDER provider);

void AIL2OAL_API_set_sample_address(SNDSAMPLE *s, const void *start, uint32_t len);

int32_t AIL2OAL_API_sample_playback_rate(SNDSAMPLE *s);

void AIL2OAL_API_set_sample_playback_rate(SNDSAMPLE *s, int32_t playback_rate);

int32_t AIL2OAL_API_sample_volume(SNDSAMPLE *s);

void AIL2OAL_API_set_sample_volume(SNDSAMPLE *s, int32_t level);

void AIL2OAL_API_set_sample_type(SNDSAMPLE *s, int32_t format, uint32_t flags);

void AIL2OAL_API_set_sample_user_data(SNDSAMPLE *s, uint32_t index, intptr_t value);

void AIL2OAL_API_set_sample_loop_count(SNDSAMPLE *s, int32_t loop_count);

void AIL2OAL_API_start_sample(SNDSAMPLE *s);

void AIL2OAL_API_end_sample(SNDSAMPLE *s);

void AIL2OAL_API_release_sample_handle(SNDSAMPLE *s);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AILSS_H_
