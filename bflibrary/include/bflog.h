/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bflog.h
 *     Header file for glog.cpp, gerrorlg.cpp.
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
#ifndef BFLIBRARY_GLOG_H_
#define BFLIBRARY_GLOG_H_

#include <stdio.h>
#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

enum TbLogFlags {
    LbLog_Overwrite    = 0x0001,
    LbLog_Append       = 0x0002,
    LbLog_Create       = 0x0004,
    LbLog_ContinueLast = 0x0008,
    LbLog_DateInHeader = 0x0010,
    LbLog_TimeInHeader = 0x0020,
    LbLog_DateInLines  = 0x0040,
    LbLog_TimeInLines  = 0x0080,
    LbLog_LoopedFile   = 0x0100,
};

#define LOG_PREFIX_LEN 32

#pragma pack(1)

struct TbLog {
    char Filename[FILENAME_MAX];
    char Prefix[LOG_PREFIX_LEN];
    ulong Flags;
    TbBool Initialised;
    TbBool Created;
    TbBool Suspended;
    ulong Position;
};

#if 0 // original bullfrog structure
struct TbLog { // sizeof=194
    char Filename[144]; // offset=0
    char Prefix[32]; // offset=145
    ulong Flags; // offset=178
    DwBool Initialised; // offset=182
    DwBool Created; // offset=186
    DwBool Suspended; // offset=190
};
#endif

#pragma pack()

/**
 * Log message at error severity level.
 */
int LbErrorLog(const char *format, ...);

/**
 * Log message at warning severity level.
 */
int LbWarnLog(const char *format, ...);

/**
 * Log message at information/sync severity level.
 */
int LbSyncLog(const char *format, ...);

/**
 * Log message at debug severity level.
 */
int LbDbgLog(const char *format, ...);


TbResult LbLog(struct TbLog *log, const char *fmt_str, va_list arg);
TbResult LbLogSetPrefix(struct TbLog *log, const char *prefix);
TbResult LbLogSetPrefixFmt(struct TbLog *log, const char *format, ...);
TbResult LbLogSetup(struct TbLog *log, const char *filename, ulong flags);
TbResult LbLogClose(struct TbLog *log);

int LbLogDelete();
int LbLogSuspend();
int LbLogRestart();

/**
 * Set up logging subsystem and begin writing program execution log at given location.
 */
TbResult LbErrorLogSetup(const char *directory, const char *filename, ubyte flag);

/**
 * Close log file and finalize the logging subsystem.
 */
TbResult LbErrorLogReset(void);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_GLOG_H_
/******************************************************************************/
