/******************************************************************************/
// Bullfrog Engine Smacker Playback Library - for use to remake classic games
// like Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smack2ail.c
 *     Interface for bounding the Smacer library to AIL interface.
 * @par Purpose:
 *     Allows to easily play audio paths from SMK videos through AIL interface.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2011 - 13 Jun 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "smack2ail.h"

#include <stddef.h>
#include <time.h>
#include "bftime.h"
/******************************************************************************/
extern void *SmackMSSDigDriver;
extern uint32_t MSSSpeed;
extern uint32_t MSSTimerPeriod;

extern uint32_t timeradjust;

extern uint8_t RADAPI (*LowSoundOpenAddr)(uint8_t, SmackSndTrk *);
extern void RADAPI (*LowSoundCloseAddr)(SmackSndTrk *);
extern uint32_t RADAPI (*LowSoundPlayedAddr)(SmackSndTrk *);
extern void RADAPI (*LowSoundPurgeAddr)(SmackSndTrk *);
extern void RADAPI (*LowSoundOffAddr)(SmackSndTrk *);
extern void RADAPI (*SmackTimerSetupAddr)(void);
extern uint32_t RADAPI (*SmackTimerReadAddr)(void);
extern void RADAPI (*SmackTimerDoneAddr)(void);
extern void RADAPI (*LowSoundCheckAddr)(void);
extern void RADAPI (*LowSoundOnAddr)(void);
extern void RADAPI (*LowSoundVolPanAddr)(uint32_t, uint32_t, SmackSndTrk *);

/******************************************************************************/
uint8_t RADAPI MSSLOWSOUNDOPEN(uint8_t flags, SmackSndTrk *sstrk)
{
    uint8_t ret;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_MSSLOWSOUNDOPEN\n"
        : "=r" (ret) : "g" (flags), "g" (sstrk));
    return ret;
}

void RADAPI MSSLOWSOUNDCLOSE(SmackSndTrk *sstrk)
{
    asm volatile (
      "push %0\n"
      "call ASM_MSSLOWSOUNDCLOSE\n"
        :  : "g" (sstrk));
    return;
}

uint32_t RADAPI MSSLOWSOUNDPLAYED(SmackSndTrk *sstrk)
{
    uint32_t ret;
    asm volatile (
      "push %1\n"
      "call ASM_MSSLOWSOUNDPLAYED\n"
        : "=r" (ret) : "g" (sstrk));
    return ret;
}

void RADAPI MSSLOWSOUNDPURGE(SmackSndTrk *sstrk)
{
    asm volatile (
      "push %0\n"
      "call ASM_MSSLOWSOUNDPURGE\n"
        :  : "g" (sstrk));
    return;
}

void RADAPI MSSSMACKTIMERSETUP(void)
{
    asm volatile (
      "call ASM_MSSSMACKTIMERSETUP\n"
        :  : );
    return;
}

void RADAPI MSSSMACKTIMERDONE(void)
{
    asm volatile (
      "call ASM_MSSSMACKTIMERDONE\n"
        :  : );
    return;
}

void RADAPI MSSLOWSOUNDCHECK(void)
{
    asm volatile (
      "call ASM_MSSLOWSOUNDCHECK\n"
        :  : );
    return;
}

void RADAPI MSSLOWSOUNDVOLPAN(uint32_t pan, uint32_t volume, SmackSndTrk *sstrk)
{
    asm volatile (
      "push %2\n"
      "push %1\n"
      "push %0\n"
      "call ASM_MSSLOWSOUNDVOLPAN\n"
        :  : "g" (pan), "g" (volume), "g" (sstrk));
    return;
}

/** Default timer routine; returns a timer value, in miliseconds.
 */
uint32_t RADAPI DEFSMACKTIMERREAD(void)
{
#if defined(DOS)||defined(GO32)
    // Every 82C54 timer tick (18.2 per second) activates INT 08h,
    // which increases this value by 1.
    uint32_t tick = PEEKL(0x46C);
    if (tick < lasttimerread)
        timeradjust += lasttimerread - tick;
    lasttimerread = tick;
    return 2746 * (unsigned long long)(timeradjust + tick) / 50;
#else
    clock_t tick = clock();
    return 1000 * (unsigned long long)tick / CLOCKS_PER_SEC;
#endif
}

/** Timer routine when linked to AIL; returns a timer value, in miliseconds.
 */
uint32_t RADAPI MSSSMACKTIMERREAD(void)
{
    return LbTimerClock();
}

/* define asm-to-c functions to set as callbacks */
uint8_t RADAPI ac_MSSLOWSOUNDOPEN(uint8_t flags, SmackSndTrk *sstrk);
void RADAPI ac_MSSLOWSOUNDCLOSE(SmackSndTrk *sstrk);
uint32_t RADAPI ac_MSSLOWSOUNDPLAYED(SmackSndTrk *sstrk);
void RADAPI ac_MSSLOWSOUNDPURGE(SmackSndTrk *sstrk);
void RADAPI ac_MSSSMACKTIMERSETUP(void);
void RADAPI ac_MSSSMACKTIMERDONE(void);
void RADAPI ac_MSSLOWSOUNDCHECK(void);
void RADAPI ac_MSSLOWSOUNDVOLPAN(uint32_t pan, uint32_t volume, SmackSndTrk *sstrk);
uint32_t RADAPI ac_DEFSMACKTIMERREAD(void);
uint32_t RADAPI ac_MSSSMACKTIMERREAD(void);


uint8_t RADAPI SMACKSOUNDUSEMSS(uint32_t speed, void *digdrv)
{
#if 1
    uint8_t ret;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_SMACKSOUNDUSEMSS\n"
        : "=r" (ret) : "g" (speed), "g" (digdrv));
    return ret;
#endif
    if (SmackTimerReadAddr)
        return 0;
    SmackMSSDigDriver = digdrv;
    if (speed < 200)
        speed = 200;
    MSSSpeed = speed;
    LowSoundOpenAddr = MSSLOWSOUNDOPEN;
    LowSoundCloseAddr = MSSLOWSOUNDCLOSE;
    LowSoundPlayedAddr = MSSLOWSOUNDPLAYED;
    LowSoundPurgeAddr = MSSLOWSOUNDPURGE;
    LowSoundOffAddr = MSSLOWSOUNDPURGE;
    SmackTimerSetupAddr = MSSSMACKTIMERSETUP;
    SmackTimerReadAddr = MSSSMACKTIMERREAD;
    SmackTimerDoneAddr = MSSSMACKTIMERDONE;
    MSSTimerPeriod = 1193181 / speed;
    LowSoundCheckAddr = MSSLOWSOUNDCHECK;
    LowSoundOnAddr = NULL;
    LowSoundVolPanAddr = MSSLOWSOUNDVOLPAN;
    return 1;
}

/******************************************************************************/
