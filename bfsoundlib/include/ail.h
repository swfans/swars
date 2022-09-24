/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ail.h
 *     Header file for ail.c.
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
#ifndef AIL2OAL_AIL_H_
#define AIL2OAL_AIL_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#define AIL_MAX_DRVRS 16
#define AIL_N_PREFS 19
#define AIL_N_TIMERS 16

int32_t AIL2OAL_API_startup(void);
void AIL2OAL_API_shutdown(void);

int32_t AIL2OAL_API_set_preference(uint32_t number, int32_t value);

AIL_DRIVER *AIL2OAL_API_install_driver(const uint8_t *driver_image, uint32_t n_bytes);
void AIL2OAL_API_uninstall_driver(AIL_DRIVER *drvr);

const SNDCARD_IO_PARMS *AIL2OAL_API_get_IO_environment(AIL_DRIVER *drvr);

/** Lock AIL timer service (to enable atomic operations).
 */
void AIL_lock(void);

/** Unlock AIL timer service.
 */
void AIL_unlock(void);

void *AIL2OAL_API_set_timer_user(HSNDTIMER timer, void *user_data);

void AIL2OAL_API_set_timer_period(HSNDTIMER timer, uint32_t usec);

void AIL_release_timer_handle(HSNDTIMER timer);

void AIL2OAL_API_release_all_timers(void);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AIL_H_
