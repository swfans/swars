/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssxmidi.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     SS functions from MSS API.
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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "mssxmidi.h"
#include "ail.h"
#include "aildebug.h"
#include "dllload.h"
#include "memfile.h"
#include "msssys.h"
#include "miscutil.h"
/******************************************************************************/
extern char GTL_prefix[128];
extern char SoundDriverPath[144];
extern char AIL_error[256];
extern SNDCARD_IO_PARMS AIL_last_IO_attempt;
extern int32_t AIL_preference[AIL_N_PREFS];

extern MDI_DRIVER *MDI_first;
extern uint32_t MDI_entry;
extern SNDSEQUENCE *MDI_S;
extern int32_t MDI_i,MDI_j,MDI_n,MDI_sequence_done;
extern int32_t MDI_q,MDI_t;
extern uint32_t MDI_channel,MDI_status,MDI_type,MDI_len;
extern const uint8_t *MDI_ptr;
extern const uint8_t *MDI_event;

extern int32_t MDI_locked;

void AILXMIDI_end(void);

/** Lock function, doubling as start of locked code.
 */
void AILXMIDI_start(void)
{
    if (MDI_locked)
        return;
    AIL_VMM_lock_range(AILXMIDI_start, AILXMIDI_end);

    AIL_vmm_lock(GTL_prefix, sizeof(GTL_prefix));
    AIL_vmm_lock(&MDI_entry, sizeof(MDI_entry));
    AIL_vmm_lock(&MDI_S, sizeof(MDI_S));
    AIL_vmm_lock(&MDI_i, sizeof(MDI_i));
    AIL_vmm_lock(&MDI_j, sizeof(MDI_j));
    AIL_vmm_lock(&MDI_n, sizeof(MDI_n));
    AIL_vmm_lock(&MDI_sequence_done, sizeof(MDI_sequence_done));
    AIL_vmm_lock(&MDI_q, sizeof(MDI_q));
    AIL_vmm_lock(&MDI_t, sizeof(MDI_t));
    AIL_vmm_lock(&MDI_channel, sizeof(MDI_channel));
    AIL_vmm_lock(&MDI_status, sizeof(MDI_status));
    AIL_vmm_lock(&MDI_type, sizeof(MDI_type));
    AIL_vmm_lock(&MDI_len, sizeof(MDI_len));
    AIL_vmm_lock(&MDI_ptr, sizeof(MDI_ptr));
    AIL_vmm_lock(&MDI_event, sizeof(MDI_event));

    MDI_locked = 1;
}

/** Force transmission of any buffered MIDI traffic.
 */
void XMI_flush_buffer(MDI_DRIVER *mdidrv)
{
    VDI_CALL VDI;

    if (mdidrv->message_count < 1)
        return;

    VDI.CX = mdidrv->message_count;

    AIL_call_driver(mdidrv->drvr, MDI_MIDI_XMIT, &VDI, NULL);
    mdidrv->message_count = 0;
    mdidrv->offset = 0;
}

/** Write channel voice message to MIDI driver buffer
*/
void XMI_MIDI_message(MDI_DRIVER *mdidrv, int32_t status,
        int32_t d1, int32_t d2)
{
#if defined(DOS)||defined(GO32)
    {
        uint32_t size;

        size = XMI_message_size(status);

        if ((mdidrv->offset + size) > sizeof(mdidrv->DST->MIDI_data))
            XMI_flush_buffer(mdidrv);

        mdidrv->DST->MIDI_data[mdidrv->offset++] = (int8_t)(status & 0xff);
        mdidrv->DST->MIDI_data[mdidrv->offset++] = (int8_t)(d1 & 0xff);

        if (size == 3)
            mdidrv->DST->MIDI_data[mdidrv->offset++] = (int8_t)(d2 & 0xff);

        mdidrv->message_count++;
    }
#endif
}

