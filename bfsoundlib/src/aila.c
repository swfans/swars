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
#include "aildebug.h"
#include "memfile.h"
/******************************************************************************/
/** Callback function addrs for timers.
*/
extern AILTIMERCB timer_callback[AIL_N_TIMERS];

/** States of timers (0=free 1=off 2=on)
 */
extern int32_t timer_status[AIL_N_TIMERS];

/** Modified DDA error counts for timers
 */
extern int32_t timer_cb_elapsed_times[AIL_N_TIMERS];

/** Modified DDA limit values for timers
 */
extern int32_t timer_cb_periods[AIL_N_TIMERS];

/** Amount of triggers pending (number of times
 * to call the callback).
 */
extern int32_t timer_trigger[AIL_N_TIMERS];

/** User parameters for timer callbacks.
 */
extern int32_t timer_user[AIL_N_TIMERS];

/** Last divisor value written to PIT.
 */
extern uint32_t AIL_PIT_divisor;

/** PIT timer interval in uS.
 */
extern uint32_t AIL_PIT_period;

extern uint32_t AIL_bkgnd_flag;

extern uint32_t AIL_lock_count;

extern int32_t AIL_ISR_IRQ;

extern int32_t old_sp;

void AIL2OAL_API_lock(void)
{
  ++AIL_lock_count;
}

void AIL2OAL_API_unlock(void)
{
  --AIL_lock_count;
}

void AILA_startup(void)
{
    // removed DOS-specific calls, place 1
    AILA_VMM_lock();
    AIL_bkgnd_flag = 0;
    AIL_lock_count = 0;
    AIL_PIT_period = -1;
    AIL_ISR_IRQ = -1;
    memset(timer_status, 0, sizeof(timer_status));
    memset(timer_cb_elapsed_times, 0, sizeof(timer_cb_elapsed_times));
    memset(timer_cb_periods, 0, sizeof(timer_cb_periods));
    memset(timer_trigger, 0, sizeof(timer_trigger));
    // removed DOS-specific calls, place 2
    AIL_set_timer_period((AIL_N_TIMERS-1) * 4, 54925);
    // removed DOS-specific calls, place 3
}

void AILA_shutdown(void)
{
    // removed DOS-specific calls, place 1
    AIL2OAL_set_PIT_divisor(0);
    // removed DOS-specific calls, place 2
}

int32_t AIL2OAL_API_call_driver(AIL_DRIVER *drvr, int32_t fn,
        VDI_CALL *in, VDI_CALL *out)
{
    return 0;
}

void *AIL2OAL_API_get_real_vect(uint32_t vectnum)
{
    // removed DOS-specific `int` call
    return NULL;
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

void AIL2OAL_API_set_timer_frequency(HSNDTIMER timer, uint32_t hertz)
{
    if (timer == -1)
        return;
    AIL_set_timer_period(timer, 1000000 / hertz);
}

void AIL2OAL_set_PIT_divisor(uint32_t divsr)
{
    // removed DOS-specific calls, place 1
    AIL_PIT_divisor = divsr;
    // removed DOS-specific calls, place 2
}

uint32_t AIL2OAL_API_interrupt_divisor(void)
{
    return AIL_PIT_divisor;
}

void AIL2OAL_set_PIT_period(uint32_t period)
{
    uint32_t divsr;

    divsr = 0;
    if (period < 54925)
        divsr = 10000 * period / 8380;
    AIL2OAL_set_PIT_divisor(divsr);
}

void AIL2OAL_program_timers(void)
{
    uint32_t peri, min_peri;
    HSNDTIMER i;

    AIL_lock();

    min_peri = 0xFFFFFFFF;
    for (i = 0; i < AIL_N_TIMERS; i++)
    {
        if (timer_status[i] != AILT_FREE)
        {
            peri = timer_cb_periods[i];
            if (peri < min_peri)
                min_peri = peri;
        }
    }

    if (min_peri != AIL_PIT_period)
    {
        AIL_PIT_period = min_peri;
        AIL2OAL_set_PIT_period(min_peri);
        memset(timer_cb_elapsed_times, 0, sizeof(timer_cb_elapsed_times));
    }
    AIL_unlock();
}

HSNDTIMER AIL2OAL_API_register_timer(AILTIMERCB fn)
{
    size_t n;

    AIL_lock();
    for (n = 0; n < AIL_N_TIMERS; n++)
    {
        if (timer_status[n] != AILT_FREE)
            continue;

        timer_status[n] = AILT_STOPPED;
        timer_callback[n] = fn;

        AIL_unlock();
        return n;
    }

    AIL_unlock();
    return -1;
}

void AIL2OAL_API_release_timer_handle(HSNDTIMER timer)
{
    if (timer == -1)
        return;

    timer_status[timer] = AILT_FREE;
}

void AILA_VMM_lock(void)
{
    AIL_VMM_lock_range(timer_callback, ((uint8_t *)&old_sp) + sizeof(old_sp));
    AIL_VMM_lock_range(AIL2OAL_API_lock, AILA_VMM_lock);
}

/******************************************************************************/
