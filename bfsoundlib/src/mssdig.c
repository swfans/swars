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
#include "ailss.h"
#include "aildebug.h"
/******************************************************************************/
extern bool sound_initialised;
extern DIG_DRIVER *sound_driver;

DIG_DRIVER *AIL2OAL_API_install_DIG_driver_file(const char *fname,
        const SNDCARD_IO_PARMS *iop)
{
    DIG_DRIVER *digdrv;
    AIL_DRIVER *drvr;

    if (!sound_initialised)
        return NULL;

    drvr = AIL_install_driver(NULL, 0);
    digdrv = SS_construct_DIG_driver(drvr, iop);

// TODO place is sub-function

#if 0
    digdrv->timer = timer_register_callback((TimerCallback)update_sound);
    AIL2OAL_API_set_timer_user(digdrv->timer, digdrv);

    digdrv->timer = timer_register_callback((TimerCallback)SS_serve);
    AIL2OAL_API_set_timer_user(digdrv->timer, digdrv);
#endif

    drvr->descriptor = digdrv;
    drvr->initialized = 1;
// TODO place is sub-function - end

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

void AIL2OAL_API_uninstall_DIG_driver(DIG_DRIVER *digdrv)
{
   AIL_uninstall_driver(digdrv->drvr);
}
/******************************************************************************/
