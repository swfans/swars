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
#include <limits.h>
#include <assert.h>

#include "mssxmidi.h"
#include "ail.h"
#include "aildebug.h"
#include "miscutil.h"
#include "memfile.h"
#include "drv_oal.h"
#include "msssys.h"
#include "miscutil.h"
#if LBS_ENABLE_WILDMIDI
#  include "wildmidi_lib.h"
#endif
/******************************************************************************/
const uint8_t *MDI_ptr;
const uint8_t *MDI_event;
uint32_t MDI_status, MDI_len;
uint32_t MDI_type, MDI_channel;
SNDSEQUENCE *MDI_S;
int32_t MDI_i, MDI_j, MDI_n;
int32_t MDI_q, MDI_t;
int32_t MDI_sequence_done;

static uint32_t XMI_serve_entry = 0;

char GTL_prefix[128] = "SAMPLE";
extern char SoundDriverPath[144];

extern MDI_DRIVER *MDI_first;
uint32_t MDI_entry = 0;
int32_t MDI_use_locked = 0;

/******************************************************************************/

void AILXMIDI_end(void);

/** Lock function, doubling as start of locked code.
 */
void AILXMIDI_start(void)
{
    if (MDI_use_locked)
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

    MDI_use_locked = 1;
}

/** Initialize state table entries.
 */
static void XMI_init_sequence_state(SNDSEQUENCE *seq)
{
   int32_t i;
   static MDI_CTRL_LOG temp_log;

    // Initialize logical-physical channel map to identity set
    for (i=0; i < AIL_NUM_CHANS; i++)
        seq->chan_map[i] = i;

    // Initialize all logged controllers to -1
    memset(&temp_log, -1, sizeof(MDI_CTRL_LOG));
    memcpy(&seq->shadow, &temp_log, sizeof(MDI_CTRL_LOG));


    // Initialize FOR loop counters
    for (i=0; i < AIL_FOR_NEST; i++)
        seq->FOR_loop_count[i] = -1;

    // Initialize note queue
    for (i=0; i < AIL_MAX_NOTES; i++)
        seq->note_chan[i] = -1;
    seq->note_count = 0;

    // Initialize timing variables
    // Default to 4/4 time at 120 beats/minute
    seq->interval_count =  0;
    seq->beat_count   =  0;
    seq->measure_count = -1;
    seq->beat_fraction =  0;
    seq->time_fraction =  0;
    seq->time_numerator =  4;
    seq->time_per_beat =  500000*16;
    seq->interval_num =  0;
}

/** Reset sequence pointers and initialize state table entries.
 */
