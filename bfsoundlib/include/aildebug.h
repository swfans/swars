/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file aildebug.h
 *     Header file for aildebug.c.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
 * @par Comment:
 *     Wrappers for MSS API functions, providing debug log capabilities.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef AIL2OAL_AILDEBUG_H_
#define AIL2OAL_AILDEBUG_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
extern uint16_t AIL_debug;
extern uint16_t AIL_sys_debug;
extern FILE *AIL_debugfile;
extern uint32_t AIL_indent;
/******************************************************************************/

/** Initialize MSS and prepares for the installation and use of sound drivers.
 */
int32_t AIL_startup(void);

/** Shuts down all active sound drivers, restore normal system timer operation,
 * and disconnect the API timer interrupt handler.
 */
void AIL_shutdown(void);

/** Set one of internal MSS parameters.
 *
 * @param number Specifies index of the preference to set.
 * @param value The desired preference value.
 */
int32_t AIL_set_preference(uint32_t number, int32_t value);

#define FILE_READ_WITH_SIZE ((void *)(int32_t)-1)

/** Read file into memory at *dest, allocating memory first if dest=NULL.
 */
void *AIL_file_read(const char *fname, void *dest);

/** Returns time since first initialization of the library, in miliseconds.
 */
uint32_t AIL_ms_count(void);

/** Define a filename prefix for application's Global Timbre Library
 * files.
 *
 * Under MS-DOS, prefix may be up to 8 characters long and must not end
 * in a period.
 */
void AIL_set_GTL_filename_prefix(char const *prefix);

/** Set error text representing last error.
 */
void AIL_set_error(const char *error_msg);

AIL_DRIVER *AIL_install_driver(const uint8_t *driver_image, uint32_t n_bytes);

/** Shut down and unload driver from memory; free driver descriptor
 *
 * Note: Not all DPMI hosts reclaim low DOS memory properly when freed.
 * This function never needs to be called by most applications.
 */
void AIL_uninstall_driver(AIL_DRIVER *drvr);

const SNDCARD_IO_PARMS *AIL_get_IO_environment(AIL_DRIVER *drvr);

void AIL_restore_USE16_ISR(int32_t irq);

/** Get a real-mode interrupt vector.
 */
void *AIL_get_real_vect(uint32_t vectnum);

/** Set a real-mode interrupt vector.
 */
void AIL_set_real_vect(uint32_t vectnum, void *real_ptr);

/** Disable hardware interrupts.
 */
uint32_t AIL_disable_interrupts(void);

/** Restore previous interrupt state.
 */
void AIL_restore_interrupts(uint32_t FD_reg);

/** Get value last used by the API to program the PIT chip.
 */
uint32_t AIL_interrupt_divisor(void);

/** Register an application function as a timer callback routine, and
 * obtain a timer handle which may be used to control the timer.
 */
HSNDTIMER AIL_register_timer(AILTIMERCB fn);

/** Release timer handle(s).
 */
void AIL_release_timer_handle(HSNDTIMER timer);

/** Set user data pointer for given timer.
 */
void *AIL_set_timer_user(HSNDTIMER timer, void *user_data);

/** Set master volume for all sequences.
 */
void AIL_set_XMIDI_master_volume(MDI_DRIVER *mdidrv, int32_t master_volume);

/** Lock an unprotected physical channel.
 *
 * @return Gives 0 if lock attempt failed.
 */
int32_t AIL_lock_channel(MDI_DRIVER *mdidrv);

/** Release (unlock) a locked physical channel.
 */
void AIL_release_channel(MDI_DRIVER *mdidrv, int32_t channel);

/** Get status of sample, one of SNDSMP_* enum members.
 */
uint32_t AIL_sample_status(SNDSAMPLE *s);

/** Get double-buffer playback status for sample.
 *
 * @return Gives one of:
 *   0: Buffer 0 is ready to be filled (buffer 1 or neither buffer playing),
 *   1: Buffer 1 is ready to be filled (buffer 0 playing),
 *  -1: Both buffers are already full.
 */
int32_t AIL_sample_buffer_ready(SNDSAMPLE *s);

/** Set parameters of existing HSAMPLE according to file data.
 *
 * Returns 0 on error, else 1.
 */
int32_t AIL_set_sample_file(SNDSAMPLE *s, const void *file_image, int32_t block);

/** Load, install, and initialize digital audio driver according to
 * contents of DIG_INI file.
 */
int32_t AIL_install_DIG_INI(DIG_DRIVER **digdrv);

/** Load, install, and initialize digital audio driver.
 */
DIG_DRIVER *AIL_install_DIG_driver_file(char const *fname, SNDCARD_IO_PARMS *iop);

/** Uninstall digital audio driver.
 */
void AIL_uninstall_DIG_driver(DIG_DRIVER *digdrv);

/** Set master volume for all samples.
 */
void AIL_set_digital_master_volume(DIG_DRIVER *digdrv, int32_t master_volume);

