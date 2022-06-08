/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gerrorlg.cpp
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
#include <string.h>
#include <stdarg.h>
#include "bflog.h"

#include "bffile.h"
/******************************************************************************/
TbBool error_log_initialised = false;
struct TbLog error_log;

int LbErrorLog(const char *format, ...)
{
    va_list val;
    int result;

    if (!error_log_initialised)
        return Lb_FAIL;
    LbLogSetPrefix(&error_log, "Error: ");
    va_start(val, format);
    result = LbLog(&error_log, format, val);
    va_end(val);
    return result;
}

int LbWarnLog(const char *format, ...)
{
    va_list val;
    int result;

    if (!error_log_initialised)
        return Lb_FAIL;
    LbLogSetPrefix(&error_log, "Warning: ");
    va_start(val, format);
    result = LbLog(&error_log, format, val);
    va_end(val);
    return result;
}

int LbSyncLog(const char *format, ...)
{
    va_list val;
    int result;

    if (!error_log_initialised)
        return Lb_FAIL;
    LbLogSetPrefix(&error_log, "Sync: ");
    va_start(val, format);
    result = LbLog(&error_log, format, val);
    va_end(val);
    return result;
}

int LbDbgLog(const char *format, ...)
{
    va_list val;
    int result;

    if (!error_log_initialised)
        return Lb_FAIL;
    LbLogSetPrefix(&error_log, "Debug: ");
    va_start(val, format);
    result = LbLog(&error_log, format, val);
    va_end(val);
    return result;
}

TbResult LbErrorLogSetup(const char *directory, const char *filename, ubyte flag)
{
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
    flags = (flag == Lb_ERROR_LOG_NEW) ? LbLog_Overwrite : LbLog_Append;
    flags |= LbLog_TimeInHeader | LbLog_DateInHeader | LbLog_Create;
    if (LbLogSetup(&error_log, log_filename, flags) != Lb_SUCCESS) {
        return Lb_FAIL;
    }
    error_log_initialised = true;
    return Lb_SUCCESS;
}

TbResult LbErrorLogReset(void)
{
    TbResult result;

    if (!error_log_initialised)
        return Lb_FAIL;
    result = LbLogClose(&error_log);
    error_log_initialised = false;
    return result;
}

/******************************************************************************/