static void XMI_rewind_sequence(SNDSEQUENCE *seq)
{
    unsigned long int wildpos;

    // Initialize sequence state table
    XMI_init_sequence_state(seq);

    // Initialize event pointer to start of XMIDI EVNT chunk data
    seq->EVNT_ptr = (uint8_t *)seq->EVNT + 8;

    wildpos = 0;
#if LBS_ENABLE_WILDMIDI
    WildMidi_FastSeek(seq->ICA, &wildpos);
#endif
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

/** Flush notes in one channel only.
 */
void XMI_flush_channel_notes(SNDSEQUENCE *seq, int32_t channel)
{
    int32_t i;

    for (i=0; i < AIL_MAX_NOTES; i++)
    {
        if (seq->note_chan[i] != channel)
            continue;
        // Send MIDI Note Off message
        XMI_send_channel_voice_message(seq, seq->note_chan[i] |MDI_EV_NOTE_OFF,
                seq->note_num [i], 0, 0);
        // Release queue entry
        seq->note_chan[i] = -1;
    }
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

/** Write system exclusive message to MIDI driver buffer.
 */
void XMI_sysex_message(MDI_DRIVER *mdidrv, uint8_t const *message,
        int32_t size)
{
    int32_t bound_size;

    XMI_flush_buffer(mdidrv);

    bound_size = sizeof(mdidrv->DST->MIDI_data);
    if (bound_size > size)
        bound_size = size;

    memmove(mdidrv->DST->MIDI_data, message, bound_size);

    mdidrv->message_count++;

    XMI_flush_buffer(mdidrv);
}

void XMI_write_log(MDI_CTRL_LOG *log, int32_t status, int32_t data_1, int32_t data_2)
{
    int32_t st;
    int32_t ch;

    st = status & 0xf0;
    ch = status & 0x0f;

    switch (st)
    {
    case MDI_EV_PROGRAM:
        log->program[ch] = data_1;
        break;

    case MDI_EV_PITCH:
        log->pitch_l[ch] = data_1;
        log->pitch_h[ch] = data_2;
        break;

    case MDI_EV_CONTROL:
        switch (data_1)
        {
        case MDI_CTR_CHAN_LOCK:
            log->c_lock[ch] = data_2;
            break;

        case MDI_CTR_CHAN_PROTECT:
            log->c_prot[ch] = data_2;
            break;

        case MDI_CTR_VOICE_PROTECT:
            log->c_v_prot[ch] = data_2;
            break;

        case MDI_CTR_PATCH_BANK_SEL:
            log->bank[ch]  = data_2;
            break;

        case MDI_CTR_INDIRECT_C_PFX:
            log->indirect[ch] = data_2;
            break;

        case MDI_CTR_CALLBACK_TRIG:
            log->callback[ch] = data_2;
            break;

        case MDI_CTR_MODULATION:
            log->mod[ch]   = data_2;
            break;

        case MDI_CTR_PART_VOLUME:
            log->vol[ch]   = data_2;
            break;

        case MDI_CTR_PANPOT:
            log->pan[ch]   = data_2;
            break;

        case MDI_CTR_EXPRESSION:
            log->exp[ch]   = data_2;
            break;

        case MDI_CTR_SUSTAIN:
            log->sus[ch]   = data_2;
            break;

        case MDI_CTR_REVERB:
            log->reverb[ch] = data_2;
            break;

        case MDI_CTR_CHORUS:
            log->chorus[ch] = data_2;
            break;

        case MDI_CTR_DATA_MSB:
            log->bend_range[ch] = data_2;
            break;
        }
    }
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

    // Unlink from MDI_DRIVER chain - but there is no chain, only head, so just NULL it
    if (mdidrv == MDI_first)
        MDI_first = NULL;

    // Stop sequencer timer service
    AIL_release_timer_handle(mdidrv->timer);

    OPENAL_free_sources_for_sequences(mdidrv);

    OPENAL_free_buffers(mdidrv->n_sequences);

#if LBS_ENABLE_WILDMIDI
    WildMidi_Shutdown();
#endif

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
                MDI_CTR_DATA_MSB, AIL_preference[MDI_DEFAULT_BEND_RANGE]);

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
    SNDCARD_IO_PARMS use;
    int32_t i;
    int32_t detected;
    VDI_CALL VDI;
    uint32_t smp_rate;

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
    {
        AIL_call_driver(mdidrv->drvr, DRV_GET_INFO, NULL, &VDI);
        mdidrv->DDT = (void *) (((uint32_t)VDI.DX << 4) + (uint32_t)VDI.AX);
        mdidrv->DST = (void *) (((uint32_t)VDI.CX << 4) + (uint32_t)VDI.BX);
    }

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
    memset(&VDI, 0, sizeof(VDI_CALL));
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

    OPENAL_create_buffers(AIL_preference[MDI_SEQUENCES]);

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

    OPENAL_create_sources_for_sequences(mdidrv);

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

    smp_rate = 22050;

#if LBS_ENABLE_WILDMIDI
    i = WildMidi_Init("conf/midipats.cfg", smp_rate, WM_MO_ENHANCED_RESAMPLING);
    if (i < 0) {
        AIL_set_error("Cannot init music - invalid/missing WildMIDI config");
        AIL_set_error(WildMidi_GetError());
        AIL_call_driver(mdidrv->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
        mdidrv->drvr->initialized = 0;
        AIL_MEM_free_lock(mdidrv->sequences, mdidrv->n_sequences * sizeof(SNDSEQUENCE));
        AIL_MEM_free_lock(mdidrv, sizeof(MDI_DRIVER));
        return NULL;
    }
    WildMidi_MasterVolume(100);
#endif

    mdidrv->system_data[0] = smp_rate;

    // Allocate timer for XMIDI sequencing
    mdidrv->timer = AIL_register_timer(XMI_serve);
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

    return mdidrv;
}

/** Map sequence's logical channel to desired physical output channel.
 */
void AIL2OAL_API_map_sequence_channel(SNDSEQUENCE *seq, int32_t seq_channel, int32_t new_channel)
{
    if (seq == NULL)
        return;

    // Redirect output on this sequence's channel to new channel
    seq->chan_map[seq_channel-1] = new_channel-1;

    // If channel is locked by API or other sequence, reassign it to
    // this sequence so it's not inhibited from playing
    if ((seq->driver->lock  [new_channel-1] == 1) &&
      (seq->driver->locker[new_channel-1] != seq))
        seq->driver->locker[new_channel-1] = seq;
}

void XMI_send_channel_voice_message(SNDSEQUENCE *seq, int32_t status,
        int32_t data_1, int32_t data_2, int32_t ICA_enable)
{
    int32_t st, i;
    int32_t phys, log;
    MDI_DRIVER *mdidrv;
    int32_t result;

    // Get driver for sequence
    mdidrv = seq->driver;

    // Translate logical to physical channel
    st = status & 0xf0;
    log = status & 0x0f;

    phys = seq->chan_map[log];

    // If indirect controller override active, substitute indirect
    // controller value for data_2, and cancel indirect override
    if ((st == MDI_EV_CONTROL) && (ICA_enable) && (seq->shadow.indirect[log] != -1))
    {
        data_2 = seq->shadow.indirect[log];
        seq->shadow.indirect[log] = -1;
    }

    // Update local MIDI status log
    if ((st == MDI_EV_CONTROL) || (st == MDI_EV_PROGRAM) || (st == MDI_EV_PITCH))
    {
        XMI_write_log(&seq->shadow, st | log, data_1, data_2);
    }

    // If this is a Control Change event, handle special XMIDI controllers
    // and extended features
    //
    // Controller handlers should 'break' to pass message on to driver, or
    // 'return' if message is not to be transmitted
    if (st == MDI_EV_CONTROL)
    {
        switch (data_1)
        {
        case MDI_CTR_INDIRECT_C_PFX:
            if (seq->ICA)
                seq->shadow.indirect[log] = seq->ICA[data_2];
            break;

        case MDI_CTR_CALLBACK_PFX:
            if (seq->prefix_callback != NULL)
                seq->shadow.indirect[log] = seq->prefix_callback(seq, log, data_2);
            break;

        case MDI_CTR_PB_RANGE:
            XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | log, MDI_CTR_RPN_LSB, 0, 0);

            XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | log, MDI_CTR_RPN_MSB, 0, 0);

            XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | log, MDI_CTR_DATA_LSB, 0, 0);
            break;

        case MDI_CTR_PART_VOLUME:
            data_2 = (data_2 * seq->volume * mdidrv->master_volume) / (127 * 127);
            if (data_2 > 127)
                data_2 = 127;
            if (data_2 < 0)
                data_2 = 0;
            break;

        case MDI_CTR_CLEAR_BEAT_BAR:
            seq->beat_count = 0;
            seq->measure_count = 0;
            seq->beat_fraction = 0;
            seq->beat_fraction -= seq->time_fraction;

            // If beat/bar callback function active, trigger it
            if (seq->beat_callback != NULL)
            {
                seq->beat_callback(mdidrv, seq, 0, 0);
            }
            return;

        case MDI_CTR_CALLBACK_TRIG:
            if (seq->trigger_callback != NULL)
            {
                seq->trigger_callback(seq, log, data_2);
            }
            return;

        case MDI_CTR_FOR_LOOP:
            return; // DISABLED - we re-used FOR_ptrs array in this wrapper

        case MDI_CTR_NEXT_LOOP:
            return; // DISABLED - we re-used FOR_ptrs array in this wrapper

        case MDI_CTR_SEQ_BRANCH:
            AIL_branch_index(seq, data_2);
            return;

        case MDI_CTR_CHAN_PROTECT:
            // If channel is already locked, it's too late to protect it
            if (mdidrv->lock[phys] == 1)
                return;
            // Otherwise, set 0 (UNLOCKED, and by implication, UNPROTECTED)
            // or 2 (PROTECTED)
            if (data_2 < 64)
                mdidrv->lock[phys] = 0;
            else
                mdidrv->lock[phys] = 2;
            return;

        case MDI_CTR_CHAN_LOCK:
            if (data_2 >= 64)
            {
                // Lock a physical channel (1-based), if possible
                i = AIL_lock_channel(mdidrv);
                if (!i)
                    return;
                // Map sequence channel (0-based) to locked physical
                // channel (1-based)
                AIL_map_sequence_channel(seq, log + 1, i);
                // Keep track of which sequence locked the channel, so
                // other sequences can be inhibited from writing to it
                mdidrv->locker[i - 1] = seq;
            }
            else
            {
                // Channel must be locked in order to release it
                if (mdidrv->lock[phys] != 1)
                    return;
                // Turn all notes off in channel
                XMI_flush_channel_notes(seq, log);
                // Release locked physical channel (1-based)
                AIL_release_channel(mdidrv, phys + 1);
                // Re-establish normal physical channel mapping
                // for logical channel
                AIL_map_sequence_channel(seq, log + 1, log + 1);
            }
            return;
        }
    }

    // If this physical channel is locked by the API or by another
    // sequence, return
    if ((mdidrv->lock[phys] == 1) && (mdidrv->locker[phys] != seq))
        return;

    // Keep track of overall physical channel note counts
    if (st == MDI_EV_NOTE_ON)
        ++mdidrv->notes[phys];
    else if (st == MDI_EV_NOTE_OFF)
        --mdidrv->notes[phys];

    // Keep track of most recent sequence to use channel
    mdidrv->user[phys] = seq;

    // If logical channel muted with XMIDI Channel Mute controller (107),
    // return without transmitting note-on events
    if ((st == MDI_EV_NOTE_ON) && (seq->shadow.c_mute[log] >= 64))
        return;

    // Allow application a chance to process the event...
    if (mdidrv->event_trap != NULL)
    {
        result = mdidrv->event_trap(mdidrv, seq, st | phys, data_1, data_2);
        if (result)
            return;
    }

    // ...otherwise, transmit message to driver
    XMI_MIDI_message(mdidrv, st | phys, data_1, data_2);
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