void AIL2OAL_API_set_GTL_filename_prefix(char const *prefix)
{
    int i;

    strcpy(GTL_prefix, prefix);

    // Truncate prefix string at final '.' character, if it exists and
    // if it occurs after the last directory separator
    for (i = strlen(GTL_prefix)-1; i > 0; i--)
    {
        if ((GTL_prefix[i] == '\\') || (GTL_prefix[i] == '/'))
        {
            if (i == (int)strlen(GTL_prefix)-1) {
                GTL_prefix[i] = '\0';
            }
            break;
        }

        if (GTL_prefix[i] == '.') {
            GTL_prefix[i] = '\0';
            break;
        }
    }
}

int32_t AIL2OAL_API_MDI_driver_type(MDI_DRIVER *mdidrv)
{
    const char *name;

    // If driver name corresponds to Tandy or IBM speaker driver, return
    // speaker type
    //
    // Name field was added in VDI 1.12 spec, so don't check earlier drivers
    if (mdidrv->drvr->VHDR->driver_version >= 0x112)
    {
        name = mdidrv->drvr->VHDR->dev_name;

        if (strcasecmp(name, "Tandy 3-voice music") == 0)
            return MDIDRVRTYPE_SPKR;
        if (strcasecmp(name, "IBM internal speaker music") == 0)
            return MDIDRVRTYPE_SPKR;
    }

    // If no GTL suffix, assume it's a hardwired General MIDI device
    name = mdidrv->DDT->GTL_suffix;
    if ((name == NULL) || (name[0] == '\0'))
        return MDIDRVRTYPE_GM;

     // If GTL suffix = '.AD', it's an OPL-2
     //
     // Note: Creative AWE32 driver incorrectly declares '.AD' GTL prefix,
     // so provide workaround here - if driver bigger than 20K, it's not one
     // of our FM drivers!
    if (strcasecmp(name, ".AD") == 0)
    {
        if (mdidrv->drvr->size > 20480)
            return MDIDRVRTYPE_GM;
        return MDIDRVRTYPE_FM_2;
    }

    // If GTL suffix = '.OPL', it's an OPL-3
    if (strcasecmp(name, ".OPL") == 0)
        return MDIDRVRTYPE_FM_4;

    // Otherwise, it's a currently-undefined GTL type - assume it's a GM device
    return MDIDRVRTYPE_GM;
}

/** Call device I/O verification function using current detection policy.
 */
int32_t XMI_attempt_MDI_detection(MDI_DRIVER *mdidrv, const SNDCARD_IO_PARMS *iop)
{
    SNDCARD_IO_PARMS *f;
    SNDCARD_IO_PARMS try;
    uint32_t i;

    // Set up working I/O params structure
    try = *iop;

    // If any needed parameters are not specified, use parameter
    // values from first factory-default IO structure
    if (mdidrv->drvr->VHDR->num_IO_configurations > 0)
    {
        f = (SNDCARD_IO_PARMS *) (mdidrv->drvr->VHDR->common_IO_configurations);

        if (try.IO < 1)
            try.IO = f->IO;

        if (try.IRQ < 1)
            try.IRQ = f->IRQ;

        if (try.DMA_8_bit < 1)
            try.DMA_8_bit = f->DMA_8_bit;

        if (try.DMA_16_bit < 1)
            try.DMA_16_bit = f->DMA_16_bit;

        for (i=0; i < 4; i++)
        {
            if (try.IO_reserved[i] < 1)
                try.IO_reserved[i] = f->IO_reserved[i];
        }
    }

    // Copy IO parameter block to driver
    mdidrv->drvr->VHDR->IO = try;

    // Call detection function
    return AIL_call_driver(mdidrv->drvr, DRV_VERIFY_IO, NULL, NULL);
}

/** Timer interrupt routine for XMIDI sequencing.
 */
void XMI_serve(MDI_DRIVER *mdidrv)
{
    asm volatile (
      "push %0\n"
      "call ASM_XMI_serve\n"
      "add $0x4, %%esp\n"
        :  : "g" (mdidrv));
}

/** Uninstall XMIDI audio driver, freeing all allocated resources.
 *
 * This function is called via the AIL_DRIVER.destructor vector only
 */
