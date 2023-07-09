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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ail.h"
#include "aila.h"
#include "aildebug.h"
#include "memfile.h"
/******************************************************************************/

/** List of installed AIL drivers.
 */
extern AIL_DRIVER * AIL_driver[AIL_MAX_DRVRS];

/** AIL "preferences" array.
 */
extern int32_t AIL_preference[AIL_N_PREFS];

/** DIG_DRIVER list head.
 */
DIG_DRIVER *DIG_first = NULL;

/** MDI_DRIVER list head.
 */
MDI_DRIVER *MDI_first = NULL;

/** ASCII error type string.
 */
extern char AIL_error[256];

/** Last SNDCARD_IO_PARMS structure used to attempt device detection.
 */
extern SNDCARD_IO_PARMS AIL_last_IO_attempt;

extern uint32_t AIL_entry;
extern int32_t AIL_flags;
extern int32_t AIL_use_locked;
extern uint32_t AIL_bkgnd_flag;

extern int32_t timer_cb_periods[AIL_N_TIMERS];
extern int32_t timer_cb_elapsed_times[AIL_N_TIMERS];
extern void *timer_user[AIL_N_TIMERS];
extern int32_t timer_status[AIL_N_TIMERS];

void AIL2OAL_end(void);

void AIL2OAL_start(void)
{
    if (AIL_use_locked)
        return;

    AIL_VMM_lock_range(AIL2OAL_start, AIL2OAL_end);

    AIL_vmm_lock(AIL_driver, sizeof(AIL_driver));
    AIL_vmm_lock(AIL_preference, sizeof(AIL_preference));
    AIL_vmm_lock(AIL_error, sizeof(AIL_error));
    AIL_vmm_lock(&AIL_last_IO_attempt, sizeof(AIL_last_IO_attempt));
    AIL_vmm_lock(&AIL_entry, sizeof(AIL_entry));
    AIL_vmm_lock(&AIL_flags, sizeof(AIL_flags));

    AIL_use_locked = 1;
}

void AIL_lock(void)
{
   AIL2OAL_API_lock();
}

void AIL_unlock(void)
{
   AIL2OAL_API_unlock();
}

int32_t AIL2OAL_API_startup(void)
{
    int i;

    AIL2OAL_start();

    AIL_set_preference(0, 200);
    AIL_set_preference(1, 1);
    AIL_set_preference(2, 0x8000);
    AIL_set_preference(3, 100);
    AIL_set_preference(4, 16);
    AIL_set_preference(5, 100);
    AIL_set_preference(6, 655);
    AIL_set_preference(7, 0);
    AIL_set_preference(8, 0);
    AIL_set_preference(9, 1);
    AIL_set_preference(10, 0);
    AIL_set_preference(11, 120);
    AIL_set_preference(12, 8);
    AIL_set_preference(13, 127);
    AIL_set_preference(14, 1);
    AIL_set_preference(15, 0);
    AIL_set_preference(16, 2);
    AIL_set_preference(17, 1);
    AIL_set_preference(18, 1);

    for (i=0; i < AIL_MAX_DRVRS; i++) {
        AIL_driver[i] = NULL;
    }
    AIL_error[0] = '\0';

    AILA_startup();

   return 1;
}

void AIL2OAL_API_shutdown(void)
{
    int32_t i;

    // Shut down and unload all registered drivers
    for (i = AIL_MAX_DRVRS-1; i >= 0; i--) {
        AIL_DRIVER *drvr;

        if ((drvr = AIL_driver[i]) != NULL)
            AIL_uninstall_driver(drvr);
    }

   // Release all application timers
   AIL_release_all_timers();

   // Shut down assembly API
   AILA_shutdown();
}

void AIL2OAL_API_set_error(const char *error_msg)
{
    strcpy(AIL_error, error_msg);
}

const char *AIL_API_last_error(void)
{
   return AIL_error;
}

