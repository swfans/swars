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
#include "mssdig.h"
/******************************************************************************/
extern size_t sound_source_count;
extern SNDSAMPLE sound_samples[];
extern bool sound_initialised;
extern DIG_DRIVER *sound_driver;

static DIG_DRIVER *SS_construct_DIG_driver(AIL_DRIVER *drvr, const SNDCARD_IO_PARMS *iop)
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

DIG_DRIVER *AIL2OAL_API_install_DIG_driver_file(const char *fname,
        const SNDCARD_IO_PARMS *iop)
{
    DIG_DRIVER *digdrv;
    AIL_DRIVER *drvr;

    if (!sound_initialised)
        return NULL;

    drvr = AIL_install_driver(NULL, 0);
    digdrv = SS_construct_DIG_driver(drvr, iop);

#if 0
    digdrv->timer = timer_register_callback ((TimerCallback) update_sound);
    timer_set_user_data (digdrv->timer, digdrv);
	
    digdrv->timer = timer_register_callback ((TimerCallback) &SS_serve);
    timer_set_user_data (digdrv->timer, digdrv);
#endif

    drvr->descriptor = digdrv;
    drvr->initialized = 1;

    return digdrv;
}

int32_t AIL2OAL_API_install_DIG_INI(DIG_DRIVER **digdrv)
{
  if (sound_driver != NULL)
    return -1;

  sound_driver = AIL2OAL_API_install_DIG_driver_file(NULL, NULL);
  *digdrv = sound_driver;

  if (sound_driver == NULL)
    return -1;

  return 0;
}

/******************************************************************************/
