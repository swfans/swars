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
TbBool error_log_initialised = false;
struct TbLog error_log;
/******************************************************************************/
TbResult LbLog(struct TbLog *log, const char *fmt_str, va_list arg);
/******************************************************************************/

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

int LbLogSuspend_UNUSED()
{
// code at 0001:000d20e4
}

int LbLogRestart_UNUSED()
{
// code at 0001:000d2118
}

/******************************************************************************/
