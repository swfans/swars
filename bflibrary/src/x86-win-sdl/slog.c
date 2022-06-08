/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file glog.cpp
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdarg.h>
#include "bflog.h"

#include "bfmemut.h"
#include "bfstrut.h"
#include "bftime.h"
/******************************************************************************/
extern TbBool error_log_initialised;
extern struct TbLog error_log;
/******************************************************************************/

TbResult LbErrorLogSetup(const char *directory, const char *filename, ubyte flag)
{
//TODO enable when logging is ready
#if 0
    const char *fixed_fname;
    char log_filename[FILENAME_MAX];
    ulong flags;

    if (error_log_initialised)
        return Lb_FAIL;
    if ((filename != NULL) && (filename[0] != '\0'))
        fixed_fname = filename;
    else
        fixed_fname = "error.log";
    if (LbFileMakeFullPath(true, directory, fixed_fname, log_filename, FILENAME_MAX)
      != Lb_SUCCESS) {
        return Lb_FAIL;
    }
    flags = (flag==0)+1;
    flags |= LbLog_TimeInHeader | LbLog_DateInHeader | 0x04;
    if (LbLogSetup(&error_log, log_filename, flags) == Lb_SUCCESS) {
        return Lb_FAIL;
    }
#endif
    error_log_initialised = 1;
    return Lb_SUCCESS;
}

TbResult LbErrorLogReset(void)
{
    if (!error_log_initialised)
        return Lb_FAIL;
    return LbLogClose(&error_log);
}

TbResult LbLog(struct TbLog *log, const char *fmt_str, va_list arg)
{
    enum Header {
        NONE   = 0,
        CREATE = 1,
        APPEND = 2,
    };
//TODO enable when logging is ready
#if 0
    FILE *file;
    TbBool need_initial_newline;
    ubyte header;
    const char *accmode;

    if (!log->Initialised)
        return Lb_FAIL;
    if (log->Suspended)
        return Lb_SUCCESS;
    header = NONE;
    need_initial_newline = false;
    if (!log->Created)
    {
        if (((log->Flags & LbLog_Create) != 0) && !LbFileExists(log->Filename))
        {
            header = CREATE;
        }
        else if (((log->Flags & LbLog_Overwrite) != 0) && ((log->Flags & LbLog_Create) != 0))
        {
            header = CREATE;
        }
        else if (((log->Flags & LbLog_Append) != 0) && ((log->Flags & LbLog_ContinueLast) != 0))
        {
            need_initial_newline = true;
            header = APPEND;
        }
    }
    if ((log->Created) || ((log->Flags & LbLog_Overwrite) == 0))
        accmode = "a";
    else
        accmode = "w";
    file = fopen(log->Filename, accmode);
    if (file == NULL) {
        return Lb_FAIL;
    }
    log->Created = true;
    if (header != NONE)
    {
        const char *actn;
        short at_used;

        if ( need_initial_newline )
            fprintf(file, "\n");
        if (header == CREATE)
        {
            fprintf(file, PACKAGE" ver "VERSION" (%s release)\n",
              (__DEBUG) ? "debug" : "standard");
            actn = "CREATED";
        } else
        {
            actn = "APPENDED";
        }
        fprintf(file, "LOG %s", actn);
        at_used = 0;
        if ((log->Flags & LbLog_TimeInHeader) != 0)
        {
            struct TbTime curr_time;
            LbTime(&curr_time);
            fprintf(file, "  @ %02d:%02d:%02d",
                curr_time.Hour,curr_time.Minute,curr_time.Second);
            at_used = 1;
        }
        if ((log->Flags & LbLog_DateInHeader) != 0)
        {
            struct TbDate curr_date;
            LbDate(&curr_date);
            const char *sep;
            if ( at_used )
                sep = " ";
            else
                sep = "  @ ";
            fprintf(file," %s%02d-%02d-%d", sep, curr_date.Day, curr_date.Month, curr_date.Year);
        }
        fprintf(file, "\n\n");
    }
    if ((log->Flags & LbLog_DateInLines) != 0)
    {
        struct TbDate curr_date;
        LbDate(&curr_date);
        fprintf(file,"%02d-%02d-%d ", curr_date.Day, curr_date.Month, curr_date.Year);
    }
    if ((log->Flags & LbLog_TimeInLines) != 0)
    {
        struct TbTime curr_time;
        LbTime(&curr_time);
        fprintf(file, "%02d:%02d:%02d ",
            curr_time.Hour, curr_time.Minute, curr_time.Second);
    }
    if (log->Prefix[0] != '\0')
        fprintf(file, log->Prefix);
    vfprintf(file, fmt_str, arg);
    log->Position = ftell(file);
    fclose(file);
#endif
    return Lb_SUCCESS;
}

int LbLogDelete_UNUSED()
{
// code at 0001:000d2098
}

/******************************************************************************/
