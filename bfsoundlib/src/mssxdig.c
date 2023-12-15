/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssdig.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     XDig functions from MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 11 Dec 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <limits.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "mssxdig.h"
#include "mssxmidi.h"
#include "memfile.h"
#include "miscutil.h"
#include "ail.h"
#include "aildebug.h"
/******************************************************************************/

// TODO calls below are disabled - non-remade functions
#define XDIG_set_pitch(ws, voice) while(0)
#define XDIG_set_volume(ws, voice) while(0)
#define WVL_search(lib, bank, dt) 0

/** MIDI event interpreter callback function.
 * Used to create wave synthesiser.
 *
 * @return Gives 1 if event handled by wave interpreter, or 0 to pass event
 *   to XMIDI driver.
 */
int32_t XDIG_MIDI_trap(MDI_DRIVER *mdidrv, SNDSEQUENCE *seq,
  int32_t status, int32_t data_1, int32_t data_2)
{
    int32_t st, ch;
    int32_t i, voice;
    uint32_t t;
    WAVE_SYNTH *ws;

    ws = (WAVE_SYNTH*) mdidrv->system_data[0];

    // Get physical MIDI channel and status byte for event
    st = status & 0xf0;
    ch = status & 0x0f;

    // Update MIDI status log
    if ((st == MDI_EV_CONTROL) || (st == MDI_EV_PROGRAM)
        || (st == MDI_EV_PITCH))
    {
        XMI_write_log(&ws->controls, status, data_1, data_2);
    }

    // Process event
    if ((st == MDI_EV_NOTE_ON) && (data_2 == 0))
    {
        st = MDI_EV_NOTE_OFF;
    }

    switch (st)
    {
    // Process MIDI Control Change events
    case MDI_EV_CONTROL:
        switch (data_1)
        {
        // Volume / panpot / expression controllers
        case MDI_CTR_PART_VOLUME:
        case MDI_CTR_EXPRESSION:
        case MDI_CTR_PANPOT:
            for (voice = 0; voice < ws->n_voices; voice++)
            {
                if ((ws->chan[voice] == ch)
                  && (AIL_sample_status(ws->s[voice]) == SNDSMP_PLAYING))
                    XDIG_set_volume(ws, voice);
            }
            break;

        // Adjust pitch bend range
        case MDI_CTR_DATA_MSB:
            for (voice = 0; voice < ws->n_voices; voice++)
            {
                if ((ws->chan[voice] == ch)
                  && (AIL_sample_status(ws->s[voice]) == SNDSMP_PLAYING))
                    XDIG_set_pitch(ws, voice);
            }
            break;

        // All Notes Off message
        case MDI_CTR_ALL_NOTES_OFF:
            for (voice = 0; voice < ws->n_voices; voice++)
            {
                if ((ws->chan[voice] == ch)
                  && (AIL_sample_status(ws->s[voice]) == SNDSMP_PLAYING))
                    AIL_end_sample(ws->s[voice]);
            }
            break;
        }
        break;

    // Process MIDI Program Change events
    case MDI_EV_PROGRAM:
        // Get pointer to wave library entry for this patch #
        ws->wave[ch] = WVL_search(ws->library, ws->controls.bank[ch], data_1);
        break;

    // Process MIDI Pitch Bend events
    case MDI_EV_PITCH:
        for (voice = 0; voice < ws->n_voices; voice++)
        {
            if ((ws->chan[voice] == ch)
              && (AIL_sample_status(ws->s[voice]) == SNDSMP_PLAYING))
                XDIG_set_pitch(ws, voice);
        }
        break;

    // Process MIDI Note On events
    case MDI_EV_NOTE_ON:
        // If percussion channel (10), select wave library entry based on
        // key #, rather than patch #
        if (ch == AIL_PERCUSS_CHAN)
            ws->wave[ch] = WVL_search(ws->library, 127, data_1);
        // Pass event to XMIDI driver if wave library entry not found
        if (ws->wave[ch] == NULL)
            break;
        // Find an available sample to play note
        for (voice = 0; voice < ws->n_voices; voice++)
        {
            if (AIL_sample_status(ws->s[voice]) == SNDSMP_DONE)
                break;
        }

        // If no samples available, steal least-recently-triggered
        // sample for this note
        if (voice == ws->n_voices)
        {
            t = ULONG_MAX;
            for (i = 0; i < ws->n_voices; i++)
            {
                if (ws->time[i] <= t) {
                    t = ws->time[i];
                    voice = i;
                }
            }

            AIL_end_sample(ws->s[voice]);
        }

        // Record channel, note number, velocity, and LRU timestamp
        ws->chan[voice] = ch;
        ws->note[voice] = data_1;
        ws->root[voice] = ws->wave[ch]->root_key;
        ws->rate[voice] = ws->wave[ch]->playback_rate;
        ws->vel[voice] = data_2;
        ws->time[voice] = ws->event++;

        // Initialize sample
        AIL_init_sample(ws->s[voice]);

        AIL_set_sample_type(ws->s[voice], ws->wave[ch]->format,
            ws->wave[ch]->flags);

        AIL_set_sample_address(ws->s[voice],
            AIL_ptr_add(ws->library, ws->wave[ch]->file_offset),
            ws->wave[ch]->size);


        XDIG_set_pitch(ws, voice);
        XDIG_set_volume(ws, voice);

        // Finally, activate sample
        AIL_start_sample(ws->s[voice]);

        // Return without passing note event to XMIDI driver
        return 1;

    // Process MIDI Note Off events (melodic voices only)
    case MDI_EV_NOTE_OFF:
        if (ch == AIL_PERCUSS_CHAN)
            break;
        for (voice = 0; voice < ws->n_voices; voice++)
        {
            if ((ws->chan[voice] == ch) && (ws->note[voice] == data_1)
              && (AIL_sample_status(ws->s[voice]) == SNDSMP_PLAYING)) {
                AIL_end_sample(ws->s[voice]);
                return 1;
            }
        }
        break;
    }

    // Pass all but note on/note off messages through to driver,
    // to keep device's MIDI state up to date
    return 0;
}

WAVE_SYNTH *AIL2OAL_API_create_wave_synthesizer(DIG_DRIVER *digdrv,
  MDI_DRIVER *mdidrv, void const *wave_lib, int32_t polyphony)
{
    // TODO - function not remade
    return NULL;
}

void AIL2OAL_API_destroy_wave_synthesizer(WAVE_SYNTH *ws)
{
    int32_t i;

    if (ws == NULL)
        return;

    // Release allocated sample handles
    for (i=0; i < ws->n_voices; i++) {
        AIL_release_sample_handle(ws->s[i]);
    }

    // Restore previous trap callback functions
    AIL_register_event_callback(ws->mdidrv, ws->prev_event_fn);
    AIL_register_timbre_callback(ws->mdidrv, ws->prev_timb_fn);

    AIL_MEM_free_lock(ws, sizeof(WAVE_SYNTH));
}

/******************************************************************************/