/** Allocate a SNDSAMPLE structure for use with a given driver.
 */
SNDSAMPLE *AIL_allocate_sample_handle(DIG_DRIVER *dig);

/** Initialize a SNDSAMPLE structure to baseline values.
 *
 * Sample must be allocated (not free), done playing, and stopped.
 */
void AIL_init_sample(SNDSAMPLE *s);

/** Set sample user data value at specified index.
 *
 * Any desired value may be stored at one of eight user data words
 * associated with a given SNDSAMPLE.
 *
 * Callback functions may access the user data array at interrupt time.
 */
void AIL_set_sample_user_data(SNDSAMPLE *s, uint32_t index, intptr_t value);

/** Set a RIB provider for a specified sample pipeline phase.
 */
HAILPROVIDER AIL_set_sample_processor(SNDSAMPLE *s,
    uint32_t pipeline_stage, HAILPROVIDER provider);

/** Set starting address and length of sample.
 */
void AIL_set_sample_address(SNDSAMPLE *s, const void *start, uint32_t len);

/** Get sample playback rate in hertz.
 */
int32_t AIL_sample_playback_rate(SNDSAMPLE *s);

/** Set sample playback rate in hertz.
 */
void AIL_set_sample_playback_rate(SNDSAMPLE *s, int32_t playback_rate);

/** Set sample data format and flags.
 *
 * Byte order for multi-byte samples is not changeable, must be little-endian.
 *
 * @param s Target Sound Sample struct instance.
 * @param format Valid format values are DIG_F_* enum members.
 * @param flag Valid flags are DIG_PCM_* enum members.
 */
void AIL_set_sample_type(SNDSAMPLE *s, int32_t format, uint32_t flags);

/** Set sample loop count.
 *
 * @param s Target Sound SAMPLE struct instance.
 * @param loop_count Count values:
 *    1: Single iteration, no looping;
 *    0: Loop indefinitely;
 *    n: Play sample n times.
 */
void AIL_set_sample_loop_count(SNDSAMPLE *s, int32_t loop_count);

/** Start playback of sample from beginning.
 *
 * Sample must first have been initialized with
 * AIL_init_sample() and then AIL_set_sample_address() or
 * AIL_load_sample_buffer().
 *
 * Playback will begin at the next DMA half-buffer transition.
 */
void AIL_start_sample(SNDSAMPLE *s);

/** Terminate playback of sample, setting sample status to SNDSMP_DONE.
 */
void AIL_end_sample(SNDSAMPLE *s);

/** Free a SAMPLE structure for later allocation.
 */
void AIL_release_sample_handle(SNDSAMPLE *s);

/** Initialize a SEQUENCE structure to prepare for playback of desired
 * XMIDI sequence file image.
 *
 * Should not be called from callback function.
 *
 * @param seq Target Sound SEQUENCE struct instance.
 *   Sequence has to be allocated (not free), done playing, and stopped.
 * @param start Source file image buffer.
 * @param sequence_num
 *
 * @result Gives 0 if sequence initialization failed,
 *        -1 if initialized OK but timbre was missing,
 *         1 if initialization and timbre-loading successful.
 */
int32_t AIL_init_sequence(SNDSEQUENCE *seq, const void *start,  int32_t sequence_num);

/** Stop playback of sequence.
 *
 * Sequence playback may be resumed with AIL_resume_sequence(), or
 * restarted from the beginning with AIL_start_sequence().
 */
void AIL_stop_sequence(SNDSEQUENCE *seq);

/** Resume playback of previously stopped sequence.
 */
void AIL_resume_sequence(SNDSEQUENCE *seq);

/** Terminate playback of sequence, setting sequence status to SEQ_DONE.
 */
void AIL_end_sequence(SNDSEQUENCE *seq);

/** Set relative tempo percentage for sequence, 0-100+ %.
 */
void AIL_set_sequence_tempo(SNDSEQUENCE *seq, int32_t tempo, int32_t milliseconds);

/** Set volume scaling factor for all channels in sequence, 0-127.
 *
 * Values above 127 cause "compression" effect.
 */
void AIL_set_sequence_volume(SNDSEQUENCE *seq, int32_t volume, int32_t milliseconds);

/** Report relative beat and measure count for current XMIDI sequence.
 */
void AIL_sequence_position(SNDSEQUENCE *seq, int32_t *beat, int32_t *measure);

/** Get status of sequence.
 */
uint32_t AIL_sequence_status(SNDSEQUENCE *seq);

/** Get number of sequence loops remaining.
 *
 * @return Values:
 *    1 indicates that the sequence is on its last iteration,
 *    0 indicates that the sequence is looping indefinitely.
 */
int32_t AIL_sequence_loop_count(SNDSEQUENCE *seq);

/** System-independent delay in 1/60 second intervals.
 *
 * Returns at once if called from background.
 */
void AIL_delay(int32_t intervals);

/** Returns TRUE if called from within timer handler or callback function.
 */
int32_t AIL_background(void);

