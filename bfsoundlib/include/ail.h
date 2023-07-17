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

/** AIL "preferences" array.
 */
extern int32_t AIL_preference[AIL_N_PREFS];

/** Last SNDCARD_IO_PARMS structure used to attempt device detection.
 *
 * Set during detection of both DIG and MDI drivers.
 */
extern SNDCARD_IO_PARMS AIL_last_IO_attempt;


/** Initialize AIL API modules and resources.
 *
 * Must be called prior to any other AIL_...() calls.
 */
int32_t AIL2OAL_API_startup(void);

/** Shut down AIL API modules and resources, unloading all installed
 * drivers from memory.
 *
 * No further AIL_...() calls other than AIL_startup() are permissible.
 */
void AIL2OAL_API_shutdown(void);

/** Locks AIL library elements in memory, and marks the start of AIL library area.
 *
 * To be used internally. Not guaranteed to really mark the start,
 * as that depends on the compiler.
 */
void AIL2OAL_start(void);

/** Unlocks AIL library elements in memory, and marks the end of AIL library area.
 *
 * To be used internally. Not guaranteed to really mark the end,
 * as that depends on the compiler.
 */
void AIL2OAL_end(void);

void AIL2OAL_API_set_error(const char *error_msg);

const char *AIL_API_last_error(void);

int32_t AIL2OAL_API_read_INI(AIL_INI *ini, char *fname);

void AIL2OAL_API_delay(int32_t intervals);

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

/** Timer interrupt routine to provide periodic service to driver,
 * if requested.
 */
void AIL_driver_server(void *clientval);

void *AIL2OAL_API_set_timer_user(HSNDTIMER timer, void *user_data);

void AIL2OAL_API_set_timer_period(HSNDTIMER timer, uint32_t usec);

int32_t AIL2OAL_API_background(void);

void AIL_release_timer_handle(HSNDTIMER timer);

void AIL2OAL_API_release_all_timers(void);

void AIL2OAL_API_start_timer(HSNDTIMER timer);

void AIL2OAL_API_start_all_timers(void);

void AIL2OAL_API_stop_timer(HSNDTIMER timer);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AIL_H_
