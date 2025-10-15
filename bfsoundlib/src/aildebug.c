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
#include "miscutil.h"
#include "ailss.h"
#include "mssdig.h"
#include "mssxdig.h"
#include "mssxmidi.h"
/******************************************************************************/
static long long tmcount_start = 0;

FILE *AIL_debugfile;
uint16_t AIL_debug;
uint16_t AIL_sys_debug;
uint32_t AIL_indent;

/******************************************************************************/

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
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug)) {
        if (prefix)
            fprintf(AIL_debugfile, "%s(\"%s\")\n", __func__, prefix);
        else
            fprintf(AIL_debugfile, "%s(NULL)\n", __func__);
    }

    AIL2OAL_API_set_GTL_filename_prefix(prefix);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_error(const char *error_msg)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug)) {
        if (error_msg)
            fprintf(AIL_debugfile, "%s(\"%s\")\n", __func__, error_msg);
        else
            fprintf(AIL_debugfile, "%s(NULL)\n", __func__);
    }

    AIL2OAL_API_set_error(error_msg);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void *AIL_file_read(const char *fname, void *dest)
{
    void *rdest;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug)) {
        if (fname)
            fprintf(AIL_debugfile, "%s(\"%s\", 0x%p)\n", __func__, fname, dest);
        else
            fprintf(AIL_debugfile, "%s(NULL, 0x%p)\n", __func__, dest);
    }

    rdest = AIL_API_file_read(fname, dest);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", rdest);
    AIL_indent--;

    return rdest;
}

void *AIL_get_real_vect(uint32_t vectnum)
{
    void *real_ptr;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%u)\n", __func__, vectnum);

    real_ptr = AIL2OAL_API_get_real_vect(vectnum);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", real_ptr);
    AIL_indent--;

    return real_ptr;
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

uint32_t AIL_disable_interrupts(void)
{
    uint32_t result;

    result = AIL2OAL_API_disable_interrupts();

    return result;
}

void AIL_restore_interrupts(uint32_t FD_reg)
{
    AIL2OAL_API_restore_interrupts(FD_reg);
}

uint32_t AIL_interrupt_divisor(void)
{
    uint32_t divisor;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s()\n", __func__);

    divisor = AIL2OAL_API_interrupt_divisor();

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %u\n", divisor);
    AIL_indent--;

    return divisor;
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

void AIL_release_timer_handle(HSNDTIMER timer)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%ld)\n", __func__, (long)timer);

    AIL2OAL_API_release_timer_handle(timer);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
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

int32_t AIL_sample_buffer_ready(SNDSAMPLE *s)
{
    int32_t bufstat;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    bufstat = AIL2OAL_API_sample_buffer_ready(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", bufstat);
    AIL_indent--;

    return bufstat;
}

SNDSAMPLE *AIL_allocate_sample_handle(DIG_DRIVER *digdrv)
{
    SNDSAMPLE *s;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, digdrv);

    s = AIL2OAL_API_allocate_sample_handle(digdrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", s);
    AIL_indent--;

    return s;
}

int32_t AIL_set_sample_file(SNDSAMPLE *s, const void *file_image, int32_t block)
{
   int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p, %d)\n", __func__, s, file_image, block);

    result = AIL2OAL_API_set_sample_file(s, file_image, block);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", result);
    AIL_indent--;

    return result;
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

intptr_t AIL_sample_user_data(SNDSAMPLE *s, uint32_t index)
{
   intptr_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u)\n", __func__, s, index);

    result = AIL2OAL_API_sample_user_data(s, index);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", (int)result);
    AIL_indent--;

    return result;
}

void AIL_set_sample_user_data(SNDSAMPLE *s, uint32_t index, intptr_t value)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u, %d)\n", __func__, s, index, value);

    AIL2OAL_API_set_sample_user_data(s, index, value);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

HAILPROVIDER AIL_set_sample_processor(SNDSAMPLE *s,
    uint32_t pipeline_stage, HAILPROVIDER provider)
{
   HAILPROVIDER result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u, %d)\n", __func__, s, pipeline_stage, provider);

    result = AIL2OAL_API_set_sample_processor(s, pipeline_stage, provider);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", (int)result);
    AIL_indent--;

    return result;
}

void AIL_set_sample_address(SNDSAMPLE *s, const void *start, uint32_t len)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p, %u)\n", __func__, s, start, len);

    AIL2OAL_API_set_sample_address(s, start, len);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

int32_t AIL_sample_playback_rate(SNDSAMPLE *s)
{
   int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    result = AIL2OAL_API_sample_playback_rate(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", (int)result);
    AIL_indent--;

    return result;
}

void AIL_set_sample_playback_rate(SNDSAMPLE *s, int32_t playback_rate)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u)\n", __func__, s, playback_rate);

    AIL2OAL_API_set_sample_playback_rate(s, playback_rate);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