void XMI_destroy_MDI_driver(MDI_DRIVER *mdidrv)
{
    int32_t i;

    // Stop all playing sequences to avoid hung notes
    for (i = 0; i < mdidrv->n_sequences; i++) {
        AIL_end_sequence(&mdidrv->sequences[i]);
    }

    // Stop sequencer timer service
    AIL_release_timer_handle(mdidrv->timer);

    // Release memory resources
    AIL_MEM_free_lock(mdidrv->sequences, mdidrv->n_sequences * sizeof(SNDSEQUENCE));
    AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
}

/** Initialize XMIDI device.
 */
void init_mdi_defaults(MDI_DRIVER *mdidrv)
{
    int32_t i;

    // Initialize synthesizer to General MIDI defaults
    for (i=0; i < AIL_NUM_CHANS; i++)
    {
        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_PATCH_BANK_SEL, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_PROGRAM | i,
                0, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_PITCH   | i,
                0x00,0x40);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_VOICE_PROTECT, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_MODULATION, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_PART_VOLUME, AIL_preference[MDI_DEFAULT_VOLUME]);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_PANPOT, 64);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_EXPRESSION, 127);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_SUSTAIN, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_REVERB, 40);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_CHORUS, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_RPN_LSB, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_RPN_MSB, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_DATA_LSB, 0);

        XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | i,
                MDI_CTR_PB_RANGE, AIL_preference[MDI_DEFAULT_BEND_RANGE]);

        XMI_flush_buffer(mdidrv);

        if (!(i & 3))
            AIL_delay(3);
    }
}

/** Install and initialize XMIDI audio driver.
 */
MDI_DRIVER *XMI_construct_MDI_driver(AIL_DRIVER *drvr, const SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;
#if 0
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_XMI_construct_MDI_driver\n"
      "add $0x8, %%esp\n"
        : "=r" (mdidrv) : "g" (drvr), "g" (iop));
#else
    SNDCARD_IO_PARMS use;
    int32_t i;
    int32_t detected;
    VDI_CALL VDI;

    // Ensure that all AILXMIDI code and data is locked into memory
    AILXMIDI_start();

    // Allocate memory for MDI_DRIVER structure
    mdidrv = AIL_MEM_alloc_lock(sizeof(MDI_DRIVER));
    if (mdidrv == NULL) {
        AIL_set_error("Could not allocate memory for driver.");
        return NULL;
    }

    mdidrv->drvr = drvr;

    // Reject driver if not of type .MDI
    if (mdidrv->drvr->type != AIL3MDI) {
        AIL_set_error(".MDI driver required.");
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }

#if defined(DOS)||defined(GO32)
    // Get DDT and DST addresses
    AIL_call_driver(mdidrv->drvr, DRV_GET_INFO, NULL, &VDI);

    mdidrv->DDT = (void *) (((uint32_t)VDI.DX << 4) + (uint32_t)VDI.AX);
    mdidrv->DST = (void *) (((uint32_t)VDI.CX << 4) + (uint32_t)VDI.BX);

    // Copy library environment string to DST, if requested
    envname = mdidrv->DDT->library_environment;

    if (!((envname == NULL) || (envname[0] == '\0')))
    {
        envval = getenv(envname);

        if (!((envval == NULL) || (envval[0] == '\0')))
        {
            i = strlen(envval);
            if (i > 127) i = 127;
            memcpy(mdidrv->DST->library_directory, envval, i);
            mdidrv->DST->library_directory[i] = '\0';
        }
    }

    // Copy AIL 2.X GTL filename to DST, if valid
    // (By default, AIL applications use SAMPLE.XXX GTL filenames)
    {
        char *envname;
        char *envval;

        envname = mdidrv->DDT->GTL_suffix;
        if (!((envname == NULL) || (envname[0] == '\0'))) {
            strcpy(mdidrv->DST->GTL_filename, GTL_prefix);
            strcat(mdidrv->DST->GTL_filename, envname);
        } else {
            mdidrv->DST->GTL_filename[0] = '\0';
        }
    }
