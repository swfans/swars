/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file aildebug.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     Wrappers for MSS API functions, providing debug log capabilities.
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>

#include "aildebug.h"
#include "ail.h"
#include "aila.h"
#include "ailss.h"
#include "mssdig.h"
#include "mssxmidi.h"
/******************************************************************************/
extern uint16_t AIL_debug;
extern uint16_t AIL_sys_debug;
extern FILE *AIL_debugfile;
extern uint32_t AIL_indent;

static long long tmcount_start = 0;

int32_t AIL_startup(void)
{
    const char *logfname;
    int32_t ret;

    // Get profile string for debug script, and enable debug mode if script filename valid

    AIL_debug = 0;
    AIL_sys_debug = 0;

    logfname = getenv("AIL_DEBUG");

    if (logfname != NULL) {
        if (getenv("AIL_SYS_DEBUG"))
            AIL_sys_debug = 1;
        AIL_debugfile = fopen(logfname, "w+t");
    }

    if (AIL_debugfile != NULL) {
       char loctime[25];
       time_t elapstime;
       struct tm *adjtime;

       time(&elapstime);
       adjtime = localtime(&elapstime);
       strcpy(loctime,asctime(adjtime));
       loctime[sizeof(loctime)-1] = 0;

       fprintf(AIL_debugfile,
           "-------------------------------------------------------------------------------\n"
           "Miles Sound System API reimplementation call log\n"
           "Start time: %s\n"
           "-------------------------------------------------------------------------------\n"
           "\n", loctime);

        AIL_debug = 1;
    }
    AIL_indent = 1;

    if (AIL_debug && AIL_sys_debug)
        fprintf(AIL_debugfile, "%s()\n", __func__);

    ret = AIL2OAL_API_startup();

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", ret);
    AIL_indent--;

    return ret;
}

void AIL_shutdown(void)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s()\n", __func__);

    AIL2OAL_API_shutdown();

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug)) {
        fprintf(AIL_debugfile, "Finished\n");
        fclose(AIL_debugfile);
    }
    AIL_indent--;
}

uint32_t AIL_ms_count(void)
{
    long long now;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    now = tv.tv_sec;
    now *= 1000000;
    now += tv.tv_usec;
    if (!tmcount_start) {
        tmcount_start = now;
    }
    return ((now - tmcount_start) / 1000);
}

int32_t AIL_set_preference(uint32_t number, int32_t value)
{
   int32_t ret;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%u,%u)\n", __func__, number, value);

    ret = AIL2OAL_API_set_preference(number, value);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", ret);
    AIL_indent--;

   return ret;
}

void AIL_set_GTL_filename_prefix(char const *prefix)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(\"%s\")\n", __func__, prefix);

    AIL2OAL_API_set_GTL_filename_prefix(prefix);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_error(const char *error_msg)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(\"%s\")\n", __func__, error_msg);

    AIL2OAL_API_set_error(error_msg);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_real_vect(uint32_t vectnum, void *real_ptr)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%u, 0x%p)\n", __func__, vectnum, real_ptr);

    AIL2OAL_API_set_real_vect(vectnum, real_ptr);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

HSNDTIMER AIL_register_timer(AILTIMERCB fn)
{
    HSNDTIMER timer;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, fn);

    timer = AIL2OAL_API_register_timer(fn);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %ld\n", timer);
    AIL_indent--;

    return timer;
}

void *AIL_set_timer_user(HSNDTIMER timer, void *user_data)
{
    void *prev_data;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%ld, 0x%p)\n", __func__, timer, user_data);

    prev_data = AIL2OAL_API_set_timer_user(timer, user_data);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", prev_data);
    AIL_indent--;

    return prev_data;
}

AIL_DRIVER *AIL_install_driver(const uint8_t *driver_image, uint32_t n_bytes)
{
    AIL_DRIVER *drvr;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p,%u)\n", __func__, driver_image, n_bytes);

    drvr = AIL2OAL_API_install_driver(driver_image, n_bytes);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", drvr);
    AIL_indent--;

    return drvr;
}