int32_t AIL_sample_volume(SNDSAMPLE *s)
{
   int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    result = AIL2OAL_API_sample_volume(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", (int)result);
    AIL_indent--;

    return result;
}

void AIL_set_sample_volume(SNDSAMPLE *s, int32_t level)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u)\n", __func__, s, level);

    AIL2OAL_API_set_sample_volume(s, level);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

int32_t AIL_sample_pan(SNDSAMPLE *s)
{
   int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    result = AIL2OAL_API_sample_pan(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", (int)result);
    AIL_indent--;

    return result;
}

void AIL_set_sample_pan(SNDSAMPLE *s, int32_t level)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u)\n", __func__, s, level);

    AIL2OAL_API_set_sample_pan(s, level);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_sample_type(SNDSAMPLE *s, int32_t format, uint32_t flags)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u, 0x%x)\n", __func__, s, format, flags);

    AIL2OAL_API_set_sample_type(s, format, flags);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_sample_loop_count(SNDSAMPLE *s, int32_t loop_count)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d)\n", __func__, s, loop_count);

    AIL2OAL_API_set_sample_loop_count(s, loop_count);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_start_sample(SNDSAMPLE *s)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    AIL2OAL_API_start_sample(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_end_sample(SNDSAMPLE *s)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    AIL2OAL_API_end_sample(s);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_release_sample_handle(SNDSAMPLE *s)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, s);

    AIL2OAL_API_release_sample_handle(s);

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

void AIL_release_sequence_handle(SNDSEQUENCE *seq)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, seq);

    AIL2OAL_API_release_sequence_handle(seq);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

int32_t AIL_init_sequence(SNDSEQUENCE *seq, const void *start,  int32_t sequence_num)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p, %d)\n", __func__, seq, start,  sequence_num);

    result = AIL2OAL_API_init_sequence(seq, start,  sequence_num);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%X\n", result);
    AIL_indent--;

    return result;
}

void AIL_start_sequence(SNDSEQUENCE *seq)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, seq);

    AIL2OAL_API_start_sequence(seq);

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

void AIL_set_sequence_tempo(SNDSEQUENCE *seq, int32_t tempo, int32_t milliseconds)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d, %d)\n", __func__, seq, tempo, milliseconds);

    AIL2OAL_API_set_sequence_tempo(seq, tempo, milliseconds);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_sequence_volume(SNDSEQUENCE *seq, int32_t volume, int32_t milliseconds)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d, %d)\n", __func__, seq, volume, milliseconds);

    AIL2OAL_API_set_sequence_volume(seq, volume, milliseconds);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

uint32_t AIL_sequence_status(SNDSEQUENCE *seq)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, seq);

    result = AIL2OAL_API_sequence_status(seq);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%X\n", result);
    AIL_indent--;

    return result;
}

int32_t AIL_sequence_loop_count(SNDSEQUENCE *seq)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, seq);

    result = AIL2OAL_API_sequence_loop_count(seq);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%X\n", result);
    AIL_indent--;

    return result;
}

void AIL_sequence_position(SNDSEQUENCE *seq, int32_t *beat, int32_t *measure)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p, 0x%p)\n", __func__, seq, beat, measure);

    AIL2OAL_API_sequence_position(seq, beat, measure);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Results = 0x%X, 0x%X\n", *beat, *measure);
    AIL_indent--;
}

void AIL_branch_index(SNDSEQUENCE *seq, uint32_t marker)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %u)\n", __func__, seq, marker);

    AIL2OAL_API_branch_index(seq, marker);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_map_sequence_channel(SNDSEQUENCE *seq, int32_t seq_channel, int32_t new_channel)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d, %d)\n", __func__, seq, seq_channel, new_channel);

    AIL2OAL_API_map_sequence_channel(seq, seq_channel, new_channel);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

AILTRIGGERCB AIL_register_trigger_callback(SNDSEQUENCE *seq, AILTRIGGERCB callback)
{
    AILTRIGGERCB result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p)\n", __func__, seq, callback);

    result = AIL2OAL_API_register_trigger_callback(seq, callback);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", result);
    AIL_indent--;

    return result;
}

AILSEQUENCECB AIL_register_sequence_callback(SNDSEQUENCE *seq, AILSEQUENCECB callback)
{
    AILSEQUENCECB result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p)\n", __func__, seq, callback);

    result = AIL2OAL_API_register_sequence_callback(seq, callback);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", result);
    AIL_indent--;

    return result;
}

AILBEATCB AIL_register_beat_callback(SNDSEQUENCE *seq, AILBEATCB callback)
{
    AILBEATCB result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p)\n", __func__, seq, callback);

    result = AIL2OAL_API_register_beat_callback(seq, callback);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", result);
    AIL_indent--;

    return result;
}

AILEVENTCB AIL_register_event_callback(MDI_DRIVER *mdidrv, AILEVENTCB callback)
{
    AILEVENTCB result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p)\n", __func__, mdidrv, callback);

    result = AIL2OAL_API_register_event_callback(mdidrv, callback);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", result);
    AIL_indent--;

    return result;
}