#else
    // No DDT or DST addresses
    mdidrv->DDT = NULL;
    mdidrv->DST = NULL;
#endif

    // Verify hardware I/O parameters
    memset(&AIL_last_IO_attempt, -1, sizeof(SNDCARD_IO_PARMS));

    // If explicit IO_PARMS structure provided by application, try it
    // first
    detected = 0;
    if (iop != NULL) {
        AIL_last_IO_attempt = *iop;
        if (XMI_attempt_MDI_detection(mdidrv, iop)) {
            detected = 1;
            use = *iop;
        }
    }

    // Next, try device-specific environment string (if applicable)
    if (!detected)
    {
        iop = AIL_get_IO_environment(mdidrv->drvr);
        if (iop != NULL) {
            AIL_last_IO_attempt = *iop;
            if (XMI_attempt_MDI_detection(mdidrv, iop)) {
                detected = 1;
                use = *iop;
            }
        }
    }

    // Finally, try all common_IO_configurations[] entries in driver
    if ((!detected) && (AIL_preference[AIL_SCAN_FOR_HARDWARE] == 1))
    {
        for (i = 0; i < mdidrv->drvr->VHDR->num_IO_configurations; i++)
        {
            iop = &((SNDCARD_IO_PARMS *)
                (mdidrv->drvr->VHDR->common_IO_configurations))[i];

            if (i == 0) {
                AIL_last_IO_attempt = *iop;
            }
            if (XMI_attempt_MDI_detection(mdidrv, iop)) {
                detected = 1;
                use = *iop;
                break;
            }
        }
    }

    // If all detection attempts failed, return NULL
    if (detected) {
        AIL_last_IO_attempt = use;
    } else {
        AIL_set_error("XMIDI sound hardware not found.");
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }

    // Initialize device
    AIL_call_driver(mdidrv->drvr, DRV_INIT_DEV, NULL, NULL);

    mdidrv->drvr->initialized = 1;

    // Initialize instrument manager
    AIL_call_driver(mdidrv->drvr, MDI_INIT_INS_MGR, NULL, &VDI);
    if (!VDI.AX) {
        AIL_set_error("Could not initialize instrument manager.");
        AIL_call_driver(mdidrv->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
        mdidrv->drvr->initialized = 0;
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }

    // Allocate SNDSEQUENCE structures for driver
    mdidrv->n_sequences = AIL_preference[MDI_SEQUENCES];
    mdidrv->sequences = AIL_MEM_alloc_lock(sizeof(SNDSEQUENCE) * mdidrv->n_sequences);
    if (mdidrv->sequences == NULL) {
        AIL_set_error("Could not allocate SEQUENCE structures.");
        AIL_call_driver(mdidrv->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
        mdidrv->drvr->initialized = 0;
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }

    for (i = 0; i < mdidrv->n_sequences; i++)
    {
        mdidrv->sequences[i].status = SNDSEQ_FREE;
        mdidrv->sequences[i].driver = mdidrv;
    }

    // Initialize miscellaneous MDI_DRIVER variables
    mdidrv->event_trap = NULL;
    mdidrv->timbre_trap = NULL;

    mdidrv->message_count = 0;
    mdidrv->offset = 0;

    mdidrv->interval_time = 1000000L / AIL_preference[MDI_SERVICE_RATE];

    mdidrv->disable = 0;

    mdidrv->master_volume = 127;

    // Initialize channel lock table to NULL (all physical channels
    // available)
    for (i = 0; i < AIL_NUM_CHANS; i++)
    {
        mdidrv->lock[i] = 0;
        mdidrv->locker[i] = NULL;
        mdidrv->owner[i] = NULL;
        mdidrv->user[i] = NULL;
        mdidrv->state[i] = 0;
        mdidrv->notes[i] = 0;
    }

    // Allocate timer for XMIDI sequencing
    mdidrv->timer = AIL_register_timer((AILTIMERCB)XMI_serve);
    if (mdidrv->timer == -1) {
        AIL_set_error("Out of timer handles.");
        AIL_call_driver(mdidrv->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
        mdidrv->drvr->initialized = 0;
        AIL_MEM_free_lock(mdidrv->sequences, mdidrv->n_sequences * sizeof(SNDSEQUENCE));
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }

    AIL_set_timer_user(mdidrv->timer, mdidrv);

    // Set destructor handler and descriptor
    mdidrv->drvr->destructor = (void *) XMI_destroy_MDI_driver;
    mdidrv->drvr->descriptor = mdidrv;

    init_mdi_defaults(mdidrv);

    // Start XMIDI timer service and return MDI_DRIVER descriptor
    AIL_set_timer_frequency(mdidrv->timer, AIL_preference[MDI_SERVICE_RATE]);
    AIL_start_timer(mdidrv->timer);
#endif
    return mdidrv;
}

/** Send MIDI channel voice message associated with a specific sequence
 *
 * Includes controller logging and XMIDI extensions.
 *
 * Warnings: ICA_enable should be 0 when calling outside XMIDI event loop
 * May be recursively called by XMIDI controller handlers.
 */
void XMI_send_channel_voice_message(SNDSEQUENCE *seq, int32_t status,
        int32_t data_1, int32_t data_2, int32_t ICA_enable)
{
    asm volatile (
      "push %4\n"
      "push %3\n"
      "push %2\n"
      "push %1\n"
      "push %0\n"
      "call ASM_XMI_send_channel_voice_message\n"
      "add $0x14, %%esp\n"
        :  : "g" (seq), "g" (status), "g" (data_1), "g" (data_2), "g" (ICA_enable));
}

/** Transmit logged channel controller values
 */
void XMI_refresh_channel(SNDSEQUENCE *seq, int32_t ch)
{
    if (seq->shadow.bank[ch] != -1) {
        XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_PATCH_BANK_SEL, seq->shadow.bank[ch], 0);
    }

    if (seq->shadow.program[ch] != -1) {
        XMI_send_channel_voice_message(seq, MDI_EV_PROGRAM | ch,
                seq->shadow.program[ch], 0, 0);
    }

    if (seq->shadow.pitch_h[ch] != -1) {
        XMI_send_channel_voice_message(seq, MDI_EV_PITCH | ch,
                seq->shadow.pitch_l[ch], seq->shadow.pitch_h[ch], 0);
    }

    if (seq->shadow.c_mute[ch] != -1) {
        XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_CHAN_MUTE, seq->shadow.c_mute[ch], 0);
    }

    if (seq->shadow.c_prot[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_CHAN_PROTECT, seq->shadow.c_prot[ch], 0);
    }

    if (seq->shadow.c_v_prot[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_VOICE_PROTECT, seq->shadow.c_v_prot[ch], 0);
    }

    if (seq->shadow.mod[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_MODULATION, seq->shadow.mod[ch], 0);
    }

    if (seq->shadow.vol[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_PART_VOLUME, seq->shadow.vol[ch], 0);
    }

    if (seq->shadow.pan[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_PANPOT, seq->shadow.pan[ch], 0);
    }

    if (seq->shadow.exp[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_EXPRESSION, seq->shadow.exp[ch], 0);
    }

    if (seq->shadow.sus[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_SUSTAIN, seq->shadow.sus[ch], 0);
    }

    if (seq->shadow.reverb[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_REVERB, seq->shadow.reverb[ch], 0);
    }

    if (seq->shadow.chorus[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_CHORUS, seq->shadow.chorus[ch], 0);
    }

    if (seq->shadow.bend_range[ch] != -1) {
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
                MDI_CTR_PB_RANGE, seq->shadow.bend_range[ch], 0);
    }
}