void XMI_update_volume(SNDSEQUENCE *seq)
{
    int32_t ch;

    for (ch=0; ch < AIL_NUM_CHANS; ch++)
    {
      // Skip channels with no volume controller history
      if (seq->shadow.vol[ch] == -1)
         continue;

      // Retransmit volume values to permit volume scaling
      XMI_send_channel_voice_message(seq, MDI_EV_CONTROL | ch,
              MDI_CTR_PART_VOLUME, seq->shadow.vol[ch], 0);
    }
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

/** Update XMIDI playback when playing natively within AIL library.
 *
 * This wrapper is unable to really play the MIDI without relying on
 * an external synthesizer, so this is mostly for reference.
 *
 * @return Gives 0 if the sequence is still playing, 1 if we're done,
 *     2 if there was a time or resource consuming event and we should
 *     skip further sequences processing in this tick.
 */
int32_t XMI_playback_update(SNDSEQUENCE *seq)
{
    MDI_DRIVER *mdidrv = seq->driver;
    int32_t i, j, sequence_done;

    sequence_done = 0;

    // Add tempo percent to tempo overflow counter
    seq->tempo_error += seq->tempo_percent;

    // Execute interval zero, one, or more times, depending on tempo DDA
    // count
    while (seq->tempo_error >= 100)
    {
        int32_t q, t;
        uint32_t channel, status, type, len;
        uint8_t const *ptr;
        uint8_t const *event;

        seq->tempo_error -= 100;

        // Decrement note times and turn off any expired notes
        if (seq->note_count > 0)
        {
            for (i = 0; i < AIL_MAX_NOTES; i++)
            {
                if (seq->note_chan[i] == -1)
                    continue;

                if (--seq->note_time[i] > 0)
                    continue;

                // Note expired -- send MIDI Note Off message
                XMI_send_channel_voice_message(seq, seq->note_chan[i] | MDI_EV_NOTE_OFF,
                    seq->note_num [i], 0, 0);

                // Release queue entry, decrement sequence note count,
                // and exit loop if no active sequence notes left
                seq->note_chan[i] = -1;

                if (--seq->note_count == 0)
                    break;
            }
        }

        // Decrement interval delta-time count and process next interval if ready
        if (--seq->interval_count <= 0)
        {
            // Fetch events until next interval's delta-time byte (< 0x80)
            while ((!sequence_done) && ((status = *seq->EVNT_ptr) >= 0x80))
            {
                switch (status)
                {
                // Process MIDI meta-event
                case MDI_EV_META:
                    type = *(seq->EVNT_ptr+1);

                    seq->EVNT_ptr += 2;
                    len = XMI_read_VLN((const uint8_t **)&seq->EVNT_ptr);

                    switch (type)
                    {
                    // End-of-track event (XMIDI end-of-sequence)
                    case MDI_META_EOT:
                        // Set sequence_done to inhibit post-interval
                        // processing
                        sequence_done = 1;

                        // If loop count == 0, loop indefinitely
                        //
                        // Otherwise, decrement loop count and, if the
                        // result is not zero, return to beginning of
                        // sequence
                        if ((seq->loop_count == 0)
                            ||
                            (--seq->loop_count != 0))
                        {
                            seq->EVNT_ptr = (uint8_t *) seq->EVNT + 8;

                            seq->beat_count = 0;
                            seq->measure_count = -1;
                            seq->beat_fraction = 0;

                            // If beat/bar callback function active,
                            // trigger it
                            if (seq->beat_callback != NULL)
                                seq->beat_callback(seq->driver, seq, 0, 0);

                            break;
                        }

                        // Otherwise, stop sequence and set status
                        // to SNDSEQ_DONE
                        //
                        // Reset loop count to 1, to enable unlooped replay
                        seq->loop_count = 1;

                        AIL_stop_sequence(seq);
                        seq->status = SNDSEQ_DONE;

                        // Invoke end-of-sequence callback function, if any
                        if (seq->EOS != NULL)
                            seq->EOS(seq);
                        break;

                    // Tempo event
                    case MDI_META_TEMPO:
                        // Calculate tempo as 1/16-uS per MIDI
                        // quarter-note
                        t = ((int32_t) *(seq->EVNT_ptr ) << 16) +
                        ((int32_t) *(seq->EVNT_ptr+1) << 8 ) +
                        ((int32_t) *(seq->EVNT_ptr+2) );

                        seq->time_per_beat = t * 16;
                        break;

                    // Time signature event
                    case MDI_META_TIME_SIG:
                        // Fetch time numerator
                        seq->time_numerator = *seq->EVNT_ptr;

                        // Fetch time denominator: 0 = whole note,
                        // 1 = half-note, 2 = quarter-note, 3 = eighth-note...
                        t = *(seq->EVNT_ptr+1) - 2;

                        // Calculate beat period in terms of quantization
                        // rate
                        q = 16000000L / AIL_preference[MDI_SERVICE_RATE];

                        if (t < 0) {
                            t = -t;
                            seq->time_fraction = (q >> t);
                        } else {
                            seq->time_fraction = (q << t);
                        }

                        // Predecrement beat fraction for this interval;
                        // signal beginning of new measure
                        seq->beat_fraction = 0;
                        seq->beat_fraction -= seq->time_fraction;

                        seq->beat_count = 0;
                        seq->measure_count++;

                        // If beat/bar callback function active,
                        // trigger it
                        if (seq->beat_callback != NULL)
                            seq->beat_callback(seq->driver, seq, seq->beat_count, seq->measure_count);

                        break;
                    }

                    seq->EVNT_ptr += len;
                    break;

                // Process MIDI System Exclusive message
                case MDI_EV_SYSEX:
                case MDI_EV_ESC:
                    // Read message length and copy data to buffer
                    ptr = (uint8_t *) seq->EVNT_ptr + 1;

                    len = XMI_read_VLN((const uint8_t **)&ptr);

                    len += (int32_t) ((uint32_t)ptr - (uint32_t)seq->EVNT_ptr);

                    XMI_sysex_message(mdidrv, seq->EVNT_ptr, len);

                    seq->EVNT_ptr += len;
                    break;

                    // Process MIDI channel voice message
                default:
                    event = seq->EVNT_ptr;
                    channel = status & 0x0f;
                    status = status & 0xf0;

                    // Transmit message with ICA override enabled
                    XMI_send_channel_voice_message(seq, *seq->EVNT_ptr,
                        *(seq->EVNT_ptr+1), *(seq->EVNT_ptr+2), 1);

                    // Index next event
                    //
                    // Allocate note queue entries for Note On messages
                    if (status != MDI_EV_NOTE_ON)
                    {
                        // If this was a control change event which caused
                        // a branch to take place -- either a FOR/NEXT
                        // controller or a user callback with an API branch
                        // call -- then don't skip the current event
                        if (event == seq->EVNT_ptr)
                            seq->EVNT_ptr += XMI_message_size(*seq->EVNT_ptr);
                    }
                    else
                    {
                        // Find free note queue entry
                        for (i=0; i < AIL_MAX_NOTES; i++) {
                            if (seq->note_chan[i] == -1)
                                break;
                        }

                        // Shut down sequence if note queue overflows
                        //
                        // Should never happen since excessive polyphony is
                        // trapped by MIDIFORM
                        if (i == AIL_MAX_NOTES) {
                            AIL_set_error("Internal note queue overflow.");
                            AIL_stop_sequence(seq);
                            seq->status = SNDSEQ_DONE;
                            XMI_serve_entry = 0;
                            return 2;
                        }

                        // Increment sequence-based note counter
                        //
                        // Record note's channel, number, and duration
                        ++seq->note_count;

                        seq->note_chan[i] = channel;
                        seq->note_num [i] = *(seq->EVNT_ptr+1);

                        seq->EVNT_ptr += 3;
                        seq->note_time[i] = XMI_read_VLN((const uint8_t **)&seq->EVNT_ptr);
                    }
                    break;
                }
            }

            // Terminate this interval and set delta-time count to skip
            // next 0-127 intervals
            if (!sequence_done)
                seq->interval_count = *seq->EVNT_ptr++;
        }

        if (!sequence_done)
        {
            // Advance beat/bar counters
            seq->beat_fraction += seq->time_fraction;

            if (seq->beat_fraction >= seq->time_per_beat)
            {
                seq->beat_fraction -= seq->time_per_beat;
                ++seq->beat_count;

                if (seq->beat_count >= seq->time_numerator) {
                    seq->beat_count = 0;
                    ++seq->measure_count;
                }

                // If beat/bar callback function active, trigger it
                if (seq->beat_callback != NULL) {
                    AIL_sequence_position(seq, &i, &j);
                    seq->beat_callback(seq->driver, seq, i, j);
                }
            }
        }
    }
    return sequence_done;
}

/** Timer interrupt routine for XMIDI sequencing.
 */
void XMI_serve(void *clientval)
{
    MDI_DRIVER *mdidrv = clientval;
    SNDSEQUENCE *seq;
    int32_t n, sequence_done;

    // Exit at once if service disabled
    if (mdidrv->disable)
        return;

    // Disallow re-entrant calls (but leave interrupts enabled so that
    // .DIG processing can run in a separate thread)
    if (XMI_serve_entry)
        return;

    XMI_serve_entry = 1;

#if defined(DOS)||defined(GO32)
    // No pre-process call was required with AIL internal synth support
#else
    // Unqueue finished OAL buffers
    OPENAL_unqueue_finished_mdi_sequences(mdidrv);
#endif

    // Process all active sequences
    for (n = mdidrv->n_sequences,seq = &mdidrv->sequences[0]; n; --n,++seq)
    {
        // Skip sequence if stopped, finished, or not allocated
        if (seq->status != SNDSEQ_PLAYING)
            continue;

        // Bump sequence interval number counter
        ++seq->interval_num;

#if defined(DOS)||defined(GO32)
        // Play by directly processing MIDI notes
        sequence_done = XMI_playback_update(seq);
#else
        // Do software synth and queue new buffers on OAL sources
        sequence_done = OPENAL_update_mdi_sequence(seq);
#endif

        if (sequence_done == 2)
            break;

        if (!sequence_done)
        {
            // Update volume ramp, if any
            if (seq->volume != seq->volume_target)
            {
                seq->volume_accum += seq->driver->interval_time;

                while (seq->volume_accum >= seq->volume_period)
                {
                    seq->volume_accum -= seq->volume_period;

                    if (seq->volume_target > seq->volume)
                        ++seq->volume;
                    else
                        --seq->volume;
                    if (seq->volume == seq->volume_target)
                        break;
                }

                // Update volume controllers once every 8 intervals
                // to avoid generating excessive MIDI traffic
                if (!(seq->interval_num & 0x07))
                    XMI_update_volume(seq);
            }

            // Update tempo ramp, if any
            if (seq->tempo_percent != seq->tempo_target)
            {
                seq->tempo_accum += seq->driver->interval_time;

                while (seq->tempo_accum >= seq->tempo_period)
                {
                    seq->tempo_accum -= seq->tempo_period;

                    if (seq->tempo_target > seq->tempo_percent)
                        ++seq->tempo_percent;
                    else
                        --seq->tempo_percent;

                    if (seq->tempo_percent == seq->tempo_target)
                        break;
                }
            }
        }
    }

    // Flush MIDI event buffer at least once every timer tick
    XMI_flush_buffer(mdidrv);

    XMI_serve_entry = 0;
}

void AIL2OAL_API_set_XMIDI_master_volume(MDI_DRIVER *mdidrv, int32_t master_volume)
{
    SNDSEQUENCE *seq;
    int32_t i;

    // Set new volume; return if redundant setting
    if (mdidrv->master_volume == master_volume)
       return;

    mdidrv->master_volume = master_volume;

    // Force all sequences to update their volume controllers
    MSSLockedIncrementPtr(mdidrv->disable);

    for (i = mdidrv->n_sequences, seq = &mdidrv->sequences[0]; i; i--,seq++)
    {
        if ((seq->status != SNDSEQ_PLAYING) && (seq->status != SNDSEQ_PLAYINGRELEASED))
            continue;
        XMI_update_volume(seq);
    }

    MSSLockedDecrementPtr(mdidrv->disable);
}

void AIL2OAL_API_sequence_position(SNDSEQUENCE *seq, int32_t *beat, int32_t *measure)
{
    int32_t nbeat, nmeas, nbfrac;
    int i;

    if (seq == NULL)
        return;

    seq->driver->disable++;

    // Advance beat/measure count
    nbeat = seq->beat_count;
    nmeas = seq->measure_count;
    nbfrac = seq->beat_fraction;
    for (i = 0; i < AIL_preference[MDI_QUANT_ADVANCE]; i++)
    {
        int32_t tpbeat;

        tpbeat = seq->time_per_beat;
        nbfrac += seq->time_fraction;
        if (nbfrac >= tpbeat)
        {
            nbeat++;
            nbfrac -= tpbeat;
            if (nbeat >= seq->time_numerator) {
                nmeas++;
                nbeat = 0;
            }
        }
    }
    if (nmeas < 0)
      nmeas = 0;

    if (measure != NULL)
      *measure = nmeas;
    if (beat != NULL)
      *beat = nbeat;

    seq->driver->disable--;
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
    int32_t i;

    // Lock timer services to prevent reentry
    AIL_lock();

    // Look for an unallocated sequence structure
    for (i=0; i < mdidrv->n_sequences; i++)
    {
        if (mdidrv->sequences[i].status == SNDSEQ_FREE)
            break;
    }

    // If all structures in use, return NULL
    if (i == mdidrv->n_sequences) {
        AIL_set_error("Out of sequence handles.");
        AIL_unlock();
        return NULL;
    }

    seq = &mdidrv->sequences[i];

    // Initialize sequence
    seq->status = SNDSEQ_DONE;
    XMI_init_sequence_state(seq);
    // Initialize end-of-sequence callback
    seq->EOS = NULL;

    // Clear pointers which we reuse for software synth
    for (i=0; i < AIL_FOR_NEST; i++)
        seq->FOR_ptrs[i] = NULL;
    seq->ICA = NULL;

    AIL_unlock();
    return seq;
}

/** Free a SNDSEQUENCE structure for later allocation.
 */
void AIL2OAL_API_release_sequence_handle(SNDSEQUENCE *seq)
{
    if (seq == NULL)
        return;

    // Turn off all playing notes in sequence; release all channels
    AIL_stop_sequence(seq);
    // Set 'free' flag
    seq->status = SNDSEQ_FREE;
#if LBS_ENABLE_WILDMIDI
    // Release the WildMidi handle
    if (seq->ICA != NULL)
        WildMidi_Close(seq->ICA);
#endif
    if (seq->FOR_ptrs[0] != NULL)
        AIL_MEM_free_lock(seq->FOR_ptrs[0], SOUND_MAX_BUFSIZE);
}

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(const char *fname, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;
    AIL_DRIVER *drvr;
    int32_t *driver_image;

#if defined(DOS)||defined(GO32)
    // Open the driver file
    driver_image = (int32_t*) AIL_file_read(fname, FILE_READ_WITH_SIZE);
    if ((driver_image == NULL) && (AIL_redist_directory[0] != '\0'))
    {
        char fn[256];
        strcpy(fn, AIL_redist_directory);
        if (fn[strlen(fn)-1] != '/')
            strcat(fn, "/");
        strcat(fn, fname);

        driver_image = (int32_t*) AIL_file_read(fn, FILE_READ_WITH_SIZE);
    }
#else
    // Prepare fake driver data; make sure it has at least 7 bytes beyond size
    // to allow magic value check
    driver_image = (int32_t*) AIL_MEM_alloc_lock(sizeof(struct VDI_HDR) + 4);
    if (driver_image != NULL) {
        memset(driver_image, 0, sizeof(struct VDI_HDR) + 4);
        driver_image[0] = sizeof(struct VDI_HDR);
        strcpy((char*)&driver_image[1], "AIL3MDI");
    }
#endif
    if (driver_image == NULL) {
        AIL_set_error("Driver file not found.");
        return NULL;
    }

    drvr = AIL_install_driver((uint8_t*)(&driver_image[1]), driver_image[0]);

    AIL_MEM_free_lock(driver_image, driver_image[0] + 4);

    if (drvr == NULL)
        return NULL;

    mdidrv = XMI_construct_MDI_driver(drvr, iop);
    if (mdidrv == NULL)
        AIL_uninstall_driver(drvr);

    // Chain of MDI drivers is not preserved by this wrapper, but its head is available
    MDI_first = mdidrv;
    return mdidrv;
}

void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv)
{
    AIL_uninstall_driver(mdidrv->drvr);
    // The uninstall will call destructor to free MDI_DRIVER as well, so we're done
}

