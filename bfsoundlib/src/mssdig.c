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
#include "ailss.h"
#include "aildebug.h"
/******************************************************************************/
extern bool sound_initialised;

DIG_DRIVER *AIL2OAL_API_install_DIG_driver_file(const char *fname,
        const SNDCARD_IO_PARMS *iop)
{
    DIG_DRIVER *digdrv;
    AIL_DRIVER *drvr;
    int32_t *driver_image;

    if (!sound_initialised)  // SWPort hack, to be removed
        return NULL;

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
    driver_image = (int32_t*) AIL_MEM_alloc_lock(20);
    if (driver_image != NULL) {
        driver_image[0] = 16;
        driver_image[1] = 0;
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
