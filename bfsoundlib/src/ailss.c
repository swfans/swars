/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ailss.c
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
#include <assert.h>

#include "ailss.h"
#include "aildebug.h"
#include "ail.h"
#include "mssdig.h"
/******************************************************************************/
/** Recognized file types
 */
enum SampleFileTypes {
    SMP_FTYP_VOC = 0,
    SMP_FTYP_WAV = 1,
    SMP_FTYP_ASI = 2,
};

extern size_t sound_source_count;
extern SNDSAMPLE sound_samples[];

DIG_DRIVER *SS_construct_DIG_driver(AIL_DRIVER *drvr, const SNDCARD_IO_PARMS *iop)
{
    DIG_DRIVER *digdrv;
    int32_t n;

    digdrv = calloc(1, sizeof(*digdrv));
    assert(sizeof(SNDSAMPLE) == 0x894);

    digdrv->n_samples = sound_source_count;
    digdrv->buffer_flag = calloc(1, sizeof (int16_t));
    digdrv->build_buffer = calloc(digdrv->n_samples, 4);
    digdrv->build_size = 4 * digdrv->n_samples;
    digdrv->bytes_per_channel = 2;
    digdrv->channels_per_sample = 2;
    digdrv->channels_per_buffer = 2;
    digdrv->drvr = drvr;
    digdrv->hw_format = 3;
    digdrv->master_volume = 127;
    digdrv->playing  = 1;
    digdrv->DMA_rate = 44100;
    digdrv->half_buffer_size = 2048;
    digdrv->samples = sound_samples;

    for (n = 0; n < digdrv->n_samples; n++)
    {
        digdrv->samples[n].driver = digdrv;
        digdrv->samples[n].status = 1;
    }

    return digdrv;
}

void AIL2OAL_API_init_sample(SNDSAMPLE *s)
{
    asm volatile (
      "push %0\n"
      "call ASM_AIL_API_init_sample\n"
      "add $0x4, %%esp\n"
        :  : "g" (s) : "eax" );
}

SNDSAMPLE *AIL2OAL_API_allocate_sample_handle(DIG_DRIVER *dig)
{
    int32_t i;
    SNDSAMPLE *s;

    if (dig == NULL)
        return NULL;

    AIL_lock();

    // Look for an unallocated sample structure
    for (i = 0; i < dig->n_samples; i++)
    {
        if (dig->samples[i].status == SNDSMP_FREE)
            break;
    }

    if (i == dig->n_samples)
    {
        AIL_set_error("Out of sample handles.");
        AIL_unlock();
        return NULL;
    }

    s = &dig->samples[i];

    // Initialize sample to status SNDSMP_DONE with nominal
    // sample attributes
    AIL_init_sample(s);

    AIL_unlock();
    return s;
}

void AIL2OAL_API_end_sample(SNDSAMPLE *s)
{
    if (s == NULL)
        return;

    // Make sure sample has been allocated
    if (s->status == SNDSMP_FREE)
        return;

    // If sample is not already done, halt it and invoke its end-of-buffer
    // and end-of-sample callback functions
    if (s->status != SNDSMP_DONE)
    {
        s->status = SNDSMP_DONE;

        if (s->EOB != NULL)
            s->EOB(s);
        if (s->EOS != NULL)
            s->EOS(s);
    }
}

uint32_t AIL2OAL_API_sample_status(SNDSAMPLE *s)
{
    return s->status;
}

/** Process .VOC file block.
 *
 * Called by .VOC initialization code and as end-of-sample callback
 * function (interrupt-based).
 *
 * If play_flag clear, search for first block after desired marker (if
 * any) and return without playing it.
 */
void AIL_process_VOC_block(SNDSAMPLE *s, int32_t play_flag)
{
    asm volatile (
      "push %1\n"
      "push %0\n"
      "call ASM_AIL_process_VOC_block\n"
      "add $0x8, %%esp\n"
        :  : "g" (s), "g" (play_flag) : "eax" );
}

/** Create sample instance by parsing .WAV file.
 */
void AIL_process_WAV_image(const AILSOUNDINFO *info, SNDSAMPLE *s)
{
    asm volatile (
      "push %1\n"
      "push %0\n"
      "call ASM_AIL_process_WAV_image\n"
      "add $0x8, %%esp\n"
        :  : "g" (info), "g" (s) : "eax" );
}

int32_t AIL2OAL_API_set_sample_file(SNDSAMPLE *s, const void *file_image, int32_t block)
{
    int32_t ftype;

    if ((s == NULL) || (file_image == NULL))
        return 0;

    AIL_init_sample(s);

    ftype = -1;
    if (ftype == -1)
    {
        if (strncasecmp(file_image, "Creative", 8) == 0)
            ftype = SMP_FTYP_VOC;
    }
    if (ftype == -1)
    {
        if (strncasecmp(file_image + 8, "WAVE", 4) == 0)
            ftype = SMP_FTYP_WAV;
    }
    if (ftype == -1)
    {
        AIL_set_error("Unknown digital audio file type.");
        return 0;
    }

    // Copy file attributes to sample
    switch (ftype)
    {
    case SMP_FTYP_WAV:
        s->system_data[6] = 0;
        AIL_process_WAV_image(file_image, s);
        break;
    case SMP_FTYP_VOC:
        // TODO pointer to integer conversion, change to mind the bits!
        // Store pointer to sample data
        s->system_data[1] = (uint32_t)file_image + *(uint16_t *)(file_image + 20);
        s->system_data[4] = block;
        s->system_data[6] = 0;
        s->system_data[5] = (block == -1);
        AIL_process_VOC_block(s, 0);
        break;
    default: // s->system_data[6] == -1
        AIL_set_error("Invalid or missing data block.");
        return 0;
    }
    return 1;
}

void AIL2OAL_API_release_sample_handle(SNDSAMPLE *s)
{
    if (s == NULL)
        return;

    s->status = SNDSMP_FREE;

    // Shut down any installed pipeline providers
    AIL_set_sample_processor(s, SNDSMST_SAMPLE_ALL_STAGES, 0);
}

AILSAMPLECB AIL2OAL_API_register_EOS_callback(SNDSAMPLE *s, AILSAMPLECB EOS)
{
    AILSAMPLECB old;

    if (s == NULL)
        return NULL;

    old = s->EOS;
    s->EOS = EOS;

    return old;
}

HAILPROVIDER AIL2OAL_API_set_sample_processor(SNDSAMPLE *s,
    uint32_t pipeline_stage, HAILPROVIDER provider)
{
    // No need to implement - filters are rarely used
    return 0;
}

void AIL2OAL_API_set_sample_user_data(SNDSAMPLE *s, uint32_t index, int32_t value)
{
    if (s == NULL)
        return;
    s->user_data[index] = value;
}
/******************************************************************************/