MDI_DRIVER *AIL2OAL_API_open_XMIDI_driver(uint32_t flags)
{
    // Since both Windows and old DOS API should work, there is no reason
    // not to call the DOS initialization here
    return AIL2OAL_API_install_MDI_driver_file("opl3.mdi", NULL);
}

void AIL2OAL_API_close_XMIDI_driver(MDI_DRIVER *mdidrv)
{
    AIL2OAL_API_uninstall_MDI_driver(mdidrv);
}

int32_t AIL2OAL_API_init_sequence(SNDSEQUENCE *seq, const void *start,  int32_t sequence_num)
{
    const uint8_t *image;
    const uint8_t *end;
    uint32_t len;
    int32_t i;
#if ENABLE_TIMBRE
    TIMB_chunk *T;
    static uint8_t TIMB[sizeof(seq->driver->DST->MIDI_data)];
    uint32_t bank, patch;
    VDI_CALL VDI;
    int32_t j;
#endif

    if (seq == NULL)
        return 0;

    seq->status = SNDSEQ_DONE;

    // Find requested sequence in XMIDI image
    image = XMI_find_sequence(start, sequence_num);
    if (image == NULL) {
        AIL_set_error("Invalid XMIDI sequence.");
        return 0;
    }

    // Locate IFF chunks within FORM XMID:
    //
    // TIMB = list of bank/patch pairs needed to play sequence (optional)
    // RBRN = list of branch target points (optional)
    // EVNT = XMIDI event list (mandatory)
    len = 8 + XMI_swap32(*(uint32_t*)(image + 4));
    end = image + len;
    image += 12;

    seq->TIMB = NULL;
    seq->RBRN = NULL;
    seq->EVNT = NULL;

    while (image < end)
    {
        if (memcmp(image, "TIMB", 4) == 0)
            seq->TIMB = (uint8_t *)image;
        if (memcmp(image, "RBRN", 4) == 0)
            seq->RBRN = (uint8_t *)image;
        if (memcmp(image, "EVNT", 4) == 0)
            seq->EVNT = (uint8_t *)image;

        image += 8 + XMI_swap32(*(uint32_t*)(image + 4));
    }

    // Sequence must contain EVNT chunk
    if (seq->EVNT == NULL) {
        AIL_set_error("Invalid XMIDI sequence.");
        return 0;
    }

#if LBS_ENABLE_WILDMIDI
    // Release the previous WildMidi handle
    if (seq->ICA != NULL)
        WildMidi_Close(seq->ICA);
#endif

    // Initialize sequence callback and state data
    seq->ICA = NULL;
    seq->prefix_callback = NULL;
    seq->trigger_callback = NULL;
    seq->beat_callback = NULL;
    seq->EOS = NULL;
    seq->loop_count = 1;

    XMI_rewind_sequence(seq);

    // Initialize volume and tempo
    seq->volume = AIL_preference[MDI_DEFAULT_VOLUME];
    seq->volume_target = AIL_preference[MDI_DEFAULT_VOLUME];
    seq->volume_period = 0;
    seq->volume_accum = 0;

    seq->tempo_percent = 100;
    seq->tempo_target = 100;
    seq->tempo_period = 0;
    seq->tempo_accum = 0;
    seq->tempo_error = 0;

    len = XMI_whole_size(start);
#if LBS_ENABLE_WILDMIDI
    // The (otherwise unused) ICA pointer will be our WildMIDI handle
    seq->ICA = WildMidi_OpenBuffer(start, len);
    if (seq->ICA == NULL) {
        AIL_set_error("Could not init WildMIDI SNDSEQUENCE");
        AIL_set_error(WildMidi_GetError());
        return 0;
    }
#endif

#if LBS_ENABLE_WILDMIDI
    // Move to the selected sequence
    i = sequence_num;
    while (i > 0) {
        WildMidi_SongSeek(seq->ICA, 1);
        i--;
    }
#endif

    // Reuse one of (otherwise unused) FOR_ptrs for sw synth buffer
    if (seq->FOR_ptrs[0] == NULL)
        seq->FOR_ptrs[0] = AIL_MEM_alloc_lock(SOUND_MAX_BUFSIZE);

    // If no TIMB chunk present, return success
    if (seq->TIMB == NULL)
        return 1;

#if ENABLE_TIMBRE
    // Make modifiable copy of TIMB chunk
    len = 8 + XMI_swap32(*((uint32_t*)seq->TIMB + 1));
    if (len > sizeof(seq->driver->DST->MIDI_data))
        len = sizeof(seq->driver->DST->MIDI_data);
    memcpy(TIMB, seq->TIMB, len);

    T = (TIMB_chunk*)TIMB;

    // If timbre-request callback function registered, pass each bank/patch
    // pair to the function to see if it has to be requested from the driver
    //
    // Remove references to any timbres handled by the callback function
    if (seq->driver->timbre_trap != NULL)
    {
        i = 0;

        while (i < T->n_entries)
        {
            patch = ((uint32_t) T->timbre[i]) & 0xff;
            bank = (((uint32_t) T->timbre[i]) & 0xff00) >> 8;

            if (!seq->driver->timbre_trap(seq->driver, bank, patch))
            {
                // Timbre request was not handled by the callback - check next timbre
                ++i;
            }
            else
            {
                // Timbre request was handled - excise from TIMB chunk and test next timbre
                for (j = i + 1; j < T->n_entries; j++)
                    T->timbre[j - 1] = T->timbre[j];

                T->n_entries--;

                if (T->lsb < 2) {
                    T->lsb -= 2;
                    T->msb--;
                } else {
                    T->lsb -= 2;
                }
            }
        }
    }

    // If all timbre requests have been handled, or the sequence contains no
    // timbre references, return success
    //
    // Otherwise, call driver to request timbre set installation
    if (T->n_entries == 0)
        return 1;

    // If called from background function (not recommended), return without
    // attempting to load timbres from disk
    if (AIL_background()) {
        AIL_set_error("No timbres loaded.");
        return -1;
    }

    // Disable XMIDI service while accessing MIDI_data[] buffer
    seq->driver->disable++;

    // Copy TIMB chunk to driver's XMIDI buffer, and call driver
    // MDI_INSTALL_T_SET function
    XMI_flush_buffer(seq->driver);

    memcpy(seq->driver->DST->MIDI_data, T, sizeof(seq->driver->DST->MIDI_data));

    AIL_call_driver(seq->driver->drvr, MDI_INSTALL_T_SET, NULL, &VDI);

    // Re-enable XMIDI service and check for errors
    seq->driver->disable--;

    if (!VDI.AX) {
        set_timbre_error(VDI.BX >> 8, VDI.BX & 0xff);
        return -1;
    }
#else
    AIL_set_error("Timbres not supported, ignoring.");
#endif

    return 1;
}

