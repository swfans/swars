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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "aildebug.h"
#include "ail.h"
#include "aila.h"
#include "ailss.h"
#include "mssxmidi.h"
/******************************************************************************/
extern uint16_t AIL_debug;
extern uint16_t AIL_sys_debug;
extern FILE *AIL_debugfile;
extern uint32_t AIL_indent;

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

MDI_DRIVER *AIL_install_MDI_driver_file(char *filename, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;

    AIL_indent++;
    if (AIL_debug && (AIL_indent == 1 || AIL_sys_debug))
        fprintf(AIL_debugfile, "%s(%s,0x%p)\n", __func__, filename, iop);

    mdidrv = AIL2OAL_API_install_MDI_driver_file(filename, iop);

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
