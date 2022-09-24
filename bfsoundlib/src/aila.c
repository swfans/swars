/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file aila.c
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

#include "aila.h"
#include "ail.h"
/******************************************************************************/
extern int32_t AIL_ISR_IRQ;

int32_t AIL2OAL_API_call_driver(AIL_DRIVER *drvr, int32_t fn,
        VDI_CALL *in, VDI_CALL *out)
{
    return 0;
}

void AIL2OAL_API_set_real_vect(uint32_t vectnum, void *real_ptr)
{
    // removed DOS-specific `int` call
}

void AIL2OAL_API_restore_USE16_ISR(int32_t irq)
{
    if ((irq != -1) && (irq == AIL_ISR_IRQ))
    {
        // removed DOS-specific `int` call
        AIL_ISR_IRQ = -1;
    }
}

/******************************************************************************/
