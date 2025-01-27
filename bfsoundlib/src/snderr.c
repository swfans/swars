/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file snderr.c
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

#include "snderr.h"

#include "bftypes.h"
#include "bflog.h"
/******************************************************************************/
char SoundProgressMessage[256];

TbBool DebugAudio = false;

/******************************************************************************/

void InitDebugAudio(void)
{
    if (getenv("DEBUG_BF_AUDIO"))
        DebugAudio = 1;
}

void SoundProgressLog(const char *msg)
{
    if (!DebugAudio)
        return;

    if (LbErrorLogReady()) {
        LbSyncLog("Bfsnd: %s", msg);
    } else {
        fputs(msg, stdout);
    }
}


/******************************************************************************/
