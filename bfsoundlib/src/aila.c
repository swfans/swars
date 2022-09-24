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

void AILA_startup(void)
{
#if 1
    asm volatile ("call ASM_AILA_startup\n"
        :  :  : "eax" );
#else
  // removed DOS-specific calls, place 1
  AILA_VMM_lock();
  AIL_entry_count = 0;
  AIL_lock_count = 0;
  AIL_PIT_period = -1;
  AIL_ISR_IRQ = -1;
  memset(timer_status, 0, sizeof(timer_status));
  memset(timer_callback_elapsed_times, 0, sizeof(timer_callback_elapsed_times));
  memset(timer_callback_periods, 0, sizeof(timer_callback_periods));
  memset(timer_trigger, 0, sizeof(timer_trigger));
  // removed DOS-specific calls, place 2
  AIL_set_timer_period((AIL_N_TIMERS-1) * 4, 54925);
  // removed DOS-specific calls, place 3
#endif
}

void AILA_shutdown(void)
{
    asm volatile ("call ASM_AILA_shutdown\n"
        :  :  : "eax" );
}

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