int32_t AIL2OAL_API_read_INI(AIL_INI *ini, char *fname)
{
    FILE *in;
    AIL_INI work;
    char buffer[80];
    uint32_t iobuf[67];  // three initial, 64 for buffering
    int32_t i;
    char *value;
    char *parm,*end_parm;

    // Init workspace
    memset(&work, 0, sizeof(work));
    memset(&work.IO, -1, sizeof(work.IO));

    // Open .INI file
    in = fopen(fname, "rt");

    if (in == 0)
        return 0;

    iobuf[0] = sizeof(iobuf);
    iobuf[1] = 0;

    while (fgets(buffer, sizeof(buffer), in) != NULL)
    {
        // Parse line
        for (i = strlen(buffer)-1; i >= 0; i--)
        {
            // Remove trailing whitespace
            if (buffer[i] == ' ') {
                buffer[i] = '\0';
            } else {
                break;
            }
        }

        // Find "parm" (1st word on line)
        for (i = 0; i < (int32_t)strlen(buffer); i++)
        {
            if (buffer[i] != ' ')
                break;
        }

        parm = (char *)&buffer[i];

        // Find "value" (2nd word on line)
        for (; i < (int32_t)strlen(buffer); i++)
        {
            if (buffer[i] == ' ')
                break;
        }

        end_parm = (char *)&buffer[i];

        for (; i < (int32_t)strlen(buffer); i++)
        {
            if (buffer[i] != ' ')
                break;
        }

        value = (char *)&buffer[i];

        // Reject unparsable lines
        if (i >= (int32_t)strlen(buffer))
            continue;

        *end_parm = 0;

        // Reject comments explicitly
        if (parm[0] == ';')
            continue;

        // Interpret line parameters
        if (!strncasecmp((char*)parm, "DRIVER", sizeof("DRIVER"))) {
            strcpy(work.driver_name, value);
        } else if (!strncasecmp((char*)parm, "DEVICE", sizeof("DEVICE"))) {
            strcpy(work.device_name, value);
        } else if (!strncasecmp((char*)parm, "IO_ADDR", sizeof("IO_ADDR"))) {
            work.IO.IO =         (int16_t) (strtoul(value, NULL, 16) & 0xffff);
        } else if (!strncasecmp((char*)parm, "IRQ", sizeof("IRQ"))) {
            work.IO.IRQ =        (int16_t) (strtoul(value, NULL, 10) & 0xffff);
        } else if (!strncasecmp((char*)parm, "DMA_8_bit", sizeof("DMA_8_bit"))) {
            work.IO.DMA_8_bit =  (int16_t) (strtoul(value, NULL, 10) & 0xffff);
        } else if (!strncasecmp((char*)parm, "DMA_16_bit", sizeof("DMA_16_bit"))) {
            work.IO.DMA_16_bit = (int16_t) (strtoul(value, NULL, 10) & 0xffff);
        }
    }

    fclose(in);

    // Fail call if neither driver nor device valid
    if ((!strlen(work.driver_name)) || (!strlen(work.device_name)))
    {
        AIL_set_error("Corrupted .INI file.");
        return 0;
    }

    *ini = work;
    return 1;
}

AIL_DRIVER *AIL2OAL_API_install_driver(const uint8_t *driver_image, uint32_t n_bytes)
{
    AIL_DRIVER *drvr;

    drvr = calloc(1, sizeof(*drvr));

    drvr->type = 0;

    return drvr;
}

void AIL2OAL_API_uninstall_driver(AIL_DRIVER *drvr)
{
    int i;

    // Call high-level destructor to free any allocated resources
    if (drvr->destructor != NULL) {
        drvr->destructor((DIG_DRIVER *)drvr->descriptor);
    }

    // Stop periodic timer service, if enabled
    if (drvr->server != -1) {
        AIL_release_timer_handle(drvr->server);
    }

    // If device successfully initialized, shut it down
    if (drvr->initialized)
    {
        if (drvr->PM_ISR != -1) {
            AIL_restore_USE16_ISR(drvr->PM_ISR);
        }
        AIL_call_driver(drvr, DRV_SHUTDOWN_DEV, NULL, NULL);
    }

    // Unlink driver from INT 66H chain
    AIL_set_real_vect(0x66, (void *)drvr->VHDR->prev_ISR);

    // Free low-memory buffer and AIL_DRIVER descriptor
    AIL_MEM_free_DOS(drvr->buf, drvr->seg, drvr->sel);
    AIL_MEM_free_lock(drvr, sizeof(AIL_DRIVER));

    for (i = 0; i < AIL_MAX_DRVRS; i++)
    {
        if (AIL_driver[i] == drvr) {
            AIL_driver[i] = NULL;
        }
    }
}

