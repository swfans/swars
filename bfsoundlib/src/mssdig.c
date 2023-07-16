/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssdig.c
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

#include "mssdig.h"
#include "memfile.h"
#include "miscutil.h"
#include "ail.h"
#include "ailss.h"
#include "aildebug.h"
#include "drv_oal.h"
/******************************************************************************/
extern size_t sound_free_buffer_count;

static uint32_t SS_serve_entry = 0;
static int32_t SS_serve_flags = 0;

/** Call device I/O verification function using current detection policy.
 */
int32_t SS_attempt_DIG_detection(DIG_DRIVER *digdrv, const SNDCARD_IO_PARMS *iop)
{
    SNDCARD_IO_PARMS *f;
    SNDCARD_IO_PARMS try;
    uint32_t i;

    // Set up working I/O params structure
    try = *iop;

    // If any needed parameters are not specified, use parameter
    // values from first factory-default IO structure
    if (digdrv->drvr->VHDR->num_IO_configurations > 0)
    {
        f = (SNDCARD_IO_PARMS *) (digdrv->drvr->VHDR->common_IO_configurations);

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
    digdrv->drvr->VHDR->IO = try;

    // Call detection function
    //
    // If application wishes to avoid 16-bit DMA channel usage,
    // attempt detection on 8-bit channels only
    //
    // Otherwise, try either/both channels
    {
        VDI_CALL VDI;
        if (AIL_preference[DIG_ALLOW_16_BIT_DMA] == 0)
            VDI.DX = DIG_DETECT_8_BIT_ONLY;
        else
            VDI.DX = DIG_DETECT_8_AND_16_BITS;
        return AIL_call_driver(digdrv->drvr, DRV_VERIFY_IO, &VDI, NULL);
    }
}

DIG_DRIVER *SS_construct_DIG_driver(AIL_DRIVER *drvr, const SNDCARD_IO_PARMS *iop)
{
    DIG_DRIVER *digdrv;
    SNDCARD_IO_PARMS use;
    int32_t i;
    int32_t detected;

    // Ensure that all AIL code and data is locked into memory
    AIL2OAL_start();
    assert(sizeof(SNDSAMPLE) == 0x894);

    // Allocate memory for DIG_DRIVER structure
    digdrv = (DIG_DRIVER *)AIL_MEM_alloc_lock(sizeof(*digdrv));
    if (digdrv == NULL) {
        AIL_set_error("Cannot alloc mem for DIG driver.");
        return NULL;
    }
    memset(digdrv, 0, sizeof(*digdrv));

    digdrv->drvr = drvr;

    // Reject driver if not of type .DIG
    if (digdrv->drvr->type != AIL3DIG) {
        AIL_set_error(".DIG driver required.");
        AIL_MEM_free_lock(digdrv, sizeof(*digdrv));
        return NULL;
    }

#if defined(DOS)||defined(GO32)
    // Get DDT and DST addresses, the DOS driver way
    {
        VDI_CALL VDI;
        AIL_call_driver(digdrv->drvr, DRV_GET_INFO, NULL, &VDI);
        digdrv->DDT = (DIG_DDT *) (((uint32_t) (VDI.DX) << 4) + (uint32_t)VDI.AX);
        digdrv->DST = (DIG_DST *) (((uint32_t) (VDI.CX) << 4) + (uint32_t)VDI.BX);
    }
#else
    // Allocate local DDT and DST structures
    digdrv->DDT = (DIG_DDT *)AIL_MEM_alloc_lock(sizeof(DIG_DDT));
    memset(digdrv->DDT, 0, sizeof(DIG_DDT));
    digdrv->DST = (DIG_DST *)AIL_MEM_alloc_lock(sizeof(DIG_DST));
    memset(digdrv->DST, 0, sizeof(DIG_DST));
    // Hard-code data for OpenAL playback
    digdrv->DDT->format_supported[DIG_F_STEREO_16] = 1;
    digdrv->DDT->format_data[DIG_F_STEREO_16].minimum_DMA_half_buffer_size = 2048;
    digdrv->DDT->format_data[DIG_F_STEREO_16].maximum_DMA_half_buffer_size = 2048;
    digdrv->DDT->format_data[DIG_F_STEREO_16].minimum_physical_sample_rate = 44100;
    digdrv->DDT->format_data[DIG_F_STEREO_16].nominal_physical_sample_rate = 44100;
    digdrv->DDT->format_data[DIG_F_STEREO_16].maximum_physical_sample_rate = 44100;
#endif

    // Initialize miscellaneous DIG_DRIVER members
    digdrv->buffer_flag = &(digdrv->DST->active_buffer);
    digdrv->last_buffer = -1;

    digdrv->playing = 0;
    digdrv->quiet = 0;

    digdrv->n_active_samples = 0;

    digdrv->master_volume = 127;

    // Verify hardware I/O parameters
    memset(&AIL_last_IO_attempt, -1, sizeof(SNDCARD_IO_PARMS));

    // If explicit IO_PARMS structure provided by application, try it
    // first
    detected = 0;
    if (iop != NULL) {
        AIL_last_IO_attempt = *iop;
        if (SS_attempt_DIG_detection(digdrv, iop)) {
            detected = 1;
            use = *iop;
        }
    }

    // Next, try device-specific environment string (if applicable)
    if (!detected)
    {
        iop = AIL_get_IO_environment(digdrv->drvr);
        if (iop != NULL) {
            AIL_last_IO_attempt = *iop;
            if (SS_attempt_DIG_detection(digdrv, iop)) {
                detected = 1;
                use = *iop;
            }
        }
    }

    // Finally, try all common_IO_configurations[] entries in driver
    if ((!detected) && (AIL_preference[AIL_SCAN_FOR_HARDWARE] == 1))
    {
        for (i = 0; i < digdrv->drvr->VHDR->num_IO_configurations; i++)
        {
            iop = &((SNDCARD_IO_PARMS *)
                (digdrv->drvr->VHDR->common_IO_configurations))[i];

            if (i == 0) {
                AIL_last_IO_attempt = *iop;
            }
            if (SS_attempt_DIG_detection(digdrv, iop)) {
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
        AIL_set_error("Digital sound hardware not found.");
        AIL_MEM_free_lock(digdrv, sizeof(DIG_DRIVER));
        return NULL;
    }

    OPENAL_create_buffers_for_samples(digdrv);

    // The pair of DMA half-buffers is not needed
    digdrv->DMA_buf = NULL;
    digdrv->DMA_seg = 0;
    digdrv->DMA_sel = 0;

    // Configure working DMA half-buffers
    if (!SS_configure_buffers(digdrv)) {
        if (digdrv->DMA_buf != NULL)
            AIL_MEM_free_DOS(digdrv->DMA_buf, digdrv->DMA_seg, digdrv->DMA_sel);
        AIL_MEM_free_lock(digdrv, sizeof(DIG_DRIVER));
        return NULL;
    }

    // Initialize device
    AIL_call_driver(digdrv->drvr, DRV_INIT_DEV, NULL, NULL);

    digdrv->drvr->initialized = 1;

    // Allocate build buffer
    digdrv->samples_per_buffer = digdrv->half_buffer_size /
        (digdrv->channels_per_sample * digdrv->bytes_per_channel);

    digdrv->channels_per_buffer = digdrv->half_buffer_size /
        digdrv->bytes_per_channel;

    digdrv->build_size = sizeof(uint32_t) * digdrv->channels_per_buffer;

    digdrv->build_buffer = (int32_t *)AIL_MEM_alloc_lock(digdrv->build_size);

    if (digdrv->build_buffer == NULL)
    {
        AIL_set_error("Could not allocate build buffer.");
        if (drvr->PM_ISR != -1)
            AIL_restore_USE16_ISR(drvr->PM_ISR);
        AIL_call_driver(digdrv->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
        digdrv->drvr->initialized = 0;
        if (digdrv->DMA_buf != NULL)
            AIL_MEM_free_DOS(digdrv->DMA_buf, digdrv->DMA_seg, digdrv->DMA_sel);
        AIL_MEM_free_lock(digdrv, sizeof(DIG_DRIVER));
        return NULL;
    }

    digdrv->n_samples = AIL_preference[DIG_MIXER_CHANNELS];

    // Allocate physical SNDSAMPLE structures for driver
    digdrv->samples = (SNDSAMPLE *)AIL_MEM_alloc_lock(sizeof(SNDSAMPLE) * digdrv->n_samples);

    if (digdrv->samples == NULL)
    {
        AIL_set_error("Could not allocate SNDSAMPLE structures.");
        if (drvr->PM_ISR != -1)
            AIL_restore_USE16_ISR(drvr->PM_ISR);
        AIL_call_driver(digdrv->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
        digdrv->drvr->initialized = 0;
        if (digdrv->DMA_buf != NULL)
            AIL_MEM_free_DOS(digdrv->DMA_buf, digdrv->DMA_seg, digdrv->DMA_sel);
        AIL_MEM_free_lock(digdrv->build_buffer, digdrv->build_size);
        AIL_MEM_free_lock(digdrv, sizeof(DIG_DRIVER));
        return NULL;
    }

    for (i=0; i < digdrv->n_samples; i++) {
        SNDSAMPLE *s = &digdrv->samples[i];
        memset(s, 0, sizeof(SNDSAMPLE));
        s->status = SNDSMP_FREE;
        s->driver = digdrv;
    }

    OPENAL_create_sources_for_samples(digdrv);

    // Allocate timer for DMA buffer service
    digdrv->timer = AIL_register_timer(SS_serve);

    if (digdrv->timer == -1)
    {
        AIL_set_error("Out of timer handles.");
        if (drvr->PM_ISR != -1)
            AIL_restore_USE16_ISR(drvr->PM_ISR);
        AIL_call_driver(digdrv->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
        digdrv->drvr->initialized = 0;
        if (digdrv->DMA_buf != NULL)
            AIL_MEM_free_DOS(digdrv->DMA_buf, digdrv->DMA_seg, digdrv->DMA_sel);
        AIL_MEM_free_lock(digdrv->samples, sizeof(SNDSAMPLE) * digdrv->n_samples);
        AIL_MEM_free_lock(digdrv->build_buffer, digdrv->build_size);
        AIL_MEM_free_lock(digdrv, sizeof(DIG_DRIVER));
        return NULL;
    }

    AIL_set_timer_user(digdrv->timer, digdrv);
    AIL_set_timer_frequency(digdrv->timer, AIL_preference[DIG_SERVICE_RATE]);
    AIL_start_timer(digdrv->timer);
    sound_fake_timer_initialize();

    // Set destructor handler and descriptor
    digdrv->drvr->destructor = (AILTIMERCB)SS_destroy_DIG_driver;
    digdrv->drvr->descriptor = digdrv;

    // Flush DMA buffers with silence
    SS_flush(digdrv);
    SS_copy(digdrv, digdrv->DMA[0]);
    SS_copy(digdrv, digdrv->DMA[1]);

    return digdrv;
}

void SS_destroy_DIG_driver(DIG_DRIVER *digdrv)
{
    int32_t i;

    // Halt DMA traffic and flush buffer with silence
    if (digdrv->playing)
    {
        SS_stop_DIG_driver_playback(digdrv);
        SS_flush(digdrv);
        SS_copy(digdrv, digdrv->DMA[0]);
        SS_copy(digdrv, digdrv->DMA[1]);
        AIL_delay(10);
    }

    // Stop buffer timer service
    AIL_release_timer_handle(digdrv->timer);

    // Release any open sample handles (to ensure that pipeline resources
    // are deallocated properly)
    for (i = 0; i < digdrv->n_samples; i++)
    {
        SNDSAMPLE *s;
        s = &digdrv->samples[i];
        if (s->status != SNDSMP_FREE)
            AIL_release_sample_handle(s);
    }

    OPENAL_free_sources_for_samples(digdrv);

    OPENAL_free_buffers_for_samples(digdrv);

    AIL_MEM_free_DOS(digdrv->DMA_buf, digdrv->DMA_seg, digdrv->DMA_sel);
    AIL_MEM_free_lock(digdrv->samples, sizeof(SNDSAMPLE) * digdrv->n_samples);
    AIL_MEM_free_lock(digdrv->build_buffer, 4 * digdrv->build_size);
#if defined(DOS)||defined(GO32)
    // No need to free DDT nor DST
#else
    // Free the local DDT and DST structures
    AIL_MEM_free_lock(digdrv->DDT, sizeof(DIG_DDT));
    AIL_MEM_free_lock(digdrv->DST, sizeof(DIG_DST));
#endif
    AIL_MEM_free_lock(digdrv, sizeof(DIG_DRIVER));
}

void SS_start_DIG_driver_playback(DIG_DRIVER *digdrv)
{
    VDI_CALL VDI;

    if (digdrv->playing)
        return;

    VDI.DX = (uint16_t)digdrv->hw_format;
    VDI.CX = (uint16_t)digdrv->DMA_rate;
    AIL_call_driver(digdrv->drvr, DIG_START_P_CMD, &VDI, NULL);
    digdrv->playing = 1;
}

void SS_stop_DIG_driver_playback(DIG_DRIVER *digdrv)
{
    if (!digdrv->playing)
        return;

    // Stop playback ASAP and return all buffers
    AIL_call_driver(digdrv->drvr, DIG_STOP_P_REQ, NULL, NULL);
    digdrv->playing = 0;
}

void SS_serve(void *clientval)
{
    DIG_DRIVER *digdrv = clientval;
    VDI_CALL    VDI;
    int32_t cnt,n;
    int32_t current;
    SNDSAMPLE *s;

    // Disallow re-entrant calls
    if (SS_serve_entry)
        return;

    SS_serve_entry = 1;

    // If desired, disable interrupts while manipulating DMA buffers
    if (AIL_preference[DIG_SS_LOCK])
        SS_serve_flags = AIL_disable_interrupts();

    // If driver not transmitting buffer data, return
    if (!digdrv->playing) {
        if (AIL_preference[DIG_SS_LOCK])
            AIL_restore_interrupts(SS_serve_flags);
        SS_serve_entry = 0;
        return;
    }

    // Return immediately if buffer has not switched
    current = *digdrv->buffer_flag;

    if ((current == -1) || (current == digdrv->last_buffer)) {
        if (AIL_preference[DIG_SS_LOCK])
            AIL_restore_interrupts(SS_serve_flags);
        SS_serve_entry = 0;
        return;
    }

    digdrv->last_buffer = current;

    // Flush build buffer with silence
    SS_flush(digdrv);

    // Merge active samples (if any) into build buffer
    cnt = 0;

    for (n = digdrv->n_samples,s = &digdrv->samples[0]; n; --n,++s)
    {
        // Skip sample if stopped, finished, or not allocated
        if (s->status != SNDSMP_PLAYING)
            continue;
        ++cnt;
        // Convert sample to 16-bit signed format and mix with
        // contents of build buffer
        SS_stream_to_buffer(s);
    }

    // Set number of active samples
    digdrv->n_active_samples = cnt;

    // Copy build buffer contents to DMA buffer
    SS_copy(digdrv, digdrv->DMA[current ^ 1]);

    // Send buffer-service VSE callback to driver, if required
    if (digdrv->hw_mode_flags & DIG_BUFFER_SERVICE) {
        VDI.CX = 0;
        VDI.DX = (current ^ 1);
        AIL_call_driver(digdrv->drvr, DIG_VSE, &VDI, NULL);
    }

    // If no samples active for two consecutive interrupts,
    // request DMA halt
    if (digdrv->n_active_samples)
        digdrv->quiet = 0;
    else if (digdrv->quiet++ == 2)
        SS_stop_DIG_driver_playback(digdrv);

    // Restore interrupts and return
    if (AIL_preference[DIG_SS_LOCK])
        AIL_restore_interrupts(SS_serve_flags);

    SS_serve_entry = 0;
}

DIG_DRIVER *AIL2OAL_API_install_DIG_driver_file(const char *fname,
        const SNDCARD_IO_PARMS *iop)
{
    DIG_DRIVER *digdrv;
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
        strcpy((char*)&driver_image[1], "AIL3DIG");
    }
#endif
    if (driver_image == NULL)
    {
        AIL_set_error("Driver file not found.");
        return NULL;
    }

    drvr = AIL_install_driver((uint8_t*)(&driver_image[1]), driver_image[0]);

    AIL_MEM_free_lock(driver_image, driver_image[0] + 4);

    if (drvr == NULL)
        return NULL;

    digdrv = SS_construct_DIG_driver(drvr, iop);

    if (digdrv == NULL)
        AIL_uninstall_driver(drvr);

    return digdrv;
}

int32_t AIL2OAL_API_install_DIG_INI(DIG_DRIVER **digdrv)
{
    AIL_INI ini;

#if defined(DOS)||defined(GO32)
    // Attempt to read DIG_INI file
    if (!AIL_read_INI(&ini, "DIG.INI"))
    {
        AIL_set_error("Unable to open file DIG.INI.");
        return AIL_NO_INI_FILE;
    }
#else
    // Pretend we have a generic driver from DIG_INI
    memset(&ini, 0, sizeof(ini));
    sprintf(ini.driver_name, "%s", "SB16.DIG");
#endif

    *digdrv = AIL_install_DIG_driver_file(ini.driver_name,
                                     &ini.IO);

    if (*digdrv == NULL)
        return AIL_INIT_FAILURE;

    return AIL_INIT_SUCCESS;
}

void AIL2OAL_API_uninstall_DIG_driver(DIG_DRIVER *digdrv)
{
   AIL_uninstall_driver(digdrv->drvr);
}
/******************************************************************************/