void AIL2OAL_API_release_channel(MDI_DRIVER *mdidrv, int32_t channel)
{
    int32_t i, j, ch;
    SNDSEQUENCE *seq;

    // Convert channel # to 0-based internal notation
    ch = channel-1;

    // If channel is not locked, return
    if (mdidrv->lock[ch] != 1)
        return;

    // Disable XMIDI service while unlocking channel
    mdidrv->disable++;

    // Restore channel's original state and ownership
    mdidrv->lock[ch] = mdidrv->state[ch];
    mdidrv->user[ch] = mdidrv->owner[ch];

    // Release sustain pedal and turn all notes off in channel,
    // regardless of sequence
    XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | ch,
            MDI_CTR_SUSTAIN, 0);

    for (i = mdidrv->n_sequences, seq = &mdidrv->sequences[0]; i; --i,++seq)
    {
        if (seq->status == SNDSEQ_FREE)
            continue;

        for (j=0; j < AIL_MAX_NOTES; j++)
        {
            if (seq->note_chan[j] == -1)
                continue;
            if (seq->chan_map[seq->note_chan[j]] != ch)
                continue;
            XMI_send_channel_voice_message(seq,
                    seq->note_chan[j] | MDI_EV_NOTE_OFF,
                    seq->note_num [j], 0, 0);
            seq->note_chan[j] = -1;
        }
    }

     // Bring channel up to date with owner's current controller values, if
     // owner is valid sequence
    if (mdidrv->owner[ch] != NULL)
    {
        if (mdidrv->owner[ch]->status != SNDSEQ_FREE)
            XMI_refresh_channel(mdidrv->owner[ch], ch);
    }

    mdidrv->disable--;
}