const SNDCARD_IO_PARMS *AIL2OAL_API_get_IO_environment(AIL_DRIVER *drvr)
{
    static SNDCARD_IO_PARMS iop = {0x220, 7, 1, 1, {0, 0, 0, 0}};
    return &iop;
}

int32_t AIL2OAL_API_set_preference(uint32_t number, int32_t value)
{
    int32_t oldval;

    oldval = AIL_preference[number];
    AIL_preference[number] = value;
    return oldval;
}

void AIL2OAL_API_delay(int32_t intervals)
{
    uint32_t last_tick;

    if (AIL_bkgnd_flag)
        return;

    // 16 = 62.5 intervals per second
    last_tick = AIL_ms_count() + (16*intervals);
    while (AIL_ms_count() < last_tick)
        ;
}

int32_t AIL2OAL_API_background(void)
{
    return AIL_bkgnd_flag;
}

void AIL2OAL_API_set_timer_period(HSNDTIMER timer, uint32_t usec)
{
    if (timer == -1)
        return;

    // Begin atomic operation
    AIL_lock();

    // Reset timer and set new period in microseconds
    timer_cb_periods[timer] = usec;
    timer_cb_elapsed_times[timer] = 0;
    AIL2OAL_program_timers();

    // End atomic operation
    AIL_unlock();
}

void *AIL2OAL_API_set_timer_user(HSNDTIMER timer, void *user_data)
{
  void *retval;

  assert (timer >= 0);
  assert (timer % sizeof (AILTIMERCB) == 0);
  assert (timer < AIL_N_TIMERS);

  retval = timer_user[timer];
  timer_user[timer] = user_data;

  return retval;
}

void AIL2OAL_API_release_all_timers(void)
{
    HSNDTIMER i;

    AIL_lock();
    for (i = 0; i < AIL_N_TIMERS; i++) {
        AIL_release_timer_handle(i);
    }
    AIL_unlock();
}

void AIL2OAL_API_start_timer(HSNDTIMER timer)
{
    AIL_lock();
    if (timer != -1)
    {
        if (timer_status[timer] == AILT_STOPPED)
            timer_status[timer] = AILT_RUNNING;
    }
    AIL_unlock();
}

void AIL2OAL_API_start_all_timers(void)
{
    HSNDTIMER i;

    AIL_lock();
    for (i = 0; i < AIL_N_TIMERS; i++) {
        AIL_start_timer(i);
    }
    AIL_unlock();
}

void AIL2OAL_API_stop_timer(HSNDTIMER timer)
{
    AIL_lock();
    if (timer != -1)
    {
        if (timer_status[timer] == AILT_RUNNING)
            timer_status[timer] = AILT_STOPPED;
    }
    AIL_unlock();
}

void AIL2OAL_end(void)
{
    if (!AIL_use_locked)
        return;

    AIL_VMM_unlock_range(AIL2OAL_start, AIL2OAL_end);

    AIL_vmm_unlock(AIL_driver, sizeof(AIL_driver));
    AIL_vmm_unlock(AIL_preference, sizeof(AIL_preference));
    AIL_vmm_unlock(AIL_error, sizeof(AIL_error));
    AIL_vmm_unlock(&AIL_last_IO_attempt, sizeof(AIL_last_IO_attempt));
    AIL_vmm_unlock(&AIL_entry, sizeof(AIL_entry));
    AIL_vmm_unlock(&AIL_flags, sizeof(AIL_flags));

    AIL_use_locked = 0;
}
/******************************************************************************/