void AIL2OAL_API_start_sequence(SNDSEQUENCE *seq)
{
    if (seq == NULL)
        return;

    // Make sure sequence has been allocated
    if (seq->status == SNDSEQ_FREE)
        return;

    AIL_stop_sequence(seq);
    // Rewind sequence to beginning
    XMI_rewind_sequence(seq);
    seq->status = SNDSEQ_PLAYING;
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

    OPENAL_stop_sequence(seq);

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
    MDI_DRIVER *mdidrv;
    int32_t log;
    int32_t ch;

    if (seq == NULL)
        return;

    // Make sure sequence has been previously stopped
    if (seq->status == SNDSEQ_STOPPED)
    {
        // Re-establish channel locks
        mdidrv = seq->driver;

        for (log = 0; log < AIL_NUM_CHANS; log++)
        {
            if (seq->shadow.c_lock[log] >= 64)
            {
                ch = AIL_lock_channel(mdidrv) - 1;
                seq->chan_map[log] = (ch == -1) ? log : ch;
            }
        }

        // Re-establish logged controller values (except
        // Channel Lock, which was done above)
        for (log=0; log < AIL_NUM_CHANS; log++)
            XMI_refresh_channel(seq,log);

        seq->status = SNDSEQ_PLAYING;
    }
    else if (seq->status == SNDSEQ_DONE)
    {
        seq->status = SNDSEQ_PLAYING;
    }
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

void AIL2OAL_API_set_sequence_tempo(SNDSEQUENCE *seq, int32_t tempo, int32_t milliseconds)
{
    if (seq == NULL)
        return;

    // Disable XMIDI service while altering tempo control data
    MSSLockedIncrementPtr(seq->driver->disable);

    // Set new tempo target; exit if no change
    seq->tempo_target = tempo;

    if (seq->tempo_percent == seq->tempo_target) {
        MSSLockedDecrementPtr(seq->driver->disable);
        return;
    }

    // Otherwise, set up tempo ramp
    if (milliseconds == 0) {
        seq->tempo_percent = seq->tempo_target;
    } else {
        seq->tempo_period = (milliseconds * 1000L) /
            labs(seq->tempo_percent - seq->tempo_target);
        seq->tempo_accum = 0;
    }

   // Restore XMIDI service and return
   MSSLockedDecrementPtr(seq->driver->disable);
}

void AIL2OAL_API_set_sequence_volume(SNDSEQUENCE *seq, int32_t volume, int32_t milliseconds)
{
    if (seq == NULL)
        return;

    // Disable XMIDI service while altering volume control data
    MSSLockedIncrementPtr(seq->driver->disable);

    // Set new volume target; exit if no change
    seq->volume_target = volume;

    if (seq->volume == seq->volume_target) {
        MSSLockedDecrementPtr(seq->driver->disable);
        return;
    }

    // Otherwise, set up volume ramp
    if (milliseconds == 0) {
        seq->volume = seq->volume_target;
    } else {
        seq->volume_period = (milliseconds * 1000L) /
            labs(seq->volume - seq->volume_target);
        seq->volume_accum = 0;
    }

    // Restore interrupt state, update channel volume settings, and exit
    XMI_update_volume(seq);

    // Restore XMIDI service and return
    MSSLockedDecrementPtr(seq->driver->disable);
}

uint32_t AIL2OAL_API_sequence_status(SNDSEQUENCE *seq)
{
    if (seq == NULL)
        return 0;
    return seq->status;
}

int32_t AIL2OAL_API_sequence_loop_count(SNDSEQUENCE *seq)
{
    if (seq == NULL)
        return -1;
    return seq->loop_count;
}

void AIL2OAL_API_branch_index(SNDSEQUENCE *seq, uint32_t marker)
{
    // Not implemented in this wrapper
}

AILTRIGGERCB AIL2OAL_API_register_trigger_callback(SNDSEQUENCE *seq, AILTRIGGERCB callback)
{
    AILTRIGGERCB old;

    if (seq == NULL)
        return NULL;

    old = seq->trigger_callback;
    seq->trigger_callback = callback;

    return old;
}

AILSEQUENCECB AIL2OAL_API_register_sequence_callback(SNDSEQUENCE *seq, AILSEQUENCECB callback)
{
    AILSEQUENCECB old;

    if (seq == NULL)
        return NULL;

    old = seq->EOS;
    seq->EOS = callback;

    return old;
}

AILBEATCB AIL2OAL_API_register_beat_callback(SNDSEQUENCE *seq, AILBEATCB callback)
{
    AILBEATCB old;

    if (seq == NULL)
        return NULL;

    old = seq->beat_callback;
    seq->beat_callback = callback;

    return old;
}

AILEVENTCB AIL2OAL_API_register_event_callback(MDI_DRIVER *mdidrv, AILEVENTCB callback)
{
    AILEVENTCB old;

    old = mdidrv->event_trap;
    mdidrv->event_trap = callback;

    return old;
}

AILTIMBRECB AIL2OAL_API_register_timbre_callback(MDI_DRIVER *mdidrv, AILTIMBRECB callback)
{
    AILTIMBRECB old;

    if (mdidrv == NULL)
        return NULL;

    old = mdidrv->timbre_trap;
    mdidrv->timbre_trap = callback;
    return old;
}

void AIL2OAL_API_set_sequence_user_data(SNDSEQUENCE *seq, uint32_t index, intptr_t value)
{
    if (seq == NULL)
        return;
   seq->user_data[index] = value;
}

intptr_t AIL2OAL_API_sequence_user_data(SNDSEQUENCE *seq, uint32_t index)
{
    if (seq == NULL)
        return 0;
    return seq->user_data[index];
}

int32_t AIL2OAL_API_lock_channel(MDI_DRIVER *mdidrv)
{
    int32_t i,j;
    int32_t ch,best;
    SNDSEQUENCE *seq;

    // Disable XMIDI service while locking channel
    MSSLockedIncrementPtr(mdidrv->disable);

    // Search for highest channel # with lowest note activity,
    // skipping already-locked and protected physical channels
    ch = -1;
    best = LONG_MAX;

    for (i = AIL_MAX_LOCK_CHAN; i >= AIL_MIN_LOCK_CHAN; i--)
    {
        if (i == AIL_PERCUSS_CHAN)
            continue;

        if ((mdidrv->lock[i] == 1) ||
            (mdidrv->lock[i] == 2))
            continue;
        if (mdidrv->notes[i] < best) {
            best = mdidrv->notes[i];
            ch = i;
        }
    }

    // If no unprotected channels available, ignore
    // lock protection and try again
    if (ch == -1)
    {
        for (i = AIL_MAX_LOCK_CHAN; i >= AIL_MIN_LOCK_CHAN; i--)
        {
            if (i == AIL_PERCUSS_CHAN)
                continue;

            if (mdidrv->lock[i] == 1)
                continue;

            if (mdidrv->notes[i] < best) {
                best = mdidrv->notes[i];
                ch = i;
            }
        }
    }

    // If no unlocked channels available, return failure
    if (ch == -1) {
        MSSLockedDecrementPtr(mdidrv->disable);
        return 0;
    }

    // Otherwise, release sustain pedal and turn off all active notes in
    // physical channel, regardless of sequence
    XMI_MIDI_message(mdidrv, MDI_EV_CONTROL | ch, MDI_CTR_SUSTAIN, 0);

    for (i = mdidrv->n_sequences,seq = &mdidrv->sequences[0]; i; --i,++seq)
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
                seq->note_chan[j] | MDI_EV_NOTE_OFF, seq->note_num [j], 0, 0);
            seq->note_chan[j] = -1;
        }
    }

    // Lock channel
    // By default, API asserts ownership of channel (locker=NULL), and
    // last sequence to use channel is recorded as its original owner
    mdidrv->state[ch] = mdidrv->lock[ch];
    mdidrv->lock[ch] = 1;
    mdidrv->locker[ch] = NULL;
    mdidrv->owner[ch] = mdidrv->user[ch];

    // Return 1-based channel number to caller
    MSSLockedDecrementPtr(mdidrv->disable);

    return ch+1;
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