/** Flush sequence note queue.
 */
void XMI_flush_note_queue(SNDSEQUENCE *seq)
{
    int32_t i, nmsgs;

    nmsgs = 0;

    for (i=0; i < AIL_MAX_NOTES; i++)
    {
        if (seq->note_chan[i] == -1)
            continue;

        // Send MIDI Note Off message
        XMI_send_channel_voice_message(seq,
                seq->note_chan[i] | MDI_EV_NOTE_OFF,
                seq->note_num [i], 0, 0);

        // Release queue entry and increment "note off" count
        seq->note_chan[i] = -1;

        nmsgs++;
    }

   seq->note_count = 0;

    // If any messages were sent, delay before returning to give
    // slower MPU-401 devices enough time to process MIDI data
    if ((nmsgs) && (!AIL_background()))
        AIL_delay(3);
}

int32_t AIL2OAL_API_install_MDI_INI(MDI_DRIVER **mdidrv)
{
    AIL_INI ini;

    // Attempt to read MDI_INI file
    if (!AIL_read_INI(&ini, "MDI.INI")) {
        AIL_set_error("Unable to open file MDI.INI.");
        return AIL_NO_INI_FILE;
    }

    *mdidrv = AIL_install_MDI_driver_file(ini.driver_name, &ini.IO);
    if (*mdidrv == NULL) {
        return AIL_INIT_FAILURE;
    }
    return AIL_INIT_SUCCESS;
}

SNDSEQUENCE *AIL2OAL_API_allocate_sequence_handle(MDI_DRIVER *mdidrv)
{
    SNDSEQUENCE *seq;
    asm volatile (
      "push %1\n"
      "call ASM_AIL_API_allocate_sequence_handle\n"
      "add $0x4, %%esp\n"
        : "=r" (seq) : "g" (mdidrv));
    return seq;
}

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(char *fname, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;
#if 0
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_AIL_API_install_MDI_driver_file\n"
      "add $0x8, %%esp\n"
        : "=r" (mdidrv) : "g" (fname), "g" (iop));
#else
    char locstr[156];
    int32_t flen;
    void *drvbuf;
    AIL_DRIVER *drvr;

    sprintf(locstr, "%s/%s", SoundDriverPath, fname);
    drvbuf = FILE_read(locstr, NULL);
    if (drvbuf == NULL) {
        strcpy(AIL_error, "Driver file not found\n");
        return NULL;
    }
    flen = FILE_size(locstr);
    drvr = AIL_install_driver(drvbuf, flen);
    MEM_free(drvbuf);
    if (drvr == NULL) {
        return NULL;
    }
    mdidrv = XMI_construct_MDI_driver(drvr, iop);
    if (mdidrv == NULL)
        AIL_uninstall_driver(drvr);
