/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file miscutil.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     Miscelanous utility functions from MSS API.
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
#include <fcntl.h>
#include <assert.h>

#include "miscutil.h"
#include "mssal.h"
/******************************************************************************/

int32_t XMI_message_size(int32_t status)
{
    switch (status & 0xf0)
    {
    case MDI_EV_NOTE_OFF:
    case MDI_EV_NOTE_ON:
    case MDI_EV_POLY_PRESS:
    case MDI_EV_CONTROL:
    case MDI_EV_PITCH:
        return 3;

    case MDI_EV_PROGRAM:
    case MDI_EV_CHAN_PRESS:
        return 2;
    }

   return 0;
}

/******************************************************************************/