/** Sets period between timer triggers, providing frequency.
 */
void AIL_set_timer_frequency(HSNDTIMER timer, uint32_t hertz);

/** Sets period between timer triggers, in microseconds.
 */
void AIL_set_timer_period(HSNDTIMER timer, uint32_t usec);

/** Start given timer.
 */
void AIL_start_timer(HSNDTIMER timer);

/** Start all configured timers.
 */
void AIL_start_all_timers(void);

/** Stop timer(s).
 */
void AIL_stop_timer(HSNDTIMER timer);

void AIL_release_all_timers(void);

/** Set end-of-sample (EOS) callback function for sample.
 *
 * Allows to provide a callback function which will be invoked
 * when all valid sample buffers have finished playing.
 *
 * When not double-buffering, the end-of-sample callback will be
 * triggered immediately after the end of buffer 0.
 *
 * This function returns the sample's previous EOS callback handler
 * address, or NULL if no EOS callback handler was registered.
 */
AILSAMPLECB AIL_register_EOS_callback(SNDSAMPLE *s, AILSAMPLECB EOS);

int32_t AIL_call_driver(AIL_DRIVER *drvr, int32_t fn,
        VDI_CALL *in, VDI_CALL *out);

/** Allocate a SNDSEQUENCE structure for use with a given driver.
 */
SNDSEQUENCE *AIL_allocate_sequence_handle(MDI_DRIVER *mdidrv);

/** Free a SNDSEQUENCE structure for later allocation.
 */
void AIL_release_sequence_handle(SNDSEQUENCE *seq);

/** Start playback of sequence from beginning.
 *
 * At a minimum, sequence must first have been initialized with a prior
 * call to AIL_init_sequence().
 */
void AIL_start_sequence(SNDSEQUENCE *seq);

/** Repors the MIDI synthesis technology available with a specified driver.
 */
int32_t AIL_MDI_driver_type(MDI_DRIVER *mdidrv);

/** Map sequence's logical channel to desired physical output channel.
 */
void AIL_map_sequence_channel(SNDSEQUENCE *seq, int32_t seq_channel, int32_t new_channel);

/** Transmit MIDI channel voice message via desired physical channel.
 *
 * This function disregards channel locking and other XMIDI features.
 */
void AIL_send_channel_voice_message(MDI_DRIVER *mdidrv, SNDSEQUENCE *seq,
        int32_t status, int32_t data_1, int32_t data_2);

/** Branch immediately to specified Sequence Branch Index point.
 */
void AIL_branch_index(SNDSEQUENCE *seq, uint32_t marker);

/** Install function handler for XMIDI Callback Trigger events.
 */
AILTRIGGERCB AIL_register_trigger_callback(SNDSEQUENCE *seq, AILTRIGGERCB callback);

/** Install function handler for end-of-sequence callbacks.
 */
AILSEQUENCECB AIL_register_sequence_callback(SNDSEQUENCE *seq, AILSEQUENCECB callback);

/** Install callback function handler for XMIDI beat/bar change events.
 */
AILBEATCB AIL_register_beat_callback(SNDSEQUENCE *seq, AILBEATCB callback);

/** Install callback function handler for MIDI/XMIDI event trap.
 */
AILEVENTCB AIL_register_event_callback(MDI_DRIVER *mdidrv, AILEVENTCB callback);

/** Set sequence user data value at specified index.
 *
 * Any desired value may be stored at one of eight user data words
 * associated with a given SNDSEQUENCE
 *
 * Callback functions may access the user data array at interrupt time.
 */
void AIL_set_sequence_user_data(SNDSEQUENCE *seq, uint32_t index, intptr_t value);

/** Get sequence user data value at specified index.
 *
 * Any desired value may be stored at one of eight user data words
 * associated with a given SNDSEQUENCE
 *
 * Callback functions may access the user data array at interrupt time.
 */
intptr_t AIL_sequence_user_data(SNDSEQUENCE *seq, uint32_t index);

/** Load, install, and initialize MIDI audio driver according to
 *  contents of MDI_INI file.
 */
int32_t AIL_install_MDI_INI(MDI_DRIVER **mdidrv);

/** Read .INI file with sound hardware configuration.
 */
int32_t AIL_read_INI(AIL_INI *ini, char *fname);

/** Install and initialize DOS XMIDI audio driver.
 */
MDI_DRIVER *AIL_install_MDI_driver_file(const char *fname, SNDCARD_IO_PARMS *iop);

/** Uninstall DOS XMIDI audio driver.
 */
void AIL_uninstall_MDI_driver(MDI_DRIVER *mdidrv);

/** Install and initialize XMIDI audio driver.
 */
MDI_DRIVER *AIL_open_XMIDI_driver(uint32_t flags);

/** Uninstall XMIDI audio driver.
 *
 * This should not be called directly as it is within AIL_DRIVER.destructor vector.
 */
void AIL_close_XMIDI_driver(MDI_DRIVER *mdidrv);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AILDEBUG_H_