#endif
    return mdidrv;
}

void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv)
{
#if 0
    asm volatile (
      "push %0\n"
      "call ASM_AIL_API_uninstall_MDI_driver\n"
      "add $0x4, %%esp\n"
        :  : "g" (mdidrv));
#endif
   AIL_uninstall_driver(mdidrv->drvr);
}

MDI_DRIVER *AIL2OAL_API_open_XMIDI_driver(uint32_t flags)
{
    int32_t i;
    MDI_DRIVER *mdidrv;

    mdidrv = AIL_MEM_alloc_lock(sizeof(MDI_DRIVER));
    if (mdidrv == NULL) {
        AIL_set_error("Could not allocate memory for driver descriptor.");
        return NULL;
    }

    // Allocate SEQUENCE structures for driver and set params
    mdidrv->n_sequences = AIL_preference[MDI_SEQUENCES];
    mdidrv->sequences = AIL_MEM_alloc_lock(
        sizeof(struct SNDSEQUENCE) * mdidrv->n_sequences);
    if (mdidrv->sequences == NULL) {
        AIL_set_error("Could not allocate SEQUENCE structures.");
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }

    for (i = 0; i < mdidrv->n_sequences; i++)
    {
        mdidrv->sequences[i].status = SNDSEQ_FREE;
        mdidrv->sequences[i].driver = mdidrv;
    }

    // Initialize miscellaneous MDI_DRIVER variables
    mdidrv->event_trap = NULL;
    mdidrv->timbre_trap = NULL;

    mdidrv->interval_time = 1000000L / AIL_preference[MDI_SERVICE_RATE];

    mdidrv->disable = 0;

    mdidrv->master_volume = 127;

    // Initialize channel lock table to NULL (all physical channels
    // available)
    for (i = 0; i < AIL_NUM_CHANS; i++)
    {
        mdidrv->lock[i] = 0;
        mdidrv->locker[i] = NULL;
        mdidrv->owner[i] = NULL;
        mdidrv->user[i] = NULL;
        mdidrv->state[i] = 0;
        mdidrv->notes[i] = 0;
    }

    // Allocate timer for XMIDI sequencing
    mdidrv->timer = AIL_register_timer((AILTIMERCB)XMI_serve);
    if (mdidrv->timer == -1) {
        AIL_set_error("Out of timer handles.");
        AIL_MEM_free_lock(mdidrv->sequences, mdidrv->n_sequences * sizeof(SNDSEQUENCE));
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }

    AIL_set_timer_user(mdidrv->timer, (void *)mdidrv);
    init_mdi_defaults(mdidrv);

    // Start XMIDI timer service and return MDI_DRIVER descriptor
    AIL_set_timer_frequency(mdidrv->timer, AIL_preference[MDI_SERVICE_RATE]);
    AIL_start_timer(mdidrv->timer);

    // Link HMDIDRIVER into chain
    if (MDI_first != NULL)
        mdidrv->win.next = MDI_first;
    else
        mdidrv->win.next = NULL;

    MDI_first = mdidrv;
    return mdidrv;
}

void AIL2OAL_API_close_XMIDI_driver(MDI_DRIVER *mdidrv)
{
    MDI_DRIVER *cur, *prev;
    int32_t i;

    // Stop all playing sequences to avoid hung notes
    for (i = 0; i < mdidrv->n_sequences; i++)
        AIL_end_sequence(&mdidrv->sequences[i]);

    // Unlink from MDI_DRIVER chain
    if (mdidrv == MDI_first)
    {
        MDI_first = mdidrv->win.next;
    }
    else
    {
        prev = MDI_first;
        cur = MDI_first->win.next;

        while (cur != mdidrv)
        {
            if (cur == NULL)
                return;
            prev = cur;
            cur = cur->win.next;
        }
        prev->win.next = cur->win.next;
    }

    // Stop sequencer timer service
    AIL_release_timer_handle(mdidrv->timer);

    // Release memory resources
    AIL_MEM_free_lock(mdidrv->sequences, mdidrv->n_sequences * sizeof(SNDSEQUENCE));
    AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
}

