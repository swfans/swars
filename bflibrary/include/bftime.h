/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bftime.h
 *     Header file for stime.cpp.
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
#ifndef BFLIBRARY_STIME_H_
#define BFLIBRARY_STIME_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct TbDate { // sizeof=5
    ubyte Day; // offset=0
    ubyte Month; // offset=1
    ushort Year; // offset=2
    ubyte DayOfWeek; // offset=4
};

typedef struct TbDate TbDate;

struct TbTime { // sizeof=4
    ubyte Hour; // offset=0
    ubyte Minute; // offset=1
    ubyte Second; // offset=2
    ubyte HSecond; // offset=3
};

typedef struct TbTime TbTime;

#pragma pack()

int LbDate();
int LbTime();

TbClockMSec LbTimerClock(void);

/** Sleep until LbTimerClock() returns given value.
 */
TbBool LbSleepUntil(TbClockMSec endtime);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_STIME_H_
/******************************************************************************/
