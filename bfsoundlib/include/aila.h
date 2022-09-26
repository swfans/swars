/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file aila.h
 *     Header file for aila.c.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
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
#ifndef AIL2OAL_AILA_H_
#define AIL2OAL_AILA_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

void AIL2OAL_API_lock(void);
void AIL2OAL_API_unlock(void);

void AILA_startup(void);
void AILA_shutdown(void);

void AILA_VMM_lock(void);

int32_t AIL2OAL_API_call_driver(AIL_DRIVER *drvr, int32_t fn,
        VDI_CALL *in, VDI_CALL *out);

void AIL2OAL_API_restore_USE16_ISR(int32_t irq);

HSNDTIMER AIL2OAL_API_register_timer(AILTIMERCB fn);

/** Set real-mode interrupt vector.
 */
void AIL2OAL_API_set_real_vect(uint32_t vectnum, void *real_ptr);

void AIL2OAL_API_set_timer_frequency(HSNDTIMER timer, uint32_t hertz);

/** Write to 8253 Programmable Interval Timer chip.
 */
void AIL2OAL_set_PIT_divisor(uint32_t divsr);

/** Set 8253 Programmable Interval Timer to desired period, in microseconds.
 */
void AIL2OAL_set_PIT_period(uint32_t period);

/** Establish timer interrupt rates based on fastest active timer.
 */
void AIL2OAL_program_timers(void);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AILA_H_