void AIL_uninstall_driver(AIL_DRIVER *drvr)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, drvr);

    AIL2OAL_API_uninstall_driver(drvr);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

const SNDCARD_IO_PARMS *AIL_get_IO_environment(AIL_DRIVER *drvr)
{
    const SNDCARD_IO_PARMS *iop;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, drvr);

    iop = AIL2OAL_API_get_IO_environment(drvr);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", iop);
    AIL_indent--;

    return iop;
}

void AIL_restore_USE16_ISR(int32_t irq)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%d)\n", __func__, irq);

    AIL2OAL_API_restore_USE16_ISR(irq);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

uint32_t AIL_sample_status(SNDSAMPLE *s)
{
    uint32_t status;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    status = AIL2OAL_API_sample_status(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%X\n", status);
    AIL_indent--;

    return status;
}

SNDSAMPLE *AIL_allocate_sample_handle(DIG_DRIVER *dig)
{
    SNDSAMPLE *s;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, dig);

    s = AIL2OAL_API_allocate_sample_handle(dig);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", s);
    AIL_indent--;

    return s;
}

void AIL_init_sample(SNDSAMPLE *s)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    AIL2OAL_API_init_sample(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_stop_sequence(SNDSEQUENCE *seq)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, seq);

    AIL2OAL_API_stop_sequence(seq);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_resume_sequence(SNDSEQUENCE *seq)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, seq);

    AIL2OAL_API_resume_sequence(seq);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_end_sequence(SNDSEQUENCE *seq)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, seq);

    AIL2OAL_API_end_sequence(seq);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_delay(int32_t intervals)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%d)\n", __func__, intervals);

    AIL2OAL_API_delay(intervals);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

int32_t AIL_background(void)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s()\n", __func__);

    result = AIL2OAL_API_background();

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", result);
    AIL_indent--;

    return result;
}

void AIL_set_timer_frequency(HSNDTIMER timer, uint32_t hertz)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%ld, %u)\n", __func__, timer, hertz);

    AIL2OAL_API_set_timer_frequency(timer, hertz);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_timer_period(HSNDTIMER timer, uint32_t usec)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%ld, %u)\n", __func__, timer, usec);

    AIL2OAL_API_set_timer_period(timer, usec);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_start_timer(HSNDTIMER timer)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%ld)\n", __func__, timer);

    AIL2OAL_API_start_timer(timer);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_start_all_timers(void)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s()\n", __func__);

    AIL2OAL_API_start_all_timers();

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_release_all_timers(void)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s()\n", __func__);

    AIL2OAL_API_release_all_timers();

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

int32_t AIL_call_driver(AIL_DRIVER *drvr, int32_t fn,
        VDI_CALL *in, VDI_CALL *out)
{
    int32_t ret;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p,%d,0x%p,0x%p)\n", __func__, drvr, fn, in, out);

    ret = AIL2OAL_API_call_driver(drvr, fn, in, out);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", ret);
    AIL_indent--;

    return ret;
}

DIG_DRIVER *AIL_install_DIG_driver_file(char const *fname, SNDCARD_IO_PARMS *iop)
{
    DIG_DRIVER *digdrv;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(\"%s\", 0x%p)\n", __func__, fname, iop);

    digdrv = AIL2OAL_API_install_DIG_driver_file(fname, iop);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", digdrv);
    AIL_indent--;

    return digdrv;
}

int32_t AIL_install_DIG_INI(DIG_DRIVER **digdrv)
{
    int32_t ret;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, digdrv);

    ret = AIL2OAL_API_install_DIG_INI(digdrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", ret);
    AIL_indent--;

    return ret;
}

void AIL_uninstall_DIG_driver(DIG_DRIVER *digdrv)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, digdrv);

    AIL2OAL_API_uninstall_DIG_driver(digdrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

int32_t AIL_MDI_driver_type(MDI_DRIVER *mdidrv)
{
    int32_t dtype;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, mdidrv);

    dtype = AIL2OAL_API_MDI_driver_type(mdidrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", dtype);
    AIL_indent--;

    return dtype;
}

int32_t AIL_install_MDI_INI(MDI_DRIVER **mdidrv)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, mdidrv);

    result = AIL2OAL_API_install_MDI_INI(mdidrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", result);
    AIL_indent--;

    return result;
}