void AIL2OAL_API_stop_sequence(SNDSEQUENCE *seq)
{
    MDI_DRIVER *mdidrv;
    int32_t log, phys;

    if (seq == NULL)
        return;

     // Make sure sequence is currently playing
    if ((seq->status != SNDSEQ_PLAYING)  &&
      (seq->status != SNDSEQ_PLAYINGRELEASED))
        return;

    // Mask 'playing' status
    seq->status = SNDSEQ_STOPPED;

    // Turn off any active notes in sequence
    XMI_flush_note_queue(seq);

    // Prepare sequence's channels for use with other sequences, leaving
    // shadow array intact for later recovery by AIL_resume_sequence()
    mdidrv = seq->driver;

    for (log = 0; log < AIL_NUM_CHANS; log++)
    {
        phys = seq->chan_map[log];

        // If sustain pedal on, release it
        if (seq->shadow.sus[log] >= 64)
            XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | phys,
                    MDI_CTR_SUSTAIN, 0);

       // If channel-lock protection active, cancel it
        if (seq->shadow.c_prot[log] >= 64)
            mdidrv->lock[phys] = 0;

      // If voice-stealing protection active, cancel it
        if (seq->shadow.c_v_prot[log] >= 64)
            XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | phys,
                    MDI_CTR_VOICE_PROTECT, 0);

      // Finally, if channel was locked, release it
       if (seq->shadow.c_lock[log] >= 64)
           AIL_release_channel(mdidrv, phys+1);
    }
}

void AIL2OAL_API_resume_sequence(SNDSEQUENCE *seq)
{
    asm volatile (
      "push %0\n"
      "call ASM_AIL_API_resume_sequence\n"
      "add $0x4, %%esp\n"
        :  : "g" (seq));
}

void AIL2OAL_API_end_sequence(SNDSEQUENCE *seq)
{
    if (seq == NULL)
        return;

    // Make sure sequence is currently allocated
    if ((seq->status == SNDSEQ_FREE) || (seq->status == SNDSEQ_DONE))
        return;

    // Stop sequence and set 'done' status
    AIL_stop_sequence(seq);

    seq->status = SNDSEQ_DONE;

   // Call EOS handler, if any
    if (seq->EOS != NULL) {
        ((AILSEQUENCECB)seq->EOS)(seq);
    }
}

/** Unlock function, doubling as end of locked code.
 */
void AILXMIDI_end(void)
{
    if (!MDI_locked)
        return;
    AIL_VMM_unlock_range(AILXMIDI_start, AILXMIDI_end);

    AIL_vmm_unlock(GTL_prefix, sizeof(GTL_prefix));
    AIL_vmm_unlock(&MDI_entry, sizeof(MDI_entry));
    AIL_vmm_unlock(&MDI_S, sizeof(MDI_S));
    AIL_vmm_unlock(&MDI_i, sizeof(MDI_i));
    AIL_vmm_unlock(&MDI_j, sizeof(MDI_j));
    AIL_vmm_unlock(&MDI_n, sizeof(MDI_n));
    AIL_vmm_unlock(&MDI_sequence_done, sizeof(MDI_sequence_done));
    AIL_vmm_unlock(&MDI_q, sizeof(MDI_q));
    AIL_vmm_unlock(&MDI_t, sizeof(MDI_t));
    AIL_vmm_unlock(&MDI_channel, sizeof(MDI_channel));
    AIL_vmm_unlock(&MDI_status, sizeof(MDI_status));
    AIL_vmm_unlock(&MDI_type, sizeof(MDI_type));
    AIL_vmm_unlock(&MDI_len, sizeof(MDI_len));
    AIL_vmm_unlock(&MDI_ptr, sizeof(MDI_ptr));
    AIL_vmm_unlock(&MDI_event, sizeof(MDI_event));

    MDI_locked = 0;
}

/******************************************************************************/
