/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ail.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     Lib lifecycle functions from MSS API.
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
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "ail.h"
/******************************************************************************/
AIL_DRIVER *AIL2OAL_API_install_driver(const uint8_t *driver_image, uint32_t n_bytes)
{
    AIL_DRIVER *drvr;

    drvr = calloc(1, sizeof(*drvr));

    drvr->type = 0;

    return drvr;
}

/******************************************************************************/