int32_t AIL_read_INI(AIL_INI *ini, char *fname)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, \"%s\")\n", __func__, ini, fname);

    result = AIL2OAL_API_read_INI(ini, fname);

    if (result && AIL_debug && (AIL_indent == 1 || AIL_sys_debug)) {
         fprintf(AIL_debugfile, "Driver = %s\n", ini->driver_name);
         fprintf(AIL_debugfile, "Device = %s\n", ini->device_name);
         fprintf(AIL_debugfile, "IO     = %X\n", ini->IO.IO);
         fprintf(AIL_debugfile, "IRQ    = %d\n", ini->IO.IRQ);
         fprintf(AIL_debugfile, "DMA_8  = %d\n", ini->IO.DMA_8_bit);
         fprintf(AIL_debugfile, "DMA_16 = %d\n", ini->IO.DMA_16_bit);
    }

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", result);
    AIL_indent--;

    return result;
}

void AIL_release_channel(MDI_DRIVER *mdidrv, int32_t channel)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d)\n", __func__, mdidrv, channel);

    AIL2OAL_API_release_channel(mdidrv, channel);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

SNDSEQUENCE *AIL_allocate_sequence_handle(MDI_DRIVER *mdidrv)
{
    SNDSEQUENCE *result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, mdidrv);

    result = AIL2OAL_API_allocate_sequence_handle(mdidrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", result);
    AIL_indent--;

    return result;
}

MDI_DRIVER *AIL_open_XMIDI_driver(uint32_t flags)
{
    MDI_DRIVER *mdidrv;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%X)\n", __func__, flags);

    mdidrv = NULL;
#if defined(WIN32) && 0 // disabled as we do not use win32-specific MIDI API
    if (mdidrv == NULL)
    {
        uint32_t opt;
        opt = (flags & AIL_OPEN_XMIDI_NULL_DRIVER) ? MIDI_NULL_DRIVER : MIDI_MAPPER;
        if (AIL2OAL_API_midiOutOpen(&mdidrv, 0, opt))
            mdidrv = NULL;
    }
#elif defined(DOS)||defined(GO32)
    if (mdidrv == NULL)
    {
        if (flags & AIL_OPEN_XMIDI_NULL_DRIVER)
            mdidrv = AIL2OAL_API_install_MDI_driver_file("NULL.MDI",0);
    }
    if (mdidrv == NULL)
    {
        if (AIL2OAL_API_install_MDI_INI(&mdidrv) != AIL_INIT_SUCCESS)
            mdidrv = NULL;
    }
#else
    if (mdidrv == NULL)
    {
        mdidrv = AIL2OAL_API_open_XMIDI_driver(flags);
    }
#endif

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", mdidrv);
    AIL_indent--;

    return mdidrv;
}

void AIL_close_XMIDI_driver(MDI_DRIVER *mdidrv)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, mdidrv);

#if defined(WIN32) && 0 // disabled as we do not use win32-specific MIDI API
    AIL2OAL_API_midiOutClose(mdidrv);
#elif defined(DOS)||defined(GO32)
    AIL2OAL_API_uninstall_MDI_driver(mdidrv)
#else
    AIL2OAL_API_close_XMIDI_driver(mdidrv);
#endif

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

MDI_DRIVER *AIL_install_MDI_driver_file(char *fname, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%s,0x%p)\n", __func__, fname, iop);

    mdidrv = AIL2OAL_API_install_MDI_driver_file(fname, iop);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", mdidrv);
    AIL_indent--;

    return mdidrv;
}

void AIL_uninstall_MDI_driver(MDI_DRIVER *mdidrv)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, mdidrv);

    AIL2OAL_API_uninstall_MDI_driver(mdidrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

/******************************************************************************/
