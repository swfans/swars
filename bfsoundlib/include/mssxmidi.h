/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssxmidi.h
 *     Header file for mssxmidi.c.
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
#ifndef AIL2OAL_MSSXMIDI_H_
#define AIL2OAL_MSSXMIDI_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

// The support is not fully implemented in this library
#define ENABLE_TIMBRE 0

void AIL2OAL_API_set_GTL_filename_prefix(char const *prefix);

int32_t AIL2OAL_API_MDI_driver_type(MDI_DRIVER *mdidrv);

void AIL2OAL_API_set_XMIDI_master_volume(MDI_DRIVER *mdidrv, int32_t master_volume);

int32_t AIL2OAL_API_install_MDI_INI(MDI_DRIVER **mdidrv);

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(const char *filename, SNDCARD_IO_PARMS *iop);
void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv);

MDI_DRIVER *AIL2OAL_API_open_XMIDI_driver(uint32_t flags);
void AIL2OAL_API_close_XMIDI_driver(MDI_DRIVER *mdidrv);

SNDSEQUENCE *AIL2OAL_API_allocate_sequence_handle(MDI_DRIVER *mdidrv);

void AIL2OAL_API_release_sequence_handle(SNDSEQUENCE *seq);

int32_t AIL2OAL_API_init_sequence(SNDSEQUENCE *seq, const void *start,  int32_t sequence_num);

void AIL2OAL_API_start_sequence(SNDSEQUENCE *seq);

void AIL2OAL_API_stop_sequence(SNDSEQUENCE *seq);

void AIL2OAL_API_resume_sequence(SNDSEQUENCE *seq);

void AIL2OAL_API_end_sequence(SNDSEQUENCE *seq);

void AIL2OAL_API_set_sequence_tempo(SNDSEQUENCE *seq, int32_t tempo, int32_t milliseconds);

void AIL2OAL_API_set_sequence_volume(SNDSEQUENCE *seq, int32_t volume, int32_t milliseconds);

void AIL2OAL_API_sequence_position(SNDSEQUENCE *seq, int32_t *beat, int32_t *measure);

uint32_t AIL2OAL_API_sequence_status(SNDSEQUENCE *seq);

int32_t AIL2OAL_API_sequence_loop_count(SNDSEQUENCE *seq);

void AIL2OAL_API_branch_index(SNDSEQUENCE *seq, uint32_t marker);

AILTRIGGERCB AIL2OAL_API_register_trigger_callback(SNDSEQUENCE *seq, AILTRIGGERCB callback);
AILSEQUENCECB AIL2OAL_API_register_sequence_callback(SNDSEQUENCE *seq, AILSEQUENCECB callback);
AILBEATCB AIL2OAL_API_register_beat_callback(SNDSEQUENCE *seq, AILBEATCB callback);
AILEVENTCB AIL2OAL_API_register_event_callback(MDI_DRIVER *mdidrv, AILEVENTCB callback);

void AIL2OAL_API_set_sequence_user_data(SNDSEQUENCE *seq, uint32_t index, intptr_t value);
intptr_t AIL2OAL_API_sequence_user_data(SNDSEQUENCE *seq, uint32_t index);

void AIL2OAL_API_map_sequence_channel(SNDSEQUENCE *seq, int32_t seq_channel, int32_t new_channel);

void AIL2OAL_API_send_channel_voice_message(MDI_DRIVER *mdidrv, SNDSEQUENCE *seq,
        int32_t status, int32_t data_1, int32_t data_2);

int32_t AIL2OAL_API_lock_channel(MDI_DRIVER *mdidrv);

void AIL2OAL_API_release_channel(MDI_DRIVER *mdidrv, int32_t channel);

/** Send MIDI channel voice message associated with a specific sequence
 *
 * Includes controller logging and XMIDI extensions.
 *
 * Warnings: ICA_enable should be 0 when calling outside XMIDI event loop
 * May be recursively called by XMIDI controller handlers.
 */
void XMI_send_channel_voice_message(SNDSEQUENCE *seq, int32_t status,
        int32_t data_1, int32_t data_2, int32_t ICA_enable);

/** Timer interrupt routine for XMIDI sequencing.
 *
 * Has anything to do with interrupts and DMA channels only on bare metal systems
 * like DOS. On modern OSes, its periodical calls just transfer data buffers
 * to a sound interface like OpenAL.
 */
void XMI_serve(void *clientval);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSXMIDI_H_