/** Transmit MIDI channel voice message via desired physical channel.
 *
 * This function disregards channel locking and other XMIDI features.
 */
void AIL2OAL_API_send_channel_voice_message(MDI_DRIVER *mdidrv, SNDSEQUENCE *seq,
        int32_t status, int32_t data_1, int32_t data_2)
{
    int32_t result;

    // Get driver handle to use (derive from sequence handle if driver NULL)
    if (mdidrv == NULL)
    {
        if (seq == NULL)
            return;
        mdidrv = seq->driver;
    }

    // Disable XMIDI service while accessing MIDI_data[] buffer
    MSSLockedIncrementPtr(mdidrv->disable);

    if (seq == NULL)
    {
        // If this is a Part Volume (7) controller, scale its value by the
        // driver's master volume setting
        if (((status & 0xf0) == MDI_EV_CONTROL) && (data_1 == MDI_CTR_PART_VOLUME))
        {
            data_2 = (data_2 * mdidrv->master_volume) / 127;

            if (data_2 > 127)
                data_2 = 127;
            if (data_2 < 0)
                data_2 = 0;
        }

        // If no sequence handle given, transmit message on physical channel
        // without XMIDI logging
        result = 0;

        if (mdidrv->event_trap != NULL) {
            mdidrv->event_trap(mdidrv, NULL, status, data_1, data_2);
        }

        if (!result)
            XMI_MIDI_message(mdidrv, status, data_1, data_2);
    }
    else
    {
        // Otherwise, perform logical-to-physical translation and XMIDI
        // interpretation based on sequence handle, when transmitting
        // message
        XMI_send_channel_voice_message(seq, status, data_1, data_2, 0);
    }

    // Reenable XMIDI service
    MSSLockedDecrementPtr(mdidrv->disable);
}

/** Unlock function, doubling as end of locked code.
 */
void AILXMIDI_end(void)
{
    if (!MDI_use_locked)
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

    MDI_use_locked = 0;
}

/******************************************************************************/
