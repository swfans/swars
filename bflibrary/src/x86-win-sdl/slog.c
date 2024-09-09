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
#include "bfconfig.h"
#include "bflog.h"

#include "bfmemut.h"
#include "bfstrut.h"
#include "bffile.h"
#include "bftime.h"
/******************************************************************************/

enum LogHeaderAction {
    NONE   = 0,
    CREATE = 1,
    APPEND = 2,
};

/******************************************************************************/

TbResult LbLogSetPrefix(struct TbLog *log, const char *prefix)
{
    if (!log->Initialised)
        return Lb_FAIL;
    if (prefix) {
        LbStringCopy(log->Prefix, prefix, LOG_PREFIX_LEN);
    } else {
        LbMemorySet(log->Prefix, 0, LOG_PREFIX_LEN);
    }
    return Lb_SUCCESS;
}

TbResult LbLogSetPrefixFmt(struct TbLog *log, const char *format, ...)
{
    va_list val;

    if (!log->Initialised)
        return Lb_FAIL;
    if (format) {
        va_start(val, format);
        vsprintf(log->Prefix, format, val);
        va_end(val);
    } else {
        LbMemorySet(log->Prefix, 0, LOG_PREFIX_LEN);
    }
    return Lb_SUCCESS;
}

TbResult LbLogSetup(struct TbLog *log, const char *filename, ulong flags)
{
    log->Initialised = false;
    LbMemorySet(log->Filename, 0, FILENAME_MAX);
    LbMemorySet(log->Prefix, 0, LOG_PREFIX_LEN);
    log->Initialised = false;
    log->Created = false;
    log->Suspended = false;
    if (LbStringLength(filename) > FILENAME_MAX)
        return Lb_FAIL;
    LbStringCopy(log->Filename, filename, FILENAME_MAX);
    log->Flags = flags;
    log->Initialised = true;
    log->Position = 0;
    return Lb_SUCCESS;
}

TbResult LbLogClose(struct TbLog *log)
{
    if (!log->Initialised)
        return Lb_FAIL;
    LbMemorySet(log->Filename, 0, FILENAME_MAX);
    LbMemorySet(log->Prefix, 0, LOG_PREFIX_LEN);
    log->Flags = 0;
    log->Initialised = false;
    log->Created = false;
    log->Suspended = false;
    log->Position = 0;
    return Lb_SUCCESS;
}

TbResult LbLogSuspend(struct TbLog *log)
{
    if (!log->Initialised)
        return Lb_FAIL;
    log->Suspended = true;
    return Lb_SUCCESS;
}

TbResult LbLogRestart(struct TbLog *log)
{
    if (!log->Initialised)
        return Lb_FAIL;
    log->Suspended = false;
    return Lb_SUCCESS;
}

TbResult LbLogDelete(struct TbLog *log)
{
    if (!log->Initialised)
        return Lb_FAIL;
    if (LbFileDelete(log->Filename) != Lb_SUCCESS)
        return Lb_FAIL;
    log->Created = false;
    return Lb_SUCCESS;
}

void LbI_LogHeader(struct TbLog *log, FILE *file, ubyte header)
{
    const char *actn;
    short at_used;
    TbBool need_initial_newline;

    need_initial_newline = (header == APPEND);
    if ( need_initial_newline )
        fprintf(file, "\n");
    if (header == CREATE)
    {
        const char *rel_kind;
#  ifdef __DEBUG
        rel_kind = "debug";
#  else
        rel_kind = "standard";
#  endif
        fprintf(file, LB_PACKAGE" ver "LB_VERSION" (%s release)\n", rel_kind);
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

TbResult LbLog(struct TbLog *log, const char *fmt_str, va_list arg)
{
    FILE *file;
    ubyte header;
    const char *accmode;

    if (!log->Initialised)
        return Lb_FAIL;
    if (log->Suspended)
        return Lb_SUCCESS;
    header = NONE;
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
        LbI_LogHeader(log, file, header);
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
        fprintf(file, "%s", log->Prefix);
    vfprintf(file, fmt_str, arg);
    log->Position = ftell(file);
    fclose(file);
    return Lb_SUCCESS;
}

/******************************************************************************/
