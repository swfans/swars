/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssamplst.c
 *     Sound Sample List support routines.
 * @par Purpose:
 *     Functions for handling a list/queue of sound samples.
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
#include <assert.h>

#include "ssamplst.h"
#include "aildebug.h"
#include "mssal.h"
/******************************************************************************/

extern TbBool sample_queue_handle_initiated;
extern TbBool sample_queue_handle_stopped;
extern SNDSAMPLE *sample_queue_handle;

/******************************************************************************/

void StopSampleQueueList(void)
{
    int i;

    if (!sample_queue_handle_initiated)
        return;

    sample_queue_handle_stopped = 1;
    AIL_register_EOS_callback(sample_queue_handle, 0);
    AIL_end_sample(sample_queue_handle);

    for (i = 0; i < 8; i++)
        AIL_set_sample_user_data(sample_queue_handle, i, 0);
}

/******************************************************************************/