AILTIMBRECB AIL_register_timbre_callback(MDI_DRIVER *mdidrv, AILTIMBRECB callback)
{
    AILTIMBRECB result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p)\n", __func__, mdidrv, callback);

    result = AIL2OAL_API_register_timbre_callback(mdidrv, callback);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", result);
    AIL_indent--;

    return result;
}

void AIL_set_sequence_user_data(SNDSEQUENCE *seq, uint32_t index, intptr_t value)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d, %d)\n", __func__, seq, index, value);

    AIL2OAL_API_set_sequence_user_data(seq, index, value);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

intptr_t AIL_sequence_user_data(SNDSEQUENCE *seq, uint32_t index)
{
    intptr_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d)\n", __func__, seq, index);

    result = AIL2OAL_API_sequence_user_data(seq, index);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%X\n", result);
    AIL_indent--;

    return result;
}

int32_t AIL_minimum_sample_buffer_size(DIG_DRIVER *digdrv,
  int32_t playback_rate, int32_t format)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d, %d)\n", __func__, digdrv, playback_rate, format);

    result = AIL2OAL_API_minimum_sample_buffer_size(digdrv, playback_rate, format);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", result);
    AIL_indent--;

    return result;
}

void AIL_load_sample_buffer(SNDSAMPLE *s, int32_t buff_num,
  void *buffer, uint32_t len)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d, 0x%p, %u)\n", __func__, s, buff_num, buffer, len);

    AIL2OAL_API_load_sample_buffer(s, buff_num, buffer, len);

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

void AIL_stop_timer(HSNDTIMER timer)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%ld)\n", __func__, timer);

    AIL2OAL_API_stop_timer(timer);

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

AILSAMPLECB AIL_register_EOS_callback(SNDSAMPLE *s, AILSAMPLECB EOS)
{
    AILSAMPLECB result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p)\n", __func__, s, EOS);

    result = AIL2OAL_API_register_EOS_callback(s, EOS);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", result);
    AIL_indent--;

    return result;
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
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug)) {
        if (fname)
            fprintf(AIL_debugfile, "%s(\"%s\", 0x%p)\n", __func__, fname, iop);
        else
            fprintf(AIL_debugfile, "%s(NULL, 0x%p)\n", __func__, iop);
    }

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

int32_t AIL_digital_master_volume(DIG_DRIVER *digdrv)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, digdrv);

    result = AIL2OAL_API_digital_master_volume(digdrv);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = %d\n", (int)result);
    AIL_indent--;

    return result;
}

void AIL_set_digital_master_volume(DIG_DRIVER *digdrv, int32_t master_volume)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d)\n", __func__, digdrv, master_volume);

    AIL2OAL_API_set_digital_master_volume(digdrv, master_volume);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

void AIL_set_XMIDI_master_volume(MDI_DRIVER *mdidrv, int32_t master_volume)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, %d)\n", __func__, mdidrv, master_volume);

    AIL2OAL_API_set_XMIDI_master_volume(mdidrv, master_volume);

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
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug)) {
        if (fname)
            fprintf(AIL_debugfile, "%s(0x%p, \"%s\")\n", __func__, ini, fname);
        else
            fprintf(AIL_debugfile, "%s(0x%p, NULL)\n", __func__, ini);
    }

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

int32_t AIL_lock_channel(MDI_DRIVER *mdidrv)
{
    int32_t result;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, mdidrv);

    result = AIL2OAL_API_lock_channel(mdidrv);

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

void AIL_send_channel_voice_message(MDI_DRIVER *mdidrv, SNDSEQUENCE *seq,
        int32_t status, int32_t data_1, int32_t data_2)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p, 0x%p, %d, %d, %d)\n",
           __func__, mdidrv, seq, status, data_1, data_2);

    AIL2OAL_API_send_channel_voice_message(mdidrv, seq, status, data_1, data_2);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
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
            mdidrv = AIL2OAL_API_install_MDI_driver_file("null.mdi", NULL);
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

MDI_DRIVER *AIL_install_MDI_driver_file(const char *fname, SNDCARD_IO_PARMS *iop)
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

WAVE_SYNTH *AIL_create_wave_synthesizer(DIG_DRIVER *digdrv,
  MDI_DRIVER *mdidrv, void const *wave_lib, int32_t polyphony)
{
    WAVE_SYNTH *ws;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p,0x%p,0x%p,%d)\n", __func__,
          digdrv, mdidrv, wave_lib, polyphony);

    ws = AIL2OAL_API_create_wave_synthesizer(
      digdrv, mdidrv, wave_lib, polyphony);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Result = 0x%p\n", ws);
    AIL_indent--;

    return ws;
}

void AIL_destroy_wave_synthesizer(WAVE_SYNTH *ws)
{
    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(0x%p)\n", __func__, ws);

    AIL2OAL_API_destroy_wave_synthesizer(ws);

    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "Finished\n");
    AIL_indent--;
}

/******************************************************************************/
