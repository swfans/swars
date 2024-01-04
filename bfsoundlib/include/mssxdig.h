/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssxdig.h
 *     Header file for mssxdig.c.
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
#ifndef AIL2OAL_MSSXDIG_H_
#define AIL2OAL_MSSXDIG_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

/** Max virtual wave synth voice cnt */
#define MAX_W_VOICES 16

typedef struct WAVE_ENTRY WAVE_ENTRY;

/** Wave synth library entry.
 */
struct WAVE_ENTRY {
    int32_t bank;                      /**< XMIDI bank, MIDI patch for sample */
    int32_t patch;
    int32_t root_key;                  /**< Root MIDI note # for sample (or -1) */
    uint32_t file_offset;              /**< Offset of wave data from start-of-file */
    uint32_t size;                     /**< Size of wave sample in bytes */
    int32_t format;                    /**< DIG_F format (8/16 bits, mono/stereo) */
    uint32_t flags;                    /**< DIG_PCM_SIGN / DIG_PCM_ORDER (stereo) */
    int32_t playback_rate;             /**< Playback rate in hertz */
};

/** Virtual "wave synthesizer" descriptor.
 */
struct WAVE_SYNTH {
    MDI_DRIVER *mdidrv;                /**< MIDI driver for use with synthesizer */
    DIG_DRIVER *digdrv;                /**< Digital driver for use with synthesizer */
    WAVE_ENTRY *library;               /**< Pointer to wave library */
    AILEVENTCB  prev_event_fn;         /**< Previous MIDI event trap function */
    AILTIMBRECB prev_timb_fn;          /**< Previous timbre request trap function */
    MDI_CTRL_LOG controls;             /**< MIDI controller states */
    WAVE_ENTRY *wave[AIL_NUM_CHANS];   /**< Pointer to WAVE_ENTRY for each channel */
    SNDSAMPLE *s[MAX_W_VOICES];        /**< List of HSAMPLE voices */
    int32_t n_voices;                  /**< Actual # of voices allocated to synth */
    int32_t chan[MAX_W_VOICES];        /**< MIDI channel for each voice, or -1 */
    int32_t note[MAX_W_VOICES];        /**< MIDI note number for voice */
    int32_t root[MAX_W_VOICES];        /**< MIDI root note for voice */
    int32_t rate[MAX_W_VOICES];        /**< Playback rate for voice */
    int32_t vel[MAX_W_VOICES];         /**< MIDI note velocity for voice */
    uint32_t time[MAX_W_VOICES];       /**< Timestamp for voice */
    uint32_t event;                    /**< Event counter for LRU timestamps */
};


WAVE_SYNTH *AIL2OAL_API_create_wave_synthesizer(DIG_DRIVER *digdrv,
  MDI_DRIVER *mdidrv, void const *wave_lib, int32_t polyphony);

void AIL2OAL_API_destroy_wave_synthesizer(WAVE_SYNTH *ws);
/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSXDIG_H_
